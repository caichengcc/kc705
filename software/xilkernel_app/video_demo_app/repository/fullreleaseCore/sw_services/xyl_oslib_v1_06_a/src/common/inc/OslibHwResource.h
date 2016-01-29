/**
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2012 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
******************************************************************************/

/**
*
* @file OslibHwResource.h
*
* Contains the HW resource type definition.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a HJ  22/03/12  First release \n
* </pre>
*
******************************************************************************/

#ifndef INC_OslibHwResource_h
#define INC_OslibHwResource_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
# include "OslibHwAccess.h"

#ifdef OSLIB_ADD_PCI_SUPPORT
# include "OsHwPciResource.h"
#endif
#ifdef OSLIB_ADD_USB_SUPPORT
# include "OsHwUsbResource.h"
#endif
#ifdef OSLIB_ADD_CPU_SUPPORT
# include "OsHwCpuResource.h"
#endif
#ifdef OSLIB_ADD_CPU_VLINK_SUPPORT
# include "OsHwCpuVlinkResource.h"
#endif



/********************************* Types *************************************/

/**
* OsHwResourceT describes resources as given on the BUS. This structure must be initialized manually
* by the OS dependant code and then passes as input argument to the function \ref OsHwAccessInit.
* Input structure to the \ref OsHwAccessInit function
**/
/** HW resource descriptor */
typedef struct _OsHwResource
{
    OsBusTypeE  busType;

    union
    {
        /** Undefined resource */
        OsU8                    undefined;

#ifdef OSLIB_ADD_PCI_SUPPORT
        /** PCI resource */
        OsHwPciResourceT        pci;
#endif

#ifdef OSLIB_ADD_USB_SUPPORT
        /** USB resource */
        OsHwUsbResourceT        usb;
#endif

#ifdef OSLIB_ADD_CPU_SUPPORT
        /** CPU resource */
        OsHwCpuResourceT        cpu;
#endif

#ifdef OSLIB_ADD_CPU_VLINK_SUPPORT
        /** CPU VLINK resource */
        OsHwCpuVlinkResourceT   cpuVlink;
#endif

    } bus;

} OsHwResourceT;



#ifdef __cplusplus
}
#endif


#endif /* INC_OslibHwResource_h */
