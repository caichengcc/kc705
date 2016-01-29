
/**************************************************************************
*
*     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
*     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
*     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
*     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS
*     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*     FOR A PARTICULAR PURPOSE.
*
*     (c) Copyright 2007 Xilinx, Inc.
*     All rights reserved.
*
**************************************************************************/



/**************************************************************************
* Filename:     hello_uart.c
*
* Description:
* Application that prints Hello Uart to the RS232.
*
*
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  jel  5-15-07  Initial Release
* 1.01  jel  10-16-07 Added Hardware and Simulation Support
* modified on 2,Feb,2012 srinivasa attili; modified for Kintex7 Demo
*
*
*
******************************************************************************/


/***************************** Include Files *********************************/
#include "xparameters.h"
#include "xuartns550_l.h"
#include "xbasic_types.h"
#include <string.h>
#include "board_test_app.h"


#if !SIM
#include "stdio.h"
#endif

/************************** Constant Definitions *****************************/


/************************** Function Prototypes ******************************/
#if SIM
static void printf(char *s)
{
  while (*s) {
    XUartNs550_SendByte(UART_BASEADDR, *s);
    s++;
  }
}
#endif

int hello_uart2 (int uartbaud, char* log)
{

   xil_printf("\n\r********************************************************");
   xil_printf("\n\r********************************************************");
   xil_printf("\n\r**     KC705 - UART Test                              **");
   xil_printf("\n\r********************************************************");
   xil_printf("\n\r********************************************************\r\n");

   xil_printf("Testing UART\r\n");
   xil_printf("%d,8,N,1\r\n", uartbaud);
   xil_printf("Hello world!\r\n");
   xil_printf("UART Test Passed\r\n\r\n");

   strcpy(log, "Testing UART\r\n");
   strcat(log, "9600,8,N,1\r\n");
   strcat(log, "Hello world!\r\n");
   strcat(log, "UART Test Passed\r\n\r\n");

   return 0;

}

int hello_uart (int uartbaud) {

   xil_printf("\n\r********************************************************");
   xil_printf("\n\r********************************************************");
   xil_printf("\n\r**     KC705 - UART Test                              **");
   xil_printf("\n\r********************************************************");
   xil_printf("\n\r********************************************************\r\n");

   xil_printf("Testing UART\r\n");
   xil_printf("%d,8,N,1\r\n", uartbaud);
   xil_printf("Hello world!\r\n");
   xil_printf("UART Test Passed\r\n\r\n");


   return 0;

}
