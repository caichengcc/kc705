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

#include <stdio.h>
#include <stdlib.h>

#include "xparameters.h"
#include "xgpio.h"
#include "xbasic_types.h"
#include "xuartns550_l.h"
#include "board_test_app.h"
//#include "xil_sprintf.h"

#define	XPAR_ROTARY_GPIO_BASEADDR XPAR_GPIO_INTERFACE_ROTARY_GPIO_BASEADDR

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
#define	XGpio_mGetDataReg		XGpio_ReadReg
#define	XGpio_mSetDataDirection	XGpio_WriteReg


#define	DIR_LEFT	(-1)
#define	DIR_RIGHT	(+1)

// Main Loop

int hello_rotary2(void)
{
    unsigned int data;
    int pulses=0, dir=0;

    //set GPIO input mode
	XGpio_mSetDataReg(XPAR_ROTARY_GPIO_BASEADDR, 4, 0xffffffff);

	/////////////////////////////////////
	// STATE 1: Get the direction pulse
	//xil_printf("   \r\nState1 \r\n");
	do
	{
		// get hold of a pulse that tells one of below
		// 		bits[1:0] = 01 Left rotation
		//		bits[1:0] = 10 Right rotation
		//		bit 2     =  1 button press

		data = XGpio_mGetDataReg(XPAR_ROTARY_GPIO_BASEADDR, 0);
		if(data & 0x1)
		{
			dir = DIR_LEFT;
			break;
		}
		if(data & 0x2)
		{
			dir = DIR_RIGHT;
			break;
		}

	} while( (data& 0x3) == 0);

	//////////////////////////////////////////////
	// STATE 2: Get the pulses from both switches
	//xil_printf("   State2 \r\n");
	do
	{
		data = XGpio_mGetDataReg(XPAR_ROTARY_GPIO_BASEADDR, 0);
	} while( (data& 0x3) != 0x3);

	/////////////////////////////////////////////////////
	// STATE 3: Get the pulses from both switches to NULL
	//xil_printf("   State3 \r\n");
	do
	{
		data = XGpio_mGetDataReg(XPAR_ROTARY_GPIO_BASEADDR, 0);
	} while( (data& 0x3) != 0);

	// RESULT TO USER
	pulses += dir;
	xil_printf("%s-%d  [Exit: press anykey]\r\n",
			(dir==DIR_RIGHT) ? "Anti-Clockwise" : "     Clockwise",
			abs(pulses) );

	return dir;
}

int hello_rotary(void)
{

    unsigned int data;
    int pulses=0, dir=0;

	/***
    XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
    XUartNs550_mSetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);
    ***/

    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r**     KC705 - Rotary Switch Test                     **");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************\r\n");
    xil_printf("Watch the ROTARY pulses count:\r\n");

    xil_printf("press any key to exit the test\r\n");
    XUartNs550_ReadReg(STDIN_BASEADDRESS, XUN_RBR_OFFSET);

    //set GPIO input mode
	XGpio_mSetDataReg(XPAR_ROTARY_GPIO_BASEADDR, 4, 0xffffffff);

	while(1)
	{
		/////////////////////////////////////
		// STATE 1: Get the direction pulse
		//xil_printf("   \r\nState1 \r\n");
		do
		{
			// get hold of a pulse that tells one of below
			// 		bits[1:0] = 01 Left rotation
			//		bits[1:0] = 10 Right rotation
			//		bit 2     =  1 button press

			data = XGpio_mGetDataReg(XPAR_ROTARY_GPIO_BASEADDR, 0);
			if(data & 0x1)
			{
				dir = DIR_LEFT;
				break;
			}
			if(data & 0x2)
			{
				dir = DIR_RIGHT;
				break;
			}

			if( XUartNs550_IsReceiveData(STDIN_BASEADDRESS) )
				goto rotary_exit;

		} while( (data& 0x3) == 0);

		//////////////////////////////////////////////
		// STATE 2: Get the pulses from both switches
		//xil_printf("   State2 \r\n");
		do
		{
			data = XGpio_mGetDataReg(XPAR_ROTARY_GPIO_BASEADDR, 0);
			if( XUartNs550_IsReceiveData(STDIN_BASEADDRESS) )
				goto rotary_exit;

		} while( (data& 0x3) != 0x3);

		/////////////////////////////////////////////////////
		// STATE 3: Get the pulses from both switches to NULL
		//xil_printf("   State3 \r\n");
		do
		{
			data = XGpio_mGetDataReg(XPAR_ROTARY_GPIO_BASEADDR, 0);
			if( XUartNs550_IsReceiveData(STDIN_BASEADDRESS) )
				goto rotary_exit;
		} while( (data& 0x3) != 0);

		// PRESS ANY KEY TO EXIT
		if( XUartNs550_IsReceiveData(STDIN_BASEADDRESS) )
			goto rotary_exit;

		// RESULT TO USER
		pulses += dir;
		xil_printf("%s-%d  [Exit: press anykey]\r\n",
				(dir==DIR_RIGHT) ? "Anti-Clockwise" : "     Clockwise",
				abs(pulses) );
	}

rotary_exit:
	XUartNs550_ReadReg(STDIN_BASEADDRESS, XUN_RBR_OFFSET);

	return 0;
}