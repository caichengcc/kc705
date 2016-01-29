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
* @file logiCVC_demo.h
*
* This header file contains identifiers used in logiCVC demo.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ------ ---- -------- -------------------------------------------------------
* 1.00.a AF   08/06/10 First release
* 2.00.a VD   30/09/11 Updated code to logiCVC 3.00.a driver version
*
* </pre>
*
*****************************************************************************/

#ifndef __LOGICVC_DEMO__H__
#define __LOGICVC_DEMO__H__

#ifdef __cplusplus
extern "C" {
#endif

/************************** Include Files *****************************/
#include "logiCVC_init.h"


/************************** Constant Definitions *****************************/

#define ALPHA_8B              0x0700

#define ALPHA_16B         0x3F000000

// COLORS 24-bit
#define ALPHA_24B         0xFF000000
#define RED_24B           0xFFFF0000
#define GREEN_24B         0xFF00FF00
#define BLUE_24B          0xFF0000FF
#define YELLOW_24B        0xFFFFFF00
#define PURPLE_24B        0xFFFF00FF
#define CYAN_24B          0xFF00FFFF
#define BLACK_24B         0xFF000000
#define WHITE_24B         0xFFFFFFFF
#define MED_GRAY_24B      0xFFAAAAAA

/************************** Function Prototypes *****************************/

cvcRetT logiCVCDemo(logiCvcPointerT *logiCvcInstance);

#ifdef __cplusplus
}
#endif

#endif /* __LOGICVC_DEMO__H__ */

/* @} */
