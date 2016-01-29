/*****************************************************************************
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2009 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
*****************************************************************************/

/**
*
* @file SwConfig.h
*
* @brief This file contains software configuration function prototypes.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  AF   28/01/11 First release
* </pre>
*
*****************************************************************************/

#ifndef __SW_CONFIG_H__
#define __SW_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif


/***************************** Include Files ********************************/

#include "OsTypes.h"
#include "OsTime.h"
#include "OsSpinLock.h"


/************************* Function Declarations ****************************/

/**************************** TIMER MANAGEMENT ******************************/

void    swConfig_initTimerSubsystem(void);
OsSizeT swConfig_getTicksInMs(void);
void    swConfig_startTimer(void);
void    swConfig_stopTimer(void);
OsSizeT swConfig_getTickCount(void);
void    swConfig_sleep(OsSizeT ms);
OsSizeT swConfig_getElapsedMs(OsSizeT ms);


/********************** MEMORY ALLOCATION MANAGEMENT ************************/

void *swConfig_malloc(OsSizeT size);
void  swConfig_free(void *p);


/************************* THREAD LOCK MANAGEMENT ***************************/

OsSpinLockT *swConfig_spinLockConstruct(OsObjectHandleT hParent);
void         swConfig_spinLockDestruct(OsSpinLockT *pLock);
void         swConfig_spinLockAcquire(OsSpinLockT *pLock);
void         swConfig_spinLockRelease(OsSpinLockT *pLock);


#ifdef __cplusplus
}
#endif

#endif // __SW_CONFIG_H__
