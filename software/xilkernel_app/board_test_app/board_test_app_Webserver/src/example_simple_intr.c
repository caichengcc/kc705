/******************************************************************************
*
* (c) Copyright 2010 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
******************************************************************************/
/*****************************************************************************/
/**
 *
 * @file example_simple_intr.c
 *
 * This file demonstrates how to use the xaxicdma driver on the Xilinx AXI
 * CDMA core (AXICDMA) to transfer packets in simple transfer mode through
 * interrupt.
 *
 * Modify the NUMBER_OF_TRANSFER constant to have different number of simple
 * transfers done in this test.
 *
 * This example assumes that the system has an interrupt controller.
 *
 * To see the debug print, you need a Uart16550 or uartlite in your system,
 * and please set "-DDEBUG" in your compiler options for the example, also
 * comment out the "#undef DEBUG" in xdebug.h. You need to rebuild your
 * software executable.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.00a jz   07/30/10 First release
 * </pre>
 *
 * modified on 2,Feb,2012 srinivasa attili; modified for Kintex7 Demo
 *
 ****************************************************************************/
//#include "xaxicdma.h"
//#include "xaxicdma_i.h"
#include "xdebug.h"
#include "xil_exception.h"
#include "xintc.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "stdlib.h"
//#include "perf_axi.h"
#include "stdio.h"
#include "sys/intr.h"
#include "cdma_intr.h"

#if (!defined(DEBUG))
extern int printf(const char *format, ...);
#endif

/******************** Constant Definitions **********************************/

#define SYSTEM_PERIOD		6.600	/* Period of AXI Master in NS */
#define INTC_DEVICE_ID     	XPAR_INTC_0_DEVICE_ID
#define DDR_BASE_ADDR       (XPAR_DDR3_SDRAM_S_AXI_BASEADDR)



#define BUFFER_BYTESIZE 1152000 	/* Length of the buffers for DMA transfer */

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/* Struct instance for DMA block(s) */
struct dma_chan_parms {

	u32 Dma_Base_Addr;
	u32 Perf_AXI_Base_Addr;
	u32 DMA_DEVICE_ID;
	u32 DMA_CTRL_IRPT_INTR;
	u32 SrcBuffer;
	u32 DestBuffer;
//	XAxiCdma_Config *CfgPtr;
//    XAxiCdma Engine;
};


/* Shared variables used to test the callbacks.
 */
volatile static int Done = 0;    /* Dma transfer is done */
volatile static int Error = 0;   /* Dma Bus Error occurs */

/*****************************************************************************/
/*
* Callback function for the simple transfer. It is called by the driver's
* interrupt handler.
*
* @param   CallBackRef is the reference pointer registered through transfer
*          submission. In this case, it is the pointer to the driver instance
* @param   IrqMask is the interrupt mask the driver interrupt handler passes
*          to the callback function.
* @param   IgnorePtr is a pointer that is ignored by simple callback function
*
* @return
* None
*
******************************************************************************/
#if 0
static void Example_CallBack(void *CallBackRef, u32 IrqMask, int *IgnorePtr)
{
	if (IrqMask & XAXICDMA_XR_IRQ_ERROR_MASK) {
		Error = TRUE;
	}

	if (IrqMask & XAXICDMA_XR_IRQ_IOC_MASK) {
		Done = TRUE;
	}
	return;
}
#endif

/*****************************************************************************/
/*
 * This function does one simple transfer submission.  This function is
 * included with the XAxiCdma driver, but is slightly modified for this
 * example.
 *
 * @param InstancePtr is the pointer to the driver instance
 * @param SrcAddr is the address of the source buffer
 * @param DstAddr is the address of the destination buffer
 * @param Length is the length of the transfer
 * @param SimpleCallBack is the callback function for the simple transfer
 * @param CallBackRef is the callback reference pointer
 *
 * @return
 * - XST_SUCCESS for success of submission
 * - XST_FAILURE for submission failure, maybe caused by:
 *   . Another simple transfer is still going
 *   . Another SG transfer is still going
 * - XST_INVALID_PARAM if:
 *   . Length out of valid range [1:8M]
 *   . Or, address not aligned when DRE is not built in
 *
 * @note
 * Only set the callback function if using interrupt to signal the completion.
 * If used in polling mode, please set the callback function to be NULL.
*
******************************************************************************/
#if 0
int XAxiCdma_SimpleTransfer_2(XAxiCdma *InstancePtr, u32 SrcAddr, u32 DstAddr,
	int Length, XAxiCdma_CallBackFn SimpleCallBack, void *CallBackRef)
{
	u32 WordBits;

	if ((Length < 1) || (Length > XAXICDMA_MAX_TRANSFER_LEN)) {
		return XST_INVALID_PARAM;
	}

	WordBits = (u32)(InstancePtr->WordLength - 1);

	if ((SrcAddr & WordBits) || (DstAddr & WordBits)) {

		if (!InstancePtr->HasDRE) {
			xdbg_printf(XDBG_DEBUG_ERROR,
			    "Unaligned transfer without DRE %x/%x\n\r",
			    SrcAddr, DstAddr);

			return XST_INVALID_PARAM;
		}
	}

	/* If the engine is doing transfer, cannot submit
	 */
	if (XAxiCdma_IsBusy(InstancePtr)) {
		xdbg_printf(XDBG_DEBUG_ERROR, "Engine is busy\n\r");

		return XST_FAILURE;
	}

	/* The driver is still handling the previous simple transfer
	 */
	if (InstancePtr->SimpleNotDone) {
		xdbg_printf(XDBG_DEBUG_ERROR, "Simple ongoing\n\r");

		return XST_FAILURE;
	}

	/* If the engine is in scatter gather mode, try switch to simple mode
	 */
	if (!XAxiCdma_IsSimpleMode(InstancePtr)) {

		if (XAxiCdma_SwitchMode(InstancePtr, XAXICDMA_SIMPLE_MODE) !=
		    XST_SUCCESS) {

			xdbg_printf(XDBG_DEBUG_ERROR,
			    "Cannot switch to simple mode\n\r");

			return XST_FAILURE;
		}
	}

	/* Setup the flag so that others will not step on us
	 *
	 * This flag is only set if callback function is used and if the
	 * system is in interrupt mode; otherwise, when the hardware is done
	 * with the transfer, the driver is done with the transfer
	 */
	if ((SimpleCallBack != NULL) ||
	       ((XAxiCdma_IntrGetEnabled(InstancePtr) &
	        XAXICDMA_XR_IRQ_SIMPLE_ALL_MASK) != 0x0)) {
		InstancePtr->SimpleNotDone = 1;
	}

	InstancePtr->SimpleCallBackFn = SimpleCallBack;
	InstancePtr->SimpleCallBackRef = CallBackRef;

	XAxiCdma_WriteReg(InstancePtr->BaseAddr, XAXICDMA_SRCADDR_OFFSET,
		SrcAddr);
	XAxiCdma_WriteReg(InstancePtr->BaseAddr, XAXICDMA_DSTADDR_OFFSET,
		DstAddr);

	return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
* The example to do the simple transfer through interrupt.
*
* @param   resultbuf is a pointer to the buffer to store the results
* @param   times is the number of times to run the example
*
* @return
* - XST_SUCCESS if example finishes successfully
* - XST_FAILURE if error occurs
*
* @note
* If the hardware build has problems with interrupt, then this function hangs
*
******************************************************************************/
int SimpleIntrExample(double *resultbuf, int times)
{
#if 0
	int Status;
	/* Array of structs depend on number of DMAS */
	struct dma_chan_parms dma_struct[NUMBER_OF_DMAS];
	u8  *SrcPtr;
	u8  *DestPtr;
	Done = 0;
	Error = 0;
	int n;
	u32 Index;
	double result;
	int i;


#if (NUMBER_OF_DMAS == 1)

	/* User Must Set Values For Each Struct */
	dma_struct[0].Dma_Base_Addr = XPAR_AXICDMA_0_BASEADDR;
	/* Base Address of the PERF DMA instance */
	dma_struct[0].Perf_AXI_Base_Addr = XPAR_PERF_AXI_CENTRAL_DMA_BASEADDR;
	/* Device ID */
	dma_struct[0].DMA_DEVICE_ID = XPAR_AXICDMA_0_DEVICE_ID;
	/* Interrupts of DMA block */
	dma_struct[0].DMA_CTRL_IRPT_INTR = XPAR_INTC_0_AXICDMA_0_VEC_ID;
	dma_struct[0].SrcBuffer =
		 (DDR_BASE_ADDR + 0x04000000);
	dma_struct[0].DestBuffer =
		 (DDR_BASE_ADDR + 0x05000000);
#endif


	for (n = 0; n < NUMBER_OF_DMAS; n++) {

		/* Initialize the XAxiCdma device.
		 */
		dma_struct[n].CfgPtr = XAxiCdma_LookupConfig(dma_struct[n].DMA_DEVICE_ID);
		if (!dma_struct[n].CfgPtr) {
			xdbg_printf(XDBG_DEBUG_ERROR,
			    "Cannot find config structure for device %d\n\r",
				XPAR_AXICDMA_0_DEVICE_ID);
			return XST_FAILURE;
		}

		Status = XAxiCdma_CfgInitialize(&dma_struct[n].Engine, dma_struct[n].CfgPtr, dma_struct[n].Dma_Base_Addr);

		if (Status != XST_SUCCESS) {
			xdbg_printf(XDBG_DEBUG_ERROR,
			    "Initialization failed with %d\n\r", Status);
			return XST_FAILURE;
		}
	}

	/* Setup the interrupt system
	 */
	for (n = 0; n < NUMBER_OF_DMAS; n++) {
		Status = register_int_handler(dma_struct[n].DMA_CTRL_IRPT_INTR, XAxiCdma_IntrHandler, &dma_struct[n].Engine);
		if (Status != XST_SUCCESS) {
			xil_printf("Unable to register interrupts\r\n");
		}
		enable_interrupt(dma_struct[n].DMA_CTRL_IRPT_INTR);
	}


	for(i=0; i<times; i++)
	{
		for (n = 0; n < NUMBER_OF_DMAS; n++) {
			/* Enable all (completion/error/delay) interrupts
			 */
			XAxiCdma_IntrEnable(&dma_struct[n].Engine, XAXICDMA_XR_IRQ_ALL_MASK);
		}

		for (n = 0; n < NUMBER_OF_DMAS; n++) {
			/* Initialize the source buffer bytes with a pattern and the
			 * the destination buffer bytes to zero
			 */
			SrcPtr = (u8 *)dma_struct[n].SrcBuffer ;
			DestPtr = (u8 *)dma_struct[n].DestBuffer ;
			for (Index = 0; Index < BUFFER_BYTESIZE; Index++) {
				SrcPtr[Index] = Index & 0xFF;
				DestPtr[Index] = 0;
			}

			/* Flush the SrcBuffer before the DMA transfer, in case the Data Cache
			 * is enabled
			 */
			Xil_DCacheFlushRange((u32)dma_struct[n].SrcBuffer, BUFFER_BYTESIZE);

			/* Invalidate the DestBuffer before receiving the data, in case the Data
			 * Cache is enabled
			 */
			Xil_DCacheInvalidateRange((u32)dma_struct[n].DestBuffer, BUFFER_BYTESIZE);

			Setup_PERF_AXI(dma_struct[n].Perf_AXI_Base_Addr);
		}

		for (n = 0; n < NUMBER_OF_DMAS; n++) {

			/* Try to start the DMA transfer
			 */
			Status = XAxiCdma_SimpleTransfer_2(&dma_struct[n].Engine, (u32)dma_struct[n].SrcBuffer,
			      (u32)dma_struct[n].DestBuffer, BUFFER_BYTESIZE, Example_CallBack,
			      (void *)&dma_struct[n].Engine);
		}

		for (n = 0; n < NUMBER_OF_DMAS; n++) {
			XAxiCdma_WriteReg(dma_struct[n].Dma_Base_Addr, XAXICDMA_BTT_OFFSET,
				BUFFER_BYTESIZE);
		}

		for (n = 0; n < NUMBER_OF_DMAS; n++) {
			/* Poll last PERF PLBV46 instance status register for done bit */
			Poll_Done_PERF_AXI(dma_struct[NUMBER_OF_DMAS - 1].Perf_AXI_Base_Addr);
		}

		for (n = 0; n < NUMBER_OF_DMAS; n++) {
			/* Transfer completes successfully, check data
			 *
			 * Compare the contents of destination buffer and source buffer
			 */
			for (Index = 0; Index < BUFFER_BYTESIZE; Index++) {
				if ( DestPtr[Index] != SrcPtr[Index]) {
					xdbg_printf(XDBG_DEBUG_ERROR,
					   "Data check failure %x/%x\n\r",
						DestPtr[Index], SrcPtr[Index]);
					return XST_FAILURE;
				}
			}
		}

		for (n = 0; n < NUMBER_OF_DMAS; n++) {
			result = PERF_AXI_Transfer(dma_struct[n].Perf_AXI_Base_Addr,SYSTEM_PERIOD,BUFFER_BYTESIZE);
//	    	printf("CDMA%d  %f\r\n", n, result);
			resultbuf[i+n] = result;
		}

		for (n = 0; n < NUMBER_OF_DMAS; n++) {
			PERF_AXI_mReset(dma_struct[n].Perf_AXI_Base_Addr);
			XAxiCdma_Reset(&dma_struct[n].Engine);
		}
	}

	/* Test finishes successfully, clean up and return
	 */
	for (n = 0; n < NUMBER_OF_DMAS; n++) {
		PERF_AXI_mReset(dma_struct[n].Perf_AXI_Base_Addr);
		XAxiCdma_Reset(&dma_struct[n].Engine);
		disable_interrupt(dma_struct[n].DMA_CTRL_IRPT_INTR);
	}
#endif
	return XST_SUCCESS;
}


