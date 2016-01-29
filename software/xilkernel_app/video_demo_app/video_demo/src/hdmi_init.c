/* $Id: xiic_low_level_dynamic_eeprom_example.c,v 1.1.2.1 2011/01/27 05:44:08 rvodapa Exp $ */
/******************************************************************************
*
* (c) Copyright 2006-2010 Xilinx, Inc. All rights reserved.
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
* @file xiic_low_level_dynamic_eeprom_example.c
*
* This file consists of a polled mode design example which uses the Xilinx
* IIC device in dynamic mode and low-level driver to exercise the EEPROM.
*
* The XIic_DynSend() API is used to transmit the data and XIic_DynRecv() API
* is used to receive the data.
*
* The example is tested on ML300/ML310/ML403/ML501/ML507/ML510 Xilinx boards.
*
* The ML310/ML410/ML510 boards have a on-board 64 Kb serial IIC EEPROM
* (Microchip 24LC64A). The WP pin of the IIC EEPROM is hardwired to ground on
* this board.
*
* The ML300 board has an on-board 32 Kb serial IIC EEPROM(Microchip 24LC32A).
* The WP pin of the IIC EEPROM has to be connected to ground for this example.
* The WP is connected to pin Y3 of the FPGA.
*
* The ML403 board has an on-board 4 Kb serial IIC EEPROM(Microchip 24LC04A).
* The WP pin of the IIC EEPROM is hardwired to ground on this board.
*
* The ML501/ML505/ML507/ML605/SP601/SP605 boards have an on-board 8 Kb serial
* IIC EEPROM(STM M24C08). The WP pin of the IIC EEPROM is hardwired to
* ground on these boards.
*
* The AddressType for ML300/ML310/ML410/ML510 boards should be u16 as the
* address pointer in the on board EEPROM is 2 bytes.
*
* The AddressType for ML403/ML501/ML505/ML507/ML605/SP601/SP605 boards should
* be u8 as the address pointer for the on board EEPROM is 1 byte.
*
* The 7 bit IIC Slave address of the IIC EEPROM on the ML300/ML310/ML410/ML403/
* ML501/ML505/ML507/ML510 boards is 0x50.
* The 7 bit IIC Slave address of the IIC EEPROM on the ML605/SP601/SP605 boards
* is 0x54.
* Refer to the User Guide's of the respective boards for further information
* about the IIC slave address of IIC EEPROM's.
*
* The define EEPROM_ADDRESS in this file needs to be changed depending on
* the board on which this example is to be run.
*
* This code assumes that no Operating System is being used.
*
* @note
*
* None.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ---------------------------------------------------------
* 1.00a mta  03/20/06 Created.
* 2.00a sdm  09/22/09 Converted all register accesses to 32 bit access.
* 2.01a ktn  03/17/10 Updated the information about the EEPROM's used on
*		      ML605/SP601/SP605 boards. Updated the example so that it
*		      can be used to access the entire IIC EEPROM for devices
*		      like M24C04/M24C08 that use LSB bits of the IIC device
*		      select code (IIC slave address) to specify the higher
*		      address bits of the EEPROM internal address.
*
* modified on 2,Feb,2012 srinivasa attili, removed warnings
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xiic.h"
#include "xil_io.h"
#include "xbasic_types.h"
#include "board_test_app.h"

/************************** Constant Definitions *****************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define IIC_BASE_ADDRESS	XPAR_IIC_0_BASEADDR

/*
 * The Starting address in the IIC EEPROM on which this test is performed.
 */
#define EEPROM_TEST_START_ADDRESS	0x80

/*
 * The following constant defines the address of the IIC Slave device on the
 * IIC bus. Note that since the address is only 7 bits, this constant is the
 * address divided by 2.
 * The 7 bit IIC Slave address of the IIC EEPROM on the ML300/ML310/ML403/ML410/
 * ML501/ML505/ML507/ML510 boards is 0x50. The 7 bit IIC Slave address of the IIC
 * EEPROM on the ML605/SP601/SP605 boards is 0x54.
 * Please refer the User Guide's of the respective boards for further
 * information about the IIC slave address of IIC EEPROM's.
 */
 #define EEPROM_ADDRESS		0x50

/*
 * The page size determines how much data should be written at a time.
 * The ML310/ML300 board supports a page size of 32 and 16.
 * The write function should be called with this as a maximum byte count.
 */
#define PAGE_SIZE	16

#define IIC_SLAVE_ADDRESS	1

// PCA9548 8-port IIC Switch
#define IIC_SWITCH_ADDRESS 0x74
// Connected to IIC Buses
// Bus 0
#define IIC_SI570_ADDRESS  0x5D
// Bus 1
#define IIC_FMC_HPC_ADDRESS 0x70
// Bus 2
#define IIC_FMC_LPC_ADDRESS 0x70
// Bus 3
#define IIC_EEPROM_ADDRESS 0x54
// Bus 4
#define IIC_SFP_ADDRESS 0x50
// Bus 5
#define IIC_ADV7511_ADDRESS 0x39
// Bus 6
#define IIC_DDR3_SPD_ADDRESS 0x50
#define IIC_DDR3_TEMP_ADDRESS 0x18
// Bus 7
#define IIC_SI5326_ADDRESS 0x50

#define IIC_BUS_0 0x01
#define IIC_BUS_1 0x02
#define IIC_BUS_2 0x04
#define IIC_BUS_3 0x08
#define IIC_BUS_4 0x10
#define IIC_BUS_5 0x20
#define IIC_BUS_6 0x40
#define IIC_BUS_7 0x80

/**************************** Type Definitions *******************************/

/*
 * The AddressType for ML300/ML310/ML410/ML510 boards should be u16 as the address
 * pointer in the on board EEPROM is 2 bytes.
 * The AddressType for ML403/ML501/ML505/ML507/ML605/SP601/SP605 boards should
 * be u8 as the address pointer in the on board EEPROM is 1 bytes.
 */
typedef u8 AddressType;

typedef struct {
	u8 addr;
	u8 data;
	u8 init;
} HDMI_REG;

// Updated for YCrCb output
/*
#define NUMBER_OF_HDMI_REGS  31
HDMI_REG hdmi_iic[NUMBER_OF_HDMI_REGS] = {
	{0x01, 0x00, 0x00},
	{0x02, 0x00, 0x18},
	{0x03, 0x00, 0x00},
	{0x15, 0x00, 0x01},
	{0x16, 0x00, 0xB4},
	{0x18, 0x00, 0xC6},
	{0x40, 0x00, 0x80},
	{0x41, 0x00, 0x10},
	{0x48, 0x00, 0x08},
	{0x49, 0x00, 0xA8},
	{0x4C, 0x00, 0x00},
	{0x55, 0x00, 0x00},
	{0x56, 0x00, 0x08},
	{0x96, 0x00, 0x20},
	{0x98, 0x00, 0x03},
	{0x99, 0x00, 0x02},
	{0x9A, 0x00, 0xE0},
	{0x9C, 0x00, 0x30},
	{0x9D, 0x00, 0x61},
	{0xA2, 0x00, 0xA4},
	{0xA3, 0x00, 0xA4},
	{0xA5, 0x00, 0x44},
	{0xAB, 0x00, 0x40},
	{0xAF, 0x00, 0x06},
	{0xBA, 0x00, 0xA0},
	{0xD0, 0x00, 0x3C},
	{0xD1, 0x00, 0xFF},
	{0xDE, 0x00, 0x9C},
	{0xE0, 0x00, 0xD0},
	{0xE4, 0x00, 0x60},
	{0xF9, 0x00, 0x00}
};
*/
#if 0
#define NUMBER_OF_HDMI_REGS  54
HDMI_REG hdmi_iic[NUMBER_OF_HDMI_REGS] = {
	{0x01, 0x00, 0x00},
	{0x02, 0x00, 0x18},
	{0x03, 0x00, 0x00},
	{0x15, 0x00, 0x01},
// 	{0x16, 0x00, 0xB9},
	{0x16, 0x00, 0x39},
	{0x18, 0x00, 0xC7},
	{0x19, 0x00, 0x34},
	{0x1A, 0x00, 0x04},
	{0x1B, 0x00, 0xAD},
	{0x1C, 0x00, 0x00},
	{0x1D, 0x00, 0x00},
	{0x1E, 0x00, 0x1C},
	{0x1F, 0x00, 0x1B},
	{0x20, 0x00, 0x1D},
	{0x21, 0x00, 0xDC},
	{0x22, 0x00, 0x04},
	{0x23, 0x00, 0xAD},
	{0x24, 0x00, 0x1F},
	{0x25, 0x00, 0x24},
	{0x26, 0x00, 0x01},
	{0x27, 0x00, 0x35},
	{0x28, 0x00, 0x00},
	{0x29, 0x00, 0x00},
	{0x2A, 0x00, 0x04},
	{0x2B, 0x00, 0xAD},
	{0x2C, 0x00, 0x08},
	{0x2D, 0x00, 0x7C},
	{0x2E, 0x00, 0x1B},
	{0x2F, 0x00, 0x77},
	{0x40, 0x00, 0x80},
	{0x41, 0x00, 0x10},
	{0x48, 0x00, 0x08},
	{0x49, 0x00, 0xA8},
	{0x4C, 0x00, 0x04},
	{0x55, 0x00, 0x00},
	{0x56, 0x00, 0x08},
	{0x96, 0x00, 0x20},
	{0x98, 0x00, 0x03},
	{0x99, 0x00, 0x02},
	{0x9A, 0x00, 0xE0},
	{0x9C, 0x00, 0x30},
	{0x9D, 0x00, 0x61},
	{0xA2, 0x00, 0xA4},
	{0xA3, 0x00, 0xA4},
	{0xA5, 0x00, 0x44},
	{0xAB, 0x00, 0x40},
	{0xAF, 0x00, 0x04},
	{0xBA, 0x00, 0x10},
	{0xD0, 0x00, 0x3C},
	{0xD1, 0x00, 0xFF},
	{0xDE, 0x00, 0x9C},
	{0xE0, 0x00, 0xD0},
	{0xE4, 0x00, 0x60},
	{0xF9, 0x00, 0x00}
};
#else
#define NUMBER_OF_HDMI_REGS  40
HDMI_REG hdmi_iic[NUMBER_OF_HDMI_REGS] = {
	    {0x41, 0x00, 0x10}, // Power Down Control
	                        //    R0x41[  6] = PowerDown = 0 (power-up)
	    {0xD6, 0x00, 0xC0}, // HPD Control
	                        //    R0xD6[7:6] = HPD Control = 11 (always high)
	    {0x15, 0x00, 0x01}, // Input YCbCr 4:2:2 with separate syncs
	    {0x16, 0x00, 0x3C}, // Output format 444, Input Color Depth = 8
	                        //    R0x16[  7] = Output Video Format = 0 (444)
	                        //    R0x16[5:4] = Input Video Color Depth = 11 (8 bits/color)
	                        //    R0x16[3:2] = Input Video Style = 11 (style 3)
	                        //    R0x16[  1] = DDR Input Edge = 0 (falling edge)
	                        //    R0x16[  0] = Output Color Space = 0 (RGB)
	    {0x18, 0x00, 0xAB}, // Color Space Conversion
	                        //    R0x18[  7] = CSC enable = 1 (CSC enabled)
	                        //    R0x18[6:5] = CSC Scaling Factor = 01 (+/- 2.0, -8192 - 8190)
	                        //    R0x18[4:0] = CSC coefficient A1[12:8]
	    {0x19, 0x00, 0x37}, //    R0x19[7:0] = CSC coefficient A1[ 7:0]
	    {0x1A, 0x00, 0x08}, //    R0x1A[  5] = CSC coefficient update
	                        //    R0x1A[4:0] = CSC coefficient A2[12:8]
	    {0x1B, 0x00, 0x00}, //    R0x1B[7:0] = CSC coefficient A2[ 7:0]
	    {0x1C, 0x00, 0x00}, //    R0x1C[4:0] = CSC coefficient A3[12:8]
	    {0x1D, 0x00, 0x00}, //    R0x1D[7:0] = CSC coefficient A3[ 7:0]
	    {0x1E, 0x00, 0x1A}, //    R0x1E[4:0] = CSC coefficient A4[12:8]
	    {0x1F, 0x00, 0x86}, //    R0x1F[7:0] = CSC coefficient A4[ 7:0]
	    {0x20, 0x00, 0x1A}, //    R0x20[4:0] = CSC coefficient B1[12:8]
	    {0x21, 0x00, 0x49}, //    R0x21[7:0] = CSC coefficient B1[ 7:0]
	    {0x22, 0x00, 0x08}, //    R0x22[4:0] = CSC coefficient B2[12:8]
	    {0x23, 0x00, 0x00}, //    R0x23[7:0] = CSC coefficient B2[ 7:0]
	    {0x24, 0x00, 0x1D}, //    R0x24[4:0] = CSC coefficient B3[12:8]
	    {0x25, 0x00, 0x3F}, //    R0x25[7:0] = CSC coefficient B3[ 7:0]
	    {0x26, 0x00, 0x04}, //    R0x26[4:0] = CSC coefficient B4[12:8]
	    {0x27, 0x00, 0x22}, //    R0x27[7:0] = CSC coefficient B4[ 7:0]
	    {0x28, 0x00, 0x00}, //    R0x28[4:0] = CSC coefficient C1[12:8]
	    {0x29, 0x00, 0x00}, //    R0x29[7:0] = CSC coefficient C1[ 7:0]
	    {0x2A, 0x00, 0x08}, //    R0x2A[4:0] = CSC coefficient C2[12:8]
	    {0x2B, 0x00, 0x00}, //    R0x2B[7:0] = CSC coefficient C2[ 7:0]
	    {0x2C, 0x00, 0x0E}, //    R0x2C[4:0] = CSC coefficient C3[12:8]
	    {0x2D, 0x00, 0x2D}, //    R0x2D[7:0] = CSC coefficient C3[ 7:0]
	    {0x2E, 0x00, 0x19}, //    R0x2E[4:0] = CSC coefficient C4[12:8]
	    {0x2F, 0x00, 0x14}, //    R0x2F[7:0] = CSC coefficient C4[ 7:0]
	    {0x48, 0x00, 0x08}, // Video Input Justification
	                        //    R0x48[4:3] = Video Input Justification = 01 (right justified)
	    {0x55, 0x00, 0x00}, // Set RGB in AVinfo Frame
	                        //    R0x55[6:5] = Output Format = 00 (RGB)
	    {0x56, 0x00, 0x28}, // Aspect Ratio
	                        //    R0x56[5:4] = Picture Aspect Ratio = 10 (16:9)
	                        //    R0x56[3:0] = Active Format Aspect Ratio = 1000 (Same as Aspect Ratio)
	    {0x98, 0x00, 0x03}, // ADI Recommended Write
	    {0x9A, 0x00, 0xE0}, // ADI Recommended Write
	    {0x9C, 0x00, 0x30}, // PLL Filter R1 Value
	    {0x9D, 0x00, 0x61}, // Set clock divide
	    {0xA2, 0x00, 0xA4}, // ADI Recommended Write
	    {0xA3, 0x00, 0xA4}, // ADI Recommended Write
	    {0xAF, 0x00, 0x04}, // HDMI/DVI Modes
	                        //    R0xAF[  7] = HDCP Enable = 0 (HDCP disabled)
	                        //    R0xAF[  4] = Frame Encryption = 0 (Current frame NOT HDCP encrypted)
	                        //    R0xAF[3:2] = 01 (fixed)
	                        //    R0xAF[  1] = HDMI/DVI Mode Select = 0 (DVI Mode)
	    {0xE0, 0x00, 0xD0}, // Must be set to 0xD0 for proper operation
	    {0xF9, 0x00, 0x00}  // Fixed I2C Address (This should be set to a non-conflicting I2C address)
};
#endif
u8 EepromIicAddr;		/* Variable for storing Eeprom IIC address */

/************************** Function Prototypes ******************************/

int IicLowLevelDynEeprom();

u8 EepromReadByte(AddressType Address, u8 *BufferPtr, u8 ByteCount);
u8 EepromWriteByte(AddressType Address, u8 *BufferPtr, u8 ByteCount);

extern void xil_printf( const char *ctrl1, ...);

/************************** Variable Definitions *****************************/

u8 WriteBuffer[PAGE_SIZE];	/* Write buffer for writing a page. */

u8 ReadBuffer[PAGE_SIZE];	/* Read buffer for reading a page.  */

u8 DataBuf[PAGE_SIZE];
/************************** Function Definitions *****************************/

/*****************************************************************************/
/**
* Main function to call the low level Dynamic EEPROM example.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int hdmi_init(void)
{
	int Status;

	/*
	 * Perform the Read and Write operation.
	 */
	Status = IicLowLevelDynEeprom();
	if (Status != XST_SUCCESS) {
		xil_printf("IIC HDMI device is not ready !! \r\n");
		return XST_FAILURE;
	}

	//xil_printf("IIC HDMI device is ready\r\n");
	return XST_SUCCESS;
}

/******************************************************************************
*
* The function uses the low level driver of IIC to read from the IIC EEPROM on
* the ML300/ML310 board. The addresses tested starts from 128.
*
* @param	None.
*
* @return	XST_SUCCESS if successful else XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int IicLowLevelDynEeprom()
{
	u8 BytesRead;
	u32 StatusReg;
	u8 Index;
	int Status;
	EepromIicAddr = IIC_SWITCH_ADDRESS;
	//int c;
	int i;
	//u8 ByteAddr;


	/*
	 * Initialize the IIC Core.
	 */
	Status = XIic_DynInit(IIC_BASE_ADDRESS);
	if (Status != XST_SUCCESS) {
		xil_printf("IIC driver initialization failed\r\n");
		return XST_FAILURE;
	}
	//xil_printf("IIC driver initialized \r\n");

	/*
	 * Make sure all the Fifo's are cleared and Bus is Not busy.
	 */
	while (((StatusReg = XIic_ReadReg(IIC_BASE_ADDRESS,
				XIIC_SR_REG_OFFSET)) &
				(XIIC_SR_RX_FIFO_EMPTY_MASK |
				XIIC_SR_TX_FIFO_EMPTY_MASK |
		 		XIIC_SR_BUS_BUSY_MASK)) !=
		 		(XIIC_SR_RX_FIFO_EMPTY_MASK |
				XIIC_SR_TX_FIFO_EMPTY_MASK)) {

	}

   // FOR KINTEX-7 (KC705, RevC) only: generate GPIO Reset sequence
	Xil_Out32(IIC_BASE_ADDRESS+0x124, 0xffffffff);	// bit0 = 1
	for(i=0;i<1000;i++);	// delay
	Xil_Out32(IIC_BASE_ADDRESS+0x124, 0xfffffffe);	// bit0 = 0
	for(i=0;i<1000;i++);	// delay
	Xil_Out32(IIC_BASE_ADDRESS+0x124, 0xffffffff);	// bit0 = 1
	for(i=0;i<1000;i++);	// delay
	
	/*
	 * Initialize the data to written and the read buffer.
	 */
	for (Index = 0; Index < PAGE_SIZE; Index++) {
		WriteBuffer[Index] = Index;
		ReadBuffer[Index] = 0;
		DataBuf[Index] = 0;
	}
	/*
	 * Write to the IIC SWITCH.
	 */
	EepromIicAddr = IIC_SWITCH_ADDRESS; // Alternate use of Write routine
	WriteBuffer[0] = 0x20;
	EepromWriteByte(0x20, DataBuf, 0);
	//xil_printf("IIC MUX initialized \r\n");

	/*
	 * Write Initialization Sequence to ADV7511.
	 */
	EepromIicAddr = IIC_ADV7511_ADDRESS;
	for ( Index = 0; Index < NUMBER_OF_HDMI_REGS; Index++)
	{
	  EepromWriteByte(hdmi_iic[Index].addr, &hdmi_iic[Index].init, 1);
	  //xil_printf("\r\nBytesWritten = %d\r\n", BytesWritten);
	}
	//xil_printf("IIC HDMI device initialized \r\n");

	//	xil_printf("Press Any Key to CONTINUE>\r\n");
	//	c = getchar();

	/*
	 * Read from the EEPROM.
	 */
	for ( Index = 0; Index < NUMBER_OF_HDMI_REGS; Index++)
	{
		BytesRead = EepromReadByte(hdmi_iic[Index].addr, &hdmi_iic[Index].data, 1);
		//xil_printf("\r\nBytesRead = %d\r\n", BytesRead);
		if (BytesRead != 1) {
			xil_printf("IIC HDMI device initialization failed\r\n");
			return XST_FAILURE;
		}
		// xil_printf("ReadBuffer[%d] = %02X\r\n", Index, ReadBuffer[Index]);
	}

    /*
     * Display read buffer.
     */
	#if 0
	for (Index = 0; Index < NUMBER_OF_HDMI_REGS; Index++) {
         xil_printf(" %02d) HDMI REG[0x%02X] = 0x%02X\r\n", Index,
	              hdmi_iic[Index].addr, hdmi_iic[Index].data);
	}
	#endif

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* This function writes a buffer of bytes to the IIC serial EEPROM.
*
* @param	BufferPtr contains the address of the data to write.
* @param	ByteCount contains the number of bytes in the buffer to be
*		written. Note that this should not exceed the page size of the
*		EEPROM as noted by the constant PAGE_SIZE.
*
* @return	The number of bytes written, a value less than that which was
*		specified as an input indicates an error.
*
* @note		one.
*
******************************************************************************/
Xuint8 EepromWriteByte(AddressType Address, Xuint8 *BufferPtr,
					   Xuint8 ByteCount)
{
	u8 SentByteCount;
	u8 WriteBuffer[sizeof(Address) + PAGE_SIZE];
	u8 Index;

	/*
	 * A temporary write buffer must be used which contains both the address
	 * and the data to be written, put the address in first based upon the
	 * size of the address for the EEPROM
	 */
	if (sizeof(AddressType) == 2) {
		WriteBuffer[0] = (u8) (Address >> 8);
		WriteBuffer[1] = (u8) (Address);
	} else if (sizeof(AddressType) == 1) {
		WriteBuffer[0] = (u8) (Address);
		EepromIicAddr |= (EEPROM_TEST_START_ADDRESS >> 8) & 0x7;
	}

	/*
	 * Put the data in the write buffer following the address.
	 */
	for (Index = 0; Index < ByteCount; Index++) {
		WriteBuffer[sizeof(Address) + Index] = BufferPtr[Index];
	}

	/*
	 * Write a page of data at the specified address to the EEPROM.
	 */
	SentByteCount = XIic_DynSend(IIC_BASE_ADDRESS, EepromIicAddr,
				WriteBuffer, sizeof(Address) + ByteCount,
				XIIC_STOP);

	/*
	 * Return the number of bytes written to the EEPROM.
	 */
	return SentByteCount - sizeof(Address);
}

/******************************************************************************
*
* This function reads a number of bytes from the IIC serial EEPROM into a
* specified buffer.
*
* @param	BufferPtr contains the address of the data buffer to be filled.
* @param	ByteCount contains the number of bytes in the buffer to be read.
*		This value is constrained by the page size of the device such
*		that up to 64K may be read in one call.
*
* @return	The number of bytes read. A value less than the specified input
*		value indicates an error.
*
* @note		None.
*
******************************************************************************/
Xuint8 EepromReadByte(AddressType Address, Xuint8 *BufferPtr,
 					  Xuint8 ByteCount)
{
	u8 ReceivedByteCount;
	u8 SentByteCount = 0;
	u16 StatusReg;

	/*
	 * Position the Read pointer to specific location in the EEPROM.
	 */
	do {
		StatusReg = XIic_ReadReg(IIC_BASE_ADDRESS, XIIC_SR_REG_OFFSET);
		if (!(StatusReg & XIIC_SR_BUS_BUSY_MASK)) {
			SentByteCount = XIic_DynSend(IIC_BASE_ADDRESS,
							EepromIicAddr,
							(u8 *) &Address,
							sizeof(Address),
    								 XIIC_REPEATED_START);
		}

	} while (SentByteCount != sizeof(Address));
		/*
		 * Receive the data.
		 */
		ReceivedByteCount = XIic_DynRecv(IIC_BASE_ADDRESS,
						 EepromIicAddr, BufferPtr,
						 ByteCount);

	/*
	 * Return the number of bytes received from the EEPROM.
	 */
	return ReceivedByteCount;
}
