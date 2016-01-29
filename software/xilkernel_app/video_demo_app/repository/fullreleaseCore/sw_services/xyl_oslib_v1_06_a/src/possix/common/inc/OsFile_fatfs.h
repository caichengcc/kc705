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
* @file OsFile_fatfs.h
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

#ifdef OSLIB_ADD_FATFS_SUPPORT

#ifndef INC_OsFile_fatfs_h
#define INC_OsFile_fatfs_h

#ifdef __cplusplus
extern "C"
{
#endif


/****************************** Include Files ********************************/

#include "OslibMemory.h"
#include "fatfs.h"



/******************************* Definitions *********************************/

#define OsEOF                   EOF         /**< End-of-File */
#define OsFileSeekSupport       TRUE        /**< OsFileSeekSupport support */



/********************************* Types *************************************/

struct OsFileSysDesc
{
    OsU32                       index;
};


struct OsFileSysHandle
{
    FATFS                       sys;
    OsU32                       desc;
};

struct OsFileHandle
{
    FIL *                       file;
};                                 /**< Oslib file handle type*/



/************************** Function Definitions *****************************/
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
OS_INLINE
OsFileSysStatusE
OsFileInit(
    OsFileSysHandleT    *fileSysHandle,
    OsFileSysDescT      fileSysDesc)
{
    int ret = -OS_FILE_STATUS_FILE_SYS_INIT;
    struct OsFileSysHandle *fileSys;

    PATH_TESTED();
    fileSys = (struct OsFileSysHandle *)OsMemoryAllocate(sizeof(struct OsFileSysHandle), OS_MEMORY_TYPE__UNDEFINED);;
    if(NULL == fileSys)
    {
        PATH_NOT_YET_TESTED();
        return -OS_FILE_STATUS_NOT_ENOUGH_MEM;
    }

    if(f_mount(fileSysDesc.index, &(fileSys->sys)) == FR_OK)
    {
        PATH_TESTED();
        fileSys->desc = fileSysDesc.index;
        *fileSysHandle = (OsFileSysHandleT)fileSys;
        ret = OS_FILE_STATUS_SUCCESS;
    }
    else
    {
        PATH_NOT_YET_TESTED();
        OsMemoryFree(fileSys);
        ret = -OS_FILE_STATUS_FILE_SYS_INIT;
    }

    return ret;
}

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
OS_INLINE
OsFileSysStatusE
OsFileDeinit(
    OsFileSysHandleT    *fileSysHandle)
{
    int ret = -OS_FILE_STATUS_FILE_SYS_INIT;
    struct OsFileSysHandle *fileSys = (struct OsFileSysHandle *)*fileSysHandle;

    PATH_NOT_YET_TESTED();
    if(f_mount(fileSys->desc, NULL) == FR_OK)
    {
        PATH_NOT_YET_TESTED();
        OsMemoryFree(fileSys);
        ret = OS_FILE_STATUS_SUCCESS;
    }

    return ret;
}

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
OS_INLINE
OsFileSysStatusE
OsFileOpen(
    OsFileSysHandleT    fileSysHandle,
    OsFileHandleT       *fileHandle,
    OsCharT const       *fileName,
    OsFileSysOpenModeE  mode)
{
    int fopenRet = -1;
    int flag = 0;
    int ret = -OS_FILE_STATUS_INVALID_STATE;
    struct OsFileHandle *file;

#if !_FS_READONLY
    int fstatRet = -1;
    FILINFO fno;

    PATH_TESTED();
    fstatRet = f_stat(fileName, &fno);
    if(fstatRet != FR_OK)
    {
        PATH_TESTED();
        flag = FA_CREATE_NEW;
        ret = -OS_FILE_STATUS_FILE_NOT_FOUND;
    }
    PATH_TESTED();
    if (mode & OS_FILE_OPEN_MODE_WRITE)
    {
        PATH_TESTED();
        flag |= FA_WRITE;
    }
#else
    PATH_TESTED();
    if ((mode & OS_FILE_OPEN_MODE_CREAT) || (mode & OS_FILE_OPEN_MODE_WRITE))
    {
        return -OS_FILE_STATUS_NOT_SUPPORTED;
    }
#endif
    PATH_TESTED();
    if (mode & OS_FILE_OPEN_MODE_READ)
    {
        PATH_TESTED();
        flag |= FA_READ;
    }

    if (NULL != (file = (struct OsFileHandle *)OsMalloc(sizeof(struct OsFileHandle))))
    {
        PATH_TESTED();
        if (NULL != (file->file = (FIL *)OsMalloc(sizeof(FIL))))
        {
            PATH_TESTED();
            if ((fopenRet = f_open(file->file, (fileName), flag)) != FR_OK)
            {
                PATH_TESTED();
                OsMemoryFree(file);
                file = NULL;
            }
            else
            {
                PATH_TESTED();
                *fileHandle = (OsFileHandleT)file;
                ret = OS_FILE_STATUS_SUCCESS;
            }
         }
         else
         {
            PATH_NOT_YET_TESTED();
            OsMemoryFree(file);
             ret = -OS_FILE_STATUS_NOT_ENOUGH_MEM;
         }
     }
     else
     {
        PATH_NOT_YET_TESTED();
         ret = -OS_FILE_STATUS_NOT_ENOUGH_MEM;
     }

     return ret;
}

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
OS_INLINE
OsFileSysStatusE
OsFileClose(
    OsFileHandleT       *fileHandle)
{
    int ret;
    struct OsFileHandle *file = (struct OsFileHandle *)*fileHandle;

    PATH_TESTED();
    ret = f_close(file->file);
    OsMemoryFree(file->file);
    OsMemoryFree(file);
    file = NULL;

    return OS_FILE_STATUS_SUCCESS;
}

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
OS_INLINE
OsFileSysStatusE
OsFileRead(
    OsFileHandleT       fileHandle,
    OsSizeT             size,
    OsU32               count,
    void                *ptr,
    OsU32               *read)
{
    int ret = -OS_FILE_STATUS_INVALID_STATE;
    UINT tmp;
    struct OsFileHandle *file = (struct OsFileHandle *)fileHandle;

    PATH_TESTED();
    if(f_read(file->file, (void*)(ptr), (size*count), &tmp) == FR_OK)
    {
        PATH_TESTED();
        *read = (OsU32)(tmp/size);
        ret = OS_FILE_STATUS_SUCCESS;
    }
    else
    {
        PATH_NOT_YET_TESTED();
        ret = -OS_FILE_STATUS_INVALID_STATE;
    }

    return ret;
}

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
OS_INLINE
OsFileSysStatusE
OsFileWrite(
    OsFileHandleT       fileHandle,
    OsSizeT             size,
    OsU32               count,
    const void          *ptr,
    OsU32               *written)
{
    int ret = -OS_FILE_STATUS_INVALID_STATE;
    UINT tmp;
    struct OsFileHandle *file = (struct OsFileHandle *)fileHandle;

    PATH_TESTED();
    if(f_write(file->file, ptr, (size*count), &tmp) == FR_OK)
    {
        PATH_TESTED();
        *written = (OsU32)(tmp/size);
        ret = OS_FILE_STATUS_SUCCESS;
    }
    else
    {
        PATH_NOT_YET_TESTED();
        ret = -OS_FILE_STATUS_INVALID_STATE;
    }

    return ret;
}

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
OS_INLINE
OsFileSysStatusE
OsFileSeek(
    OsFileHandleT       fileHandle,
    OsU32               offset,
    OsFileSysSeekE      origin)
{
    int ret = -OS_FILE_STATUS_INVALID_STATE;
    struct OsFileHandle *file = (struct OsFileHandle *)fileHandle;

    PATH_NOT_YET_TESTED();
    if(OS_FILE_SEEK_SET == origin)  // supports only OS_FILE_SEEK_SET
    {
        PATH_NOT_YET_TESTED();
       ret = f_lseek(file->file, (offset));
       ret = OS_FILE_STATUS_SUCCESS;
    }
    else
    {
        PATH_NOT_YET_TESTED();
        ret = -OS_FILE_STATUS_NOT_SUPPORTED;
    }

    return ret;
}

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
OS_INLINE
OsFileSysStatusE
OsFileTell(
    OsFileHandleT       fileHandle,
    OsU32               *offset)
{
    struct OsFileHandle *file = (struct OsFileHandle *)fileHandle;

    PATH_NOT_YET_TESTED();
    *offset = f_tell(file->file);

    return OS_FILE_STATUS_SUCCESS;
}

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
OS_INLINE
OsBoolT
OsFileEof(
    OsFileHandleT       fileHandle)
{
    struct OsFileHandle *file = (struct OsFileHandle *)fileHandle;

    PATH_NOT_YET_TESTED();

    return f_eof(file->file);
}


#ifdef __cplusplus
}
#endif


#endif /* INC_OsFile_fatfs_h */

#endif // #ifdef OSLIB_ADD_FATFS_SUPPORT

#endif // #ifdef OSLIB_ADD_FILE_SUPPORT
