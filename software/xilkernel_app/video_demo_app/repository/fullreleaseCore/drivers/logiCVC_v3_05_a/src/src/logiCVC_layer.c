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

/** \addtogroup logiCVCmidLayer logiCVC middle-level driver - Layer manipulation functions
    \ingroup logiCVCmid
 *   @{
 */

/**
*
* @file logiCVC_layer.c
*
* Contains logiCVC Layer manipulation functions.
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
* Sets logiCVC layer outputPosition.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    outputPosition is rectangle that defines the output size and position
*
* @return
*
* - cvcRetSuccessE if output position is set.
* - cvcRetFailureE if output position is out of limits.
*
* @note
*
* OutputPosition x_pos must be less then logiCVC Horisontal resolution or this function will just return
* OutputPosition y_pos must be less then logiCVC Vertical resolution or this function will just return
*
*****************************************************************************/
cvcRetT logiCVC_SetOutputPosition(logiCvcLayerPointerT logiCvcLayerPtr, XRECT *outputPosition)
{
    logiCVC_LayerT    * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsU32               numLayer;

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    numLayer  = pLayer->LayerNumber;

    /*  NOTE: setting output position is permitted on last layer only if there is also backgound layer enabled */
    if( (!pInstance->UseBackground && numLayer >= pInstance->NumLayers-1) ||
        ( pInstance->UseBackground && numLayer >= pInstance->NumLayers  )   )
    {
        ERROR_PRINT("logiCVC_SetOutputPosition() - Unsupported layer number %d!\n\r", (int)numLayer);
        return cvcRetFailureE;
    }

    /* Check if we are still inside the hres*vres frame */
    if (((OsU32)(outputPosition->width + outputPosition->x_pos)) > (pInstance->HorizontalRes))
    {
        if (outputPosition->x_pos >= pInstance->HorizontalRes)
        {
            ERROR_PRINT("logiCVC_SetOutputPosition() error - layer = %d : x = %d, y = %d, w = %d, h = %d, hres = %d, vres = %d\n\r", \
                         numLayer,
                         outputPosition->x_pos,
                         outputPosition->y_pos,
                         outputPosition->width,
                         outputPosition->height,
                        (int)pInstance->HorizontalRes,
                        (int)pInstance->VerticalRes);
            return cvcRetFailureE;
        }
        else
        {
            outputPosition->width = (unsigned short)
                (pInstance->HorizontalRes - outputPosition->x_pos);
        }
    }

    if (((OsU32)(outputPosition->height + outputPosition->y_pos)) > (pInstance->VerticalRes))
    {
        if (outputPosition->y_pos >= pInstance->VerticalRes)
        {
            ERROR_PRINT("logiCVC_SetOutputPosition() error - layer = %d : x = %d, y = %d, w = %d, h = %d, hres = %d, vres = %d\n\r", \
                         numLayer,
                         outputPosition->x_pos,
                         outputPosition->y_pos,
                         outputPosition->width,
                         outputPosition->height,
                        (int)pInstance->HorizontalRes,
                        (int)pInstance->VerticalRes);
            return cvcRetFailureE;
        }
        else
        {
            outputPosition->height = (unsigned short)
                (pInstance->VerticalRes - outputPosition->y_pos);
        }
    }

    CVC_mLayerWidthWrite(pInstance->RegAccessObjPtr, numLayer, outputPosition->width-1);
    CVC_mLayerHeightWrite(pInstance->RegAccessObjPtr, numLayer, outputPosition->height-1);
    CVC_mLayerPositionHorWrite(pInstance->RegAccessObjPtr,
                               numLayer,
                               outputPosition->x_pos,
                              (OsU16)pInstance->HorizontalRes-1);
    /* Last is vertical position! */
    CVC_mLayerPositionVerWrite(pInstance->RegAccessObjPtr,
                               numLayer,
                               outputPosition->y_pos,
                              (OsU16)pInstance->VerticalRes-1);
    /* Store new settings */
    pLayer->OutputPosition = *outputPosition;

    DEBUG_PRINT("logiCVC_SetOutputPosition() - layer %d : x %d, y %d, w %d, h %d, hres %d, vres %d\n\r", \
                numLayer,
                outputPosition->x_pos,
                outputPosition->y_pos,
                outputPosition->width,
                outputPosition->height,
                (int)pInstance->HorizontalRes,
                (int)pInstance->VerticalRes);

    return cvcRetSuccessE;
}


/**
*
* Sets logiCVC layer memory offset.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    x_pos is the horizontal pixel offset starting from layer top left corner
* @param[in]    y_pos is the vertical pixel offset starting from layer top left corner
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_SetMemoryOffset(logiCvcLayerPointerT logiCvcLayerPtr, OsU16 x_pos, OsU16 y_pos)
{
    logiCVC_LayerT    * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsU32               numLayer;

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    numLayer  = pLayer->LayerNumber;

    DEBUG_PRINT("logiCVC_SetMemoryOffset() - layer %d: %d %d\n\r", numLayer, x_pos, y_pos);

    CVC_mLayerMemOffsetHorWrite(pInstance->RegAccessObjPtr, numLayer, x_pos);
    CVC_mLayerMemOffsetVerWrite(pInstance->RegAccessObjPtr, numLayer, y_pos);
    pLayer->MemoryXoff = x_pos;
    pLayer->MemoryYoff = y_pos;
    /* Store new settings: */
    /* Must write this also for HW to make the change, last is vertical position! */
    CVC_mLayerPositionVerWrite(pInstance->RegAccessObjPtr,
                               numLayer,
                               pLayer->OutputPosition.y_pos,
                              (OsU16)pInstance->VerticalRes-1);
}


/**
*
* Sets control flags of logiCVC layer control register.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    mask is combination of bitmasks defined in cvcLayerControlBitMaskE
*
* @return
* - cvcRetSuccessE.
*
* @note         this function can be used for direct access to layer control register bits,
*               this is recomended use only if registers are not readable and you have
*               initalized CVC driver before driver start.
*
*****************************************************************************/
cvcRetT logiCVC_SetLayerControlFlags(logiCvcLayerPointerT logiCvcLayerPtr, cvcLayerControlBitMaskE mask)
{
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    OS_ASSERT(pLayer != NULL);
    DEBUG_PRINT("logiCVC_SetLayerControlFlags() - layer %d flags 0x%X\n\r", (int)pLayer->LayerNumber, (int)mask);

    pLayer->LxCtrl = mask;
    CVC_mWriteReg(pLayer->CvcBackPtr->RegAccessObjPtr,
        CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE * pLayer->LayerNumber + CVC_LAYER_CTRL_ROFF,
        mask);

    return cvcRetSuccessE;
}


/**
*
* Sets a logiCVC layer pixel format.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    pixelFormat defines the format to set, see \ref LX_CTRL_PIXEL_FORMAT_0 and 
                \ref LX_CTRL_PIXEL_FORMAT_1
*
* @return
* - cvcRetSuccessE if the format is successfully set.
* - cvcRetFailureE otherwise.
*
* @note         None.
*
*****************************************************************************/
cvcRetT
logiCVC_SetLayerPixelFormat(
    logiCvcLayerPointerT    logiCvcLayerPtr,
    cvcLayerControlBitMaskE pixelFormat)
{
    cvcRetT             status = cvcRetFailureE;
    logiCVC_LayerT    * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsSizeT             numLayer;
    OsU32               format;

    PATH_TESTED();

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    OS_ASSERT(pInstance != NULL);
    numLayer = pLayer->LayerNumber;
    format   = pixelFormat & LX_CTRL_PIXEL_FORMAT_MSK;
    OS_ASSERT(format <= LX_CTRL_PIXEL_FORMAT_1);

    DEBUG_PRINT("logiCVC_SetLayerPixelFormat(0x%08X) - layer %d\n\r", format, (int)numLayer);

    if ((!pInstance->ReadableRegs) && (pInstance->CoreInitBeforeDriverStart))
    {
        PATH_NOT_YET_TESTED();
        ERROR_PRINT("logiCVC_SetLayerPixelFormat() - register CVC_LAYER_CTRL_ROFF not readable, function not supported\n\r");
    }
    else
    {
        PATH_TESTED();

        if (pInstance->ReadableRegs)
        {
        	PATH_TESTED();
            pLayer->LxCtrl = CVC_mReadReg(pInstance->RegAccessObjPtr,
                CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF);
        }
        pLayer->LxCtrl &= ~LX_CTRL_PIXEL_FORMAT_MSK;
        pLayer->LxCtrl |=  format;
        CVC_mWriteReg(pInstance->RegAccessObjPtr,
            CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE*numLayer + CVC_LAYER_CTRL_ROFF,
            pLayer->LxCtrl);

        status = cvcRetSuccessE;
    }

    return status;
}


/**
*
* Enables vbuff or CLUT interrupt for particular logiCVC Layer
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    mode signals which interrupt to enable, see \ref logiCVC_layerInterruptModeE.
*               Possible values are: LOGICVC_BOTH_INT, LOGICVC_VBUFf_INT, LOGICVC_CLUT_INT
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_EnLayerInt(logiCvcLayerPointerT logiCvcLayerPtr, logiCVC_layerInterruptModeE mode)
{
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsU32               numLayer;

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    numLayer  = pLayer->LayerNumber;

    DEBUG_PRINT("logiCVC_EnLayerInt() - layer %d mode %d\n\r", (int)numLayer, (int)mode);

    if (mode == LOGICVC_BOTH_INT)
        pInstance->IntMask &= ~(0x101 << numLayer);
    else if (mode == LOGICVC_VBUFF_INT)
        pInstance->IntMask &= ~(0x001 << numLayer);
    else if (mode == LOGICVC_CLUT_INT)
        pInstance->IntMask &= ~(0x100 << numLayer);
    else
        return;

    CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_MASK_ROFF, pInstance->IntMask);
}


/**
*
* Disables vbuff or CLUT interrupt for particular logiCVC Layer
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    mode signals which interrupt to disable. LOGICVC_BOTH_INT, LOGICVC_VBUFf_INT, LOGICVC_CLUT_INT
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void logiCVC_DisLayerInt(logiCvcLayerPointerT logiCvcLayerPtr, logiCVC_layerInterruptModeE mode)
{
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    logiCVC_InstanceT * pInstance;
    OsU32               numLayer;

    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    numLayer  = pLayer->LayerNumber;

    DEBUG_PRINT("logiCVC_DisLayerInt() - layer %d mode %d\n\r", (int)numLayer, (int)mode);

    if (mode == LOGICVC_BOTH_INT)
        pInstance->IntMask |= 0x101 << numLayer;
    else if (mode == LOGICVC_VBUFF_INT)
        pInstance->IntMask |= 0x001 << numLayer;
    else if (mode == LOGICVC_CLUT_INT)
        pInstance->IntMask |= 0x100 << numLayer;
    else
        return;

    CVC_mWriteReg(pInstance->RegAccessObjPtr, CVC_INT_MASK_ROFF, pInstance->IntMask);
}


/**
*
* Gets the maximal layer alpha value.
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    pMaxLayerAlphaValue is a pointer to the location
*               to get the max layer alpha value
*
* @return
*
* - cvcRetSuccessE if transparent color is set.
* - cvcRetFailureE if transparent color isn't set.
*
* @note         None.
*
*****************************************************************************/
cvcRetT logiCVC_GetMaxLayerAlphaValue(logiCvcLayerPointerT logiCvcLayerPtr, OsU32 * pMaxLayerAlphaValue)
{
    cvcRetT status = cvcRetSuccessE;
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    OS_ASSERT(pLayer != NULL);

    if(pLayer->BitPerPix == 8)
        *pMaxLayerAlphaValue = 7; /* for 3 bit alpha value maximal alpha value is 7 */
    else if(pLayer->BitPerPix == 16)
        *pMaxLayerAlphaValue = 63; /* for 6 bit alpha value maximal alpha value is 63 */
    else if(pLayer->BitPerPix == 24)
        *pMaxLayerAlphaValue = 255; /* for 8 bit alpha value maximal alpha value is 255 */
    else
        status = cvcRetFailureE;

    DEBUG_PRINT("logiCVC_GetMaxLayerAlphaValue() - layer %d: maxAlpha %d\n\r", (int)pLayer->LayerNumber, (int)(*pMaxLayerAlphaValue));
    return status;
}

/**
*
* Sets the layer alpha of the layer
*
* @param[in]    logiCvcLayerPtr is a logiCVC layer pointer
* @param[in]    layerAlphaValue is the layer alpha value. User has to make sure that he
*               supplies 3, 6 or 8 bit value to the function depending on layer
*               color depth. So, for 16bpp layer 6 bit alpha value is used.
*
* @return
*
* - cvcRetSuccessE if transparent color is set.
* - cvcRetFailureE if transparent color isn't set.
*
* @note         None.
*
*****************************************************************************/
cvcRetT logiCVC_SetLayerAlphaValue(logiCvcLayerPointerT logiCvcLayerPtr, OsU32 layerAlphaValue)
{
    cvcRetT status = cvcRetFailureE;
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    OsU32 numLayer;

    OS_ASSERT(pLayer != NULL);
    numLayer  = pLayer->LayerNumber;
    DEBUG_PRINT("logiCVC_SetLayerAlphaValue() - layer %d: alpha %d\n\r", (int)numLayer, (int)layerAlphaValue);

    if (pLayer->AlphaMode != LOGICVC_ALPHA_MODE_LAYER)
    {
        ERROR_PRINT("logiCVC_SetLayerAlphaValue() - layer %d does not support layer alpha!\n\r", numLayer);
    }
    else
    {
        CVC_mWriteReg(pLayer->CvcBackPtr->RegAccessObjPtr,
                     (CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE * numLayer + CVC_LAYER_ALPHA_ROFF),
                      layerAlphaValue);
        status = cvcRetSuccessE;
    }

    return status;
}


/* @} */
