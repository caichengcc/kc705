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
* @file test.c
*
*
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
******************************************************************************/

// INCLUDES //
#include "xparameters.h"    // Located in: microblaze_0/include/xparameters.h
#include <stdio.h>
#include <string.h>
#include "xutil.h"
#include "xio.h"
#include "sd_test.h"
#include "board_test_app.h"

// VARIABLES //
extern unsigned int sd_status, sd_card_addr;

void wait(unsigned int i);
void memory_test(void);

// MAIN //

extern char *logbuf;
extern int logenable;


int hello_sd (void) {

   #if SIM==0
 // Init MB caches
 //PRINT("\r\n DCacheInit+");
//   microblaze_disable_dcache();
//   microblaze_init_dcache_range(XPAR_LOGIMEM_0_BASEADDR,XPAR_MICROBLAZE_0_DCACHE_BYTE_SIZE);
//   microblaze_enable_dcache();
 //PRINT("\r\n DCacheInit-");
 //PRINT("\r\n ICacheInit+");
//   microblaze_disable_icache();
//   microblaze_init_icache_range(XPAR_LOGIMEM_0_BASEADDR,XPAR_MICROBLAZE_0_CACHE_BYTE_SIZE);
//   microblaze_enable_icache();
 //PRINT("\r\n ICacheInit-");
   #endif
	int status;

   //int k,x,y, layer_num;
   //XStatus Status;
   //int instance_id, layer, tmp;
	//char temp[40];

//   while(1)
//   {

	xil_printf("\n\r********************************************************");
	xil_printf("\n\r********************************************************");
	xil_printf("\n\r**     KC705 - SD Test                                **");
	xil_printf("\n\r********************************************************");
	xil_printf("\n\r********************************************************\r\n");
   //PRINT("\n\r");
   //PRINT("******************************************************\r\n");
   //PRINT("*****    ~~|     ##                        |~~   *****\r\n");
   //PRINT("****    ~~~|     ## logicBRICKS(TM)        |~~~   ****\r\n");
   //PRINT("***    ~~~~|     ####                      |~~~~   ***\r\n");
   //PRINT("****    ~~~|     -----------------         |~~~   ****\r\n");
   //PRINT("*****    ~~|     Designed by XYLON         |~~   *****\r\n");
   //PRINT("******************************************************\r\n");
//   }

#if 1

   /********************************************************/
   // Configure SD Host
   /********************************************************/
   xil_printf("\r\n===>>> configure SD host <<<=== \r\n");
   if(logenable==1) strcat(logbuf, "\r\n===>>> configure SD host <<<=== \r\n");

   IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2F) = 0x01;  // Soft reset
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2C) = 0x205;  // Clock ctrl reg; SD clock frequency = 50MHz/2, SD clk en, Internal clk en
   IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x28) = 0x00;  // Host ctrl reg; High speed disabled, Data trans width = 1, (0x06; Host ctrl reg; High speed enabled, Data trans width = 4)  ; 0x04 High speed
   IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x29) = 0x0F;  // Power ctrl reg; SD Bus voltage select, SD Bus Power
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x36) = 0xFFFF;  // Error interrupt status en reg; Enable all
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x34) = 0xFFFF;  // Normal interrupt status en reg; Enable all
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x38) = 0x0008;  // Normal interrupt signal en reg; Enable all
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x3A) = 0x0000;  // Error interrupt signal en reg; dis Enable all

	#if 1
   xil_printf("Dump SD configuration registers \r\n");
   xil_printf("------------------------------------\r\n");
   xil_printf("offset 0x2F: written 0x01.    Read: 0x%x \r\n", IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2F) );
   xil_printf("offset 0x2C: written 0x205.   Read: 0x%x \r\n", IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2C) );
   xil_printf("offset 0x28: written 0x00.    Read: 0x%x \r\n", IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x28) );
   xil_printf("offset 0x29: written 0x0F.    Read: 0x%x \r\n", IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x29) );
   xil_printf("offset 0x36: written 0xFFFF.  Read: 0x%x \r\n", IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x36) );
   xil_printf("offset 0x34: written 0xFFFF.  Read: 0x%x \r\n", IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x34) );
   xil_printf("offset 0x38: written 0x0008.  Read: 0x%x \r\n", IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x38) );
   xil_printf("offset 0x3A: written 0x00.    Read: 0x%x \r\n\r\n", IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x3A) );
	#endif
   /********************************************************/
   // Initialize SD Card
   /********************************************************/
   xil_printf("===>>> initialize SD host <<<===\n");
   status = Init_SD_Card();
	#if 1
   if(status == -1)
   {
	   xil_printf("\r\nSD Card not accessible!!!...Trying once again.\n");
	   // TRY ONE MORE TIME
	   status = Init_SD_Card();
	   if(status == -1)
	   {
			#if 1
		   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0xE) = 0x000000C0;  // Command abort
		   IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2F) = 0x01;  	  // Soft reset
		   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x36) = 0xFFFF;  // Error interrupt status en reg; Enable all
		   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x34) = 0xFFFF;  // Normal interrupt status en reg; Enable all
		   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x38) = 0x0008;  // Normal interrupt signal en reg; Enable all
		   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x3A) = 0x0000;  // Error interrupt signal en reg; dis Enable all
			#endif
		   xil_printf("\r\nERROR !!! SD Card not inserted.\nInsert the card and try again.\n");
		   return -1;
	   }
   }
	#endif
   sd_card_addr = status;
   PRINT("\r\nSD Card Address = 0x%x\r\n", sd_card_addr);

   /********************************************************/
   // Request Card to sent its CSD registar
   /********************************************************/
   xil_printf("\r\n===>>> request card to send its CSD register <<<=== \r\n");
   if(logenable==1) strcat(logbuf, "\r\n===>>> request card to send its CSD register <<<=== \r\n");
   send_csd(sd_card_addr);

   /********************************************************/
   // Select SD Card
   /********************************************************/
   xil_printf("\r\n===>>> select SD card <<<=== \r\n");
   if(logenable==1) strcat(logbuf, "\r\n===>>> select SD card <<<=== \r\n");
   select_sd_card(sd_card_addr);

   /********************************************************/
   // Request Card to sent its status
   /********************************************************/
   xil_printf("\r\n===>>> request card to send its status <<<=== \r\n");
   if(logenable==1) strcat(logbuf, "\r\n===>>> request card to send its status <<<=== \r\n");
   send_status(sd_card_addr);

   /********************************************************/
   // Set bus width to 4
   /********************************************************/
   xil_printf("\r\n===>>> set bus width to 4 <<<=== \r\n");
   if(logenable==1) strcat(logbuf, "\r\n===>>> set bus width to 4 <<<=== \r\n");
   set_sd_bus_width(sd_card_addr, 4);

   /********************************************************/
   // Enable High Speed Mode
   /********************************************************/
   // XYLON suggested to disable High Speed Mode, set to 25MHz.
   //xil_printf("\r\n===>>> enable high speed mode <<<=== \r\n");
   //high_speed_mode_en();

   xil_printf("\r\n===>>> enable Normal speed <<<=== \r\n");
   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2C) = 0x105;  // Clock ctrl reg; SD clock frequency = 50MHz/2, SD clk en, Internal clk en


   /********************************************************/
   // Write and/or Read Single block
   /********************************************************/
   xil_printf("\r\n===>>> write single block <<<=== \r\n");
   if(logenable==1) strcat(logbuf, "\r\n===>>> write single block <<<=== \r\n");
   write_single(0x000, 0x200);

//   while(1)
//   {
//      write_single(0x0, 0x200);
//      read_single(0x000, 0x200);
//   }
   xil_printf("\r\n===>>> write_read single block <<<=== \r\n");
   if(logenable==1) strcat(logbuf, "\r\n===>>> write_read single block <<<=== \r\n");
   write_read_single(0x000, 0x200);
//   }

   xil_printf("\r\n===>>> write_read multiple blocks <<<=== \r\n");
   if(logenable==1) strcat(logbuf, "\r\n===>>> write_read multiple blocks <<<=== \r\n");
   write_read_multiple_block(0x000, 5, 0x200);

//   while(1)
//   {
      //xil_printf("\r\n===>>> write_read multiple blocks <<<=== \r\n");
      //write_read_multiple_block(0x000, 15, 0x200);
//   }

   /********************************************************/
   // DMA Write and/or Read Single block
   /********************************************************/

//   dma_write_single_block(0, 0, 0x200);

//   dma_read_single_block(0, 0, 0x200);

   /********************************************************/
   // Write and/or Read Multiple blocks
   /********************************************************/
//   write_multiple_block(0x600, 4, 0x200);
//   write_single(0x0, 0x200);
//   read_single(0x0, 0x200);

//   write_single(0x200, 0x200);
//   write_single(0x400, 0x200);
//   write_single(0x600, 0x200);

//   read_multiple_block(0x600, 4, 0x200);
//   write_multiple_block(0x0, 4, 0x200);
//   write_read_multiple_block(0x0, 15, 0x200);

   /********************************************************/
   // DMA Write and/or Read Multiple blocks
   /********************************************************/

//   dma_write_multiple_block(int sd_mem_address, int host_mem_address, int block_count, int block_size);

//   dma_read_multiple_block(int sd_mem_address, int host_mem_address, int block_count, int block_size);

//   dma_write_read_multiple_block(int sd_mem_address, int host_mem_address, int source_data_address, int block_count, int block_size);


   /********************************************************/
   // Xylon type DMA read multiple block with dummy byte insertion
   /********************************************************/

//   for (k=2; k<1024; k++) {
//      xylon_dma_read_multiple_block_dummy(0, 0, 0, k, 30);
//   }
//   for (k=2; k<1024; k++) {
//      xylon_dma_read_multiple_block_dummy(0, 0, 0, 17, k);
//   }

   /********************************************************/
   // SD Standard V.2.00 DMA read multiple block
   /********************************************************/
	/*dma_read_multiple_block(int sd_mem_address, int host_mem_address, int block_count, int block_size)*/

//   dma_read_multiple_block(0, 0xC38, 100260, 512) ;

   /********************************************************/
   // SD Standard V.2.00 DMA write multiple block
   /********************************************************/
    /*dma_write_multiple_block(int sd_mem_address, int host_mem_address, int block_count, int block_size)*/
//   dma_write_multiple_block(0xE00, 0xC3b, 101390, 512);
//   dma_write_multiple_block(0, 0xD40, 3, 512);
   /********************************************************/
   // Request Card to sent its SCR registar
   /********************************************************/
//   send_scr();

   /********************************************************/
   // Erase SD Card Blocks
   /********************************************************/
//   Erase_SD_Blocks(0x00000000, 0x00010000);

   //while(1);
       IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2F) = 0x01;  // Soft reset
	   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x36) = 0xFFFF;  // Error interrupt status en reg; Enable all
	   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x34) = 0xFFFF;  // Normal interrupt status en reg; Enable all
	   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x38) = 0x0008;  // Normal interrupt signal en reg; Enable all
	   IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x3A) = 0x0000;  // Error interrupt signal en reg; dis Enable all
	   xil_printf("SD Test Passed\r\n\r\n");
      if(logenable==1) strcat(logbuf, "SD Test Passed\r\n\r\n");

#endif
      return 0;
}



// FUNCTIONS //

void wait(unsigned int i) {
	volatile unsigned int k;
	for(k=0;k<i;k++);
}


#if 0

void memory_test (void){

    #define MEM_SIZE        (16*1024*1024)
    #define MEM_TEST_SIZE   (MEM_SIZE)
    #define MEM_TEST_OFFSET (0)
    #define X32(addr) (*(volatile Xuint32 *) (addr))
    #define X16(addr) (*(volatile Xuint16 *) (addr))
    #define X8(addr)  (*(volatile Xuint8 *)  (addr))

    /* Testing DDR Memory (Micron_DDR_MT46V16M16TG_75)*/
    PRINT("\n\rStarting MemoryTest for Micron_DDR_MT46V16M16_75:\r\n");

    XStatus status;
    int p,f;

    p=f=0;

    do
    {

        PRINT("  Running 32-bit test...");
        status = XUtil_MemoryTest32((Xuint32*)(XPAR_LOGIMEM_0_MB_BASEADDR+MEM_TEST_OFFSET), MEM_TEST_SIZE/4, 0xAAAA5555, XUT_ALLMEMTESTS);
        if (status == XST_SUCCESS) {

           PRINT(" PASSED!\r\n");
        }
        else {
           PRINT(" FAILED!\r\n");
           f++;
        }
        p++;

        PRINT("  Running 16-bit test...");
        status = XUtil_MemoryTest16((Xuint16*)(XPAR_LOGIMEM_0_MB_BASEADDR+MEM_TEST_OFFSET), MEM_TEST_SIZE/2, 0xAA55, XUT_ALLMEMTESTS);
        if (status == XST_SUCCESS) {
           PRINT(" PASSED!\r\n");
        }
        else {
           PRINT(" FAILED!\r\n");
           f++;
        }
        p++;

        PRINT("  Running  8-bit test...");
        status = XUtil_MemoryTest8((Xuint8*)(XPAR_LOGIMEM_0_MB_BASEADDR+MEM_TEST_OFFSET), MEM_TEST_SIZE, 0xA5, XUT_ALLMEMTESTS);
        if (status == XST_SUCCESS) {
           PRINT(" PASSED!\r\n");
        }
        else {
           PRINT(" FAILED!\r\n");
           f++;
        }
        p++;

        PRINT(" Pass: 0x%d", p); PRINT(" Fail: 0x%d", f); PRINT("\n\r");
    }
    while(1);

}

#endif
