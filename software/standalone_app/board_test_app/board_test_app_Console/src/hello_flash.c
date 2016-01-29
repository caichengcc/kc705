
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
*       (c) Copyright 2010 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/

/*****************************************************************************/
/**
* @file hello_flash.c
*
* @note     None.
*
* <pre>
*
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- --------------------------------------------------------
* </pre>
* Created on 2,Feb,2012 srinivasa attili; for Kintex7 Demo
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "hello_flash.h"
#include "board_test_app.h"
#include "xutil.h"
#include <stdlib.h>
#include <stdio.h>
#include "xuartns550_l.h"
/************************** Constant Definitions *****************************/

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/*
 * Instance of the XIntc driver.
 */
XIntc InterruptController;

/************************** Main Program *************************************/

int hello_flash()
{
    int Status;

    /*
     * Print Test Section Title.
     */
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r**     KC705 - FLASH Test                             **");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************\r\n");

    Status = FlashProtectionExample();
    if (Status) {
        xil_printf("Flash test failed (%d)\r\n", Status);
    }

    return 0;
}


