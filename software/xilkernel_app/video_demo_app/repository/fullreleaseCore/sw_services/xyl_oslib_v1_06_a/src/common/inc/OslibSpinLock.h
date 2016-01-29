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
* @file OslibSpinLock.h
*
* Contains relevant OS specific spin locks, as well as the spin lock
* functionality common to all the OSs.
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

#ifndef INC_OslibSpinLock_h
#define INC_OslibSpinLock_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibTypes.h"
#include "OsSpinLock.h"



/************************** Function Declarations ****************************/

/**
******************************************************************************
*
* Creates a spin lock.
*
* @param        hParent is a spin lock parent object.
*
* @return       Pointer to a newly created spin lock.
*
* @note         This type of spin lock is not to be used from an ISR.
*
******************************************************************************/
OsSpinLockT *
OsSpinLockConstruct(
    OsObjectHandleT hParent);


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
void
OsSpinLockDestruct(
    OsSpinLockT * pLock);


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
void
OsSpinLockAcquire(
    OsSpinLockT * pLock);


/**
******************************************************************************
*
* Releases a spin lock.
*
* @param        pLock is a spin lock handle.
*
* @return       None.
*
* @note         Each call must be paired with with a call to OsSpinLockAcquire.
*
******************************************************************************/
void
OsSpinLockRelease(
    OsSpinLockT * pLock);


/**
******************************************************************************
*
* Creates an interrupt spin lock.
*
* @param        hParent is an interrupt spin lock parent object.
*
* @return       Pointer to a newly created interrupt spin lock.
*
* @note         This type of spin lock is to be used from an ISR.
*
******************************************************************************/
OsIrqSpinLockT *
OsIrqSpinLockConstruct(
    OsObjectHandleT hParent);


/**
******************************************************************************
*
* Destroys an interrupt spin lock object.
*
* @param        pLock is an interrupt spin lock pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsIrqSpinLockDestruct(
    OsIrqSpinLockT * pLock);


/**
******************************************************************************
*
* Acquires an interrupt spin lock.
*
* @param        pLock is an interrupt spin lock pointer.
*
* @return       None.
*
* @note         Each call must be paired with with a call to
*               OsIrqSpinLockRelease.
*
******************************************************************************/
void
OsIrqSpinLockAcquire(
    OsIrqSpinLockT * pLock);


/**
******************************************************************************
*
* Releases an interrupt spin lock.
*
* @param        pLock is an interrupt spin lock handle.
*
* @return       None.
*
* @note         Each call must be paired with with a call to
*               OsIrqSpinLockAcquire.
*
******************************************************************************/
void
OsIrqSpinLockRelease(
    OsIrqSpinLockT * pLock);



/****************************** Include Files ********************************/
#include "OsSpinLock_inline.h"



#ifdef __cplusplus
}
#endif


#endif /* INC_OslibSpinLock_h */
