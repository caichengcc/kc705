*******************************************************************************
** © Copyright 2011 Xilinx, Inc. All rights reserved.
** This file contains confidential and proprietary information of Xilinx, Inc. 
** and is protected under U.S. and international copyright and other 
** intellectual property laws.
*******************************************************************************
**   ____  ____ 
**  /   /\/   / 
** /___/  \  /   Vendor: Xilinx 
** \   \   \/    
**  \   \        readme.txt Version: 7.0  
**  /   /        Date Last Modified: 08/23/13 
** /___/   /\    Date Created: 05/21/12
** \   \  /  \   Associated Filename: K7_embdded-trd-rdf0283.zip
**  \___\/\___\ 
** 
**  Device: Kintex-7 (xc7k325tffg900-2) FPGA
**  Purpose: KC705 Embedded Kit Reference Design and Demonstration 
**           Files
**
*********************************************************************************************************
*********************************************************************************************************
**  Important Note: 											*
**    ==> Xilkernel is used as part of this TRD for consistency with previous versions.			*
**    ==> Xilkernel is in the process of being deprecated and should not be used for new designs.	*
**    ==> Package is released with evaluation version of third party (XYLON) IP cores	*
*********************************************************************************************************
*********************************************************************************************************     
**
**  Reference: 
**    ==> http://www.xilinx.com/products/boards-and-kits/DK-K7-EMBD-G.htm
**    ==> http://www.wiki.xilinx.com/K7+Embedded+TRD+2013.2
**  Revision History: v7.0
**  Change log :
**    ==> Changes in v7.0 
**	       - Hardware design migrated to VIVADO IPI flow
**	       - All the hardware IPs and software drivers are upgraded and tested with 2013.2 tool chain
**
**    ==> Changes in v6.0 
**	       - All the hardware IPs and software drivers are upgraded and tested with 14.5 tool chain
**	       - Local pcores perf_monitor and dvi2axi are replaced with EDK pcores 
**		     axi_perf_mon and v_vid_in_axi4s, respectively.
**
**    ==> Changes in v5.0 
**             - All the hardware IPs and software drivers are upgraded and tested with 14.4 tool chain
**             - K7 Embedded kit weblink is added in the readme.txt file in the reference list
**    ==> Changes in v4.0 
**             - All the hardware IPs and software drivers are upgraded and tested with 14.3 tool chain
**             - VIVADO version of both BIST and VIDEO DEMO TRDs are added to the package with readme.txt file
**    ==> Changes in v3.0 
**             - All the hardware IPs and software drivers are upgraded and tested with 14.2 tool chain
**             - 24bit RGB to 16bit YCbCr converter IP is removed from the design as,
**                   the latest logiCVC support this functionality
**             - Coregen based axi_vtc pcore is replaced with v_tc EDK pcore
**             - Webserver look and feel is changed
**             - Bist console menu is revamped
**    ==> Changes in v2.0 
**             - All the hardware IPs and software drivers are upgraded and tested with 14.1 tool chain
**    ==> Changes in v1.0 
**             - First release of the TRD with 13.4 tool chain
**  Known Issues :
**    ==>  Refer to Master Answer Record, http://www.xilinx.com/support/answers/56174.htm
**
*******************************************************************************
**
**  Disclaimer: 
**
**		This disclaimer is not a license and does not grant any rights
**              to the materials distributed herewith. Except as otherwise 
**              provided in a valid license issued to you by Xilinx, and to the
**              maximum extent permitted by applicable law: 
**              (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL  
**              FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND 
**              CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING BUT NOT
**              LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
**              FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not 
**              be liable (whether in contract or tort, including negligence,
**              or under any other theory of liability) for any loss or damage 
**              of any kind or nature related to, arising under or in connection
**              with these materials, including for any direct, or any indirect,
**              special, incidental, or consequential loss or damage (including 
**              loss of data, profits, goodwill, or any type of loss or damage 
**              suffered as a result of any action brought by a third party) 
**              even if such damage or loss was reasonably foreseeable or 
**              Xilinx had been advised of the possibility of the same.


**  Critical Applications:
**
**		Xilinx products are not designed or intended to be fail-safe, 
**              or for use in any application requiring fail-safe performance, 
**              such as life-support or safety devices or systems, 
**		Class III medical devices, nuclear facilities, applications 
**              related to the deployment of airbags, or any other applications
**              that could lead to death, personal injury, or severe property 
**              or environmental damage (individually and collectively, 
**              "Critical Applications"). Customer assumes the sole risk and 
**              liability of any use of Xilinx products in Critical 
**              Applications, subject only to applicable laws and regulations
**              governing limitations on product liability.

**  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS 
**  FILE AT ALL TIMES.

*******************************************************************************

This readme describes the files that come with the package

*******************************************************************************

** IMPORTANT NOTES **

This ZIP file contains the reference design and demonstration files for the 
KC705 Embedded Kit. The hardware platform is an AXI interface based design.

The file structure is as follows:

 -- k7-embdded-trd-rdf0283
    |-- ready_to_test
    |-- hardware
    |   |--  sources
    |   |    |-- ip_package
    |   |    |-- hdl
    |   |    |-- constraints
    |   |--  vivado
    |        |-- scripts
    |        |-- runs 
    |-- software
    |   |--  standalone_app
    |   |    |-- board_test_app_console
    |   |--  xilkernel_app
    |        |-- board_test_app_webserver
    |        |-- video_demo_app
    |-- doc
        |--  readme.txt (this file)
        |--  k7-embedded-doc-advisory.pdf
 