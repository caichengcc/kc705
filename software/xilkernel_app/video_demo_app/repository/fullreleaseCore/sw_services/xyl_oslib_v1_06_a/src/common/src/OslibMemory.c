/**
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2011 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
******************************************************************************/

/**
*
* @file OslibMemory.c
*
* Functions for allocating and freeing memory aligned to power of 2 byte boundary.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* ------ ---- -------- ------------------------------------------------------- \n
* 1.00.a as   07/12/11 First release \n
* </pre>
*
******************************************************************************/


/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "OslibMemory.h"


/************************** Function Definitions *****************************/
/**
******************************************************************************
*
* Allocates memory aligned to power of 2 byte boundary.
*
* @param        size is the number of bytes to allocate.
* @param        type is a memory pool type.
* @param        alignment is the power of 2 byte boundary.
*
* @return       A memory pointer on success or NULL on failure.
*
* @note         None.
*
******************************************************************************/
void *
OsMemoryAllocateAligned(
    OsSizeT size,
    OsMemoryTypeE type,
    OsSizeT alignment)
{
    OsSizeT padding;
    void *mem;
    void *ptr = NULL;

    PATH_TESTED();

    padding = (1 << alignment) - 1;
    mem = OsMemoryAllocate(size + padding + sizeof(void *), type);

    if (mem != NULL)
    {
        PATH_TESTED();
        ptr = (void *)(((OsSizeT)mem + sizeof(void *) + padding) & (~padding));
        ((void**)ptr)[-1] = mem;
    }

    return ptr;
}

/**
******************************************************************************
*
* Frees alligned memory.
*
* @param        ptr is a memory pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsMemoryFreeAligned(
    void *ptr)
{
    PATH_TESTED();

    if (ptr != NULL)
    {
        PATH_TESTED();
        OsMemoryFree(((void**)ptr)[-1]);
    }
}

