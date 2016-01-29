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
* @file OslibMemory.h
*
* Contains relevant OS specific memory wrappers, as well as the memory
* handling functionality common to all the OSs.
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

#ifndef INC_OslibMemory_h
#define INC_OslibMemory_h

#ifdef __cplusplus
extern "C"
{
#endif



/********************************* Types *************************************/

/**
* OsMemoryTypeE lists different memory types.
* \brief Input argument to \ref OsMemoryAllocate.
*/
typedef enum
{
    /** Unspecified memory type. */
    OS_MEMORY_TYPE__UNDEFINED,

    /**
    * Non-paged memory type.
    * This memory type is used to store data that might be accessed when the
    * system cannot handle page faults, e.g. when ISRs or DPCs are executed.
    * On Windows, this is the case whenever the code runs at
    * IRQL >= DISPATCH_LEVEL.
    * This memory type is a scarce resource and drivers should allocate it
    * only when necessary.
    */
    OS_MEMORY_TYPE__NON_PAGED_POOL,

    /**
    * Paged memory type.
    * This memory type is used to store data that can be written to the paging
    * file. Do not access any data allocated using this memory type from ISRs
    * or DPCs.
    * On Windows, use it only when the code runs at IRQL < DISPATCH_LEVEL.
    */
    OS_MEMORY_TYPE__PAGED_POOL
} OsMemoryTypeE;



/****************************** Include Files ********************************/
#include "OslibTypes.h"
#include "OsMemory.h"


/******************************* Definitions *********************************/

/**
 *  Defined to simplify memory allocation from general application code.
 *
 *  !!! Do not use these macros from ISR or DPC code !!!
 */
#define OsMalloc(size)                      \
    OsMemoryAllocate((size), OS_MEMORY_TYPE__PAGED_POOL)

#define OsMallocAligned(size, alignment)    \
    OsMemoryAllocateAligned((size), OS_MEMORY_TYPE__PAGED_POOL, (alignment))



/************************** Function Declarations ****************************/

/**
******************************************************************************
*
* Allocates a memory.
*
* @param        size is the number of bytes to allocate.
* @param        type is a memory pool type.
*
* @return       A memory pointer on success or NULL on failure.
*
* @note         None.
*
******************************************************************************/
void *
OsMemoryAllocate(
    OsSizeT       size,
    OsMemoryTypeE type);


/**
******************************************************************************
*
* Frees a memory.
*
* @param        p is a memory pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsMemoryFree(
    void * p);

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
    OsSizeT alignment);

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
    void *ptr);


/****************************** Include Files ********************************/
#include "OsMemory_inline.h"



#ifdef __cplusplus
}
#endif


#endif /* INC_OslibMemory_h */
