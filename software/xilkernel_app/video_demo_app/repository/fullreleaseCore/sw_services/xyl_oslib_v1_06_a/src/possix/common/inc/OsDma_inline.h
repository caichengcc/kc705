/**
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2011 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
******************************************************************************/

/**
*
* @file OsDma_inline.h
*
* Windows CE OS DMA inline functions.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a HJ  22/04/11  First release \n
* </pre>
*
******************************************************************************/

#ifndef INC_OsDma_inline_h
#define INC_OsDma_inline_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "OsDma.h"



/************************** Function Definitions *****************************/


/**
******************************************************************************
*
* Obtains a DMA common buffer descriptor.
*
* @param        pDmaCommonBuffer is a DMA common buffer object pointer.
* @param        pDmaCommonBufferDescr is a pointer to the DMA common buffer
*               descriptor to be filled out.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsDmaCommonBufferGetParams(
    OsDmaCommonBufferT      * pDmaCommonBuffer,
    OsDmaCommonBufferDescrT * pDmaCommonBufferDescr)
{
    // Dummy implementation.
    PATH_NOT_YET_TESTED();
    OS_ASSERT(pDmaCommonBuffer != NULL);
    OS_ASSERT(pDmaCommonBufferDescr != NULL);
    pDmaCommonBufferDescr->logicalAddress = 0;
    pDmaCommonBufferDescr->pVirtual       = NULL;
    pDmaCommonBufferDescr->size           = 0;
}


/**
******************************************************************************
*
* Prepares the system for a DMA in operation (cache handling...).
*
* @param        pDmaCommonBuffer is a DMA common buffer object pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsDmaInPrepare(
    OsDmaCommonBufferT * pDmaCommonBuffer)
{
    // Dummy implementation.
    PATH_NOT_YET_TESTED();
    OS_ASSERT(pDmaCommonBuffer != NULL);
}


/**
******************************************************************************
*
* Finalizes the last DMA in operation (cache handling...).
*
* @param        pDmaCommonBuffer is a DMA common buffer object pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsDmaInDone(
    OsDmaCommonBufferT * pDmaCommonBuffer)
{
    // Dummy implementation.
    PATH_NOT_YET_TESTED();
    OS_ASSERT(pDmaCommonBuffer != NULL);
}


/**
******************************************************************************
*
* Prepares the system for a DMA out operation (cache handling...).
*
* @param        pDmaCommonBuffer is a DMA common buffer object pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsDmaOutPrepare(
    OsDmaCommonBufferT * pDmaCommonBuffer)
{
    // Dummy implementation.
    PATH_NOT_YET_TESTED();
    OS_ASSERT(pDmaCommonBuffer != NULL);
}


/**
******************************************************************************
*
* Finalizes the last DMA out operation (cache handling...).
*
* @param        pDmaCommonBuffer is a DMA common buffer object pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsDmaOutDone(
    OsDmaCommonBufferT * pDmaCommonBuffer)
{
    // Dummy implementation.
    PATH_NOT_YET_TESTED();
    OS_ASSERT(pDmaCommonBuffer != NULL);
}



#ifdef __cplusplus
}
#endif


#endif /* INC_OsDma_inline_h */
