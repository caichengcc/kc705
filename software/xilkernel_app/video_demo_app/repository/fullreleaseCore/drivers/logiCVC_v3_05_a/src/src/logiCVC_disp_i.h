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
* @file logiCVC_disp_i.h
*
* @brief This header file contains definitions for logiCVC supported displays.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ------ ---- -------- -------------------------------------------------------
* 1.00.a VD   18/06/07 First release
* 2.06.a DJ   27/09/11 Renamed and reorganised according to new logiCVC driver structure.
*
* </pre>
*
*****************************************************************************/

#ifndef LOGICVC_I_H /* prevent circular inclusions */
#define LOGICVC_I_H /* by using protection macros */

/***************************** Include Files ********************************/



/************************** Constant Definitions ****************************/

/**
 * Compiler switch to enable/disable all supported display types by logiCVC driver.
 * It is used when display data structure is passed directly from application.
 */
#ifdef LOGICVC_EXTERN_DT_DATA
#define SUPPORT_DT_FLAG 0
#else /* #ifdef LOGICVC_EXTERN_DT_DATA */
#define SUPPORT_DT_FLAG 1
#endif /* #ifdef LOGICVC_EXTERN_DT_DATA */

/** Change macros from 0 to 1, to enable support for certain display types.
 *  To reduce unnecessary memory consumption, if possible enable display support
 *  only for used type or types.
 */
#define SUPPORT_DT_SIMULATION               SUPPORT_DT_FLAG
#define SUPPORT_DT_512_512                  SUPPORT_DT_FLAG
#define SUPPORT_DT_1024_1024                SUPPORT_DT_FLAG
#define SUPPORT_DT_2048_2048                SUPPORT_DT_FLAG
#define SUPPORT_DT_512_384                  SUPPORT_DT_FLAG
#define SUPPORT_DT_2048_1536                SUPPORT_DT_FLAG
#define SUPPORT_DT_2048_512                 SUPPORT_DT_FLAG
#define SUPPORT_DT_VGA_640_480              SUPPORT_DT_FLAG
#define SUPPORT_DT_VGA_800_480              SUPPORT_DT_FLAG
#define SUPPORT_DT_VGA_800_600              SUPPORT_DT_FLAG
#define SUPPORT_DT_VGA_1024_768             SUPPORT_DT_FLAG
#define SUPPORT_DT_VGA_RBT_1024_768         SUPPORT_DT_FLAG
#define SUPPORT_DT_VGA_1280_1024            SUPPORT_DT_FLAG
#define SUPPORT_DT_VGA_1680_1050            SUPPORT_DT_FLAG
#define SUPPORT_DT_VGA_1920_1080            SUPPORT_DT_FLAG
#define SUPPORT_DT_VGA_1920_1200            SUPPORT_DT_FLAG
#define SUPPORT_DT_PAL                      SUPPORT_DT_FLAG
#define SUPPORT_DT_NTSC                     SUPPORT_DT_FLAG
#define SUPPORT_DT_1280_480                 SUPPORT_DT_FLAG
#define SUPPORT_DT_TD070WGCB2               SUPPORT_DT_FLAG
#define SUPPORT_DT_LQ065T9DR51U             SUPPORT_DT_FLAG
#define SUPPORT_DT_LTL652T                  SUPPORT_DT_FLAG
#define SUPPORT_DT_LTA035B                  SUPPORT_DT_FLAG
#define SUPPORT_DT_TD080WGCA1               SUPPORT_DT_FLAG
#define SUPPORT_DT_LQ070Y5DE01              SUPPORT_DT_FLAG
#define SUPPORT_DT_LLL652T                  SUPPORT_DT_FLAG
#define SUPPORT_DT_UMSH_8013MD              SUPPORT_DT_FLAG
#define SUPPORT_DT_MTF_TW70SP               SUPPORT_DT_FLAG
#define SUPPORT_DT_LTA149B780F              SUPPORT_DT_FLAG
#define SUPPORT_DT_XTRONIC_800_480          SUPPORT_DT_FLAG
#define SUPPORT_DT_XTRONIC_400_240          SUPPORT_DT_FLAG
#define SUPPORT_DT_DLF1095                  SUPPORT_DT_FLAG
#define SUPPORT_DT_LAJ123T001_JLR           SUPPORT_DT_FLAG
#define SUPPORT_DT_TCG062HVLDA_G20          SUPPORT_DT_FLAG
#define SUPPORT_DT_T_55382GD050JU_LW_A_ABN  SUPPORT_DT_FLAG
#define SUPPORT_DT_G065VN01V2               SUPPORT_DT_FLAG
#define SUPPORT_DT_TM050RBH01               SUPPORT_DT_FLAG
#define SUPPORT_DT_1920_1080                SUPPORT_DT_FLAG


#endif  /* end of protection macro */
