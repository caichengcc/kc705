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
* Filename:     board_test_app.h
*
* Description:
* APIs for the board tests.
*
* MODIFICATION HISTORY:
*
* Ver   Who               Date          Changes
* ----- ----------------  ------        ---------------------------------
* 1.00  srinivasa attili  11-June-2012  made for Kintex-7
*                                       consolidated all interfaces here
*
******************************************************************************/
#ifndef BOARD_TEST_APP_H_
#define BOARD_TEST_APP_H_

/****************************************************************/
/***** MACROS common to all                                     */
/****************************************************************/
//#define	print	xil_printf

/****************************************************************/
/*****  APIs for other interfaces                               */
/****************************************************************/
extern int main_automate(void);
extern int main_user_interact(void);

/****************************************************************/
/***** Application Program Interfaces (APIs) for peripherals    */
/****************************************************************/
extern char inbyte(void );							// Serial port/Console
//extern void xil_printf(const char *ctrl1, ...);		// Formatted Console
extern int hello_rotary(void);						// Rotary switch

extern int lcd_simple(void);						// LCD access
extern int kc_initLCD(void);
extern int kc_LCDPrintString (char* str1, char* str2);

extern int hello_bram (void);						// BRAM access
extern int hello_mem_complete (void);				// DDR3 test

extern int hello_flash();							// FLASH access
extern int FlashProtectionExample(void);

extern int hello_sd (void);							// SD card
extern int main_sysmonIntr(void);					// XADC access
extern int hello_button();							// PUSH BUTTON access


#endif /* BOARD_TEST_APP_H_ */
