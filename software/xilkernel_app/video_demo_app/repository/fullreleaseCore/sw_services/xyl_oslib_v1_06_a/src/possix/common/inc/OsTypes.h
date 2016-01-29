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
* @file OsTypes.h
*
* Contains general data type deinitions for the Possix wrapper.
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

#ifndef INC_OsTypes_h
#define INC_OsTypes_h

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>

/****************************** Include Files ********************************/




/******************************* Definitions *********************************/
#ifndef FALSE
#define FALSE                    (0 == 1) /**< Logical false */
#endif
#ifndef TRUE
#define TRUE                     (!FALSE) /**< Logical true */
#endif

#define TEXT(s)                  s        /**< Identifies a string as a Unicode string or as an ANSI string. */

#define OS_INVALID_HANDLE        NULL     /**< Invalid handle/pointer. */

#define OS_INLINE                inline __attribute__((always_inline)) extern  /**< Inline function specifier */



/********************************* Types *************************************/
typedef signed char                   OsS8;  /**< Oslib signed 8 bit type */
typedef signed short                  OsS16; /**< Oslib signed 16 bit type */
typedef signed long                   OsS32; /**< Oslib signed 32 bit type */
typedef signed long long              OsS64; /**< Oslib signed 64 bit type */

typedef unsigned char                 OsU8;  /**< Oslib unsigned 8 bit type */
typedef unsigned short                OsU16; /**< Oslib unsigned 16 bit type */
typedef unsigned long                 OsU32; /**< Oslib unsigned 32 bit type */
typedef unsigned long long            OsU64; /**< Oslib unsigned 64 bit type */

/** 32 bits for 32-bit OSs, 64 bits for 64-bit OSs. */
typedef unsigned long                 OsUptrT;

typedef unsigned char                 BOOLEAN;  /**< Oslib boolean type, FIX: rename to OsBoolT */
typedef BOOLEAN                       OsBoolT;  /**< Oslib 8 bit bool type */

/** Standard char: an 8-bit char or a wide char. */
typedef char                          OsCharT;

/** Corresponds to the integral data type returned by the sizeof operator. */
typedef size_t                        OsSizeT;

typedef void *                        OsObjectHandleT; /**< Oslib object handle type */



#ifdef __cplusplus
}
#endif


#endif /* INC_OsTypes_h */
