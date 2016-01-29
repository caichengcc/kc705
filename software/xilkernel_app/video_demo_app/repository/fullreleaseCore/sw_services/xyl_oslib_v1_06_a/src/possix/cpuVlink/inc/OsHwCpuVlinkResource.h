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
* @file OsHwCpuVlinkResource.h
*
* Contains the Possix wrapper for the Cpu Vlink resource type.
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

#ifndef INC_OsHwCpuVlinkResource_h
#define INC_OsHwCpuVlinkResource_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OsTypes.h"



/********************************* Types *************************************/


/**
* OsHwCpuVlinkResourceT describes resources as given on the BUS. This structure must be initialized manually
* by the OS dependant code and then passes as input argument to the function \ref OsHwAccessInit.
* Input structure to the \ref OsHwAccessInit function
**/
/** CPU BUS descriptor. Most important member is physical addresses of memory in its length */
typedef struct
{
    /** Base physical address of the targeted IP core. */
    OsU32 physBaseAddress;

    /** Number of consecutive byte addresses of the targeted IP core. */
    OsSizeT size;

    /** Base physical address of the Vlink bus IP core. */
    OsU32 physBaseAddressVlink;

    /** Number of consecutive byte addresses of the Vlink bus IP core. */
    OsSizeT sizeVlink;
} OsHwCpuVlinkResourceT;


#ifdef __cplusplus
}
#endif


#endif /* INC_OsHwCpuVlinkResource_h */
