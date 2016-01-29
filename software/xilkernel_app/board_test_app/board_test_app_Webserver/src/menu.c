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
*     (c) Copyright 2010 Xilinx, Inc.
*     All rights reserved.
*
**************************************************************************/
/**************************************************************************
* Filename:     menu.c
*
* Description:
* Menu that runs the board test applications.
*
*
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  cmc  8-27-09  Initial Release
*
* modified on 2,Feb,2012 srinivasa attili; modified for Kintex7 Demo
*
******************************************************************************/
#include "menu.h"
#include "board_test_app.h"
#include <ctype.h>
#include "xparameters.h"
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

//int main()
void main_BoardTestApp(void *p)
{
    int choice, exit_flag;

    // for 13.4, 0.87 build
	ENABLE_ICACHE();
	ENABLE_DCACHE();

    /* Initialize RS232_Uart_1 - Set baudrate and number of stop bits */
    //XUartNs550_SetBaud(XPAR_RS232_UART_1_BASEADDR, XPAR_XUARTNS550_CLOCK_HZ,
    //                   UART_BAUDRATE);
    //XUartNs550_SetLineControlReg(XPAR_RS232_UART_1_BASEADDR,
    //                              XUN_LCR_8_DATA_BITS);

    print("\n\r********************************************************");
    print("\n\r********************************************************");
    print("\n\r**     Xilinx Kintex-7 FPGA KC705 Evaluation Kit      **");
    print("\n\r********************************************************");
    print("\n\r********************************************************\r\n");

	exit_flag = 0;
	while(exit_flag != 1)
	{
		print("\n\r=====================================\n\r");
	    print("**  Xilinx Kintex-7 BIST MENU      **\n\r");
	    print("=====================================\n\r");
    	print("Choose Feature to Test:\r\n");
    	print("[1] uart      [2] led                \r\n");
    	print("[3] iic       [4] flash              \r\n");
    	print("[5] timer     [6] rotary switch      \r\n");
    	print("[7] switch    [8] sd card            \r\n");
    	print("[9] lcd       [A] xadc               \r\n");
    	print("[B] button    [C] ethernet loopback  \r\n");
    	print("[D] ddr3 external memory (16MB)      \r\n");
    	print("[E] ddr3 external memory (complete)  \r\n");
    	print("[F] bram internal memory             \r\n");
    	//print("[G] Repeat Tests - Power off board to stop\r\n");
    	//print("    (Coverage: UART,LED,IIC,Flash,Timer,DIPSW,SD,LCD,DDR3,BRAM,XADC)\r\n");
    	print("[0] Exit\r\n");
    	choice = inbyte();
        if (isalpha(choice))
        {
        	choice = toupper(choice);
        }
    	xil_printf("%c\r\n", choice);

		switch(choice) {
			case '0':
				exit_flag = 1;
				break;
			case '1':
				hello_uart(UART_BAUDRATE);
				break;
			case '2':
				hello_led();
				break;
			case '3':
				hello_iic();
				break;
			case '4':
				hello_flash();
				break;
			case '5':
				hello_timer();
				break;
			case '6':
				hello_rotary();
				break;
			case '7':
				hello_switch();
				break;
			case '8':
				hello_sd();
				break;
			case '9':
				lcd_simple();
				break;
			case 'A':
				main_sysmonIntr();
				break;
			case 'B':
				hello_button();
				break;
			case 'C':
				//hello_temac();
				break;
			case 'D':
				hello_mem();
				break;
			case 'E':
				//hello_mem_complete();
				break;
			case 'F':
				hello_bram();
				break;
			case 'G':
				//main_automate();	// Infinite (need to power off board to stop)
				break;
			default:
				break;
		}
		if(exit_flag != 1) {
			print("Press any key to return to main menu\r\n");
			inbyte();
		}
	}
	print("Good-bye!\r\n");
	exit(0);

    return;
}


