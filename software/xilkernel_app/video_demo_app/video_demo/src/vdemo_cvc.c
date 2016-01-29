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
* File:	  vdemo_cvc.c
* Author: srinivasa attili
* Date:	  7-sept-2011
*
* Purpose:	This file handles the video controller (Xylon CVC).
*   The CVC is Compact Video Controller from Xylon and supports many features.
*   This controller's driver is yet to be ported into Kintex-7.
*   Compliant to Xilinx Standalone BSP / XilKernel BSP.
*
* MODIFICATION HISTORY:
*
* Ver   Who               Date     Changes
* ----- --------------   -------- -----------------------------------
* 1.00  srinvasa attili   9-7-11  Initial Release
* 2.00  srinivasa attili  2-2-12  modified for Kintex7 Demo TRD
*
**************************************************************************/

/***************************** Include Files *********************************/
#include <stdio.h>
#include "xparameters.h"
#include "board_test_app.h"
#include "vdma_data.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

#if 0
// Register definitions.. Ideally should be part of BSP.
#define RST_CR_REG   	                     0x40020000
#define CVC_HSY_FP                           0x77000000
#define CVC_HSY_WIDTH                        0x77000008
#define CVC_HSY_BP                           0x77000010
#define CVC_HSY_RESOLUTION                   0x77000018
#define CVC_VSY_FP                           0x77000020
#define CVC_VSY_WIDTH                        0x77000028
#define CVC_VSY_BP                           0x77000030
#define CVC_VSY_RESOLUTION                   0x77000038
#define CVC_CTRL                             0x77000040
#define CVC_DTYPE                            0x77000048
#define CVC_BACKGROUND                       0x77000050
#define CVC_VBUFF_SEL                        0x77000058
#define CVC_CLUT_SEL                         0x77000060
#define CVC_INT_STAT                         0x77000068
#define CVC_INT_MASK                         0x77000070
#define CVC_PWRCTRL                          0x77000078
#define CVC_L0_H_OFFSET                      0x77000100
#define CVC_L0_V_OFFSET                      0x77000108
#define CVC_L0_H_POSITION                    0x77000110
#define CVC_L0_V_POSITION                    0x77000118
#define CVC_L0_WIDTH                         0x77000120
#define CVC_L0_HEIGHT                        0x77000128
#define CVC_L0_ALPHA                         0x77000130
#define CVC_L0_CTRL                          0x77000138
#define CVC_L0_TRANSPARENT                   0x77000140
#define CVC_L1_H_OFFSET                      0x77000180
#define CVC_L1_V_OFFSET                      0x77000188
#define CVC_L1_H_POSITION                    0x77000190
#define CVC_L1_V_POSITION                    0x77000198
#define CVC_L1_WIDTH                         0x770001A0
#define CVC_L1_HEIGHT                        0x770001A8
#define CVC_L1_ALPHA                         0x770001B0
#define CVC_L1_CTRL                          0x770001B8
#define CVC_L1_TRANSPARENT                   0x770001C0
#define CVC_L2_H_OFFSET                      0x77000200
#define CVC_L2_V_OFFSET                      0x77000208
#define CVC_L2_H_POSITION                    0x77000210
#define CVC_L2_V_POSITION                    0x77000218
#define CVC_L2_WIDTH                         0x77000220
#define CVC_L2_HEIGHT                        0x77000228
#define CVC_L2_ALPHA                         0x77000230
#define CVC_L2_CTRL                          0x77000238
#define CVC_L2_TRANSPARENT                   0x77000240
#define CVC_L3_H_OFFSET                      0x77000280
#define CVC_L3_V_OFFSET                      0x77000288
#define CVC_L3_H_POSITION                    0x77000290
#define CVC_L3_V_POSITION                    0x77000298
#define CVC_L3_WIDTH                         0x770002A0
#define CVC_L3_HEIGHT                        0x770002A8
#define CVC_L3_ALPHA                         0x770002B0
#define CVC_L3_CTRL                          0x770002B8
#define CVC_L3_TRANSPARENT                   0x770002C0
#define CVC_L4_CTRL                          0x77000338
#endif

/************************** Function Prototypes ******************************/
int init_CVC(void);
void shutdown_CVC(void);
int config_cvc_fullVideo_0(void);
int config_cvc_fullVideo_1(void);
int config_cvc_scaleVideo_0(void);
int config_cvc_scaleVideo_1(void);
int config_cvc_tpgVideo_0(void);
int config_cvc_tpgVideo_1(void);
int config_cvc_scaleVideo_both(void);


/************************** Variable Definitions *****************************/
//logiCVC logiCVC_instance;

/*****************************************************************************/
/**
* Initialize the CVC instance.
* Create the CVC data structure and initialize the elements.
* Reset the CVC to default state.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int init_CVC(void)
{
   /*************************************************/
   /*********** CVC programming ********************/
    *((volatile int *)( CVC_HSY_FP         )) = 0x00000057;  // (110  - 1 )= (0X6E -1) = 0X6D
    *((volatile int *)( CVC_HSY_WIDTH      )) = 0x0000002B;  // (40   - 1 )= (0X28 -1) = 0X27
    *((volatile int *)( CVC_HSY_BP         )) = 0x00000093;  // (220  - 1 )= (0XDC -1) = 0XDB
    *((volatile int *)( CVC_HSY_RESOLUTION )) = 0x0000077F;  // (1920 - 1 )= (0X500-1) = 0X4FF
    *((volatile int *)( CVC_VSY_FP         )) = 0x00000000;  // (5    - 1 )= (0X05 -1) = 0X04
    *((volatile int *)( CVC_VSY_WIDTH      )) = 0x00000000;  // (5    - 1 )= (0X05 -1) = 0X04
    *((volatile int *)( CVC_VSY_BP         )) = 0x00000000;  // (20   - 1 )= (0X14 -1) = 0X13
    *((volatile int *)( CVC_VSY_RESOLUTION )) = 0x00000013;  // (1080  - 1 )= (0X438-1) = 0X2CF
    *((volatile int *)( CVC_CTRL           )) = 0x00000115;  // HSYNC_EN =1, VSYNC_EN=1, DE_EN = 1

   *((volatile int *)( CVC_L0_CTRL        )) = 0x00000000;
   *((volatile int *)( CVC_L1_CTRL        )) = 0x00000000;  // lAYER_1_EN =1
   *((volatile int *)( CVC_L2_CTRL        )) = 0x00000000;  // lAYER_2_EN =1
   *((volatile int *)( CVC_L3_CTRL        )) = 0x00000000;  // lAYER_2_EN =1
   *((volatile int *)( CVC_DTYPE          )) = 0x00000000;

    //*((volatile int *)( RST_CR_REG )) = 0xF0;

    return XST_SUCCESS;
}

/*****************************************************************************/
/**
* Stop the CVC controller. All display layers are blanked out.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void shutdown_CVC(void)
{
	int y;

	*((volatile int *)( CVC_L0_CTRL        )) = 0x00000000;  // lAYER_0_EN =1
	*((volatile int *)( CVC_L1_CTRL        )) = 0x00000000;  // lAYER_1_EN =1
	*((volatile int *)( CVC_L2_CTRL        )) = 0x00000000;  // lAYER_2_EN =1
	*((volatile int *)( CVC_L3_CTRL        )) = 0x00000000;  // lAYER_3_EN =0
	*((volatile int *)( CVC_L4_CTRL        )) = 0x00000003;  // lAYER_4_EN =1
	*((volatile int *)( CVC_BACKGROUND     )) = 0x00123456;  //
	*((volatile int *)( CVC_DTYPE          )) = 0x00000000;  // PAL

    y = 0;
	print("***************************\r\n");
	print("********** B Y E **********\r\n");
	print("***************************\r\n");

	//print("New color changed \r\n");
	y++;
	if (y == 0)
		*((volatile int *)( CVC_BACKGROUND     )) = 0x00fff000;

	if (y == 1)
		*((volatile int *)( CVC_BACKGROUND     )) = 0x0000ff00;

	if (y == 2)
		*((volatile int *)( CVC_BACKGROUND     )) = 0x000000ff;

	if (y == 3)
	{
		*((volatile int *)( CVC_BACKGROUND     )) = 0x00123456;
		y = 0;
	}

	return;
}

/*****************************************************************************/
/**
* setup the CVC to display an input video stream (full video stream 0).
* the resolution used in this case is: 1920x1080 i.e. 1080p, i.e. Full HD.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
#if 0
int config_cvc_fullVideo_0(void)
{
  // ***********************************************************************
  // Configure Logic CVC
  // ***********************************************************************
  // General
    *((volatile int *)( CVC_HSY_FP         )) = 0x00000057;    // (88   - 1 )= (0X58 -1) = 0X57        
    *((volatile int *)( CVC_HSY_WIDTH      )) = 0x0000002B;    // (44   - 1 )= (0X2C -1) = 0X2B        
    *((volatile int *)( CVC_HSY_BP         )) = 0x00000093;    // (148  - 1 )= (0X94 -1) = 0X93        
    *((volatile int *)( CVC_HSY_RESOLUTION )) = 0x0000077F;    // (1920 - 1 )= (0X780-1) = 0X77F       
    *((volatile int *)( CVC_VSY_FP         )) = 0x00000003;    // (4    - 1 )= (0X04 -1) = 0X03        
    *((volatile int *)( CVC_VSY_WIDTH      )) = 0x00000004;    // (5    - 1 )= (0X05 -1) = 0X04        
    *((volatile int *)( CVC_VSY_BP         )) = 0x00000023;    // (36   - 1 )= (0X24 -1) = 0X23        
    *((volatile int *)( CVC_VSY_RESOLUTION )) = 0x00000437;    // (1080 - 1 )= (0X438-1) = 0X437       
    *((volatile int *)( CVC_CTRL           )) = 0x00000015;    // HSYNC_EN =1, VSYNC_EN=1, DE_EN = 1   

  // Layer -0                                                 
    *((volatile int *)( CVC_L0_H_OFFSET    )) = 0x00000000;   
    *((volatile int *)( CVC_L0_V_OFFSET    )) = 0x00000000;   
    *((volatile int *)( CVC_L0_H_POSITION  )) = 0x0000077F;   
    *((volatile int *)( CVC_L0_V_POSITION  )) = 0x00000437;   
    *((volatile int *)( CVC_L0_WIDTH       )) = 0x0000077F;   
    *((volatile int *)( CVC_L0_HEIGHT      )) = 0x00000437;   
    *((volatile int *)( CVC_L0_CTRL        )) = 0x00000007;   
    *((volatile int *)( CVC_BACKGROUND     )) = 0x00123456;   
                                                              
    *((volatile int *)( CVC_L4_CTRL        )) = 0x00000003;   

    xil_printf("config_cvc_fullVideo_0 started\r\n");
    return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
* setup the CVC to display an input video stream (full video stream 1).
* the resolution used in this case is: 1920x1080 i.e. 1080p, i.e. Full HD.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
#if 0
int config_cvc_fullVideo_1(void)
{
	// ***********************************************************************
	// Configure Logic CVC
	// ***********************************************************************
	// General
	 *((volatile int *)( CVC_HSY_FP         )) = 0x00000057;    // (88   - 1 )= (0X58 -1) = 0X57
	 *((volatile int *)( CVC_HSY_WIDTH      )) = 0x0000002B;    // (44   - 1 )= (0X2C -1) = 0X2B
	 *((volatile int *)( CVC_HSY_BP         )) = 0x00000093;    // (148  - 1 )= (0X94 -1) = 0X93
	 *((volatile int *)( CVC_HSY_RESOLUTION )) = 0x0000077F;    // (1920 - 1 )= (0X780-1) = 0X77F
	 *((volatile int *)( CVC_VSY_FP         )) = 0x00000003;    // (4    - 1 )= (0X04 -1) = 0X03
	 *((volatile int *)( CVC_VSY_WIDTH      )) = 0x00000004;    // (5    - 1 )= (0X05 -1) = 0X04
	 *((volatile int *)( CVC_VSY_BP         )) = 0x00000023;    // (36   - 1 )= (0X24 -1) = 0X23
	 *((volatile int *)( CVC_VSY_RESOLUTION )) = 0x00000437;    // (1080 - 1 )= (0X438-1) = 0X437
	 *((volatile int *)( CVC_CTRL           )) = 0x00000015;    // HSYNC_EN =1, VSYNC_EN=1, DE_EN = 1

   // Layer -1
     *((volatile int *)( CVC_L2_H_OFFSET    )) = 0x00000000;
     *((volatile int *)( CVC_L2_V_OFFSET    )) = 0x00000000;
     *((volatile int *)( CVC_L2_H_POSITION  )) = 0x0000077F;
     *((volatile int *)( CVC_L2_V_POSITION  )) = 0x00000437;
     *((volatile int *)( CVC_L2_WIDTH       )) = 0x0000077F;
     *((volatile int *)( CVC_L2_HEIGHT      )) = 0x00000437;
     *((volatile int *)( CVC_L2_CTRL        )) = 0x00000007;

     *((volatile int *)( CVC_L4_CTRL        )) = 0x00000003;

	xil_printf("config_cvc_fullVideo_1 started\r\n");
	return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
* setup the CVC to display an input video stream (full video stream 0).
* it is Scaled down image, displayed on Top Left corner (corner #0).
* the resolution used in this case is: 960x540
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
#if 0
int config_cvc_scaleVideo_0(void)
{
  // ***********************************************************************
  // Configure Logic CVC
  // ***********************************************************************
  // General
	*((volatile int *)( CVC_HSY_FP         )) = 0x00000057;    // (88   - 1 )= (0X58 -1) = 0X57
	*((volatile int *)( CVC_HSY_WIDTH      )) = 0x0000002B;    // (44   - 1 )= (0X2C -1) = 0X2B
	*((volatile int *)( CVC_HSY_BP         )) = 0x00000093;    // (148  - 1 )= (0X94 -1) = 0X93
	*((volatile int *)( CVC_HSY_RESOLUTION )) = 0x0000077F;    // (1920 - 1 )= (0X780-1) = 0X77F
	*((volatile int *)( CVC_VSY_FP         )) = 0x00000003;    // (4    - 1 )= (0X04 -1) = 0X03
	*((volatile int *)( CVC_VSY_WIDTH      )) = 0x00000004;    // (5    - 1 )= (0X05 -1) = 0X04
	*((volatile int *)( CVC_VSY_BP         )) = 0x00000023;    // (36   - 1 )= (0X24 -1) = 0X23
	*((volatile int *)( CVC_VSY_RESOLUTION )) = 0x00000437;    // (1080 - 1 )= (0X438-1) = 0X437
	*((volatile int *)( CVC_CTRL           )) = 0x00000015;    // HSYNC_EN =1, VSYNC_EN=1, DE_EN = 1
	*((volatile int *)( CVC_BACKGROUND     )) = 0x00123456;

  // Layer -0
	*((volatile int *)( CVC_L0_H_OFFSET    )) = 0x00000000;
	*((volatile int *)( CVC_L0_V_OFFSET    )) = 0x00000000;
	*((volatile int *)( CVC_L0_H_POSITION  )) = 0x0000077F;
	*((volatile int *)( CVC_L0_V_POSITION  )) = 0x00000437;
	*((volatile int *)( CVC_L0_WIDTH       )) = 0x000003BF;
	*((volatile int *)( CVC_L0_HEIGHT      )) = 0x0000021B;
	*((volatile int *)( CVC_L0_CTRL        )) = 0x00000007;


	*((volatile int *)( CVC_L4_CTRL        )) = 0x00000003;

	xil_printf("config_cvc_scaleVideo_0 started\r\n");
    return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
* setup the CVC to display an input video stream (full video stream 1).
* it is Scaled down image, displayed on Bottom Right corner (corner #3).
* the resolution used in this case is: 960x540
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
#if 0
int config_cvc_scaleVideo_1(void)
{
	// ***********************************************************************
	// Configure Logic CVC
	// ***********************************************************************
	// General
	  *((volatile int *)( CVC_HSY_FP         )) = 0x00000057;    // (88   - 1 )= (0X58 -1) = 0X57
	  *((volatile int *)( CVC_HSY_WIDTH      )) = 0x0000002B;    // (44   - 1 )= (0X2C -1) = 0X2B
	  *((volatile int *)( CVC_HSY_BP         )) = 0x00000093;    // (148  - 1 )= (0X94 -1) = 0X93
	  *((volatile int *)( CVC_HSY_RESOLUTION )) = 0x0000077F;    // (1920 - 1 )= (0X780-1) = 0X77F
	  *((volatile int *)( CVC_VSY_FP         )) = 0x00000003;    // (4    - 1 )= (0X04 -1) = 0X03
	  *((volatile int *)( CVC_VSY_WIDTH      )) = 0x00000004;    // (5    - 1 )= (0X05 -1) = 0X04
	  *((volatile int *)( CVC_VSY_BP         )) = 0x00000023;    // (36   - 1 )= (0X24 -1) = 0X23
	  *((volatile int *)( CVC_VSY_RESOLUTION )) = 0x00000437;    // (1080 - 1 )= (0X438-1) = 0X437
	  *((volatile int *)( CVC_CTRL           )) = 0x00000015;    // HSYNC_EN =1, VSYNC_EN=1, DE_EN = 1
	  *((volatile int *)( CVC_BACKGROUND     )) = 0x00123456;


	// Layer -2
	  *((volatile int *)( CVC_L2_H_OFFSET    )) = 0x00000000;
	  *((volatile int *)( CVC_L2_V_OFFSET    )) = 0x00000000;
	  *((volatile int *)( CVC_L2_H_POSITION  )) = 0x000003BF;
	  *((volatile int *)( CVC_L2_V_POSITION  )) = 0x0000021B;
	  *((volatile int *)( CVC_L2_WIDTH       )) = 0x000003BF;
	  *((volatile int *)( CVC_L2_HEIGHT      )) = 0x0000021B;
	  *((volatile int *)( CVC_L2_CTRL        )) = 0x00000007;

	  *((volatile int *)( CVC_L4_CTRL        )) = 0x00000003;

	xil_printf("config_cvc_scaleVideo_1 started\r\n");
    return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
* setup the CVC to display stream from Test Pattern Generator (tpg 0).
* it is Scaled down image, displayed on Bottom Left corner (corner #2).
* the resolution used in this case is: 960x540
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
#if 0
int config_cvc_tpgVideo_0(void)
{
  // ***********************************************************************
  // Configure Logic CVC
  // ***********************************************************************
  // General
	*((volatile int *)( CVC_HSY_FP         )) = 0x00000057;    // (88   - 1 )= (0X58 -1) = 0X57
	*((volatile int *)( CVC_HSY_WIDTH      )) = 0x0000002B;    // (44   - 1 )= (0X2C -1) = 0X2B
	*((volatile int *)( CVC_HSY_BP         )) = 0x00000093;    // (148  - 1 )= (0X94 -1) = 0X93
	*((volatile int *)( CVC_HSY_RESOLUTION )) = 0x0000077F;    // (1920 - 1 )= (0X780-1) = 0X77F
	*((volatile int *)( CVC_VSY_FP         )) = 0x00000003;    // (4    - 1 )= (0X04 -1) = 0X03
	*((volatile int *)( CVC_VSY_WIDTH      )) = 0x00000004;    // (5    - 1 )= (0X05 -1) = 0X04
	*((volatile int *)( CVC_VSY_BP         )) = 0x00000023;    // (36   - 1 )= (0X24 -1) = 0X23
	*((volatile int *)( CVC_VSY_RESOLUTION )) = 0x00000437;    // (1080 - 1 )= (0X438-1) = 0X437
	*((volatile int *)( CVC_CTRL           )) = 0x00000015;    // HSYNC_EN =1, VSYNC_EN=1, DE_EN = 1

  // Layer -1
	*((volatile int *)( CVC_L1_H_OFFSET    )) = 0x00000000;
	*((volatile int *)( CVC_L1_V_OFFSET    )) = 0x00000000;
	*((volatile int *)( CVC_L1_H_POSITION  )) = 0x0000077F;
	*((volatile int *)( CVC_L1_V_POSITION  )) = 0x0000021B;
	*((volatile int *)( CVC_L1_WIDTH       )) = 0x000003BF;
	*((volatile int *)( CVC_L1_HEIGHT      )) = 0x0000021B;
	*((volatile int *)( CVC_L1_CTRL        )) = 0x00000007;

	*((volatile int *)( CVC_L4_CTRL        )) = 0x00000003;

	xil_printf("config_cvc_tpgVideo_0 started\r\n");
    return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
* setup the CVC to display stream from Test Pattern Generator (tpg 1).
* it is Scaled down image, displayed on Bottom Left corner (corner #1).
* the resolution used in this case is: 960x540
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
#if 0
int config_cvc_tpgVideo_1(void)
{
  // ***********************************************************************
  // Configure Logic CVC
  // ***********************************************************************
  // General
	*((volatile int *)( CVC_HSY_FP         )) = 0x00000057;    // (88   - 1 )= (0X58 -1) = 0X57
	*((volatile int *)( CVC_HSY_WIDTH      )) = 0x0000002B;    // (44   - 1 )= (0X2C -1) = 0X2B
	*((volatile int *)( CVC_HSY_BP         )) = 0x00000093;    // (148  - 1 )= (0X94 -1) = 0X93
	*((volatile int *)( CVC_HSY_RESOLUTION )) = 0x0000077F;    // (1920 - 1 )= (0X780-1) = 0X77F
	*((volatile int *)( CVC_VSY_FP         )) = 0x00000003;    // (4    - 1 )= (0X04 -1) = 0X03
	*((volatile int *)( CVC_VSY_WIDTH      )) = 0x00000004;    // (5    - 1 )= (0X05 -1) = 0X04
	*((volatile int *)( CVC_VSY_BP         )) = 0x00000023;    // (36   - 1 )= (0X24 -1) = 0X23
	*((volatile int *)( CVC_VSY_RESOLUTION )) = 0x00000437;    // (1080 - 1 )= (0X438-1) = 0X437
	*((volatile int *)( CVC_CTRL           )) = 0x00000015;    // HSYNC_EN =1, VSYNC_EN=1, DE_EN = 1
	*((volatile int *)( CVC_BACKGROUND     )) = 0x00123456;

  // Layer -3
	 *((volatile int *)( CVC_L3_H_OFFSET    )) = 0x00000000;
	 *((volatile int *)( CVC_L3_V_OFFSET    )) = 0x00000000;
	 *((volatile int *)( CVC_L3_H_POSITION  )) = 0x000003BF;
	 *((volatile int *)( CVC_L3_V_POSITION  )) = 0x00000437;
	 *((volatile int *)( CVC_L3_WIDTH       )) = 0x000003BF;
	 *((volatile int *)( CVC_L3_HEIGHT      )) = 0x0000021B;
	 *((volatile int *)( CVC_L3_CTRL        )) = 0x00000007;

	*((volatile int *)( CVC_L4_CTRL        )) = 0x00000003;

	xil_printf("config_cvc_tpgVideo_1 started\r\n");
    return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
* setup the CVC to display FOUR streams
*    input video stream #0, res 960x540, Top Left corner (corner #0)
*    input video stream #1, res 960x540, Bottom Right corner (corner #2)
*    Test Pattern Generator (tpg 0), res 960x540, Top Right corner (corner #1).
*    Test Pattern Generator (tpg 1), res 960x540, Bottom Right corner (corner #3).
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
#if 0
int config_cvc_scaleVideo_both(void)
{
	config_cvc_scaleVideo_0();
	config_cvc_scaleVideo_1();
	config_cvc_tpgVideo_0();
	config_cvc_tpgVideo_1();

	xil_printf("config_cvc_scaleVideo_both started\r\n");
    return XST_SUCCESS;
}
#endif

