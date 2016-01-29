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
* @file OsMisc.h
*
* Contains miscellaneous definitions for the Possix wrapper.
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

#ifndef INC_OsMisc_h
#define INC_OsMisc_h

#ifdef __cplusplus
extern "C"
{
#endif

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


#ifdef __cplusplus
}
#endif


#endif /* INC_OsMisc_h */
