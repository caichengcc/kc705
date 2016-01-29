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
* @file OsHwCpuVlinkAccess.c
*
* Register, memory and fifo access functions for CPU-Vlink bus.
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


/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "OslibTime.h"
#include "OslibHwAccess.h"
#include "OsHwCpuVlinkAccessor.h"
#include "../../common/src/OsHwAccessObject.h"


/******************************* Definitions *********************************/

/* logiVLINK Register offsets */
#define VLINK_TX_FIFO_STATUS_ROFF         0x00  /**< logiVLINK Status of side channel TX FIFO */
#define VLINK_RX_FIFO_STATUS_ROFF         0x04  /**< logiVLINK Status of side channel RX FIFO */
#define VLINK_TX_FIFO_LEVEL_ROFF          0x08  /**< logiVLINK Water Level of side channel TX FIFO */
#define VLINK_RX_FIFO_LEVEL_ROFF          0x0C  /**< logiVLINK Water Level of side channel RX FIFO */
#define VLINK_INT_STATUS_ROFF             0x10  /**< logiVLINK Interrupt status register */
#define VLINK_INT_EN_ROFF                 0x14  /**< logiVLINK Interrupt enable register */
#define VLINK_LINK_STATUS                 0x18  /**< logiVLINK link status */
#define VLINK_FIFO_RX_ROFF                0x1C  /**< logiVLINK FIFO */
#define VLINK_PRBS_ERRORS_ROFF            0x20  /**< logiVLINK Error bits collected during individual test */
#define VLINK_PRBS_SAMPLES_ROFF           0x24  /**< logiVLINK Bytes collected during individual test */

/* logiVLINK Register bitmasks */
#define VLINK_MSK_RX_FIFO_EMPTY       (1<<3)  /**< logiVLINK RX FIFO empty */
#define VLINK_MSK_TX_FIFO_FULL        (1<<1)  /**< logiVLINK TX FIFO full */

#define VLINK_MSK_RX_FIFO_EMPTY_S     (1<<5)  /**< logiVLINK RX FIFO empty interrupt clear bitmask*/
#define VLINK_MSK_TX_FIFO_FULL_S      (1<<2)  /**< logiVLINK TX FIFIO full interrupt clear bitmask*/

#define VLINK_MSK_RX_FIFO_EMPTY_INT   (1<<5)  /**< logiVLINK Enable RX FIFO empty interrupt */
#define VLINK_MSK_TX_FIFO_FULL_INT    (1<<2)  /**< logiVLINK Enable TX FIFIO full interrupt */

#define VLINK_MAX_FIFO_DEPTH 510              /**< logiVLINK fifo depth */

/** logiVLINK Register access macro */
#define VLINK_RD32(base, offset) (*(OsU32 *)((OsU8*)(base) + offset))

/********************************* Types *************************************/


/************************** Function Definitions *****************************/
static inline int vlinkWaitTxFifoNotFull(OsHwCpuVlinkAccessorT const * const pHwAccessor)
{
    int ret = 0;
    /* Wait if TX FIFO is full, with timeout */
    OsU32 statusRegister;
    OsTimerT time = OsTimeGetElapsedMs(0);
    do
    {
        statusRegister = VLINK_RD32(pHwAccessor->pBaseVlink , VLINK_TX_FIFO_STATUS_ROFF);
        if(OsTimeGetElapsedMs(time) > pHwAccessor->vLinkTimeoutMs)
        {
            OsPrintfError("Vanilla timeout (TX FULL)\n\r");
            ret = -1;
            break;
        }
    }
    while(statusRegister & VLINK_MSK_TX_FIFO_FULL);
    return ret;
}

static inline int vlinkWaitRxFifoNotEmpty(OsHwCpuVlinkAccessorT const * const pHwAccessor)
{
    int ret = 0;
    /* Wait for the status "rx_fifo_not_empty", with timeout */
    OsU32 statusRegister;
    OsTimerT time = OsTimeGetElapsedMs(0);
    do
    {
        statusRegister = VLINK_RD32(pHwAccessor->pBaseVlink , VLINK_RX_FIFO_STATUS_ROFF);
        if(OsTimeGetElapsedMs(time) > pHwAccessor->vLinkTimeoutMs)
        {
            OsPrintfError("Vanilla timeout (RX FIFO not EMPTY)\n\r");
            ret = -1;
            break;
        }
    }
    while(statusRegister & VLINK_MSK_RX_FIFO_EMPTY);
    return ret;
}

static inline OsU8 vlinkRead8(OsHwCpuVlinkAccessorT const * const pHwAccessor)
{
    return (OsU8)VLINK_RD32(pHwAccessor->pBaseVlink , VLINK_FIFO_RX_ROFF);
}

static inline OsU16 vlinkRead16(OsHwCpuVlinkAccessorT const * const pHwAccessor)
{
    return (OsU16)VLINK_RD32(pHwAccessor->pBaseVlink , VLINK_FIFO_RX_ROFF);
}

static inline OsU32 vlinkRead32(OsHwCpuVlinkAccessorT const * const pHwAccessor)
{
    return (OsU32)VLINK_RD32(pHwAccessor->pBaseVlink , VLINK_FIFO_RX_ROFF);
}


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
OsCpuVlinkMemoryRegRead8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset)
{
    OsU8 * pReg;
    OsU8   value;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > offset);

    vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    value = *pReg;

    vlinkWaitRxFifoNotEmpty(pHwAccessObject->pHwAccessor);
    return vlinkRead8(pHwAccessObject->pHwAccessor);
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
OsCpuVlinkMemoryRegRead16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset)
{
    OsU16 * pReg;
    OsU16   value;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > (offset + 1));

    vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg)) & 1));
    value = *pReg;

    vlinkWaitRxFifoNotEmpty(pHwAccessObject->pHwAccessor);
    return vlinkRead16(pHwAccessObject->pHwAccessor);
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
OsCpuVlinkMemoryRegRead32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset)
{
    OsU32 * pReg;
    OsU32   value;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > (offset + 3));

    vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg)) & 3));
    value = *pReg;

    vlinkWaitRxFifoNotEmpty(pHwAccessObject->pHwAccessor);
    return vlinkRead32(pHwAccessObject->pHwAccessor);

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
OsCpuVlinkMemoryRegWrite8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8                            value)
{
    OsU8 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > offset);

    vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
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
OsCpuVlinkMemoryRegWrite16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16                           value)
{
    OsU16 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > (offset + 1));

    vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);

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
OsCpuVlinkMemoryRegWrite32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32                           value)
{
    OsU32 * pReg;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size > (offset + 3));

    vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);

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
OsCpuVlinkMemoryFifoRead8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8 *                          pBuffer,
    OsSizeT                         count)
{
    OsU8 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);

    while (count > 0 && !vLinkErr)
    {
        OsU32 cnt = count > VLINK_MAX_FIFO_DEPTH ? VLINK_MAX_FIFO_DEPTH:count;
        OsU32 countFifo = cnt;
        /* make the read requests on vanilla mem. space */
        while (countFifo > 0 && !vLinkErr)
        {
            OsU8 tmp;
            vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
            tmp = *pReg;
            --countFifo;
        }
        countFifo = cnt;
        /* read the actual data over vlink bus*/
        while (countFifo > 0 && !vLinkErr)
        {
            vLinkErr = vlinkWaitRxFifoNotEmpty(pHwAccessObject->pHwAccessor);
            *pBuffer = vlinkRead8(pHwAccessObject->pHwAccessor);
            ++pBuffer;
            --countFifo;
        }
        count -= cnt;
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
OsCpuVlinkMemoryFifoRead16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16 *                         pBuffer,
    OsSizeT                         count)
{
    OsU16 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 2 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 1));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 1));

    while (count > 0 && !vLinkErr)
    {
        OsU32 cnt = count > VLINK_MAX_FIFO_DEPTH ? VLINK_MAX_FIFO_DEPTH:count;
        OsU32 countFifo = cnt;
        /* make the read requests on vanilla mem. space */
        while (countFifo > 0 && !vLinkErr)
        {
            OsU16 tmp;
            vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
            tmp = *pReg;
            --countFifo;
        }
        countFifo = cnt;
        /* read the actual data over vlink bus*/
        while (countFifo > 0 && !vLinkErr)
        {
            vLinkErr = vlinkWaitRxFifoNotEmpty(pHwAccessObject->pHwAccessor);
            *pBuffer = vlinkRead16(pHwAccessObject->pHwAccessor);
            ++pBuffer;
            --countFifo;
        }
        count -= cnt;
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
OsCpuVlinkMemoryFifoRead32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32 *                         pBuffer,
    OsSizeT                         count)
{
    OsU32 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 4 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 3));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 3));

    while (count > 0 && !vLinkErr)
    {
        OsU32 cnt = count > VLINK_MAX_FIFO_DEPTH ? VLINK_MAX_FIFO_DEPTH:count;
        OsU32 countFifo = cnt;
        /* make the read requests on vanilla mem. space */
        while (countFifo > 0 && !vLinkErr)
        {
            OsU32 tmp;
            vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
            tmp = *pReg;
            --countFifo;
        }
        countFifo = cnt;
        /* read the actual data over vlink bus*/
        while (countFifo > 0 && !vLinkErr)
        {
            vLinkErr = vlinkWaitRxFifoNotEmpty(pHwAccessObject->pHwAccessor);
            *pBuffer = vlinkRead32(pHwAccessObject->pHwAccessor);
            ++pBuffer;
            --countFifo;
        }
        count -= cnt;
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
OsCpuVlinkMemoryFifoWrite8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8              const *       pBuffer,
    OsSizeT                         count)
{
    OsU8 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);

    while (count > 0 && !vLinkErr)
    {
        vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
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
OsCpuVlinkMemoryFifoWrite16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16             const *       pBuffer,
    OsSizeT                         count)
{
    OsU16 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 2 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 1));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 1));

    while (count > 0 && !vLinkErr)
    {
        vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
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
OsCpuVlinkMemoryFifoWrite32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32             const *       pBuffer,
    OsSizeT                         count)
{
    OsU32 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 4 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 3));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 3));

    while (count > 0 && !vLinkErr)
    {
        vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
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
OsCpuVlinkMemoryRegMultiRead8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8 *                          pBuffer,
    OsSizeT                         count)
{
    OsU8 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);

    while (count > 0 && !vLinkErr)
    {
        OsU32 cnt = count > VLINK_MAX_FIFO_DEPTH ? VLINK_MAX_FIFO_DEPTH:count;
        OsU32 countFifo = cnt;
        /* make the read requests on vanilla mem. space */
        while (countFifo > 0 && !vLinkErr)
        {
            OsU32 tmp;
            vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
            tmp = *pReg;
            ++pReg;
            --countFifo;
        }
        countFifo = cnt;
        /* read the actual data over vlink bus*/
        while (countFifo > 0 && !vLinkErr)
        {
            vLinkErr = vlinkWaitRxFifoNotEmpty(pHwAccessObject->pHwAccessor);
            *pBuffer = vlinkRead8(pHwAccessObject->pHwAccessor);
            ++pBuffer;
            --countFifo;
        }
        count -= cnt;
    }
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
OsCpuVlinkMemoryRegMultiRead16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16 *                         pBuffer,
    OsSizeT                         count)
{
    OsU16 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 2 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 1));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 1));

    while (count > 0 && !vLinkErr)
    {
        OsU32 cnt = count > VLINK_MAX_FIFO_DEPTH ? VLINK_MAX_FIFO_DEPTH:count;
        OsU32 countFifo = cnt;
        /* make the read requests on vanilla mem. space */
        while (countFifo > 0 && !vLinkErr)
        {
            OsU32 tmp;
            vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
            tmp = *pReg;
            ++pReg;
            --countFifo;
        }
        countFifo = cnt;
        /* read the actual data over vlink bus*/
        while (countFifo > 0 && !vLinkErr)
        {
            vLinkErr = vlinkWaitRxFifoNotEmpty(pHwAccessObject->pHwAccessor);
            *pBuffer = vlinkRead16(pHwAccessObject->pHwAccessor);
            ++pBuffer;
            --countFifo;
        }
        count -= cnt;
    }

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
OsCpuVlinkMemoryRegMultiRead32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32 *                         pBuffer,
    OsSizeT                         count)
{
    OsU32 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 4 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 3));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 3));

    while (count > 0 && !vLinkErr)
    {
        OsU32 cnt = count > VLINK_MAX_FIFO_DEPTH ? VLINK_MAX_FIFO_DEPTH:count;
        OsU32 countFifo = cnt;
        /* make the read requests on vanilla mem. space */
        while (countFifo > 0 && !vLinkErr)
        {
            OsU32 tmp;
            vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
            tmp = *pReg;
            ++pReg;
            --countFifo;
        }
        countFifo = cnt;
        /* read the actual data over vlink bus*/
        while (countFifo > 0 && !vLinkErr)
        {
            vLinkErr = vlinkWaitRxFifoNotEmpty(pHwAccessObject->pHwAccessor);
            *pBuffer = vlinkRead8(pHwAccessObject->pHwAccessor);
            ++pBuffer;
            --countFifo;
        }
        count -= cnt;
    }
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
OsCpuVlinkMemoryRegMultiWrite8(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU8              const *       pBuffer,
    OsSizeT                         count)
{
    OsU8 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU8 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    //OsPrintfInfo("OsCpuMemoryRegMultiWrite8 (phy 0x%x, log 0x%x ), srcBuff=0x%x, count=0x%x\n",pHwAccessObject->pHwAccessor->physBaseAddress + offset, (int)pReg, pBuffer, count);

    while (count > 0 && !vLinkErr)
    {
        vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
        *pReg = *pBuffer;
        ++pBuffer;
        ++pReg;
        --count;
    }
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
OsCpuVlinkMemoryRegMultiWrite16(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU16             const *       pBuffer,
    OsSizeT                         count)
{
    OsU16 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 2 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU16 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 1));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 1));

    while (count > 0 && !vLinkErr)
    {
        vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
        *pReg = *pBuffer;
        ++pBuffer;
        ++pReg;
        --count;
    }
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
OsCpuVlinkMemoryRegMultiWrite32(
    OsHwAccessObjectT const * const pHwAccessObject,
    OsRegOffsetT                    offset,
    OsU32             const *       pBuffer,
    OsSizeT                         count)
{
    OsU32 * pReg;
    int vLinkErr = 0;

    OS_ASSERT(pHwAccessObject->pHwAccessor->size >= (offset + 4 * count));
    OS_ASSERT(pBuffer != NULL);

    pReg = (OsU32 *)(pHwAccessObject->pHwAccessor->pBase + offset);
    OS_ASSERT(0 == (((OsUptrT)(pReg))    & 3));
    OS_ASSERT(0 == (((OsUptrT)(pBuffer)) & 3));

    while (count > 0 && !vLinkErr)
    {
        vLinkErr = vlinkWaitTxFifoNotFull(pHwAccessObject->pHwAccessor);
        *pReg = *pBuffer;
        ++pBuffer;
        ++pReg;
        --count;
    }
}


/*! \endhtmlonly - prevents doubled documentation of functions in rtf, latex doc */
