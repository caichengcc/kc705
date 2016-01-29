/*
 * Copyright (c) 2007-2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Created on 2,Feb,2012 srinivasa attili;  for Kintex7 Demo
 *
 */

#include <string.h>

#include "xstatus.h"
#include "xil_types.h"

#include "pthread.h"
#include "mfs_config.h"
#include "lwip/sys.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

#include "sys/process.h"

#include "webserver.h"
#include "platform_fs.h"
#include "platform_gpio.h"
#include "cdma_intr.h"
//#include "vdma_data.h"

#include "menu.h"
#include "board_test_app.h"

#define GET_BUFSIZE  4096
#define BUFSIZE      1024

extern int hello_uart2 (int uartbaud, char* log);
extern int hello_iic2 (char* log);
extern int hello_timer2 (char* result);
extern int hello_switch2 (char* log);
extern int hello_bram (void);
extern int main_sysmonIntr2(char *adcData);
extern int hello_rotary2(void);
extern int hello_led2 (int ledpos);
extern int hello_button2(int pos);
extern int hello_sd (void);
extern void msleep(int delay);

unsigned char logbufr[15000];
char* logbuf = (char*)logbufr;
int logenable=1;

/*
 * Maximum number of benchmark data points to post at one time
 */
#define MAX_BENCHMARK_DATA 10

/*
 * Provide mutual exclusion for CDMA benchmark data POST
 */
sys_sem_t cdma_mutex;
sys_sem_t *cdma_mutexp = &cdma_mutex;	// LWIP140

/*
 * Store the benchmark data points and count the number of benchmark
 * data points currently stored.
 */
//double *benchmark_databuf;
//int count_benchmark_data = 0;


char *notfound_header = "<html> \
	<head> \
		<title>404</title> \
  		<style type=\"text/css\"> \
		div#request {background: #eeeeee} \
		</style> \
	</head> \
	<body> \
	<h1>404 Page Not Found</h1> \
	<div id=\"request\">";

char *notfound_footer = "</div> \
	</body> \
	</html>";

char *redirect_home = "<html> \
         <head> \
           <meta HTTP-EQUIV=\"REFRESH\" content=\"0; url=index.html\"> \
         </head> \
         </html> ";

char *js_header = "<html> \
	<head> \
		<title>LEDs</title> \
  		<style type=\"text/css\"> \
		div#request {background: #eeeeee} \
		</style> \
	</head> \
	<body> \
	<h1>LEDs Value</h1> \
	<div id=\"request\"> \
	<script type=\"text/javascript\"> \
	var switchval; \
	switchval = \"";

char *js_footer = "\" \
	document.write(switchval); \
	</script> \
	</div> \
	</body> \
	</html>";

/*
 * Provide a thread-safe memory allocation
 * NOTE: consider LWIP mem_malloc(), mem_free()
 */
static void *
safe_malloc (size_t size)
{
    void *p;

    xmk_enter_kernel();
    p = malloc(size);
    xmk_leave_kernel();

    return p;
}

/*
 * Provide a thread-safe memory allocation
 */
static void
safe_free (void *ptr)
{
    xmk_enter_kernel();
    free(ptr);
    xmk_leave_kernel();
}

/*
 * Allocate the mutexes used handling HTTP requests.
 * Implemented with the LWIP sys_sem facilities.
 */
void
init_http_mutexes()
{
    //cdma_mutex = sys_sem_new(1);	// LWIP130
    sys_sem_new(cdma_mutexp, 1);	// LWIP140
}

/* dynamically generate 404 response:
 *	this inserts the original request string in betwween the notfound_header & footer strings
 */
int
do_404(int sd, char *req, int rlen)
{
    int len, hlen;
    char buf[BUFSIZE];

    len = strlen(notfound_header) + strlen(notfound_footer) + rlen;

    hlen = generate_http_header(buf, "html", len);
    if (lwip_write(sd, buf, hlen) != hlen) {
        xil_printf("error writing http header to socket\n\r");
        xil_printf("HTTP header = %s\n\r", buf);
        return -1;
    }

    lwip_write(sd, notfound_header, strlen(notfound_header));
    lwip_write(sd, req, rlen);
    lwip_write(sd, notfound_footer, strlen(notfound_footer));

    return 0;
}

/*
 * Copy the complete next HTTP header line from the mondo input string to 'out'.
 * Reads from the socket if needed.
 */
static int
get_next_line (int sd, char *out, char *in, int *inindexp, int *inbuflenp)
{
    int dsti;

    if (*inbuflenp - *inindexp < 2) {
        xil_printf("%s: remaining buflen %d - %d\n\r", __FUNCTION__,
            *inbuflenp, *inindexp);
    }

    /* End of headers? */
    if ((in[*inindexp] == '\r') &&
        (in[*inindexp + 1] == '\n')) {
       return 0;
    }

    dsti = 0;
    while (in[*inindexp] && (in[*inindexp] != '\n')) {
        /* Copy input buf to output buf */
        out[dsti] = in[*inindexp];
        (*inindexp)++;

        /* Don't overflow the output buffer */
        if (dsti < BUFSIZE - 2) {
            dsti++;
        }

        /* Refill the input buffer */
        if ((*inbuflenp - *inindexp) <= 0) {
            *inbuflenp = read(sd, in, RECV_BUF_SIZE);
            *inindexp = 0;
            if (*inbuflenp <= 0) {
                xil_printf("%s: Insufficient socket data.\n\r", __FUNCTION__);
                return 0;
            }
        }
    }
    if (dsti >= BUFSIZE - 2) {
        xil_printf("%s: Header larger than BUFSIZE (truncated).\n\r",
            __FUNCTION__);
    }

    (*inindexp)++;
    out[dsti] = 0;

    return (dsti);
}

/*
 * Fish out an integer field in the header info, such as "Content-Length:"
 */
static int
get_http_header_int_val (char **http_headers, const char *which)
{
    int linelen;
    const char *header;
    int i;

    if (http_headers == 0) {
        xil_printf("%s: NULL http_headers\n\r", __FUNCTION__);
        return 0;
    }

    i = 0;
    while (http_headers[i]) {
        header = http_headers[i];
        if (strncmp(which, header, strlen(which)) == 0) {
            linelen = atoi(&header[strlen(which)]);
            return linelen;
        }
        i++;
    }

    return 0;
}

/*
 * Fetch data POSTed to the server; place it in *databuf provided by caller.
 */
#if 0
static int
receive_post_data (int sd, char *in, int rlen,
                   char *databuf, int datalen, int skip_http_hdr)
{
    int i, read_len, http_hdr, hdr_bytes;

    /* Skip the request header to get to any data received in the
     * first buffer. It terminates with \n\r\n. Any additional headers
     * are counted against the Content-Length, and the header size should
     * be subtracted from the amount of client data to read.
     */
    i = 0;
    http_hdr = 0;
    hdr_bytes = 0;
    while (http_hdr < skip_http_hdr) {
        /* End of this header ? */
        while (!((in[i] == '\n') && (in[i + 1] == '\r') &&
                 (in[i + 2] == '\n'))) {
            if ((i + 3) > rlen) {
                /* End of buffer data; End of header not encountered yet.
                 * Read more data.
                 */
                i = 0;
                rlen = read(sd, in, RECV_BUF_SIZE);
                if (rlen <= 0) {
                    xil_printf("receive_post_data: insufficient data.\n\r");
                    return 0;
                }
            } else {
                i++;
                hdr_bytes++;
            }
        }
        hdr_bytes += 3;
        i += 3;

        if (http_hdr > 0) {
            /* Additional headers count against the Content-Length specified
             * in the first header.
             */
            datalen -= hdr_bytes;
        }

        /* The remainder of this buffer belongs to the next header */
        http_hdr++;
        hdr_bytes = rlen - i;
    }

    /* Copy any data bytes present in the current buffer */
    rlen -= i;
    memcpy(databuf, &in[i], rlen);
    databuf += rlen;
    datalen -= rlen;

    /* read the remainder */
    while (datalen > 0) {
        read_len = read(sd, databuf, datalen);
        if (read_len < 0) {
            xil_printf("read: %d (datalen %d)\n\r", read_len, datalen);
            return read_len;
        }
        databuf += read_len;
        datalen -= read_len;
    }
    *databuf = 0;

    return 0;
}
#endif

#define MAX_HTTP_HEADERS  30

/*
 * Returns an array of all the HTTP headers.
 */
static char **
http_parse_headers (int sd, char *req, int *rlen)
{
    char *line;
    int linelen;
    int indexp;
    char **http_headers;
    int total_headers;

    http_headers = 0;
    line = safe_malloc(BUFSIZE);
    if (line == 0) {
        xil_printf("Unable to allocate buffer.\n\r");
        return http_headers;
    }
    http_headers = safe_malloc(sizeof(char*) * (MAX_HTTP_HEADERS + 1));
    if (http_headers == 0) {
        xil_printf("Unable to allocate header space.\n\r");
        safe_free(line);
        return http_headers;
    }
    total_headers = 0;

    indexp = 0;
    while (total_headers < MAX_HTTP_HEADERS) {
        /* Fetch a header line */
        linelen = get_next_line(sd, line, req, &indexp, rlen);
        if (linelen) {
            /* Copy this header line into an array */
            http_headers[total_headers] = safe_malloc(linelen + 4);
            if (http_headers[total_headers]) {
                strncpy(http_headers[total_headers], line, linelen + 2);
            } else {
                xil_printf("Unable to allocate HTTP header %d buffer(%d)\n\r",
                           total_headers, linelen);
                safe_free(line);
                return 0;
            }
            total_headers++;
        } else {
            break;
        }
    }
    if (total_headers == MAX_HTTP_HEADERS) {
        xil_printf("%s: total_headers = MAX_HTTP_HEADERS\n\r", __FUNCTION__);
    }

    safe_free(line);
    http_headers[total_headers] = 0;
    return(http_headers);
}

/*
 * Free header array obtained with http_parse_headers()
 */
static void
http_free_headers (char **http_headers)
{
    int i;

    if (http_headers == 0) {
        xil_printf("%s: NULL http_headers\n\r", __FUNCTION__);
        return;
    }

    i = 0;
    while (http_headers[i]) {
        safe_free(http_headers[i]);
        http_headers[i] = 0;
        i++;
    }
    safe_free(http_headers);
}

/*
 * Display header array obtained with http_parse_headers()
 */
static void
print_http_headers (char **http_headers)
{
    int i;

    return;	// DUMMY - for DEBUG

    if (http_headers == 0) {
        return;
    }

    xil_printf("HTTP HEADERS:\n\r");
    xil_printf("========================================\n\r");
    i = 0;
    while (http_headers[i]) {
        xil_printf("%d: %s\n\r", i, http_headers[i]);
        i++;
    }
    xil_printf("========================================\n\r");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* skip past 'POST /' */
/* then check for cmd/xxxx */
int is_cmd_cdma(char *buf)  { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "cdmaxhr",   7)); }
int is_cmd_uart(char *buf)  { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "uartxhr",   7)); }
int is_cmd_temac(char *buf) { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "temacxhr",  8)); }
int is_cmd_iic(char *buf)   { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "iicxhr",    6)); }
int is_cmd_timer(char *buf) { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "timerxhr",  8)); }
int is_cmd_intrc(char *buf) { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "intrcxhr",  8)); }
int is_cmd_ddr3c(char *buf) { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "ddr3cxhr",  8)); }
int is_cmd_nand(char *buf)  { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "nandxhr",   7)); }
int is_cmd_led(char *buf)   { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "ledxhr",    6)); }
int is_cmd_dipsw(char *buf) { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "dipswxhr",  8)); }
int is_cmd_bram(char *buf)  { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "bramxhr",   7)); }
int is_cmd_pushb(char *buf) { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "pushbxhr",  8)); }
int is_cmd_lcd(char *buf)   { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "lcdxhr",    6)); }
int is_cmd_rotory(char *buf){ return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "rotoryxhr", 9)); }
int is_cmd_qspi(char *buf)  { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "qspixhr",   7)); }
int is_cmd_sd(char *buf)    { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "sdxhr",     5)); }
int is_cmd_xadc(char *buf)  { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "xadcxhr",   7)); }
int is_cmd_vdemo(char *buf) { return (!strncmp(buf, "cmd", 3) && !strncmp(buf + 4, "vdemoxhr",  8)); }

int get_param(char* req, int p_index)
{
	int i;
	int pos = 0;

	for(i=0; (char)req[i] != (int)NULL; i++)
	{
		if(req[i] == ',')
		{
			pos = atoi( (req+i+1) );
			if(p_index == 0) break;
			p_index--;
		}
	}

	memset(logbuf, 0, sizeof(logbufr) );
	return pos;
}

static int
http_post_uart (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
	//result = strtok( req, ",");		//+15
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos==0)
	{
		status = hello_uart2(UART_BAUDRATE, logbuf);
		if(status==0) strcat(logbuf, "UART test PASSED");
		else          strcat(logbuf, "UART test FAILED");
		strcat(logbuf, "\r\n================\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    printf("http POST: uart data: %s\n\r", logbuf);
	hlen = generate_http_header(buf, "txt", len);
 	len = lwip_write(sd, buf, hlen);
 	printf("lwip_write for header, req=%d, sent=%d\r\n", hlen, len);

 	len = lwip_write(sd, logbuf, strlen(logbuf));
 	printf("lwip_write for buffer, req=%d, sent=%d\r\n", (int)strlen(logbuf), len);

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}

static int
http_post_iic (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	//print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
    //result = strtok( req+16, ",");	//+16
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos==0)
	{
		status = hello_iic2(logbuf);
		if(status==0) strcat(logbuf, "IIC test PASSED\r\n");
		else          strcat(logbuf, "IIC test FAILED\r\n");
		strcat(logbuf, "===============\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
	printf("http POST: iic data: %s\n\r", logbuf);
    hlen = generate_http_header(buf, "txt", len);
    lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    return 0;
}

static int
http_post_timer (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
    //result = strtok( req+16, ",");	//+16
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos==0)
	{
		status = hello_timer2(logbuf);
		if(status==0) strcat(logbuf, "TIMER test PASSED\r\n");
		else          strcat(logbuf, "TIMER test FAILED\r\n");
		strcat(logbuf, "==============\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    printf("http POST: timer data: %s\n\r", logbuf);
	hlen = generate_http_header(buf, "txt", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

     return 0;
}

static int
http_post_intrc (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
	//sys_arch_sem_wait(cdma_mutex, 0);		// LWIP130
	sys_arch_sem_wait(cdma_mutexp, 0);		// LWIP140

	#if 0
	http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
	if (http_headers) {
		datalen = get_http_header_int_val(http_headers, "Content-Length:");
		http_free_headers(http_headers);
	} else {
		datalen = 0;
	}
	#endif

    pos = get_param(req, 0);
    //result = strtok( req+16, ",");	//+16
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos==0)
	{
		status = hello_iic2(logbuf);
		if(status==0) strcat(logbuf, "INTC test PASSED\r\n");
		else          strcat(logbuf, "INTC test FAILED\r\n");
		strcat(logbuf, "===============\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
	printf("http POST: iic data: %s\n\r", logbuf);
    hlen = generate_http_header(buf, "txt", len);
    lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}
/*
 * Display the switch values
 */
static int
http_post_dipsw (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	//int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;
	//unsigned int s;

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
    //print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
    //result = strtok( req, ",");		//+15
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos==0)
	{
		hello_switch2(logbuf);
		strcat(logbuf, "================\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    hlen = generate_http_header(buf, "txt", len);
    lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

#if 0
 	len = generate_http_header(buf, "txt", n_switches);
    p = buf + len;
    for (n = 0; n < n_switches; n++) {
        *p++ = '0' + (s & 0x1);
        s >>= 1;
    }
    len += n_switches;

    lwip_write(sd, buf, len);
#endif

    len = 0;

    return 0;
}

static int
http_post_bram (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
    //result = strtok( req+15, ",");
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos==0)
	{
		status = hello_bram();
		if(status==0) strcat(logbuf, "BRAM test PASSED");
		else          strcat(logbuf, "BRAM test FAILED");
		strcat(logbuf, "\r\n================\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    hlen = generate_http_header(buf, "txt", len);
    lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}

static int
http_post_lcd (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
    //result = strtok( req+15, ",");
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos == 0)
	{
		status = lcd_simple();
		if(status==0) strcat(logbuf, "LCD test PASSED");
		else          strcat(logbuf, "LCD test FAILED");
		strcat(logbuf, "\r\n================\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    printf("http POST: led data: %s\n\r", logbuf);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}

static int
http_post_ddr3c (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
	//result = strtok( req+15, ",");
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos==0)
	{
		status = hello_mem();
		if(status==0) strcat(logbuf, "MEM test PASSED");
		else          strcat(logbuf, "MEM test FAILED");
		strcat(logbuf,"\r\n==========\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    hlen = generate_http_header(buf, "txt", len);
    lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}

static int
http_post_sd(int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
	//result = strtok( req+15, ",");
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos==0)
	{
		status = hello_sd();
		if(status==0) strcat(logbuf, "SD test PASSED");
		else          strcat(logbuf, "SD test FAILED");
		strcat(logbuf,"\r\n==========\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    printf("http POST: sd data: %s\n\r", logbuf);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}
static int
http_post_xadc (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	//int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
    //result = strtok( req+15, ",");
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos < 10)
	{
		main_sysmonIntr2(logbuf);
		//strcat(logbuf,"\r\n==========\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    printf("http POST: xadc data: %s\n\r", logbuf);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}
static int
http_post_rotory(int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
    //result = strtok( req+15, ",");
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos < 10)
	{
		if(pos == 0)
		{
			strcpy(logbuf, "test for 10 steps (Clockwise /Anti-Clockwise) \n");
			strcat(logbuf, "Turn the rotary Clockwise /Anti-Clockwise \n\n");
		}
		else
		{
			status = hello_rotary2();
			if(status==1) strcat(logbuf, "rotary turned Anti-Clockwise");
			else          strcat(logbuf, "rotary turned Clockwise");
		}
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    printf("http POST: rotary data: %s\n\r", logbuf);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}
static int
http_post_nand (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
	//result = strtok( req+15, ",");
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos==0)
	{
		status = hello_flash();
		if(status==0) strcat(logbuf, "NAND flash test PASSED\r\n");
		else          strcat(logbuf, "NAND flash test FAILED\r\n");
		strcat(logbuf, "============\r\n");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    printf("http POST: nand data: %s\n\r", logbuf);
	hlen = generate_http_header(buf, "txt", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    return 0;
}
/*
 * Toggle LEDs
 */
static int
http_post_led (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	//int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

	#if 0
	http_headers = http_parse_headers(sd, req, &rlen);
	//print_http_headers(http_headers);
	if (http_headers) {
		datalen = get_http_header_int_val(http_headers, "Content-Length:");
		http_free_headers(http_headers);
	} else {
		datalen = 0;
	}
	#endif

	pos = get_param(req, 0);
	//result = strtok( req, ",");		//+16
	//result = strtok( NULL, ",");
	//ledpos = atoi( result );

	if(pos <= 7)
	{
		hello_led2(pos);
		sprintf(logbuf, "LED %d", pos);
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    hlen = generate_http_header(buf, "txt", len);
    lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    return 0;
}
static int
http_post_pushb (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	//int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

	//strcpy(logbuf, "PRESS THE BUTTONS IN THE FOLLOWING ORDER\r\n \r\n");
	//strcat(logbuf, "Press west button and see if west LED glows\r\n");
	//strcat(logbuf, "Press south button and see if south LED glows\r\n");
	//strcat(logbuf, "Press east button and see if east LED glows\r\n");
	//strcat(logbuf, "Press north button and see if north LED glows\r\n");
	//strcat(logbuf, "Press center button and see if center LED glows\r\n");
	//strcat(logbuf, "Press any button to turn off all LEDs\r\n\r\n");
	//strcat(logbuf, "================================\r\n");

    pos = get_param(req, 0);
    //result = strtok( req+15, ",");
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	switch(pos)
	{
		case 0:
			strcpy(logbuf, "Press West button");	// message for West button
			break;
		case 1:
			hello_button2((pos-1));		// wait for West button
			strcpy(logbuf, "Press South button");	// message for South button
			break;
		case 2:
			hello_button2((pos-1));		// wait for South button (button broke, cannot test)
			strcpy(logbuf, "Press East button");	// message for East button
			break;
		case 3:
			hello_button2((pos-1));		// wait for East button
			strcpy(logbuf, "Press North button");	// message for North button
			break;
		case 4:
			hello_button2((pos-1));		// wait for North button
			strcpy(logbuf, "Press Center button");	// message for Center button
			break;
		case 5:
			hello_button2((pos-1));		// wait for Center button
			strcpy(logbuf, "Press Any button");		// message for ANY button
			break;
		case 6:
			hello_button2((pos-1));		// wait for Any button
			strcpy(logbuf, "DONE");					// end of test
			break;
	}

	len = strlen(logbuf);
	hlen = generate_http_header(buf, "txt", len);
	lwip_write(sd, buf, hlen);
	lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}

/*
 * Display the benchmark values
 */
static int
http_post_cdma (int sd, char *req, int rlen)
{
    //int datalen;
    char **http_headers;
	//char *result=NULL;
	//int status;
    int  len, hlen;
    char buf[BUFSIZE];
	//int  pos;

    //xil_printf("enter http_post_cdma\n\r");
    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        //datalen = 0;
    }

    //get_perf_results(perf_results);
    //xil_printf("write: %d  read: %d \r\n", perf_results[0], perf_results[1]);
    //sprintf(string, "%d,%d", perf_results[0], perf_results[1]);
    //sprintf(string, "%d, %d", perf_results[j++], perf_results[j++]);
    //for(i=0; i<MAX_VDMA_INSTANCE; i++){
   // 	sprintf(temp, "%d, ", perf_results[j++]);
   // 	strcat(string, temp);
    //}

	len = strlen(logbuf);
	//safe_free(resultbuf);
    printf("http POST: benchmark data: %s\n\r", logbuf);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}

#if 0
static int
http_post_temac (int sd, char *req, int rlen)
{
	int status;
    int    datalen;
    int		len, hlen;
    char   buf[BUFSIZE];
    char **http_headers;
    char string[100];

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }

	status = hello_temac();
	if(status == XST_FAILURE) {
		xil_printf("Error getting benchmarking data \n\r");
	}

	if(status==0) sprintf(string, "TEMAC test PASSED");
	else          sprintf(string, "TEMAC test FAILED");

	len = strlen(string);
    printf("http POST: temac data: %s\n\r", string);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, string, strlen(string));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}
#endif

static int
http_post_qspi (int sd, char *req, int rlen)
{
    //int datalen;
    //char **http_headers;
	//char *result=NULL;
	//int status;
    int  len, hlen;
    char buf[BUFSIZE];
	int  pos;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

	#if 0
    http_headers = http_parse_headers(sd, req, &rlen);
	print_http_headers(http_headers);
    if (http_headers) {
        datalen = get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        datalen = 0;
    }
	#endif

    pos = get_param(req, 0);
    //result = strtok( req+15, ",");
	//result = strtok( NULL, ",");
	//pos = atoi( result );

	if(pos == 0)
	{
		/*
		status = hello_qspi();
		if(status==0) sprintf(string, "Q-SPI test PASSED");
		else          sprintf(string, "Q-SPI test FAILED");
		*/
		sprintf(logbuf, "Q-SPI test to be implemented");
	}
	else strcpy(logbuf, "DONE");

	len = strlen(logbuf);
    printf("http POST: qspi data: %s\n\r", logbuf);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}



static int
http_post_vdemo (int sd, char *req, int rlen)
{
	int status=0;
    //int    datalen;
    int    len, hlen;
    char   buf[BUFSIZE];
    char **http_headers;
    char string[100];
    char vsel, ssel, psel;
    //char *result=NULL;

    /* Lock mutex */
    //sys_arch_sem_wait(cdma_mutex, 0);	// LWIP130
    sys_arch_sem_wait(cdma_mutexp, 0);	// LWIP140

    http_headers = http_parse_headers(sd, req, &rlen);
	//print_http_headers(http_headers);
    if (http_headers) {
        get_http_header_int_val(http_headers, "Content-Length:");
        http_free_headers(http_headers);
    } else {
        //datalen = 0;
    }

    vsel = get_param(req, 0);
    ssel = get_param(req, 1);
    psel = get_param(req, 2);
    //result = strtok( req+18, ",");
    //vsel = atoi( result );
    //result = strtok( NULL, ",");
    //ssel = atoi( result );
    //result = strtok( NULL, ",");
    //psel = atoi( result );

    xil_printf("http_post_vdemo: vsel=0x%x, ssel=0x%x, psel=0x%x\n", vsel, ssel, psel);

    //status = hello_vdemo(vsel, ssel, psel);

	if(status==0) sprintf(string, "VDEMO test PASSED");
	else          sprintf(string, "VDEMO test FAILED");

	len = strlen(string);
    printf("http POST: vdemo data: %s\n\r", string);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, string, strlen(string));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    //sys_sem_signal(cdma_mutex);	// LWIP130
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}


/*
 * Handle HTTP POST data from the client.
 */
int
do_http_post(int sd, char *req, int rlen)
{
	/* skip past 'POST /' */
	/* then check for cmd/xxxx */
    if (is_cmd_cdma(req+6))
    {
        http_post_cdma(sd, req, rlen);
    }
    else if (is_cmd_uart(req+6))
    {
        http_post_uart(sd, req, rlen);
    }
    else if (is_cmd_temac(req+6))
    {
        //http_post_temac(sd, req, rlen);
    }
    else if (is_cmd_iic(req+6))
    {
        http_post_iic(sd, req, rlen);
    }
    else if (is_cmd_timer(req+6))
    {
        http_post_timer(sd, req, rlen);
    }
    else if (is_cmd_intrc(req+6))
    {
        http_post_intrc(sd, req, rlen);
    }
    else if (is_cmd_ddr3c(req+6))
    {
        http_post_ddr3c(sd, req, rlen);
    }
    else if (is_cmd_nand(req+6))
    {
        http_post_nand(sd, req, rlen);
    }
    else if (is_cmd_led(req+6))
    {
        http_post_led(sd, req, rlen);
    }
    else if (is_cmd_dipsw(req+6))
    {
        http_post_dipsw(sd, req, rlen);
    }
    else if (is_cmd_bram(req+6))
    {
        http_post_bram(sd, req, rlen);
    }
    else if (is_cmd_pushb(req+6))
    {
        http_post_pushb(sd, req, rlen);
    }
    else if (is_cmd_lcd(req+6))
    {
        http_post_lcd(sd, req, rlen);
    }
    else if (is_cmd_rotory(req+6))
    {
        http_post_rotory(sd, req, rlen);
    }
    else if (is_cmd_qspi(req+6))
    {
        http_post_qspi(sd, req, rlen);
    }
    else if (is_cmd_sd(req+6))
    {
        http_post_sd(sd, req, rlen);
    }
    else if (is_cmd_xadc(req+6))
    {
        http_post_xadc(sd, req, rlen);
    }
    else if (is_cmd_vdemo(req+6))
    {
        http_post_vdemo(sd, req, rlen);
    }
    else
    {
        xil_printf("http POST: %s unsupported command\n\r", req);
    }

    return 0;
}

/* respond for a file GET request */
int
do_http_get(int sd, char *req, int rlen)
{
    char filename[MAX_FILENAME];
    char buf[GET_BUFSIZE];
    int fsize, hlen, n;
    int fd;
    char *fext;

    /* determine file name */
    extract_file_name(filename, req, rlen, MAX_FILENAME);

    /* respond with 404 if not present */
    mfs_lock();
    n = mfs_exists_file(filename);
    mfs_unlock();
    if (n == 0) {
        xil_printf("requested file %s not found, returning 404\n\r", filename);
        do_404(sd, req, rlen);
        return -1;
    }

    /* respond with correct file */

    /* debug statement on UART */
    // xil_printf("http GET: %s\n\r", filename);

    /* get a pointer to file extension */
    fext = get_file_extension(filename);

    mfs_lock();
    fd = mfs_file_open(filename, MFS_MODE_READ);

    /* obtain file size,
     * note that lseek with offset 0, MFS_SEEK_END does not move file pointer */
    fsize = mfs_file_lseek(fd, 0, MFS_SEEK_END);
    mfs_unlock();

    /* write the http headers */
    hlen = generate_http_header(buf, fext, fsize);
    msleep(1);
    if (lwip_write(sd, buf, hlen) != hlen) {
        xil_printf("error writing http header to socket\n\r");
        xil_printf("http header = %s\n\r", buf);
        return -1;
    }

    /* now write the file */
    while (fsize) {
        int w;

        mfs_lock();
        n = mfs_file_read(fd, buf, BUFSIZE);
        mfs_unlock();

        if ((w = lwip_write(sd, buf, n)) != n) {
            xil_printf
                ("error writing file (%s) to socket, remaining unwritten bytes = %d\n\r",
                 filename, fsize - n);
            xil_printf
                ("attempted to lwip_write %d bytes, actual bytes written = %d\n\r",
                 n, w);
            break;
        }

        fsize -= n;
    }

    mfs_lock();
    mfs_file_close(fd);
    mfs_unlock();

    return 0;
}

enum http_req_type
{ HTTP_GET, HTTP_POST, HTTP_UNKNOWN };

enum http_req_type
decode_http_request(char *req, int l)
{
    char *get_str = "GET";
    char *post_str = "POST";

    if (!strncmp(req, get_str, strlen(get_str)))
        return HTTP_GET;

    if (!strncmp(req, post_str, strlen(post_str)))
        return HTTP_POST;

    return HTTP_UNKNOWN;
}

/* generate and write out an appropriate response for the http request */
int
generate_response(int sd, char *http_req, int http_req_len)
{
    enum http_req_type request_type =
        decode_http_request(http_req, http_req_len);

    switch (request_type) {
    case HTTP_GET:
        return do_http_get(sd, http_req, http_req_len);
    case HTTP_POST:
        return do_http_post(sd, http_req, http_req_len);
    default:
        return do_404(sd, http_req, http_req_len);
    }
}
