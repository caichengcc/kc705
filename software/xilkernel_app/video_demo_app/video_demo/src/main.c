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
 * Created on 2,Feb,2012 srinivasa attili
 *
 */

#include <stdio.h>
#include <sys/unistd.h>

#include "xmk.h"
#include "xenv_standalone.h"
#include "xparameters.h"

#include "platform_config.h"
#include "platform.h"

#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/init.h"
#include "netif/xadapter.h"
#include "lwip/dhcp.h"
#include <sys/timer.h>

#include "config_apps.h"
#include "mfs_config.h"
#include "board_test_app.h"

sys_sem_t main_mutex;
sys_sem_t *main_mutexp = &main_mutex;	// LWIP140

void
print_ip(char *msg, struct ip_addr *ip)
{
    print(msg);
    xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
            ip4_addr3(ip), ip4_addr4(ip));
}

void
print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw)
{

    print_ip("Board IP: ", ip);
    print_ip("Netmask : ", mask);
    print_ip("Gateway : ", gw);
}

struct netif server_netif;

#undef	WEBSERVER_USES_DHCP

#ifdef WEBSERVER_USES_DHCP
void dhcp_fine_tmr_thread(void *p)
{
	while(1)
	{
		sys_sleep(500);
		dhcp_fine_tmr();
	};
	return;
}
void dhcp_coars_tmr_thread(void *p)
{
	while(1)
	{
		sys_sleep(60000);
		dhcp_coarse_tmr();
	};
	return;
}
#endif

void network_thread(void *p)
{
    struct netif *netif;
    struct ip_addr ipaddr, netmask, gw;
	#ifdef WEBSERVER_USES_DHCP
	int i=0;
	#endif

    /* the mac address of the board. this should be unique per board */
    unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

    netif = &server_netif;

    /* Lock mutex */
    sys_arch_sem_wait(main_mutexp, 0);

#ifndef WEBSERVER_USES_DHCP	// this code is for STATIC IP

    /* initliaze IP addresses to be used */
    IP4_ADDR(&ipaddr,  192, 168,   1, 10);
    IP4_ADDR(&netmask, 255, 255, 255,  0);
    IP4_ADDR(&gw,      192, 168,   1,  1);

    /* print out IP settings of the board */
    print("\n\r\n\r");
    print("-----lwIP Socket Mode Demo Application ------\n\r");
    print_ip_settings(&ipaddr, &netmask, &gw);

    /* print all application headers */
    print_headers();

    /* Add network interface to the netif_list, and set it as default */
    if (!xemac_add(netif, &ipaddr, &netmask, &gw,
        mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
        xil_printf("Error adding N/W interface\n\r");
        return;
    }

#else	// WEBSERVER_USES_DHCP: this code is for DHCP

    IP4_ADDR(&ipaddr,  0, 0, 0, 0);
    IP4_ADDR(&netmask, 0, 0, 0, 0);
    IP4_ADDR(&gw,      0, 0, 0, 0);

    /* Add network interface to the netif_list, and set it as default */
    if (!xemac_add(netif, &ipaddr, &netmask, &gw,
        mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
        xil_printf("Error adding N/W interface\n\r");
        return;
    }

    /* print out IP settings of the board */
    print("\n\r\n\r");
    print("-----lwIP Socket Mode Demo Application (DHCP enable) ------\n\r");
    //print_ip_settings(&ipaddr, &netmask, &gw);

    sys_thread_new("NW_THREAD", dhcp_fine_tmr_thread, NULL, THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
    sys_thread_new("NW_THREAD", dhcp_coars_tmr_thread, NULL, THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);

    dhcp_start(netif);
    do
    {
    	sys_sleep(5000);
    	if(netif->ip_addr.addr != 0)
    		break;

    	i++;
    	if(i == 50)
    	{
    		print("1 minute passed, DHCP has not acquired IP address !! \r\n");
    		break;
    	}
    } while(1);

    xil_printf("DHCP got an IP address to Kintex-7 \r\n");
    print_ip_settings(&ipaddr, &netmask, &gw);

#endif

    netif_set_default(netif);

    /* specify that the network if is up */
    netif_set_up(netif);

    /* start packet receive thread - required for lwIP operation */
    sys_thread_new("xemacif_input_thread",
            (void(*)(void*))xemacif_input_thread, netif,
            THREAD_STACKSIZE,
            DEFAULT_THREAD_PRIO);

    /* now we can start application threads */
    launch_app_threads();

    /* unlock mutex */
    sys_sem_signal(main_mutexp);

    return;
}

/*
 * Initial thread entrypoint
 */
int
main_thread()
{
	/* initialize lwIP before calling sys_thread_new */
    xil_printf("initialize LWIP\n\r");

    lwip_init();
    sys_sem_new(main_mutexp, 1);
    init_http_mutexes();

    /* any thread using lwIP should be created using sys_thread_new */
    xil_printf("create network_thread\n\r");
    sys_thread_new("NW_THREAD", network_thread, NULL,
            THREAD_STACKSIZE,
            DEFAULT_THREAD_PRIO);
    sleep(6);
    /* Lock mutex */
    sys_arch_sem_wait(main_mutexp, 0);

	#if 1
    main_thread_vdemo(NULL);
	#else
	sleep(2);
	sys_thread_new("VDEMO_THREAD", main_thread_vdemo, NULL,
			THREAD_STACKSIZE,
			DEFAULT_THREAD_PRIO);
	sleep(2);
	#endif

	sys_sem_signal(main_mutexp);
	sys_sem_free(main_mutexp);		// destroy the semaphore, it is no more used
    return 0;
}


#define DATA_SIZE   400
#define N_THREADS   4
int input_data[DATA_SIZE];
int init_TimeBaseDetection(void);

int main()
{
	int i;

	xil_printf("main.. \r\n");
	init_platform();
	xil_printf("init_platform done. \r\n");

	/* Assign random data to the input array */
	for (i = 0; i < DATA_SIZE; i++)
		input_data[i] = i + 1;

	// this is related to the Vide Demo.
	// the TimebaseDetection accesses 0x5080xxxx memory area for register read/write operations
	// however, the Xilkernel is crashing, as this memory area is not mapped into app region
	// as a workaround, we are using the TimeBaseDetection before starting xilkernel
	// Need to investigate in xilkernel to map this memory region into app area.
	init_TimeBaseDetection();

	/* Initialize xilkernel */
	xil_printf("xilkernel_init.. \r\n");
	xilkernel_init();

	/* Start the kernel */
	//xil_printf("xilkernel_start.. \r\n");
	xilkernel_start();

	/* Create the master thread */
	xil_printf("static thread.. \r\n");
	//xmk_add_static_thread(main_thread, 0);
	//xmk_add_static_thread(test_thread, 0);
	//xmk_add_static_thread(master_thread, 0);


	/* Never reached */
	xil_printf("cleanup_platform.. \r\n");
	cleanup_platform();

	return 0;

	#if 0
    if (init_platform() < 0) {
        xil_printf("ERROR initializing platform.\n\r");
        return -1;
    }

    /* start the kernel - does not return */
    xil_printf("Starting XILKERNEL.\n\r");
    xilkernel_main();
    return 0;
	#endif
}
