/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
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
 * created on 2,Feb,2012 srinivasa attili; modified for Kintex7 Demo
 *
 */

#include "lwip/inet.h"
#include "lwip/ip_addr.h"
#include "lwip/sys.h"

#include "config_apps.h"
#include "webserver.h"

void 
print_headers()
{
    xil_printf("\n\r");
    xil_printf("%20s %6s %s\n\r", "Server", "Port", "Connect With..");
    xil_printf("%20s %6s %s\n\r", "--------------------", "------", "--------------------");

    if (INCLUDE_WEB_SERVER)
        print_web_app_header();

    xil_printf("\n\r");
}

void web_application_thread(void *);

void
launch_app_threads()
{
    /* start webserver thread */
    if (INCLUDE_WEB_SERVER)
        sys_thread_new("httpd", web_application_thread, 0,
            THREAD_STACKSIZE,
            DEFAULT_THREAD_PRIO);

}
