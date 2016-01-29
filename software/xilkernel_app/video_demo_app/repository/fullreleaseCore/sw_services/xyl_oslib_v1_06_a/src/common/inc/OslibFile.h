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
* @file OslibFile.h
*
* Contains relevant OS specific file system wrappers, as well as the file 
* system functionality common to all the OSs. OSLIB_ADD_FILE_SUPPORT needs to 
* be defined.
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

#ifndef INC_OslibFile_h
#define INC_OslibFile_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibTypes.h"
#include "OsDebug.h"



/******************************* Definitions *********************************/

/** Forward declaration. */
typedef struct OsFileSysHandle  *OsFileSysHandleT;  /**< OsFileSysHandleT */

typedef struct OsFileSysDesc    OsFileSysDescT;     /**< OsFileSysDescT */

typedef struct OsFileHandle     *OsFileHandleT;     /**< OsFileHandleT */


/**
* OsFileSysStatusE lists different status types for return values of some of 
* Xylon OS library file system functions.
* \brief Return value of functions \ref OsFileInit, \ref OsFileDeinit, 
* \ref OsFileOpen, \ref OsFileClose, \ref OsFileRead, \ref OsFileWrite, 
* \ref OsFileSeek and \ref OsFileTell.
*/
typedef enum
{
    OS_FILE_STATUS_SUCCESS = 0,     /**< Success */
    OS_FILE_STATUS_NOT_ENOUGH_MEM,  /**< Error: not enough memory for memory 
                                        allocation */
    OS_FILE_STATUS_NOT_SUPPORTED,   /**< Error: not supported by the file 
                                        system */
    OS_FILE_STATUS_FILE_SYS_INIT,   /**< Error: file system initialization 
                                        failed */
    OS_FILE_STATUS_NO_FILE_SYS,     /**< Error: no file system present; 
                                        function OsFileInit needs to be 
                                        successfully called before any other 
                                        file system functions are called */
    OS_FILE_STATUS_HANDLE_INVALID,  /**< Error: file handler invalid; function 
                                        OsFileOpen need to be called 
                                        successfully and the file handle from 
                                        that function needs to be used by other 
                                        Oslib file system functions */
    OS_FILE_STATUS_FILE_NOT_FOUND,  /**< Error: file not found */
    OS_FILE_STATUS_QUOTA_EXCEEDED,  /**< Error: quota exceeded; limitation of 
                                        file system (e.g. number of opened 
                                        files) */
    OS_FILE_STATUS_INVALID_STATE   /**< Error: invalid state (miscellaneous) */
}OsFileSysStatusE;

/**
* OsFileSysOpenModeE lists different input arguments types for mode in open 
* function.
* \brief Input argument to \ref OsFileOpen.
*/
typedef enum
{
    OS_FILE_OPEN_MODE_READ  = 0x1,  /**< Open file in read mode */
    OS_FILE_OPEN_MODE_WRITE = 0x2,  /**< Open file in write mode */
    OS_FILE_OPEN_MODE_CREAT = 0x4   /**< Creat new if file does not exist */
}OsFileSysOpenModeE;

/**
* OsFileSysSeekE lists different input arguments types for origin in seek 
* function.
* \brief Input argument to \ref OsFileSeek.
*/
typedef enum
{
    OS_FILE_SEEK_SET,   /**< Beginning of file */
    OS_FILE_SEEK_CUR,   /**< Current position of the file pointer */
    OS_FILE_SEEK_END    /**< End of file */
}OsFileSysSeekE;



/************************** Function Declarations ****************************/

/**
******************************************************************************
*
* Initializes file system support.
*
* @param[out]       fileSysHandle receives the file system handle type 
*                   OsFileSysHandleT if the function call is successful.
* @param[in]        OsFileSysDescT is file system descriptor.
*
* @return           OsFileSysStatusE status.
*
* @note             This function must be called before any other file system 
*                   functions are called.
*
******************************************************************************/
OsFileSysStatusE
OsFileInit(
    OsFileSysHandleT    *fileSysHandle,
    OsFileSysDescT      fileSysDesc);

/**
******************************************************************************
*
* Deinitializes file system support.
*
* @param[in]        fileSysHandle is the file system handle type OsFileSysHandleT 
*                   to be closed.
*
* @return           None.
*
* @note             OsFileSysStatusE status.
*
******************************************************************************/
OsFileSysStatusE
OsFileDeinit(
    OsFileSysHandleT    *fileSysHandle);

/**
******************************************************************************
*
* Opens the file which name is specified by the parameter fileName in a mode 
* specified by the parameter mode and if successful creates a handle for the 
* opened file which is then used by other Oslib file system functions.
*
* @param[in]        fileSysHandle is the file system handle type 
*                   OsFileSysHandleT received by function OsFileInit.
* @param[out]       fileHandle receives the file handle type OsFileHandleT if 
*                   the function call is successful.
* @param[in]        fileName contains the name of the file to be opened. This 
*                   paramenter must follow the file name specifications of the 
*                   running environment and can include a path if the system 
*                   supports it.
* @param[in]        mode specifies the desired mode OsFileSysOpenModeE for the 
*                   opened file.
*
* @return           OsFileSysStatusE status.
*
* @note             None.
*
******************************************************************************/
OsFileSysStatusE
OsFileOpen(
    OsFileSysHandleT    fileSysHandle,
    OsFileHandleT       *fileHandle,
    OsCharT const       *fileName,
    OsFileSysOpenModeE  mode);

/**
******************************************************************************
*
* Closes the file that is associated by the file handle and disassociates it.
*
* @param[in]        fileHandle file handle of the file to be closed.
*
* @return           OsFileSysStatusE status.
*
* @note             None.
*
******************************************************************************/
OsFileSysStatusE
OsFileClose(
    OsFileHandleT       *fileHandle);

/**
******************************************************************************
* Reads an array of count elements, each one with a size of size bytes, from 
* the file associated by the file handle and stores them in the block of memory 
* specified by ptr. The postion indicator of the stream is advanced by the 
* total amount of bytes read. The total amount of bytes read if successful is 
* (size * count).
*
* @param[in,out]    fileHandle file handle of the file to be read.
* @param[in]        size is size in bytes of each element to be read.
* @param[in]        count is the number of elements, each one with a size of 
*                   size bytes to be read.
* @param[out]       ptr is a pointer to a block of memory with a minimum size 
*                   of (size*count) bytes, if successful the size of the block 
*                   is (size*read) bytes.
* @param[out]       read is the number of elements read, each one with a size 
*                   of size bytes.
*
* @return           OsFileSysStatusE status.
*
* @note             None.
*
******************************************************************************/
OsFileSysStatusE
OsFileRead(
    OsFileHandleT       fileHandle,
    OsSizeT             size,
    OsU32               count,
    void                *ptr,
    OsU32               *read);

/**
******************************************************************************
* Writes an array of count elements, each one with a size of size bytes, from 
* the block of memory pointed by ptr to the current position in the file 
* associated by the file handle. The postion indicator of the file handle is 
* advanced by the total number of bytes written. The total amount of bytes 
* written is (size * count).
*
* @param[in,out]    fileHandle file handle of the file to be written.
* @param[in]        size is size in bytes of each element to be written.
* @param[in]        count is the number of elements, each one with a size of 
*                   size bytes to be written.
* @param[in]        ptr is a pointer to the array of elements to be written.
* @param[out]       written is the number of elements actually written, each 
*                   one with a size of size bytes.
*
* @return           OsFileSysStatusE status.
*
* @note             None.
*
******************************************************************************/
OsFileSysStatusE
OsFileWrite(
    OsFileHandleT       fileHandle,
    OsSizeT             size,
    OsU32               count,
    const void          *ptr,
    OsU32               *written);

/**
******************************************************************************
* Sets the position indicator associated with the file handle to a new position 
* defined by adding offset to a reference position specified by origin.
*
* @param[in,out]    fileHandle file handle of the desired file.
* @param[in]        offset is number of bytes to offset from origin.
* @param[in]        origin is the position from where the offset is added. It 
*                   is specified by one of the following constants defined by 
*                   OsFileSysSeekE.
*
* @return           OsFileSysStatusE status.
*
* @note             None.
*
******************************************************************************/
OsFileSysStatusE
OsFileSeek(
    OsFileHandleT       fileHandle,
    OsU32               offset,
    OsFileSysSeekE      origin);

/**
******************************************************************************
* Gets the current position in the file associated by the file handle. 
* Parameter offset returns the current value of the position indicator of the 
* file.
*
* @param[in,out]    fileHandle file handle of the desired file.
* @param[out]       offset if successful is the current value of the position 
*                   indicator.
*
* @return           OsFileSysStatusE status.
*
* @note             None.
*
******************************************************************************/
OsFileSysStatusE
OsFileTell(
    OsFileHandleT       fileHandle,
    OsU32               *offset);

/**
******************************************************************************
* Checks whether the End-of-File indicator associated with the file handle is 
* set, returning a value different from zero if it is. This indicator is 
* generally set by a previous operation on the file handle that reached the 
* End-of-File. Further operations on the stream once the End-of-File has been 
* reached will fail until another function successfully sets the indicator to a 
* new value.

*
* @param[in]        fileHandle file handle of the desired file.
*
* @return           TRUE if true,
*                   FALSE if false.
*
* @note             None.
*
******************************************************************************/
OsBoolT
OsFileEof(
    OsFileHandleT       fileHandle);

/****************************** Include Files ********************************/
#include "OsFile.h"

/**
* Note: define OsEOF (End-of-File) and OsFileSeekSupport (OsFileSeekSupport 
* support) in file OsFile.h of file/files contained by OsFile.h.
*/


#ifdef __cplusplus
}
#endif


#endif /* INC_OslibFile_h */

#endif // #ifdef OSLIB_ADD_FILE_SUPPORT
