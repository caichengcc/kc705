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
* @file OslibHwAccess.h
*
* Contains relevant OS specific HW access wrappers, as well as the HW access
* functionality common to all the OSs.
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

#ifndef INC_OslibHwAccess_h
#define INC_OslibHwAccess_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibTypes.h"



/********************************* Types *************************************/

/** Register offset. */
typedef OsSizeT             OsRegOffsetT;

/** Forward declaration. */
struct _OsHwResource;

/**
 OS bus type enum
**/
typedef enum
{
    OS_BUS_TYPE__UNDEFINED, /**< Undefined or unssupported, should not be used */
    OS_BUS_TYPE__PCI,       /**< PCI register/memory access */
    OS_BUS_TYPE__USB,       /**< USB access */
    OS_BUS_TYPE__CPU,       /**< Cpu register/memory access */
    OS_BUS_TYPE__CPU_VLINK  /**< Cpu register/memory access to the remote IP core over the Vlink (vanilla bus) */

} OsBusTypeE;



/************************** Function Declarations ****************************/

/**
******************************************************************************
*
* Initializes a register range. If the range is in memory space,
* performs register range mapping to nonpaged system space.
*
* @param        pHwResource points to a HW resource descriptor.
* @param        bOrdered is TRUE if transfers cannot be optimised.
*
* @return       Pointer to a register range descriptor, or NULL on failure.
*
* @note         None.
*
******************************************************************************/
struct _OsHwAccessObject *
OsHwAccessInit(
    struct _OsHwResource const * const pHwResource,
    BOOLEAN                            bOrdered);


/**
******************************************************************************
*
* Deinitializes a register range. If the range is memory mapped,
* performs register range unmapping from nonpaged system space.
*
* @param        pHwAccessObject points to a register range descriptor.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsHwAccessDeinit(
    struct _OsHwAccessObject * const pHwAccessObject);


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
OsU8
OsRegRead8(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset);


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
OsU16
OsRegRead16(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset);


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
OsU32
OsRegRead32(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset);


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
void
OsRegWrite8(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU8                                   value);


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
void
OsRegWrite16(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU16                                  value);


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
void
OsRegWrite32(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU32                                  value);


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
void
OsRegFifoRead8(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU8                           * const pBuffer,
    OsSizeT                                count);


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
void
OsRegFifoRead16(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU16                          * const pBuffer,
    OsSizeT                                count);


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
void
OsRegFifoRead32(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU32                          * const pBuffer,
    OsSizeT                                count);


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
void
OsRegFifoWrite8(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU8                     const * const pBuffer,
    OsSizeT                                count);


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
void
OsRegFifoWrite16(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU16                    const * const pBuffer,
    OsSizeT                                count);


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
void
OsRegFifoWrite32(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU32                    const * const pBuffer,
    OsSizeT                                count);


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
void
OsRegMultiRead8(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU8                           * const pBuffer,
    OsSizeT                                count);


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
void
OsRegMultiRead16(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU16                          * const pBuffer,
    OsSizeT                                count);


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
void
OsRegMultiRead32(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU32                          * const pBuffer,
    OsSizeT                                count);


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
void
OsRegMultiWrite8(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU8                     const * const pBuffer,
    OsSizeT                                count);


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
void
OsRegMultiWrite16(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU16                    const * const pBuffer,
    OsSizeT                                count);


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
void
OsRegMultiWrite32(
    struct _OsHwAccessObject const * const pHwAccessObject,
    OsRegOffsetT                           offset,
    OsU32                    const * const pBuffer,
    OsSizeT                                count);

/**
******************************************************************************
*
* Reports the register range virtual address belonging
* to a specified OsHwAccessObject.
*
* @param        pHwAccessObject points to a register range descriptor.
*
* @return       Virtual address or NULL if it does not exist.
*
* @note         None.
*
******************************************************************************/
void *
OsHwAccessGetVirtualAddress(
    struct _OsHwAccessObject const * const pHwAccessObject);



/****************************** Include Files ********************************/
#include "OsHwAccess_inline.h"



#ifdef __cplusplus
}
#endif


#endif /* INC_OslibHwAccess_h */
