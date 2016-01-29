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
* @file OsMemory_inline.h
*
* Possix memory handling inline functions.
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

#ifndef INC_OsMemory_inline_h
#define INC_OsMemory_inline_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include <stdlib.h>
#include "OslibDebug.h"
#include "OsMemory.h"
#include "SwConfig.h"



/************************** Function Definitions *****************************/

/**
******************************************************************************
*
* Allocates a memory buffer.
*
* @param        size is the number of bytes to allocate.
* @param        type is a memory pool type.
*
* @return       A memory buffer pointer on success or NULL on failure.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void *
OsMemoryAllocate(
    OsSizeT size,
    OsMemoryTypeE type)
{
    void *p;
    PATH_TESTED();
    p = swConfig_malloc(size);
    if (p == NULL)
    {
        OsPrintfError("OsMemoryAllocate allocation failure\n\r");
    }
    return p;
}


/**
******************************************************************************
*
* Frees a memory buffer.
*
* @param        p is a memory pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsMemoryFree(
    void * pBuffer)
{
    PATH_TESTED();
    swConfig_free(pBuffer);
}



#ifdef __cplusplus
}
#endif


#endif /* INC_OsMemory_inline_h */
