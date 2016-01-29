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
* created on 2,Feb,2012 srinivasa attili; for Kintex7 Demo TRD
**************************************************************************/


/***************************** Include Files *********************************/
#include "xmk.h"
#include "lwip/sys.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include <sys/timer.h>
#include "pthread.h"
#include "mb_interface.h"
#include "board_test_app.h"
#include "logiCVC_demo.h"
#include "vdma_data.h"

/***************************** Constants *********************************/
#define DELAY_SECONDS(sec)				\
		{ int i;						\
		  for(i=0; i<sec*200000; i++);	\
		}

#define PRINT(...) xil_printf(__VA_ARGS__)
#define ERROR_PRINT(...) PRINT(__VA_ARGS__)
#define DEBUG_PRINT(...) PRINT(__VA_ARGS__)
#define APP_PRINT(...) PRINT(__VA_ARGS__)

/***************************** Global variables *********************************/
struct VDMA_PERF_MONITOR perf;
int new_vsel=0, new_psel=0, new_ssel=0, new_lsel=LIVEVIDEO_DISABLE;
sys_sem_t vdemo_mutex;
sys_sem_t *vdemo_mutexp = &vdemo_mutex;	// LWIP140

/***************************** External functions *********************************/
extern XStatus init_logiCVC(logiCVC_plfInitCallbackT *plfDisplayCallbacks, int DemoMode);
extern XStatus reset_logiCVC(void);
extern XStatus disable_logiCVC_layers(void);
extern XStatus setup_logiCVC(int selection);

/***************************** Internal functions *********************************/
static int setup_VDMA(int lsel, int vsel, int ssel);
static int check_VDMA(int lsel, int selection);
void* perfmon_thread(void *p);
void* vpattern_thread(void *p);


/*****************************************************************************/
/**
* ENTRY POINT : This is the ONLY user interface.
*    this function stops the current video selections and
*    sets up the video streams, CVC as per new selection.
*
* @param	lsel : Live Video enabled or not
* @param    vsel : video selection (full or scaled)
* @param    ssel : selection of scaled videos (applicable if vsel is set to scaled)
* @param    psel : selection of any patterns (none / alpha blend / move)
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		:
*
******************************************************************************/
int hello_vdemo(int lsel, int vsel, int ssel, int psel)
{
	int Status, ret = XST_SUCCESS;
	int failed_streams=0;
	// Reset the global flags, we set them to new selections
	new_vsel = 0;
	new_psel = 0;
	new_ssel = 0;
	new_lsel = LIVEVIDEO_DISABLE;

	/************************************************************************
	 * stop the current video process                                       *
	 *    Step 1: deactivate the Video IN (GPIO gating)                     *
	 *    Step 2: stop the CVC processing frames (disable all layers)       *
	 *    Step 3: stop all the VDMA channels                                *
	 *    Step 4: clear the performance numbers                             *
	 ************************************************************************/

	//xil_printf("  reset VDMAs \r\n");
	reset_VDMA();
	//xil_printf("  reset logiCVC \r\n");
	reset_logiCVC();
	//xil_printf("  disable logiCVC layers\r\n");
	disable_logiCVC_layers();
	//xil_printf("  deactivate video system \r\n");
	deactivate_vdemo_system();
	//xil_printf("  reset TPGs \r\n");
	//reset_TPG();
	//xil_printf("  stop perf monitor \r\n");
	stop_PerfMonitor();
	//xil_printf("  reset perfmon statistics \r\n");
	reset_PerfMonitor_stats(&perf);

	/*******************************************************************************
	 * configure to new video process                                              *
	 *     Step 5: reset the logiCVC state machine                                 *
	 *     Step 6: reconfigure the VDMA as per new selection                       *
	 *     Step 7: activate the Video IN (GPIO gating)                             *
	 *     Step 8: check for VDMA activity (park pointer)                          *
	 *     Step 9: reconfigure the CVC                                             *
	 *     Screen Layout:                                                          *
	 *     		STREAM0 => Layer0 : DVI_IN_0 or TPG_1 (full or scaled, top left)   *
	 *     		STREAM1 => Layer2 : DVI_IN_1 or TPG_3 (full or scaled, bot right)  *
	 *     		TPG_0   => Layer1 : TPG_0 (always scaled, top right)               *
	 *     		TPG_2   => Layer3 : TPG_2 (always scaled, bot left)                *
	 *******************************************************************************/
	//reset_logiCVC_DTYPE();
	//xil_printf("  setup VDMAs for new stream \r\n");

	// Nothing to setup, exit
	if( (vsel==0) && (ssel==0) )
		return XST_SUCCESS;

	setup_VDMA(lsel, vsel, ssel);
	if(vsel != 0)
	{
		//xil_printf("  activate video system\r\n");
		activate_vdemo_system(lsel, vsel, ssel);
	}
	///xil_printf("  reset logiCVC\r\n");
	reset_logiCVC();
	//sleep(1);

	//xil_printf("  check VDMA, setup logiCVC \r\n");
	switch(vsel)
	{
		case WS_STREAM0_FULL:
			Status = check_VDMA(lsel, SYS_STREAM0_FULL);
			if(Status == XST_SUCCESS)
				 setup_logiCVC(SYS_STREAM0_FULL);
			else
			{
				failed_streams |= WS_STREAM0_FULL;
				ret = XST_FAILURE;
			}
			break;

		case WS_STREAM1_FULL:
			Status = check_VDMA(lsel, SYS_STREAM1_FULL);
			if(Status == XST_SUCCESS)
				 setup_logiCVC(SYS_STREAM1_FULL);
			else
			{
				failed_streams |= WS_STREAM1_FULL;
				ret = XST_FAILURE;
			}
			break;

		case WS_SCALED_STREAMS:
			if(ssel & WS_STREAM0_SCALED)
			{
				Status = check_VDMA(lsel, SYS_STREAM0_SCALED);
				if(Status == XST_SUCCESS)
					 setup_logiCVC(SYS_STREAM0_SCALED);
				else
				{
					failed_streams |= WS_STREAM0_SCALED;
					ret = XST_FAILURE;
				}
			}
			if(ssel & WS_STREAM1_SCALED)
			{
				Status = check_VDMA(lsel, SYS_STREAM1_SCALED);
				if(Status == XST_SUCCESS)
					 setup_logiCVC(SYS_STREAM1_SCALED);
				else
				{
					failed_streams |= WS_STREAM1_SCALED;
					ret = XST_FAILURE;
				}
			}
			if(ssel & WS_TPG0_SCALED)
			{
				Status = check_VDMA(lsel, SYS_TPG0_SCALED);
				if(Status == XST_SUCCESS)
					 setup_logiCVC(SYS_TPG0_SCALED);
				else
				{
					failed_streams |= WS_TPG0_SCALED;
					ret = XST_FAILURE;
				}
			}
			if(ssel & WS_TPG2_SCALED)
			{
				Status = check_VDMA(lsel, SYS_TPG2_SCALED);
				if(Status == XST_SUCCESS)
					 setup_logiCVC(SYS_TPG2_SCALED);
				else
				{
					failed_streams |= WS_TPG2_SCALED;
					ret = XST_FAILURE;
				}
			}
			break;
	}

	if(psel != 0)
		sleep(2000);	// keep the screen steady for a while, before apply patterns

	// UPDATE Global flags
	new_vsel = vsel;
	new_psel = psel;
	new_ssel = ssel;
	new_lsel = lsel;

	// Error, return from here
	if(ret != XST_SUCCESS)
		return failed_streams;

//	xil_printf("  start perfmon\r\n");
	start_PerfMonitor();

#if 0
	xil_printf("===========================\r\n");
	xil_printf(" DVI_0(0x5030): 0x00=0x%08x 0x30=0x%08x \r\n",
			*((volatile int *)(0x50300000)), *((volatile int *)(0x50300030)) );
	xil_printf(" SCL_0(0x5040): 0x00=0x%08x 0x30=0x%08x \r\n",
			*((volatile int *)(0x50400000)), *((volatile int *)(0x50400030)) );
	xil_printf(" DVI_1(0x5090): 0x00=0x%08x 0x30=0x%08x \r\n",
			*((volatile int *)(0x50900000)), *((volatile int *)(0x50900030)) );
	xil_printf(" SCL_2(0x50A0): 0x00=0x%08x 0x30=0x%08x \r\n",
			*((volatile int *)(0x50A00000)), *((volatile int *)(0x50A00030)) );
	xil_printf(" TPG_0(0x5070): 0x00=0x%08x 0x30=0x%08x \r\n",
			*((volatile int *)(0x50700000)), *((volatile int *)(0x50700030)) );
	xil_printf(" TPG_2(0x50c0): 0x00=0x%08x 0x30=0x%08x \r\n",
			*((volatile int *)(0x50C00000)), *((volatile int *)(0x50C00030)) );
	xil_printf(" L0_CTRL: 0x%0x\r\n", *((volatile int *)(0x50D00138)) );
	xil_printf(" L1_CTRL: 0x%0x\r\n", *((volatile int *)(0x50D001B8)) );
	xil_printf(" L2_CTRL: 0x%0x\r\n", *((volatile int *)(0x50D00238)) );
	xil_printf(" L3_CTRL: 0x%0x\r\n", *((volatile int *)(0x50D002B8)) );
	xil_printf(" L4_CTRL: 0x%0x\r\n", *((volatile int *)(0x50D00338)) );
	xil_printf("\r\n");
#endif

	return XST_SUCCESS;
}

static int setup_VDMA(int lsel, int vsel, int ssel)
{
	//////////////////////
	// FULL VIDEO STREAMS

	if(vsel == WS_STREAM0_FULL)
	{
		//xil_printf("    config_vdma_fullVideo_0 \r\n");
		config_vdma_fullVideo_0();
	}
	if(vsel == WS_STREAM1_FULL)
	{
		//xil_printf("    config_vdma_fullVideo_1 \r\n");
		config_vdma_fullVideo_1();
	}
	////////////////////////
	// SCALED VIDEO STREAMS
	if(vsel == WS_SCALED_STREAMS)
	{
		if(ssel & WS_STREAM0_SCALED)
		{
			//xil_printf("    config_vdma_scaleVideo_0 \r\n");
			config_vdma_scaleVideo_0();
		}
		if(ssel & WS_STREAM1_SCALED)
		{
			//xil_printf("    config_vdma_scaleVideo_1 \r\n");
			config_vdma_scaleVideo_1();
		}
		if(ssel & WS_TPG0_SCALED)
		{
			//xil_printf("    config_vdma_tpgVideo_0 \r\n");
			config_vdma_tpgVideo_0();
		}
		if(ssel & WS_TPG2_SCALED)
		{
			//xil_printf("    config_vdma_tpgVideo_1 \r\n");
			config_vdma_tpgVideo_1();
		}
	}
	return 0;
}

static int check_VDMA(int lsel, int selection)
{
	int Status = XST_SUCCESS;

	switch(selection)
	{
		case SYS_STREAM0_FULL:
			Status = check_config_vdma_fullVideo_0();
			break;

		case SYS_STREAM1_FULL:
			Status = check_config_vdma_fullVideo_1();
			break;

		case SYS_STREAM0_SCALED:
			Status = check_config_vdma_scaleVideo_0();
			break;

		case SYS_STREAM1_SCALED:
			Status = check_config_vdma_scaleVideo_1();
			break;

		case SYS_TPG0_SCALED:
			Status = check_config_vdma_tpgVideo_0();
			break;

		case SYS_TPG2_SCALED:
			Status = check_config_vdma_tpgVideo_1();
			break;
	}

	if(Status != XST_SUCCESS)
	{
		xil_printf("Error: Check the input stream ");
		if(selection == SYS_STREAM0_FULL)   { xil_printf("SYS_STREAM0_FULL \r\n");  }
		if(selection == SYS_STREAM1_FULL)   { xil_printf("SYS_STREAM1_FULL \r\n");  }
		if(selection == SYS_STREAM0_SCALED) { xil_printf("SYS_STREAM0_SCALED \r\n");}
		if(selection == SYS_STREAM1_SCALED) { xil_printf("SYS_STREAM1_SCALED \r\n");}
		if(selection == SYS_TPG0_SCALED)    { xil_printf("SYS_TPG0_SCALED \r\n");   }
		if(selection == SYS_TPG2_SCALED)    { xil_printf("SYS_TPG2_SCALED \r\n");   }
	}
	return Status;
}

int get_currentFrmaeNumber_VDMA(int selection)
{
	int Status = XST_SUCCESS;

    //xil_printf("get_currentFrmaeNumber_VDMA, %d \r\n", selection);
	switch(selection)
	{
		case SYS_STREAM0_FULL:
		    //xil_printf("get_currentFrmaeNumber_fullVideo_0 \r\n");
			Status = get_currentFrmaeNumber_fullVideo_0();
			break;

		case SYS_STREAM1_FULL:
		    //xil_printf("get_currentFrmaeNumber_fullVideo_1 \r\n");
			Status = get_currentFrmaeNumber_fullVideo_1();
			break;

		case SYS_STREAM0_SCALED:
		    //xil_printf("get_currentFrmaeNumber_scaleVideo_0 \r\n");
			Status = get_currentFrmaeNumber_scaleVideo_0();
			break;

		case SYS_STREAM1_SCALED:
		    //xil_printf("get_currentFrmaeNumber_scaleVideo_1 \r\n");
			Status = get_currentFrmaeNumber_scaleVideo_1();
			break;

		case SYS_TPG0_SCALED:
		    //xil_printf("get_currentFrmaeNumber_tpgVideo_0 \r\n");
			Status = get_currentFrmaeNumber_tpgVideo_0();
			break;

		case SYS_TPG2_SCALED:
		    //xil_printf("get_currentFrmaeNumber_tpgVideo_1 \r\n");
			Status = get_currentFrmaeNumber_tpgVideo_1();
			break;
	}

	return Status;
}

/*****************************************************************************/
/**
* Main thread : Xilkernel invokes this thread for Video Demo APIs
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		:
*
******************************************************************************/
void main_thread_vdemo(void* arg)
{
	int status;
	unsigned int pid_perfmon;
	unsigned int pid_vpattern;

    logiCVC_plfInitCallbackT plfDisplayCallbacks =
    {
        0,      /**< sets external clock required by logiCVC for requsted resolution*/
        0,            /**< enables and powers up indexed display*/
        0,  /**< enables backlights for indexed display */
        0       /**< sets backlight for indexed display, value: brightness level (0..100)*/
    };
	//printf("ggegegr2\n");

    sys_sem_new(vdemo_mutexp, 1);

	deactivate_vdemo_system();
	//init_TimeBaseDetection();		// invoked before xilkernel_start to avoid XMK exception
	init_Timebase();

	//init_DVI();		// select video output: onboard DVI connector
	//init_FMC_DVI();	// select video output: FMV DVI connector
	hdmi_init();		// select video output: onboard HDMI connector
	set_hdmi_output_color();

	init_VDMA();
	//reset_logiCVC_DTYPE();
	init_logiCVC(&plfDisplayCallbacks, VIDEO_STREAM_MODE);
	//reset_logiCVC_DTYPE();
	status = init_PerfMonitor(&perf);
	if(status != XST_SUCCESS)
		printf("Failed to load perfmon driver \r\n");
	stop_PerfMonitor();
	reset_PerfMonitor_stats(&perf);
	//activate_vdemo_system(LIVEVIDEO_DISABLE);
	sleep(3);

    sys_arch_sem_wait(vdemo_mutexp, 0);
	pthread_create((pthread_t*)&pid_perfmon, NULL, perfmon_thread, NULL);
    sleep(1);

    sys_arch_sem_wait(vdemo_mutexp, 0);
	pthread_create((pthread_t*)&pid_vpattern, NULL, vpattern_thread, NULL);
    sleep(1);

	sys_sem_free(vdemo_mutexp);		// destroy the semaphore, it is no more used
	return;
}

/*****************************************************************************/
/**
* Pattern Repeat Thread : This thread is used to implement the Animation effects
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		:
*
******************************************************************************/
void* vpattern_thread(void *p)
{
	xil_printf("spawned video patterns thread successfully \r\n");

	/* unlock mutex */
    sleep(1);
	sys_sem_signal(vdemo_mutexp);

   	while(1)
	{
		repeat_patterns_logiCVC(new_vsel, new_psel, new_ssel);
		if(new_psel != 0)
			sleep(3);		// milli seconds
	}
	return 0;
}

/*****************************************************************************/
/**
* Performance Monitor Thread : Used to collect statistics on timely basis
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		:
*
******************************************************************************/
void* perfmon_thread(void *p)
{
    /* Lock mutex */
	xil_printf("spawned perfmon thread successfully \r\n");

	/* unlock mutex */
    sleep(1);
	sys_sem_signal(vdemo_mutexp);

   	while(1)
	{
		// check for statistics are ready
		if(perf.pollmode == 1)
		{
			sleep(1);		// milli seconds
			PerfMon_pollHandler(&perf);	// get the statistics for every 3 seconds
		}

		if(perf.dataReady == 1)
		{
			//vdemo_print_statistics(msg, &perf);
			perf.dataReady = 0;
			//xil_printf("  press <Enter> to stop.\r\n" );
			//xil_printf("Masking Perf Monitor Interrupt \r\n");
			//XIntc_Disable(&InterruptController, PERFMON_INTR_ID);
		}
	}
	return 0;
}
int get_perf_results(char* perf_results)
{
	unsigned int whole1, thousandths1;
	unsigned int whole2, thousandths2;

	whole1 = perf.read_perf;
	thousandths1 = (perf.read_perf - whole1) * 1000;
	whole2 = perf.write_perf;
	thousandths2 = (perf.write_perf - whole2) * 1000;

	sprintf(perf_results, "%d.%-d, %d.%-d, ",
			whole1, thousandths1, whole2, thousandths2);

	#if 0
	xil_printf("perf_results: %s \r\n", perf_results);
	xil_printf("===========================\r\n");
	xil_printf("  DVI_0(0x5030): 0x%08x \r\n",*((volatile int *)(0x50300028)));
	xil_printf("  SCL_0(0x5040): 0x%08x \r\n",*((volatile int *)(0x50400028)));
	xil_printf("  DVI_1(0x5090): 0x%08x \r\n",*((volatile int *)(0x50900028)));
	xil_printf("  SCL_2(0x50A0): 0x%08x \r\n",*((volatile int *)(0x50A00028)));
	xil_printf("  TPG_0(0x5070): 0x%08x \r\n",*((volatile int *)(0x50700028)));
	xil_printf("  TPG_2(0x50c0): 0x%08x \r\n",*((volatile int *)(0x50C00028)));
	xil_printf("\r\n");
	#endif

	#if 0
	perf_results[j++] = perf.read_perf;
	perf_results[j++] = perf.write_perf;
	//perf_results[j++] = perf->perfIntrCnt;

	for(i=0; i<MAX_VDMA_INSTANCE; i++)
	{
		perf_results[j++] = VdmaInstance[i].ReadIntrCnt;
		perf_results[j++] = VdmaInstance[i].ReadErrIntrCnt;
		perf_results[j++] = VdmaInstance[i].WriteIntrCnt;
		perf_results[j++] = VdmaInstance[i].WriteErrIntrCnt;
	}
	#endif
	return 0;
}


/* @} */
