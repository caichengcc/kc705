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
* @file OslibHwAccess.c
*
* Implements required HW access functionality for the Linux OS wrapper.
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


/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "OslibHwResource.h"
#include "OslibHwAccess.h"
#ifdef OSLIB_ADD_PCI_SUPPORT
# include "OsHwPciAccessConfig.h"
#endif
#ifdef OSLIB_ADD_USB_SUPPORT
# include "OsHwUsbAccessConfig.h"
#endif
#ifdef OSLIB_ADD_CPU_SUPPORT
# include "OsHwCpuAccessConfig.h"
#endif
#ifdef OSLIB_ADD_CPU_VLINK_SUPPORT
# include "OsHwCpuVlinkAccessConfig.h"
#endif



/************************** Function Definitions *****************************/

/**
******************************************************************************
*
* Initializes a hardware access descriptor. If the range is in memory space,
* performs hardware access mapping to nonpaged system space.
*
* @param        pHwResource points to a HW resource descriptor.
* @param        bOrdered is TRUE if transfers cannot be optimised.
*
* @return       Pointer to a hardware access descriptor, or OS_INVALID_HANDLE on failure.
*
* @note         None.
*
******************************************************************************/
struct _OsHwAccessObject *
OsHwAccessInit(
    struct _OsHwResource const * const pHwResource,
    BOOLEAN                            bOrdered)
{
    struct _OsHwAccessObject * pOsHwAccessObject = OS_INVALID_HANDLE;
    PATH_TESTED();

    if (pHwResource != OS_INVALID_HANDLE)
    {
        OsHwResourceT const * const pResource
            = (OsHwResourceT const * const)pHwResource;

        PATH_TESTED();

        switch (pResource->busType)
        {
#ifdef OSLIB_ADD_PCI_SUPPORT
        case OS_BUS_TYPE__PCI:
            PATH_TESTED();
            pOsHwAccessObject = OsHwPciAccessInit(&pResource->bus.pci, bOrdered);
            break;
#endif

#ifdef OSLIB_ADD_USB_SUPPORT
        case OS_BUS_TYPE__USB:
            PATH_NOT_YET_TESTED();
            pOsHwAccessObject = OsHwUsbAccessInit(&pResource->bus.usb, bOrdered);
            break;
#endif

#ifdef OSLIB_ADD_CPU_SUPPORT
        case OS_BUS_TYPE__CPU:
            PATH_TESTED();
            pOsHwAccessObject = OsHwCpuAccessInit(&pResource->bus.cpu, bOrdered);
            break;
#endif

#ifdef OSLIB_ADD_CPU_VLINK_SUPPORT
        case OS_BUS_TYPE__CPU_VLINK:
            PATH_TESTED();
            pOsHwAccessObject = OsHwCpuVlinkAccessInit(&pResource->bus.cpuVlink, bOrdered);
            break;
#endif

        default:
            PATH_TESTED();
            break;
        }
    }

    return pOsHwAccessObject;
}


/**
******************************************************************************
*
* Deinitializes a hardware access descriptor. If the range is memory mapped,
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
OsHwAccessDeinit(
    struct _OsHwAccessObject * const pHwAccessObject)
{
    PATH_TESTED();
    if (pHwAccessObject != OS_INVALID_HANDLE)
    {
        OsHwAccessObjectT const * const pObject
            = (OsHwAccessObjectT const * const)pHwAccessObject;

        PATH_TESTED();

        switch (pObject->busType)
        {
#ifdef OSLIB_ADD_PCI_SUPPORT
        case OS_BUS_TYPE__PCI:
            PATH_TESTED();
            OsHwPciAccessDeinit(pHwAccessObject);
            break;
#endif

#ifdef OSLIB_ADD_USB_SUPPORT
        case OS_BUS_TYPE__USB:
            PATH_NOT_YET_TESTED();
            OsHwUsbAccessDeinit(pHwAccessObject);
            break;
#endif

#ifdef OSLIB_ADD_CPU_SUPPORT
        case OS_BUS_TYPE__CPU:
            PATH_NOT_YET_TESTED();
            OsHwCpuAccessDeinit(pHwAccessObject);
            break;
#endif

#ifdef OSLIB_ADD_CPU_VLINK_SUPPORT
        case OS_BUS_TYPE__CPU_VLINK:
            PATH_NOT_YET_TESTED();
            OsHwCpuVlinkAccessDeinit(pHwAccessObject);
            break;
#endif

        default:
            PATH_NOT_YET_TESTED();
            break;
        }
    }
}


/**
******************************************************************************
*
* Reports the register range virtual address belonging
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
OsHwAccessGetVirtualAddress(
    struct _OsHwAccessObject const * const pHwAccessObject)
{
    void * pVirtual = NULL;
    PATH_TESTED();

    if (pHwAccessObject != OS_INVALID_HANDLE)
    {
        OsHwAccessObjectT const * const pObject
            = (OsHwAccessObjectT const * const)pHwAccessObject;

        PATH_TESTED();

        switch (pObject->busType)
        {
#ifdef OSLIB_ADD_PCI_SUPPORT
        case OS_BUS_TYPE__PCI:
            PATH_TESTED();
            pVirtual = OsHwPciAccessGetVirtualAddress(pHwAccessObject);
            break;
#endif

#ifdef OSLIB_ADD_USB_SUPPORT
        case OS_BUS_TYPE__USB:
            PATH_NOT_YET_TESTED();
            pVirtual = OsHwUsbAccessGetVirtualAddress(pHwAccessObject);
            break;
#endif

#ifdef OSLIB_ADD_CPU_SUPPORT
        case OS_BUS_TYPE__CPU:
            PATH_NOT_YET_TESTED();
            pVirtual = OsHwCpuAccessGetVirtualAddress(pHwAccessObject);
            break;
#endif

#ifdef OSLIB_ADD_CPU_VLINK_SUPPORT
        case OS_BUS_TYPE__CPU_VLINK:
            PATH_NOT_YET_TESTED();
            pVirtual = OsHwCpuVlinkAccessGetVirtualAddress(pHwAccessObject);
            break;
#endif

        default:
            PATH_NOT_YET_TESTED();
            break;
        }
    }

    return pVirtual;
}
