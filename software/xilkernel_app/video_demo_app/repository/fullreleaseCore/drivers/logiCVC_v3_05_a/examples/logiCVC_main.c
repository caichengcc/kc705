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

/** \addtogroup logiCVCdemo logiCVC driver example
* @{
*/

/**
*
* @file logiCVC_main.c
*
* Prepares environment and runs logiCVC demo.
*
* @note     This file can be copied into SW project and locally changed.
*
*****************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
// #include "platform_config.h"
#include "xil_cache.h"
#ifdef STDOUT_IS_PS7_UART
#include "xuartps.h"
#elif defined(STDOUT_IS_16550)
#include "xuartns550_l.h"
#endif
#ifdef __arm__
#define UART_BAUD 115200
#else
#define UART_BAUD 9600
#endif

#ifdef USE_XYLON_PLATFORM_LIBRARY
#include "plf.h"
#endif
#include "logiCVC_demo.h"


/***************** Macros (Inline Functions) Definitions ********************/

#define PRINT(...)       OsPrintfInfo(__VA_ARGS__)
#define DEBUG_PRINT(...) PRINT(__VA_ARGS__)
#define ERROR_PRINT(...) PRINT(__VA_ARGS__)
#define APP_PRINT(...)   PRINT(__VA_ARGS__)

/************************** Constant Definitions ****************************/
#ifdef USE_XYLON_PLATFORM_LIBRARY
#define PLATFORM_SUPPORTED_DISPLAY_INIT 1
#endif
/************************** Local Variable Definitions ****************************/

static logiCvcPointerT logiCvcInstance[XPAR_LOGICVC_NUM_INSTANCES];


/************************** Function Definitions ****************************/

/**
*
* Function that handles basic initalisation required on some platforms.
*
* @return   0
*
*
*****************************************************************************/
void basePlatformInit(void)
{
    /* cache init */
    Xil_ICacheDisable();
    Xil_DCacheDisable();

#ifdef __PPC__
    Xil_ICacheEnableRegion(CACHEABLE_REGION_MASK);
    Xil_DCacheEnableRegion(CACHEABLE_REGION_MASK);
#elif __MICROBLAZE__
#ifdef XPAR_MICROBLAZE_USE_ICACHE
    Xil_ICacheEnable();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
    Xil_DCacheEnable();
#endif
#endif
    /* uart init */
#ifdef STDOUT_IS_PS7_UART
    /* Use the PS UART for Zynq devices */
    /* Halts on ISE14.1 P.15
    XUartPs Uart_Ps_0;
    XUartPs_Config *Config_0 = XUartPs_LookupConfig(UART_DEVICE_ID);
    XUartPs_CfgInitialize(&Uart_Ps_0, Config_0, Config_0->BaseAddress);
    XUartPs_SetBaudRate(&Uart_Ps_0, UART_BAUD);*/
#elif defined(STDOUT_IS_16550)
    XUartNs550_SetBaud(STDOUT_BASEADDR, XPAR_XUARTNS550_CLOCK_HZ, UART_BAUD);
    XUartNs550_SetLineControlReg(STDOUT_BASEADDR, XUN_LCR_8_DATA_BITS);
#endif

    OsTimeInit(); // init time subsystem (calls swConfig_initTimerSubsystem())
#ifdef USE_XYLON_PLATFORM_LIBRARY
    //plf_init(PLF_SKIP_AUDIO_INIT | PLF_SKIP_VIDEOINPUT_INIT);
    plfSerial_init();
    plfI2c_init();
    plfDisplay_init();
#endif
}


/**
*
* Initializes HW platform specific HW, logiCVC IP core and runs logiCVC demo
*
* @return   0
*
* @note     This file can be copied into local SW project and changed at will.
*
*****************************************************************************/
int main(void)
{
    unsigned int displayTypes[] = {
        LOGICVC_DT_VGA_1024_768
    };
#if PLATFORM_SUPPORTED_DISPLAY_INIT
    logiCVC_plfInitCallbackT plfDisplayCallbacks =
    {
        plfDisplay_setFrequency,      /**< sets external clock required by logiCVC for requsted resolution*/
        plfDisplay_enablePowerSupply, /**< enables and powers up indexed display*/
        plfDisplay_enableSignals,     /**< enables video signals */
        plfDisplay_enableBacklight,   /**< enables backlights for indexed display */
        plfDisplay_setBacklight       /**< sets backlight for indexed display, value: brightness level (0..100)*/
    };
#else /* #if PLATFORM_SUPPORTED_DISPLAY_INIT */
    /* if you use LCD display make sure to initilize this callbacks to some
       valid functions that will set pixel clock and do power sequencing */
    logiCVC_plfInitCallbackT plfDisplayCallbacks =
    {
        NULL, /**< sets external clock required by logiCVC for requsted resolution*/
        NULL, /**< enables and powers up indexed display*/
        NULL, /**< enables video signals */
        NULL, /**< enables backlights for indexed display */
        NULL  /**< sets backlight for indexed display, value: brightness level (0..100)*/
    };
#endif /* #if PLATFORM_SUPPORTED_DISPLAY_INIT */
    logiCVC_plfInitCallbackT *pPlfDisplayCallbacks = &plfDisplayCallbacks;
    logiCvcLayerPointerT pLayer;
    logiCvcClutPointerT  pClut;
    int id, i, layer, layers;

    basePlatformInit();

    APP_PRINT("\n\rlogiCVC initialization:\n\r");
    APP_PRINT("************************************************\n\r\n\r");

    /* Initialize driver and get logiCVC data */
    for(id = 0; id < XPAR_LOGICVC_NUM_INSTANCES; id++)
    {
        OsU32 flags = (skipNone);
        logiCvcInstance[id] =
            logiCVC_init(id, displayTypes[id], flags, pPlfDisplayCallbacks);
        if(logiCvcInstance[id] != NULL)
        {
            DEBUG_PRINT("logiCVC %d initialised successfully!\n\r", id);
        }
        else
        {
            ERROR_PRINT("Error initializing logiCVC %d\n\r", id);
            return 0;
        }

        /* Find if there are any 8bpp layers and initialize default CLUT */
        /* as CLUT is otherwise initialized to zeros                     */
        /* (default CLUT will set simple RRRGGGBB (332) color palette)   */
        layers = logiCVC_GetCommonParam(logiCvcInstance[id], CVC_NUM_LAYERS_E);
        for (layer = 0; layer < layers; layer++)
        {
            pLayer = logiCVC_GetLayerPointer(logiCvcInstance[id], layer);
            OsU32 bitsPerPix = logiCVC_GetLayerParam(pLayer, CVC_LAYER_COLOR_BITS_E);
            OsU32 alphaMode = logiCVC_GetLayerParam(pLayer, CVC_LAYER_ALPHA_MODE_E);
            if (bitsPerPix == 8 &&
                (alphaMode == LOGICVC_ALPHA_MODE_CLUT_INTERNAL_ARGB_8888 ||
                 alphaMode == LOGICVC_ALPHA_MODE_CLUT_INTERNAL_ARGB_6565))
            {
                pClut = logiCVC_GetClutPointer(pLayer, 0);
                for (i = 0; i < 256; i++)
                {
                    OsU32 color = ALPHA_24B | ((i & 0xe0)<<16) | ((i & 0x1c)<<11) | ((i & 3)<<6);
                    logiCVC_ClutWrite(pClut, i, color);
                }
            }
        }
    }

    logiCVCDemo(logiCvcInstance);

    return 0;
}

/* @} */
