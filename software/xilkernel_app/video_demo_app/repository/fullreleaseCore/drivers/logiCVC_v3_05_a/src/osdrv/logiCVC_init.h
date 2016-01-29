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
* @file logiCVC_init.h
*
* @brief This header file contains type definitions for logiCVC initialization.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ------ ---- -------- -------------------------------------------------------
* 1.00.a VD   18/06/07 First release as xylon_hardware_init.c
* 1.10.a GP   15/04/08 Renamed to logicbricks_init, reorganised and moved to platform lib
* 2.00.a DJ   16/08/10 Renamed to logiCVC_init, reorganised and moved to platform logicBRICKS
* 3.00.a DJ   30/09/11 Renamed and reorganised according to logiCVC 3.00.a driver structure
*
* </pre>
*
*****************************************************************************/

#ifndef __LOGICVC_INIT_H__
#define __LOGICVC_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif


/***************************** Include Files ********************************/

#include "xparameters.h"

#ifdef XPAR_LOGICVC_NUM_INSTANCES

#include "logiCVC.h"


/**************************** Type Definitions ******************************/



/**
 * This enum contains the flags that enable parts of logiCVC initialization code
 */
typedef enum
{
    skipNone             = (0),      /**< Noting is skipped */
    skipCvcInitRegisters = (1<<0),   /**< Only init driver but doesn't write in any of logiCVC registers */
    skipCvcPowerControl  = (1<<1),   /**< Skip power sequence */
    skipClearVideoMemory = (1<<2),   /**< Skip clearing video memory and CLUTs */
    skipEnableLayers     = (1<<3),   /**< Skip enabling layers */
    skipDisplayControl   = (1<<4),   /**< Skip calling display control functions \ref logiCVC_plfInitCallbackT */
    skipAll              = (1<<5),   /**< Skip all but the logiCVC driver init */
    clearAllLayerBuffers = (1<<6)    /**< Clear all buffers on all logiCVC layers, valid if skipClearVideoMemory is not set */
} logiCVC_init_flagsE;

/**
 * This enum contains callbacks to different display/board specific function that will be called from  \ref logiCVC_init
 */
typedef struct
{
    void (*plfDisplay_setFrequency)(OsU8 deviceId, OsU32 freqMhz, OsObjectHandleT objHandle);   /**< sets external clock required by logiCVC for requsted resolution */
    void (*plfDisplay_enablePowerSupply)(OsU8 deviceId, OsObjectHandleT objHandle);             /**< enables indexed display power supply */
    void (*plfDisplay_enableSignals)(OsU8 deviceId, OsObjectHandleT objHandle);                 /**< enables indexed display control and data signals */
    void (*plfDisplay_enableBacklight)(OsU8 deviceId, OsObjectHandleT objHandle);               /**< enables indexed display backlight */
    void (*plfDisplay_setBacklight)(OsU8 deviceId, OsU8 value, OsObjectHandleT objHandle);      /**< sets backlight for indexed display, value: brightness level (0..100)*/
} logiCVC_plfInitCallbackT;



/************************** Function Prototypes *****************************/

logiCvcPointerT logiCVC_init(OsSizeT instanceId,
                             OsSizeT displayType,
                             logiCVC_init_flagsE flags,
                             logiCVC_plfInitCallbackT *plfCallback);

logiCvcPointerT logiCVC_initDd(OsSizeT instanceId,
                               logiCvcDisplayT const *pCvcDisplayData,
                               logiCVC_init_flagsE flags,
                               logiCVC_plfInitCallbackT *plfCallback);

#endif /* #ifdef XPAR_LOGICVC_NUM_INSTANCES */

#ifdef __cplusplus
}
#endif

#endif /* __LOGICVC_INIT_H__ */

/* @} */
