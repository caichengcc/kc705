/*****************************************************************************
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2011 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
*****************************************************************************/
/****************************************************************************/

/** \addtogroup logiCVCmidBuffering logiCVC middle-level driver - Double/Tripple buffering
*     manipulation functions
    \ingroup logiCVCmid
 *   @{
 */

/**
*
* @file logiCVC_buff.c
*
* Contains logiCVC Double/Tripple buffering manipulation functions.
* See the logiCVC.h header file for more details on this driver.
*
*****************************************************************************/

/***************************** Include Files ********************************/
#include "logiCVC.h"
#include "logiCVC_l.h"
#include "logiCVC_instance.h"
#include "logiCVC_disp_i.h"


/***************** Macros (Inline Functions) Definitions ********************/

#define PRINT(format, ...)          OsPrintfInfo(TEXT(format),  ##__VA_ARGS__)
#define ERROR_PRINT(format, ...)    OsPrintfError(TEXT(format), ##__VA_ARGS__)
#if (defined(GENDRV_CVC_DEBUG) || defined(GENDRV_GLOBAL_DEBUG))
#define DEBUG_PRINT(format, ...)    OsPrintfInfo(TEXT(format),  ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...)
#endif


/************************** Function definitions *****************************/

/**
*
* Enables od disables layer external buffer switching (tripple buffering)
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    bEnable when set to 1 enables external switching of video buffers.
*               This functionality is used when there is an external video source
*               writing to logiCVC-ML  layer video buffer..
*
* @return
* - cvcRetSuccessE on success.
* - cvcRetFailureE on failure.
*
* @note         None.
*
*****************************************************************************/
cvcRetT logiCVC_SetExternalBufferSwitchingCtrl(logiCvcLayerPointerT logiCvcLayerPtr, OsBoolT bEnable)
{
    cvcRetT status = cvcRetFailureE;
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    OS_ASSERT(pLayer != NULL);

    DEBUG_PRINT("logiCVC_SetExternalBufferSwitchingCtrl() - layer %d, en/dis %d\n\r",
        (int)pLayer->LayerNumber, bEnable?1:0);

    if ((!pLayer->CvcBackPtr->ReadableRegs) && (pLayer->CvcBackPtr->CoreInitBeforeDriverStart))
    {
        ERROR_PRINT("logiCVC_SetExternalBufferSwitchingCtrl() - register CVC_LAYER_CTRL_ROFF not readable, function not supported\n\r");
    }
    else
    {
        if (pLayer->CvcBackPtr->ReadableRegs)
        {
            pLayer->LxCtrl = CVC_mReadReg(
                pLayer->CvcBackPtr->RegAccessObjPtr,
                CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE * pLayer->LayerNumber + CVC_LAYER_CTRL_ROFF);
        }

        if (bEnable)
        {
            pLayer->LxCtrl |= LX_CTRL_EN_EXT_VBUFF_SW_MSK;
        }
        else
        {
            pLayer->LxCtrl &= ~LX_CTRL_EN_EXT_VBUFF_SW_MSK;
        }

        CVC_mWriteReg(
            pLayer->CvcBackPtr->RegAccessObjPtr,
            CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE * pLayer->LayerNumber + CVC_LAYER_CTRL_ROFF,
            pLayer->LxCtrl);
        status = cvcRetSuccessE;
    }

    return status;
}


/**
*
* Reports current logiCVC layer buffer.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
*
* @return       Current buffer number.
*
* @note         None.
*
*****************************************************************************/
OsSizeT logiCVC_GetCurrentBufferNumber(logiCvcLayerPointerT logiCvcLayerPtr)
{
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    OsSizeT bufNum;
    OS_ASSERT(pLayer != NULL);

    bufNum = (OsSizeT)(0x03 &
        (   (CVC_mReadReg(pLayer->CvcBackPtr->RegAccessObjPtr, CVC_DOUBLE_VBUFF_ROFF)
         >> (pLayer->LayerNumber << 1)) ) );

    DEBUG_PRINT("logiCVC_GetCurrentBufferNumber() - layer %d buff %d\n\r", (int)pLayer->LayerNumber, (int)bufNum);
    return bufNum;
}


/**
*
* Obtains a logiCVC layer buffer pointer.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    bufNum is the ordinal number of buffer
*
* @return       logiCVC layer buffer pointer
*
* @note         None.
*
*****************************************************************************/
logiCvcBufferPointerT logiCVC_GetBufferPointer(logiCvcLayerPointerT logiCvcLayerPtr, OsSizeT bufNum)
{
    logiCvcBufferPointerT bufPtr = NULL;

    if (bufNum >= MAX_NUMBER_OF_BUFFERS)
    {
        ERROR_PRINT("logiCVC_GetBufferPointer() - Unsupported buffer number!\n\r");
    }
    else
    {
        logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
        OS_ASSERT(pLayer != NULL);
        bufPtr = &pLayer->aBuffer[bufNum];
    }

    return bufPtr;
}


/**
*
* Schedules a logiCVC layer buffer change.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    bufNum is the ordinal number of buffer
*
* @return
* - cvcRetSuccessE if buffer switch was successful.
* - cvcRetFailureE if bufNum is not correct.
*
* @note         None.
*
*****************************************************************************/
cvcRetT logiCVC_ChangeBuffer(logiCvcLayerPointerT logiCvcLayerPtr, OsSizeT bufNum)
{
    cvcRetT status = cvcRetSuccessE;
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    OS_ASSERT(pLayer != NULL);
    DEBUG_PRINT("logiCVC_ChangeBuffer() - layer %d bufNum %d\n\r", (int)pLayer->LayerNumber, (int)bufNum);

    if (bufNum >= MAX_NUMBER_OF_BUFFERS)
    {
        ERROR_PRINT("logiCVC_ChangeBuffer() - Unsupported buffer number!\n\r");
        status = cvcRetFailureE;
    }
    else
    {
        OsU32 sel     = bufNum << (pLayer->LayerNumber << 1);
        OsU32 selWrEn = 1 << (10 + pLayer->LayerNumber);
        pLayer->CvcBackPtr->accBufferSelect |= (sel | selWrEn);
    }

    return status;
}


/**
*
* Reports current logiCVC layer CLUT.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
*
* @return       Current CLUT number.
*
* @note         None.
*
*****************************************************************************/
OsSizeT logiCVC_GetCurrentClutNumber(logiCvcLayerPointerT logiCvcLayerPtr)
{
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    OsSizeT clutNum;
    OS_ASSERT(pLayer != NULL);

    clutNum = (OsSizeT)(0x03 &
        (   (CVC_mReadReg(pLayer->CvcBackPtr->RegAccessObjPtr, CVC_DOUBLE_CLUT_ROFF)
         >> (pLayer->LayerNumber << 1)) ) );

    DEBUG_PRINT("logiCVC_GetCurrentClutNumber() - layer %d CLUT %d\n\r", (int)pLayer->LayerNumber, (int)clutNum);
    return clutNum;
}


/**
*
* Obtains a logiCVC CLUT pointer. NOTE: This clut won't be accessible over the CLUT registers until
* logiCVC actually switches to this CLUT. By default CLUT 0 is accessible. To switch CLUT use functions
*  \ref logiCVC_ChangeClut and \ref logiCVC_Sync.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    clutNum is the ordinal number of CLUT
*
* @return       logiCVC CLUT pointer
*
* @note         None.
*
*****************************************************************************/
logiCvcClutPointerT logiCVC_GetClutPointer(logiCvcLayerPointerT logiCvcLayerPtr, OsSizeT clutNum)
{
    logiCvcClutPointerT clutPtr = NULL;

    if (clutNum >= MAX_NUMBER_OF_CLUTS)
    {
        ERROR_PRINT("logiCVC_GetClutPointer() - Unsupported CLUT number!\n\r");
    }
    else
    {
        logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
        OS_ASSERT(pLayer != NULL);
        clutPtr = &pLayer->aClut[clutNum];
    }

    return clutPtr;
}


/**
*
* Schedules a logiCVC layer CLUT change.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    clutNum is the ordinal number of CLUT
*
* @return
* - cvcRetSuccessE if CLUT switch was successful.
* - cvcRetFailureE if clutNum is not correct.
*
* @note         None.
*
*****************************************************************************/
cvcRetT logiCVC_ChangeClut(logiCvcLayerPointerT logiCvcLayerPtr, OsSizeT clutNum)
{
    cvcRetT status = cvcRetSuccessE;
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    OS_ASSERT(pLayer != NULL);
    DEBUG_PRINT("logiCVC_ChangeClut() - layer %d clutNum %d\n\r", (int)pLayer->LayerNumber, (int)clutNum);

    if (clutNum >= MAX_NUMBER_OF_CLUTS)
    {
        ERROR_PRINT("logiCVC_ChangeClut() - Unsupported CLUT number!\n\r");
        status = cvcRetFailureE;
    }
    else
    {
        OsU32 sel     = clutNum << (pLayer->LayerNumber << 1);
        OsU32 selWrEn = 1 << (10 + pLayer->LayerNumber);
        pLayer->CvcBackPtr->accClutSelect |= (sel | selWrEn);
    }

    return status;
}


/**
*
* Aborts scheduled logiCVC layer buffer and CLUT changes.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer.
* @param[in]    bWait specifies whether to wait until the change is done.
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_AbortChange(logiCvcLayerPointerT logiCvcLayerPtr, OsBoolT bWait)
{
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsU32 numLayer;

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    numLayer  = pLayer->LayerNumber;

    DEBUG_PRINT("logiCVC_AbortChange() %d: - acc 0x%08X 0x%08X\n\r",
        numLayer, pInstance->accBufferSelect, pInstance->accClutSelect);

    if (bWait)
    {
        while(!logiCVC_IsSynced(pInstance))
        {
            ;
        }
    }
    else
    {
        OsU32 sel     = 3 << (numLayer << 1);
        OsU32 selWrEn = 1 << (10 + numLayer);
        OsU32 clutNumPrev, clutNumCurr;
        OsU32 buffNumPrev, buffNumCurr;
        pInstance->accClutSelect   &= ~(sel | selWrEn);
        pInstance->wrClutSelect    &= ~(sel | selWrEn);
        pInstance->accBufferSelect &= ~(sel | selWrEn);
        pInstance->wrBufferSelect  &= ~(sel | selWrEn);

        /* Make sure that any pending switch requests are cancelled. */
        clutNumCurr = logiCVC_GetCurrentClutNumber(logiCvcLayerPtr);
        buffNumCurr = logiCVC_GetCurrentBufferNumber(logiCvcLayerPtr);
        do
        {
            clutNumPrev = clutNumCurr;
            buffNumPrev = buffNumCurr;

            sel = clutNumCurr << (numLayer << 1);
            CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_DOUBLE_CLUT_ROFF, sel | selWrEn);

            sel = buffNumCurr << (numLayer << 1);
            CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_DOUBLE_VBUFF_ROFF, sel | selWrEn);

            clutNumCurr = logiCVC_GetCurrentClutNumber(logiCvcLayerPtr);
            buffNumCurr = logiCVC_GetCurrentBufferNumber(logiCvcLayerPtr);

            CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_ROFF, 0x101 << numLayer);
        }
        while ((clutNumCurr != clutNumPrev) || (buffNumCurr != buffNumPrev));
    }
}


/**
*
* Executes all the scheduled logiCVC layer buffer and CLUT changes.
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC instance to be worked on.
* @param[in]    bWait specifies whether to wait until the change is done.
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_Sync(logiCvcPointerT logiCvcPtr, OsBoolT bWait)
{
    OsSizeT n;
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    OS_ASSERT(pInstance != NULL);
    DEBUG_PRINT("logiCVC_Sync(%d) - 0x%08X 0x%08X 0x%08X 0x%08X\n\r",
        bWait,
        pInstance->accBufferSelect, pInstance->wrBufferSelect,
        pInstance->accClutSelect, pInstance->wrClutSelect);

    /* Do not attempt to sync the layers that are already being synced */
    if (   ((pInstance->accBufferSelect & pInstance->wrBufferSelect & (0x1F << 10)) != 0)
        || ((pInstance->accClutSelect   & pInstance->wrClutSelect   & (0x1F << 10)) != 0) )
    {
        while(!logiCVC_IsSynced(logiCvcPtr))
        {
            ;
        }
    }

    if (pInstance->accBufferSelect != 0)
    {
        OsU32 reg = CVC_mReadReg(pInstance->RegAccessObjPtr, CVC_DOUBLE_VBUFF_ROFF);

        /* Do not attempt to sync the buffers that are already in sync */
        for (n=0; n < LOGICVC_MAX_LAYERS_PER_CVC; ++n)
        {
            OsSizeT num = (OsSizeT)(0x03 & (reg >> (n << 1)));
            OsU32 wrSelBit = 1 << (10 + n);

            if (pInstance->accBufferSelect & wrSelBit)
            {
                OsSizeT reqBuffNum = (OsSizeT)(0x03 & (pInstance->accBufferSelect >> (n << 1)));
                if (reqBuffNum == num)
                {
                    pInstance->accBufferSelect &= ~wrSelBit;
                }
            }
        }

        if (pInstance->accBufferSelect & (0x1F << 10))
        {
            CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_ROFF, ((pInstance->accBufferSelect >> 10) & 0x1F) << 0);
            CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_DOUBLE_VBUFF_ROFF, pInstance->accBufferSelect);
            pInstance->wrBufferSelect |= pInstance->accBufferSelect;
        }

        pInstance->accBufferSelect = 0;
    }

    if (pInstance->accClutSelect != 0)
    {
        OsU32 reg = CVC_mReadReg(pInstance->RegAccessObjPtr, CVC_DOUBLE_CLUT_ROFF);

        /* Do not attempt to sync the CLUTs that are already in sync */
        for (n=0; n < LOGICVC_MAX_LAYERS_PER_CVC; ++n)
        {
            OsSizeT num = (OsSizeT)(0x03 & (reg >> (n << 1)));
            OsU32 wrSelBit = 1 << (10 + n);

            if (pInstance->accClutSelect & wrSelBit)
            {
                OsSizeT reqNum = (OsSizeT)(0x03 & (pInstance->accClutSelect >> (n << 1)));
                if (reqNum == num)
                {
                    pInstance->accClutSelect &= ~wrSelBit;
                }
            }
        }

        if (pInstance->accClutSelect & (0x1F << 10))
        {
            CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_ROFF, ((pInstance->accClutSelect >> 10) & 0x1F) << 5);
            CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_DOUBLE_CLUT_ROFF, pInstance->accClutSelect);
            pInstance->wrClutSelect |= pInstance->accClutSelect;
        }

        pInstance->accClutSelect = 0;
    }

    if (bWait)
    {
        while (!logiCVC_IsSynced(logiCvcPtr))
        {
            ;
        }
    }
}


/**
*
* Reports whether all the scheduled buffer/CLUT changes have been peformed.
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC instance to be worked on
*
* @return
* - TRUE if all the scheduled buffer/CLUT changes have been peformed.
* - FALSE otherwise.
*
* @note         None.
*
*****************************************************************************/
OsBoolT logiCVC_IsSynced(logiCvcPointerT logiCvcPtr)
{
    OsU32 intStatus;
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    OS_ASSERT(pInstance != NULL);

    intStatus = CVC_mReadReg(pInstance->RegAccessObjPtr, CVC_INT_ROFF);

    if (pInstance->wrBufferSelect != 0)
    {
        OsU32 bitsToCheck = (pInstance->wrBufferSelect >> 10) & 0x1F;

        if ((bitsToCheck & (intStatus & 0x1F)) != bitsToCheck)
        {
            return FALSE;
        }

        pInstance->wrBufferSelect = 0;
    }

    if (pInstance->wrClutSelect != 0)
    {
        OsU32 bitsToCheck = (pInstance->wrClutSelect >> 10) & 0x1F;

        if ((bitsToCheck & ((intStatus >> 8) & 0x1F)) != bitsToCheck)
        {
            return FALSE;
        }

        pInstance->wrClutSelect = 0;
    }

    DEBUG_PRINT("logiCVC_IsSynced() - YES\n\r");

    return TRUE;
}

/* @} */
