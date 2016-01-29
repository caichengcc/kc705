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
* Purpose:	Configure the VDMA instances that needed for Video Demo application.
*          Compliant to Xilinx Standalone BSP / XilKernel BSP.
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
#include <sys/intr.h>
#include <sys/timer.h>
#include "lwip/sys.h"
#include <sys/stats.h>
#include <sys/process.h>
#include <pthread.h>
#include "stdio.h"
#include "xaxivdma.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xintc.h"

#include "board_test_app.h"
#include "config_apps.h"
#include "vdma_data.h"

/************************** Global Variables *****************************/
static int ReadDone;
static int ReadError;
static int WriteDone;
static int WriteError;

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define RETRY_COUNT	3		// VDMA channel configuration error recover trials

// Fill common data for Write Frame, every time a VDMA instance is used
#define VDMA_FILL_WRDATA		\
		vdma->WriteCfg.Stride        = 0x2000;		\
		vdma->WriteCfg.FrameDelay    = 0;			\
		vdma->WriteCfg.EnableSync    = 0;			\
		vdma->WriteCfg.GenLockRepeat = 0;			\
		vdma->WriteCfg.PointNum      = 0;			\
		vdma->WriteCfg.EnableCircularBuf  = 1;		\
		vdma->WriteCfg.EnableFrameCounter = 0;		\
		vdma->WriteCfg.FixedFrameStoreAddr= 0;		\
		vdma->WriteIntrCnt = 0;						\
		vdma->WriteErrIntrCnt = 0;					\
		vdma->FrameCfg.ReadFrameCount      = NUMBER_OF_READ_FRAMES;		\
		vdma->FrameCfg.WriteFrameCount     = NUMBER_OF_WRITE_FRAMES;	\
		vdma->FrameCfg.ReadDelayTimerCount = DELAY_TIMER_COUNTER;		\
		vdma->FrameCfg.WriteDelayTimerCount= DELAY_TIMER_COUNTER;

// Fill common data for Read Frame, every time a VDMA instance is used
#define VDMA_FILL_RDDATA		\
		vdma->ReadCfg.Stride        = 0x2000;		\
		vdma->ReadCfg.FrameDelay    = 0;			\
		vdma->ReadCfg.EnableSync    = 0;			\
		vdma->ReadCfg.GenLockRepeat = 0;			\
		vdma->ReadCfg.PointNum      = 0;			\
		vdma->ReadCfg.EnableCircularBuf  = 1;		\
		vdma->ReadCfg.EnableFrameCounter = 0;		\
		vdma->ReadCfg.FixedFrameStoreAddr= 0;		\
		vdma->ReadIntrCnt = 0;						\
		vdma->ReadErrIntrCnt = 0;

#define VDMA_READREG(addr, reg)		reg = *((volatile int *)(addr ));
#define	VDMA_WRITEREG(addr, data)	*((volatile int *)(addr)) = data;

/************************** Function Prototypes ******************************/
int init_VDMA(void);
int reset_VDMA(void);
int config_vdma_fullVideo_0(void);
int config_vdma_fullVideo_1(void);
int config_vdma_scaleVideo_0(void);
int config_vdma_scaleVideo_1(void);
int config_vdma_scaleVideo_both(void);
int config_vdma_tpgVideo_0(void);
int config_vdma_tpgVideo_1(void);

// Internal helper functions
static int init_vdma_instance(int id);
static int reset_vdma_instance(XAxiVdma *InstancePtr, int direction);
static int config_vdma_instance(int id, int access);
static int check_config_vdma(int id, int direction);
static int get_currentFrameNumber_vdma(int id, int direction);
static int config_vdma_IntrSystem(struct VDMA_INSTANCE *vdma, int ReadIntrId, int WriteIntrId);
static void reset_vdma_statistics(struct VDMA_INSTANCE *vdma);

// Interrupt Handlers (common event handlers for all instances, instance pointer differentiates)
static void vdma_ReadCallBack(void *CallbackRef, u32 Mask);
static void vdma_ReadErrorCallBack(void *CallbackRef, u32 Mask);
static void vdma_WriteCallBack(void *CallbackRef, u32 Mask);
static void vdma_WriteErrorCallBack(void *CallbackRef, u32 Mask);
static int reconfig_VDMA_instance(struct VDMA_INSTANCE *InstancePtr, int direction);

/************************** Variable Definitions *****************************/
struct VDMA_INSTANCE VdmaInstance[MAX_VDMA_INSTANCE];
XIntc InterruptController;	/* The instance of the Interrupt Controller. */


/*****************************************************************************/
/**
* INITIALIZE all VDMA channels for the Video Demo.
*   - create the VDMA instances for:
*     DVI_0, DVI_1, SCALER_0, SCALER_2, TPG_0, TPG_2
*   - initialize the data elements
*   - Initialize interrupt controller
*   - setup interrupt handling for each of the VDMA instances
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		: at the end of this function, all VDMA instances are ready
*             to transfer data
*
******************************************************************************/
int init_VDMA(void)
{
	int i;
	int Status;

	/***********************************************************/
	/***********  INITIALIZE VDMA INSTANCES  *******************/
	/***********************************************************/
	// DVI_0
	i = VDMA_INSTANCE_DVI_0;
	VdmaInstance[i].deviceid    = XPAR_V_IN_PIPE_1_DVI_IN_0_VDMA_DEVICE_ID;
	VdmaInstance[i].rd_intrid   = -1;
	VdmaInstance[i].wr_intrid   = XPAR_MICROBLAZE_SUBSYSTEM_INTERRUPT_CNTLR_V_IN_PIPE_1_DVI_IN_0_VDMA_S2MM_INTROUT_INTR;
	VdmaInstance[i].access_type = VDMA_WR_ONLY;

	// DVI_1
	i = VDMA_INSTANCE_DVI_1;
	VdmaInstance[i].deviceid    = XPAR_V_IN_PIPE_2_DVI_IN_1_VDMA_DEVICE_ID;
	VdmaInstance[i].rd_intrid   = -1;
	VdmaInstance[i].wr_intrid   = XPAR_MICROBLAZE_SUBSYSTEM_INTERRUPT_CNTLR_V_IN_PIPE_2_DVI_IN_1_VDMA_S2MM_INTROUT_INTR;
	VdmaInstance[i].access_type = VDMA_WR_ONLY;

	// SCALER_0
	i = VDMA_INSTANCE_SCALER_0;
	VdmaInstance[i].deviceid    = XPAR_MEM_SCALER_1_SCALER_0_VDMA_DEVICE_ID;
	VdmaInstance[i].rd_intrid   = XPAR_MICROBLAZE_SUBSYSTEM_INTERRUPT_CNTLR_MEM_SCALER_1_SCALER_0_VDMA_S2MM_INTROUT_INTR;
	VdmaInstance[i].wr_intrid   = XPAR_MICROBLAZE_SUBSYSTEM_INTERRUPT_CNTLR_MEM_SCALER_1_SCALER_0_VDMA_MM2S_INTROUT_INTR;
	VdmaInstance[i].access_type = VDMA_RD_WR;

	// SCALER_2
	i = VDMA_INSTANCE_SCALER_2;
	VdmaInstance[i].deviceid    = XPAR_MEM_SCALER_2_SCALER_2_VDMA_DEVICE_ID;
	VdmaInstance[i].rd_intrid   = XPAR_MICROBLAZE_SUBSYSTEM_INTERRUPT_CNTLR_MEM_SCALER_2_SCALER_2_VDMA_MM2S_INTROUT_INTR;
	VdmaInstance[i].wr_intrid   = XPAR_MICROBLAZE_SUBSYSTEM_INTERRUPT_CNTLR_MEM_SCALER_2_SCALER_2_VDMA_S2MM_INTROUT_INTR;
	VdmaInstance[i].access_type = VDMA_RD_WR;

	// TPG_0
	i = VDMA_INSTANCE_TPG_0;
	VdmaInstance[i].deviceid    = XPAR_INLINE_SCALER_1_SCALER_1_VDMA_DEVICE_ID;
	VdmaInstance[i].rd_intrid   = -1;
	VdmaInstance[i].wr_intrid   = XPAR_MICROBLAZE_SUBSYSTEM_INTERRUPT_CNTLR_INLINE_SCALER_1_SCALER_1_VDMA_S2MM_INTROUT_INTR;
	VdmaInstance[i].access_type = VDMA_WR_ONLY;

	// TPG_2
	i = VDMA_INSTANCE_TPG_2;
	VdmaInstance[i].deviceid    = XPAR_INLINE_SCALER_2_SCALER_3_VDMA_DEVICE_ID;
	VdmaInstance[i].rd_intrid   = -1;
	VdmaInstance[i].wr_intrid   = XPAR_MICROBLAZE_SUBSYSTEM_INTERRUPT_CNTLR_INLINE_SCALER_2_SCALER_3_VDMA_S2MM_INTROUT_INTR;
	VdmaInstance[i].access_type = VDMA_WR_ONLY;

	/*************************************************************/
	/***********  SETUP  INTERRUPT CONTROLLER  *******************/
	/*************************************************************/
	#if 0
	Status = XIntc_Initialize(&InterruptController, XPAR_INTC_0_DEVICE_ID);
	if ( (Status != XST_SUCCESS) && (Status != XST_DEVICE_IS_STARTED) ) {
		xil_printf("  XIntc_Initialize Failed or Already started!! \r\n");
		return XST_FAILURE;
	}
	#endif

	/*************************************************************/
	/**************  SETUP  VDMA INSTANCES  **********************/
	/*************************************************************/
	for(i=0; i<MAX_VDMA_INSTANCE; i++)
	{
		Status = init_vdma_instance(i);
		if (Status != XST_SUCCESS) {
			xil_printf("  init_vdma_instance %d Failed\r\n", i);
			return XST_FAILURE;
		}

		//xil_printf("setup interrupts for id=%x\r\n", i);
		Status = config_vdma_IntrSystem(&VdmaInstance[i],
				VdmaInstance[i].rd_intrid, VdmaInstance[i].wr_intrid);
		if (Status != XST_SUCCESS) {
			xil_printf("  init_vdma_instance %d Failed\r\n", i);
			return XST_FAILURE;
		}
	}

	/*************************************************************/
	/**************  SETUP VDMA INTERRUPTS  **********************/
	/*************************************************************/
	#if 0
	// Activate interrupt controller
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

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* function to initialize the GIVEN VDMA instance. This function does initial
* configuration, and set the default conditions for the channel.
* at the end of this call, the VDMA instance is ready for data transfer
*
* @param	id : the ID of VDMA channel to be initialized (refer vdma_data.h).
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		this function can be called in two occasions.
* 			1. at the beginning (start of application)
* 			2. during error condition of individual channel to reset
*
******************************************************************************/
static int init_vdma_instance(int id)
{
	XAxiVdma* AxiVdma               = &(VdmaInstance[id].AxiVdma);
	XAxiVdma_FrameCounter* FrameCfg = &(VdmaInstance[id].FrameCfg);
	XAxiVdma_Config* Config;
	int Status;

	/* The information of the XAxiVdma_Config comes from hardware build.
	 * The user IP should pass this information to the AXI DMA core.
	 */
	VdmaInstance[id].Config = XAxiVdma_LookupConfig(VdmaInstance[id].deviceid);
	if (!VdmaInstance[id].Config) {
		xil_printf("No video DMA found for ID %d\r\n", id);
		return XST_FAILURE;
	}
	Config = VdmaInstance[id].Config;

	/* Initialize DMA engine */
	Status = XAxiVdma_CfgInitialize(AxiVdma, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("Configuration Initialization failed %d\r\n", id);
		return XST_FAILURE;
	}

	#if 0
	if( (VdmaInstance[id].access_type & VDMA_WR_ONLY) == VDMA_WR_ONLY )
	{
		Status = XAxiVdma_SetFrmStore(AxiVdma, NUMBER_OF_WRITE_FRAMES, XAXIVDMA_WRITE);
		if (Status != XST_SUCCESS) {
			xil_printf("Setting Frame Store Number Failed in Write Channel %d\r\n", id);
			return XST_FAILURE;
		}
	}

	if( (VdmaInstance[id].access_type & VDMA_RD_ONLY) == VDMA_RD_ONLY )
	{
		Status = XAxiVdma_SetFrmStore(AxiVdma, NUMBER_OF_READ_FRAMES, XAXIVDMA_READ);
		if (Status != XST_SUCCESS) {
			xil_printf("Setting Frame Store Number Failed in Read Channel %d\r\n", id);
			return XST_FAILURE;
		}
	}
	#endif

	/* Setup frame counter and delay counter for both channels
	 *
	 * This is to monitor the progress of the test only
	 *
	 * WARNING: In free-run mode, interrupts may overwhelm the system.
	 * In that case, it is better to disable interrupts.
	 */
	FrameCfg->ReadFrameCount = NUMBER_OF_READ_FRAMES;
	FrameCfg->WriteFrameCount = NUMBER_OF_WRITE_FRAMES;
	FrameCfg->ReadDelayTimerCount = DELAY_TIMER_COUNTER;
	FrameCfg->WriteDelayTimerCount = DELAY_TIMER_COUNTER;

	#if 0
	Status = XAxiVdma_SetFrameCounter(AxiVdma, FrameCfg);
	if (Status != XST_SUCCESS) {
		xil_printf("Set frame counter failed %d\r\n", id);
		return XST_FAILURE;
	}
	#endif

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* function to setup the VDMA channels for INPUT VIDEO (stream #0 FULL) demo.
* this function configures all required VDMA channels for this demo.
* at the end of this call, the VDMA instances are ready for Full Video #0 demo
* FLOW:  input stream ==> DVI_IN_0 (S2MM) ==> SCALER_0 (MM2S)
*        SCALER_0 (S2MM)
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		We use the 3 VDMA channels for the Full Video (stream #0) demo
* 			DIV_IN_0 (S2MM)
* 			SCALER_0 (S2MM)
* 			SCALER_0 (MM2S)
*
******************************************************************************/
int config_vdma_fullVideo_0(void)
{
	struct VDMA_INSTANCE *vdma;

	/*********** DIV_IN_0 S2MM programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_DVI_0];
	VDMA_FILL_WRDATA;
	vdma->WriteIntrCnt = 0;
	vdma->WriteErrIntrCnt = 0;
	vdma->WriteCfg.VertSizeInput = 0x438;
	vdma->WriteCfg.HoriSizeInput = 0x1E00;
	vdma->WriteCfg.FrameStoreStartAddr[0] = (VMEM_BASE | 0x00000000);	//0xE0000000;
	vdma->WriteCfg.FrameStoreStartAddr[1] = (VMEM_BASE | 0x00870000);	//0xE0870000;
	vdma->WriteCfg.FrameStoreStartAddr[2] = (VMEM_BASE | 0x010E0000);	//0xE10E0000;
	config_vdma_instance(VDMA_INSTANCE_DVI_0, XAXIVDMA_WRITE);

	return XST_SUCCESS;
}

int check_config_vdma_fullVideo_0(void)
{
	int Status = XST_SUCCESS;

	/*********** DIV_IN_0 S2MM programming *****************/
	#if 1
	int retry;
	for(retry=0; retry < RETRY_COUNT; retry++)
	{
		Status = check_config_vdma(VDMA_INSTANCE_DVI_0, XAXIVDMA_WRITE);
		if(Status == XST_SUCCESS) break;
	}
	if(Status != XST_SUCCESS) return XST_FAILURE;
	#endif

	return Status;
}
int get_currentFrmaeNumber_fullVideo_0(void)
{
	int Status;

	/*********** DIV_IN_0 S2MM programming *****************/
	Status = get_currentFrameNumber_vdma(VDMA_INSTANCE_DVI_0, XAXIVDMA_WRITE);
	return Status;
}

/*****************************************************************************/
/**
* function to setup the VDMA channels for FULL VIDEO (stream #1) demo.
* this function configures all required VDMA channels for this demo.
* at the end of this call, the VDMA instances are ready for Full Video #1 demo
* FLOW:  input stream ==> DVI_IN_1 (S2MM) ==> SCALER_2 (MM2S)
*        SCALER_2 (S2MM)
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		We use the 3 VDMA channels for the Full Video (stream #1) demo
* 			DVI_IN_1 (S2MM)
*			SCALER_2 (S2MM)
*			SCALER_2 (MM2S)
******************************************************************************/
int config_vdma_fullVideo_1(void)
{
	struct VDMA_INSTANCE *vdma;

	/*********** DIV_IN_1 S2MM programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_DVI_1];
	VDMA_FILL_WRDATA;
	vdma->WriteCfg.VertSizeInput = 0x438;
	vdma->WriteCfg.HoriSizeInput = 0x1E00;
	vdma->WriteCfg.FrameStoreStartAddr[0] = (VMEM_BASE | 0x06540000);	//0xE6540000;
	vdma->WriteCfg.FrameStoreStartAddr[1] = (VMEM_BASE | 0x06DB0000);	//0xE6DB0000;
	vdma->WriteCfg.FrameStoreStartAddr[2] = (VMEM_BASE | 0x07620000);	//0xE7620000;
	config_vdma_instance(VDMA_INSTANCE_DVI_1, XAXIVDMA_WRITE);

	return XST_SUCCESS;
}

int check_config_vdma_fullVideo_1(void)
{
	int Status = XST_SUCCESS;

	/*********** DIV_IN_1 S2MM programming *****************/
	#if 1
	int retry;
	for(retry=0; retry < RETRY_COUNT; retry++)
	{
		Status = check_config_vdma(VDMA_INSTANCE_DVI_1, XAXIVDMA_WRITE);
		if(Status == XST_SUCCESS) break;
	}
	if(Status != XST_SUCCESS) return XST_FAILURE;
	#endif

	return Status;
}
int get_currentFrmaeNumber_fullVideo_1(void)
{
	int Status;

	/*********** DIV_IN_1 S2MM programming *****************/
	Status = get_currentFrameNumber_vdma(VDMA_INSTANCE_DVI_1, XAXIVDMA_WRITE);
	return Status;
}

/*****************************************************************************/
/**
* function to setup the VDMA channels for SCALED VIDEO (stream #0) demo.
* this function configures all required VDMA channels for this demo.
* at the end of this call, the VDMA instances are ready for Scaled Video #0 demo
* FLOW:  input stream ==> DVI_IN_0 (S2MM) ==> SCALER_0 (MM2S)
*        SCALER_0 (S2MM)
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		We use the 3 VDMA channels for the Scaled Video (stream #0) demo
* 			DVI_IN_0 (S2MM)
*			SCALER_0 (S2MM)
*			SCALER_0 (MM2S)
******************************************************************************/
int config_vdma_scaleVideo_0(void)
{
	//int Status;
	//int retry;
	struct VDMA_INSTANCE *vdma;

	/*********** DIV_IN_0 S2MM programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_DVI_0];
	VDMA_FILL_WRDATA;
	vdma->WriteCfg.VertSizeInput = 0x438;
	vdma->WriteCfg.HoriSizeInput = 0x1E00;
	vdma->WriteCfg.FrameStoreStartAddr[0] = (VMEM_BASE | 0x01950000);	//0xE1950000;
	vdma->WriteCfg.FrameStoreStartAddr[1] = (VMEM_BASE | 0x021C0000);	//0xE21C0000;
	vdma->WriteCfg.FrameStoreStartAddr[2] = (VMEM_BASE | 0x02A30000);	//0xE2A30000;
	config_vdma_instance(VDMA_INSTANCE_DVI_0, XAXIVDMA_WRITE);

	/*********** SCALER_0 S2MM programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_SCALER_0];
	VDMA_FILL_WRDATA;
	vdma->WriteCfg.VertSizeInput = 0x21C;
	vdma->WriteCfg.HoriSizeInput = 0xF00;
	vdma->WriteCfg.FrameStoreStartAddr[0] = (VMEM_BASE | 0x00000000);	//0xE0000000;
	vdma->WriteCfg.FrameStoreStartAddr[1] = (VMEM_BASE | 0x00870000);	//0xE0870000;
	vdma->WriteCfg.FrameStoreStartAddr[2] = (VMEM_BASE | 0x010E0000);	//0xE10E0000;
	config_vdma_instance(VDMA_INSTANCE_SCALER_0, XAXIVDMA_WRITE);

	/*********** SCALER_0 MM2S programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_SCALER_0];
	VDMA_FILL_RDDATA;
	vdma->ReadCfg.VertSizeInput = VdmaInstance[VDMA_INSTANCE_DVI_0].WriteCfg.VertSizeInput;
	vdma->ReadCfg.HoriSizeInput = VdmaInstance[VDMA_INSTANCE_DVI_0].WriteCfg.HoriSizeInput;
	vdma->ReadCfg.FrameStoreStartAddr[0] = VdmaInstance[VDMA_INSTANCE_DVI_0].WriteCfg.FrameStoreStartAddr[0];
	vdma->ReadCfg.FrameStoreStartAddr[1] = VdmaInstance[VDMA_INSTANCE_DVI_0].WriteCfg.FrameStoreStartAddr[1];
	vdma->ReadCfg.FrameStoreStartAddr[2] = VdmaInstance[VDMA_INSTANCE_DVI_0].WriteCfg.FrameStoreStartAddr[2];
	config_vdma_instance(VDMA_INSTANCE_SCALER_0, XAXIVDMA_READ);

	//return Status;
	return XST_SUCCESS;
}
int check_config_vdma_scaleVideo_0(void)
{
	int Status = XST_SUCCESS;

	#if 1
	int retry;
	for(retry=0; retry < RETRY_COUNT; retry++)
	{
		Status = check_config_vdma(VDMA_INSTANCE_SCALER_0, XAXIVDMA_WRITE);
		if(Status == XST_SUCCESS) break;
	}
	if(Status != XST_SUCCESS) return XST_FAILURE;
	#endif
	return Status;
}
int get_currentFrmaeNumber_scaleVideo_0(void)
{
	int Status;

	Status = get_currentFrmaeNumber_fullVideo_0();
	return Status;
}

/*****************************************************************************/
/**
* function to setup the VDMA channels for SCALED VIDEO (stream #1) demo.
* this function configures all required VDMA channels for this demo.
* at the end of this call, the VDMA instances are ready for Scaled Video #1 demo
* FLOW:  input stream ==> DVI_IN_1 (S2MM) ==> SCALER_2 (MM2S)
*        SCALER_2 (S2MM)
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		We use the 3 VDMA channels for the Scaled Video (stream #1) demo
* 			DVI_IN_1 (S2MM)
*			SCALER_2 (S2MM)
*			SCALER_2 (MM2S)
******************************************************************************/
int config_vdma_scaleVideo_1(void)
{
	//int Status;
	//int retry;
	struct VDMA_INSTANCE *vdma;

	/*********** DIV_IN_1 S2MM programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_DVI_1];
	VDMA_FILL_WRDATA;
	vdma->WriteCfg.VertSizeInput = 0x438;
	vdma->WriteCfg.HoriSizeInput = 0x1E00;
	vdma->WriteCfg.FrameStoreStartAddr[0] = (VMEM_BASE | 0x07E90000);	//0xE7E90000;
	vdma->WriteCfg.FrameStoreStartAddr[1] = (VMEM_BASE | 0x08700000);	//0xE8700000;
	vdma->WriteCfg.FrameStoreStartAddr[2] = (VMEM_BASE | 0x08F70000);	//0xE8F70000;
	config_vdma_instance(VDMA_INSTANCE_DVI_1, XAXIVDMA_WRITE);

	/*********** SCALER_2 S2MM programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_SCALER_2];
	VDMA_FILL_WRDATA;
	vdma->WriteCfg.VertSizeInput = 0x21C;
	vdma->WriteCfg.HoriSizeInput = 0xF00;
	vdma->WriteCfg.FrameStoreStartAddr[0] = (VMEM_BASE | 0x06540000);	//0xE6540000;
	vdma->WriteCfg.FrameStoreStartAddr[1] = (VMEM_BASE | 0x06DB0000);	//0xE6DB0000;
	vdma->WriteCfg.FrameStoreStartAddr[2] = (VMEM_BASE | 0x07620000);	//0xE7620000;
	config_vdma_instance(VDMA_INSTANCE_SCALER_2, XAXIVDMA_WRITE);

	/*********** SCALER_2 MM2S programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_SCALER_2];
	VDMA_FILL_RDDATA;
	vdma->ReadCfg.VertSizeInput = VdmaInstance[VDMA_INSTANCE_DVI_1].WriteCfg.VertSizeInput;
	vdma->ReadCfg.HoriSizeInput = VdmaInstance[VDMA_INSTANCE_DVI_1].WriteCfg.HoriSizeInput;
	vdma->ReadCfg.FrameStoreStartAddr[0] = VdmaInstance[VDMA_INSTANCE_DVI_1].WriteCfg.FrameStoreStartAddr[0];
	vdma->ReadCfg.FrameStoreStartAddr[1] = VdmaInstance[VDMA_INSTANCE_DVI_1].WriteCfg.FrameStoreStartAddr[1];
	vdma->ReadCfg.FrameStoreStartAddr[2] = VdmaInstance[VDMA_INSTANCE_DVI_1].WriteCfg.FrameStoreStartAddr[2];
	config_vdma_instance(VDMA_INSTANCE_SCALER_2, XAXIVDMA_READ);

	//return Status;
	return XST_SUCCESS;
}
int check_config_vdma_scaleVideo_1(void)
{
	int Status = XST_SUCCESS;

	#if 1
	int retry;
	for(retry=0; retry < RETRY_COUNT; retry++)
	{
		Status = check_config_vdma(VDMA_INSTANCE_SCALER_2, XAXIVDMA_WRITE);
		if(Status == XST_SUCCESS) break;
	}
	if(Status != XST_SUCCESS) return XST_FAILURE;
	#endif

	return Status;
}
int get_currentFrmaeNumber_scaleVideo_1(void)
{
	int Status;

	Status = get_currentFrmaeNumber_fullVideo_1();
	return Status;
}

/*****************************************************************************/
/**
* function to setup the VDMA channels for TPG VIDEO #0 demo.
* this function configures all required VDMA channels for this demo.
* at the end of this call, the VDMA instances are ready for TPG Video #0 demo
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		We use the 3 VDMA channels for the Full Video (stream #0) demo
* 			TPG_0 (S2MM)
******************************************************************************/
int config_vdma_tpgVideo_0(void)
{
	struct VDMA_INSTANCE *vdma;

	/*********** TPG_0 S2MM programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_TPG_0];
	VDMA_FILL_WRDATA;
	vdma->WriteCfg.VertSizeInput = 0x21C;
	vdma->WriteCfg.HoriSizeInput = 0xF00;
	vdma->WriteCfg.FrameStoreStartAddr[0] = (VMEM_BASE | 0x032A0000);	//0xE32A0000;
	vdma->WriteCfg.FrameStoreStartAddr[1] = (VMEM_BASE | 0x03B10000);	//0xE3B10000;
	vdma->WriteCfg.FrameStoreStartAddr[2] = (VMEM_BASE | 0x04380000);	//0xE4380000;
	config_vdma_instance(VDMA_INSTANCE_TPG_0, XAXIVDMA_WRITE);

	return XST_SUCCESS;
}
int check_config_vdma_tpgVideo_0(void)
{
	int Status= XST_SUCCESS;

	/*********** TPG_0 S2MM programming *****************/
	#if 1
	int retry;
	for(retry=0; retry < RETRY_COUNT; retry++)
	{
		Status = check_config_vdma(VDMA_INSTANCE_TPG_0, XAXIVDMA_WRITE);
		if(Status == XST_SUCCESS) break;
	}
	if(Status != XST_SUCCESS) return XST_FAILURE;
	#endif

	return Status;
}
int get_currentFrmaeNumber_tpgVideo_0(void)
{
	int Status;

	Status = get_currentFrameNumber_vdma(VDMA_INSTANCE_TPG_0, XAXIVDMA_WRITE);
	return Status;
}

/*****************************************************************************/
/**
* function to setup the VDMA channels for TPG VIDEO (stream #2) demo.
* this function configures all required VDMA channels for this demo.
* at the end of this call, the VDMA instances are ready for TPG Video #2 demo
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		We use the 3 VDMA channels for the TPG Video (stream #2) demo
* 			TPG_2 (S2MM)
******************************************************************************/
int config_vdma_tpgVideo_1(void)
{
	//int Status;
	//int retry;
	struct VDMA_INSTANCE *vdma;

	/*********** TPG_2 S2MM programming *****************/
	vdma = &VdmaInstance[VDMA_INSTANCE_TPG_2];
	VDMA_FILL_WRDATA;
	vdma->WriteCfg.VertSizeInput = 0x21C;
	vdma->WriteCfg.HoriSizeInput = 0xF00;
	vdma->WriteCfg.FrameStoreStartAddr[0] = (VMEM_BASE | 0x097E0000);	//0xE97E0000;
	vdma->WriteCfg.FrameStoreStartAddr[1] = (VMEM_BASE | 0x0A050000);	//0xEA050000;
	vdma->WriteCfg.FrameStoreStartAddr[2] = (VMEM_BASE | 0x0A8C0000);	//0xEA8C0000;
	config_vdma_instance(VDMA_INSTANCE_TPG_2, XAXIVDMA_WRITE);

	return XST_SUCCESS;
}
int check_config_vdma_tpgVideo_1(void)
{
	int Status = XST_SUCCESS;

	/*********** TPG_2 S2MM programming *****************/
	#if 1
	int retry;
	for(retry=0; retry < RETRY_COUNT; retry++)
	{
		Status = check_config_vdma(VDMA_INSTANCE_TPG_2, XAXIVDMA_WRITE);
		if(Status == XST_SUCCESS) break;
	}
	if(Status != XST_SUCCESS) return XST_FAILURE;
	#endif

	return Status;
}
int get_currentFrmaeNumber_tpgVideo_1(void)
{
	int Status;

	Status = get_currentFrameNumber_vdma(VDMA_INSTANCE_TPG_2, XAXIVDMA_WRITE);
	return Status;
}


/*****************************************************************************/
/**
* function to setup the VDMA channels for
* 	SCALED VIDEO (stream #0)
* 	SCALED VIDEO (stream #1)
* 	SCALED Test Pattern #1
* 	SCALED Test Pattern #2
* this function configures all required VDMA channels for this demo.
*
* @param	None
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		We use the 3 VDMA channels for the Full Video (stream #0) demo
* 			DVI_IN_0 (S2MM)
*			SCALER_0 (S2MM)
*			SCALER_0 (MM2S)
* 			DVI_IN_1 (S2MM)
*			SCALER_2 (S2MM)
*			SCALER_2 (MM2S)
*			TPG_0    (S2MM)
*			TPG_2    (S2MM)
*
******************************************************************************/
int config_vdma_scaleVideo_both(void)
{
	int Status;

#if 1
	// Prepare Video_0 (input stream #1) in scaled form
	Status = config_vdma_scaleVideo_0();
	if (Status != XST_SUCCESS) {
		xil_printf("config_vdma_scaleVideo_0 failed %d\r\n", Status);
		return XST_FAILURE;
	}
#endif

#if 1
	// Prepare Video_1 (input stream #2) in scaled form
	Status = config_vdma_scaleVideo_1();
	if (Status != XST_SUCCESS) {
		xil_printf("config_vdma_scaleVideo_1 failed %d\r\n", Status);
		return XST_FAILURE;
	}
#endif

#if 1
	// Prepare TPG_0 (Test Pattern Generator #1) in scaled form
	Status = config_vdma_tpgVideo_0();
	if (Status != XST_SUCCESS) {
		xil_printf("config_vdma_tpgVideo_0 failed %d\r\n", Status);
		return XST_FAILURE;
	}
#endif

#if 1
	// Prepare TPG_1 (Test Pattern Generator #2) in scaled form
	Status = config_vdma_tpgVideo_1();
	if (Status != XST_SUCCESS) {
		xil_printf("config_vdma_tpgVideo_1 failed %d\r\n", Status);
		return XST_FAILURE;
	}
#endif

	return Status;
}

int check_config_vdma_scaleVideo_both(void)
{
	int Status;

#if 1
	// Prepare Video_0 (input stream #1) in scaled form
	Status = check_config_vdma_scaleVideo_0();
	if (Status != XST_SUCCESS) {
		xil_printf("CHECK config_vdma_scaleVideo_0 failed %d\r\n", Status);
		return XST_FAILURE;
	}
#endif

#if 1
	// Prepare Video_1 (input stream #2) in scaled form
	Status = check_config_vdma_scaleVideo_1();
	if (Status != XST_SUCCESS) {
		xil_printf("CHECK config_vdma_scaleVideo_1 failed %d\r\n", Status);
		return XST_FAILURE;
	}
#endif

#if 1
	// Prepare TPG_0 (Test Pattern Generator #1) in scaled form
	Status = check_config_vdma_tpgVideo_0();
	if (Status != XST_SUCCESS) {
		xil_printf("CHECK config_vdma_tpgVideo_0 failed %d\r\n", Status);
		return XST_FAILURE;
	}
#endif

#if 1
	// Prepare TPG_1 (Test Pattern Generator #2) in scaled form
	Status = check_config_vdma_tpgVideo_1();
	if (Status != XST_SUCCESS) {
		xil_printf("CHECK config_vdma_tpgVideo_1 failed %d\r\n", Status);
		return XST_FAILURE;
	}
#endif

	return Status;
}

/*****************************************************************************/
/**
* function to setup the given VDMA instance. This function programs the GIVEN
* VDMA channel with GIVEN parameters.
*
* @param	id : the ID of VDMA channel to be initialized.
*           access: the access type for this VDMA configuration (READ or WRITE)
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		this function can be called in two occasions.
* 			1. at the beginning (start of application)
* 			2. during error condition of individual channel to reset
*
******************************************************************************/
static int config_vdma_instance(int id, int access)
{
	int Status;
	struct VDMA_INSTANCE *vdma;
	//volatile unsigned int i;

	vdma = &VdmaInstance[id];

	if(access == XAXIVDMA_WRITE)
	{
		// step1: VDMA configuration
		Status = XAxiVdma_DmaConfig(&vdma->AxiVdma, XAXIVDMA_WRITE, &vdma->WriteCfg);
		if (Status != XST_SUCCESS) {
			xil_printf("  channel write config failed %d, id=%d\r\n", Status, id);
			return XST_FAILURE;
		}

		// External FSYNC (TUSER based) is enabled, IF
		// 		(VDMA_INSTANCE_DVI_0 or VDMA_INSTANCE_DVI_0) and  S2MM CHANNEL
		#if 0
		if(id == VDMA_INSTANCE_DVI_0)
			XAxiVdma_FsyncSrcSelect(&vdma->AxiVdma, XAXIVDMA_S2MM_TUSER_FSYNC,XAXIVDMA_WRITE);
		if(id == VDMA_INSTANCE_DVI_1)
			XAxiVdma_FsyncSrcSelect(&vdma->AxiVdma, XAXIVDMA_S2MM_TUSER_FSYNC,XAXIVDMA_WRITE);
		#endif

		// step2: VDMA buffer addresses setup
		Status = XAxiVdma_DmaSetBufferAddr(&vdma->AxiVdma, XAXIVDMA_WRITE,
					vdma->WriteCfg.FrameStoreStartAddr);
		if (Status != XST_SUCCESS) {
			xil_printf("Write channel set buffer address failed %d\r\n", Status);
			return XST_FAILURE;
		}

		// step3: VDMA frame setup
		#if 0
		Status = XAxiVdma_SetFrameCounter(&vdma->AxiVdma, &vdma->FrameCfg);
		if (Status != XST_SUCCESS) {
			xil_printf("Set frame counter for write failed \r\n");
			return XST_FAILURE;
		}
		#endif

		// step4: Interrupt enable
		XAxiVdma_IntrEnable(&vdma->AxiVdma, XAXIVDMA_IXR_ALL_MASK, XAXIVDMA_WRITE);

		// step5: start write-frame
		Status = XAxiVdma_StartWriteFrame(&vdma->AxiVdma, &vdma->WriteCfg);
		if (Status != XST_SUCCESS) {
			xil_printf("StartWriteFrame for write failed %d\r\n", Status);
			return XST_FAILURE;
		}

		// step6: START THE TRANSFER
		Status = XAxiVdma_DmaStart(&vdma->AxiVdma, XAXIVDMA_WRITE);
		if (Status != XST_SUCCESS) {
			xil_printf("DmaStart for write failed %d\r\n", Status);
			return XST_FAILURE;
		}
		#if 0
		VDMA_READREG(vdma->AxiVdma.BaseAddr+0x30, i);
		i |= 0x8;
		VDMA_WRITEREG(vdma->AxiVdma.BaseAddr+0x30, i);
		VDMA_READREG(vdma->AxiVdma.BaseAddr+0x30, i);
		xil_printf("genlock: 0x%0x = 0x%0x \r\n",vdma->AxiVdma.BaseAddr,i);
		#endif
	}

	if(access == XAXIVDMA_READ)
	{
		// step1: VDMA configuration
		Status = XAxiVdma_DmaConfig(&vdma->AxiVdma, XAXIVDMA_READ, &vdma->ReadCfg);
		if (Status != XST_SUCCESS) {
			xil_printf("  channel read config failed %d, id=%d\r\n", Status, id);
			return XST_FAILURE;
		}

		// step2: VDMA buffer addresses setup
		Status = XAxiVdma_DmaSetBufferAddr(&vdma->AxiVdma, XAXIVDMA_READ,
					vdma->ReadCfg.FrameStoreStartAddr);
		if (Status != XST_SUCCESS) {
			xil_printf("Read channel set buffer address failed %d\r\n", Status);
			return XST_FAILURE;
		}

		// step3: VDMA frame setup
		#if 0
		Status = XAxiVdma_SetFrameCounter(&vdma->AxiVdma, &vdma->FrameCfg);
		if (Status != XST_SUCCESS) {
			xil_printf("Set frame counter failed \r\n");
			return XST_FAILURE;
		}
		#endif

		// step4: Interrupt enable
		XAxiVdma_IntrEnable(&vdma->AxiVdma, XAXIVDMA_IXR_ALL_MASK, XAXIVDMA_READ);

		// step5: start read-frame
		Status = XAxiVdma_StartReadFrame(&vdma->AxiVdma, &vdma->ReadCfg);
		if (Status != XST_SUCCESS) {
			xil_printf("StartWriteFrame for read failed %d\r\n", Status);
			return XST_FAILURE;
		}

		// step6: START THE TRANSFER
		Status = XAxiVdma_DmaStart(&vdma->AxiVdma, XAXIVDMA_READ);
		if (Status != XST_SUCCESS) {
			xil_printf("DmaStart for read failed %d\r\n", Status);
			return XST_FAILURE;
		}
		#if 0
		VDMA_READREG(vdma->AxiVdma.BaseAddr+0x00, i);
		i |= 0x8;
		VDMA_WRITEREG(vdma->AxiVdma.BaseAddr+0x00, i);
		VDMA_READREG(vdma->AxiVdma.BaseAddr+0x00, i);
		xil_printf("genlock: 0x%0x = 0x%0x \r\n", vdma->AxiVdma.BaseAddr,i);
		#endif
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* function to check the VDMA configuration is valid or invalid.
* if the configuration is proper, we start receiving interrupts from that channel
* The interrupts from VDMA channel are two types:
* 	READ : Read Count,  Read Error Count
* 	WRITE: Write Count, Write Error Count
* Wait for interrupt, or timeout.
*
* @param	id: Id of the VDMA channel to be checked
*           direction: data direction set for this channel (READ or WRITE)
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		This is a blocking call.
* 			this function waits until it receives atleast one
* 			valid or error interrupt.
*
******************************************************************************/
#if 0
int vdma_chk_tmr=0;
void* vdma_tmr_thread(void *p)
{
	int msec=500;

	xil_printf("check_config_vdma timer start %d mSecc \r\n", msec);
	sys_sleep(msec);		// milli seconds
	xil_printf("check_config_vdma time out \r\n");
	vdma_chk_tmr=1;		// time up
	return 0;
}
#endif

static int check_config_vdma(int id, int direction)
{
	struct VDMA_INSTANCE *vdma = &VdmaInstance[id];
	XAxiVdma *instance = &(vdma->AxiVdma);
	//int park1, park2;
	//int wait_for_frames=1;
	volatile unsigned int x;
	volatile unsigned int addr;
	//unsigned int pid;
	//p_stat ps;
	unsigned int i;
	int sec_now, sec_end;

	/*---------------------------------------------------------*/
	// Clear any VDMA errors
	/*---------------------------------------------------------*/
	//xil_printf("\r\n check vdma\r\n\r\n");
	//XAxiVdma_GetDmaChannelErrors(instance, direction);
	//XAxiVdma_ClearDmaChannelErrors(instance, direction,0x0000fffc);

#if 0
	/*---------------------------------------------------------*/
	// get current park pointer value
	/*---------------------------------------------------------*/
	VDMA_READREG(instance->BaseAddr+0x28, i);
	if(direction == XAXIVDMA_READ)
		 park1 = (i & 0x1f0000)   >> 16;	// mm2s
	else park1 = (i & 0x1f000000) >> 24;	// s2mm
	//printf("a. dir=%d baseaddr=0x%x, data=0x%x, park1= 0x%x \r\n",
	//		direction, (unsigned int)instance->BaseAddr, i, park1);

	/*---------------------------------------------------------*/
	// set new park pointer value to verify
	/*---------------------------------------------------------*/
	if( (id==VDMA_INSTANCE_TPG_0) || (id=VDMA_INSTANCE_TPG_2) )
		 park1 += 1;		// wait for 2 frames to change
	else park1 += 2;
	park1 %= 3;
	//printf("b. new park value to check: %d \r\n", park1);
#endif

	/*---------------------------------------------------------*/
	// Wait till new set value is reached or 5 seconds timeout
	/*---------------------------------------------------------*/
	sec_now = sys_time (NULL);
	sec_end = sec_now + 3;

	while(1)
	{
		/*
		if(direction == XAXIVDMA_READ)
			 park2 = (i & 0x1f0000)   >> 16;	// mm2s
		else park2 = (i & 0x1f000000) >> 24;	// s2mm

		if(park1 == park2)
			break;
		*/

		// FIX1: Clear ALL error status
		if(direction == XAXIVDMA_READ) {
			addr = instance->BaseAddr+0x04;
			//xil_printf("clear error Base:0x%x \r\n", addr);
			VDMA_READREG(addr, i);
			VDMA_WRITEREG(addr, i);

			// FIX2: if Video0 scaled, clear DVI_0 errors also
			if(id==VDMA_INSTANCE_SCALER_0)
			{
				addr = VdmaInstance[VDMA_INSTANCE_DVI_0].AxiVdma.BaseAddr+0x04;
				//xil_printf("clear error Base:0x%x \r\n", addr);
				VDMA_READREG(addr, i);
				VDMA_WRITEREG(addr, i);
			}

			// FIX3: if Video1 scaled, clear DVI_1 errors also
			if(id==VDMA_INSTANCE_SCALER_2)
			{
				addr = VdmaInstance[VDMA_INSTANCE_DVI_1].AxiVdma.BaseAddr+0x04;
				//xil_printf("clear error Base:0x%x \r\n", addr);
				VDMA_READREG(addr, i);
				VDMA_WRITEREG(addr, i);
			}
		}
		
		else 
		{
			addr = instance->BaseAddr+0x34;
			//xil_printf("clear error Base:0x%x \r\n", addr);
			VDMA_READREG(addr, i);
			VDMA_WRITEREG(addr, i);

			// FIX2: if Video0 scaled, clear DVI_0 errors also
			if(id==VDMA_INSTANCE_SCALER_0)
			{
				addr = VdmaInstance[VDMA_INSTANCE_DVI_0].AxiVdma.BaseAddr+0x34;
				//xil_printf("clear error Base:0x%x \r\n", addr);
				VDMA_READREG(addr, i);
				VDMA_WRITEREG(addr, i);
			}

			// FIX3: if Video1 scaled, clear DVI_1 errors also
			if(id==VDMA_INSTANCE_SCALER_2)
			{
				addr = VdmaInstance[VDMA_INSTANCE_DVI_1].AxiVdma.BaseAddr+0x34;
				//xil_printf("clear error Base:0x%x \r\n", addr);
				VDMA_READREG(addr, i);
				VDMA_WRITEREG(addr, i);
			}
		}

		// FRMPTR_STS register
		addr = instance->BaseAddr+0x28;
		VDMA_READREG(addr, x);
		//xil_printf("Base:0x%x, data:0x%x \r\n", addr, x);
		if(direction == XAXIVDMA_WRITE)
		{
		     x = x & 0x0F000000;
             if(x == 0x01000000) break;
		}
		else {
            x = x & 0x00000F00;
            if(x == 0x00000100) break;
		}

		sec_now = sys_time (NULL);
		if(sec_now == sec_end)
		{
			printf("check vdma failed, id=%d \r\n", id);
			return XST_FAILURE;
		}
	}
    print("Park Pointer register of VDMA done \r\n ");
	return XST_SUCCESS;
}

static int get_currentFrameNumber_vdma(int id, int direction)
{
	int curFrameNum;
	int i;

    //xil_printf("get_currentFrameNumber_vdma: id=%d, dir=%d \r\n", id, direction);

    struct VDMA_INSTANCE *vdma = &VdmaInstance[id];
	XAxiVdma *instance = &(vdma->AxiVdma);

	VDMA_READREG(instance->BaseAddr+0x28, i);
	if(direction == XAXIVDMA_READ)
		curFrameNum = i & 0x1f;					// mm2s
	else curFrameNum = (i & 0x1f0000) >> 16;	// s2mm

	//xil_printf("currentFrameNumber=%d \r\n", curFrameNum);
	return curFrameNum;
}

/*****************************************************************************/
/*
* This function setups the interrupt system so interrupts can occur for the
* VDMA. This function assumes INTC component exists in the hardware system.
*
* @param	AxiDmaPtr:  pointer to the instance of the DMA engine
* 			ReadIntrId: read channel Interrupt ID.
* 			WriteIntrId: write channel Interrupt ID.
*
* @return	XST_SUCCESS if successful, otherwise XST_FAILURE.
*
* @note		We have 4 interrupt handlers (ReadOK, ReadError, WriteOK and WriteError)
* 			Same Interrupts Handlers are used for ALL VDMA instances.
* 			When an interrupt occurs (any of the above 4), we differentiate
* 			by means of the VDMA instance caused that particular interrupt.
*
******************************************************************************/
static int config_vdma_IntrSystem(struct VDMA_INSTANCE *vdma, int ReadIntrId, int WriteIntrId)
{
	XAxiVdma *AxiVdmaPtr = &vdma->AxiVdma;

	if(ReadIntrId != -1)
	{
		#if 0
		/* Register Interrupt handler functions */
		Status = XIntc_Connect(&InterruptController, ReadIntrId,
				(XInterruptHandler) XAxiVdma_ReadIntrHandler, (void*)AxiVdmaPtr);
		if (Status != XST_SUCCESS) {
			xil_printf("  XIntc_Connect Failed, rd_intrid=%x\r\n", ReadIntrId);
			return XST_FAILURE;
		}

		/* Register call back functions */
		XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_GENERAL,
					vdma_ReadCallBack,
					(void *)vdma, XAXIVDMA_READ);

		XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_ERROR,
					vdma_ReadErrorCallBack,
					(void *)vdma, XAXIVDMA_READ);

		/* Enable interrupt */
		XIntc_Enable(&InterruptController, ReadIntrId);

		#else
		/* Register Interrupt handler functions */
		register_int_handler(ReadIntrId,
				(XInterruptHandler) XAxiVdma_ReadIntrHandler,
				AxiVdmaPtr);
		/* Register call back functions */
		XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_GENERAL,
					vdma_ReadCallBack,
					(void *)vdma, XAXIVDMA_READ);

		XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_ERROR,
					vdma_ReadErrorCallBack,
					(void *)vdma, XAXIVDMA_READ);

		/* Enable interrupt */
		//enable_interrupt(ReadIntrId);
		#endif
	}

	if(WriteIntrId != -1)
	{
		#if 0
		/* Register Interrupt handler functions */
		Status = XIntc_Connect(&InterruptController, WriteIntrId,
					(XInterruptHandler) XAxiVdma_WriteIntrHandler, (void*)AxiVdmaPtr );
		if (Status != XST_SUCCESS) {
			xil_printf("  XIntc_Connect Failed, wr_intrid=%x\r\n", WriteIntrId);
			return XST_FAILURE;
		}

		/* Register call back functions */
		XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_GENERAL,
					vdma_WriteCallBack,
					(void *)vdma, XAXIVDMA_WRITE);

		XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_ERROR,
					vdma_WriteErrorCallBack,
					(void *)vdma, XAXIVDMA_WRITE);

		/* Enable interrupt */
		XIntc_Enable(&InterruptController, WriteIntrId);

		#else
		/* Register Interrupt handler functions */
		register_int_handler(WriteIntrId,
				(XInterruptHandler) XAxiVdma_WriteIntrHandler,
				AxiVdmaPtr);
		/* Register call back functions */
		XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_GENERAL,
					vdma_WriteCallBack,
					(void *)vdma, XAXIVDMA_WRITE);

		XAxiVdma_SetCallBack(AxiVdmaPtr, XAXIVDMA_HANDLER_ERROR,
					vdma_WriteErrorCallBack,
					(void *)vdma, XAXIVDMA_WRITE);

		/* Enable interrupt */
		//enable_interrupt(WriteIntrId);
		#endif
	}

	return XST_SUCCESS;
}

/*****************************************************************************/
/*
 * Call back function for read channel
 *
 * This callback clears the interrupts and updates the transfer status.
 *
 * @param	CallbackRef: call back reference pointer
 *				this is pointer to the VDMA instance that caused the interrupt
 *			Mask:  interrupt mask passed in from the driver
 *
 * @return	Every VDMA instance maintains the occurance of the interrupt
 * 			by means of the counter (se below: vdma->xxx++)
*
******************************************************************************/
static void vdma_ReadCallBack(void *CallbackRef, u32 Mask)
{
	struct VDMA_INSTANCE *vdma = (struct VDMA_INSTANCE*) CallbackRef;
	if (Mask & XAXIVDMA_IXR_FRMCNT_MASK) {
		//xil_printf("read callback, id=%d\r\n", vdma->deviceid);
		ReadDone += 1;
	}
	xil_printf("vdma_ReadCallBack\r\n");
	vdma->ReadIntrCnt++;
}

/*****************************************************************************/
/*
 * Call back function for read channel error interrupt
 * This callback clears the interrupts and updates the transfer status.
 *
 * @param	CallbackRef: call back reference pointer
 *				this is pointer to the VDMA instance that caused the interrupt
 *			Mask:  interrupt mask passed in from the driver
 *
 * @return	Every VDMA instance maintains the occurance of the interrupt
 * 			by means of the counter (se below: vdma->xxx++)
*
******************************************************************************/
static void vdma_ReadErrorCallBack(void *CallbackRef, u32 Mask)
{
	struct VDMA_INSTANCE *vdma = (struct VDMA_INSTANCE*) CallbackRef;
	if (Mask & XAXIVDMA_IXR_ERROR_MASK) {
		ReadError += 1;
		//xil_printf("read error callback\r\n");
		reconfig_VDMA_instance(vdma, XAXIVDMA_READ);
	}
	xil_printf("vdma_ReadErrorCallBack\r\n");
	vdma->ReadErrIntrCnt++;
}

/*****************************************************************************/
/*
 * Call back function for write channel
 * This callback clears the interrupts and updates the transfer status.
 *
 * @param	CallbackRef: call back reference pointer
 *				this is pointer to the VDMA instance that caused the interrupt
 *			Mask:  interrupt mask passed in from the driver
 *
 * @return	Every VDMA instance maintains the occurance of the interrupt
 * 			by means of the counter (se below: vdma->xxx++)
*
******************************************************************************/
static void vdma_WriteCallBack(void *CallbackRef, u32 Mask)
{
	struct VDMA_INSTANCE *vdma = (struct VDMA_INSTANCE*) CallbackRef;
	if (Mask & XAXIVDMA_IXR_FRMCNT_MASK) {
		WriteDone += 1;
		//xil_printf("write callback, id=%d\r\n", vdma->deviceid);
	}
	xil_printf("vdma_WriteCallBack\r\n");
	vdma->WriteIntrCnt++;
}

/*****************************************************************************/
/*
* Call back function for write channel error interrupt
 * This callback clears the interrupts and updates the transfer status.
 *
 * @param	CallbackRef: call back reference pointer
 *				this is pointer to the VDMA instance that caused the interrupt
 *			Mask:  interrupt mask passed in from the driver
 *
 * @return	Every VDMA instance maintains the occurance of the interrupt
 * 			by means of the counter (se below: vdma->xxx++)
*
******************************************************************************/
static void vdma_WriteErrorCallBack(void *CallbackRef, u32 Mask)
{
	struct VDMA_INSTANCE *vdma = (struct VDMA_INSTANCE*) CallbackRef;
	if (Mask & XAXIVDMA_IXR_ERROR_MASK) {
		reconfig_VDMA_instance(vdma, XAXIVDMA_WRITE);
		//xil_printf("write error callback\r\n");
		WriteError += 1;
	}
	xil_printf("vdma_WriteErrorCallBack\r\n");
	vdma->WriteErrIntrCnt++;
}

/*****************************************************************************/
/**
* Main function to setup the VDMA channels.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
static int reconfig_VDMA_instance(struct VDMA_INSTANCE *InstancePtr, int direction)
{
	int Status;
	struct VDMA_INSTANCE *vdma;
	struct VDMA_INSTANCE vdma_bkp;

	deactivate_vdemo_system();

	///////////////////////////////
	// backup the current settings
	vdma_bkp.deviceid = InstancePtr->deviceid;
	vdma_bkp.access_type = InstancePtr->access_type;
	vdma_bkp.rd_intrid = InstancePtr->rd_intrid;
	vdma_bkp.wr_intrid = InstancePtr->wr_intrid;

	// backup write data
	vdma_bkp.WriteCfg.VertSizeInput = InstancePtr->WriteCfg.VertSizeInput;
	vdma_bkp.WriteCfg.HoriSizeInput = InstancePtr->WriteCfg.HoriSizeInput;
	vdma_bkp.WriteCfg.FrameStoreStartAddr[0] = InstancePtr->WriteCfg.FrameStoreStartAddr[0];
	vdma_bkp.WriteCfg.FrameStoreStartAddr[1] = InstancePtr->WriteCfg.FrameStoreStartAddr[1];
	vdma_bkp.WriteCfg.FrameStoreStartAddr[2] = InstancePtr->WriteCfg.FrameStoreStartAddr[2];

	// backup read data
	vdma_bkp.ReadCfg.VertSizeInput = InstancePtr->ReadCfg.VertSizeInput;
	vdma_bkp.ReadCfg.HoriSizeInput = InstancePtr->ReadCfg.HoriSizeInput;
	vdma_bkp.ReadCfg.FrameStoreStartAddr[0] = InstancePtr->ReadCfg.FrameStoreStartAddr[0];
	vdma_bkp.ReadCfg.FrameStoreStartAddr[1] = InstancePtr->ReadCfg.FrameStoreStartAddr[1];
	vdma_bkp.ReadCfg.FrameStoreStartAddr[2] = InstancePtr->ReadCfg.FrameStoreStartAddr[2];

	//////////////////////////////////////////
	// reset and initialize the VDMA instance
	reset_vdma_instance(&InstancePtr->AxiVdma, direction);

	InstancePtr->access_type = vdma_bkp.access_type;	// restore access type (RD/WR/Both)
	Status = init_vdma_instance(vdma_bkp.deviceid);
	if (Status != XST_SUCCESS) {
		xil_printf("  init_vdma_instance %d Failed\r\n", vdma_bkp.access_type);
		return XST_FAILURE;
	}
	Status = config_vdma_IntrSystem(&VdmaInstance[vdma_bkp.deviceid],
					VdmaInstance[vdma_bkp.deviceid].rd_intrid,
					VdmaInstance[vdma_bkp.deviceid].wr_intrid);
	if (Status != XST_SUCCESS) {
		xil_printf("  init_vdma_instance %d Failed\r\n", vdma_bkp.deviceid);
		return XST_FAILURE;
	}

	///////////////////////////////
	// reprogram the VDMA instance
	vdma = InstancePtr;
	if(direction == XAXIVDMA_WRITE)
	{
		VDMA_FILL_WRDATA;
		vdma->WriteCfg.VertSizeInput = vdma_bkp.WriteCfg.VertSizeInput;
		vdma->WriteCfg.HoriSizeInput = vdma_bkp.WriteCfg.HoriSizeInput;
		vdma->WriteCfg.FrameStoreStartAddr[0] = vdma_bkp.WriteCfg.FrameStoreStartAddr[0];
		vdma->WriteCfg.FrameStoreStartAddr[1] = vdma_bkp.WriteCfg.FrameStoreStartAddr[1];
		vdma->WriteCfg.FrameStoreStartAddr[2] = vdma_bkp.WriteCfg.FrameStoreStartAddr[2];
		Status = config_vdma_instance(vdma->deviceid, XAXIVDMA_WRITE);
	}
	else
	{
		VDMA_FILL_RDDATA;
		vdma->ReadCfg.VertSizeInput = vdma_bkp.ReadCfg.VertSizeInput;
		vdma->ReadCfg.HoriSizeInput = vdma_bkp.ReadCfg.HoriSizeInput;
		vdma->ReadCfg.FrameStoreStartAddr[0] = vdma_bkp.ReadCfg.FrameStoreStartAddr[0];
		vdma->ReadCfg.FrameStoreStartAddr[1] = vdma_bkp.ReadCfg.FrameStoreStartAddr[1];
		vdma->ReadCfg.FrameStoreStartAddr[2] = vdma_bkp.ReadCfg.FrameStoreStartAddr[2];
		Status = config_vdma_instance(vdma->deviceid, XAXIVDMA_READ);
	}

	activate_vdemo_system(new_lsel, new_vsel, new_ssel);
	return Status;
}

int reset_VDMA(void)
{
	// DVI_IN_0 (only Write channel exists)
	//xil_printf("    reset VDMA_INSTANCE_DVI_0 \r\n");
	//XAxiVdma_StartParking(&VdmaInstance[VDMA_INSTANCE_DVI_0].AxiVdma, 0,XAXIVDMA_WRITE);
	reset_vdma_instance(&VdmaInstance[VDMA_INSTANCE_DVI_0].AxiVdma, XAXIVDMA_WRITE);
	reset_vdma_statistics(&VdmaInstance[VDMA_INSTANCE_DVI_0]);

	// DVI_IN_1 (only Write channel exists)
	//xil_printf("    reset VDMA_INSTANCE_DVI_1 \r\n");
	//XAxiVdma_StartParking(&VdmaInstance[VDMA_INSTANCE_DVI_1].AxiVdma, 0,XAXIVDMA_WRITE);
	reset_vdma_instance(&VdmaInstance[VDMA_INSTANCE_DVI_1].AxiVdma, XAXIVDMA_WRITE);
	reset_vdma_statistics(&VdmaInstance[VDMA_INSTANCE_DVI_1]);

	// TPG_0 (only Write channel exists)
	//xil_printf("    reset VDMA_INSTANCE_TPG_0 \r\n");
	//XAxiVdma_StartParking(&VdmaInstance[VDMA_INSTANCE_TPG_0].AxiVdma, 0,XAXIVDMA_WRITE);
	reset_vdma_instance(&VdmaInstance[VDMA_INSTANCE_TPG_0].AxiVdma, XAXIVDMA_WRITE);
	reset_vdma_statistics(&VdmaInstance[VDMA_INSTANCE_TPG_0]);

	// TPG_2 (only Write channel exists)
	//xil_printf("    reset VDMA_INSTANCE_TPG_2 \r\n");
	//XAxiVdma_StartParking(&VdmaInstance[VDMA_INSTANCE_TPG_2].AxiVdma, 0,XAXIVDMA_WRITE);
	reset_vdma_instance(&VdmaInstance[VDMA_INSTANCE_TPG_2].AxiVdma, XAXIVDMA_WRITE);
	reset_vdma_statistics(&VdmaInstance[VDMA_INSTANCE_TPG_2]);

	// SCALER_0 (both Read and Write channels exist)
	//xil_printf("    reset VDMA_INSTANCE_SCALER_0 read \r\n");
	//XAxiVdma_StartParking(&VdmaInstance[VDMA_INSTANCE_SCALER_0].AxiVdma, 0,XAXIVDMA_READ);
	reset_vdma_instance(&VdmaInstance[VDMA_INSTANCE_SCALER_0].AxiVdma, XAXIVDMA_READ);
	//xil_printf("    reset VDMA_INSTANCE_SCALER_0 write \r\n");
	//XAxiVdma_StartParking(&VdmaInstance[VDMA_INSTANCE_SCALER_0].AxiVdma, 0,XAXIVDMA_WRITE);
	reset_vdma_instance(&VdmaInstance[VDMA_INSTANCE_SCALER_0].AxiVdma, XAXIVDMA_WRITE);
	reset_vdma_statistics(&VdmaInstance[VDMA_INSTANCE_SCALER_0]);

	// SCALER_2 (both Read and Write channels exist)
	//xil_printf("    reset VDMA_INSTANCE_SCALER_2 read \r\n");
	//XAxiVdma_StartParking(&VdmaInstance[VDMA_INSTANCE_SCALER_2].AxiVdma, 0,XAXIVDMA_READ);
	reset_vdma_instance(&VdmaInstance[VDMA_INSTANCE_SCALER_2].AxiVdma, XAXIVDMA_READ);
	//xil_printf("    reset VDMA_INSTANCE_SCALER_2 write \r\n");
	//XAxiVdma_StartParking(&VdmaInstance[VDMA_INSTANCE_SCALER_2].AxiVdma, 0,XAXIVDMA_WRITE);
	reset_vdma_instance(&VdmaInstance[VDMA_INSTANCE_SCALER_2].AxiVdma, XAXIVDMA_WRITE);
	reset_vdma_statistics(&VdmaInstance[VDMA_INSTANCE_SCALER_2]);

    return 0;
}

#if 0
static int reset_vdma_instance_OLD(XAxiVdma *InstancePtr, int direction)
{
	int Status;

	// Interrupt disable
	XAxiVdma_IntrDisable(InstancePtr, XAXIVDMA_IXR_ALL_MASK, direction);
	XAxiVdma_DmaStop(InstancePtr, direction);
	XAxiVdma_Reset(InstancePtr, direction);
	while(1)
	{
		Status = XAxiVdma_ResetNotDone(InstancePtr,	direction);
		if(Status == 0)
			break;
	}

	return 0;
}
#endif

static int reset_vdma_instance(XAxiVdma *InstancePtr, int direction)
{
	//int Status;
	unsigned int i;
	int sec_now, sec_end;

	// check if the VDMA already in RUN state
	if(direction==XAXIVDMA_WRITE)
	{
		VDMA_READREG(InstancePtr->BaseAddr+0x30, i);
	}
	else
	{
		VDMA_READREG(InstancePtr->BaseAddr+0x00, i);
	}
	if( (i & 0x1) == 0)
		return 0;

	// Interrupt disable
	XAxiVdma_IntrDisable(InstancePtr, XAXIVDMA_IXR_ALL_MASK, direction);
	XAxiVdma_DmaStop(InstancePtr, direction);

	sec_now = sys_time (NULL);
	sec_end = sec_now + 3;
	while(1)
	{
		if(direction==XAXIVDMA_WRITE)
		{
			VDMA_READREG(InstancePtr->BaseAddr+0x34, i);
		}
		else
		{
			VDMA_READREG(InstancePtr->BaseAddr+0x04, i);
		}
		if( (i & 0x1) == 0x1)
			break;

		sec_now = sys_time (NULL);
		if(sec_now == sec_end)
		{
			printf("wait for VDMA reset failed \r\n");
			return XST_FAILURE;
		}
	}

	/*
	XAxiVdma_Reset(InstancePtr, direction);
	while(1)
	{
		Status = XAxiVdma_ResetNotDone(InstancePtr,	direction);
		if(Status == 0)
			break;
	}
	*/

	return 0;
}

static void reset_vdma_statistics(struct VDMA_INSTANCE *vdma)
{
	vdma->ReadIntrCnt     = 0;
	vdma->ReadErrIntrCnt  = 0;
	vdma->WriteIntrCnt    = 0;
	vdma->WriteErrIntrCnt = 0;

	ReadDone = 0;
	ReadError = 0;
	WriteDone = 0;
	WriteError = 0;

	return;
}
