#if 0
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
* File:	  vdemo_vdma.c
* Author: srinivasa attili
* Date:	  7-sept-2011
*
* Purpose:	This module handles the Performance Monitor that measures
*   throughput on DDR AXI MM interface.
*   Compliant to Xilinx Standalone BSP / XilKernel BSP.
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
#include "xmk.h"
#include "sys/intr.h"
#include <stdio.h>
#include "xparameters.h"
#include "xintc.h"
#include "xil_exception.h"
#include "board_test_app.h"
#include "vdma_data.h"

/************************** Constant Definitions *****************************/
#undef	PERFMON_HWINTR_AVAILABLE

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

// Register definitions
#define PERFMON_BEGIN					XPAR_AXIPMON_0_BASEADDR	// 0x40000000	// 0x40030000
#define PERFMON_CONTROL_REG				(PERFMON_BEGIN+0x00)
#define PERFMON_STATUS_REG				(PERFMON_BEGIN+0x04)
#define PERFMON_RD_DATA_L_REG			(PERFMON_BEGIN+0x08)	// read  throughput bits 31 - 0
#define PERFMON_RD_DATA_H_REG			(PERFMON_BEGIN+0x0C)	// read  throughput bits 64 - 32
#define PERFMON_WR_DATA_L_REG			(PERFMON_BEGIN+0x10)	// write throughput bits 31 - 0
#define PERFMON_WR_DATA_H_REG			(PERFMON_BEGIN+0x14)	// write throughput bits 64 - 32
#define PERF_MON_SAMPLE_RATE_REG		(PERFMON_BEGIN+0x18)

// bit fields for PERFMON_CONTROL_REG
#define	PERFMON_WRCOUNT_ENABLE_MASK		0x1
#define	PERFMON_RDCOUNT_ENABLE_MASK		0x2
#define	PERFMON_INTR_ENABLE_MASK		0x4

// bit fields for PERFMON_STATUS_REG
#define	PERFMON_DATA_READY_MASK			0x1
#define	PERFMON_INTR_STATUS_MASK		0x1
#define	PERFMON_INTR_CLEAR_MASK			(~PERFMON_INTR_STATUS_MASK & PERFMON_INTR_STATUS_MASK)

// bit fields for PERFMON_SAMPLE_RATE_REG
#define	PERFMON_TICK_NSEC				5			// tick duration is 5 nano seconds - 200 MHz
#define	PERFMON_SAMPLE_COUNT(ms)		( ((ms)/1000) * (1000000000/(PERFMON_TICK_NSEC)) )

#define PERFMON_READREG(addr, reg)		reg = *((volatile int *)(addr ));
#define	PERFMON_WRITEREG(addr, data)	*((volatile int *)(addr)) = data;

/************************** Function Prototypes ******************************/
// initialize performance monitor
int init_PerfMonitor(struct VDMA_PERF_MONITOR *perf);
int reset_PerfMonitor_stats(struct VDMA_PERF_MONITOR *perf);

// performance monitor configuration
int enable_PerfMonitor(struct VDMA_PERF_MONITOR *perf, int CntMode, int IntrMode);
int disable_PerfMonitor(struct VDMA_PERF_MONITOR *perf);
int ioctl_PerfMonitor(struct VDMA_PERF_MONITOR *perf, int Cmd, int Data);

// performance monitor data handling
void PerfMon_pollHandler(struct VDMA_PERF_MONITOR *perf);
void PerfMon_IntrHandler(void *perf);

/************************** Variable Definitions *****************************/

/*****************************************************************************/
/**
* initialize the performance monitor
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int init_PerfMonitor(struct VDMA_PERF_MONITOR *perf)
{
	perf->pollmode = 1;		// default Poll mode
	disable_PerfMonitor(perf);
	ioctl_PerfMonitor(perf, PERFMON_IOCTL_SET_SAMPLERATE, 1000);
	enable_PerfMonitor(perf, PERFMON_RDWR_COUNT, PERFMON_INTR_MODE);

	return 0;
}

void stop_PerfMonitor(void)
{
	PERFMON_WRITEREG(PERFMON_CONTROL_REG, 0);
}

void start_PerfMonitor(void)
{
	unsigned int reg = 0;

	PERFMON_READREG(PERFMON_CONTROL_REG, reg);
	reg |= (PERFMON_WRCOUNT_ENABLE_MASK| PERFMON_RDCOUNT_ENABLE_MASK);
	PERFMON_WRITEREG(PERFMON_CONTROL_REG, reg);
	PERFMON_WRITEREG(PERFMON_STATUS_REG, PERFMON_INTR_CLEAR_MASK);
}

int reset_PerfMonitor_stats(struct VDMA_PERF_MONITOR *perf)
{
	perf->read_perf = 0;
	perf->write_perf = 0;
	perf->perfIntrCnt = 0;

	return 0;
}

/*****************************************************************************/
/**
* Main function to INITIALIZE Performance Monitor.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int enable_PerfMonitor(struct VDMA_PERF_MONITOR *perf, int CntMode, int IntrMode)
{
	unsigned int reg = 0;
	#ifdef PERFMON_HWINTR_AVAILABLE
	int Status;
	#endif

	// performance monitor select data collection
	if(CntMode & PERFMON_RD_COUNT)
		reg |= PERFMON_RDCOUNT_ENABLE_MASK;
	if(CntMode & PERFMON_WR_COUNT)
		reg |= PERFMON_WRCOUNT_ENABLE_MASK;
	if(CntMode & PERFMON_RDWR_COUNT)
		reg |= (PERFMON_WRCOUNT_ENABLE_MASK| PERFMON_RDCOUNT_ENABLE_MASK);
	reg &= ~(PERFMON_INTR_ENABLE_MASK);		// disable interrupt
	PERFMON_WRITEREG(PERFMON_CONTROL_REG, reg);
	PERFMON_WRITEREG(PERFMON_STATUS_REG, PERFMON_INTR_CLEAR_MASK);

	perf->pollmode = 1;		// default Poll mode

	#ifdef PERFMON_HWINTR_AVAILABLE
	if(IntrMode == PERFMON_INTR_MODE)
	{
		// HOOK THE ISR FOR PERFORMANCE INTERRUPT
		// Step1: Enable Interrupt Controller (if already not done).

		Status = XIntc_Initialize(&InterruptController, XPAR_INTC_0_DEVICE_ID);
		if ( (Status != XST_SUCCESS) && (Status != XST_DEVICE_IS_STARTED) ) {
			xil_printf("  XIntc_Initialize Failed or Already started !! \r\n");
			return XST_FAILURE;
		}
		#if 1	// standalone
		// Step2: Link interrupt handler and Performance Monitor's Interrupt Line
		Status = XIntc_Connect(&InterruptController, PERFMON_INTR_ID,
					(XInterruptHandler) PerfMon_IntrHandler, (void*)perf);
		if (Status != XST_SUCCESS) {
			xil_printf("  XIntc_Connect Failed, rd_intrid=%x\r\n", PERFMON_INTR_ID);
			return XST_FAILURE;
		}
		#else	// xilkernel
		register_int_handler(PERFMON_INTR_ID,
				(XInterruptHandler) PerfMon_IntrHandler, perf);
		#endif

		// ENABLE PERFORMANCE INTERRUPT IN HARDWARE
		PERFMON_READREG(PERFMON_CONTROL_REG, reg);
		reg |= PERFMON_INTR_ENABLE_MASK;
		PERFMON_WRITEREG(PERFMON_CONTROL_REG, reg);
		PERFMON_WRITEREG(PERFMON_STATUS_REG, PERFMON_INTR_CLEAR_MASK);
		perf->pollmode = 0;

		/* Step3: Enable performance monitor interrupt */
		#if 1	// standalone
		XIntc_Enable(&InterruptController, PERFMON_INTR_ID);
		#else	// xilkernel
		enable_interrupt(PERFMON_INTR_ID);
		#endif

		#if 1	// standalone
		// Step4: Activate interrupt controller
		Status = XIntc_Start(&InterruptController, XIN_REAL_MODE);
		if (Status != XST_SUCCESS) {
			xil_printf("  XIntc_Start Failed\r\n");
			return XST_FAILURE;
		}
		// Enable interrupts from the hardware
		Xil_ExceptionInit();
		Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XIntc_InterruptHandler,
				(void *)&InterruptController);
		Xil_ExceptionEnable();
		#endif
	}
	#endif	// PERFMON_HWINTR_AVAILABLE

	return XST_SUCCESS;
}

int disable_PerfMonitor(struct VDMA_PERF_MONITOR *perf)
{
	// disable hardware
	PERFMON_WRITEREG(PERFMON_CONTROL_REG, 0);

	// Disable performance monitor data collect
	if(perf->pollmode == 0)
	{
		#ifdef PERFMON_HWINTR_AVAILABLE
		#if 1	// standalone
				XIntc_Disable(&InterruptController, PERFMON_INTR_ID);
				XIntc_Disconnect(&InterruptController, PERFMON_INTR_ID);
		#else	// xilkernel
		disble_interrupt(PERFMON_INTR_ID);
		#endif
		#endif
	}
	perf->pollmode = 1;
	perf->perfIntrCnt = 0;

	return XST_SUCCESS;
}


int ioctl_PerfMonitor(struct VDMA_PERF_MONITOR *perf, int Cmd, int Data)
{
	unsigned int data=0;
	unsigned int ret = XST_SUCCESS;

	switch(Cmd)
	{
	case PERFMON_IOCTL_SET_SAMPLERATE:
		data = PERFMON_SAMPLE_COUNT(Data);
		PERFMON_WRITEREG(PERF_MON_SAMPLE_RATE_REG, data);
		break;

	case PERFMON_IOCTL_GET_SAMPLERATE:
		PERFMON_READREG(PERF_MON_SAMPLE_RATE_REG, ret);
		break;

	default:
		break;
	}

	xil_printf("Value into sample rate: 0x%x \r\n", data);

	return ret;
}

/*****************************************************************************/
/**
* Interrupt handler from Performance Monitor hardware
* This interrupt occurs whenever performance data is collected in hardware
* (appox. every 1 second)
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
void PerfMon_IntrHandler(void * perf)
{
	PerfMon_pollHandler(perf);		// remove this another level of function call.
}

/*****************************************************************************/
/**
* Get the Performance reading on DDR AXI MM.
* The DMA activities will demonstrate the bandwidth usage.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
void PerfMon_pollHandler(struct VDMA_PERF_MONITOR *perf)
{
	volatile unsigned int reg;
	volatile unsigned wr_perf_L=0, wr_perf_H=0;
	volatile unsigned rd_perf_L=0, rd_perf_H=0;
	double p;
	double one_mb = (double)(1024*1024);

	// Incase of Poll mode, we need to check the Per Data availability (Status reg)
	// Incase of Intr mode, we reach this point only if Per Data is ready in HW

	if(perf->pollmode == 1)
	{
		// Performance Monitor Status register
		PERFMON_READREG(PERFMON_CONTROL_REG, reg);
		if( (reg & 0x1) != 0x1)
			return;
	}

	PERFMON_READREG(PERFMON_WR_DATA_L_REG, wr_perf_L);
	PERFMON_READREG(PERFMON_WR_DATA_H_REG, wr_perf_H);	// ignore for now
	PERFMON_READREG(PERFMON_RD_DATA_L_REG, rd_perf_L);
	PERFMON_READREG(PERFMON_RD_DATA_H_REG, rd_perf_H);	// ignore for now

	#if 0
	wr_perf_L = (wr_perf_L >> 20);		// wr_perf /= (1024*1024);
	wr_perf_L = (wr_perf_L << 9);		// wr_perf *= 512;
	wr_perf_L = (wr_perf_L >> 10);		// wr_perf /= 1024;
	perf->write_perf = wr_perf_L;
	#else
	//p = (double) ((wr_perf_L/(1024*1024))*512)/1024;
	p = (double)wr_perf_L/one_mb;
	p = (double)p* (double)512;
	p = (double) (p / 1024);
	perf->write_perf = p;
	#endif

	#if 0
	rd_perf_L = (rd_perf_L >> 20);		// rd_perf /= (1024*1024);
	rd_perf_L = (rd_perf_L << 9);		// rd_perf *= 512;
	rd_perf_L = (rd_perf_L >> 10);		// rd_perf /= 1024;
	perf->read_perf = rd_perf_L;
	#else
	//p = (double) ((rd_perf_L/(1024*1024))*512)/1024;
	p = (double)rd_perf_L/one_mb;
	p = (double)p* (double)512;
	p = (double) (p / 1024);
	perf->read_perf = p;
	#endif

	//printf("Rd: 0x%x %f, Wr: 0x%x %f\n",
	//		rd_perf_L, perf->read_perf, wr_perf_L, perf->write_perf);

	perf->perfIntrCnt++;
	perf->dataReady = 1;

	// clear interrupt status
	PERFMON_WRITEREG(PERFMON_STATUS_REG, PERFMON_INTR_CLEAR_MASK);
	PERFMON_WRITEREG(PERFMON_STATUS_REG, PERFMON_INTR_CLEAR_MASK);

	return;
}
#endif
