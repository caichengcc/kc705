/* $Id: */
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
******************************************************************************/
/*****************************************************************************/
/**
*
* @file vdma_data.h
*
* Defines the functions and includes the proper headers.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who                Date     Changes
* ----- -----------------  --------  ----------------------
* 1.00  srinivasa attili   9-7-11   Initial Release
* 2.00  srinivasa attili   2-2-12   for Kintex7 Demo TRD
*
******************************************************************************/
/***************************** Include Files *********************************/

#include "xparameters.h"	/* defines XPAR values */
#include "xaxivdma.h"
#include "xintc.h"

/************************** Constant Definitions ****************************/
// Constants used for CVC Layers
// ---------------------------------
	#define	VMEM_BASE				0x80000000 	//0xE0000000
	#define	CVC_L0_BASE_ADDRESS		(VMEM_BASE | 0x0000000)
	#define	CVC_L1_BASE_ADDRESS		(VMEM_BASE | 0x32A0000)
	#define	CVC_L2_BASE_ADDRESS		(VMEM_BASE | 0x6540000)
	#define	CVC_L3_BASE_ADDRESS		(VMEM_BASE | 0x97E0000)
	// the below code should have been provided as part of Xylon CVC driver.
	#define	CVC_BASE			XPAR_LOGICVC_0_REGS_BASEADDR
	#define CVC_HSY_FP			(CVC_BASE+0x0000)
	#define CVC_HSY_WIDTH       (CVC_BASE+0x0008)
	#define CVC_HSY_BP          (CVC_BASE+0x0010)
	#define CVC_HSY_RESOLUTION  (CVC_BASE+0x0018)
	#define CVC_VSY_FP          (CVC_BASE+0x0020)
	#define CVC_VSY_WIDTH       (CVC_BASE+0x0028)
	#define CVC_VSY_BP          (CVC_BASE+0x0030)
	#define CVC_VSY_RESOLUTION  (CVC_BASE+0x0038)
	#define CVC_CTRL            (CVC_BASE+0x0040)
	#define CVC_DTYPE           (CVC_BASE+0x0048)
	#define CVC_BACKGROUND      (CVC_BASE+0x0050)
	#define CVC_VBUFF_SEL       (CVC_BASE+0x0058)
	#define CVC_CLUT_SEL        (CVC_BASE+0x0060)
	#define CVC_INT_STAT        (CVC_BASE+0x0068)
	#define CVC_INT_MASK        (CVC_BASE+0x0070)
	#define CVC_PWRCTRL         (CVC_BASE+0x0078)
	#define CVC_L0_H_OFFSET     (CVC_BASE+0x0100)
	#define CVC_L0_V_OFFSET     (CVC_BASE+0x0108)
	#define CVC_L0_H_POSITION   (CVC_BASE+0x0110)
	#define CVC_L0_V_POSITION   (CVC_BASE+0x0118)
	#define CVC_L0_WIDTH        (CVC_BASE+0x0120)
	#define CVC_L0_HEIGHT       (CVC_BASE+0x0128)
	#define CVC_L0_ALPHA        (CVC_BASE+0x0130)
	#define CVC_L0_CTRL         (CVC_BASE+0x0138)
	#define CVC_L0_TRANSPARENT  (CVC_BASE+0x0140)
	#define CVC_L1_H_OFFSET     (CVC_BASE+0x0180)
	#define CVC_L1_V_OFFSET     (CVC_BASE+0x0188)
	#define CVC_L1_H_POSITION   (CVC_BASE+0x0190)
	#define CVC_L1_V_POSITION   (CVC_BASE+0x0198)
	#define CVC_L1_WIDTH        (CVC_BASE+0x01A0)
	#define CVC_L1_HEIGHT       (CVC_BASE+0x01A8)
	#define CVC_L1_ALPHA        (CVC_BASE+0x01B0)
	#define CVC_L1_CTRL         (CVC_BASE+0x01B8)
	#define CVC_L1_TRANSPARENT  (CVC_BASE+0x01C0)
	#define CVC_L2_H_OFFSET     (CVC_BASE+0x0200)
	#define CVC_L2_V_OFFSET     (CVC_BASE+0x0208)
	#define CVC_L2_H_POSITION   (CVC_BASE+0x0210)
	#define CVC_L2_V_POSITION   (CVC_BASE+0x0218)
	#define CVC_L2_WIDTH        (CVC_BASE+0x0220)
	#define CVC_L2_HEIGHT       (CVC_BASE+0x0228)
	#define CVC_L2_ALPHA        (CVC_BASE+0x0230)
	#define CVC_L2_CTRL         (CVC_BASE+0x0238)
	#define CVC_L2_TRANSPARENT  (CVC_BASE+0x0240)
	#define CVC_L3_H_OFFSET     (CVC_BASE+0x0280)
	#define CVC_L3_V_OFFSET     (CVC_BASE+0x0288)
	#define CVC_L3_H_POSITION   (CVC_BASE+0x0290)
	#define CVC_L3_V_POSITION   (CVC_BASE+0x0298)
	#define CVC_L3_WIDTH        (CVC_BASE+0x02A0)
	#define CVC_L3_HEIGHT       (CVC_BASE+0x02A8)
	#define CVC_L3_ALPHA        (CVC_BASE+0x02B0)
	#define CVC_L3_CTRL         (CVC_BASE+0x02B8)
	#define CVC_L3_TRANSPARENT  (CVC_BASE+0x02C0)
	#define CVC_L4_CTRL       	(CVC_BASE+0x0338)


// Constants used for VDMA channels
// ---------------------------------
	#define VDMA_INSTANCE_DVI_0		0		// index into VDMA Instances array
	#define VDMA_INSTANCE_DVI_1		1
	#define VDMA_INSTANCE_SCALER_0	2
	#define VDMA_INSTANCE_SCALER_2	3
	#define VDMA_INSTANCE_TPG_0		4
	#define VDMA_INSTANCE_TPG_2		5
	#define MAX_VDMA_INSTANCE		6

	#define VDMA_NONE				0x0
	#define VDMA_RD_ONLY			0x1
	#define VDMA_WR_ONLY			0x2
	#define VDMA_RD_WR				(VDMA_RD_ONLY | VDMA_WR_ONLY)

	#define NUMBER_OF_READ_FRAMES	3
	#define NUMBER_OF_WRITE_FRAMES	3
	#define DELAY_TIMER_COUNTER		10

// Constants used for Performance Monitor
// --------------------------------------
	#define	PERFMON_INTR_ID			XPAR_INTERRUPT_CNTLR_PERF_MONITOR_0_PERF_IRPT_INTR
	// performance monitor select data collection
	#define	PERFMON_RD_COUNT				0x1
	#define	PERFMON_WR_COUNT				0x2
	#define	PERFMON_RDWR_COUNT				0x4
	// performance monitor interrupt mode
	#define	PERFMON_INTR_MODE				1
	#define	PERFMON_POLL_MODE				2
	// performance monitor IOCTL commands
	#define PERFMON_IOCTL_SET_SAMPLERATE	1
	#define PERFMON_IOCTL_GET_SAMPLERATE	2

// Constants used for Video streams selections
// -------------------------------------------
	#define VIDEO_STREAM_MODE	1	// demo with actual video streams
	#define	INTERNAL_DEMO_MODE	2	// demo with internal patterns (no live video stream

	#define	LIVEVIDEO_DISABLE	0
	#define	LIVEVIDEO_ENABLE	1

	// selections used for VDMA and CVC setup
	#define	SYS_STREAM0_FULL	1		// DVI_IN_0 full   or TPG_1 full
	#define	SYS_STREAM1_FULL	2		// DVI_IN_1 full   or TPG_3 full
	#define	SYS_STREAM0_SCALED	3		// DVI_IN_0 scaled or TPG_1 scaled
	#define	SYS_STREAM1_SCALED	4		// DVI_IN_1 scaled or TPG_3 scaled
	#define	SYS_TPG0_SCALED		5		// TPG_0 scaled (always)
	#define	SYS_TPG2_SCALED		6		// TPG_2 scaled (always)

	// selections from Web server
	#define	WS_STREAM_NONE		0		// DVI_IN_0 full   or TPG_1 full
	#define	WS_STREAM0_FULL		1		// DVI_IN_0 full   or TPG_1 full
	#define	WS_STREAM1_FULL		2		// DVI_IN_1 full   or TPG_3 full
	#define	WS_SCALED_STREAMS	3
	#define	WS_STREAM0_SCALED	0x1		// DVI_IN_0 scaled or TPG_1 scaled
	#define	WS_STREAM1_SCALED	0x2		// DVI_IN_1 scaled or TPG_3 scaled
	#define	WS_TPG0_SCALED		0x4		// TPG_0 scaled (always)
	#define	WS_TPG2_SCALED		0x8		// TPG_2 scaled (always)
	#define	WS_ALL_SCALED		0xF		// all 4 streams

/**************************** Type Definitions ******************************/

struct VDMA_INSTANCE {
	XAxiVdma AxiVdma;
	XAxiVdma_Config *Config;
	XAxiVdma_FrameCounter FrameCfg;
	XAxiVdma_DmaSetup ReadCfg;
	XAxiVdma_DmaSetup WriteCfg;

	int deviceid;
	int rd_intrid;
	int wr_intrid;
	int access_type;

	// Statistics
	int ReadIntrCnt;
	int ReadErrIntrCnt;
	int WriteIntrCnt;
	int WriteErrIntrCnt;

	unsigned int s2mm_Address0;
	unsigned int s2mm_Address1;
	unsigned int s2mm_Address2;

	unsigned int mm2s_Address0;
	unsigned int mm2s_Address1;
	unsigned int mm2s_Address2;
};

struct VDMA_PERF_MONITOR {
	float read_perf;
	float write_perf;
	unsigned int perfIntrCnt;
	char pollmode;
	char dataReady;
};

extern int new_vsel, new_psel, new_ssel, new_lsel;

/************************** Function Prototypes *****************************/
//APIs exported to webserver
extern void main_vdemo_webserver(void *p);
extern void vdemo_webserver_reset(void);
extern void main_vdemo_console(void);

// CVC APIs - Xylon (LogiBricks) Compact Video Controller
extern int init_CVC(void);
extern void shutdown_CVC(void);
extern int config_cvc_fullVideo_0(void);
extern int config_cvc_fullVideo_1(void);
extern int config_cvc_scaleVideo_0(void);
extern int config_cvc_scaleVideo_1(void);
extern int config_cvc_tpgVideo_0(void);
extern int config_cvc_tpgVideo_1(void);
extern int config_cvc_scaleVideo_both(void);
extern XStatus repeat_patterns_logiCVC(int vsel, int psel, int ssel);

// DVI APIs - Chrontel's DVI controller chip on IIC bus
extern int init_DVI(void);

// PERFORMANCE MONITOR APIs - to init and get DDR AXI MM read/write throughput
extern int init_PerfMonitor(struct VDMA_PERF_MONITOR *perf);
extern int reset_PerfMonitor_stats(struct VDMA_PERF_MONITOR *perf);
extern int enable_PerfMonitor(struct VDMA_PERF_MONITOR *perf, int CntMode, int IntrMode);
extern int disable_PerfMonitor(struct VDMA_PERF_MONITOR *perf);
extern int ioctl_PerfMonitor(struct VDMA_PERF_MONITOR *perf, int Cmd, int Data);
extern void PerfMon_pollHandler(struct VDMA_PERF_MONITOR *perf);
extern void stop_PerfMonitor(void);
extern void start_PerfMonitor(void);

// TIMEBASE APIs - to detect input stream resolutions and TPG resolutions
extern int init_TimeBaseDetection(void);
extern int init_Timebase(void);
extern int reset_TPG(void);

// VDMA APIs - Video Direct Memory Access
extern int init_VDMA(void);
extern int reset_VDMA(void);
extern int config_vdma_fullVideo_0(void);
extern int config_vdma_fullVideo_1(void);
extern int config_vdma_scaleVideo_0(void);
extern int config_vdma_scaleVideo_1(void);
extern int config_vdma_scaleVideo_both(void);
extern int config_vdma_tpgVideo_0(void);
extern int config_vdma_tpgVideo_1(void);

extern int check_config_vdma_fullVideo_0(void);
extern int check_config_vdma_fullVideo_1(void);
extern int check_config_vdma_scaleVideo_0(void);
extern int check_config_vdma_scaleVideo_1(void);
extern int check_config_vdma_scaleVideo_both(void);
extern int check_config_vdma_tpgVideo_0(void);
extern int check_config_vdma_tpgVideo_1(void);

extern int get_currentFrmaeNumber_fullVideo_0(void);
extern int get_currentFrmaeNumber_fullVideo_1(void);
extern int get_currentFrmaeNumber_scaleVideo_0(void);
extern int get_currentFrmaeNumber_scaleVideo_1(void);
extern int get_currentFrmaeNumber_scaleVideo_both(void);
extern int get_currentFrmaeNumber_tpgVideo_0(void);
extern int get_currentFrmaeNumber_tpgVideo_1(void);

extern int get_currentFrmaeNumber_VDMA(int selection);

// Global APIs
extern void deactivate_vdemo_system(void);
void activate_vdemo_system(int lsel, int vsel, int ssel);

/************************** Variable Definitions ****************************/
extern struct VDMA_INSTANCE VdmaInstance[MAX_VDMA_INSTANCE];
extern struct VDMA_PERF_MONITOR perf;
extern XIntc InterruptController;
extern int repeatVdemo;
