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
* File:	  vdma_timebase.c
* Author: srinivasa attili
* Date:	  7-sept-2011
*
* Purpose:	This module handles the time base setup for the Video Demo
*   application. The timebases of input stream is checked to understand
*   the input stream resolution.
*   similarly the time base for Test Pattern Generators are set to generate
*   test patterns at at the given resolution.
*   Compliant to Xilinx Standalone BSP / XilKernel BSP.
*
* MODIFICATION HISTORY:
*
* Ver   Who                Date     Changes
* ----- -----------------  --------  ----------------------
* 1.00  srinivasa attili   9-7-11   Initial Release
* 2.00  srinivasa attili   2-2-12   for Kintex7 Demo TRD
*
**************************************************************************/

/***************************** Include Files *********************************/
#include "xparameters.h"
#include "vdma_data.h"
#include <stdio.h>

/************************** Constant Definitions *****************************/
#define NO_VTC

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

// Register definitions.. Ideally should be part of BSP.

#define RST_CR_REG		XPAR_DISPLAY_CONTROLLER_GPIO_RST_MUX_BASEADDR

// -- TPG Register Offset (only TPG IP)
#define TPG_ONLY_PAT_SEL			0x00
#define TPG_ONLY_PAT_MOTION			0x04
#define TPG_ONLY_XHAIRS_H_V			0x08
#define TPG_ONLY_FRAME_HLEN			0x0C
#define TPG_ONLY_ZPLATE_H			0x10
#define TPG_ONLY_ZPLATE_V			0x14
#define TPG_ONLY_BOX_SIZE			0x18
#define TPG_ONLY_BOX_COLOUR			0x1C

// -- TPG Register Offset (TPG+DVI IP)
#define TPG_DVI_CONTROL				0x0000
#define TPG_DVI_STATUS				0x0004
#define TPG_DVI_ERROR				0x0008
#define TPG_DVI_IRQ_EN				0x000C
#define TPG_DVI_VERSION				0x0010
#define TPG_DVI_ACTIVE_SIZE			0x0020
#define TPG_DVI_PATTERN_CONTROL		0x0100
#define TPG_DVI_MOTION_SPEED		0x0104
#define TPG_DVI_CROSS_HAIRS			0x0108
#define TPG_DVI_ZPLATE_HOR_CONTROL	0x010C
#define TPG_DVI_ZPLATE_VER_CONTROL	0x0110
#define TPG_DVI_BOX_SIZE			0x0114
#define TPG_DVI_BOX_COLOR			0x0118
#define TPG_DVI_STUCK_PIXEL_THRESH	0x011C
#define TPG_DVI_NOISE_GAIN			0x0120

#define	TPG_IP_TPGONLY			1
#define	TPG_IP_TPGDVI			2

#define	TPG_DVI_ENABLE_DVI		0x1000
#define	TPG_DVI_ENABLE_TPG		0x1029

// TPG Pattern Selection (bits 0:4 of PAT_SEL register)
#define	PASSTHROUGH			0x00		// when "0000" => -- Input passthrough
#define	HORZ_RAMP			0x01		// when "0001" => -- horizontal ramp
#define	VERT_RAMP			0x02		// when "0010" => -- vertical ramp
#define	TEMPORAL_RAMP		0x03		// when "0011" => -- temporal ramp
#define	FILL_RED			0x04		// when "0100" => -- red
#define	FILL_GREEN			0x05		// when "0101" => -- green
#define	FILL_BLUE			0x06		// when "0110" => -- blue
#define	FILL_BLACK			0x07		// when "0111" => -- black
#define	FILL_WHITE			0x08		// when "1000" => -- white
#define	COLOR_BARS			0x09		// when "1001" => -- bars
#define	ZONEPLATE_SWEEP		0x0A		// when "1010" => -- ZonePlate/Sweep
#define	PAT_NONE			0xFF
// NEW TPG IP (TPG+DVI)
#define	COLOR_BARS2			0x29		// when "1001" => -- bars
#define	ZONEPLATE_SWEEP2	0x10021		// when "1010" => -- ZonePlate/Sweep

#define TpgWriteReg(BaseAddress, RegOffset, Data) \
	*((volatile int *)((BaseAddress)+(RegOffset))) = (Data);

// -- Time Base Generator REGISTERS
#ifdef NO_VTC

#define	TIME_BASE_BEGIN			XPAR_TIMEBASE_0_BASEADDR
#define TIME_BASE_CR			(TIME_BASE_BEGIN+0x00)
#define TIME_BASE_ACT_SIZE		(TIME_BASE_BEGIN+0x60)
#define TIME_BASE_ENCODE		(TIME_BASE_BEGIN+0x68)
#define TIME_BASE_POL			(TIME_BASE_BEGIN+0x6C)
#define TIME_BASE_HSIZE			(TIME_BASE_BEGIN+0x70)
#define TIME_BASE_VSIZE			(TIME_BASE_BEGIN+0x74)
#define TIME_BASE_HSYNC			(TIME_BASE_BEGIN+0x78)
#define TIME_BASE_VBLANK		(TIME_BASE_BEGIN+0x7C)
#define TIME_BASE_VSYNC			(TIME_BASE_BEGIN+0x80)

#else
//New VTC Values
#define VTC_BASE             0x50500000
#define VTC_CR	             (VTC_BASE+0x00)
#define VTC_GEN_ACT_SIZE     (VTC_BASE+0x60)
#define VTC_GEN_STAT         (VTC_BASE+0x64)
#define VTC_GEN_ENCODE       (VTC_BASE+0x68)
#define VTC_GEN_POL          (VTC_BASE+0x6c)
#define VTC_GEN_HSIZE        (VTC_BASE+0x70)
#define VTC_GEN_VSIZE        (VTC_BASE+0x74)
#define VTC_GEN_HSYNC        (VTC_BASE+0x78)
#define VTC_GEN_VBLANKH      (VTC_BASE+0x7c)
#define VTC_GEN_VSYNC        (VTC_BASE+0x80)
#define VTC_GEN_VSYNCH       (VTC_BASE+0x84)

#define E_Clk		148500
#define E_HTotal	2200
#define E_HSyncLen	44
#define E_HBP		148
#define E_HActive	1920
#define E_HFP		88
#define E_VTotal	1125
#define E_VActive	1080
#define E_VFP		4
#define E_VSyncLen	5
#define E_VBP		36

#endif

/************************/
/* TPG REGISTERS        */

struct TpgConfig
{
	unsigned int BaseAddr;
	int tpgIP_Mode;
	int PatternSel;
	int PatternMotion;
	int FrameHeightLen;
	int ZplateHorz;
	int ZplateVert;
	int BoxSize;
	int BoxColor;
	int ActiveSize;
};
struct TpgConfig TpgData[4];

/************************** Function Prototypes ******************************/
int init_TimeBaseDetection(void);
int init_Timebase(void);
void config_tpg(int tpg, int enable);

/************************** Variable Definitions *****************************/


/*****************************************************************************/
/**
* reset the system
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*Cnt
* @note		None.
*
******************************************************************************/
void activate_vdemo_system(int lsel, int vsel, int ssel)
{
	int VideoMux=0;

	switch(lsel)
	{
		/********** LIVE VIDEO SELECTION **********/
		case LIVEVIDEO_DISABLE:
			//TpgWriteReg(TpgData[0].BaseAddr, TPG_DVI_PATTERN_CONTROL,
			//		(TpgData[0].PatternSel | TPG_DVI_ENABLE_TPG));
			//TpgWriteReg(TpgData[2].BaseAddr, TPG_DVI_PATTERN_CONTROL,
			//		(TpgData[2].PatternSel | TPG_DVI_ENABLE_TPG));
			config_tpg(0, 1);
			config_tpg(2, 1);
			VideoMux=0x0;
			//xil_printf("======== live video disable \r\n");
			break;

		default:	// LIVEVIDEO_ENABLE
			TpgWriteReg(TpgData[0].BaseAddr, TPG_DVI_PATTERN_CONTROL, TPG_DVI_ENABLE_DVI);
			TpgWriteReg(TpgData[2].BaseAddr, TPG_DVI_PATTERN_CONTROL, TPG_DVI_ENABLE_DVI);
			VideoMux=0xF;
			//xil_printf("======== live video enable \r\n");
			break;
	}

	switch(vsel)
	{
		case WS_STREAM0_FULL:
		case WS_STREAM1_FULL:
			/********** FULL STREAM SELECTION **********/
			*((volatile int *)( RST_CR_REG )) = 0xF0 | VideoMux;
			//xil_printf("======== full stream \r\n");
			break;

		case WS_SCALED_STREAMS:
			/********** SCALED STREAM SELECTION **********/
			switch(ssel)
			{
				case WS_STREAM0_SCALED:
					//xil_printf("======== scale 0 \r\n");
					*((volatile int *)( RST_CR_REG )) = 0x10|VideoMux; break;
				case WS_STREAM1_SCALED:
					//xil_printf("======== scale 1 \r\n");
					*((volatile int *)( RST_CR_REG )) = 0x40|VideoMux; break;
				case WS_TPG0_SCALED:
					//xil_printf("======== tpg 0 \r\n");
					*((volatile int *)( RST_CR_REG )) = 0x20|VideoMux; break;
				case WS_TPG2_SCALED:
					//xil_printf("======== tpg 2 \r\n");
					*((volatile int *)( RST_CR_REG )) = 0x80|VideoMux; break;
				case WS_ALL_SCALED:
					//xil_printf("======== all scale \r\n");
					*((volatile int *)( RST_CR_REG )) = 0xF0|VideoMux; break;
			}
	}

	return;
}
void deactivate_vdemo_system(void)
{
	*((volatile int *)( RST_CR_REG )) = 0x00;
	//*((volatile int *)(TIME_BASE_CR)) = 0;
	return;
}
/*****************************************************************************/
/**
* setup time bases (resolutions) of pattern generators.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int init_Timebase(void)
{
	#ifndef NO_VTC
    unsigned int hActive;
    unsigned int hSync_start;
    unsigned int hSync_end;
    unsigned int hTotal;

    unsigned int vActive;
    unsigned int vSync_start;
    unsigned int vSync_end;
    unsigned int vTotal;
    unsigned int vBlankh_start;
    unsigned int vBlankh_end;
    unsigned int vSyncH_start;
    unsigned int vSyncH_end;
	#endif

	#ifdef NO_VTC
  // ***********************************************************************
  // Time Base generator Configuration
  // ***********************************************************************
    *((volatile int *)(TIME_BASE_ACT_SIZE  )) = 0x04380780; // Active size
    *((volatile int *)(TIME_BASE_ENCODE    )) = 0x00000002;	// Video mode, YUV 422
    *((volatile int *)(TIME_BASE_POL       )) = 0x0000003F; // Outputs polarity
    *((volatile int *)(TIME_BASE_HSIZE     )) = 0x00000898; // H size including blanking
    *((volatile int *)(TIME_BASE_VSIZE     )) = 0x00000465; // V size including blanking
    *((volatile int *)(TIME_BASE_HSYNC     )) = 0x080407D8; // hsync duration setting
    *((volatile int *)(TIME_BASE_VBLANK    )) = 0x07800780;	// vblank duration setting
    *((volatile int *)(TIME_BASE_VSYNC     )) = 0x0440043B; // vsynd duration setting
    *((volatile int *)(TIME_BASE_CR        )) = 0x03F5EF06; // Control register, has to be written at last

    #else
    // New VTC
	#if 0
    *((volatile int *)(VTC_GEN_ACT_SIZE)) = 0x04380780;	// vactive-hactive
    *((volatile int *)(VTC_GEN_ENCODE ))  = 0x00000002;	// 0x00000000;
    *((volatile int *)(VTC_GEN_POL))      = 0x0000003F;
    *((volatile int *)(VTC_GEN_HSIZE))    = 0x00000898;	// htotal
    *((volatile int *)(VTC_GEN_VSIZE))    = 0x00000465;	// vtotal
    *((volatile int *)(VTC_GEN_HSYNC))    = 0x080407D8;
    *((volatile int *)(VTC_GEN_VBLANKH))  = 0x07800780;	// 0x04650439;
    *((volatile int *)(VTC_GEN_VSYNC))    = 0x0440043B;	// 0x0441043D;
    *((volatile int *)(VTC_GEN_VSYNCH))   = 0X07D807D8;
    *((volatile int *)(VTC_CR))           = 0x03F5EF04;
	#endif

    // Calculating timing parameters
    hActive = E_HActive;
    hSync_start = hActive + E_HFP;
    hSync_end = hSync_start + E_HSyncLen;
    hTotal = E_HTotal;

    vActive = E_VActive;
    vSync_start = vActive + E_VFP - 1; // one line is componseted with vSyncH offset cycles
    vSync_end = vSync_start + E_VSyncLen;
    vTotal = E_VTotal;
    vBlankh_start = hActive;               // after active lines
    vBlankh_end = hActive;
    vSyncH_start = hActive + E_HFP;
    vSyncH_end =vSyncH_start;

    // Reset the VTC core
    *((volatile int *)(VTC_CR)) = 0x83F5EF04;	// bit31 = SW_RESET
    while(1)
    {
    	// SW_RESET is self clear
    	if ( (*((volatile int *)(VTC_CR)) & 0x80000000) == 0)
    		break;
    }

    *((volatile int *)(VTC_GEN_ACT_SIZE)) = (vActive << 16)| hActive;
    *((volatile int *)(VTC_GEN_ENCODE)) =  0x00000002;
    *((volatile int *)(VTC_GEN_POL)) =  0x0000003F;
    *((volatile int *)(VTC_GEN_HSIZE)) =  hTotal;
    *((volatile int *)(VTC_GEN_VSIZE)) = vTotal;
    *((volatile int *)(VTC_GEN_HSYNC)) = (hSync_end << 16) | hSync_start;
    *((volatile int *)(VTC_GEN_VBLANKH)) = (vBlankh_end << 16) | vBlankh_start;
    *((volatile int *)(VTC_GEN_VSYNC)) = (vSync_end << 16) | vSync_start;
    *((volatile int *)(VTC_GEN_VSYNCH)) = (vSyncH_end << 16) | vSyncH_start;
    *((volatile int *)(VTC_CR)) = 0x03F5EF06;	//0x03F5EF04;

    printf("VTC_GEN_ACT_SIZE: 0x%x\n", *((volatile int *)(VTC_GEN_ACT_SIZE)) );
    printf("VTC_GEN_ENCODE  : 0x%x\n", *((volatile int *)(VTC_GEN_ENCODE )) );
    printf("VTC_GEN_POL     : 0x%x\n", *((volatile int *)(VTC_GEN_POL)) );
    printf("VTC_GEN_HSIZE   : 0x%x\n", *((volatile int *)(VTC_GEN_HSIZE)) );
    printf("VTC_GEN_VSIZE   : 0x%x\n", *((volatile int *)(VTC_GEN_VSIZE)) );
    printf("VTC_GEN_HSYNC   : 0x%x\n", *((volatile int *)(VTC_GEN_HSYNC)) );
    printf("VTC_GEN_VBLANKH : 0x%x\n", *((volatile int *)(VTC_GEN_VBLANKH)) );
    printf("VTC_GEN_VSYNC   : 0x%x\n", *((volatile int *)(VTC_GEN_VSYNC)) );
    printf("VTC_GEN_VSYNCH  : 0x%x\n", *((volatile int *)(VTC_GEN_VSYNCH)) );
    printf("VTC_CR          : 0x%x\n", *((volatile int *)(VTC_CR)) );

    #endif

    // TPG_0 (TPG+DVI)
	TpgData[0].BaseAddr       = XPAR_TPG_2_BASEADDR;
	TpgData[0].tpgIP_Mode     = TPG_IP_TPGDVI;
	TpgData[0].PatternSel     = COLOR_BARS2;
	TpgData[0].PatternMotion  = 0xB;	// 0x15;
	TpgData[0].FrameHeightLen = 0x04380780;
	TpgData[0].ZplateHorz     = 0xff;
	TpgData[0].ZplateVert     = 0xff;
	TpgData[0].BoxSize        = 0x7f;
	TpgData[0].BoxColor       = 0xABCDEFED;
	TpgData[0].ActiveSize     = 0x04380780;

    // TPG_1 (ONLY TPG)
	TpgData[1].BaseAddr       = XPAR_TPG_0_BASEADDR;
	TpgData[1].tpgIP_Mode     = TPG_IP_TPGDVI;	//TPG_IP_TPGONLY;
	TpgData[1].PatternSel     = 0x2a;			//ZONEPLATE_SWEEP;
	TpgData[1].PatternMotion  = 0xB;			//0x13;
	TpgData[1].FrameHeightLen = 0x04380780;
	TpgData[1].ZplateHorz     = 0x4a;			//0xFF;
	TpgData[1].ZplateVert     = 0x03;			//0xFF;
	TpgData[1].BoxSize        = 0x4f;			//0x8F;
	TpgData[1].BoxColor       = 0x76543210;		//0x76543210;
	TpgData[1].ActiveSize     = 0x04380780;

    // TPG_2 (TPG+DVI)
	TpgData[2].BaseAddr       = XPAR_TPG_3_BASEADDR;
	TpgData[2].tpgIP_Mode     = TPG_IP_TPGDVI;
	TpgData[2].PatternSel     = ZONEPLATE_SWEEP2;
	TpgData[2].PatternMotion  = 0xB;	// 0x17;
	TpgData[2].FrameHeightLen = 0x04380780;
	TpgData[2].ZplateHorz     = 0x2F;
	TpgData[2].ZplateVert     = 0xFFF;
	TpgData[2].BoxSize        = 0x8F;
	TpgData[2].BoxColor       = 0xFFFF5566;
	TpgData[2].ActiveSize     = 0x04380780;

    // TPG_3 (Only TPG)
	TpgData[3].BaseAddr       = XPAR_TPG_1_BASEADDR;
	TpgData[3].tpgIP_Mode     = TPG_IP_TPGDVI;	//TPG_IP_TPGONLY;
	TpgData[3].PatternSel     = 0x2a;			//ZONEPLATE_SWEEP;
	TpgData[3].PatternMotion  = 0xB;					//0x13;
	TpgData[3].FrameHeightLen = 0x04380780;
	TpgData[3].ZplateHorz     = 0x1;				//0x2F;
	TpgData[3].ZplateVert     = 0x1;					//0xfff;
	TpgData[3].BoxSize        = 0x6F;					//0x8F;
	TpgData[3].BoxColor       = 0x76543210;
	TpgData[3].ActiveSize     = 0x04380780;

    // Test Pattern Generator Configuration
	config_tpg(0, 1);		// default enable
	config_tpg(1, 1);		// default disable (DVI_0_IN in case of Live Video)
	config_tpg(2, 1);		// default enable
    config_tpg(3, 1);		// default disable (DVI_1_IN in case of Live Video)

	return 0;
}

void config_tpg(int tpg, int enable)
{
	unsigned int base;
	int pattern = TpgData[tpg].PatternSel;

	base = TpgData[tpg].BaseAddr;

	switch(TpgData[tpg].tpgIP_Mode)
	{
		case  TPG_IP_TPGONLY:
				if(enable == 0)
				{
					TpgWriteReg(base, TPG_ONLY_PAT_SEL, 0x0);
				}
				else
				{
					pattern &= 0xf;
					pattern |= 0x200;
					TpgWriteReg(base, TPG_ONLY_PAT_SEL, pattern);
				}

				TpgWriteReg(base, TPG_ONLY_PAT_MOTION, TpgData[tpg].PatternMotion);
				TpgWriteReg(base, TPG_ONLY_FRAME_HLEN, TpgData[tpg].FrameHeightLen);
				TpgWriteReg(base, TPG_ONLY_ZPLATE_H,   TpgData[tpg].ZplateHorz);
				TpgWriteReg(base, TPG_ONLY_ZPLATE_V,   TpgData[tpg].ZplateVert);
				TpgWriteReg(base, TPG_ONLY_BOX_SIZE,   TpgData[tpg].BoxSize);
				TpgWriteReg(base, TPG_ONLY_BOX_COLOUR, TpgData[tpg].BoxColor);
				break;

		case TPG_IP_TPGDVI:
			if(enable == 0)
			{
				TpgWriteReg(base, TPG_DVI_PATTERN_CONTROL, TPG_DVI_ENABLE_DVI);
			}
			TpgWriteReg(base, TPG_DVI_CONTROL,            0x00000001);
			TpgWriteReg(base, TPG_DVI_PATTERN_CONTROL,    (TpgData[tpg].PatternSel | TPG_DVI_ENABLE_DVI));
			TpgWriteReg(base, TPG_DVI_MOTION_SPEED,       TpgData[tpg].PatternMotion);
			TpgWriteReg(base, TPG_DVI_ACTIVE_SIZE,        TpgData[tpg].ActiveSize);
			TpgWriteReg(base, TPG_DVI_ZPLATE_HOR_CONTROL, TpgData[tpg].ZplateHorz);
			TpgWriteReg(base, TPG_DVI_ZPLATE_VER_CONTROL, TpgData[tpg].ZplateVert);
			TpgWriteReg(base, TPG_DVI_BOX_SIZE,           TpgData[tpg].BoxSize);
			TpgWriteReg(base, TPG_DVI_BOX_COLOR,          TpgData[tpg].BoxColor);
			TpgWriteReg(base, TPG_DVI_CONTROL,            0x00000003);
			break;
	}

	return;
}

int reset_TPG(void)
{
	// apply software reset
	TpgWriteReg(TpgData[0].BaseAddr, TPG_DVI_CONTROL, 0x80000000);
	TpgWriteReg(TpgData[1].BaseAddr, TPG_DVI_CONTROL, 0x80000000);
	TpgWriteReg(TpgData[2].BaseAddr, TPG_DVI_CONTROL, 0x80000000);
	TpgWriteReg(TpgData[3].BaseAddr, TPG_DVI_CONTROL, 0x80000000);

	// clear software reset
	TpgWriteReg(TpgData[0].BaseAddr, TPG_DVI_CONTROL, 0x1);
	TpgWriteReg(TpgData[1].BaseAddr, TPG_DVI_CONTROL, 0x1);
	TpgWriteReg(TpgData[2].BaseAddr, TPG_DVI_CONTROL, 0x1);
	TpgWriteReg(TpgData[3].BaseAddr, TPG_DVI_CONTROL, 0x1);

    // TPG_0 (TPG+DVI)
	TpgData[0].BaseAddr       = XPAR_TPG_2_BASEADDR;
	TpgData[0].tpgIP_Mode     = TPG_IP_TPGDVI;
	TpgData[0].PatternSel     = COLOR_BARS2;
	TpgData[0].PatternMotion  = 0xB;	// 0x15;
	TpgData[0].FrameHeightLen = 0x04380780;
	TpgData[0].ZplateHorz     = 0xff;
	TpgData[0].ZplateVert     = 0xff;
	TpgData[0].BoxSize        = 0x7f;
	TpgData[0].BoxColor       = 0xABCDEFED;
	TpgData[0].ActiveSize     = 0x04380780;

    // TPG_1 (ONLY TPG)
	TpgData[1].BaseAddr       = XPAR_TPG_0_BASEADDR;
	TpgData[1].tpgIP_Mode     = TPG_IP_TPGDVI;	//TPG_IP_TPGONLY;
	TpgData[1].PatternSel     = 0x2a;			//ZONEPLATE_SWEEP;
	TpgData[1].PatternMotion  = 0xB;			//0x13;
	TpgData[1].FrameHeightLen = 0x04380780;
	TpgData[1].ZplateHorz     = 0x4a;			//0xFF;
	TpgData[1].ZplateVert     = 0x03;			//0xFF;
	TpgData[1].BoxSize        = 0x4f;			//0x8F;
	TpgData[1].BoxColor       = 0x76543210;		//0x76543210;
	TpgData[1].ActiveSize     = 0x04380780;

    // TPG_2 (TPG+DVI)
	TpgData[2].BaseAddr       = XPAR_TPG_3_BASEADDR;
	TpgData[2].tpgIP_Mode     = TPG_IP_TPGDVI;
	TpgData[2].PatternSel     = ZONEPLATE_SWEEP2;
	TpgData[2].PatternMotion  = 0xB;	// 0x17;
	TpgData[2].FrameHeightLen = 0x04380780;
	TpgData[2].ZplateHorz     = 0x2F;
	TpgData[2].ZplateVert     = 0xFFF;
	TpgData[2].BoxSize        = 0x8F;
	TpgData[2].BoxColor       = 0xFFFF5566;
	TpgData[2].ActiveSize     = 0x04380780;

    // TPG_3 (Only TPG)
	TpgData[3].BaseAddr       = XPAR_TPG_1_BASEADDR;
	TpgData[3].tpgIP_Mode     = TPG_IP_TPGDVI;	//TPG_IP_TPGONLY;
	TpgData[3].PatternSel     = 0x2a;			//ZONEPLATE_SWEEP;
	TpgData[3].PatternMotion  = 0xB;					//0x13;
	TpgData[3].FrameHeightLen = 0x04380780;
	TpgData[3].ZplateHorz     = 0x1;				//0x2F;
	TpgData[3].ZplateVert     = 0x1;					//0xfff;
	TpgData[3].BoxSize        = 0x6F;					//0x8F;
	TpgData[3].BoxColor       = 0x76543210;
	TpgData[3].ActiveSize     = 0x04380780;

    // Test Pattern Generator Configuration
	config_tpg(0, 1);		// default enable
	config_tpg(1, 1);		// default disable (DVI_0_IN in case of Live Video)
	config_tpg(2, 1);		// default enable
    config_tpg(3, 1);		// default disable (DVI_1_IN in case of Live Video)

	return 0;
}

void set_hdmi_output_color(void)
{
	// FOR HDMI output selection to Normal
	*((volatile int *)(XPAR_GPIO_3_BASEADDR)) = 0; //0x100;
	return;
}

int init_TimeBaseDetection(void)
{
   //int x,y;

    // Reset input channels
    //*((volatile int *)( RST_CR_REG)) = 0xFFFFFFFF;

	#if 0
  // ***********************************************************************
  // DVI_0 Input Video detection
  // ***********************************************************************
    *((volatile int *)(TIME_BASE_DET_0_CR)) = 0x00000002;   // Enable Detection

     xil_printf("\n *********************************************************\r");
     xil_printf("\n ***************** Video Input Detectioin ****************\r");
     xil_printf("\n *********************************************************\r\n ");
     x =   *((volatile int *)(TIME_BASE_DET_0_SR ));
     y = x&&0x400000;
     xil_printf("DVI_0 Vsync Polarity : %d \r\n " , y);
     y = x&&0x100000;
     xil_printf("DVI_0 Hsync Polarity : %d \r\n " , y);
     y = x&&0x2000000;
     xil_printf("DVI_0 DE    Polarity : %d \r\n " , y);

     x =   *((volatile int *)(TIME_BASE_DET_0_HORZ_0 ));
     x = x >> 16;
     y =   *((volatile int *)(TIME_BASE_DET_0_HORZ_2 ));
     xil_printf("DVI_0 Horizontal resolution : %d \r\n " , x-y);


     x =   *((volatile int *)(TIME_BASE_DET_0_VERT_0 ));
     x = x >> 16;
     y =   *((volatile int *)(TIME_BASE_DET_0_VERT_2 ));
     xil_printf("DVI_0 Vertical resolution   : %d \r\n " , x-y);

  // ***********************************************************************
  // DVI_1 Input Video detection
  // ***********************************************************************
    *((volatile int *)(TIME_BASE_DET_1_CR         )) = 0x00000002;   // Enable Detection

    //x =   *((volatile int *)(TIME_BASE_DET_1_SR ));
    x =   *((volatile int *)(0x50800028 ));
     y = x&&0x400000;
     xil_printf("\nDVI_1 Vsync Polarity : %d \r\n " , y);
     y = x&&0x100000;
     xil_printf("DVI_1 Hsync Polarity : %d \r\n " , y);
     y = x&&0x2000000;
     xil_printf("DVI_1 DE    Polarity : %d \r\n " , y);

     x =   *((volatile int *)(TIME_BASE_DET_1_HORZ_0 ));
     x = x >> 16;
     y =   *((volatile int *)(TIME_BASE_DET_1_HORZ_2 ));
     xil_printf("DVI_1 Horizontal resolution : %d \r\n " , x-y);


     x =   *((volatile int *)(TIME_BASE_DET_1_VERT_0 ));
     x = x >> 16;
     y =   *((volatile int *)(TIME_BASE_DET_1_VERT_2 ));
     xil_printf("DVI_1 Vertical resolution   : %d \r\n " , x-y);
	#endif

     return 0;
}

