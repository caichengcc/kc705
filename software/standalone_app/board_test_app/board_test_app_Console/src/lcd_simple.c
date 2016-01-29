/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2009 Xilinx Inc.
*       All rights reserved.
*
* Created on 2,Feb,2012 srinivasa attili; for Kintex7 Demo
*
******************************************************************************/


#include "xparameters.h"
#include "stdio.h"
#include "xgpio.h"
#include "xbasic_types.h"
#include "xuartns550_l.h"
#include "board_test_app.h"
//#include "xil_sprintf.h"


#ifndef UART_BASEADDR
#define UART_BASEADDR XPAR_UARTNS550_0_BASEADDR
#endif
#define UART_CLOCK    XPAR_UARTNS550_0_CLOCK_FREQ_HZ

#if !SIM
#define UART_BAUDRATE 9600                      /* real hardware */
#else
#define UART_BAUDRATE (UART_CLOCK / 16 / 3)     /* simulation */
#endif

#define	XGpio_mSetDataReg		XGpio_WriteReg
#define	XGpio_mSetDataDirection	XGpio_WriteReg



// Main Loop
int lcd_simple(void)
{

    //char alm_str[20];
    //int i;

    /***
    XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
    XUartNs550_mSetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);
    ***/

    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r**     KC705 - LCD Test                               **");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************\r\n");

    kc_initLCD();

    // Print to the LCD
    // NOTE: Keep each string to be 16 characters max.
   	kc_LCDPrintString ("  Embedded TRD  ","KC705 BIST DEMO!");
    return 0;
}
