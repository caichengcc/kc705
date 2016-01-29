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
* @file OsFile.h
*
* Contains general system file deinitions for the Possix wrapper and branching 
* to differned file systems. OSLIB_ADD_FILE_SUPPORT needs to be defined.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a AS  10/04/12  First release \n
* </pre>
*
******************************************************************************/

#ifdef OSLIB_ADD_FILE_SUPPORT

#ifndef INC_OsFile_h
#define INC_OsFile_h

#ifdef __cplusplus
extern "C"
{
#endif


/****************************** Include Files ********************************/
#ifdef OSLIB_ADD_FATFS_SUPPORT
#include "OsFile_fatfs.h"
#else
#error "ERROR: OSLIB_ADD_FILE_SUPPORT define but no file system defined!"
#endif



/******************************* Definitions *********************************/



/********************************* Types *************************************/



#ifdef __cplusplus
}
#endif


#endif /* INC_OsFile_h */

#endif // #ifdef OSLIB_ADD_FILE_SUPPORT
