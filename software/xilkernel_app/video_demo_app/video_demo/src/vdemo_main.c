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
*
* File:	  vdemo_main.c
* Author: srinivasa attili
* Date:	  7-sept-2011
*
* Purpose: Entry point for the Video Demo application.
*    main() function is invoked at the beginning.
*    Compliant to Xilinx Standalone BSP / XilKernel BSP.
*
* MODIFICATION HISTORY:
*
* Ver   Who                Date     Changes
* ----- -----------------  --------  ----------------------
* 1.00  srinivasa attili   9-7-11   Initial Release
* 2.00  srinivasa attili   2-2-12   for Kintex7 Demo TRD
*
*
**************************************************************************/

/***************************** Include Files *********************************/
#include <stdio.h>
#include <ctype.h>
#include "xparameters.h"
#include "vdma_data.h"
#include "xuartlite_l.h"
#include "board_test_app.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
// MACRO to flush keyboard buffer
#define FlushKeyboard	\
		while( !(XUartLite_IsReceiveEmpty(STDIN_BASEADDRESS)) )	{ \
			inbyte();	\
		};

// MACRO checks if any key pressed (TRUE=no key pressed, FALSE=key pressed
#define iskeypress	(!(XUartLite_IsReceiveEmpty(STDIN_BASEADDRESS)))

// MACRO causes delay #seconds.. use actual delay APIs when OS is used.
#define DELAY_SECONDS(sec)	\
		{ int i;			\
		  for(i=0; i<sec*200000; i++);	\
		}

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
struct VDMA_PERF_MONITOR perf;


void vdemo_banner(void)
{
	#if 0
	/* Initialize RS232_Uart_1 - Set baudrate and number of stop bits */
	XUartNs550_SetBaud(XPAR_RS232_UART_1_BASEADDR, XPAR_XUARTNS550_CLOCK_HZ, 9600);
	XUartNs550_SetLineControlReg(XPAR_RS232_UART_1_BASEADDR, XUN_LCR_8_DATA_BITS);
	#endif
    print("\n\r****************************************************************");
    print("\n\r****************************************************************");
    print("\n\r**     Xilinx Virtex-6 FPGA ML605 Evaluation Kit Video Demo   **");
    print("\n\r****************************************************************");
    print("\n\r****************************************************************\r\n");

    return;
}

int vdemo_menu(void)
{
	int choice;

	print("\n*Note* This Demo works only with 1920x1080 resolution input \r\n");
	print("\n ********************************************************\r");
	print("\n ************ Choose Video Stream to display ************\r");
	print("\n ********************************************************\r\n");
	print("\tSTREAM #0 (Input Video_0)     \r\n ");
	print("\t  1: Full (1920x1080)         \r\n ");
	print("\t  2: Scaled (960x540)         \r\n \r\n");
	print("\tSTREAM #1 (Input Video_1)     \r\n ");
	print("\t  3: Full (1920x1080)         \r\n ");
	print("\t  4: Scaled (960x540)         \r\n \r\n");
	print("\tTEST PATTERNS (always scaled) \r\n ");
	print("\t  5: Pattern_0 (960x540)      \r\n ");
	print("\t  6: Pattern_1 (960x540)      \r\n \r\n");
	print("\tALL Streams (scaled- 960x540) \r\n ");
	print("\t  7: Four tiles               \r\n");
	print("\t0: EXIT\r\n");

	do {
		print("Enter your choice: ");
		choice = inbyte();
	} while (!isdigit(choice));
	xil_printf("%c\r\n", choice);

	return choice;
}

void vdemo_print_statistics(char* msg, struct VDMA_PERF_MONITOR *perf)
{
	int i;
	char *vdma_str[] =
		{	"DVI_0",
			"DVI_1",
			"SCL_0",
			"SCL_2",
			"TPG_0",
			"TPG_2"
		};

	xil_printf("  ----------------------------------------------------------------------------------\r\n");
	for(i=0; i<MAX_VDMA_INSTANCE; i++)
	{
		xil_printf("  |Interrupts |ID:%s  |Rd:%-6d   |RdErr:%-6d    |Wr:%-6d    |WrErr:%-6d   |\r\n",
				vdma_str[i],
				VdmaInstance[i].ReadIntrCnt,
				VdmaInstance[i].ReadErrIntrCnt,
				VdmaInstance[i].WriteIntrCnt,
				VdmaInstance[i].WriteErrIntrCnt);
	}
	xil_printf("  ----------------------------------------------------------------------------------\r\n");
	xil_printf("  |%s: performance: Write(Gbps)= %-2d   Read(Gbps)= %-2d   Interrupts: %-5d         \r\n",
				msg, perf->write_perf, perf->read_perf, perf->perfIntrCnt );
	xil_printf("  ----------------------------------------------------------------------------------\r\n");

	return;
}

/*****************************************************************************/
/**
* This is the Entry function.
* Main function of the Video Demo application.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
void main_vdemo_webserver(void *p)
{
	/** Initialize all HW components **/
	vdemo_banner();				// Application Banner
	init_TimeBaseDetection();	// detect the input streams and their resolutions
	init_Timebase();			// setup time base for Test Pattern Generators (tpg #0, #1)
	//init_DVI();					// initialize the DVI controller for output display
	init_VDMA();				// initialize ALL Video DMA channels
	init_PerfMonitor(&perf);	// initialize Performance Monitor (DDR AXI MM throughput monitor)
}
void vdemo_webserver_reset(void)
{
	reset_VDMA();					// reset the video system before new video stream
	init_CVC();						// reset the video IP
	reset_PerfMonitor_stats(&perf);
}

#if 0
void main_vdemo_console(void)
{
    int choice, exit_flag;
    char msg[30];

    /** Initialize all HW components **/
	vdemo_banner();				// Application Banner
	init_TimeBaseDetection();	// detect the input streams and their resolutions
	init_Timebase();			// setup time base for Test Pattern Generators (tpg #0, #1)
	init_DVI();					// initialize the DVI controller for output display
	init_VDMA();				// initialize ALL Video DMA channels
	init_PerfMonitor(&perf);	// initialize Performance Monitor (DDR AXI MM throughput monitor)

	exit_flag = 0;
	while(exit_flag != 1) {
		// STEP 1:  Display menu and Get input selection from user.
		choice = vdemo_menu();
		reset_VDMA();					// reset the video system before new video stream
		init_CVC();						// reset the video IP
		reset_PerfMonitor_stats(&perf);	// reset performance monitor

		// STEP 2: Take action on the selected stream display
		switch(choice) {
    		case '0':
				exit_flag = 1;
				break;

			// Input video 0 full
    		case '1':
				config_vdma_fullVideo_0();
				config_cvc_fullVideo_0();
				strcpy(msg, "video_0 full");
				break;

			// Input video 0 scaled
    		case '2':
				config_vdma_scaleVideo_0();
				config_cvc_scaleVideo_0();
				strcpy(msg, "video_0 scaled");
				break;

			// Input video 1 full
    		case '3':
				config_vdma_fullVideo_1();
				config_cvc_fullVideo_1();
				strcpy(msg, "video_1 full");
				break;

			// Input video 1 scaled
    		case '4':
				config_vdma_scaleVideo_1();
				config_cvc_scaleVideo_1();
				strcpy(msg, "video_1 scaled");
				break;

			// test pattern 0 (always scaled)
    		case '5':
				config_vdma_tpgVideo_0();
				config_cvc_tpgVideo_0();
				strcpy(msg, "tpg_0");
				break;

			// test pattern 1 (always scaled)
    		case '6':
				config_vdma_tpgVideo_1();
				config_cvc_tpgVideo_1();
				strcpy(msg, "tpg_1");
				break;

			// all videos and test patterns (4 total), scaled
    		case '7':
				config_vdma_scaleVideo_both();
				init_Timebase();
				config_cvc_scaleVideo_both();
				strcpy(msg, "all videos scaled");
				break;

    		default:
				break;
		}

    	if(exit_flag == 1)
    		break;		// Exit

		xil_printf("DDR AXI MM throughput data (Gbps): \r\n" );
		FlushKeyboard;

		// STEP 3: keep display the statistics
		//	Wait for 2 conditions: statistics are ready; any key pressed
    	while(1)
    	{
    		// check if any key pressed
    		if( iskeypress )
			{
				break;
			}

    		// check for statistics are ready
    		if(perf.pollmode == 1)
    		{
    			DELAY_SECONDS(3);			// if performance monitor is in poll mode,
    			PerfMon_pollHandler(&perf);	// get the statistics for every 3 seconds
    		}

    		if(perf.dataReady == 1)
			{
        		vdemo_print_statistics(msg, &perf);
    			perf.dataReady = 0;
        		xil_printf("  press <Enter> to stop.\r\n" );
    			//xil_printf("Masking Perf Monitor Interrupt \r\n");
    			//XIntc_Disable(&InterruptController, PERFMON_INTR_ID);
			}
    	}
    	FlushKeyboard;	// read and discard the remaining pressed keys
	}

	shutdown_CVC();
    print("Good-bye!\r\n");

    return;
}
#endif


