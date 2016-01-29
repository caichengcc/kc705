/**
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2009 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
******************************************************************************/

/**
*
* @file OsSpinLock_inline.h
*
* Windows CE OS spin lock inline functions.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a HJ  30/10/09  First release \n
* </pre>
*
******************************************************************************/

#ifndef INC_OsSpinLock_inline_h
#define INC_OsSpinLock_inline_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "OsSpinLock.h"
#include "SwConfig.h"


/************************** Function Definitions *****************************/

/**
******************************************************************************
*
* Creates a spin lock.
*
* @param        hParent is a spin lock parent object. Unused on Windows CE.
*
* @return       Pointer to a newly created spin lock.
*
* @note         This type of spin lock is not to be used from an ISR.
*
******************************************************************************/
OS_INLINE
OsSpinLockT *
OsSpinLockConstruct(
    OsObjectHandleT hParent)
{
    PATH_TESTED();
    return swConfig_spinLockConstruct(hParent);
}


/**
******************************************************************************
*
* Destroys a spin lock object.
*
* @param        pLock is a spin lock pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsSpinLockDestruct(
    OsSpinLockT * pLock)
{
    PATH_TESTED();
    swConfig_spinLockDestruct(pLock);
}

/**
******************************************************************************
*
* Acquires a spin lock.
*
* @param        pLock is a spin lock pointer.
*
* @return       None.
*
* @note         Each call must be paired with with a call to OsSpinLockRelease.
*
******************************************************************************/
OS_INLINE
void
OsSpinLockAcquire(
    OsSpinLockT * pLock)
{
    PATH_TESTED();
    swConfig_spinLockAcquire(pLock);
}


/**
******************************************************************************
*
* Releases a spinlock.
*
* @param        pLock is a spinlock handle.
*
* @return       None.
*
* @note         Each call must be paired with with a call to OsSpinLockAcquire.
*
******************************************************************************/
OS_INLINE
void
OsSpinLockRelease(
    OsSpinLockT * pLock)
{
    PATH_TESTED();
    swConfig_spinLockRelease(pLock);
}


#ifdef __cplusplus
}
#endif


#endif /* INC_OsSpinLock_inline_h */
