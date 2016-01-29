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
* @file OsHwCpuResource.h
*
* Contains the wrapper Cpu HW resource type.
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
//#warning "INC_OsHwCpuResource_h +"

#ifndef INC_OsHwCpuResource_h
#define INC_OsHwCpuResource_h

//#warning "INC_OsHwCpuResource_h -"

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OsTypes.h"



/********************************* Types *************************************/


/**
* OsHwCpuResourceT describes resources as given on the BUS. This structure must be initialized manually
* by the OS dependant code and then passes as input argument to the function \ref OsHwAccessInit.
* Input structure to the \ref OsHwAccessInit function
**/
/** CPU BUS descriptor. Most important member is physical addresses of memory in its length */
typedef struct _OsHwCpuResource
{
    /** Base physical address. */
    OsU32 physBaseAddress;

    /** Number of consecutive byte addresses. */
    OsSizeT size;

} OsHwCpuResourceT;



#ifdef __cplusplus
}
#endif


#endif /* INC_OsHwCpuResource_h */
