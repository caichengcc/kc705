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
* @file OsHwCpuVlinkAccessConfig.h
*
* Contains required CPU Vlink HW access functionality for the Possix wrapper.
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

#ifndef INC_OsHwCpuVlinkAccessConfigk_h
#define INC_OsHwCpuVlinkAccessConfig_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OsHwCpuVlinkResource.h"



/********************************* Types *************************************/



/************************** Function Declarations ****************************/

/**
******************************************************************************
*
* Initializes a Cpu Vlink hardware access. If the range is in memory space,
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
    BOOLEAN                        bOrdered);


/**
******************************************************************************
*
* Deinitializes a Cpu Vlink hardware access. If the range is memory mapped,
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
    struct _OsHwAccessObject * const pHwAccessObject);


/**
******************************************************************************
*
* Reports the Cpu Vlink register range virtual address belonging
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
    struct _OsHwAccessObject const * const pHwAccessObject);




#ifdef __cplusplus
}
#endif


#endif /* INC_OsHwCpuVlinkAccessConfig_h */
