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
#define MAX_FILE_SIZE 10000000

/****************************************************************/
/***** Global variables                                         */
/****************************************************************/
extern int new_vsel, new_psel, new_ssel;

#ifdef SLIDESHOW
extern int filter_file_counter;
#endif

/****************************************************************/
/***** Application Program Interfaces (APIs) for peripherals    */
/****************************************************************/
extern char inbyte(void );									// Serial port/Console
//extern void xil_printf(const char *ctrl1, ...);				// Formatted Console
extern void main_thread_vdemo(void* arg);
extern void xilkernel_init(void);
extern void xilkernel_start (void);

extern int main_sysmonIntr2(char *adcData, int printable);	// XADC access
extern int get_perf_results(char* perf_results);
extern int hello_vdemo(int lsel, int vsel, int ssel, int psel);

extern int hdmi_init(void);
extern void set_hdmi_output_color(void);

extern void print_headers(void);
extern void launch_app_threads(void);
extern void init_http_mutexes(void);


#ifdef SLIDESHOW
extern int sd_file_read(char *filename, char *read_buffer);
#endif

#endif /* BOARD_TEST_APP_H_ */
