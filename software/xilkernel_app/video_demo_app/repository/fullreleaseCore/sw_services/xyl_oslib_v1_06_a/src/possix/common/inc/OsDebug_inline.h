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
* @file OsDebug_inline.h
*
* Possix debug functionality inline functions.
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

#ifndef INC_OsDebug_inline_h
#define INC_OsDebug_inline_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OsDebug.h"



/************************** Function Definitions *****************************/

/**
******************************************************************************
*
* Initializes debug support.
*
* @param        None.
*
* @return       None.
*
* @note         This function must be called before any other debug functions
*               (e.g. \ref OsPrintfInfo) or macros
*               (e.g. \ref PATH_NOT_YET_TESTED) are called.
*
******************************************************************************/
OS_INLINE
void
OsDebugInit(void)
{
    /* Nothing to do. */
}



#ifdef __cplusplus
}
#endif


#endif /* INC_OsDebug_inline_h */
