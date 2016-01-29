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
* @file OsMisc.c
*
* Possix miscellaneous inline functions.
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
#ifdef XYL_OSLIB_POSSIX_XILINX_BSP
//#include "xio.h" /* Xilinx input output header - Microblaze only*/
#include "xil_io.h"
#else
#include <stdint.h>
#include <endian.h>
#endif
#include "OslibDebug.h"
#include "OsMisc.h"
#include "OsTypes.h"


/************************** Function Definitions *****************************/


/**
******************************************************************************
*
* Performs a 32-bit endian conversion to little endian.
* This conversion happens on big endian machines only.
* On little endian machine, no conversion takes place.
*
* @param        src contains the value to be converted.
* @param        pDest is an address to receive the LE value.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsToLe32(
    OsU32   src,
    OsU32 * pDest)
{
#ifdef XYL_OSLIB_POSSIX_XILINX_BSP
    PATH_TESTED();
    //XIo_ToLittleEndian32((u32)src, (u32*)pDest);
    if(0x12 == Xil_Htonl(0x12)) /* we are big-endian */
    {
        *pDest = Xil_EndianSwap32(src);
    }
    else
    {
        *pDest = src;
    }
#else
    PATH_NOT_YET_TESTED();
    *pDest = (OsU32)htole32((uint32_t) src);
#endif
}


/**
******************************************************************************
*
* Performs a 32-bit endian conversion to big endian.
* This conversion happens on little endian machines only.
* On big endian machine, no conversion takes place.
*
* @param        src contains the value to be converted.
* @param        pDest is an address to receive the BE value.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsToBe32(
    OsU32   src,
    OsU32 * pDest)
{
#ifdef XYL_OSLIB_POSSIX_XILINX_BSP
    PATH_NOT_YET_TESTED();
    //XIo_ToBigEndian32((u32)src, (u32*)pDest);
    *pDest = Xil_Htonl(src);
#else
    PATH_NOT_YET_TESTED();
    *pDest = (OsU32)htobe32((uint32_t) src);
#endif
}

