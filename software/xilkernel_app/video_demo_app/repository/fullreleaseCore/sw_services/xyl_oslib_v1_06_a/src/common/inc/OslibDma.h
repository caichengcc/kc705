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
* @file OslibDma.h
*
* Contains relevant OS specific DMA functionality common to all the OSs.
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

#ifndef INC_OslibDma_h
#define INC_OslibDma_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibTypes.h"
#include "OsDma.h"



/********************************* Types *************************************/
typedef struct
{
    OsSizeT  size;
    OsSizeT  allignment;
    OsU64    desiredPhysicalAddress;

} OsDmaCommonBufferConfigT;

typedef struct
{
    OsU64    logicalAddress;
    void   * pVirtual;
    OsSizeT  size;

} OsDmaCommonBufferDescrT;



/************************** Function Declarations ****************************/

/**
******************************************************************************
*
* Creates a DMA common buffer.
*
* @param        pDma is a DMA object pointer.
* @param        pDmaCommonBufferConfig is a pointer to the DMA common buffer
*               containing a buffer configuration.
*
* @return       Pointer to a newly created DMA common buffer object.
*
* @note         None.
*
******************************************************************************/
OsDmaCommonBufferT *
OsDmaCommonBufferConstruct(
    OsDmaT                   * pDma,
    OsDmaCommonBufferConfigT * pDmaCommonBufferConfig);


/**
******************************************************************************
*
* Destroys a DMA common buffer.
*
* @param        pDmaCommonBuffer is a DMA common buffer object pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsDmaCommonBufferDestruct(
    OsDmaCommonBufferT * pDmaCommonBuffer);


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
void
OsDmaCommonBufferGetParams(
    OsDmaCommonBufferT      * pDmaCommonBuffer,
    OsDmaCommonBufferDescrT * pDmaCommonBufferDescr);


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
void
OsDmaInPrepare(
    OsDmaCommonBufferT * pDmaCommonBuffer);


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
void
OsDmaInDone(
    OsDmaCommonBufferT * pDmaCommonBuffer);


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
void
OsDmaOutPrepare(
    OsDmaCommonBufferT * pDmaCommonBuffer);


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
void
OsDmaOutDone(
    OsDmaCommonBufferT * pDmaCommonBuffer);



/****************************** Include Files ********************************/
#include "OsDma_inline.h"



#ifdef __cplusplus
}
#endif


#endif /* INC_OslibDma_h */
