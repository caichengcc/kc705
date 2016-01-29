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
* @file OsHwAccess_inline.h
*
* Possix HW access inline functions.
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

#ifndef INC_OsHwAccess_inline_h
#define INC_OsHwAccess_inline_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "../src/OsHwAccessObject.h"



/************************** Function Definitions *****************************/

/**
******************************************************************************
*
* Reads a byte from the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a register/port offset in bytes.
*
* @return       Byte read from the specified address.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
OsU8
OsRegRead8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfRead8 != NULL);
    return pHwAccessObject->pfRead8(pHwAccessObject, offset);
}


/**
******************************************************************************
*
* Reads a halfword from the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a register/port offset in bytes.
*
* @return       Halfword read from the specified address.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
OsU16
OsRegRead16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfRead16 != NULL);
    return pHwAccessObject->pfRead16(pHwAccessObject, offset);
}


/**
******************************************************************************
*
* Reads a word from the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a register/port offset in bytes.
*
* @return       Word read from the specified address.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
OsU32
OsRegRead32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfRead32 != NULL);
    return pHwAccessObject->pfRead32(pHwAccessObject, offset);
}


/**
******************************************************************************
*
* Writes a byte to the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a register/port offset in bytes.
* @param        value specifies a byte to be written.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsRegWrite8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8                            value)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfWrite8 != NULL);
    pHwAccessObject->pfWrite8(pHwAccessObject, offset, value);
}


/**
******************************************************************************
*
* Writes a halfword to the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a register/port offset in bytes.
* @param        value specifies a halfword to be written.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsRegWrite16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16                           value)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfWrite16 != NULL);
    pHwAccessObject->pfWrite16(pHwAccessObject, offset, value);
}


/**
******************************************************************************
*
* Writes a word to the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a register/port offset in bytes.
* @param        value specifies a word to be written.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsRegWrite32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32                           value)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfWrite32 != NULL);
    pHwAccessObject->pfWrite32(pHwAccessObject, offset, value);
}


/**
******************************************************************************
*
* Reads a number of bytes from the specified FIFO address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a FIFO offset in bytes.
* @param        pBuffer is a buffer to receive the bytes.
* @param        count specifies the number of bytes.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsRegFifoRead8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8                    * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfFifoRead8 != NULL);
    pHwAccessObject->pfFifoRead8(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Reads a number of halfwords from the specified FIFO address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a FIFO offset in bytes.
* @param        pBuffer is a buffer to receive the halfwords.
* @param        count specifies the number of halfwords.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsRegFifoRead16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16                   * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfFifoRead16 != NULL);
    pHwAccessObject->pfFifoRead16(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Reads a number of words from the specified FIFO address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a FIFO offset in bytes.
* @param        pBuffer is a buffer to receive the words.
* @param        count specifies the number of words.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsRegFifoRead32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32                   * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfFifoRead32 != NULL);
    pHwAccessObject->pfFifoRead32(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Writes a number of bytes to the specified FIFO address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a FIFO offset in bytes.
* @param        pBuffer is a buffer containing bytes to be written.
* @param        count specifies the number of bytes.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsRegFifoWrite8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8              const * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfFifoWrite8 != NULL);
    pHwAccessObject->pfFifoWrite8(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Writes a number of halfwords to the specified FIFO address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a FIFO offset in bytes.
* @param        pBuffer is a buffer containing halfwords to be written.
* @param        count specifies the number of halfwords.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsRegFifoWrite16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16             const * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfFifoWrite16 != NULL);
    pHwAccessObject->pfFifoWrite16(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Writes a number of words to the specified FIFO address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a FIFO offset in bytes.
* @param        pBuffer is a buffer containing words to be written.
* @param        count specifies the number of words.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsRegFifoWrite32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32             const * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfFifoWrite32 != NULL);
    pHwAccessObject->pfFifoWrite32(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Reads a number of bytes starting from the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a start register/port offset in bytes.
* @param        pBuffer is a buffer to receive the bytes.
* @param        count specifies the number of bytes.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
OS_INLINE
void
OsRegMultiRead8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8                    * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfMultiRead8 != NULL);
    pHwAccessObject->pfMultiRead8(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Reads a number of halfwords starting from the specified register/port
* address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a start register/port offset in bytes.
* @param        pBuffer is a buffer to receive the halfwords.
* @param        count specifies the number of halfwords.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
OS_INLINE
void
OsRegMultiRead16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16                   * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfMultiRead16 != NULL);
    pHwAccessObject->pfMultiRead16(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Reads a number of words starting from the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a start register/port offset in bytes.
* @param        pBuffer is a buffer to receive the words.
* @param        count specifies the number of words.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
OS_INLINE
void
OsRegMultiRead32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32                   * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfMultiRead32 != NULL);
    pHwAccessObject->pfMultiRead32(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Writes a number of bytes starting from the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a start register/port offset in bytes.
* @param        pBuffer is a buffer containing bytes to be written.
* @param        count specifies the number of bytes.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
OS_INLINE
void
OsRegMultiWrite8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8              const * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfMultiWrite8 != NULL);
    pHwAccessObject->pfMultiWrite8(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Writes a number of halfwords starting from the specified register/port
* address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a start register/port offset in bytes.
* @param        pBuffer is a buffer containing halfwords to be written.
* @param        count specifies the number of halfwords.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
OS_INLINE
void
OsRegMultiWrite16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16             const * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfMultiWrite16 != NULL);
    pHwAccessObject->pfMultiWrite16(pHwAccessObject, offset, pBuffer, count);
}


/**
******************************************************************************
*
* Writes a number of words starting from the specified register/port address.
*
* @param        pHwAccessObject points to a register range descriptor.
* @param        offset is a start register/port offset in bytes.
* @param        pBuffer is a buffer containing words to be written.
* @param        count specifies the number of words.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
OS_INLINE
void
OsRegMultiWrite32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32             const * const pBuffer,
    OsSizeT                         count)
{
    PATH_TESTED();
    OS_ASSERT(pHwAccessObject != NULL);
    OS_ASSERT(pHwAccessObject->pfMultiWrite32 != NULL);
    pHwAccessObject->pfMultiWrite32(pHwAccessObject, offset, pBuffer, count);
}



#ifdef __cplusplus
}
#endif


#endif /* INC_OsHwAccess_inline_h */
