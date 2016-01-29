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
 ***********************************************************************
 *
 * Synopsis: this file handles the Web client requests and
 *           act upon them.
 *
 * created on 2,Feb,2012 srinivasa attili
 *
 *************************************************************************/

/////////////////////////////////////////////////
// INCLUDE FILES
/////////////////////////////////////////////////
#include <string.h>
#include "xstatus.h"
#include "pthread.h"
#include "mfs_config.h"
#include "lwip/sys.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "sys/process.h"
#include "webserver.h"
#include "platform_fs.h"
#include "vdma_data.h"
#include "board_test_app.h"

/////////////////////////////////////////////////
// CONSTANTS
/////////////////////////////////////////////////
#undef SLIDESHOW			// disable slideshow feature
#define GET_BUFSIZE					4096
#define BUFSIZE						1024
#define MAX_NUM_OF_FILES_TO_COPY	128
#define MAX_BENCHMARK_DATA			10	// Max# benchmark data points to post at one time

/////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////
#ifdef SLIDESHOW
char filter_filename_string[MAX_NUM_OF_FILES_TO_COPY][32];
char read_buffer[MAX_FILE_SIZE];
#endif
unsigned char logbufr[15000];
char* logbuf = (char*)logbufr;
int logenable=1;
sys_sem_t cdma_mutex;					// mutual exclusion for benchmark data POST
sys_sem_t *cdma_mutexp = &cdma_mutex;	// LWIP140
/*
 * Store the benchmark data points and count the number of benchmark
 * data points currently stored.
 */
//double *benchmark_databuf;
//int count_benchmark_data = 0;

#ifdef SLIDESHOW
char *slideshow_header =
		"<html> \n \
		<head> \n \
		<title> Xilinx: XilKernel Demonstration over WebServer </title> \n \
		 <script language=\"javascript\"> \n \
		 <!--  \n \
		 var images = new Array(); \n ";

char slideshow_images[1500];
char *slideshow_images_OK=
		"images[0] = \"images/1.jpg\"; \n \
		images[1] = \"images/2.jpg\"; \n \
		images[2] = \"images/3.jpg\"; \n \
		images[3] = \"images/4.jpg\"; \n \
		images[4] = \"images/5.jpg\"; \n ";

char *slideshow_footer =
		 "var next=0; \n \
		function nextimg() \n \
		 { \n \
			 document.getElementById(\'id1\').src = images[next]; \n \
			 document.getElementById(\'id1\').style.visibility = \'visible\'; \n \
			 next++; \n \
			 if(next > imagecount) next=0; \n \
		 } \n \
		 function previmg() \n \
		 { \n \
			 document.getElementById(\'id1\').src = images[next]; \n \
			 document.getElementById(\'id1\').style.visibility = \'visible\'; \n \
			 next--; \n \
			 if(next < 0) next=imagecount; \n \
		} \n \
		 --> \n \
		 </script> \n \
		</head> \n \
		<body> \n \
		THIS IS SLIDESHOW WEBPAGE  \n \
		<div align=\"left\">   \n \
		<form name=\"slideshow_form\">   \n \
			<button height=\"10\" width=\"10\" name=\"next\" onClick=\"nextimg()\"> Next Image </button>  \n \
			<button height=\"10\" width=\"10\" name=\"next\" onClick=\"previmg()\"> Prev Image </button>  </br> \n \
			<img width=\"300\" height=\"300\" id=\"id1\" src=\"images/k7_vdemo_minimum.JPG\" style=\"visibility:hidden\"/>   \n \
		</form>  \n \
		</body>  \n \
		</html>  \n ";
#endif	// SLIDESHOW

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
#if 0
static void *
safe_malloc (size_t size)
{
    void *p;

    xmk_enter_kernel();
    p = malloc(size);
    xmk_leave_kernel();

    return p;
}
#endif

/*
 * Provide a thread-safe memory allocation
 */
#if 0
static void
safe_free (void *ptr)
{
    xmk_enter_kernel();
    free(ptr);
    xmk_leave_kernel();
}
#endif

/*
 * Allocate the mutexes used handling HTTP requests.
 * Implemented with the LWIP sys_sem facilities.
 */
void
init_http_mutexes()
{
    //cdma_mutex = sys_sem_new(1);
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
        xil_printf("http header = %s\n\r", buf);
        return -1;
    }

    lwip_write(sd, notfound_header, strlen(notfound_header));
    lwip_write(sd, req, rlen);
    lwip_write(sd, notfound_footer, strlen(notfound_footer));

    return 0;
}

#ifdef SLIDESHOW
int
do_slideshow(int sd, char *req, int rlen)
{
    int len, hlen;
    char buf[BUFSIZE];

    //len = strlen(slideshow_header) + strlen(slideshow_footer) + rlen;
    len = strlen(slideshow_header) + strlen(slideshow_images) + strlen(slideshow_footer) + rlen;

    hlen = generate_http_header(buf, "html", len);
    if (lwip_write(sd, buf, hlen) != hlen) {
        xil_printf("error writing http header to socket\n\r");
        xil_printf("http header = %s\n\r", buf);
        return -1;
    }

    lwip_write(sd, slideshow_header, strlen(slideshow_header));
    lwip_write(sd, slideshow_images, strlen(slideshow_images));
    //lwip_write(sd, req, rlen);
    lwip_write(sd, slideshow_footer, strlen(slideshow_footer));
    //lwip_write(sd, req, rlen);

    return 0;
}
#endif	// SLIDESHOW

/*
 * Copy the complete next HTTP header line from the mondo input string to 'out'.
 * Reads from the socket if needed.
 */
#if 0
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
#endif

/*
 * Fish out an integer field in the header info, such as "Content-Length:"
 */
#if 0
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
#endif

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
#if 0
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
#endif

/*
 * Free header array obtained with http_parse_headers()
 */
#if 0
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
#endif

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

	for(i=0; req[i] != '\0'; i++)
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

/*
 * Display the benchmark values
 */
static int
http_post_cdma (int sd, char *req, int rlen)
{
    int  len, hlen;
    char buf[BUFSIZE];

    sys_arch_sem_wait(cdma_mutexp, 0);

	get_perf_results(logbuf);
	main_sysmonIntr2(buf, 0);
	strcat(logbuf, buf);
	//xil_printf("%s\n", logbuf);

	len = strlen(logbuf);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, logbuf, strlen(logbuf));

    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutexp);	// LWIP140

    return 0;
}

static int
http_post_vdemo (int sd, char *req, int rlen)
{
	int status=0;
    int len, hlen;
    char buf[BUFSIZE];
    char vsel, ssel, psel, lsel;
    char flist[5000];

    /* Lock mutex */
    sys_arch_sem_wait(cdma_mutexp, 0);

    //http_headers = http_parse_headers(sd, req, &rlen);
    //if (http_headers) {
    //    datalen = get_http_header_int_val(http_headers, "Content-Length:");
    //    http_free_headers(http_headers);
    //} else {
    //    datalen = 0;
    //}

    lsel = get_param(req, 0);	// live selection
    vsel = get_param(req, 1);	// video stream selection
    ssel = get_param(req, 2);	// scale selection
    psel = get_param(req, 3);	// pattern selection

    xil_printf("===>>> http_post_vdemo: lsel=0x%x, vsel=0x%x, ssel=0x%x, psel=0x%x <<<===\r\n",
    		lsel, vsel, ssel, psel);

    if(ssel == 0x20)
    {
		#ifdef SLIDESHOW
    	// SLIDE SHOW (retrieve JPGs stored on SD card, and display them in Webclient window)
    	status = get_SDFS_imageList(filter_filename_string, MAX_NUM_OF_FILES_TO_COPY, 32);
    	printf("filtered files: %d\n", filter_file_counter);
    	memset(flist, 0, sizeof(flist));
    	memset(slideshow_images, 0, sizeof(slideshow_images));

		sprintf(buf, "var imagecount = %d;\n", filter_file_counter);
		strcat(slideshow_images, buf);
    	for(i=0; i<filter_file_counter; i++)
    	{
    		sprintf(buf, "images[%d] = \"sdfs\\\\%s\";\n", i, filter_filename_string[i]);
    		//sprintf(buf, "images[%d] = \"images/%d.jpg\";\n", i, i);
    		strcat(slideshow_images, buf);
    	}

		printf("=====\n%s\n=====\n", slideshow_images);
		//do_slideshow(sd, req, rlen);
    	//printf("files=%d, filelist: %s\n", filter_file_counter, flist);

		#endif	// SLIDESHOW
    }

    // NORMAL SHOW (display Live/Internal Video on the monitor)
    else
    {
    	status = hello_vdemo(lsel, vsel, ssel, psel);
		if(status==0)
			sprintf(flist, "0");
		else
		{
			sprintf(flist, "%d", status);
			//printf("====\n %s \n====\n", flist);
		}
    }
	len = strlen(flist);
	hlen = generate_http_header(buf, "svg", len);
 	lwip_write(sd, buf, hlen);
    lwip_write(sd, flist, strlen(flist));

    //xil_printf("response sent to web client: %s \r\n", string);
    len = 0;
    hlen = 0;

    /* unlock mutex */
    sys_sem_signal(cdma_mutexp);

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

	#ifdef SLIDESHOW
    int w;
    char *sd_filename;
    int status;
	#endif

    /* determine file name */
    extract_file_name(filename, req, rlen, MAX_FILENAME);

    //xil_printf("========>>>>>>>> Filename: %s  <<<<<<<<======== \r\n", filename);

    /****************************************************************/
    /********************   GET FILE FROM SDFS   ********************/
    /****************************************************************/
	#ifdef SLIDESHOW
    if( strncmp(filename, "sdfs", 4) == 0)
    {
    	// get the file from SDFS
    	sd_filename = &filename[5];		// skip the volume part: "sdfs/"
        fext = get_file_extension(filename);

    	mfs_lock();
        status = sd_file_read(sd_filename, read_buffer);
        if(status == -1)
        {
            xil_printf("unable to read requested file %s from SD, returning 404\n\r", filename);
            do_404(sd, req, rlen);
            return -1;
        }
        fsize = status;
        mfs_unlock();

        /* write the http headers */
        hlen = generate_http_header(buf, fext, fsize);
        if (lwip_write(sd, buf, hlen) != hlen) {
            xil_printf("error writing http header to socket\n\r");
            xil_printf("http header = %s\n\r", buf);
            return -1;
        }

        /* now write the file */
		if ((w = lwip_write(sd, read_buffer, fsize)) != fsize)
		{
			xil_printf("error writing file (%s) to socket, remaining unwritten bytes = %d\n\r",
				 filename, fsize - n);
		}

        return 0;
    }

    if(strcmp(filename,"slideshow.html") ==0)
    {
    	do_slideshow(sd, req, rlen);
    	return 0;
	}
	#endif	// SLIDESHOW

    /***************************************************************/
    /********************   GET FILE FROM MFS   ********************/
    /***************************************************************/
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
