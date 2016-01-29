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
* @file OsHwCpuVlinkAccessConfig.c
*
* Implements required HW access functionality for the Possix wrapper.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a GP   23/03/12 First release \n
* </pre>
*
******************************************************************************/


/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "OslibMemory.h"
#include "OslibHwAccess.h"
#include "../inc/OsHwCpuVlinkResource.h"
#include "OsHwCpuVlinkAccessor.h"
#include "../inc/OsHwCpuVlinkAccessConfig.h"
#include "OsHwCpuVlinkAccess.c"



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
OsHwCpuVlinkAccessInit(
    OsHwCpuVlinkResourceT const * const pHwResource,
    BOOLEAN                     bOrdered)
{
    OsHwAccessObjectT         * pHwAccessObject;
    OsHwCpuVlinkAccessorT     * pHwAccess;

    OS_ASSERT(pHwResource != NULL);
    OS_ASSERT(pHwResource->physBaseAddress != 0);

    pHwAccessObject =
        (OsHwAccessObjectT *)OsMemoryAllocate(sizeof(*pHwAccessObject), OS_MEMORY_TYPE__NON_PAGED_POOL);

    if (pHwAccessObject != NULL)
    {
        pHwAccess =
            (OsHwCpuVlinkAccessorT *)OsMemoryAllocate(sizeof(*pHwAccess), OS_MEMORY_TYPE__NON_PAGED_POOL);

        if (pHwAccess != NULL)
        {
              pHwAccess->pBase = (OsU8 *)pHwResource->physBaseAddress;
              pHwAccess->pBaseVlink = (OsU8 *)pHwResource->physBaseAddressVlink;
              pHwAccess->vLinkTimeoutMs = 100; /* 100ms FIXME */

            if (pHwAccess->pBase != NULL && pHwAccess->pBaseVlink != NULL)
            {
                pHwAccess->physBaseAddress      = pHwResource->physBaseAddress;
                pHwAccess->physBaseAddressVlink = pHwResource->physBaseAddressVlink;

                pHwAccess->size                 = pHwResource->size;
                pHwAccess->sizeVlink            = pHwResource->sizeVlink;

                pHwAccessObject->busType = OS_BUS_TYPE__CPU_VLINK;

                pHwAccessObject->pfRead8   = &OsCpuVlinkMemoryRegRead8;
                pHwAccessObject->pfRead16  = &OsCpuVlinkMemoryRegRead16;
                pHwAccessObject->pfRead32  = &OsCpuVlinkMemoryRegRead32;
                pHwAccessObject->pfWrite8  = &OsCpuVlinkMemoryRegWrite8;
                pHwAccessObject->pfWrite16 = &OsCpuVlinkMemoryRegWrite16;
                pHwAccessObject->pfWrite32 = &OsCpuVlinkMemoryRegWrite32;

                pHwAccessObject->pfFifoRead8   = &OsCpuVlinkMemoryFifoRead8;
                pHwAccessObject->pfFifoRead16  = &OsCpuVlinkMemoryFifoRead16;
                pHwAccessObject->pfFifoRead32  = &OsCpuVlinkMemoryFifoRead32;
                pHwAccessObject->pfFifoWrite8  = &OsCpuVlinkMemoryFifoWrite8;
                pHwAccessObject->pfFifoWrite16 = &OsCpuVlinkMemoryFifoWrite16;
                pHwAccessObject->pfFifoWrite32 = &OsCpuVlinkMemoryFifoWrite32;

                pHwAccessObject->pfMultiRead8   = &OsCpuVlinkMemoryRegMultiRead8;
                pHwAccessObject->pfMultiRead16  = &OsCpuVlinkMemoryRegMultiRead16;
                pHwAccessObject->pfMultiRead32  = &OsCpuVlinkMemoryRegMultiRead32;
                pHwAccessObject->pfMultiWrite8  = &OsCpuVlinkMemoryRegMultiWrite8;
                pHwAccessObject->pfMultiWrite16 = &OsCpuVlinkMemoryRegMultiWrite16;
                pHwAccessObject->pfMultiWrite32 = &OsCpuVlinkMemoryRegMultiWrite32;

                pHwAccessObject->pHwAccessor = pHwAccess;

                return pHwAccessObject;
            }

            OsMemoryFree(pHwAccess);
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
OsHwCpuVlinkAccessDeinit(
    OsHwAccessObjectT * const pHwAccessObject)
{
	OsHwCpuVlinkAccessorT * pHwAccess;

    OS_ASSERT(pHwAccessObject != NULL);

    pHwAccess = pHwAccessObject->pHwAccessor;
    OS_ASSERT(pHwAccess != NULL);

    if (OS_BUS_TYPE__CPU_VLINK == pHwAccessObject->busType)
    {
        //iounmap((void *)pHwAccess->pBase);

        pHwAccess->physBaseAddress = 0;
        pHwAccess->size            = 0;
        pHwAccess->pBase           = NULL;

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

        OsMemoryFree(pHwAccess);
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
OsHwCpuVlinkAccessGetVirtualAddress(
    OsHwAccessObjectT const * const pHwAccessObject)
{
    return ((pHwAccessObject != NULL) && (pHwAccessObject->pHwAccessor != NULL))
        ? (void*)pHwAccessObject->pHwAccessor->pBase
        : (void*)NULL;
}
