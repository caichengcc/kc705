/*****************************************************************************
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2011 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
*****************************************************************************/

/****************************************************************************/
/**
*
* @file logiCVC_disp_i.c
*
* This file contains internal logiCVC initialization data for different display types.
*
*****************************************************************************/

/***************************** Include Files *********************************/

#include "logiCVC.h"
#include "logiCVC_disp_i.h"

/************************** Function Definitions *****************************/


/* Display structure data */

#if SUPPORT_DT_SIMULATION
static logiCvcDisplayT const display_simulation = {
                                                16,         // SHSY_FP
                                                96,         // SHSY
                                                16,         // SHSY_BP
                                                640,        // SHSY_RES
                                                1,          // SVSY_FP
                                                1,          // SVSY
                                                1,          // SVSY_BP
                                                4,          // SVSY_RES
                                                0x011f,     // SCTRL
                                                0x0040,     // SDTYPE
                                                323,        // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_512_512) /* Added only for logiCVC testing */
static logiCvcDisplayT const display_512_512 = {
                                                16,         // SHSY_FP
                                                48,         // SHSY
                                                64,         // SHSY_BP
                                                512,        // SHSY_RES
                                                3,          // SVSY_FP
                                                11,         // SVSY
                                                8,          // SVSY_BP
                                                512,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                20467,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_1024_1024) /* Added only for logiCVC testing */
static logiCvcDisplayT const display_1024_1024 = {
                                                64,         // SHSY_FP
                                                104,        // SHSY
                                                168,        // SHSY_BP
                                                1024,       // SHSY_RES
                                                3,          // SVSY_FP
                                                10,         // SVSY
                                                26,         // SVSY_BP
                                                1024,       // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                86741,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_2048_2048) /* Added only for logiCVC testing */
static logiCvcDisplayT const display_2048_2048 = {
                                                160,        // SHSY_FP
                                                224,        // SHSY
                                                384,        // SHSY_BP
                                                2048,       // SHSY_RES
                                                3,          // SVSY_FP
                                                10,         // SVSY
                                                60,         // SVSY_BP
                                                2048,       // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                358364,     // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_512_384) /* Added only for logiCVC testing */
static logiCvcDisplayT const display_512_384 = {
                                                16,         // SHSY_FP
                                                48,         // SHSY
                                                64,         // SHSY_BP
                                                512,        // SHSY_RES
                                                3,          // SVSY_FP
                                                4,          // SVSY
                                                10,         // SVSY_BP
                                                384,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                15398,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_2048_1536) /* Added only for logiCVC testing ; QXGA standard 2048*1536  */
static logiCvcDisplayT const display_2048_1536 = {
                                                48,         // SHSY_FP
                                                32,         // SHSY
                                                80,         // SHSY_BP
                                                2048,       // SHSY_RES
                                                3,          // SVSY_FP
                                                4,          // SVSY
                                                37,         // SVSY_BP
                                                1536,       // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                209318,     // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_2048_512) /* Added only for logiCVC testing */
static logiCvcDisplayT const display_2048_512 = {
                                                48,         // SHSY_FP
                                                32,         // SHSY
                                                80,         // SHSY_BP
                                                2048,       // SHSY_RES
                                                3,          // SVSY_FP
                                                11,         // SVSY
                                                8,          // SVSY_BP
                                                512,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                209318,     // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_VGA_640_480) /* VESA standard 640x480 60Hz (VGA) */
static logiCvcDisplayT const display_vga_640_480 = {
                                                16,         // SHSY_FP
                                                96,         // SHSY
                                                48,         // SHSY_BP
                                                640,        // SHSY_RES
                                                10,         // SVSY_FP
                                                2,          // SVSY
                                                33,         // SVSY_BP
                                                480,        // SVSY_RES
                                                0x001f,     // SCTRL HSY and VSY enabled and inverted and BLANK enabled
                                                0x0,        // SDTYPE
                                                25175,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_VGA_800_480) /* Xenarc 800x480 60Hz VGA monitor (WVGA)*/
static logiCvcDisplayT const display_vga_800_480 = {
                                                64,         // SHSY_FP
                                                80,         // SHSY
                                                56,         // SHSY_BP
                                                800,        // SHSY_RES
                                                28,         // SVSY_FP
                                                3,          // SVSY
                                                14,         // SVSY_BP
                                                480,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                31500,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_VGA_800_600) /* VESA standard 800x600 60Hz (SVGA) */
static logiCvcDisplayT const display_vga_800_600 = {
                                                40,         // SHSY_FP
                                                128,        // SHSY
                                                88,         // SHSY_BP
                                                800,        // SHSY_RES
                                                1,          // SVSY_FP
                                                4,          // SVSY
                                                23,         // SVSY_BP
                                                600,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                40000,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_VGA_1024_768) /* VESA standard 1024x768 60Hz (XGA) */
static logiCvcDisplayT const display_vga_1024_768 = {
                                                24,         // SHSY_FP
                                                136,        // SHSY
                                                160,        // SHSY_BP
                                                1024,       // SHSY_RES
                                                3,          // SVSY_FP
                                                6,          // SVSY
                                                29,         // SVSY_BP
                                                768,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                65000,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_VGA_RBT_1024_768) /* VESA standard 1024x768 60Hz, Reduced Blanking Timings, 56MHz clock, +Hsync, -Vsync */
static logiCvcDisplayT const display_vga_rbt_1024_768 = {
                                                48,         // SHSY_FP
                                                32,         // SHSY
                                                80,         // SHSY_BP
                                                1024,       // SHSY_RES
                                                3,          // SVSY_FP
                                                4,          // SVSY
                                                15,         // SVSY_BP
                                                768,        // SVSY_RES
                                                0x001d,     // SCTRL   +Hsync -Vsync
                                                0x0,        // SDTYPE
                                                56000,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_VGA_1280_1024) /* VESA standard 1280x1024 60Hz (SXGA) */
static logiCvcDisplayT const display_vga_1280_1024 = {
                                                48,         // SHSY_FP
                                                112,        // SHSY
                                                248,        // SHSY_BP
                                                1280,       // SHSY_RES
                                                1,          // SVSY_FP
                                                3,          // SVSY
                                                38,         // SVSY_BP
                                                1024,       // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                108000,     // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_VGA_1680_1050 /* VESA standard 1680x1050 60Hz (WSXGA+) REDUCED BLANKING */
static logiCvcDisplayT const display_vga_1680_1050 = {
                                                48,         // SHSY_FP
                                                32,         // SHSY
                                                80,         // SHSY_BP
                                                1680,       // SHSY_RES
                                                3,          // SVSY_FP
                                                6,          // SVSY
                                                21,         // SVSY_BP
                                                1050,       // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                119000,     // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_VGA_1920_1080 /* CEA standard 1920x1080p 60Hz (Full HD) */
static logiCvcDisplayT const display_vga_1920_1080 = {
                                                88,         // SHSY_FP
                                                44,         // SHSY
                                                148,        // SHSY_BP
                                                1920,       // SHSY_RES
                                                4,          // SVSY_FP
                                                5,          // SVSY
                                                36,         // SVSY_BP
                                                1080,       // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                148500,     // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                          };
#endif

#if SUPPORT_DT_VGA_1920_1200 /* VESA standard 1920x1200 60Hz (WUXGA) REDUCED BLANKING */
static logiCvcDisplayT const display_vga_1920_1200 = {
                                                48,         // SHSY_FP
                                                32,         // SHSY
                                                80,         // SHSY_BP
                                                1920,       // SHSY_RES
                                                3,          // SVSY_FP
                                                6,          // SVSY
                                                26,         // SVSY_BP
                                                1200,       // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                154000,     // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                          };
#endif

#if SUPPORT_DT_PAL /* PAL standard 720*576 */
static logiCvcDisplayT const display_PAL = {
                                                1,          // SHSY_FP
                                                144,        // SHSY
                                                1,          // SHSY_BP
                                                720,        // SHSY_RES
                                                1,          // SVSY_FP
                                                26,         // SVSY
                                                1,          // SVSY_BP
                                                288,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                27000,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_NTSC /* NTSC standard 720*488 */
static logiCvcDisplayT const display_NTSC = {
                                                1,          // SHSY_FP
                                                138,        // SHSY
                                                1,          // SHSY_BP
                                                720,        // SHSY_RES
                                                1,          // SVSY_FP
                                                19,         // SVSY
                                                1,          // SVSY_BP
                                                244,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0040,     // SDTYPE
                                                27000,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_1280_480
static logiCvcDisplayT const display_1280_480 = {
                                                47,         // SHSY_FP
                                                112,        // SHSY
                                                249,        // SHSY_BP
                                                1280,       // SHSY_RES
                                                3,          // SVSY_FP
                                                2,          // SVSY
                                                40,         // SVSY_BP
                                                480,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                53172,      // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_TD070WGCB2 /* TOPPOLY WVGA 7" TFT LCD  854x480  */
static logiCvcDisplayT const display_TD070WGCB2 = {
                                                40,         // SHSY_FP
                                                115,        // SHSY
                                                45,         // SHSY_BP
                                                856,        // SHSY_RES (should be (854 - 1) but logiCVC requires horizontal resolution as a multiple of 4)
                                                23,         // SVSY_FP
                                                4,          // SVSY
                                                18,         // SVSY_BP
                                                480,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                33074,      // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_LQ065T9DR51U /* Modified LQ065T9DR51U 6.5" TFT LCD 400x240 display settings for RSE demo  */
static logiCvcDisplayT const display_LQ065T9DR51U = {
                                                256,        // SHSY_FP
                                                12,         // SHSY
                                                4,          // SHSY_BP
                                                1152,       // SHSY_RES
                                                20,         // SVSY_FP
                                                4,          // SVSY
                                                18,         // SVSY_BP
                                                240,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                23752,      // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_LTL652T
static logiCvcDisplayT const display_LTL652T = {
                                                16,         // SHSY_FP
                                                15,         // SHSY
                                                49,         // SHSY_BP
                                                400,        // SHSY_RES
                                                14,         // SVSY_FP
                                                3,          // SVSY
                                                15,         // SVSY_BP
                                                240,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x04a3,     // SDTYPE
                                                7834,       // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_LTA035B
static logiCvcDisplayT const display_LTA035B = {
                                                18,         // SHSY_FP
                                                51,         // SHSY
                                                35,         // SHSY_BP
                                                320,        // SHSY_RES
                                                4,          // SVSY_FP
                                                2,          // SVSY
                                                16,         // SVSY_BP
                                                240,        // SVSY_RES
                                                0x011f,     // SCTRL
                                                0x0,        // SDTYPE
                                                6640,       // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_LTA149B780F /* TOSHIBA 14.9" LCD 1280x390 */
static logiCvcDisplayT const display_LTA149B780F = {
                                                32,         // SHSY_FP
                                                32,         // SHSY
                                                64,         // SHSY_BP
                                                1280,       // SHSY_RES
                                                1,          // SVSY_FP
                                                3,          // SVSY
                                                24,         // SVSY_BP
                                                390,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                35313,      // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_TD080WGCA1 /* TOPPOLY TFT 8" LCD 800x480 */
static logiCvcDisplayT const display_TD080WGCA1 = {
                                                64,         // SHSY_FP
                                                128,        // SHSY
                                                64,         // SHSY_BP
                                                800,        // SHSY_RES
                                                33,         // SVSY_FP
                                                4,          // SVSY
                                                8,          // SVSY_BP
                                                480,        // SVSY_RES
                                                0x011f,     // SCTRL
                                                0x0,        // SDTYPE
                                                33264,      // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_LQ070Y5DE01 /* 7" LCD 400x480 */
static logiCvcDisplayT const display_LQ070Y5DE01 = {
                                                44,         // SHSY_FP
                                                101,        // SHSY
                                                1,          // SHSY_BP
                                                400,        // SHSY_RES
                                                27,         // SVSY_FP
                                                2,          // SVSY
                                                6,          // SVSY_BP
                                                480,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0040,     // SDTYPE
                                                16871,      // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_LLL652T)
static logiCvcDisplayT const display_LLL652T = {
                                                32,         // SHSY_FP
                                                50,         // SHSY
                                                46,         // SHSY_BP
                                                400,        // SHSY_RES
                                                3,          // SVSY_FP
                                                9,          // SVSY
                                                10,         // SVSY_BP
                                                240,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                8300,       // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_UMSH_8013MD /* Extended 7" WVGA TFT 800x480 */
static logiCvcDisplayT const display_UMSH_8013MD = {
                                                25,         // SHSY_FP
                                                50,         // SHSY
                                                25,         // SHSY_BP
                                                800,        // SHSY_RES
                                                5,          // SVSY_FP
                                                10,         // SVSY
                                                5,          // SVSY_BP
                                                480,        // SVSY_RES
                                                0x011f,     // SCTRL
                                                0x0,        // SDTYPE
                                                27000,      // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_MTF_TW70SP
static logiCvcDisplayT const display_MTF_TW70SP = {
                                                64,         // SHSY_FP
                                                128,        // SHSY
                                                64,         // SHSY_BP
                                                800,        // SHSY_RES
                                                5,          // SVSY_FP
                                                10,         // SVSY
                                                5,          // SVSY_BP
                                                480,        // SVSY_RES
                                                0x011f,     // SCTRL
                                                0x0,        // SDTYPE
                                                27000,      // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_XTRONIC_800_480
static logiCvcDisplayT const display_XTRONIC_800_480 = {
                                                64,         // SHSY_FP
                                                128,        // SHSY
                                                64,         // SHSY_BP
                                                800,        // SHSY_RES
                                                113,        // SVSY_FP
                                                16,         // SVSY
                                                16,         // SVSY_BP
                                                480,        // SVSY_RES
                                                0x011f,     // SCTRL
                                                0x0,        // SDTYPE
                                                39600,      // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if SUPPORT_DT_XTRONIC_400_240
static logiCvcDisplayT const display_XTRONIC_400_240 = {
                                                32,         // SHSY_FP
                                                64,         // SHSY
                                                32,         // SHSY_BP
                                                400,        // SHSY_RES
                                                5,          // SVSY_FP
                                                5,          // SVSY
                                                13,         // SVSY_BP
                                                240,        // SVSY_RES
                                                0x011f,     // SCTRL
                                                0x0,        // SDTYPE
                                                8332,       // Pixel clock in KHz for 60Hz refresh rate
                                                50,         // Power enable delay in ms
                                                30          // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_DLF1095)
static logiCvcDisplayT const display_DLF1095 = {
                                                24,         // SHSY_FP
                                                122,        // SHSY
                                                160,        // SHSY_BP
                                                1024,       // SHSY_RES
                                                3,          // SVSY_FP
                                                6,          // SVSY
                                                29,         // SVSY_BP
                                                768,        // SVSY_RES
                                                0x01f,      // SCTRL
                                                0x0,        // SDTYPE
                                                64318,      // Pixel clock in KHz for 60Hz refresh rate
                                                200,        // Power enable delay in ms
                                                20          // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_LAJ123T001_JLR)
static logiCvcDisplayT const display_LAJ123T001_JLR = {
                                                64,         // SHSY_FP
                                                44,         // SHSY
                                                64,         // SHSY_BP
                                                1280,       // SHSY_RES
                                                2,          // SVSY_FP
                                                1,          // SVSY
                                                4,          // SVSY_BP
                                                480,        // SVSY_RES
                                                0x01f,      // SCTRL
                                                0x0,        // SDTYPE
                                                42427,      // Pixel clock in KHz for 60Hz refresh rate
                                                100,        // Power enable delay in ms
                                                20          // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_TCG062HVLDA_G20)
static logiCvcDisplayT const display_TCG062HVLDA_G20 = {
                                                65,         // SHSY_FP
                                                30,         // SHSY
                                                65,         // SHSY_BP
                                                640,        // SHSY_RES
                                                251,        // SVSY_FP
                                                17,         // SVSY
                                                17,         // SVSY_BP
                                                240,        // SVSY_RES
                                                0x01f,      // SCTRL
                                                0x0,        // SDTYPE
                                                25200,      // Pixel clock in KHz for 60Hz refresh rate
                                                150,        // Power enable delay in ms
                                                200         // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_T_55382GD050JU_LW_A_ABN)
static logiCvcDisplayT const display_T_55382GD050JU_LW_A_ABN = {
                                                167,        // SHSY_FP
                                                16,         // SHSY
                                                73,         // SHSY_BP
                                                800,        // SHSY_RES
                                                12,         // SVSY_FP
                                                2,          // SVSY
                                                31,         // SVSY_BP
                                                480,        // SVSY_RES
                                                0x01f,      // SCTRL
                                                0x0,        // SDTYPE
                                                33264,      // Pixel clock in KHz for 60Hz refresh rate
                                                150,        // Power enable delay in ms
                                                200         // Signal enable delay in ms
                                            };
#endif

#if (SUPPORT_DT_G065VN01V2)
static logiCvcDisplayT const display_G065VN01V2 = {
                                                65,         // SHSY_FP
                                                30,         // SHSY
                                                65,         // SHSY_BP
                                                640,        // SHSY_RES
                                                15,         // SVSY_FP
                                                15,         // SVSY
                                                15,         // SVSY_BP
                                                480,        // SVSY_RES
                                                0x015,      // SCTRL
                                                0x0,        // SDTYPE
                                                25200,      // Pixel clock in KHz for 60Hz refresh rate
                                                150,        // Power enable delay in ms
                                                200         // Signal enable delay in ms
                                            };
#endif

#if defined(SUPPORT_DT_TM050RBH01) /* Shanghai Tianma TM050RBH01 Ver 1.7 5" 800x480 RGB LCD w/ 4-wire touch panel (29 inputs)*/
static logiCvcDisplayT display_TM050RBH01  = {
                                                40,         // SHSY_FP
                                                48,         // SHSY
                                                40,         // SHSY_BP
                                                800,        // SHSY_RES
                                                13,         // SVSY_FP
                                                3,          // SVSY
                                                29,         // SVSY_BP
                                                480,        // SVSY_RES
                                                0x001f,     // SCTRL
                                                0x0,        // SDTYPE
                                                30000,      // Pixel clock in KHz for 60Hz refresh rate
                                                200,        // Power enable delay in ms
                                                50          // Signal enable delay in ms
                                         };
#endif

#if (SUPPORT_DT_1920_1080)
static logiCvcDisplayT const display_1920_1080 = {
                                                88 - 1,     // SHSY_FP
                                                44 - 1,     // SHSY
                                                148 - 1,    // SHSY_BP
                                                1920,       // SHSY_RES
                                                4 - 1,      // SVSY_FP
                                                5 - 1,      // SVSY
                                                36 - 1,     // SVSY_BP
                                                1080,       // SVSY_RES
                                                0x0015,     // SCTRL
                                                0x0,        // SDTYPE
                                                148500,     // Pixel clock in KHz for 60Hz refresh rate
                                                0,          // Power enable delay in ms
                                                0           // Signal enable delay in ms
                                            };
#endif




/************************** Function definitions *****************************/

/**
*
* Initialize a logiCVC_dispInit structure with init data according to display type.
*
* @param    displayType  is the unique id of the device.
*
* @return
* - on success pointer to display data if display is supported.
* - on failure NULL
*
* @note
*
* None.
*
*****************************************************************************/
logiCvcDisplayT const *logiCVC_GetDispInitData(logiCvcDisplayTypeE displayType)
{
    logiCvcDisplayT const *pDisplayData;

    switch(displayType)
    {
#if SUPPORT_DT_SIMULATION
        case LOGICVC_DT_SIMULATION:
            pDisplayData = &display_simulation;
            break;
#endif
#if SUPPORT_DT_512_512
        case LOGICVC_DT_512_512:
            pDisplayData = &display_512_512;
            break;
#endif
#if SUPPORT_DT_1024_1024
        case LOGICVC_DT_1024_1024:
            pDisplayData = &display_1024_1024;
            break;
#endif
#if SUPPORT_DT_2048_2048
        case LOGICVC_DT_2048_2048:
            pDisplayData = &display_2048_2048;
            break;
#endif
#if SUPPORT_DT_512_384
        case LOGICVC_DT_512_384:
            pDisplayData = &display_512_384;
            break;
#endif
#if SUPPORT_DT_2048_1536
        case LOGICVC_DT_2048_1536:
            pDisplayData = &display_2048_1536;
            break;
#endif
#if SUPPORT_DT_2048_512
        case LOGICVC_DT_2048_512:
            pDisplayData = &display_2048_512;
            break;
#endif
#if SUPPORT_DT_VGA_640_480
        case LOGICVC_DT_VGA_640_480:
            pDisplayData = &display_vga_640_480;
            break;
#endif
#if SUPPORT_DT_VGA_800_480
        case LOGICVC_DT_VGA_800_480:
            pDisplayData = &display_vga_800_480;
            break;
#endif
#if SUPPORT_DT_VGA_800_600
        case LOGICVC_DT_VGA_800_600:
            pDisplayData = &display_vga_800_600;
            break;
#endif
#if SUPPORT_DT_VGA_1024_768
        case LOGICVC_DT_VGA_1024_768:
            pDisplayData = &display_vga_1024_768;
            break;
#endif
#if SUPPORT_DT_VGA_RBT_1024_768
        case LOGICVC_DT_VGA_RBT_1024_768:
            pDisplayData = &display_vga_rbt_1024_768;
            break;
#endif
#if SUPPORT_DT_VGA_1280_1024
        case LOGICVC_DT_VGA_1280_1024:
            pDisplayData = &display_vga_1280_1024;
            break;
#endif
#if SUPPORT_DT_VGA_1680_1050
        case LOGICVC_DT_VGA_1680_1050:
            pDisplayData = &display_vga_1680_1050;
            break;
#endif
#if SUPPORT_DT_VGA_1920_1080
        case LOGICVC_DT_VGA_1920_1080:
            pDisplayData = &display_vga_1920_1080;
            break;
#endif
#if SUPPORT_DT_VGA_1920_1200
        case LOGICVC_DT_VGA_1920_1200:
            pDisplayData = &display_vga_1920_1200;
            break;
#endif
#if SUPPORT_DT_PAL
        case LOGICVC_DT_PAL:
            pDisplayData = &display_PAL;
            break;
#endif
#if SUPPORT_DT_NTSC
        case LOGICVC_DT_NTSC:
            pDisplayData = &display_NTSC;
            break;
#endif
#if SUPPORT_DT_1280_480
        case LOGICVC_DT_1280_480:
            pDisplayData = &display_1280_480;
            break;
#endif
#if SUPPORT_DT_TD070WGCB2
        case LOGICVC_DT_TD070WGCB2:
            pDisplayData = &display_TD070WGCB2;
            break;
#endif
#if SUPPORT_DT_LQ065T9DR51U
        case LOGICVC_DT_LQ065T9DR51U:
            pDisplayData = &display_LQ065T9DR51U;
            break;
#endif
#if SUPPORT_DT_LTL652T
        case LOGICVC_DT_LTL652T:
            pDisplayData = &display_LTL652T;
            break;
#endif
#if SUPPORT_DT_LTA035B
        case LOGICVC_DT_LTA035B:
            pDisplayData = &display_LTA035B;
            break;
#endif
#if SUPPORT_DT_LTA149B780F
        case LOGICVC_DT_LTA149B780F:
            pDisplayData = &display_LTA149B780F;
            break;
#endif
#if SUPPORT_DT_TD080WGCA1
        case LOGICVC_DT_TD080WGCA1:
            pDisplayData = &display_TD080WGCA1;
            break;
#endif
#if SUPPORT_DT_LQ070Y5DE01
        case LOGICVC_DT_LQ070Y5DE01:
            pDisplayData = &display_LQ070Y5DE01;
            break;
#endif
#if SUPPORT_DT_LLL652T
        case LOGICVC_DT_LLL652T:
            pDisplayData = &display_LLL652T;
            break;
#endif
#if SUPPORT_DT_UMSH_8013MD
        case LOGICVC_DT_UMSH_8013MD:
            pDisplayData = &display_UMSH_8013MD;
            break;
#endif
#if SUPPORT_DT_MTF_TW70SP
        case LOGICVC_DT_MTF_TW70SP:
            pDisplayData = &display_MTF_TW70SP;
            break;
#endif
#if SUPPORT_DT_XTRONIC_800_480
        case LOGICVC_DT_XTRONIC_800_480:
            pDisplayData = &display_XTRONIC_800_480;
            break;
#endif
#if SUPPORT_DT_XTRONIC_400_240
        case LOGICVC_DT_XTRONIC_400_240:
            pDisplayData = &display_XTRONIC_400_240;
            break;
#endif
#if (SUPPORT_DT_DLF1095)
        case LOGICVC_DT_DLF1095:
            pDisplayData = &display_DLF1095;
            break;
#endif
#if (SUPPORT_DT_LAJ123T001_JLR)
        case LOGICVC_DT_LAJ123T001_JLR:
            pDisplayData = &display_LAJ123T001_JLR;
            break;
#endif
#if (SUPPORT_DT_TCG062HVLDA_G20)
        case LOGICVC_DT_TCG062HVLDA_G20:
            pDisplayData = &display_TCG062HVLDA_G20;
            break;
#endif
#if (SUPPORT_DT_T_55382GD050JU_LW_A_ABN)
        case LOGICVC_DT_T_55382GD050JU_LW_A_ABN:
            pDisplayData = &display_T_55382GD050JU_LW_A_ABN;
            break;
#endif
#if (SUPPORT_DT_G065VN01V2)
        case LOGICVC_DT_G065VN01V2:
            pDisplayData = &display_G065VN01V2;
            break;
#endif
#if SUPPORT_DT_TM050RBH01
      case LOGICVC_DT_TM050RBH01:
         pDisplayData = &display_TM050RBH01;
      break;
#endif
#if (SUPPORT_DT_1920_1080)
        case LOGICVC_DT_1920_1080:
            pDisplayData = &display_1920_1080;
            break;
#endif

        default:
            pDisplayData = NULL;
    }

    return pDisplayData;
}
