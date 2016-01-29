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
* @file OsHwCpuAccessConfig.h
*
* Contains required CPU HW access functionality for the Possix wrapper.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a GP  30/10/09  First release \n
* </pre>
*
******************************************************************************/

#ifndef INC_OsHwCpuAccessConfig_h
#define INC_OsHwCpuAccessConfig_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OsTypes.h"
#include "../../../cpuAccess/OsHwCpuResource.h"


/********************************* Types *************************************/


/************************** Function Declarations ****************************/

/**
******************************************************************************
*
* Initializes a Cpu hardware access. If the range is in memory space,
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
    BOOLEAN                        bOrdered);


/**
******************************************************************************
*
* Deinitializes a Cpu hardware access. If the range is memory mapped,
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
    struct _OsHwAccessObject * const pHwAccessObject);


/**
******************************************************************************
*
* Reports the Cpu register range virtual address belonging
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
    struct _OsHwAccessObject const * const pHwAccessObject);



#ifdef __cplusplus
}
#endif


#endif /* INC_OsHwCpuAccessConfig_h */
