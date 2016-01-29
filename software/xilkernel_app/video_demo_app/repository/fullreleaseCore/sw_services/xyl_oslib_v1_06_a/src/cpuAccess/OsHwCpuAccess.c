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
* @file OsHwAccessBus.c
*
* Declares required CPU bus access functionality for the Linux OS wrapper.
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


/****************************** Include Files ********************************/
//#include <linux/slab.h> // FIX not available outside linux
#include "OslibDebug.h"
#include "OslibHwAccess.h"
#include "OsHwCpuAccessor.h"
#include "OsHwAccessObject.h"



/******************************* Definitions *********************************/



/************************** Function Definitions *****************************/

/**
******************************************************************************
*
* Reads a byte from the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a register offset in bytes.
*
* @return       Byte read from the specified address.
*
* @note         None.
*
******************************************************************************/
static OsU8
OsCpuMemoryRegRead8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset)
{
    OsU8 * pReg;
    OsU8   value;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > offset);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    value = *pReg;

    return value;
}


/**
******************************************************************************
*
* Reads a halfword from the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a register offset in bytes.
*
* @return       Halfword read from the specified address.
*
* @note         None.
*
******************************************************************************/
static OsU16
OsCpuMemoryRegRead16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset)
{
    OsU16 * pReg;
    OsU16   value;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > (offset + 1));

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg)) & 1));
    value = *pReg;

    return value;
}


/**
******************************************************************************
*
* Reads a word from the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a register offset in bytes.
*
* @return       Word read from the specified address.
*
* @note         None.
*
******************************************************************************/
static OsU32
OsCpuMemoryRegRead32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset)
{
    OsU32 * pReg;
    OsU32   value;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > (offset + 3));

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg)) & 3));
    value = *pReg;

    return value;
}


/**
******************************************************************************
*
* Writes a byte to the specified register.
*
* @param        HwAccess points to a hardware access descriptor.
* @param        offset is a register offset in bytes.
* @param        value specifies a byte to be written.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
static void
OsCpuMemoryRegWrite8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8                            value)
{
    OsU8 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > offset);
    //OsPrintfInfo("OsCpuMemoryRegWrite8 (phy 0x%x, log 0x%x ), value=0x%x",pHwAccessObject->pHwAccessor->physBaseAddress + offset, (int)pReg, value);
    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    *pReg = value;
}


/**
******************************************************************************
*
* Writes a halfword to the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a register offset in bytes.
* @param        value specifies a halfword to be written.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
static void
OsCpuMemoryRegWrite16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16                           value)
{
    OsU16 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > (offset + 1));

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg)) & 1));
    *pReg = value;
}


/**
******************************************************************************
*
* Writes a word to the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a register offset in bytes.
* @param        value specifies a word to be written.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
static void
OsCpuMemoryRegWrite32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32                           value)
{
    OsU32 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > (offset + 3));

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    //OsPrintfInfo("OsCpuMemoryRegWrite32 (phy 0x%x, log 0x%x ), value=0x%x\n",pHwAccessObject->pHwAccessor->physBaseAddress + offset, (int)pReg, value);
    OS_ASSERT(0 == (((OsUptrT)(pReg)) & 3));
    *pReg = value;
}


/**
******************************************************************************
*
* Reads a number of bytes from the specified FIFO over register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer to receive the bytes.
* @param        count specifies the number of bytes.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
static void
OsCpuMemoryFifoRead8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8 *                          pBuffer,
    OsSizeT                         count)
{
    OsU8 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);

    while (count > 0)
    {
        *pBuffer = *pReg;
        ++pBuffer;
        --count;
    }
}


/**
******************************************************************************
*
* Reads a number of halfwords from the specified FIFO over register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer to receive the halfwords.
* @param        count specifies the number of halfwords.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
static void
OsCpuMemoryFifoRead16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16 *                         pBuffer,
    OsSizeT                         count)
{
    OsU16 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 2 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 1));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 1));

    while (count > 0)
    {
        *pBuffer = *pReg;
        ++pBuffer;
        --count;
    }
}


/**
******************************************************************************
*
* Reads a number of words from the specified FIFO over register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer to receive the words.
* @param        count specifies the number of words.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
static void
OsCpuMemoryFifoRead32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32 *                         pBuffer,
    OsSizeT                         count)
{
    OsU32 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 4 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 3));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 3));

    while (count > 0)
    {
        *pBuffer = *pReg;
        ++pBuffer;
        --count;
    }
}


/**
******************************************************************************
*
* Writes a number of bytes to the specified FIFO over register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer containing bytes to be written.
* @param        count specifies the number of bytes.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
static void
OsCpuMemoryFifoWrite8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8              const *       pBuffer,
    OsSizeT                         count)
{
    OsU8 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);

    while (count > 0)
    {
        *pReg = *pBuffer;
        ++pBuffer;
        --count;
    }
}


/**
******************************************************************************
*
* Writes a number of halfwords to the specified FIFO over register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer containing halfwords to be written.
* @param        count specifies the number of halfwords.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
static void
OsCpuMemoryFifoWrite16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16             const *       pBuffer,
    OsSizeT                         count)
{
    OsU16 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 2 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 1));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 1));

    while (count > 0)
    {
        *pReg = *pBuffer;
        ++pBuffer;
        --count;
    }
}


/**
******************************************************************************
*
* Writes a number of words to the specified FIFO over register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer containing words to be written.
* @param        count specifies the number of words.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
static void
OsCpuMemoryFifoWrite32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32             const *       pBuffer,
    OsSizeT                         count)
{
    OsU32 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 4 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 3));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 3));

    while (count > 0)
    {
        *pReg = *pBuffer;
        ++pBuffer;
        --count;
    }
}


/**
******************************************************************************
*
* Reads a number of bytes starting from the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer to receive the bytes.
* @param        count specifies the number of bytes.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
static void
OsCpuMemoryRegMultiRead8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8 *                          pBuffer,
    OsSizeT                         count)
{
    OsU8 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);

#if 0
    while (count > 0)
    {
        *pBuffer = *pReg;
        ++pBuffer;
        ++pReg;
        --count;
    }
#else
    memcpy(pBuffer, pReg, count);
#endif
}


/**
******************************************************************************
*
* Reads a number of halfwords starting from the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer to receive the halfwords.
* @param        count specifies the number of halfwords.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
static void
OsCpuMemoryRegMultiRead16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16 *                         pBuffer,
    OsSizeT                         count)
{
    OsU16 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 2 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 1));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 1));

#if 0
    while (count > 0)
    {
        *pBuffer = *pReg;
        ++pBuffer;
        ++pReg;
        --count;
    }
#else
    memcpy(pBuffer, pReg, (count*2));
#endif
}


/**
******************************************************************************
*
* Reads a number of words starting from the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer to receive the words.
* @param        count specifies the number of words.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
static void
OsCpuMemoryRegMultiRead32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32 *                         pBuffer,
    OsSizeT                         count)
{
    OsU32 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 4 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 3));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 3));

#if 0
    while (count > 0)
    {
        *pBuffer = *pReg;
        ++pBuffer;
        ++pReg;
        --count;
    }
#else
    memcpy(pBuffer, pReg, (count*4));
#endif
}


/**
******************************************************************************
*
* Writes a number of bytes starting from the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer containing bytes to be written.
* @param        count specifies the number of bytes.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
static void
OsCpuMemoryRegMultiWrite8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8              const *       pBuffer,
    OsSizeT                         count)
{
    OsU8 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    //OsPrintfInfo("OsCpuMemoryRegMultiWrite8 (phy 0x%x, log 0x%x ), srcBuff=0x%x, count=0x%x\n",pHwAccessObject->pHwAccessor->physBaseAddress + offset, (int)pReg, pBuffer, count);

#if 0
    while (count > 0)
    {
        *pReg = *pBuffer;
        ++pBuffer;
        ++pReg;
        --count;
    }
#else
    memcpy(pReg, pBuffer, count);
#endif
}


/**
******************************************************************************
*
* Writes a number of halfwords starting from the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer containing halfwords to be written.
* @param        count specifies the number of halfwords.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
static void
OsCpuMemoryRegMultiWrite16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16             const *       pBuffer,
    OsSizeT                         count)
{
    OsU16 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 2 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 1));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 1));

#if 0
    while (count > 0)
    {
        *pReg = *pBuffer;
        ++pBuffer;
        ++pReg;
        --count;
    }
#else
    memcpy(pReg, pBuffer, (count*2));
#endif
}


/**
******************************************************************************
*
* Writes a number of words starting from the specified register.
*
* @param        pHwAccessObject points to a hardware access descriptor.
* @param        offset is a start register offset in bytes.
* @param        pBuffer is a buffer containing words to be written.
* @param        count specifies the number of words.
*
* @return       None.
*
* @note         Register offset gets incremented.
*
******************************************************************************/
static void
OsCpuMemoryRegMultiWrite32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32             const *       pBuffer,
    OsSizeT                         count)
{
    OsU32 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 4 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 3));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 3));

#if 0
    while (count > 0)
    {
        *pReg = *pBuffer;
        ++pBuffer;
        ++pReg;
        --count;
    }
#else
    memcpy(pReg, pBuffer, (count*4));
#endif
}


/*! \endhtmlonly - prevents doubled documentation of functions in rtf, latex doc */
