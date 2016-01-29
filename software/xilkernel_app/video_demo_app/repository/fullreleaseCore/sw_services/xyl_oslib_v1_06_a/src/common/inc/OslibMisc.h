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
* @file OslibMisc.h
*
* Contains miscellaneous OS specific definitions.
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

#ifndef INC_OslibMisc_h
#define INC_OslibMisc_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibTypes.h"
#include "OsMisc.h"



/************************** Function Declarations ****************************/

/**
******************************************************************************
*
* Performs a 16-bit endian change.
*
* @param        src contains the value to be changed.
*
* @return       Changed value.
*
* @note         None.
*
******************************************************************************/
OsU16
OsChangeEndian16(
    OsU16 src);


/**
******************************************************************************
*
* Performs a 32-bit endian change.
*
* @param        src contains the value to be changed.
*
* @return       Changed value.
*
* @note         None.
*
******************************************************************************/
OsU32
OsChangeEndian32(
    OsU32 src);


/**
******************************************************************************
*
* Performs a 32-bit endian conversion to little endian.
* This conversion happens on big endian machines only.
* On little endian machine, no conversion takes place.
*
* @param        src contains the value to be converted.
* @param        pDest is an address to receive the LE value.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsToLe32(
    OsU32   src,
    OsU32 * pDest);


/**
******************************************************************************
*
* Performs a 32-bit endian conversion to big endian.
* This conversion happens on little endian machines only.
* On big endian machine, no conversion takes place.
*
* @param        src contains the value to be converted.
* @param        pDest is an address to receive the BE value.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsToBe32(
    OsU32   src,
    OsU32 * pDest);



/****************************** Include Files ********************************/
#include "OsMisc_inline.h"



#ifdef __cplusplus
}
#endif


#endif /* INC_OslibMisc_h */
