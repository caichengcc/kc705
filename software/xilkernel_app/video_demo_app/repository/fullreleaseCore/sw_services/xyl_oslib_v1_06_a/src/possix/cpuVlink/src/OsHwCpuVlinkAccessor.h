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
* @file OsHwCpuVlinkAccessor.h
*
* Defines required CPU Vlink access structure for the Possix wrapper.
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


#ifndef INC_OsHwCpuVlinkAccessor_h
#define INC_OsHwCpuVlinkAccessor_h


/**  CPU OS HW accessor contains physical and virual addresses of mapped memory range */
typedef struct _OsHwAccessor
{
    /** Base physical address. */
    OsU32 physBaseAddress;

    /** Number of consecutive byte addresses. */
    OsSizeT size;

    /** Base virtual address that maps the base physical address. */
    OsU8 *pBase;

    /** Base physical address of Vlink. */
    OsU32 physBaseAddressVlink;

    /** Number of consecutive byte addresses of Vlink. */
    OsSizeT sizeVlink;

    /** Base virtual address that maps the base physical address of Vlink. */
    OsU8 *pBaseVlink;

    /** Timeout interval of Vlink in milliseconds */
    OsSizeT vLinkTimeoutMs;


} OsHwCpuVlinkAccessorT;

#endif /* INC_OsHwCpuVlinkAccessor_h */
