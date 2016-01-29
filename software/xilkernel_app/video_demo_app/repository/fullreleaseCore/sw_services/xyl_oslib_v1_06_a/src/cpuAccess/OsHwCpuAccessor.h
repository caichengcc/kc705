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
* @file OsHwAccessBus.h
*
* Defines required CPU bus access structure for the Linux OS wrapper.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a HJ   30/10/09 First release \n
* 1.00.b DJ   27/05/10 HW access functions moved into header file, \n
*                      functions Init & Deinit moved into Config file\n
* </pre>
*
******************************************************************************/

//#warning "INC_OsHwCpuAccessor_h +"

#ifndef INC_OsHwCpuAccessor_h
#define INC_OsHwCpuAccessor_h

//#warning "INC_OsHwCpuAccessor_h -"

/**  CPU OS HW accessor contains physical and virual addresses of mapped memory range */
typedef struct _OsHwAccessor
{
    /** Base physical address. */
    OsU32 physBaseAddress;

    /** Number of consecutive byte addresses. */
    OsSizeT size;

    /** Base virtual address that maps the base physical address. */
    OsU8 *pBase;

} OsHwCpuAccessorT;

#endif /* INC_OsHwCpuAccessor_h */
