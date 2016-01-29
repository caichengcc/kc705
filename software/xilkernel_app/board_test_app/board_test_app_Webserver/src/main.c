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
 * Created on 2,Feb,2012 srinivasa attili; for Kintex7 Demo
 *
 */

#include <stdio.h>

#include "xmk.h"
#include "xenv_standalone.h"
#include "xparameters.h"

#include "platform_config.h"
#include "platform.h"

#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/init.h"
#include "netif/xadapter.h"

#include "config_apps.h"
#include "mfs_config.h"
#include "app.h"
#include "xil_cache.h"		// for 13.4, 0.87 build

/*
 * Macros to enable/disable caches.
 */
#ifndef ENABLE_ICACHE
#define ENABLE_ICACHE()		Xil_ICacheEnable()
#endif
#ifndef	ENABLE_DCACHE
#define ENABLE_DCACHE()		Xil_DCacheEnable()
#endif
#ifndef	DISABLE_ICACHE
#define DISABLE_ICACHE()	Xil_ICacheDisable()
#endif
#ifndef DISABLE_DCACHE
#define DISABLE_DCACHE()	Xil_DCacheDisable()
#endif

extern void xilkernel_init(void);
extern void xilkernel_start (void);
extern void main_BoardTestApp(void *p);

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

void network_thread(void *p)
{
    struct netif *netif;
    struct ip_addr ipaddr, netmask, gw;

    /* the mac address of the board. this should be unique per board */
    unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

    netif = &server_netif;

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

    init_http_mutexes();

    #if 1	// enable this, if we want to support BOTH uart and webserver simultaneously.

    /* any thread using lwIP should be created using sys_thread_new */
    xil_printf("--------------------------------\n\r");
    xil_printf("**     Thread 1: Web server   **\n\r");
    xil_printf("--------------------------------\n\r");
    xil_printf("create network_thread\n\r");
    sys_thread_new("NW_THREAD", network_thread, NULL,
            THREAD_STACKSIZE,
            DEFAULT_THREAD_PRIO);
	#endif

	#if 0	// enable this, if we want to support BOTH uart and webserver simultaneously.
    sleep(500);
    xil_printf("--------------------------------\n\r");
    xil_printf("** Thread 2: Driver test apps **\n\r");
    xil_printf("--------------------------------\n\r");
    sys_thread_new("MENU_THREAD", main_BoardTestApp, NULL,
            THREAD_STACKSIZE,
            5);	///// REDUCE PRIORITY for
	#endif

    return 0;
}

#define DATA_SIZE   400
#define N_THREADS   4
int input_data[DATA_SIZE];

int test_thread(void *p)
{
    //int choice, exit_flag;

    // for 13.4, 0.87 build
	ENABLE_ICACHE();
	ENABLE_DCACHE();

    print("\n\r********************************************************");
    print("\n\r********************************************************");
    print("\n\r**     Xilinx Kintex-7 FPGA KC705 Evaluation Kit      **");
    print("\n\r********************************************************");
    print("\n\r********************************************************\r\n");

	print("Choose Feature to Test:\r\n");
	print("1: UART Test\r\n");
	print("2: LED Test\r\n");
	print("3: IIC Test\r\n");
	print("4: FLASH Test\r\n");
	print("5: TIMER Test\r\n");
	print("6: ROTARY Test\r\n");
	print("7: SWITCH Test\r\n");
	print("8: SD Test\r\n");
	print("9: LCD Test\r\n");
	print("A: DDR3 External Memory Test\r\n");
	print("B: BRAM Internal Memory Test\r\n");
	print("C: ETHERNET Loopback Test\r\n");
	print("D: BUTTON Test\r\n");
	print("E: XADC Test\r\n");
	print("0: Exit\r\n");
	return 0;
}

int
main()
{
    int i;

    xil_printf("main.. \r\n");
    init_platform();
    xil_printf("init_platform done. \r\n");

    /* Assign random data to the input array */

    for (i = 0; i < DATA_SIZE; i++)
        input_data[i] = i + 1;

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
#endif

    return 0;
}
