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
* @file OsSpinLock.h
*
* Contains required spin lock functionality for the Possix wrapper.
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

#ifndef INC_OsSpinLock_h
#define INC_OsSpinLock_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/



/********************************* Types *************************************/

/** Spinlock type definitions. */
typedef int          OsSpinLockT;
typedef OsSpinLockT  OsIrqSpinLockT;


#ifdef __cplusplus
}
#endif


#endif /* INC_OsSpinLock_h */
