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
* @file OsHwCpuAccessConfig.c
*
* Implements required cpu access functionality for the Possic wrapper.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a HJ   30/10/09 First release \n
* 1.10.a DJ   27/05/10 HW access functions moved into header file, \n
*                      functions Init & Deinit moved into Config file\n
* </pre>
*
******************************************************************************/


/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "OslibMemory.h"
#include "OslibHwAccess.h"
#include "OslibHwResource.h"
#include "../../../cpuAccess/OsHwCpuResource.h"
#include "../../../cpuAccess/OsHwCpuAccessor.h"
#include "../inc/OsHwCpuAccessConfig.h"
#include "../../../cpuAccess/OsHwCpuAccess.c"



/******************************* Definitions *********************************/



/************************** Function Definitions *****************************/

/**
****************************************************************************
*
* Initializes a CPU hardware access. If the range is in memory space,
* performs hardware access mapping to nonpaged system space.
*
* @param        pHwResource points to a HW resource descriptor.
* @param        bOrdered is TRUE if transfers cannot be optimised.
*
* @return       Pointer to a hardware access descriptor, or NULL on failure.
*
* @note         None.
*
******************************************************************************/
struct _OsHwAccessObject *
OsHwCpuAccessInit(
    OsHwCpuResourceT const * const pHwResource,
    BOOLEAN                     bOrdered)
{
    OsHwAccessObjectT * pHwAccessObject;
    OsHwCpuAccessorT     * pCpuHwAccess;

    OS_ASSERT(pHwResource != NULL);
    OS_ASSERT(pHwResource->physBaseAddress != 0);

    pHwAccessObject =
        (OsHwAccessObjectT *)OsMemoryAllocate(sizeof(OsHwAccessObjectT), OS_MEMORY_TYPE__NON_PAGED_POOL);

    if (pHwAccessObject != NULL)
    {
        pCpuHwAccess =
            (struct _OsHwAccessor *)OsMemoryAllocate(sizeof(OsHwCpuAccessorT), OS_MEMORY_TYPE__NON_PAGED_POOL);

        if (pCpuHwAccess != NULL)
        {
              pCpuHwAccess->pBase = (OsU8 *)pHwResource->physBaseAddress;

            if (pCpuHwAccess->pBase != NULL)
            {
                pCpuHwAccess->physBaseAddress = pHwResource->physBaseAddress;

                pCpuHwAccess->size = pHwResource->size;

                pHwAccessObject->busType = OS_BUS_TYPE__CPU;

                pHwAccessObject->pfRead8   = &OsCpuMemoryRegRead8;
                pHwAccessObject->pfRead16  = &OsCpuMemoryRegRead16;
                pHwAccessObject->pfRead32  = &OsCpuMemoryRegRead32;
                pHwAccessObject->pfWrite8  = &OsCpuMemoryRegWrite8;
                pHwAccessObject->pfWrite16 = &OsCpuMemoryRegWrite16;
                pHwAccessObject->pfWrite32 = &OsCpuMemoryRegWrite32;

                pHwAccessObject->pfFifoRead8   = &OsCpuMemoryFifoRead8;
                pHwAccessObject->pfFifoRead16  = &OsCpuMemoryFifoRead16;
                pHwAccessObject->pfFifoRead32  = &OsCpuMemoryFifoRead32;
                pHwAccessObject->pfFifoWrite8  = &OsCpuMemoryFifoWrite8;
                pHwAccessObject->pfFifoWrite16 = &OsCpuMemoryFifoWrite16;
                pHwAccessObject->pfFifoWrite32 = &OsCpuMemoryFifoWrite32;

                pHwAccessObject->pfMultiRead8   = &OsCpuMemoryRegMultiRead8;
                pHwAccessObject->pfMultiRead16  = &OsCpuMemoryRegMultiRead16;
                pHwAccessObject->pfMultiRead32  = &OsCpuMemoryRegMultiRead32;
                pHwAccessObject->pfMultiWrite8  = &OsCpuMemoryRegMultiWrite8;
                pHwAccessObject->pfMultiWrite16 = &OsCpuMemoryRegMultiWrite16;
                pHwAccessObject->pfMultiWrite32 = &OsCpuMemoryRegMultiWrite32;

                pHwAccessObject->pHwAccessor = pCpuHwAccess;

                return pHwAccessObject;
            }

            OsMemoryFree(pCpuHwAccess);
        }

        OsMemoryFree(pHwAccessObject);
    }

    return NULL;
}


/**
******************************************************************************
*
* Deinitializes a CPU hardware access. If the range is memory mapped,
* performs hardware access unmapping from nonpaged system space.
*
* @param        pHwAccessObject points to a hardware access descriptor.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsHwCpuAccessDeinit(
    OsHwAccessObjectT * const pHwAccessObject)
{
	OsHwCpuAccessorT * pCpuHwAccess;

    OS_ASSERT(pHwAccessObject != NULL);

    pCpuHwAccess = pHwAccessObject->pHwAccessor;
    OS_ASSERT(pCpuHwAccess != NULL);

    if (OS_BUS_TYPE__CPU == pHwAccessObject->busType)
    {
        //iounmap((void *)pCpuHwAccess->pBase);

        pCpuHwAccess->physBaseAddress = 0;
        pCpuHwAccess->size            = 0;
        pCpuHwAccess->pBase           = NULL;

        pHwAccessObject->busType = OS_BUS_TYPE__UNDEFINED;

        pHwAccessObject->pfRead8   = NULL;
        pHwAccessObject->pfRead16  = NULL;
        pHwAccessObject->pfRead32  = NULL;
        pHwAccessObject->pfWrite8  = NULL;
        pHwAccessObject->pfWrite16 = NULL;
        pHwAccessObject->pfWrite32 = NULL;

        pHwAccessObject->pfFifoRead8   = NULL;
        pHwAccessObject->pfFifoRead16  = NULL;
        pHwAccessObject->pfFifoRead32  = NULL;
        pHwAccessObject->pfFifoWrite8  = NULL;
        pHwAccessObject->pfFifoWrite16 = NULL;
        pHwAccessObject->pfFifoWrite32 = NULL;

        pHwAccessObject->pfMultiRead8   = NULL;
        pHwAccessObject->pfMultiRead16  = NULL;
        pHwAccessObject->pfMultiRead32  = NULL;
        pHwAccessObject->pfMultiWrite8  = NULL;
        pHwAccessObject->pfMultiWrite16 = NULL;
        pHwAccessObject->pfMultiWrite32 = NULL;

        OsMemoryFree(pCpuHwAccess);
        OsMemoryFree(pHwAccessObject);
    }
}

/**
******************************************************************************
*
* Reports the CPU register range virtual address belonging
* to a specified OsHwAccessObject.
*
* @param        pHwAccessObject points to a register range descriptor.
*
* @return       Virtual address.
*
* @note         None.
*
******************************************************************************/
void *
OsHwCpuAccessGetVirtualAddress(
    OsHwAccessObjectT const * const pHwAccessObject)
{
    return ((pHwAccessObject != NULL) && (pHwAccessObject->pHwAccessor != NULL))
        ? (void*)pHwAccessObject->pHwAccessor->pBase
        : (void*)NULL;
}
