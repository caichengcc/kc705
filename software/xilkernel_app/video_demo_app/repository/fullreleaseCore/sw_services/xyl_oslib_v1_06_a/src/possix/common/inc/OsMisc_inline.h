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
* @file OsMisc_inline.h
*
* Possix miscellaneous inline functions.
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

#ifndef INC_OsMisc_inline_h
#define INC_OsMisc_inline_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "OsMisc.h"
#include "OsTypes.h"


/************************** Function Definitions *****************************/

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
OS_INLINE
OsU16
OsChangeEndian16(
    OsU16 src)
{
    PATH_NOT_YET_TESTED();
    return (  ((src & 0x00FF) << 8)
            | ((src & 0xFF00) >> 8) );
}


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
OS_INLINE
OsU32
OsChangeEndian32(
    OsU32 src)
{
    PATH_NOT_YET_TESTED();
    return (  ((src & 0x000000FF) << 24)
            | ((src & 0x0000FF00) <<  8)
            | ((src & 0x00FF0000) >>  8)
            | ((src & 0xFF000000) >> 24) );
}


#ifdef __cplusplus
}
#endif


#endif /* INC_OsMisc_inline_h */
