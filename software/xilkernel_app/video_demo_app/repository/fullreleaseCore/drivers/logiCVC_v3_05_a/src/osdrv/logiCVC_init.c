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

/** \addtogroup logiCVC_XPS_osdrv logiCVC high-level XPS driver
* @{
*/

/**
*
* @file logiCVC_init.c
*
* @brief This file contains the functions for logiCVC initialization.
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xparameters.h"
#ifdef XPAR_LOGICVC_NUM_INSTANCES

#include <stdio.h>
#include "logiCVC_init.h"
#include "logiCVC_g.h"


/************************** Constant Definitions ****************************/

#define TRANSP_COLOR_8BPP_CLUT_16 0xf813
#define TRANSP_COLOR_8BPP_CLUT_24 0x00ff009C
#define TRANSP_COLOR_16BPP        0xf813
#define TRANSP_COLOR_24BPP        0x00ff009C
#define BACKGROUND_COLOR_24BPP    0x00000000


/***************** Macros (Inline Functions) Definitions ********************/
#ifdef SIM // defined when compiling for simulations
#define PRINT(...)
#else
#define PRINT(...)       OsPrintfInfo(__VA_ARGS__)
#endif
#if (defined(GENDRV_CVC_DEBUG) || defined(GENDRV_GLOBAL_DEBUG))
#define DEBUG_PRINT(format, ...)   PRINT(__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...)
#endif
#define ERROR_PRINT(...) PRINT(__VA_ARGS__)
#define APP_PRINT(...)   PRINT(__VA_ARGS__)


/************************** Function Definitions *************************/

/**
*
* Initializes a logiCVC instance. Uses a display type ID.
*
* @param    instanceId      is the unique id of the logCVC device.
*                           Values from 0 to (XPAR_LOGICVC_NUM_INSTANCES-1)
* @param    displayType     is number defining logiCVC display type (e.g. LOGICVC_DT_VGA_640_480)
* @param    flags           are flags of logiCVC_init_flagsE type, and define different options
* @param    plfCallback     is pointer to structure containing callbacks for clock, display and
*                           backlight initialization. Pointer in this structure must be initialized to valid
*                           platform functions or set to NULL in which case they wont be called. plfCallback
*                           can also be set to NULL in which case 0 callback will be called.
*
* @return   logiCVC instance pointer if everything starts up as expected \n
*           NULL if the init was unsuccessful
*
* @note
*  - plfCallback structure must be initialized with platform function for clock, display and
*        backlight initialization when logiCVC is not controlling the display
*  - logiCVC background and layer transparent colors will be set to default values defined in this file
*
*****************************************************************************/
logiCvcPointerT logiCVC_init(OsSizeT instanceId,
                             OsSizeT displayType,
                             logiCVC_init_flagsE flags,
                             logiCVC_plfInitCallbackT *plfCallback)
{
    /* Get display data from logiCVC supported display types database */
    return logiCVC_initDd(instanceId,
                          logiCVC_GetDispInitData(displayType),
                          flags,
                          plfCallback);
}


/**
*
* Initializes a logiCVC instance. Uses a display data structure.
*
* @param    instanceId      is the unique id of the logCVC device.
*                           Values from 0 to (XPAR_LOGICVC_NUM_INSTANCES-1)
* @param    pCvcDisplayData points to a logiCVC display data structure, i.e. defines a display
* @param    flags           are flags of logiCVC_init_flagsE type, and define different options
* @param    plfCallback     is pointer to structure containing callbacks for clock, display and
*                           backlight initialization. Pointer in this structure must be initialized to valid
*                           platform functions or set to NULL in which case they wont be called. plfCallback
*                           can also be set to NULL in which case 0 callback will be called.
*
* @return   logiCVC instance pointer if everything starts up as expected \n
*           NULL if the init was unsuccessful
*
* @note
*  - plfCallback structure must be initialized with platform function for clock, display and
*        backlight initialization when logiCVC is not controlling the display
*  - logiCVC background and layer transparent colors will be set to default values defined in this file
*
*****************************************************************************/
logiCvcPointerT logiCVC_initDd(OsSizeT instanceId,
                               logiCvcDisplayT const *pCvcDisplayData,
                               logiCVC_init_flagsE flags,
                               logiCVC_plfInitCallbackT *plfCallback)
{
    OsHwResourceT osHwCvcRegResource;
    OsHwResourceT osHwVideoResource;
    logiCvcConfigT const * logiCvcConfigPtr;
    logiCvcPointerT logiCvcPtr;
    logiCvcLayerPointerT logiCvcLayerPtr;
    logiCvcBufferPointerT logiCvcBufferPtr;
    OsU32 layer, layers, bitsPerPix, alphaMode;
    XRECT clearRect;
    cvcStartFlagsE cvcStartFlags;

    /*
    * Lookup the device configuration in the configuration table. Use this
    * configuration info when initializing this component.
    */
    logiCvcConfigPtr = logiCvcLookupConfig(instanceId);
    if(logiCvcConfigPtr == NULL)
    {
        ERROR_PRINT("logiCVC_init() - Device %d not found!\n\r", instanceId);
        return NULL;
    }

    /* Initialize HW access object and call logiCVC initialize */
#if (OS_LIBRARY_VER < 0x00010600) /* support for older oslib */
    osHwCvcRegResource.physBaseAddress = logiCvcConfigPtr->RegBaseAddr;
    osHwCvcRegResource.size = logiCvcConfigPtr->RegHighAddr - logiCvcConfigPtr->RegBaseAddr + 1;
    osHwVideoResource.physBaseAddress = logiCvcConfigPtr->MemoryLowAddr;
    osHwVideoResource.size = logiCvcConfigPtr->MemoryHighAddr - logiCvcConfigPtr->MemoryLowAddr + 1;
#else
    osHwCvcRegResource.busType = OS_BUS_TYPE__CPU;
    osHwCvcRegResource.bus.cpu.physBaseAddress = logiCvcConfigPtr->RegBaseAddr;
    osHwCvcRegResource.bus.cpu.size = logiCvcConfigPtr->RegHighAddr - logiCvcConfigPtr->RegBaseAddr + 1;
    osHwVideoResource.busType = OS_BUS_TYPE__CPU;
    osHwVideoResource.bus.cpu.physBaseAddress = logiCvcConfigPtr->MemoryLowAddr;
    osHwVideoResource.bus.cpu.size = logiCvcConfigPtr->MemoryHighAddr - logiCvcConfigPtr->MemoryLowAddr + 1;
#endif

    logiCvcPtr = logiCVC_Initialize(logiCvcConfigPtr, &osHwCvcRegResource, &osHwVideoResource);
    if(logiCvcPtr != NULL)
    {
        if(!(flags & skipAll))
        {
            if((pCvcDisplayData != NULL) && ((pCvcDisplayData->HorizontalRes) <= logiCvcConfigPtr->RowStride))
            {
                /* Set PLL frequency if needed */
                if(!(flags & skipDisplayControl))
                {
                    if((plfCallback != NULL) && (plfCallback->plfDisplay_setFrequency != NULL))
                    {
                        plfCallback->plfDisplay_setFrequency(instanceId, pCvcDisplayData->PixelClock/1000, (OsObjectHandleT)logiCvcPtr);
                    }
                }

                /* Initialize displays and start logiCVC */
                if(flags & skipCvcInitRegisters)
                {
                    cvcStartFlags = CVC_START_SKIP_REGS_WRITE_E;
                }
                else
                {
                    cvcStartFlags = CVC_START_NORMAL_E;
                }
                logiCVC_Start(logiCvcPtr, pCvcDisplayData, cvcStartFlags);

                /* Apply display supply voltage if needed */
                if((plfCallback != NULL) && (plfCallback->plfDisplay_enablePowerSupply != NULL) && (!(flags & skipDisplayControl)))
                {
                    plfCallback->plfDisplay_enablePowerSupply(instanceId, (OsObjectHandleT)logiCvcPtr);
                }
                if(!(flags & skipCvcPowerControl))
                {
                    logiCVC_PowerControl(logiCvcPtr, CVC_PWR_VDD, 1);
                    OsMsDelay(pCvcDisplayData->PowerOnDelay);
                }
                if((plfCallback != NULL) && (plfCallback->plfDisplay_enableSignals != NULL) && (!(flags & skipDisplayControl)))
                {
                    plfCallback->plfDisplay_enableSignals(instanceId, (OsObjectHandleT)logiCvcPtr);
                }
                if(!(flags & skipCvcPowerControl))
                {
                    logiCVC_PowerControl(logiCvcPtr, CVC_PWR_SIGNALS, 1);
                    OsMsDelay(pCvcDisplayData->SignalOnDelay);
                }

                /* Get logiCVC number of layers */
                layers = logiCVC_GetCommonParam(logiCvcPtr, CVC_NUM_LAYERS_E);

                /* Clear video memory if needed */
                if(!(flags & skipClearVideoMemory))
                {
                    for(layer = 0; layer < layers; layer++)
                    {
                        int numBuffers, buff;
                        OsU32 blackColor;
                        logiCvcLayerPtr = logiCVC_GetLayerPointer(logiCvcPtr, layer);
                        numBuffers = logiCVC_GetLayerParam(logiCvcLayerPtr, CVC_LAYER_NUM_BUFFERS_USABLE_E);
                        blackColor = logiCVC_ConvertArgb32ToLayerColorFormat(logiCvcLayerPtr, 0);
                        clearRect.x_pos = 0;
                        clearRect.y_pos = 0;
                        clearRect.width = logiCVC_GetCommonParam(logiCvcPtr, CVC_HRES_E);
                        clearRect.height = logiCVC_GetCommonParam(logiCvcPtr, CVC_VRES_E);
                        if(!(flags & clearAllLayerBuffers)) /* clear only first buffer */
                        {
                            numBuffers = numBuffers>=1 ? 1:0;
                        }
                        for(buff = 0; buff < numBuffers; ++buff)
                        {
                            logiCvcBufferPtr = logiCVC_GetBufferPointer(logiCvcLayerPtr, buff);
                            logiCVC_FillRectangle(logiCvcBufferPtr, &clearRect, blackColor);
                        }
                        bitsPerPix = logiCVC_GetLayerParam(logiCvcLayerPtr, CVC_LAYER_COLOR_BITS_E);
                        if(bitsPerPix == 8)
                        {
                            int i;
                            logiCvcClutPointerT logiCvcClutPtr = logiCVC_GetClutPointer(logiCvcLayerPtr, 0);
                            for(i = 0; i < 256; ++i)
                            {
                                logiCVC_ClutWrite(logiCvcClutPtr, i, 0);
                            }
                        }
                        DEBUG_PRINT("logiCVC_%d, Layer %d - cleared with 0x00000000\n\r", instanceId, layer);
                    }
                }
                /* Enable all layers */
                if(!(flags & skipEnableLayers))
                {
                    for(layer = 0; layer < layers; layer++)
                    {
                        logiCvcLayerPtr = logiCVC_GetLayerPointer(logiCvcPtr, layer);
                        if(flags & skipCvcInitRegisters)
                        {
                            logiCVC_SetLayerControlFlags(logiCvcLayerPtr, LX_CTRL_EN_LAYER_MSK);
                        }
                        else
                        {
                            logiCVC_LayerEnDis(logiCvcLayerPtr, TRUE);
                        }
                    }
                }
                /* Turn on backlight after display is ready and all layers are cleared */
                if((plfCallback != NULL) && (plfCallback->plfDisplay_enableBacklight != NULL) && (!(flags & skipDisplayControl)))
                {
                    plfCallback->plfDisplay_enableBacklight(instanceId, (OsObjectHandleT)logiCvcPtr);
                }
                if(!(flags & skipCvcPowerControl))
                {
                    logiCVC_PowerControl(logiCvcPtr, CVC_PWR_BACKLIGHT, 1);
                }
                if((plfCallback != NULL) && (plfCallback->plfDisplay_setBacklight != NULL) && (!(flags & skipDisplayControl)))
                {
                    plfCallback->plfDisplay_setBacklight(instanceId, 80, (OsObjectHandleT)logiCvcPtr);
                }

                for(layer = 0; layer < layers; layer++)
                {
                    logiCvcLayerPtr = logiCVC_GetLayerPointer(logiCvcPtr, layer);
                    bitsPerPix = logiCVC_GetLayerParam(logiCvcLayerPtr, CVC_LAYER_COLOR_BITS_E);
                    alphaMode = logiCVC_GetLayerParam(logiCvcLayerPtr, CVC_LAYER_ALPHA_MODE_E);
                    if(bitsPerPix == 8)
                    {
                        if(alphaMode == LOGICVC_ALPHA_MODE_CLUT_INTERNAL_ARGB_6565)
                            logiCVC_SetTransparentColor(logiCvcLayerPtr, TRANSP_COLOR_8BPP_CLUT_16);
                        else if(alphaMode == LOGICVC_ALPHA_MODE_CLUT_INTERNAL_ARGB_8888)
                            logiCVC_SetTransparentColor(logiCvcLayerPtr, TRANSP_COLOR_8BPP_CLUT_24);
                    }
                    else if(bitsPerPix == 16)
                    {
                        logiCVC_SetTransparentColor(logiCvcLayerPtr, TRANSP_COLOR_16BPP);
                    }
                    else
                    {
                        logiCVC_SetTransparentColor(logiCvcLayerPtr, TRANSP_COLOR_24BPP);
                    }
                }
            }
            else
            {
                ERROR_PRINT("logiCVC_init() - ERROR logiCVC %d!\n\r", instanceId);
                if(pCvcDisplayData == NULL)
                {
                    ERROR_PRINT("No display data!\n\r", instanceId);
                }
                else
                {
                    ERROR_PRINT("Display HRES %d - VRAM HRES %d\n\r",
                        (pCvcDisplayData->HorizontalRes), logiCvcConfigPtr->RowStride);
                }
                logiCVC_Deinitialize(logiCvcPtr);
                logiCvcPtr = NULL;
            }
        }
    }

    return logiCvcPtr;
}


#endif /* #ifdef XPAR_LOGICVC_NUM_INSTANCES */

/* @} */
