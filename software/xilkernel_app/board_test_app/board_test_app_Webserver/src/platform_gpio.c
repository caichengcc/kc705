/*
 * Copyright (c) 2007 Xilinx, Inc.  All rights reserved.
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
 *Created on 2,Feb,2012 srinivasa attili; for Kintex7 Demo
 *
 */

#include "platform_gpio.h"
#include "xparameters.h"
#include <string.h>

#if defined(XPAR_LEDS_8BIT_BASEADDR)
#define LED_BASE XPAR_LEDS_8BIT_BASEADDR
#elif defined(XPAR_LEDS_4BIT_BASEADDR)
#define LED_BASE XPAR_LEDS_4BIT_BASEADDR
#elif defined(XPAR_LEDS_6BIT_BASEADDR)
#define LED_BASE XPAR_LEDS_6BIT_BASEADDR
#else
#define NO_GPIOS
#endif

#if defined(XPAR_DIP_SWITCHES_8BIT_BASEADDR)
#define DIP_BASE XPAR_DIP_SWITCHES_8BIT_BASEADDR
#elif defined(XPAR_DIPS_4BIT_BASEADDR)
#define DIP_BASE XPAR_DIPS_4BIT_BASEADDR
#elif defined(XPAR_PUSH_BUTTONS_POSITION_BASEADDR)
#define DIP_BASE XPAR_PUSH_BUTTONS_POSITION_BASEADDR
#endif

void
platform_init_gpios()
{
#ifndef NO_GPIOS
    /* set led gpio data direction to output */
    *(volatile unsigned int*)(LED_BASE + 4) = 0;

    /* set dip switch gpio data direction to in */
    *(volatile unsigned int*)(DIP_BASE + 4) = ~0;

    /* initialize leds to OFF */
    *(volatile int *)(LED_BASE) = 0;
#endif
}

int
set_leds(char *data)
{

#ifndef NO_GPIOS
	int gpioData;  // Data to be sent to the GPIO
    int data_input[2];
    int data_len;  // Length of data sent to Web server
    int position = 0;  // Position in two digit Hex number

    data_len = strlen(data);
    while (position < data_len) {
	    switch(data[position]) {
	        case '0':
	           data_input[position] = 0;
	           break;
	        case '1':
	           data_input[position] = 1;
	           break;
			case '2':
	           data_input[position] = 2;
	           break;
	        case '3':
	           data_input[position] = 3;
	           break;
	        case '4':
	           data_input[position] = 4;
	           break;
	        case '5':
	           data_input[position] = 5;
	           break;
	        case '6':
	           data_input[position] = 6;
	           break;
	        case '7':
	           data_input[position] = 7;
	           break;
	        case '8':
	           data_input[position] = 8;
	           break;
	        case '9':
	           data_input[position] = 9;
	           break;
	        case 'a': case 'A':
	           data_input[position] = 10;
	           break;
	        case 'b': case 'B':
	           data_input[position] = 11;
	           break;
	        case 'c': case 'C':
	           data_input[position] = 12;
	           break;
	        case 'd': case 'D':
	           data_input[position] = 13;
	           break;
	        case 'e': case 'E':
	           data_input[position] = 14;
	           break;
	        case 'f': case 'F':
	           data_input[position] = 15;
	           break;
	        default:
	           data_input[position] = 0;
	           break;
    	}
    	position = position + 1;
	}
	if(data_len == 2)
	{
		gpioData = 16 * data_input[0] + data_input[1];
	}
	else
	{
		gpioData = data_input[0];
	}
    *(volatile int *)(LED_BASE) = gpioData;
#endif
    return 0;
}

unsigned int
get_switch_state()
{
#ifdef NO_GPIOS
    return 0x0;
#else
    return *(volatile unsigned int *)(DIP_BASE);
#endif
}
