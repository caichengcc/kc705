/**
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2010 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
******************************************************************************/
/**
*
* @file sd_funcs.c
*
* Defines SD Card access functions.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a JI   13/11/09 First release \n
* </pre>
*
* modified on 2,Feb,2012 srinivasa attili; modified for Kintex7 Demo
*
*******************************************************************************/
#if 1

#include "xparameters.h"    // Located in: microblaze_0/include/xparameters.h
#include "stdio.h"
#include "xutil.h"
#include "xio.h"
#include "sd_test.h"

#include "lwip/sys.h"
#include <sys/stats.h>
#include <sys/process.h>
#include "board_test_app.h"
#include <pthread.h>
#include "config_apps.h"

#define XPAR_LOGISDHC_0_MEM_BASEADDR 0x10000000  // necessary for compiling when no dma is used; remove this else
#define	SD_RETRY	5000		// Number of trials to get response from SD card
					// After these trials if response is not found, it is decided that SD card is not inserted

volatile unsigned int present_state, int_stat;
unsigned short err_int_stat_en;
unsigned int sd_status, sd_response[4], sd_card_ccs, sd_card_addr, block_size, sdma_addr, block_cnt, byte_cnt;
unsigned int sd_card_csd[4], rd_buffer[128*15],mult_rd_buffer[128*15], wr_buffer[128*15], dma_buffer[128*15];
int i=0, j=0;

short Init_SD_Card();
void send_csd(int address);
void select_sd_card(int address);
void send_status(int address);
void set_sd_bus_width(int address, char width);
void send_scr();
void high_speed_mode_en();
unsigned char check_errors();
void write_single(int address, int block_size);
void read_single(int address, int block_size);
void write_read_single(int address, int block_size);
void read_multiple_block(int sd_mem_address, int block_count, int block_size);
void write_multiple_block(int sd_mem_address, int block_count, int block_size);
void write_read_multiple_block(int sd_mem_address, int block_count, int block_size);
void dma_read_single_block(int sd_mem_address, int host_mem_address, int block_size);
void dma_read_multiple_block(int sd_mem_address, int host_mem_address, int block_count, int block_size);
void dma_write_single_block(int sd_mem_address, int host_mem_address, int block_size);
void dma_write_multiple_block(int sd_mem_address, int host_mem_address, int block_count, int block_size);
void dma_write_read_multiple_block(int sd_mem_address, int host_mem_address, int source_data_address, int block_count, int block_size);
void xylon_dma_read_multiple_block_dummy(int sd_mem_address, int host_mem_address, short pix_start_byte, short hres, short vres);
unsigned char write_io_byte(unsigned char func, unsigned int reg_addr, unsigned char data, unsigned char raw_flag);
unsigned char read_io_byte(unsigned char func, unsigned int reg_addr);
void Erase_SD_Blocks(int start_addr, int end_addr);


/****************************************************************************/
/**
*
* Initialize SD Card and return SD Card address on the bus.
*
* @param[in]    None
*
* @param[out]   None
*
* @return       SD Card address
*
* @note         None
*
*
*****************************************************************************/
short Init_SD_Card()
{
	#if 1
	unsigned int timer;
   PRINT("Starting SD Card Initialization...\n");

   for(timer=0; timer < SD_RETRY; timer++)
   {
	   present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1) before doing anything
	   if((present_state & 0x3) == 0)
		   break;
   }
   present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1) before doing anything
   if((present_state & 0x3) != 0)
	   return -1;

   //PRINT("present1 state = 0x%x\n", present_state);

   #else

      PRINT("\n\n\rStarting SD Card Initialization...");

      do
         present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1) before doing anything
      while((present_state & 0x3) != 0);  //Check Present state reg (bit 0 and 1) before doing anything
	#endif

   /********************************************************/
   // CMD0 GO_IDLE_STATE, No response
   /********************************************************/
   //PRINT("\n\n\rCMD0 GO_IDLE_STATE");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x00000000;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0000;  // Command reg; Command index, No data, no command index check, no CRC check, No Response

   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]); //should be 0x1AA

   /********************************************************/
   // CMD8 SEND_IF_COND, response R7
   /********************************************************/
   //PRINT("\n\n\rCMD8 SEND_IF_COND");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x000001AA;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x081A;  // Command reg; Command index, No data, command index check, CRC check, Response R7
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]); //should be 0x1AA
   // If we get any response to CMD8 it means that the SD card is version 2.0 or later.
   // If the response is valid the SD card has a compatible voltage range
   /********************************************************/

   /********************************************************/
   // ACMD41 SD_SEND_OP_COND, response R3 (to send any ACMD command, host first has to send APP_CMD)
   /********************************************************/
   //PRINT("\n\n\rACMD41 SD_SEND_OP_COND. First CMD55 APP_CMD is sent");
   timer=0;
   do {
      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x00000000;  // command argument
      IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
      IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x371A;  // Command reg; Command index, No data, command index check, CRC check, Response R1
      do
         present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1)
      while((present_state & 0x3) != 0);
      //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
      //PRINT("\n\rSD response = 0x%x", sd_response[0]);

      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x40FF0000;  // command argument
      IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
      IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x2902;  // Command reg; Command index, No data, no command index check, no CRC check, Response R3
      do
         present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1)
      while((present_state & 0x3) != 0);
      sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
      //PRINT("\n\rSD response = 0x%x", sd_response[0]);

      if(timer==SD_RETRY)
    	  break;
      timer++;
   } while((sd_response[0] & 0x80000000) != 0x80000000); // Do this until SD Card sends ready (bit 31 is high)

   // error, response timeout
   if( (sd_response[0] & 0x80000000) != 0x80000000)
   {
	   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x00000000;  // command argument
	   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
	   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0000;  // Command reg; Command index, No data, no command index check, no CRC check, No Response
	   PRINT("No response from SD card.\n");
	   do
	      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
	   while((present_state & 0x3) != 0);
	   return -1;
   }

   if ((sd_response[0] & 0x40000000) == 0x40000000) sd_card_ccs = 1;
   // If, after the card responces ready, bit 30 (CCS) is high it means that the SD card is High Capacity Card (2GB < X < 32GB)
   // otherwise it is Standard capacity (<=2GB)
   /********************************************************/

   /********************************************************/
   // CMD2 ALL_SEND_CID, LONG response R2
   /********************************************************/
   //PRINT("\n\n\rCMD2 ALL_SEND_CID");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x00000000;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0209;  // Command reg; Command index, No data, no command index check, CRC check, Response R2
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //sd_response[1] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x14); // read response argument
   //sd_response[2] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x18); // read response argument
   //sd_response[3] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x1C); // read response argument
   //PRINT("\n\rSD Card CID = 0x%06x, 0x%08x, 0x%08x, 0x%08x", sd_response[3], sd_response[2], sd_response[1], sd_response[0]);

   /********************************************************/
   // CMD3 SEND_RELATIVE_ADDR, response R6
   /********************************************************/
   //PRINT("\n\n\rCMD3 SEND_RELATIVE_ADDR");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x00000000;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x031A;  // Command reg; Command index, No data, command index check, CRC check, Response R6
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);
   sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   // After the above command (CMD3) SD card identification is finnished and SD Card is in Stand-By mode.

   check_errors();
   PRINT("\n\rSD Card Initialization finished!\n\r");

   return (short) (sd_response[0]>>16);
}

/****************************************************************************/
/**
*
* Sends a command to SD Card to reply with Card CSD register.
*
* @param[in]    address is the address of the card.
*
* @param[out]   None
*
* @return       None
*
* @note         SD Card has to be in Stand-By state to accept this command
*
*
*****************************************************************************/
void send_csd(int address) {

   PRINT("\n\n\rSend CSD function START!");

   /********************************************************/
   // CMD9 SEND_CSD, LONG response R2
   /********************************************************/
   //PRINT("\n\n\rCMD9 SEND_CSD");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = address<<16;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0909;  // Command reg; Command index, No data, no command index check, CRC check, Response R2
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //sd_response[1] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x14); // read response argument
   //sd_response[2] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x18); // read response argument
   //sd_response[3] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x1C); // read response argument
   //PRINT("\n\rSD response = 0x%06x, 0x%08x, 0x%08x, 0x%08x", sd_response[3], sd_response[2], sd_response[1], sd_response[0]);
   sd_card_csd[3] = (sd_response[3]<<8) | (sd_response[2]>>24);
   sd_card_csd[2] = (sd_response[2]<<8) | (sd_response[1]>>24);
   sd_card_csd[1] = (sd_response[1]<<8) | (sd_response[0]>>24);
   sd_card_csd[0] = (sd_response[0]<<8);
   PRINT("\n\rSD Card CSD = 0x%x, 0x%x, 0x%x, 0x%x", sd_card_csd[3], sd_card_csd[2], sd_card_csd[1], sd_card_csd[0]);

   check_errors();
   PRINT("\n\rSend CSD function END!\n\r");
}

/****************************************************************************/
/**
*
* Select addressed SD Card.
*
* @param[in]    address is the address of the card that wants to be selected.
*
* @param[out]   None
*
* @return       None
*
* @note         SD Card has to be in Stand-By state to accept this command
*
*
*****************************************************************************/
void select_sd_card(int address) {

   PRINT("\n\n\rSelect SD card function START!");

   /********************************************************/
   // CMD7 SELECT/DESELECT_CARD, response R1b
   /********************************************************/
   //PRINT("\n\n\rCMD7 SELECT/DESELECT_CARD");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_card_addr<<16;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x071B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);

   check_errors();
   PRINT("\n\rSelect SD card function END!\n\r");
}

/****************************************************************************/
/**
*
* Sends a command to SD Card to reply with its status.
*
* @param[in]    address is the address of the card.
*
* @param[out]   None
*
* @return       None
*
* @note         SD Card has to be in Data transfer mode to accept this command.
*
*
*****************************************************************************/
void send_status(int address) {

   PRINT("\n\n\rSD Card send status function START!");

   /********************************************************/
   // CMD13 SEND_STATUS, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD13 SEND_STATUS");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) =  address<<16;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0D1A;  // Command reg; Command index, No data, command index check, CRC check, Response R1
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);
   sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   PRINT("\n\rSD response = 0x%x", sd_response[0]);

   check_errors();
   PRINT("\n\rSD Card send status function END!\n\r");
}

/****************************************************************************/
/**
*
* Set SD Bus data width for addressed SD card.
*
* @param[in]    address defines the card address of the card that data width
*                 wants to be changed.
*               width defines data width. Supported vaues are 1 and 4!!!
*
* @param[out]   None
*
* @return       None
*
* @note         SD Card has to be in transfer state to accept this command
*
*
*****************************************************************************/
void set_sd_bus_width(int address, char width) {

   //char tmp;

   PRINT("\n\n\rSet SD bus width function START!");
   PRINT("\n\rSetting SD Bus width to %d", width);

   /********************************************************/
   // ACMD6 SET_BUS_WIDTH, response R1 (to send any ACMD command, host first has to send APP_CMD)
   /********************************************************/
   //PRINT("\n\n\rACMD6 SET_BUS_WIDTH. First CMD55 APP_CMD is sent");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = address<<16;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x371A;  // Command reg; Command index, No data, command index check, CRC check, Response R1
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);

   if (width == 1) {
      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   }
   else if(width == 4) {
      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x2;  // command argument
   }
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x061A;  // Command reg; Command index, No data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1)
   } while((present_state & 0x3) != 0);
//   sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
//   PRINT("\n\rSD Host present state = 0x%x", present_state);
//   PRINT("\n\rSD response = 0x%x", sd_response[0]);

   if (width == 1) {
      IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x28) = IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x28) & 0xFD;  // Host ctrl reg; Data trans width = 1
   }
   else if (width == 4) {
      IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x28) = IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x28) | 0x02;  // Host ctrl reg; Data trans width = 4
   }

   PRINT("\n\rSD Bus width set to %d!", width);
   check_errors();
   PRINT("\n\rSet SD bus width function END!\n\r");
}

/****************************************************************************/
/**
*
* Sends a command to SD Card to reply with Card SCR register
*
* @param[in]    None
*
* @param[out]   None
*
* @return       None
*
* @note         SD Card has to be in transfer state to accept this command
*
*
*****************************************************************************/
void send_scr() {

   PRINT("\n\n\rSD Card send SCR function START!");

   /********************************************************/
   // ACMD51 SEND_SCR, response R1 (to send any ACMD command, host first has to send APP_CMD)
   /********************************************************/
   //PRINT("\n\n\rACMD51 SEND_SCR. First CMD55 APP_CMD is sent");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_card_addr<<16;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x371A;  // Command reg; Command index, No data, command index check, CRC check, Response R1
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);

   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x00000000;  // command argument
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = 0x00000008;  // Block size reg;  Transfer block size = 8 bytes
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0010;  // Transfer reg; Single transfer, Data direction = read
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x333A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1, 11)
   } while((present_state & 0x803) != 0x800);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   i=0;
   do {
      rd_buffer[i] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20); // read buffer data port register
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);
      //PRINT("\n\rRD buffer %d = 0x%x", i, rd_buffer[i]);
      i++;
   } while ((present_state & 0x800) != 0x000);  //Check Present state reg (bit 11) buffer read enable bit (there should be two reads)

   PRINT("\n\rSD Card SCR = 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x",
            (unsigned char) (rd_buffer[0]), (unsigned char) (rd_buffer[0] >> 8), (unsigned char) (rd_buffer[0] >> 16), (unsigned char) (rd_buffer[0] >> 24),
            (unsigned char) (rd_buffer[1]), (unsigned char) (rd_buffer[1] >> 8), (unsigned char) (rd_buffer[1] >> 16), (unsigned char) (rd_buffer[1] >> 24));

   check_errors();
   PRINT("\n\rSD Card send SCR function END!\n\r");

}

/****************************************************************************/
/**
*
* Sends Host and SD Card to High speed mode (50MHz)
*
* @param[in]    None
*
* @param[out]   None
*
* @return       None
*
* @note         SD Card has to be in transfer state to accept this command
*
*
*****************************************************************************/
void high_speed_mode_en() {

   PRINT("\n\n\rHigh Speed Mode Enable function START!");

   int tmp;
   //int tmp16;
   //int tmp32;

   /********************************************************/
   // CMD6 SWITCH_FUNC, response R1;  Check function
   /********************************************************/
   //PRINT("\n\n\rCMD6 SWITCH_FUNC");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x00FFFFFF;  // command argument; Check function
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = 0x00000040;  // Block size reg;  Transfer block size = 64 bytes
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0010;  // Transfer reg; Single transfer, Data direction = read
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x063A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1, 11)
   } while((present_state & 0x803) != 0x800);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   i=0;
   do {
      rd_buffer[i] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20); // read buffer data port register
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);
      //PRINT("\n\rRD buffer %d = 0x%x", i, rd_buffer[i]);
      i++;
   } while ((present_state & 0x800) != 0x000);  //Check Present state reg (bit 11) buffer read enable bit

   if ((rd_buffer[3] & 0x200) == 0x200) { // High speed function is supported
      PRINT("\n\rSD Card supports High speed mode!");
      /********************************************************/
      // CMD6 SWITCH_FUNC, response R1;  Change to High speed mode
      /********************************************************/
      //PRINT("\n\n\rCMD6 SWITCH_FUNC");
      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x80FFFFF1;  // command argument; Change to High speed mode
      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = 0x00000040;  // Block size reg;  Transfer block size = 64 bytes
      IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0010;  // Transfer reg; Single transfer, Data direction = read
      IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x063A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
      do {
         present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1, 11)
      } while((present_state & 0x803) != 0x800);
      //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
      //PRINT("\n\rSD Host present state = 0x%x", present_state);
      //PRINT("\n\rSD response = 0x%x", sd_response[0]);
      i=0;
      do {
         rd_buffer[i] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20); // read buffer data port register
         present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);
         //PRINT("\n\rRD buffer %d = 0x%x", i, rd_buffer[i]);
         i++;
      } while ((present_state & 0x800) != 0x000);  //Check Present state reg (bit 11) buffer read enable bit

      if ((rd_buffer[4] & 0x1) == 0x1) { // SD Card returned that High speed is turned on
         tmp = IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x28);
         IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x28) = tmp | 0x04;  // Host ctrl reg; High speed enabled
         IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2C) = 0x0005;  // Clock ctrl reg; SD clock frequency = 50MHz/1, SD clk en, Internal clk en
         PRINT("\n\rSD Card and SD Host switched to High speed mode and to max SD clk of 50MHz!");
      }
      else {
         PRINT("\n\rERROR: Switching of SD Card to High speed mode failed!");
      }
      /********************************************************/
   }
   else {
      PRINT("\n\rSD Card doesn't support High speed mode!");
   }
   check_errors();
   PRINT("\n\rHigh Speed Mode Enable function END!\n\r");
}

/****************************************************************************/
/**
*
* Checks if error bits in Host Status register are set
*
* @param[in]    None
*
* @param[out]   None
*
* @return       Error; 0 if no error, 1 if error
*
* @note
*
*
*****************************************************************************/
unsigned char check_errors() {



   err_int_stat_en = IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x36); //Error interrupt status enable reg
   if ((err_int_stat_en & 0x03FF) != 0x03FF) {  // Are status bits enabled?
      PRINT("\n\rWARNING not all error interrupt enable bits are set!");
   }
   int_stat = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30); //Interrupt status reg (error and normal reg)
   if ((int_stat & 0x00008000) == 0x00008000) {  // Check error interrupt bit (if set some bits in error interrupt register are set)

	   if ((int_stat & 0x10000000) != 0x10000000)		// this is not error, only warning
		   PRINT("\n\rERROR interrupt bit is set!");

      if ((int_stat & 0x00010000) == 0x00010000) {
         PRINT("\n\rCommand timeout error!");
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x00010000;  // clear interrupt
      }
      if ((int_stat & 0x00020000) == 0x00020000) {
         PRINT("\n\rCommand CRC error!");
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x00020000;  // clear interrupt
      }
      if ((int_stat & 0x00040000) == 0x00030000) {
         PRINT("\n\rCommand end bit error!");
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x00030000;  // clear interrupt
      }
      if ((int_stat & 0x00080000) == 0x00080000) {
         PRINT("\n\rCommand index error!");
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x00080000;  // clear interrupt
      }
      if ((int_stat & 0x00100000) == 0x00100000) {
         PRINT("\n\rData timeout error!");
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x00100000;  // clear interrupt
      }
      if ((int_stat & 0x00200000) == 0x00200000) {
         PRINT("\n\rData CRC error!");
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x00200000;  // clear interrupt
      }
      if ((int_stat & 0x00400000) == 0x00400000) {
         PRINT("\n\rData end bit error!");
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x00400000;  // clear interrupt
      }
      if ((int_stat & 0x00800000) == 0x00800000) {
         PRINT("\n\rCurrent limit error!");
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x00800000;  // clear interrupt
      }
      // Following two bits are not supported because logiSDHC does not support Auto CMD12 and ADMA
      //if ((int_stat & 0x01000000) == 0x01000000) {
      //   PRINT("\n\rAuto CMD12 error!");
      //}
      //if ((int_stat & 0x02000000) == 0x02000000) {
      //   PRINT("\n\rADMA error!");
      //}
      // Vendor specific errors
      if ((int_stat & 0x10000000) == 0x10000000) {
         //PRINT("\n\rRead buffer overflow error!");	// donot display warning message
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x10000000;  // clear interrupt
      }
      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0xffff0000;  // clear interrupt
   }

   return 0;
}

/****************************************************************************
*
* Writes single block of data to memory. Block size and memory address
*  is defined with input parameters.
*
* @param[in] address is memory address to write to.
*            block_size is the number of bytes to be written to memory;
 *             1<=block_size<=512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept this command
*
******************************************************************************/
void write_single(int address, int block_size) {

   PRINT("\n\n\rWrite single function START!");

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = write
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // CMD24 WRITE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD24 WRITE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // Block size reg;  Transfer block size = 512 bytes
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = 0x0000;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = address;  // command argument; For command 24 it is address to write to
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = write
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x183A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   // Do write command now
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 10)
   } while((present_state & 0x400) != 0x400);
   for (i=0; (IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0) & 0x400) == 0x400; i++) { // Write words to buffer until full
//      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20) = (i*4+3 & 0xFF)<<24 | (i*4+2 & 0xFF)<<16 | (i*4+1 & 0xFF)<<8 | (i*4 & 0xFF);
      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20) = 0x12345678;//0xa5a5a5a5;
      //IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20) = address;
   }
   //present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1)
   } while((present_state & 0x003) != 0x000);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   check_errors();
   PRINT("\n\rWrite single function END!\n\r");
}

/****************************************************************************
*
* Reads a single block of data. Block size and memory address
*  is defined with input parameters.
*
* @param[in] address is memory address to read from.
*            block_size is the number of bytes to be read from memory;
 *             1<=block_size<=512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept this command
*
******************************************************************************/
void read_single(int address, int block_size) {

   PRINT("\n\n\rRead single function START!");

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // CMD17 READ_SINGLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD17 READ_SINGLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // Block size reg;  Transfer block size = 512 bytes
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = 0x0000;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = address;  // command argument; For command 17 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0010;  // Transfer reg; Single transfer, Data direction = read
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x113A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   // Do Read command now
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 11)
   } while((present_state & 0x800) != 0x800);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   for (i=0; (IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0) & 0x800) == 0x800; i++) { // Read words from buffer until empty
      rd_buffer[i] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20);
   }
   //present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   for (j=0; j<i; j++) {
      PRINT("\n\rRead buffer %d = 0x%x", j, rd_buffer[j]);
   }

   check_errors();
   PRINT("\n\rRead single function END!\n\r");

}

/****************************************************************************
*
* Writes single block of data and then reads it. Block size and memory address
*  is defined with input parameters.
*
* @param[in] address is memory address to write to and read from.
*            block_size is the number of bytes to be written/read from memory;
 *             1<=block_size<=512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept this command
*
******************************************************************************/
void write_read_single(int address, int block_size) {

   int err=0;

   PRINT("\n\n\rWrite Read single function START!");

   /********************************************************/
   // Init write buffer
   /********************************************************/
   for (i=0; i<(block_size/4); i++) {
      wr_buffer[i] = ((i*4+3) & 0xFF)<<24 | ((i*4+2) & 0xFF)<<16 | ((i*4+1) & 0xFF)<<8 | (i*4 & 0xFF);
   }

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

#if 1
   /********************************************************/
   // CMD24 WRITE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD24 WRITE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // Block size reg;  Transfer block size = 512 bytes
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = 0x0000;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = address;  // command argument; For command 24 it is address to write to
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = write
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x183A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   // Do write command now
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 10)
   } while((present_state & 0x400) != 0x400);
   for (i=0; (IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0) & 0x400) == 0x400; i++) { // Write words to buffer until full
      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20) = wr_buffer[i]; //(i*4+3 & 0xFF)<<24 | (i*4+2 & 0xFF)<<16 | (i*4+1 & 0xFF)<<8 | (i*4 & 0xFF);
   }
   //present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1)
   } while((present_state & 0x003) != 0x000);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
#endif

#if 1
   /********************************************************/
   // CMD17 READ_SINGLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD17 READ_SINGLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // Block size reg;  Transfer block size = 512 bytes
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = 0x0000;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = address;  // command argument; For command 17 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0010;  // Transfer reg; Single transfer, Data direction = read
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x113A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   // Do Read command now
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 11)
   } while((present_state & 0x800) != 0x800);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   for (i=0; (IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0) & 0x800) == 0x800; i++) { // Read words from buffer until empty
      rd_buffer[i] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20);
   }
   //present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   for (j=0; j<i; j++) {
      if (rd_buffer[j] != wr_buffer[j]) {
         PRINT("\n\rERROR on index %d", j);
         PRINT("\n\rRead buffer  = 0x%x", rd_buffer[j]);
         PRINT("\n\rWrite buffer = 0x%x", wr_buffer[j]);
         err=1;
      }
   }
   if (err == 0) {
      PRINT("\n\rWrite read single block test of %d bytes was successful!", block_size);
   }
#endif

   check_errors();
   PRINT("\n\rWrite Read single function END!\n\r");

}

/****************************************************************************
*
* Performs multiple block read from SD Card.
*
* @param[in] sd_mem_address is address on SD Card to read from;
*            block_count is the number of blocks to transfer;
*            block_size is the number of bytes to read per block;
*               In multiple commands, logiSDIO supports only block size of 512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept multiple read command.
*            logiSDIO can send data to SD Host quite quickly, probably faster
*              than regular SD Card because there is no asyncrounous delay.
*              Because of this, in multiple read commands I have seen that if
*              sd_clk is fast (>12.5MHz, i.e. >(base clk/4)) SD Host + SW is
*              unable to empty data from it's receive buffer in time before new
*              data is received over SD. So make sure clock is lower thab 12.5 MHz.
*              This will probably be solved with DMA transfers where SD Host
*              should empty buffer more quickly as there is no SW involved.
******************************************************************************/
void read_multiple_block(int sd_mem_address, int block_count, int block_size) {

   PRINT("\n\n\rRead multiple block function START!");
   PRINT("\n\rNumber of blocks = %d, block size = %d, sd_mem_address = %d", block_count, block_size, sd_mem_address);

   present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   //PRINT("\n\n\rpstate = %d",present_state);

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // CMD18 READ_MULTIPLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD18 READ_MULTIPLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // Block size reg;  Transfer block size
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = block_count;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 18 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0032;  // Transfer reg; Multi transfer, Data direction = read, Block count enable, No DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x123A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
// Prints commented out in order for SW to be able to read data in time before new data comes from SD.
//   sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
//   PRINT("\n\rSD response = 0x%x", sd_response[0]);
//   PRINT("\n\rSD Host present state = 0x%x", present_state);
   i=0;
   while(1) {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 11)
      while ((present_state & 0x800) == 0x800) {
         rd_buffer[i] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20);
         present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 11)
         i++;
      }
      if ((present_state & 0x200) == 0) {  //Check Present state reg (bit 9)
         break;
      }
   }
//   present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg
//   PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // CMD12 STOP_TRANSMISSION, response R1b
   /********************************************************/
   //PRINT("\n\n\rCMD12 STOP_TRANSMISSION");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0C1B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);

   PRINT("\n\r");
   for (j=0; j<i; j++) {
      PRINT("\n\rRead buffer %d = 0x%x", j, rd_buffer[j]);
   }

   check_errors();
   PRINT("\n\rRead multiple block function END!\n\r");
}

/****************************************************************************
*
* Performs multiple block write to SD Card.
*
* @param[in] sd_mem_address is address on SD Card to write to;
*            block_count is the number of blocks to transfer;
*            block_size is the number of bytes to write per block;
*               In multiple commands, logiSDIO supports only block size of 512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept multiple write command.
*
******************************************************************************/
void write_multiple_block(int sd_mem_address, int block_count, int block_size) {

   PRINT("\n\n\rWrite multiple block function START!");
   PRINT("\n\rNumber of blocks = %d, block size = %d, sd_mem_address = %d", block_count, block_size, sd_mem_address);

//   for (i=0; i<(block_size/4*block_count); i++) {
//      wr_buffer[i] = (i*4+3 & 0xFF)<<24 | (i*4+2 & 0xFF)<<16 | (i*4+1 & 0xFF)<<8 | (i*4 & 0xFF);
//   }

   for (j=0; j<block_count; j++) {
      for (i=0; i<(block_size/4); i++) {
         wr_buffer[i+j*(block_size/4)] = j;
      }
   }

   //present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   //PRINT("\n\n\rpstate = %d",present_state);

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // CMD25 WRITE_MULTIPLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD25 WRITE_MULTIPLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // Block size reg;  Transfer block size = 512 bytes
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = block_count;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 25 it is address to write to
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0022;  // Transfer reg; Multi transfer, Data direction = write, Block count enable, No DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x193A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   i=0;
   while(1) {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 10)
      while ((present_state & 0x400) == 0x400) {                //Check Present state reg (bit 10). Write words to buffer until full
         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20) = wr_buffer[i];
         present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 10)
         i++;
      }
      if ((present_state & 0x100) == 0) {  //Check Present state reg (bit 8)
         break;
      }
   }
   //present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1)
   } while((present_state & 0x003) != 0x000);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // CMD12 STOP_TRANSMISSION, response R1b
   /********************************************************/
   //PRINT("\n\n\rCMD12 STOP_TRANSMISSION");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0C1B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);

   check_errors();
   PRINT("\n\rWrite multiple block function END!\n\r");

}

/****************************************************************************
*
* Performs multiple block write and read to/from SD Card and checks data.
*
* @param[in] sd_mem_address is address on SD Card to write/read to/from;
*            block_count is the number of blocks to transfer;
*            block_size is the number of bytes to write per block;
*               In multiple commands, logiSDIO supports only block size of 512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept multiple transfer command.
*            logiSDIO can send data to SD Host quite quickly, probably faster
*              than regular SD Card because there is no asyncrounous delay.
*              Because of this, in multiple read commands I have seen that if
*              sd_clk is fast (>12.5MHz, i.e. >(base clk/4)) SD Host + SW is
*              unable to empty data from it's receive buffer in time before new
*              data is received over SD. So make sure clock is lower than 12.5 MHz.
*              This will probably be solved with DMA transfers where SD Host
*              should empty buffer more quickly as there is no SW involved.
*
******************************************************************************/
void write_read_multiple_block(int sd_mem_address, int block_count, int block_size) {

   int err=0;

   PRINT("\n\n\rWrite Read multiple block function START!");
   PRINT("\n\rNumber of blocks = %d, block size = %d, sd_mem_address = %d", block_count, block_size, sd_mem_address);

#if 1
   for (i=0; i<(block_size/4*block_count); i++) {
//      wr_buffer[i] = (i*4+3 & 0xFF)<<24 | (i*4+2 & 0xFF)<<16 | (i*4+1 & 0xFF)<<8 | (i*4 & 0xFF);
//      wr_buffer[i] = i;
      wr_buffer[i] = 0xa5a5a5a5;
//      wr_buffer[i] = 0xa5a55a5a;
//      wr_buffer[i] = 0xf0f0f0f0;
//      wr_buffer[i] = 0x0f0f0f0f;
//      wr_buffer[i] = 0x000f000f;
//      wr_buffer[i] = 0x0;
   }

   //PRINT("\n\n\rnumber of words in wbuffer = %d",i);

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   //IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // CMD25 WRITE_MULTIPLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD25 WRITE_MULTIPLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // Block size reg;  Transfer block size = 512 bytes
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = block_count;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 25 it is address to write to
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0022;  // Transfer reg; Multi transfer, Data direction = write, Block count enable, No DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x193A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   i=0;
   while(1) {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 10)
      while ((present_state & 0x400) == 0x400) {                //Check Present state reg (bit 10). Write words to buffer until full
         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20) = wr_buffer[i];
         present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 10)
         i++;
      }
      if ((present_state & 0x100) == 0) {  //Check Present state reg (bit 8)
         //PRINT("\n\rwrite done i = %d",i);
         break;
      }
   }
   //present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1)
   } while((present_state & 0x003) != 0x000);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // CMD12 STOP_TRANSMISSION, response R1b
   /********************************************************/
   //PRINT("\n\n\rCMD12 STOP_TRANSMISSION");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0C1B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b

   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg command inhibit before doing anything
   while((present_state & SDHCI_CMD_INHIBIT) != 0);
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg DAT inhibit before doing anything
   while((present_state & SDHCI_DATA_INHIBIT) != 0);
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check for busy
      //PRINT("\n\n\rcard busy ");
   } while((present_state & SDHCI_DATA_BUSY) == 0);

   //PRINT("\n\n\rpresent state  %d ", present_state);

#endif
#if 1
   /********************************************************/
   // CMD18 READ_MULTIPLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD18 READ_MULTIPLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // Block size reg;  Transfer block size
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = block_count;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 18 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0032;  // Transfer reg; Multi transfer, Data direction = read, Block count enable, No DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x123A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
      //PRINT("\n\n\rCMD inhibit");
   } while((present_state & 0x1) != 0x0);
//   Prints commented out in order for SW to be able to read data in time before new data comes from SD.
//   sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
//   PRINT("\n\rSD response = 0x%x", sd_response[0]);
//   PRINT("\n\rSD Host present state = 0x%x", present_state);
   i=0;

   while(1) {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 11)
      while ((present_state & 0x800) == 0x800) {
         mult_rd_buffer[i] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x20);
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = mult_rd_buffer[i];               // -- for simulation in ModelSim
//         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = i;
         present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 11)
         i++;
      }
      if ((present_state & 0x200) == 0) {  //Check Present state reg (bit 9)
         break;
      }
   //PRINT("\n\rread active block done i = %d",i);   // bezveze komentar
   }
   //present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   //PRINT("\n\rnumber of words in rbuffer = %d",i);

   /********************************************************/
   // CMD12 STOP_TRANSMISSION, response R1b
   /********************************************************/
   //PRINT("\n\n\rCMD12 STOP_TRANSMISSION");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0C1B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg command inhibit before doing anything
   while((present_state & SDHCI_CMD_INHIBIT) != 0);
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg DAT inhibit before doing anything
   while((present_state & SDHCI_DATA_INHIBIT) != 0);

   //PRINT("\n\rpresent state  %d ", present_state);

#if SIM
   for (j=0; j<i; j++) {
      if (mult_rd_buffer[j] != wr_buffer[j]) {
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = j;
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = mult_rd_buffer[j];
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = wr_buffer[j];
      }
   }
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = 0x13131313;
#else
   for (j=0; j<i; j++) {
      if (mult_rd_buffer[j] != wr_buffer[j]) {
         PRINT("\n\rERROR on index %d", j);
         PRINT("\n\rRead buffer  = 0x%x", mult_rd_buffer[j]);
         PRINT("\n\rWrite buffer = 0x%x", wr_buffer[j]);
         err=1;
      }
   }
   if (err == 0) {
      PRINT("\n\rWrite read test of %d blocks was successful!", block_count);
   }
#endif

#endif

   check_errors();
   PRINT("\n\rWrite Read multiple block function END!\n\r");

}

/****************************************************************************
*
* Performs DMA read of single block from SD Card.
*
* @param[in] sd_mem_address is address on SD Card to read from;
*            host_mem_address is address in Host space where Host DMA will
*               write data received from SD Card;
*            block_size is the number of bytes to read per block;
*               1<=block_size<=512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept read command
*
******************************************************************************/
void dma_read_single_block(int sd_mem_address, int host_mem_address, int block_size) {

    int new_dma_address ;

   PRINT("\n\n\rRead DMA single block function START!");
   PRINT("\n\n\rblock size = %d", block_size);

   //Clear memory with some pattern
   for (i=0; i<block_size; i++){
      IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + host_mem_address + i*4) = 0xA0A1A2A3;
   }

   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x0) = host_mem_address;  // SDMA address

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   //********************************************************/
   // DMA READ
   // CMD17 READ_SINGLE_BLOCK, response R1
   //********************************************************/
   //PRINT("\n\n\rCMD17 READ_SINGLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // SDMA boundary ;  Transfer block size
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = 0x0000;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 17 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0011;  // Transfer reg; Single transfer, Data direction = read, Block count disable, DMA transfer
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0xFF;   //Interrupt status reg; clear all int. flags
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x113A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   while(1) {
      int_stat = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30); //Interrupt status reg
      if((int_stat & 0x8) == 0x8) {  //Check DMA interrupt flag (boundary reached)
        new_dma_address = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) ;  // next contigous address for DMA transfer
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) = new_dma_address;  // renew the DMA address
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x8; //Interrupt status reg; clear DMA finnish flag
      }
      if ((int_stat & 0x2) == 0x2) {  //Check transfer complete interrupt flag
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x2; //Interrupt status reg; clear transfer complete flag
         break;
      }

      //PRINT("\n\rint_stat = 0x%x", int_stat);
   }

   check_errors();
   PRINT("\n\rRead DMA single block function END!\n\r");

}

/****************************************************************************
*
* Performs DMA read of multiple blocks from SD Card.
*
* @param[in] sd_mem_address is address on SD Card to read from;
*            host_mem_address is address in Host space where Host DMA will
*               write data received from SD Card;
*            block_count is the number of blocks to transfer;
*            block_size is the number of bytes to read per block;
*               1<=block_size<=512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept read command
*
******************************************************************************/
void dma_read_multiple_block(int sd_mem_address, int host_mem_address, int block_count, int block_size) {

    int new_dma_address ;

   PRINT("\n\n\rRead DMA multiple block function START!");
   PRINT("\n\n\rNumber of blocks = %d, block size = %d", block_count, block_size);

//   //Clear memory with some pattern
//   for (i=0; i<block_size*block_count/4; i++){
//      IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + host_mem_address + i*4) = 0xA0A1A2A3;
//   }

   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x0) = host_mem_address;  // SDMA address

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // DMA READ
   /********************************************************/
   // CMD18 READ_MULTIPLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD18 READ_MULTIPLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // SDMA boundary ;  Transfer block size
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = block_count;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 18 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0033;  // Transfer reg; Multi transfer, Data direction = read, Block count enable, DMA transfer
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0xFF;   //Interrupt status reg; clear all int. flags
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x123A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   while(1) {
      int_stat = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30); //Interrupt status reg
      if((int_stat & 0x8) == 0x8) {  //Check DMA interrupt flag (boundary reached)
        new_dma_address = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) ;  // next contigous address for DMA transfer
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) = new_dma_address;  // renew the DMA address
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x8; //Interrupt status reg; clear DMA finnish flag
        PRINT("\n\r DMA boundary detected!\n\r");
      }
      if ((int_stat & 0x2) == 0x2) {  //Check transfer complete interrupt flag
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x2; //Interrupt status reg; clear transfer complete flag
         break;
      }
   //   PRINT("\n\rint_stat = 0x%x", int_stat);
   } ;

   /********************************************************/
   // CMD12 STOP_TRANSMISSION, response R1b
   /********************************************************/
   //PRINT("\n\n\rCMD12 STOP_TRANSMISSION");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0C1B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b

   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg command inhibit before doing anything
   while((present_state & SDHCI_CMD_INHIBIT) != 0);
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg DAT inhibit before doing anything
   while((present_state & SDHCI_DATA_INHIBIT) != 0);
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check for busy
      PRINT("\n\n\rcard busy ");
   } while((present_state & SDHCI_DATA_BUSY) == 0);

   check_errors();
   PRINT("\n\rRead DMA multiple block function END!\n\r");

}

/****************************************************************************
*
* Performs DMA write of single block to SD Card.
*
* @param[in] sd_mem_address is address on SD Card to read from;
*            host_mem_address is address in Host space where Host DMA will
*               write data received from SD Card;
*            block_size is the number of bytes to read per block;
*               1<=block_size<=512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept read command
*
******************************************************************************/
void dma_write_single_block(int sd_mem_address, int host_mem_address, int block_size) {

    int new_dma_address ;

   PRINT("\n\n\rWrite DMA single block function START!");
   PRINT("\n\n\rblock size = %d", block_size);

   //Clear memory with some pattern
   for (i=0; i<block_size; i++){
      IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + host_mem_address + i*4) = 0xA0A1A2A3;
   }

   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x0) = host_mem_address;  // SDMA address

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   //********************************************************/
   // DMA WRITE
   /*********************************************************/
   // CMD24 WRITE_SINGLE_BLOCK, response R1
   //********************************************************/
   //PRINT("\n\n\rCMD24 WRITE_SINGLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // SDMA boundary ;  Transfer block size
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = 0x0000;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 17 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0001;  // Transfer reg; Single transfer, Data direction = write, Block count disable, DMA transfer
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0xFF;   //Interrupt status reg; clear all int. flags
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x183A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   while(1) {
      int_stat = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30); //Interrupt status reg
      if((int_stat & 0x8) == 0x8) {  //Check DMA interrupt flag (boundary reached)
        new_dma_address = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) ;  // next contigous address for DMA transfer
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) = new_dma_address;  // renew the DMA address
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x8; //Interrupt status reg; clear DMA finnish flag
      }
      if ((int_stat & 0x2) == 0x2) {  //Check transfer complete interrupt flag
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x2; //Interrupt status reg; clear transfer complete flag
         break;
      }

      //PRINT("\n\rint_stat = 0x%x", int_stat);
   }

   check_errors();
   PRINT("\n\rWrite DMA single block function END!\n\r");

}

/****************************************************************************
*
* Performs DMA write of multiple blocks to SD Card.
*
* @param[in] sd_mem_address is address on SD Card to read from;
*            host_mem_address is address in Host space where Host DMA will
*               write data received from SD Card;
*            block_count is the number of blocks to transfer;
*            block_size is the number of bytes to read per block;
*               1<=block_size<=512
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept read command
*
******************************************************************************/
void dma_write_multiple_block(int sd_mem_address, int host_mem_address, int block_count, int block_size) {

    int new_dma_address ;

   PRINT("\n\n\rWrite DMA multiple block function START!");
   PRINT("\n\n\rNumber of blocks = %d, block size = %d", block_count, block_size);

   //Clear memory with some pattern

    #if 0    // pattern = 0xA0A1A2A3  or position of the word
       for (i=0; i<block_size*block_count/4+1; i++){
          //IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + host_mem_address + i*4) = 0xA0A1A2A3;
          IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + host_mem_address + i*4) = i;
       }
    #endif

    #if 0    // pattern is number of the current block
       for (i=0; i< block_count; i++){
          for (j=0; j< (block_size/4); j++){
    //         IN32(XPAR_SD_HOST_0_VMEM_BASEADDR + host_mem_address + (i*block_size/4 + j*4)) = j;
             IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + host_mem_address + i*(block_size/4)*4 + j*4) = j;
    //        PRINT("\n\n\rj = %d adr = %x",j,XPAR_SD_HOST_0_VMEM_BASEADDR + (i*block_size/4 + j*4));
          }
       }
    #endif

//   for (i=0; i<block_size*block_count/4+1; i++){
//      IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + host_mem_address + i*4) = 0xA0A1A2A3;
//   }

   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x0) = host_mem_address;  // SDMA address

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // DMA WRITE
   /********************************************************/
   // CMD25 WRITE_MULTIPLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD25 READ_MULTIPLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // SDMA boundary ;  Transfer block size
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = block_count;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 18 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0023;  // Transfer reg; Multi transfer, Data direction = read, Block count enable, DMA transfer
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0xFF;   //Interrupt status reg; clear all int. flags
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x193A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   while(1) {
      int_stat = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30); //Interrupt status reg
      if((int_stat & 0x8) == 0x8) {  //Check DMA interrupt flag (boundary reached)
        new_dma_address = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) ;  // next contigous address for DMA transfer
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) = new_dma_address;  // renew the DMA address
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x8; //Interrupt status reg; clear DMA finnish flag
      }
      if ((int_stat & 0x2) == 0x2) {  //Check transfer complete interrupt flag
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x2; //Interrupt status reg; clear transfer complete flag
         break;
      }
      PRINT("\n\rint_stat = 0x%x", int_stat);
   } ;

   /********************************************************/
   // CMD12 STOP_TRANSMISSION, response R1b
   /********************************************************/
//   PRINT("\n\n\rCMD12 STOP_TRANSMISSION");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0C1B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b

   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg command inhibit before doing anything
   while((present_state & SDHCI_CMD_INHIBIT) != 0);
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg DAT inhibit before doing anything
   while((present_state & SDHCI_DATA_INHIBIT) != 0);
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check for busy
      PRINT("\n\n\rcard busy ");
   } while((present_state & SDHCI_DATA_BUSY) == 0);

   check_errors();
   PRINT("\n\rWrite DMA multiple block function END!\n\r");

}

/****************************************************************************
*
* Performs multiple DMA block write and read to/from SD Card and checks data.
*
* @param[in] sd_mem_address is address on SD Card to write/read to/from;
*            host_mem_address is address in Host space where Host DMA will write data received from SD Card;
*            source_data_addr is address in Host space that will be filled with data which will be written to the SD
*            block_count is the number of blocks to transfer;
*            block_size is the number of bytes to write per block;
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept multiple transfer command.
*
******************************************************************************/
void dma_write_read_multiple_block(int sd_mem_address, int host_mem_address, int source_data_address, int block_count, int block_size) {

    int new_dma_address, r_data, w_data ;

   PRINT("\n\n\rWrite Read DMA multiple block function START!");
   PRINT("\n\n\rNumber of blocks = %d, block size = %d", block_count, block_size);

   //Clear memory with some pattern => initialize data to be written on the SD

#if 0    // pattern = 0xA0A1A2A3  or position of the word
   for (i=0; i<block_size*block_count/4+1; i++){
      //IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + source_data_address + i*4) = 0xA0A1A2A3;
      IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + source_data_address + i*4) = i;
   }
#endif

#if 1    // pattern is number of the current block
   for (i=0; i< block_count; i++){
      for (j=0; j< (block_size/4); j++){
//         IN32(XPAR_SD_HOST_0_VMEM_BASEADDR + source_data_address + (i*block_size/4 + j*4)) = j;
         IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + source_data_address + i*(block_size/4)*4 + j*4) = j;
//        PRINT("\n\n\rj = %d adr = %x",j,XPAR_SD_HOST_0_VMEM_BASEADDR + (i*block_size/4 + j*4));
      }
   }
#endif

   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x0) = source_data_address;  // SDMA address

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // DMA WRITE
   /********************************************************/
   // CMD25 WRITE_MULTIPLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD25 WRITE_MULTIPLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // SDMA boundary ;  Transfer block size
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = block_count;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 18 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0023;  // Transfer reg; Multi transfer, Data direction = read, Block count enable, DMA transfer
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0xFF;   //Interrupt status reg; clear all int. flags
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x193A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   while(1) {
      int_stat = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30); //Interrupt status reg
      if((int_stat & 0x8) == 0x8) {  //Check DMA interrupt flag (boundary reached)
        new_dma_address = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) ;  // next contigous address for DMA transfer

        PRINT("\n\rDMA write continued at = 0x%x", new_dma_address);

        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) = new_dma_address;  // renew the DMA address
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x8; //Interrupt status reg; clear DMA finnish flag
      }
      if ((int_stat & 0x2) == 0x2) {  //Check transfer complete interrupt flag
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x2; //Interrupt status reg; clear transfer complete flag
         break;
      }
   };

   new_dma_address = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) ;  // next contigous address for DMA transfer
   PRINT("\n\rDMA write finished at = 0x%x", new_dma_address);

   /********************************************************/
   // CMD12 STOP_TRANSMISSION, response R1b
   /********************************************************/
//   PRINT("\n\n\rCMD12 STOP_TRANSMISSION");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0C1B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b

   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg command inhibit before doing anything
   while((present_state & SDHCI_CMD_INHIBIT) != 0);
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg DAT inhibit before doing anything
   while((present_state & SDHCI_DATA_INHIBIT) != 0);
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check for busy
      PRINT("\n\n\rcard busy ");
   } while((present_state & SDHCI_DATA_BUSY) == 0);

   /********************************************************/
   // DMA READ
   /********************************************************/
   // CMD18 READ_MULTIPLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD18 READ_MULTIPLE_BLOCK");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x0) = host_mem_address;  // SDMA address
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // SDMA boundary ;  Transfer block size
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = block_count;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = sd_mem_address;  // command argument; For command 18 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0033;  // Transfer reg; Multi transfer, Data direction = read, Block count enable, DMA transfer
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0xFF;   //Interrupt status reg; clear all int. flags
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x123A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   while(1) {
      int_stat = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30); //Interrupt status reg
      if((int_stat & 0x8) == 0x8) {  //Check DMA interrupt flag (boundary reached)
        new_dma_address = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) ;  // next contigous address for DMA transfer

        PRINT("\n\rDMA read continued at = 0x%x", new_dma_address);

        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) = new_dma_address;  // renew the DMA address
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x8; //Interrupt status reg; clear DMA finnish flag
      }
      if ((int_stat & 0x2) == 0x2) {  //Check transfer complete interrupt flag
        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x2; //Interrupt status reg; clear transfer complete flag
         break;
      }
   };

   new_dma_address = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + 0x0) ;  // next contigous address for DMA transfer
   PRINT("\n\rDMA read finished at = 0x%x", new_dma_address);

   /********************************************************/
   // CMD12 STOP_TRANSMISSION, response R1b
   /********************************************************/
//   PRINT("\n\n\rCMD12 STOP_TRANSMISSION");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0C1B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b

   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg command inhibit before doing anything
   while((present_state & SDHCI_CMD_INHIBIT) != 0);
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check Present state reg DAT inhibit before doing anything
   while((present_state & SDHCI_DATA_INHIBIT) != 0);
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + SDHC_PRESENT_STATE_ROFF);  //Check for busy
      PRINT("\n\n\rcard busy ");
   } while((present_state & SDHCI_DATA_BUSY) == 0);

   /********************************************************/
   // compare read and writte buffers and report errors
   // if there are any
   /********************************************************/

#if SIM
//   for (i=0; i<block_size*block_count/4; i++){
//      w_data = IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + source_data_address + i*4);
//      r_data = IN32(XPAR_LOGISDHC_0_MEM_BASEADDR + host_mem_address + i*4);
//      if (w_data != r_data) {
//        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = i;
//        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = r_data;
//        IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = w_data;
//      }
//   }
//   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR) = 0x13131313;
#else
   PRINT("\n\r");
   for (i=0; i<block_size*block_count/4; i++){
      w_data = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + source_data_address + i*4);
      r_data = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + host_mem_address + i*4);
      if (w_data != r_data) {
         PRINT("\n\rERROR on index %d", i);
         PRINT("\n\rRead buffer  = 0x%x", r_data);
         PRINT("\n\rWrite buffer = 0x%x", w_data);
         //while(1);
      }
   }
#endif

   check_errors();
   PRINT("\n\rWrite Read DMA multiple block function END!\n\r");

}

/****************************************************************************
*
* Performs a byte write to SDIO register (IO_RW_DIRECT).
*
* @param[in] func is the number of the function on SDIO Card writes to;
*            reg_addr is register address on SDIO Card to write to;
*            data is the value to be written;
*            raw_flag is read after write flag; When '1', a read is performed
*                     after write.
*
* @return    Read data after write if raw_flag was set.
*
* @note      SD Card has to be in transfer state to accept command.
*
******************************************************************************/
unsigned char write_io_byte(unsigned char func, unsigned int reg_addr, unsigned char data, unsigned char raw_flag) {

   PRINT("\n\n\rWrite to SDIO register function START!");
   PRINT("\n\rfunc = %d, reg_addr = 0x%x, data = 0x%x, raw_flag = %d", func, reg_addr, data, raw_flag);

   /********************************************************/
   // CMD52 IO_RW_DIRECT, response R5
   /********************************************************/
   //PRINT("\n\rCMD52 IO_RW_DIRECT");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = (0x1<<31) | (func<<28) | (raw_flag<<27) | (0x1<<26) | (reg_addr<<9) | (0x1<<8) | data;  // command argument; r/w, func_num, raw_flag, stuff, addr, data
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x341A;  // Command reg; Command index, no data, command index check, CRC check, Response R5
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   check_errors();
   PRINT("\n\rWrite to SDIO register function END!\n\r");
   return (unsigned char) sd_response[0];

}

/****************************************************************************
*
* Performs a byte read from SDIO register (IO_RW_DIRECT).
*
* @param[in] func is the number of the function on SDIO Card reads from;
*            reg_addr is register address on SDIO Card to read from;
*
* @return    Read data after write if raw_flag was set.
*
* @note      SD Card has to be in transfer state to accept command.
*
******************************************************************************/
unsigned char read_io_byte(unsigned char func, unsigned int reg_addr) {

   PRINT("\n\n\rRead from SDIO register function START!");
   PRINT("\n\rfunc = %d, reg_addr = 0x%x", func, reg_addr);

   /********************************************************/
   // CMD52 IO_RW_DIRECT, response R5
   /********************************************************/
   //PRINT("\n\rCMD52 IO_RW_DIRECT");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = (0x0<<31) | (func<<28) | (0x0<<27) | (0x1<<26) | (reg_addr<<9) | (0x1<<8) | 0x00;  // command argument; r/w, func_num, raw_flag, stuff, addr, data
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x341A;  // Command reg; Command index, no data, command index check, CRC check, Response R5
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   check_errors();
   PRINT("\n\rRead from SDIO register function END!\n\r");
   return (unsigned char) sd_response[0];

}



/****************************************************************************
*
* Erases SD blocks.
*
* @param[in] ;
*            ;
*
* @return    .
*
* @note      .
*
******************************************************************************/

void Erase_SD_Blocks(int start_addr, int end_addr) {

   PRINT("\n\n\rSD Erase function START!");

   /********************************************************/
   // CMD32 ERASE_WR_BLK_START, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD32 ERASE_WR_BLK_START");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = start_addr;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x201A;  // Command reg; Command index, no data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1)
   } while((present_state & 0x3) != 0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   /********************************************************/

   /********************************************************/
   // CMD33 ERASE_WR_BLK_END, response R1
   /********************************************************/
   //PRINT("\n\n\rCMD33 ERASE_WR_BLK_END");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = end_addr;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x211A;  // Command reg; Command index, no data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1)
   } while((present_state & 0x3) != 0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   /********************************************************/

   /********************************************************/
   // CMD38 ERASE, response R1b
   /********************************************************/
   //PRINT("\n\n\rCMD38 ERASE");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x00000000;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x261B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0, 1, 20)
   } while((present_state & 0x100003) != 0x100000);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD Host present state = 0x%x", present_state);
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   /********************************************************/

   check_errors();
   PRINT("\n\rSD Erase function END!\n\r");

}

/****************************************************************************
*
* Performs Xylon type DMA read of multiple block with dummy byte insertion from SD Card.
*
* @param[in] sd_mem_address is address on SD Card to read from;
*            host_mem_address is address in Host space where Host DMA will
*               write data received from SD Card;
*            pix_start_byte is start offset inside first block
*            hres is horizontal rezolution of the picture that is transfered
*            vres is vertical rezolution of the picture that is transfered
*
* @return    None.
*
* @note      SD Card has to be in transfer state to accept read command
*
******************************************************************************/
void xylon_dma_read_multiple_block_dummy(int sd_mem_address, int host_mem_address, short pix_start_byte, short hres, short vres) {

   //int tmp;
   char dummy_byte;

   PRINT("\n\n\rXylon DMA Read multiple block with dummy byte insertion function START!");
   block_size = 0x200;
   // total number of bytes to transfer is (hres * vres * 3 Bpp + offset)/block size.
   // But if hres is not dividible by 4 then bmp SW adds additional bytes to the end of each line.
   // If hres lower 2 bits are 1 then 1 byte is added, if 2 then two and if three then
   // three bytes are added at every line end.
   dummy_byte = hres % 4;
   byte_cnt = ((hres * vres * 3) + dummy_byte * vres + pix_start_byte);
   block_cnt = byte_cnt / 512;
   if (byte_cnt % 512 != 0) {
      block_cnt = block_cnt + 1;
   }

   PRINT("\n\rNumber of blocks %d, block size = %d", block_cnt, block_size);
   PRINT("\n\rpix_start_byte = %d", pix_start_byte);
   PRINT("\n\rHres = %d, Vres = %d", hres, vres);
   PRINT("\n\rsd_mem_address = %d, host_mem_address = 0x%x", sd_mem_address, host_mem_address);

#if SIM==0
   // clear memory
   for (i=0; i<block_size*block_cnt*(4/3)/4+1; i++){  // (4/3) because of dummy byte insertion
      IN32(XPAR_LOGISDHC_0_REGS_BASEADDR + host_mem_address + i*4) = 0x00000000;
   }
#endif

   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x0) = host_mem_address;  // SDMA address

   /********************************************************/
   // CMD16 SET_BLOCKLEN, response R1
   /********************************************************/
   //PRINT("\n\rCMD16 SET_BLOCKLEN");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = block_size;  // command argument; For command 16 it is block length
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer, Data direction = read, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x101A;  // Command reg; Command index, no data, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   /********************************************************/
   // DMA READ
   /********************************************************/
   // CMD18 READ_MULTIPLE_BLOCK, response R1
   /********************************************************/
   //PRINT("\n\rCMD18 READ_MULTIPLE_BLOCK");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x4) = block_size;  // Block size reg;  Transfer block size
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x6) = block_cnt;  // Block count reg
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x060000;  // command argument; For command 18 it is address to read from
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0133;  // Transfer reg; Insert dummy, Multi transfer, Data direction = read, Block count enable, DMA transfer
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xF0) = pix_start_byte;  // Pix start byte reg;
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xF4) = hres;  // Horizontal resoltion reg;
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xF6) = vres;  // Vertical resoltion reg;
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x123A;  // Command reg; Command index, data present, command index check, CRC check, Response R1
   do {
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0)
   } while((present_state & 0x1) != 0x0);
   //sd_response[0] = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x10); // read response argument
   //PRINT("\n\rSD response = 0x%x", sd_response[0]);
   //PRINT("\n\rSD Host present state = 0x%x", present_state);

   do {
      int_stat = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30); //Interrupt status reg; Wait until DMA finnished
      //PRINT("\n\rint_stat = 0x%x", int_stat);
   } while((int_stat & 0x8) != 0x8);
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x30) = 0x8; //Interrupt status reg; clear DMA finnish flag

   /********************************************************/
   // CMD12 STOP_TRANSMISSION, response R1b
   /********************************************************/
   //PRINT("\n\n\rCMD12 STOP_TRANSMISSION");
   IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x8) = 0x0;  // command argument
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xC) = 0x0000;  // Transfer reg; Single transfer,
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x0C1B;  // Command reg; Command index, No data, command index check, CRC check, Response R1b
   do
      present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24);  //Check Present state reg (bit 0 and 1)
   while((present_state & 0x3) != 0);

   check_errors();
   PRINT("\n\rXylon DMA Read multiple block with dummy byte insertion function END!\n\r");

}
#endif
