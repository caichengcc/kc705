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
* @file test.h
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
#include "xil_io.h"

#define IN8(p)      Xil_In8(p)
#define IN16(p)     Xil_In16(p)
#define IN32(p)     Xil_In32(p)

//#define IN8(p)      ( *((volatile unsigned char *)     (p)) )
//#define IN16(p)     ( *((volatile unsigned short int *)(p)) )
//#define IN32(p)     ( *((volatile unsigned int *)      (p)) )

#define SIM     0    // 1 for simulation, 0 for testing on HW
#define DUMMY   1

#define XPAR_LOGIMEM_0_MB_BASEADDR   XPAR_DDR2_SDRAM_S1_AXI_BASEADDR


#if SIM
    static inline void dummy_print(const char *format, ... )
    { }
    #define  PRINT dummy_print
#else
    #define  PRINT xil_printf
#endif

// logiSDHC Register address offsets
#define SDHC_SDMA_ADDR_ROFF               0x00
#define SDHC_BLOCK_SIZE_ROFF              0x04
#define SDHC_BLOCK_COUNT_ROFF             0x06
#define SDHC_ARGUMENT_ROFF                0x08
#define SDHC_TRANS_MODE_ROFF              0x0C
#define SDHC_COMMAND_ROFF                 0x0E
#define SDHC_RESPONSE_0_ROFF              0x10
#define SDHC_RESPONSE_1_ROFF              0x12
#define SDHC_RESPONSE_2_ROFF              0x14
#define SDHC_RESPONSE_3_ROFF              0x16
#define SDHC_RESPONSE_4_ROFF              0x18
#define SDHC_RESPONSE_5_ROFF              0x1A
#define SDHC_RESPONSE_6_ROFF              0x1C
#define SDHC_RESPONSE_7_ROFF              0x1E
#define SDHC_DATA_BUFF_ROFF               0x20
#define SDHC_PRESENT_STATE_ROFF           0x24
#define SDHC_HOST_CTRL_ROFF               0x28
#define SDHC_POWER_CTRL_ROFF              0x29
#define SDHC_BLOCK_GAP_ROFF               0x2A
#define SDHC_WAKEUP_CTRL_ROFF             0x2B
#define SDHC_CLK_CTRL_ROFF                0x2C
#define SDHC_TIMEOUT_CTRL_ROFF            0x2E
#define SDHC_SW_RESET_ROFF                0x2F
#define SDHC_NORM_INTR_STAT_ROFF          0x30
#define SDHC_ERR_INTR_STAT_ROFF           0x32
#define SDHC_NORM_INTR_STAT_EN_ROFF       0x34
#define SDHC_ERR_INTR_STAT_EN_ROFF        0x36
#define SDHC_NORM_INTR_SIG_EN_ROFF        0x38
#define SDHC_ERR_INTR_SIG_EN_ROFF         0x3A
#define SDHC_A_CMD12_ERR_STAT_ROFF        0x3C
#define SDHC_CAPABILITIES_ROFF            0x40
#define SDHC_MAX_CURR_CAPAB_ROFF          0x48
#define SDHC_FORCE_A_CMD12_ERR_STAT_ROFF  0x50
#define SDHC_FORCE_ERR_INTR_STAT_ROFF     0x52
#define SDHC_ADMA_ERR_STAT_ROFF           0x54
#define SDHC_ADMA_SYS_ADDR_0_ROFF         0x58
#define SDHC_ADMA_SYS_ADDR_1_ROFF         0x5A
#define SDHC_ADMA_SYS_ADDR_2_ROFF         0x5C
#define SDHC_ADMA_SYS_ADDR_3_ROFF         0x5E
#define SDHC_SLOT_INTR_STAT_ROFF          0xFC
#define SDHC_HOST_CTRL_VER_ROFF           0xFE

#define SDHCI_DATA_BUSY       0x100000
#define SDHCI_CMD_INHIBIT     0x00000001
#define SDHCI_DATA_INHIBIT    0x00000002
#define SDHCI_BUSY            0x00100000


//////////////////////////////////
extern short Init_SD_Card(void);
extern void send_csd(int address);
extern void select_sd_card(int address);
extern void select_sd_card(int address);
extern void send_status(int address);
extern void set_sd_bus_width(int address, char width);
extern void high_speed_mode_en(void);
extern void write_single(int address, int block_size);
extern void write_read_single(int address, int block_size);
extern void write_read_multiple_block(int sd_mem_address, int block_count, int block_size);



