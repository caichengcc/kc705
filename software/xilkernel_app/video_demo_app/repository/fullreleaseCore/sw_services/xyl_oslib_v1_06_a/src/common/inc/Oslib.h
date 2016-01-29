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
* @file Oslib.h
*
* Includes abstracted OS functionality to be used by other,
* OS independant modules.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a HJ  30/10/09  First release \n
* 1.05.a AS  21/02/12  Added function for allocating and freeing aligned memory.
*                      All mkdir/copy operations moved from makefile to tcl.\n
* 1.06.a GP  22/05/12  Multiple bus support added. Added VLINK support. Added
*                      file support on possix. Updated endian/print support.
*
* </pre>
*
******************************************************************************/

#ifndef INC_Oslib_h
#define INC_Oslib_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibTypes.h"
#include "OslibDebug.h"
#include "OslibMemory.h"
#include "OslibDma.h"
#include "OslibHwResource.h"
#include "OslibHwAccess.h"
#include "OslibSpinLock.h"
#include "OslibTime.h"
#include "OslibFile.h"
#include "OslibMisc.h"


#define OS_LIBRARY_VER 0x00010600   /**< Xylon OS library version number */


#ifdef __cplusplus
}
#endif


#endif /* INC_Oslib_h */
