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

/** \addtogroup logiCVCmid logiCVC middle-level driver - Basic functions
 *  @{
 */

/****************************************************************************/
/**
*
* @file logiCVC.c
*
* Contains basic set of most commonly used functions of the logiCVC driver. See the logiCVC.h
* header file for more details on this driver.
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


/************************** Functions Declarations ****************************/
int getNextMemSortedLayerNum(logiCVC_InstanceT *pInstance, OsSizeT numLayer);
int getPrevMemSortedLayerNum(logiCVC_InstanceT *pInstance, OsSizeT numLayer);


/************************** Constant Definitions ****************************/

static OsCharT const * const apConfigParamName[] =
{
    TEXT("LOGICVC_IP_MAJOR_REVISION"),
    TEXT("LOGICVC_IP_MINOR_REVISION"),
    TEXT("LOGICVC_IP_PATCH_LEVEL"),
    TEXT("LOGICVC_REGS_BASEADDR"),
    TEXT("LOGICVC_REGS_HIGHADDR"),
    TEXT("LOGICVC_VMEM_BASEADDR"),
    TEXT("LOGICVC_VMEM_HIGHADDR"),
    TEXT("LOGICVC_ROW_STRIDE"),
    TEXT("LOGICVC_NUM_OF_LAYERS"),
    TEXT("LOGICVC_USE_BACKGROUND"),
    TEXT("LOGICVC_READABLE_REGS"),
    TEXT("LOGICVC_LAYER_0_DATA_WIDTH"),
    TEXT("LOGICVC_LAYER_1_DATA_WIDTH"),
    TEXT("LOGICVC_LAYER_2_DATA_WIDTH"),
    TEXT("LOGICVC_LAYER_3_DATA_WIDTH"),
    TEXT("LOGICVC_LAYER_4_DATA_WIDTH"),
    TEXT("LOGICVC_LAYER_0_TYPE"),
    TEXT("LOGICVC_LAYER_1_TYPE"),
    TEXT("LOGICVC_LAYER_2_TYPE"),
    TEXT("LOGICVC_LAYER_3_TYPE"),
    TEXT("LOGICVC_LAYER_4_TYPE"),
    TEXT("LOGICVC_LAYER_0_ALPHA_MODE"),
    TEXT("LOGICVC_LAYER_1_ALPHA_MODE"),
    TEXT("LOGICVC_LAYER_2_ALPHA_MODE"),
    TEXT("LOGICVC_LAYER_3_ALPHA_MODE"),
    TEXT("LOGICVC_LAYER_4_ALPHA_MODE"),
    TEXT("LOGICVC_LAYER_0_OFFSET"),
    TEXT("LOGICVC_LAYER_1_OFFSET"),
    TEXT("LOGICVC_LAYER_2_OFFSET"),
    TEXT("LOGICVC_LAYER_3_OFFSET"),
    TEXT("LOGICVC_LAYER_4_OFFSET"),
    TEXT("LOGICVC_BUFFER_0_OFFSET"),
    TEXT("LOGICVC_BUFFER_1_OFFSET"),
    TEXT("LOGICVC_BUFFER_2_OFFSET"),
    TEXT("LOGICVC_BUFFER_3_OFFSET"),
    TEXT("LOGICVC_BUFFER_4_OFFSET")
};

static OsCharT const * const apDisplayParamName[] =
{
    TEXT("HorizontalFrontPorch"),
    TEXT("HorizontalSync"),
    TEXT("HorizontalBackPorch"),
    TEXT("HorizontalRes"),
    TEXT("VerticalFrontPorch"),
    TEXT("VerticalSync"),
    TEXT("VerticalBackPorch"),
    TEXT("VerticalRes"),
    TEXT("ControlRegister"),
    TEXT("TypeRegister"),
    TEXT("PixelClock"),
    TEXT("PowerOnDelay"),
    TEXT("SignalOnDelay")
};


/************************** Static Function definitions *****************************/

static void readVersionRegister(OsHwAccessObjectT const * const pHwAccessObject,
                                OsU32 *outMajorVer,
                                OsU32 *outMinorVer,
                                OsU32 *outPatchVer,
                                OsU32 *outLicense)
{
    OsU32 ver = CVC_mReadReg(pHwAccessObject, CVC_IPVERSION_ROFF);
    *outPatchVer = (ver >>  0) & 0x1f;
    *outMinorVer = (ver >>  5) & 0x3f;
    *outMajorVer = (ver >> 11) & 0x3f;
    *outLicense  = (ver >> 17) & 0x03;
}


OS_INLINE int getNextMemSortedLayerNum(logiCVC_InstanceT *pInstance, OsSizeT numLayer)
{
    OsSizeT i;

    OS_ASSERT(numLayer < pInstance->NumLayers);

    for(i = 0; i < pInstance->NumLayers; i++)
    {
        if(pInstance->memSortedLayerIndices[i] == numLayer)
            break;
    }
    OS_ASSERT(i >= 0 && i < pInstance->NumLayers-1);
    return pInstance->memSortedLayerIndices[i+1];
}


OS_INLINE int getPrevMemSortedLayerNum(logiCVC_InstanceT *pInstance, OsSizeT numLayer)
{
    int ret = -1;
    OsSizeT i;

    OS_ASSERT(numLayer < pInstance->NumLayers);

    for(i = 0; i < pInstance->NumLayers; i++)
    {
        if(pInstance->memSortedLayerIndices[i] == numLayer)
            break;
    }
    OS_ASSERT(i >= 0 && i < pInstance->NumLayers);
    if(i > 0 && i < pInstance->NumLayers)
    {
        ret = pInstance->memSortedLayerIndices[i-1];
    }
    return ret;
}

#if (defined(GENDRV_CVC_DEBUG) || defined(GENDRV_GLOBAL_DEBUG))
static void printConfigStruct(logiCvcConfigT const * logiCVCConfigPtr)
{
    int i;
    OsU32 *data = (OsU32*)logiCVCConfigPtr;
    DEBUG_PRINT("logiCVC printConfigStruct()\n\r");

    for(i = 0; i < sizeof(*logiCVCConfigPtr)/sizeof(OsU32); i++)
    {
        DEBUG_PRINT("%s = 0x%X\n\r", apConfigParamName[i], (int)data[i]);
    }
}
#endif

/************************** Function definitions *****************************/

/**
*
* Obtains a configuration parameter name.
*
* @param[in]    index is a configuration parameter index.
*
* @return       Configuration parameter name or a NULL pointer on an error.
*
* @note         None.
*
******************************************************************************/
OsCharT const * logiCVC_GetConfigParamName(OsSizeT index)
{
    OsCharT const * pName = NULL;

    OS_ASSERT(   (sizeof(apConfigParamName) / sizeof(apConfigParamName[0]))
              == (sizeof(logiCvcConfigT) / sizeof(OsU32)) );

    if (   (   (sizeof(apConfigParamName) / sizeof(apConfigParamName[0]))
            == (sizeof(logiCvcConfigT) / sizeof(OsU32)) )
        && (index < (sizeof(apConfigParamName) / sizeof(apConfigParamName[0]))) )
    {
        pName = apConfigParamName[index];
    }

    return pName;
}


/**
*
* Obtains a display parameter name.
*
* @param[in]    index is a display parameter index.
*
* @return       Display parameter name or a NULL pointer on an error.
*
* @note         None.
*
******************************************************************************/
OsCharT const * logiCVC_GetDisplayParamName(OsSizeT index)
{
    OsCharT const * pName = NULL;

    OS_ASSERT(   (sizeof(apDisplayParamName) / sizeof(apDisplayParamName[0]))
              == (sizeof(logiCvcDisplayT) / sizeof(OsU32)) );

    if (   (   (sizeof(apDisplayParamName) / sizeof(apDisplayParamName[0]))
            == (sizeof(logiCvcDisplayT) / sizeof(OsU32)) )
        && (index < (sizeof(apDisplayParamName) / sizeof(apDisplayParamName[0]))) )
    {
        pName = apDisplayParamName[index];
    }

    return pName;
}


/**
*
* Initialize a logiCVC instance. Default information about logiCVC and its layers
* is obtained in logiCVC structure.
*
* @param[in]    pConfig is a pointer to the structure containing core instance configuration data.
* @param[in]    pOsHwRegResource is a pointer to the register resources struct.
* @param[in]    pOsHwVideoMemResource is a pointer to the video memory resources struct.
*
* @return       Handle to logiCVC driver.
*
* @note         pOsHwVideoMemResource pointer is allowed to be NULL.
*
*****************************************************************************/
logiCvcPointerT logiCVC_Initialize(logiCvcConfigT const * const pConfig,
                                   OsHwResourceT  const * const pOsHwRegResource,
                                   OsHwResourceT  const * const pOsHwVideoMemResource)
{
    logiCVC_InstanceT *pInstance = NULL;
    OsSizeT n;
    OsSizeT cvcLayer;
    DEBUG_PRINT("logiCVC_Initialize()\n\r");

    if ((pConfig != NULL) && (pOsHwRegResource != NULL))
    {
        pInstance = (logiCVC_InstanceT *)
            OsMemoryAllocate(sizeof(logiCVC_InstanceT), OS_MEMORY_TYPE__NON_PAGED_POOL);
        if (pInstance != NULL)
        {
            pInstance->RegAccessObjPtr = OsHwAccessInit(pOsHwRegResource, TRUE);
            if (NULL == pInstance->RegAccessObjPtr)
            {
                OsMemoryFree(pInstance);
                pInstance = NULL;
            }
        }
    }

#if (defined(GENDRV_CVC_DEBUG) || defined(GENDRV_GLOBAL_DEBUG))
    printConfigStruct(pConfig);
#endif

    if (pInstance != NULL)
    {
        /* Video memory resource initialisation is not mandatory */
        if (pOsHwVideoMemResource != NULL)
        {
            pInstance->VideoAccessObjPtr = OsHwAccessInit(pOsHwVideoMemResource, FALSE);
            if (NULL == pInstance->VideoAccessObjPtr)
            {
                OsHwAccessDeinit(pInstance->RegAccessObjPtr);
                OsMemoryFree(pInstance);
                pInstance = NULL;
            }
        }
        else
        {
            pInstance->VideoAccessObjPtr = NULL;
        }
    }

    if (pInstance != NULL)
    {
        pInstance->IsReady                   = TRUE;
        pInstance->RegBaseAddr               = pConfig->RegBaseAddr;
        pInstance->RegHighAddr               = pConfig->RegHighAddr;
        pInstance->MemoryLowAddr             = pConfig->MemoryLowAddr;
        pInstance->MemoryHighAddr            = pConfig->MemoryHighAddr;
        pInstance->HorizontalRes             = 0; // default until CVC is started
        pInstance->VerticalRes               = 0; // default until CVC is started
        pInstance->NumLayers                 = pConfig->NumLayers;
        pInstance->UseBackground             = !!pConfig->UseBackground;
        pInstance->ReadableRegs              = !!pConfig->ReadableRegs;
        pInstance->IntMask                   = 0xFFFF; // reset value
        pInstance->CvcConfig                 = *pConfig;
        pInstance->CoreInitBeforeDriverStart = FALSE;
        pInstance->accBufferSelect           = 0;
        pInstance->wrBufferSelect            = 0;
        pInstance->accClutSelect             = 0;
        pInstance->wrClutSelect              = 0;

        for(cvcLayer = 0; cvcLayer < pInstance->NumLayers; cvcLayer++)
        {
            pInstance->LayerInfo[cvcLayer].CvcBackPtr = (struct logiCVC_Instance *)pInstance;
            pInstance->LayerInfo[cvcLayer].BitPerPix = pConfig->Layer_DataWidth[cvcLayer];
            if (pConfig->Layer_DataWidth[cvcLayer] == 8)
            {
                pInstance->LayerInfo[cvcLayer].BytePerPix = 1;
            }
            else if (pConfig->Layer_DataWidth[cvcLayer] == 16)
            {
                pInstance->LayerInfo[cvcLayer].BytePerPix = 2;
            }
            else
            {
                pInstance->LayerInfo[cvcLayer].BytePerPix = 4;
            }

            OS_ASSERT(pConfig->Layer_Type[cvcLayer] < CVC_LAYER_TYPE__OUT_OF_RANGE);
            pInstance->LayerInfo[cvcLayer].Type = pConfig->Layer_Type[cvcLayer];

            pInstance->LayerInfo[cvcLayer].AlphaMode = pConfig->Layer_AlphaMode[cvcLayer];
            if (   (pInstance->LayerInfo[cvcLayer].AlphaMode == LOGICVC_ALPHA_MODE_PIXEL)
                && (pInstance->LayerInfo[cvcLayer].Type      != CVC_LAYER_TYPE__ALPHA) )
            {  /* If pixel mode add aditional pixel for 16bpp and 8bpp */
                if (pConfig->Layer_DataWidth[cvcLayer] == 8)
                {
                    pInstance->LayerInfo[cvcLayer].BytePerPix = 2;
                }
                else if (pConfig->Layer_DataWidth[cvcLayer] == 16)
                {
                    pInstance->LayerInfo[cvcLayer].BytePerPix = 4;
                }
                else
                {
                    pInstance->LayerInfo[cvcLayer].BytePerPix = 4;
                }
            }

            for (n=0; n < MAX_NUMBER_OF_BUFFERS; ++n)
            {
                pInstance->LayerInfo[cvcLayer].aBuffer[n].pLayer = &pInstance->LayerInfo[cvcLayer];
                pInstance->LayerInfo[cvcLayer].aBuffer[n].offset =
                      (  pConfig->Layer_BufferA_Offset[cvcLayer]
                       + (n * pConfig->Layer_Buffer_Offset[cvcLayer]) )
                    * pConfig->RowStride * pInstance->LayerInfo[cvcLayer].BytePerPix;
                pInstance->LayerInfo[cvcLayer].aBuffer[n].index = n;
            }

            for (n=0; n < MAX_NUMBER_OF_CLUTS; ++n)
            {
                pInstance->LayerInfo[cvcLayer].aClut[n].pLayer = &pInstance->LayerInfo[cvcLayer];
                pInstance->LayerInfo[cvcLayer].aClut[n].offset =
                      LOGICVC_CLUT_0_A_OFFSET
                    + (n * (LOGICVC_CLUT_0_B_OFFSET - LOGICVC_CLUT_0_A_OFFSET))
                    + cvcLayer * (2 * LOGICVC_CLUT_SIZE * LOGICVC_CLUT_ELEM_BYTE_DIST);
                pInstance->LayerInfo[cvcLayer].aClut[n].index = n;
            }

            pInstance->LayerInfo[cvcLayer].OutputPosition.x_pos = 0;  // default until CVC is started
            pInstance->LayerInfo[cvcLayer].OutputPosition.y_pos = 0;  // default until CVC is started
            pInstance->LayerInfo[cvcLayer].OutputPosition.width = 0;  // default until CVC is started
            pInstance->LayerInfo[cvcLayer].OutputPosition.height = 0; // default until CVC is started
            pInstance->LayerInfo[cvcLayer].Stride = pConfig->RowStride;
            pInstance->LayerInfo[cvcLayer].LayerAlpha = -1; // not support in hw yet
            pInstance->LayerInfo[cvcLayer].TransparentColor = -1; // only single transparent color per CVC supported in HW yet
            pInstance->LayerInfo[cvcLayer].LxCtrl = LX_CTRL_EN_LAYER_MSK; // reset value
            pInstance->LayerInfo[cvcLayer].LayerNumber = cvcLayer;
        }
        /* Don't count background layer as a layer */
        if(pInstance->UseBackground && (pInstance->NumLayers >= 2))
        {
            --pInstance->NumLayers;
        }
        /* Sort the layers by their A buffers */
        {
            OsSizeT x,y;
            for(x=0; x<pInstance->NumLayers; x++)
            {
                pInstance->memSortedLayerIndices[x] = x;
            }
            for(x=0; x<pInstance->NumLayers; x++)
            {
                for(y=0; y<pInstance->NumLayers-1; y++)
                {
                    int layIdx     = pInstance->memSortedLayerIndices[y];
                    int layIdxNext = pInstance->memSortedLayerIndices[y+1];
                    if(pInstance->LayerInfo[layIdx].aBuffer[0].offset > pInstance->LayerInfo[layIdxNext].aBuffer[0].offset)
                    {
                        int tmp = pInstance->memSortedLayerIndices[y+1];
                        pInstance->memSortedLayerIndices[y+1] = pInstance->memSortedLayerIndices[y];
                        pInstance->memSortedLayerIndices[y] = tmp;
                    }
                }
            }
        }

        readVersionRegister(pInstance->RegAccessObjPtr,
                            &pInstance->ipMajorRevision,
                            &pInstance->ipMinorRevision,
                            &pInstance->ipPatchRevision,
                            &pInstance->ipLicenseType);

        if (   (pInstance->ipMajorRevision != pInstance->CvcConfig.ipMajorRevision)
            || (pInstance->ipMinorRevision != pInstance->CvcConfig.ipMinorRevision)
            || (pInstance->ipPatchRevision != pInstance->CvcConfig.ipPatchRevision) )
        {
            ERROR_PRINT("logiCVC_Initialize WARNING: version mismatch %d %d %d %d (endian?)\n\r",
                pInstance->ipMajorRevision,
                pInstance->ipMinorRevision,
                pInstance->ipPatchRevision,
                pInstance->ipLicenseType);
        }
    }

    return (logiCvcPointerT)pInstance;
}


/**
*
* Deinitialize a logiCVC instance.
*
* @param[in]    logiCvcPtr is a logiCVC instance pointer.
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_Deinitialize(logiCvcPointerT logiCvcPtr)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    DEBUG_PRINT("logiCVC_Deinitialize()\n\r");

    if (pInstance != NULL)
    {
        if (pInstance->RegAccessObjPtr != NULL)
        {
            OsHwAccessDeinit(pInstance->RegAccessObjPtr);
            pInstance->RegAccessObjPtr = NULL;
        }
        if (pInstance->VideoAccessObjPtr != NULL)
        {
            OsHwAccessDeinit(pInstance->VideoAccessObjPtr);
            pInstance->VideoAccessObjPtr = NULL;
        }
        OsMemoryFree(pInstance);
    }
}


/**
*
* Initializes and starts logiCVC. Display data must be filled in order to
* initialize logiCVC register accordingly.
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC instance to be worked on.
* @param[in]    pDisplayData is the pointer to the filled \ref logiCvcDisplayT structure
* @param[in]    flags determines aditional flags, see \ref cvcStartFlagsE
*
* @return
* - cvcRetSuccessE if everything starts up as expected.
* - cvcRetFailureE if the Display Type is not found in the configuration table.
*
* @note         None.
*
*****************************************************************************/
cvcRetT logiCVC_Start(logiCvcPointerT logiCvcPtr, logiCvcDisplayT const *pDisplayData, cvcStartFlagsE flags)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    OsSizeT cvcLayer;
    DEBUG_PRINT("logiCVC_Start() - flags %d\n\r", (int)flags);

    if (pInstance->CvcConfig.RowStride < pDisplayData->HorizontalRes)
    {
        ERROR_PRINT("logiCVC_Start() - Unsupported display H resolution!\n\r");
        ERROR_PRINT("Max allowed H resolution %d\n\rDisplay H resolution %d\n\r",
            pInstance->CvcConfig.RowStride, pDisplayData->HorizontalRes);
        return cvcRetFailureE;
    }

    pInstance->HorizontalRes = pDisplayData->HorizontalRes;
    pInstance->VerticalRes   = pDisplayData->VerticalRes;

    for(cvcLayer = 0; cvcLayer < pInstance->NumLayers; cvcLayer++)
    {
        if (pInstance->CvcConfig.Layer_Buffer_Offset[cvcLayer] < pInstance->VerticalRes)
        {
            DEBUG_PRINT("logiCVC_Start() - Max supported V resolution %d (layer %d)\n\rDisplay V resolution %d\n\r",
                pInstance->CvcConfig.Layer_Buffer_Offset[cvcLayer], cvcLayer, pInstance->VerticalRes);
        }
        /* Set default values into structure, hardware will contain this values initially so no need to set them */
        pInstance->LayerInfo[cvcLayer].OutputPosition.width  = (OsU16)pDisplayData->HorizontalRes;
        pInstance->LayerInfo[cvcLayer].OutputPosition.height = (OsU16)pDisplayData->VerticalRes;
    }

    if (!(flags & CVC_START_SKIP_REGS_WRITE_E))
    {
        /* Check if CVC is already started by reading the PRWCTRL register */
        if(!pInstance->ReadableRegs)
        {
            if(CVC_V_EN_MSK & CVC_mReadReg(pInstance->RegAccessObjPtr, CVC_SPWRCTRL_ROFF))
            {
                pInstance->CoreInitBeforeDriverStart = TRUE;
            }
        }

        /* Horizontal front porch */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SHSY_FP_ROFF, pDisplayData->HorizontalFrontPorch - 1);
        /* Horizontal sync */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SHSY_ROFF, pDisplayData->HorizontalSync - 1);
        /* Horizontal back porch */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SHSY_BP_ROFF, pDisplayData->HorizontalBackPorch - 1);
        /* Horizontal resolution */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SHSY_RES_ROFF, pDisplayData->HorizontalRes - 1);
        /* Vertical front porch */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SVSY_FP_ROFF, pDisplayData->VerticalFrontPorch - 1);
        /* Vertical sync */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SVSY_ROFF, pDisplayData->VerticalSync - 1);
        /* Vertical back porch */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SVSY_BP_ROFF, pDisplayData->VerticalBackPorch - 1);
        /* Vertical resolution */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SVSY_RES_ROFF, pDisplayData->VerticalRes - 1);
        /* Display control register */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SCTRL_ROFF, pDisplayData->ControlRegister);
        /* Display type register */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SDTYPE_ROFF, pDisplayData->TypeRegister);
        /* Initially disable double buffering */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_DOUBLE_VBUFF_ROFF, 0x00);
        /* Initially disable double clut buffering */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_DOUBLE_CLUT_ROFF, 0x00);
        /* Disable all logiCVC interrupts */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_MASK_ROFF, 0xFFFF);
        /* Display is initially turned off */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SPWRCTRL_ROFF, 0x00);
    }
    else
    {
        pInstance->CoreInitBeforeDriverStart = TRUE;
        while(!logiCVC_IsSynced(logiCvcPtr))
        {
            ;
        }
    }

    /* Clear any active interrupts */
    CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_ROFF, 0xFFFF);

    pInstance->accBufferSelect = 0;
    pInstance->wrBufferSelect  = 0;
    pInstance->accClutSelect   = 0;
    pInstance->wrClutSelect    = 0;

    return cvcRetSuccessE;
}

/**
*
* Returns interupt status register content.
*
* @param[in]    logiCvcPtr is a logiCVC instance pointer.
*
* @return       interupt status is a combination of bit values between
*                \ref LOGICVC_INT_L0_VBUFF_SW_MSK
*               and \ref LOGICVC_INT_L4_CLUT_SW_MSK.
*
*****************************************************************************/
OsU32 logiCVC_GetIntStatus(logiCvcPointerT logiCvcPtr)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;

    OS_ASSERT(pInstance != NULL);

    return CVC_mReadReg(pInstance->RegAccessObjPtr, CVC_INT_ROFF);
}


/**
*
* Clears interupt status (acknowledges interrupt) by writing into
* interrupt status register
*
* @param[in]    logiCvcPtr is a logiCVC instance pointer.
* @param[in]    mask can be combination of bits between
*               \ref LOGICVC_INT_L0_VBUFF_SW_MSK
*               and \ref LOGICVC_INT_L4_CLUT_SW_MSK
*
*****************************************************************************/
void logiCVC_ClearIntStatus(logiCvcPointerT logiCvcPtr, OsU32 mask)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;

    OS_ASSERT(pInstance != NULL);

    CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_ROFF, mask);
}


/**
* Enables/disables particular interrupts by writing into interrupt
* mask register. When bit is high this means that interrupt is masked.
*
* @param[in]    logiCvcPtr is a logiCVC instance pointer.
* @param[in]    mask can be combination of bit values between
*               \ref LOGICVC_INT_L0_VBUFF_SW_MSK
*               and \ref LOGICVC_INT_L4_CLUT_SW_MSK
*
*****************************************************************************/
void logiCVC_WriteIntMask(logiCvcPointerT logiCvcPtr, OsU32 mask)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;

    OS_ASSERT(pInstance != NULL);

    CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_MASK_ROFF, mask);
}


/**
*
* Enable/disable logiCVC layer .
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    bEnable: TRUE - enable,  FALSE - disable
*
* @return       None.
*
* @note         After reset only layer 0 is enabled
*
*****************************************************************************/
void logiCVC_LayerEnDis(logiCvcLayerPointerT logiCvcLayerPtr, OsBoolT bEnable)
{
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsU32 numLayer;

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    numLayer  = pLayer->LayerNumber;

    DEBUG_PRINT("logiCVC_LayerEnDis() - layer %d en/dis %d\n\r", (int)numLayer, bEnable?1:0);

    if((!pInstance->ReadableRegs) && (pInstance->CoreInitBeforeDriverStart))
    {
        ERROR_PRINT("logiCVC_LayerEnDis() - WARNING register CVC_LAYER_CTRL_ROFF not readable, function may overwrite other bits in layer control register\n\r");
        pLayer->LxCtrl = bEnable?LX_CTRL_EN_LAYER_MSK:0;
        CVC_mWriteReg(pInstance->RegAccessObjPtr,
            CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF,
            pLayer->LxCtrl);
    }
    else
    {
        if(pInstance->ReadableRegs)
        {
            pLayer->LxCtrl = CVC_mReadReg(pInstance->RegAccessObjPtr,
                             CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF);
        }
        if (bEnable)
        {
            pLayer->LxCtrl |= LX_CTRL_EN_LAYER_MSK;
        }
        else
        {
            logiCVC_AbortChange(logiCvcLayerPtr, TRUE);/* fixme if */
            pLayer->LxCtrl &= ~LX_CTRL_EN_LAYER_MSK;
        }
        CVC_mWriteReg(pInstance->RegAccessObjPtr,
            CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF,
            pLayer->LxCtrl);
        /* fixme if (!bEnable)
        {
            logiCVC_AbortChange(logiCvcLayerPtr, FALSE);
        }*/
    }
}


/**
*
* Sets or clears valid power signal for the LCD display.
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC driver
* @param[in]    pwrSignal contains logiCVC power signal flag
* @param[in]    bEnable is an enable/disable flag
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_PowerControl(logiCvcPointerT logiCvcPtr, cvcPowerSignalFlagsE pwrSignal, OsBoolT bEnable)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    OsU32 pwrCtrlReg;
    DEBUG_PRINT("logiCVC_PowerControl()\n\r");

    OS_ASSERT(pInstance != NULL);

    switch (pwrSignal)
    {
        case CVC_PWR_OFF:
            CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SPWRCTRL_ROFF, 0);
            return;
        case CVC_PWR_VDD:
            pwrSignal = CVC_EN_VDD_MSK;
            break;
        case CVC_PWR_VEE:
            pwrSignal = CVC_EN_VEE_MSK;
            break;
        case CVC_PWR_SIGNALS:
            pwrSignal = CVC_V_EN_MSK;
            break;
        case CVC_PWR_BACKLIGHT:
            pwrSignal = CVC_EN_BLIGHT_MSK;
            break;
    }

    pwrCtrlReg = CVC_mReadReg(pInstance->RegAccessObjPtr, CVC_SPWRCTRL_ROFF);
    if (bEnable)
        pwrCtrlReg |= pwrSignal;
    else
        pwrCtrlReg &= (~pwrSignal);
    CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_SPWRCTRL_ROFF, pwrCtrlReg);
}


/**
*
* Sets the background color of the logiCVC.
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC instance to be worked on.
* @param[in]    color is the transparent color (24bpp, RGB)
*
* @return
* - cvcRetSuccessE if everything starts up as expected.
* - cvcRetFailureE if bits per pixel value isn't supported.
*
* @note         Background color will be displayed on the last layer if this is
*               enabled in HW configuration.
*
*****************************************************************************/
cvcRetT logiCVC_SetBackgroundColor(logiCvcPointerT logiCvcPtr, OsU32 color)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    logiCVC_LayerT *pLayer;
    OsU32 colorLE;
    OsU8 *pColor = (OsU8 *)(&colorLE);
    DEBUG_PRINT("logiCVC_SetBackgroundColor() - color 0x%X\n\r", (int)color);

    /* If background layer is enabled in HW */
    if (!pInstance->UseBackground)
    {
        ERROR_PRINT("logiCVC_SetBackgroundColor() - Background layer not supported in HW!\n\r");
        return cvcRetFailureE;
    }

    /* Point to last layer */
    pLayer = &pInstance->LayerInfo[pInstance->NumLayers];

    OsToLe32(color, &colorLE);

    /* Color information of the last layer and use this color writing */
    if (pLayer->BitPerPix == 8)
    {
        OsU8 b = (pColor[0] >> 6) & 0x03; /* take 2bit (MSB) blue component */
        OsU8 g = (pColor[1] >> 5) & 0x07; /* take 3bit (MSB) green component */
        OsU8 r = (pColor[2] >> 5) & 0x07; /* take 3bit (MSB) red component */
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_BACKCOL_ROFF, ((r << 5) | (g << 2) | (b)));
    }
    else if (pLayer->BitPerPix == 16)
    {
        OsU8 b  = (pColor[0] >> 3) & 0x1f;
        OsU8 gl = (pColor[1] << 3) & 0xe0;
        OsU8 gh = (pColor[1] >> 5) & 0x07;
        OsU8 r  = (pColor[2]     ) & 0xf8;
        OsU8 low = b | gl;
        OsU8 hi  = r | gh;
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_BACKCOL_ROFF, ((hi<<8) | low));
    }
    else if (pLayer->BitPerPix == 24)
    {
        CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_BACKCOL_ROFF, ((pColor[2]<<16) | (pColor[1]<<8) | pColor[0]));
    }
    else
    {
        ERROR_PRINT("logiCVC_SetBackgroundColor() - Unsupported BitPerPix value!\n\r");
        return cvcRetFailureE;
    }

    return cvcRetSuccessE;
}


/**
*
* Sets the transparent color of a layer.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    color is the transparent color. User has to make sure that he
*               supplies 8, 16 or 24bpp color to the function depending on layer
*               color depth. So, for 16bpp layer only lowest 2 bytes are used.
*
* @return
* - cvcRetSuccessE if transparent color is set.
* - cvcRetFailureE if transparent color isn't set.
*
* @note         None.
*
*****************************************************************************/
cvcRetT logiCVC_SetTransparentColor(logiCvcLayerPointerT logiCvcLayerPtr, OsU32 color)
{
    cvcRetT status = cvcRetFailureE;
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsU32 numLayer;

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    numLayer  = pLayer->LayerNumber;

    DEBUG_PRINT("logiCVC_SetTransparentColor() - layer %d color 0x%X\n\r", (int)numLayer, (int)color);

    CVC_mWriteReg(pInstance->RegAccessObjPtr,
        CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_TRANSP_ROFF,
        color);

    if((!pInstance->ReadableRegs) && (pInstance->CoreInitBeforeDriverStart))
    {
        ERROR_PRINT("logiCVC_SetTransparentColor() - register CVC_LAYER_CTRL_ROFF not readable, function not supported\n\r");
    }
    else
    {
        if(pInstance->ReadableRegs)
        {
            pLayer->LxCtrl = CVC_mReadReg(pInstance->RegAccessObjPtr,
                             CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF);
        }

        pLayer->LxCtrl &= ~LX_CTRL_DIS_TRANSP_MSK;

        CVC_mWriteReg(pInstance->RegAccessObjPtr,
            CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF,
            pLayer->LxCtrl);
        status = cvcRetSuccessE;
    }

    return status;
}


/**
*
* Disables the transparent color of a layer.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
*
* @return
* - cvcRetSuccessE if transparent color is disabled.
* - cvcRetFailureE if transparent color isn't disabled.
*
* @note         None.
*
*****************************************************************************/
cvcRetT logiCVC_DisableTransparentColor(logiCvcLayerPointerT logiCvcLayerPtr)
{
    cvcRetT status = cvcRetFailureE;
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsU32 numLayer;

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    numLayer  = pLayer->LayerNumber;

    DEBUG_PRINT("logiCVC_DisableTransparentColor() - layer %d\n\r", (int)numLayer);

    if((!pInstance->ReadableRegs) && (pInstance->CoreInitBeforeDriverStart))
    {
        ERROR_PRINT("logiCVC_DisableTransparentColor() - register CVC_LAYER_CTRL_ROFF not readable, function not supported\n\r");
    }
    else
    {
        if(pInstance->ReadableRegs)
        {
            pLayer->LxCtrl = CVC_mReadReg(pInstance->RegAccessObjPtr,
                CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF);
        }
        pLayer->LxCtrl |= LX_CTRL_DIS_TRANSP_MSK;
        CVC_mWriteReg(pInstance->RegAccessObjPtr,
            CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF,
            pLayer->LxCtrl);

        status = cvcRetSuccessE;
    }

    return status;
}


/**
*
* Gets a buffer offset
*
* @param[in]    logiCvcBufferPtr is a pointer to the logiCVC_BufferT structure
*
* @return       Buffer offset.
*
* @note         None.
*
*****************************************************************************/
OsU32 logiCVC_GetOffset(logiCvcBufferPointerT logiCvcBufferPtr)
{
    logiCVC_BufferT * pBuffer = (logiCVC_BufferT *)logiCvcBufferPtr;
    OS_ASSERT(pBuffer != NULL);
    return pBuffer->offset;
}


/**
*
* Write single value into CLUT
*
* @param[in]    logiCvcClutPtr is a pointer to the logiCVC_ClutT structure
* @param[in]    index is the CLUT index
* @param[in]    value is the 32bit value that will be written into CLUT
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_ClutWrite(logiCvcClutPointerT logiCvcClutPtr, OsU8 index, OsU32 value)
{
    logiCVC_ClutT  * pClut = (logiCVC_ClutT *)logiCvcClutPtr;
    logiCVC_LayerT * pLayer;

    OS_ASSERT(pClut  != NULL);
    pLayer = pClut->pLayer;
    OS_ASSERT(pLayer != NULL);

    DEBUG_PRINT("logiCVC_ClutWrite() - layer %d: clutOff 0x%X, idx %d = 0x%X\n\r",
        (int)pLayer->LayerNumber, (int)pClut->offset, index, (int)value);

    CVC_mCLUTWrite(pLayer->CvcBackPtr->RegAccessObjPtr, pClut->offset, index, value);
}


/**
*
* Read single value from CLUT
*
* @param[in]    logiCvcClutPtr is a pointer to the logiCVC_ClutT structure
* @param[in]    index is the CLUT index
*
* @return       32 bit value read from CLUT.
*
* @note         None.
*
*****************************************************************************/
OsU32 logiCVC_ClutRead(logiCvcClutPointerT logiCvcClutPtr, OsU8 index)
{
    logiCVC_ClutT  * pClut = (logiCVC_ClutT *)logiCvcClutPtr;
    logiCVC_LayerT * pLayer;
    OsU32 val;

    OS_ASSERT(pClut  != NULL);
    pLayer = pClut->pLayer;
    OS_ASSERT(pLayer != NULL);


    val = CVC_mCLUTRead(pLayer->CvcBackPtr->RegAccessObjPtr, pClut->offset, index);

    DEBUG_PRINT("logiCVC_ClutRead() - layer %d: clutOff 0x%X, idx %d = 0x%X\n\r",
        (int)pLayer->LayerNumber, index, (int)val);

    return val;
}


/**
*
* Gets the current logiCVC VSYNC state.
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC driver
*
* @return       TRUE if a VSYNC has occured since the last clear.
*
* @note         None.
*
*****************************************************************************/
OsBoolT logiCVC_GetVsyncState(logiCvcPointerT logiCvcPtr)
{
    logiCVC_InstanceT * pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    OS_ASSERT(pInstance != NULL);
    return ((CVC_mReadReg(pInstance->RegAccessObjPtr, CVC_INT_ROFF) & 0x20) != 0);
}


/**
*
* Clears the current logiCVC VSYNC state.
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC driver
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_ClearVsyncState(logiCvcPointerT logiCvcPtr)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    OS_ASSERT(pInstance != NULL);
    CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_ROFF, 0x20);
}


/**
*
* Waits for the next logiCVC VSYNC.
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC driver
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_WaitVsync(logiCvcPointerT logiCvcPtr)
{
    logiCVC_ClearVsyncState(logiCvcPtr);
    while (!logiCVC_GetVsyncState(logiCvcPtr))
    {
        ;
    }
}



/**
*
* Returns logiCVC layer pointer
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC driver
* @param[in]    numLayer is number of layer
*
* @return       Returns logiCVC layer pointer.
*
* @note         None.
*
*****************************************************************************/
logiCvcLayerPointerT logiCVC_GetLayerPointer(logiCvcPointerT logiCvcPtr, OsU16 numLayer)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    logiCvcLayerPointerT ret = NULL;

    OS_ASSERT(pInstance != NULL);

    if (numLayer >= pInstance->NumLayers)
    {
        ERROR_PRINT("logiCVC_GetLayerPointer() - Unsupported layer number %d, supported [0-%d]!\n\r", numLayer, pInstance->NumLayers-1);
    }
    else
    {
        ret = (logiCvcLayerPointerT)&pInstance->LayerInfo[numLayer];
    }
    return ret;
}


/**
*
* Returns different parameters common for whole logiCVC instance
*
* @param[in]    logiCvcPtr is a pointer to the logiCVC driver
* @param[in]    param selects what info to return, see /ref cvcCommonParamE
*
* @return       Returns valid parameter, PARAMETER_INVALID if error.
*
* @note         None.
*
*****************************************************************************/
OsU32 logiCVC_GetCommonParam(logiCvcPointerT logiCvcPtr, cvcCommonParamE param)
{
    logiCVC_InstanceT *pInstance = (logiCVC_InstanceT *)logiCvcPtr;
    OsU32 ret;

    OS_ASSERT(pInstance != NULL);

    switch(param)
    {
        case CVC_HRES_E:
            ret = pInstance->HorizontalRes;
            DEBUG_PRINT("logiCVC_GetCommonParam() - param CVC_HRES_E = 0x%X\n\r", (int)ret);
            break;
        case CVC_VRES_E:
            ret = pInstance->VerticalRes;
            DEBUG_PRINT("logiCVC_GetCommonParam() - param CVC_VRES_E = 0x%X\n\r", (int)ret);
            break;
        case CVC_NUM_LAYERS_E:
            ret = pInstance->NumLayers;
            DEBUG_PRINT("logiCVC_GetCommonParam() - param CVC_NUM_LAYERS_E = 0x%X\n\r", (int)ret);
            break;
        case CVC_USE_BACKGROUND_E:
            ret = pInstance->UseBackground;
            DEBUG_PRINT("logiCVC_GetCommonParam() - param CVC_USE_BACKGROUND_E = 0x%X\n\r", (int)ret);
            break;
        case CVC_STRIDE_E:
            ret = pInstance->LayerInfo[0].Stride;
            DEBUG_PRINT("logiCVC_GetCommonParam() - param CVC_STRIDE_E = 0x%X\n\r", (int)ret);
            break;
        case CVC_VRAM_START_PHY_ADDR_E:
            ret = pInstance->MemoryLowAddr;
            DEBUG_PRINT("logiCVC_GetCommonParam() - param CVC_VRAM_START_PHY_ADDR_E = 0x%X\n\r", (int)ret);
            break;
        case CVC_VRAM_START_VIRT_ADDR_E:
            if (pInstance->VideoAccessObjPtr != NULL)
            {
                ret = (OsU32)OsHwAccessGetVirtualAddress(pInstance->VideoAccessObjPtr);
                DEBUG_PRINT("logiCVC_GetCommonParam() - param CVC_VRAM_START_VIRT_ADDR_E = 0x%X\n\r", (int)ret);
            }
            else
            {
                ret = PARAMETER_INVALID;
                DEBUG_PRINT("logiCVC_GetCommonParam() - param CVC_VRAM_START_VIRT_ADDR_E = 0x%X (unmapped)\n\r", (int)ret);
            }
            break;
        case CVC_VRAM_SIZE_E:
            ret = pInstance->MemoryHighAddr - pInstance->MemoryLowAddr + 1;
            DEBUG_PRINT("logiCVC_GetCommonParam() - param CVC_VRAM_SIZE_E = 0x%X\n\r", (int)ret);
            break;
        default:
            ret = PARAMETER_INVALID;
            DEBUG_PRINT("logiCVC_GetCommonParam() - param INVALID\n\r", (int)ret);
            break;
    }

    return ret;
}


/**
*
* Returns different parameters of particular logiCVC layer
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    param selects what info to return, see /ref cvcLayerParamE
*
* @return       Returns number, PARAMETER_INVALID if error.
*
* @note         None.
*
*****************************************************************************/
OsU32 logiCVC_GetLayerParam(logiCvcLayerPointerT logiCvcLayerPtr, cvcLayerParamE param)
{
    OsU32 ret = PARAMETER_INVALID;
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsU32 numLayer;

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    numLayer  = pLayer->LayerNumber;

    switch(param)
    {
        case CVC_LAYER_BYTE_PER_PIX_E:
            ret = pLayer->BytePerPix;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_BYTE_PER_PIX_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_COLOR_BITS_E:
            ret = pLayer->BitPerPix;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_COLOR_BITS_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_TYPE_E:
            ret = pLayer->Type;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_TYPE_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_ALPHA_MODE_E:
            ret = pLayer->AlphaMode;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_ALPHA_MODE_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_BUFF_A_BYTE_OFFSET_E:
            ret = pLayer->aBuffer[0].offset;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_BUFF_A_BYTE_OFFSET_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_BUFF_B_BYTE_OFFSET_E:
            ret = pLayer->aBuffer[1].offset;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_BUFF_B_BYTE_OFFSET_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_BUFF_C_BYTE_OFFSET_E:
            ret = pLayer->aBuffer[2].offset;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_BUFF_C_BYTE_OFFSET_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_BUFF_BYTE_OFFSET_E:
            ret = pLayer->aBuffer[1].offset - pLayer->aBuffer[0].offset;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_BUFF_BYTE_OFFSET_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_SIZE_BYTES_TO_NEXT_LAYER_E:
            {
                if(numLayer == pInstance->memSortedLayerIndices[pInstance->NumLayers-1])
                    ret = pInstance->MemoryHighAddr - pLayer->aBuffer[0].offset;
                else
                    ret = pInstance->LayerInfo[getNextMemSortedLayerNum(pInstance, numLayer)].aBuffer[0].offset
                            - pLayer->aBuffer[0].offset;
                DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_SIZE_BYTES_TO_NEXT_LAYER_E = 0x%X\n\r", (int)numLayer, (int)ret);
            }
            break;
        case CVC_LAYER_MAX_VRES_NO_BUFFERING_E:
            {
                if(numLayer == pInstance->memSortedLayerIndices[pInstance->NumLayers-1])
                {
                    ret = pInstance->MemoryHighAddr - pLayer->aBuffer[0].offset;
                    /* Calculate number of lines */
                    ret = ret / (pLayer->BytePerPix * pLayer->Stride);
                }
                else
                {
                    ret = pInstance->LayerInfo[getNextMemSortedLayerNum(pInstance, numLayer)].aBuffer[0].offset
                          - pLayer->aBuffer[0].offset;
                    /* Calculate number of lines */
                    ret = ret / (pLayer->BytePerPix * pLayer->Stride);
                }
                DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_MAX_VRES_NO_BUFFERING_E = 0x%X\n\r", (int)numLayer, (int)ret);
            }
            break;
        case CVC_LAYER_MAX_VRES_USE_BUFFERING_E:
            {
                ret = pLayer->aBuffer[1].offset - pLayer->aBuffer[0].offset;
                /* Calculate number of lines */
                ret = ret / (pLayer->BytePerPix * pLayer->Stride);
                DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_MAX_VRES_USE_BUFFERING_E = 0x%X\n\r", (int)numLayer, (int)ret);
            }
            break;
        case CVC_LAYER_NUM_BUFFERS_USABLE_E:
            {
                OsU32 bytesToNextLayerBuffA =
                    logiCVC_GetLayerParam(logiCvcLayerPtr, CVC_LAYER_SIZE_BYTES_TO_NEXT_LAYER_E);
                OsU32 bufferOffset =
                    pLayer->aBuffer[1].offset - pLayer->aBuffer[0].offset;
                if(bytesToNextLayerBuffA < bufferOffset)
                {
                    /* There is not enough space for single buffer */
                    ret = 0;
                }
                else
                {
                    /* Calculate number of buffers */
                    ret = bytesToNextLayerBuffA / bufferOffset;
                    if (ret > LOGICVC_NUM_BUFFERS)
                        ret = LOGICVC_NUM_BUFFERS;
                }
                DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_NUM_BUFFERS_USABLE_E = 0x%X\n\r", (int)numLayer, (int)ret);
            }
            break;
        case CVC_LAYER_GET_PREV_LAYER_IN_MEM_IDX_E:
            ret = getPrevMemSortedLayerNum(pInstance, numLayer);
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_GET_PREV_LAYER_IN_MEM_IDX_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_GET_NEXT_LAYER_IN_MEM_IDX_E:
            ret = getNextMemSortedLayerNum(pInstance, numLayer);
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_GET_NEXT_LAYER_IN_MEM_IDX_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_GET_OUTPUT_POSITION_X_E:
            ret = pLayer->OutputPosition.x_pos;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_GET_OUTPUT_POSITION_X_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_GET_OUTPUT_POSITION_Y_E:
            ret = pLayer->OutputPosition.y_pos;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_GET_OUTPUT_POSITION_Y_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_GET_OUTPUT_POSITION_WIDTH_E:
            ret = pLayer->OutputPosition.width;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_GET_OUTPUT_POSITION_WIDTH_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_GET_OUTPUT_POSITION_HEIGHT_E:
            ret = pLayer->OutputPosition.height;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_GET_OUTPUT_POSITION_HEIGHT_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_GET_TRANSPARENT_COLOR_E:
            ret = pLayer->TransparentColor;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_GET_TRANSPARENT_COLOR_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_IS_ENABLED_E:
            if(pInstance->ReadableRegs)
            {
                pLayer->LxCtrl = CVC_mReadReg(pInstance->RegAccessObjPtr,
                                 CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF);
            }
            ret = (pLayer->LxCtrl & LX_CTRL_EN_LAYER_MSK)?1:0;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_IS_ENABLED_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_GET_X_MEM_OFFSET_E:
            if(pInstance->ReadableRegs)
            {
                pLayer->MemoryXoff = CVC_mReadReg(pInstance->RegAccessObjPtr,
                                     CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_HOR_OFF_ROFF);
            }
            ret = pLayer->MemoryXoff;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_GET_X_MEM_OFFSET_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
        case CVC_LAYER_GET_Y_MEM_OFFSET_E:
            if(pInstance->ReadableRegs)
            {
                pLayer->MemoryYoff = CVC_mReadReg(pInstance->RegAccessObjPtr,
                                     CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_VER_OFF_ROFF);
            }
            ret = pLayer->MemoryYoff;
            DEBUG_PRINT("logiCVC_GetLayerParam() - layer %d, param CVC_LAYER_GET_Y_MEM_OFFSET_E = 0x%X\n\r", (int)numLayer, (int)ret);
            break;
    }

    return ret;
}

/** @} */
