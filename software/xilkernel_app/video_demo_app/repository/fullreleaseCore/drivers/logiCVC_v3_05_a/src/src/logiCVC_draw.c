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

/** \addtogroup logiCVCmidDraw logiCVC middle-level driver - Drawing Functions
    \ingroup logiCVCmid
 *   @{
 */

/**
*
* @file logiCVC_draw.c
*
* Contains logiCVC drawing functions.
* See the logiCVC.h header file for more details on this driver.
*
*****************************************************************************/

/***************************** Include Files ********************************/
#include "logiCVC.h"
#include "logiCVC_l.h"
#include "logiCVC_instance.h"
#include "logiCVC_disp_i.h"


/***************************** Macro Definitions *****************************/

#define PRINT(format, ...)          OsPrintfInfo(TEXT(format),  ##__VA_ARGS__)
#define ERROR_PRINT(format, ...)    OsPrintfError(TEXT(format), ##__VA_ARGS__)
#if (defined(GENDRV_CVC_DEBUG) || defined(GENDRV_GLOBAL_DEBUG))
#define DEBUG_PRINT(format, ...)    OsPrintfInfo(TEXT(format),  ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...)
#endif



/************************** Function Declarations ****************************/
OsSizeT
logiCVC_GetY0BitOffset(
    cvcLayerControlBitMaskE pixelFormat);

OsSizeT
logiCVC_GetY1BitOffset(
    cvcLayerControlBitMaskE pixelFormat);

OsSizeT
logiCVC_GetCbBitOffset(
    cvcLayerControlBitMaskE pixelFormat);

OsSizeT
logiCVC_GetCrBitOffset(
    cvcLayerControlBitMaskE pixelFormat);

OsU8
logiCVC_LimitColorComponent(
	OsS32 val);



/************************** Function Definitions *****************************/

/**
*
* Reports the Y0 color component bit offset for the 16-bit (4:2:2) YCbCr
* format based on the specified pixel format.
*
* @param[in]    pixelFormat is the pixel format
*
* @return       Y0 component bit offset (0, 8, 16 or 24).
*
*****************************************************************************/
OS_INLINE
OsSizeT
logiCVC_GetY0BitOffset(
    cvcLayerControlBitMaskE pixelFormat)
{
    static OsU8 const aMask[2] =
    {
        0*8, 1*8
    };
    PATH_TESTED();
    OS_ASSERT(pixelFormat <= LX_CTRL_PIXEL_FORMAT_1);
    return aMask[!!pixelFormat];
}


/**
*
* Reports the Y1 color component bit offset for the 16-bit (4:2:2) YCbCr
* format based on the specified pixel format.
*
* @param[in]    pixelFormat is the pixel format
*
* @return       Y1 component bit offset (0, 8, 16 or 24).
*
*****************************************************************************/
OS_INLINE
OsSizeT
logiCVC_GetY1BitOffset(
    cvcLayerControlBitMaskE pixelFormat)
{
    static OsU8 const aMask[2] =
    {
        2*8, 3*8
    };
    PATH_TESTED();
    OS_ASSERT(pixelFormat <= LX_CTRL_PIXEL_FORMAT_1);
    return aMask[!!pixelFormat];
}


/**
*
* Reports the Cb color component bit offset for the 16-bit (4:2:2) YCbCr
* format based on the specified pixel format.
*
* @param[in]    pixelFormat is the pixel format
*
* @return       Cb component bit offset (0, 8, 16 or 24).
*
*****************************************************************************/
OS_INLINE
OsSizeT
logiCVC_GetCbBitOffset(
    cvcLayerControlBitMaskE pixelFormat)
{
    static OsU8 const aMask[2] =
    {
        1*8, 0*8
    };
    PATH_TESTED();
    OS_ASSERT(pixelFormat <= LX_CTRL_PIXEL_FORMAT_1);
    return aMask[!!pixelFormat];
}


/**
*
* Reports the Cr color component bit offset for the 16-bit (4:2:2) YCbCr
* format based on the specified pixel format.
*
* @param[in]    pixelFormat is the pixel format
*
* @return       Cr component bit offset (0, 8, 16 or 24).
*
*****************************************************************************/
OS_INLINE
OsSizeT
logiCVC_GetCrBitOffset(
    cvcLayerControlBitMaskE pixelFormat)
{
    static OsU8 const aMask[2] =
    {
        3*8, 2*8
    };
    PATH_TESTED();
    OS_ASSERT(pixelFormat <= LX_CTRL_PIXEL_FORMAT_1);
    return aMask[!!pixelFormat];
}


/**
*
* Limits a color component value to [0, 255].
*
* @param[in]    val is a color component value
*
* @return       Value limited to [0, 255].
*
*****************************************************************************/
OS_INLINE
OsU8
logiCVC_LimitColorComponent(
	OsS32 val)
{
    PATH_TESTED();
	return (OsU8)((val < 0) ? 0 : ((val > 255) ? 255 : val));
}


/**
*
* Converts an AYCbCr 32-bit value to an ARGB value.
*
* @param[in]    aycbcr is an AYCbCr 32-bit value to be converted
*
* @return       ARGB value representing the given AYCbCr value.
*
*****************************************************************************/
OsU32
logiCVC_ConvertAycbcr32ToArgb32(
    OsU32 aycbcr)
{
    OsS32 a  = (aycbcr >> 24) & 0xFF;
    OsS32 y  = (aycbcr >> 16) & 0xFF;
    OsS32 cb = (aycbcr >>  8) & 0xFF;
    OsS32 cr = (aycbcr >>  0) & 0xFF;
    OsU32 r;
    OsU32 g;
    OsU32 b;

    PATH_TESTED();

	r = logiCVC_LimitColorComponent(
	    ((1000000 * y) + (1402524 * cr)                  - (1000000 * 179)) / 1000000);

	g = logiCVC_LimitColorComponent(
		((1000000 * y) - ( 714403 * cr) - ( 344340 * cb) + (1000000 * 135)) / 1000000);

    b = logiCVC_LimitColorComponent(
		((1000000 * y)                  + (1773049 * cb) - (1000000 * 226)) / 1000000);

    return (a << 24) | (r << 16) | (g << 8) | (b << 0);
}


/**
*
* Converts an ARGB 32-bit value to an AYCbCr value.
*
* @param[in]    argb is an ARGB 32-bit value to be converted
*
* @return       AYCbCr value representing the given ARGB value.
*
*****************************************************************************/
OsU32
logiCVC_ConvertArgb32ToAycbcr32(
    OsU32 argb)
{
    OsS32 a = (argb >> 24) & 0xFF;
    OsS32 r = (argb >> 16) & 0xFF;
    OsS32 g = (argb >>  8) & 0xFF;
    OsS32 b = (argb >>  0) & 0xFF;
    OsU32 y;
    OsU32 cb;
    OsU32 cr;

    PATH_TESTED();

    y  = logiCVC_LimitColorComponent(
        ( (29900 * r) + (58700 * g) + (11400 * b)           ) / 100000);

    cr = logiCVC_LimitColorComponent(
        ( (49980 * r) - (41850 * g) - ( 8128 * b) + 12800000) / 100000);
    
    cb = logiCVC_LimitColorComponent(
        (-(16868 * r) - (33107 * g) + (49970 * b) + 12800000) / 100000);

    return (a << 24) | (y << 16) | (cb << 8) | (cr << 0);
}


/**
*
* Formats/converts an ARGB 32-bit value to the layer specific format.
*
* @param[in]    logiCvcLayerPtr is a pointer to the logiCVC_LayerT structure
* @param[in]    argb is an ARGB 32-bit value to be converted
*
* @return       Pixel color and alpha in the layer specific format
*
* @note
* For 16-bit YCbCr layers, both Y0 and Y1 are reported.
*
*****************************************************************************/
OsU32
logiCVC_ConvertArgb32ToLayerColorFormat(
    logiCvcLayerPointerT logiCvcLayerPtr,
    OsU32                argb)
{
    logiCVC_LayerT * pLayer = (logiCVC_LayerT *)logiCvcLayerPtr;
    OsU32            color = 0;

    PATH_TESTED();

    if (   (LOGICVC_ALPHA_MODE_LAYER == pLayer->AlphaMode)
    	|| (LOGICVC_ALPHA_MODE_PIXEL == pLayer->AlphaMode) )
    {
		logiCVC_InstanceT     * pInstance;
		OsS32                   a = (argb >> 24) & 0xFF;
		OsS32                   r = (argb >> 16) & 0xFF;
		OsS32                   g = (argb >>  8) & 0xFF;
		OsS32                   b = (argb >>  0) & 0xFF;
		OsS32                   y;
		OsS32                   cr;
		OsS32                   cb;
		cvcLayerControlBitMaskE pixelFormat;

		PATH_TESTED();

		OS_ASSERT(pLayer != NULL);
		pInstance = pLayer->CvcBackPtr;
		OS_ASSERT(pInstance != NULL);
		pixelFormat = pLayer->LxCtrl & LX_CTRL_PIXEL_FORMAT_MSK;
		OS_ASSERT(pixelFormat <= LX_CTRL_PIXEL_FORMAT_1);

		switch (pLayer->Type)
		{
		case CVC_LAYER_TYPE__RGB:
			PATH_TESTED();
			switch (pLayer->BytePerPix)
			{
			case 1:
				PATH_TESTED();
				if (pixelFormat != 0)
				{
					PATH_TESTED();
					color = ((r >> 5) << 0) | ((g >> 5) << 3) | ((b >> 6) << 6);
				}
				else
				{
					PATH_TESTED();
					color = ((r >> 5) << 5) | ((g >> 5) << 2) | ((b >> 6) << 0);
				}
				break;

			case 2:
				PATH_TESTED();
				if (8 == pLayer->BitPerPix)
				{
					PATH_TESTED();
					if (pixelFormat != 0)
					{
						PATH_TESTED();
						color = ((a >> 5) << 8) | ((r >> 5) << 0) | ((g >> 5) << 3) | ((b >> 6) << 6);
					}
					else
					{
						PATH_TESTED();
						color = ((a >> 5) << 8) | ((r >> 5) << 5) | ((g >> 5) << 2) | ((b >> 6) << 0);
					}
				}
				else
				{
					PATH_TESTED();
					if (pixelFormat != 0)
					{
						PATH_TESTED();
						color = ((r >> 3) <<  0) | ((g >> 2) <<  5) | ((b >> 3) << 11);
					}
					else
					{
						PATH_TESTED();
						color = ((r >> 3) << 11) | ((g >> 2) <<  5) | ((b >> 3) <<  0);
					}
				}
				break;

			case 4:
				PATH_TESTED();
				if (16 == pLayer->BitPerPix)
				{
					PATH_TESTED();
					if (pixelFormat != 0)
					{
						PATH_TESTED();
						color = ((a >> 2) << 24) | ((r >> 3) <<  0) | ((g >> 2) <<  5) | ((b >> 3) << 11);
					}
					else
					{
						PATH_TESTED();
						color = ((a >> 2) << 24) | ((r >> 3) << 11) | ((g >> 2) <<  5) | ((b >> 3) <<  0);
					}
				}
				else
				{
					PATH_TESTED();
					if (pixelFormat != 0)
					{
						PATH_TESTED();
						color = (a << 24) | (r <<  0) | (g <<  8) | (b << 16);
					}
					else
					{
						PATH_TESTED();
						color = (a << 24) | (r << 16) | (g <<  8) | (b <<  0);
					}
				}
				break;

			default:
				OS_ASSERT(FALSE);
				break;
			}
			break;

		case CVC_LAYER_TYPE__YCBCR:
			PATH_TESTED();

			y  = logiCVC_LimitColorComponent(
				( (29900 * r) + (58700 * g) + (11400 * b)           ) / 100000);
			cr = logiCVC_LimitColorComponent(
				( (49980 * r) - (41850 * g) - ( 8128 * b) + 12800000) / 100000);
			cb = logiCVC_LimitColorComponent(
				(-(16868 * r) - (33107 * g) + (49970 * b) + 12800000) / 100000);

			switch (pLayer->BytePerPix)
			{
			case 2:
				PATH_TESTED();
				if (pixelFormat != 0)
				{
					PATH_TESTED();
					color = (cr << 16) | (y << 24) | (cb << 0) | (y << 8);
				}
				else
				{
					PATH_TESTED();
					color = (cr << 24) | (y << 16) | (cb << 8) | (y << 0);
				}
				break;

			case 4:
				PATH_TESTED();
				if (pixelFormat != 0)
				{
					PATH_TESTED();
					color = (a << 24) | (y <<  0) | (cb << 8) | (cr << 16);
				}
				else
				{
					PATH_TESTED();
					color = (a << 24) | (y << 16) | (cb << 8) | (cr <<  0);
				}
				break;

			default:
				OS_ASSERT(FALSE);
				break;
			}
			break;

		case CVC_LAYER_TYPE__ALPHA:
			PATH_TESTED();
			OS_ASSERT(1 == pLayer->BytePerPix);
			color = a;
			break;

		default:
			OS_ASSERT(FALSE);
			break;
		}
    }

    return color;
}


/**
*
* Retrieves a pixel (color+alpha) from a logiCVC buffer.
*
* @param[in]    logiCvcBufferPtr is a pointer to the logiCVC_BufferT structure
* @param[in]    xOff is the horizontal pixel offset starting from buffer top left corner
* @param[in]    yOff is the vertical pixel offset starting from buffer top left corner
*
* @return       Pixel color and alpha in the buffer specific format
*
* @note
* For 16-bit YCbCr buffers, both Y0 and Y1 are retrieved.
*
*****************************************************************************/
OsU32
logiCVC_GetPixel(
    logiCvcBufferPointerT logiCvcBufferPtr,
    OsU16                 xOff,
    OsU16                 yOff)
{
    logiCVC_BufferT       * pBuffer = (logiCVC_BufferT *)logiCvcBufferPtr;
    logiCVC_LayerT        * pLayer;
    logiCVC_InstanceT     * pInstance;
    OsU32                   color = 0;

    PATH_TESTED();

    OS_ASSERT(pBuffer != NULL);
    pLayer = pBuffer->pLayer;
    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    OS_ASSERT(pInstance != NULL);

    if (pInstance->VideoAccessObjPtr != NULL)
    {
        OsSizeT pixAddr;

        PATH_TESTED();

        if ((CVC_LAYER_TYPE__YCBCR == pLayer->Type) && (2 == pLayer->BytePerPix))
        {
        	PATH_TESTED();

            pixAddr = pBuffer->offset
                    + pLayer->BytePerPix * ((xOff & ~1) + yOff * pLayer->Stride);

            color = OsRegRead32(pInstance->VideoAccessObjPtr, pixAddr);
        }
        else
        {
        	PATH_TESTED();

            pixAddr = pBuffer->offset
                    + pLayer->BytePerPix * (xOff + yOff * pLayer->Stride);

            switch (pLayer->BytePerPix)
            {
            case 1:
            	PATH_TESTED();
                color = OsRegRead8(pInstance->VideoAccessObjPtr, pixAddr);
                break;

            case 2:
            	PATH_TESTED();
                color = OsRegRead16(pInstance->VideoAccessObjPtr, pixAddr);
                break;

            case 4:
            	PATH_TESTED();
                color = OsRegRead32(pInstance->VideoAccessObjPtr, pixAddr);
                break;

            default:
                OS_ASSERT(FALSE);
                break;
            }
        }
    }

    return color;
}


/**
*
* Draws a pixel in a logiCVC buffer.
*
* @param[in]    logiCvcBufferPtr is a pointer to the logiCVC_BufferT structure
* @param[in]    xOff is the horizontal pixel offset starting from buffer top left corner
* @param[in]    yOff is the vertical pixel offset starting from buffer top left corner
* @param[in]    color is 32/16/8 bit color value to be written, given in
*               the buffer specific format (see notes for 16-bit YCbCr)
*
* @return       None.
*
* @note
* color argument is 32bit, but the value that will be written to memory
* depends on the layer BytePerPix parameter.
* For 16-bit YCbCr buffers, both Y0 and Y1 must be given.
*
*****************************************************************************/
void
logiCVC_DrawPixel(
    logiCvcBufferPointerT logiCvcBufferPtr,
    OsU16                 xOff,
    OsU16                 yOff,
    OsU32                 color)
{
    logiCVC_BufferT       * pBuffer = (logiCVC_BufferT *)logiCvcBufferPtr;
    logiCVC_LayerT        * pLayer;
    logiCVC_InstanceT     * pInstance;
    cvcLayerControlBitMaskE pixelFormat;

    PATH_TESTED();

    OS_ASSERT(pBuffer != NULL);
    pLayer = pBuffer->pLayer;
    OS_ASSERT(pLayer != NULL);
    pInstance = pLayer->CvcBackPtr;
    OS_ASSERT(pInstance != NULL);
    pixelFormat = pLayer->LxCtrl & LX_CTRL_PIXEL_FORMAT_MSK;

    if (pInstance->VideoAccessObjPtr != NULL)
    {
        OsSizeT pixAddr;

        PATH_TESTED();

        if ((CVC_LAYER_TYPE__YCBCR == pLayer->Type) && (2 == pLayer->BytePerPix))
        {
            OsU32   pixel;
            OsSizeT y1Mask;

            PATH_TESTED();

            pixAddr = pBuffer->offset
                    + pLayer->BytePerPix * ((xOff & ~1) + yOff * pLayer->Stride);

            pixel = OsRegRead32(pInstance->VideoAccessObjPtr, pixAddr);

            y1Mask = 0xFF << logiCVC_GetY1BitOffset(pixelFormat);

            if (xOff & 1) 
            {
            	PATH_TESTED();

                pixel &= ~y1Mask;
                
                pixel |= color & y1Mask;
            }
            else
            {
            	PATH_TESTED();

                pixel &= y1Mask;

                pixel |= color & ~y1Mask;
            }

            OsRegWrite32(pInstance->VideoAccessObjPtr, pixAddr, pixel);
        }
        else
        {
        	PATH_TESTED();

            pixAddr = pBuffer->offset
                    + pLayer->BytePerPix * (xOff + yOff * pLayer->Stride);

            switch (pLayer->BytePerPix)
            {
            case 1:
            	PATH_TESTED();
                OsRegWrite8(pInstance->VideoAccessObjPtr, pixAddr, (OsU8) color);
                break;

            case 2:
            	PATH_TESTED();
                OsRegWrite16(pInstance->VideoAccessObjPtr, pixAddr, (OsU16) color);
                break;

            case 4:
            	PATH_TESTED();
                OsRegWrite32(pInstance->VideoAccessObjPtr, pixAddr, color);
                break;

            default:
                OS_ASSERT(FALSE);
                break;
            }
        }
    }
}


/**
*
* Copies a rectangular area of user data to a logiCVC buffer.
*
* @param[in]    logiCvcBufferPtr is a pointer to the logiCVC_BufferT structure
* @param[in]    pDestRect points to x and y offsets, and width and height
*               of the rectangle
* @param[in]    pSrc points to the user data (image)
* @param[in]    srcStride is the amount of bytes between two adjacent lines
*               in source image. This is useful when copying only a part of
*               the image (stored in linear memory), or if you have a
*               rectangular source image. When copying a whole image from
*               linear memory, use the width specified by pDestRect
*
* @return       None.
*
* @note
* The source image data have to be given in the buffer specific format.
* For 16-bit YCbCr buffers, x_pos and width of the destination rectangle
* must both be even numbers.
*
*****************************************************************************/
void
logiCVC_CopyRectangle(
    logiCvcBufferPointerT logiCvcBufferPtr,
    XRECT               * pDestRect,
    OsU8                * pSrc,
    OsU16                 srcStride)
{
    logiCVC_BufferT * pBuffer = (logiCVC_BufferT *)logiCvcBufferPtr;
    logiCVC_LayerT  * pLayer;

    PATH_TESTED();

    OS_ASSERT(pBuffer != NULL);
    pLayer = pBuffer->pLayer;
    OS_ASSERT(pLayer != NULL);

    if (pLayer->CvcBackPtr->VideoAccessObjPtr != NULL)
    {
        OsSizeT y;
        OsSizeT xPos          = pDestRect->x_pos;
        OsSizeT width         = pDestRect->width;
        OsSizeT bytesInStride = pLayer->BytePerPix * pLayer->Stride;
        OsSizeT bytesInLine;
        OsSizeT pixAddr;

        PATH_TESTED();

        if ((CVC_LAYER_TYPE__YCBCR == pLayer->Type) && (2 == pLayer->BytePerPix))
        {
        	PATH_TESTED();

        	OS_ASSERT((srcStride & 3) == 0);
            OS_ASSERT((((OsSizeT)pSrc) & 3) == 0);
            OS_ASSERT((xPos  & 1) == 0);
            OS_ASSERT((width & 1) == 0);

            xPos  &= ~1;
            width &= ~1;
        }

		bytesInLine = pLayer->BytePerPix * width;
		pixAddr     = pBuffer->offset +
			(xPos + pDestRect->y_pos * pLayer->Stride) * pLayer->BytePerPix;

		for (y=0; y < pDestRect->height; ++y)
		{
			PATH_TESTED();
			OsRegMultiWrite8(pLayer->CvcBackPtr->VideoAccessObjPtr, pixAddr, pSrc, bytesInLine);
			pixAddr += bytesInStride;
			pSrc    += srcStride;
		}
    }
}


/**
*
* Fills a rectangular area with a color.
*
* @param[in]    logiCvcBufferPtr is a pointer to the logiCVC_BufferT structure
* @param[in]    pRect points to x and y offsets, and width and height
*               of the rectangle
* @param[in]    color is 32/16/8 bit color value to be written
*
* @return       None.
*
* @note
* For 16-bit YCbCr buffers, both Y0 and Y1 must be given.
*
*****************************************************************************/
void
logiCVC_FillRectangle(
    logiCvcBufferPointerT logiCvcBufferPtr,
    XRECT               * pRect,
    OsU32                 color)
{
    logiCVC_BufferT * pBuffer = (logiCVC_BufferT *)logiCvcBufferPtr;
    logiCVC_LayerT  * pLayer;

    PATH_TESTED();

    OS_ASSERT(pBuffer != NULL);
    pLayer = pBuffer->pLayer;
    OS_ASSERT(pLayer != NULL);

    if (pLayer->CvcBackPtr->VideoAccessObjPtr != NULL)
    {
        OsSizeT numOfStartPixels  = 0;
        OsSizeT numOfMiddlePixels = pRect->width;
        OsSizeT x;
        OsSizeT y;

        PATH_TESTED();

        if ((CVC_LAYER_TYPE__YCBCR == pLayer->Type) && (2 == pLayer->BytePerPix))
        {
        	PATH_TESTED();

    		if (numOfMiddlePixels > 0)
    		{
    	        OsSizeT numOfEndPixels = (pRect->x_pos + numOfMiddlePixels) & 1;

    	        PATH_TESTED();

    	        numOfStartPixels = pRect->x_pos & 1;

    			if (numOfStartPixels > 0)
    			{
    				PATH_TESTED();
    				numOfMiddlePixels -= numOfStartPixels;
    				x = pRect->x_pos;
    				for (y=0; y < pRect->height; ++y)
    				{
    					PATH_TESTED();
						logiCVC_DrawPixel(logiCvcBufferPtr, x, pRect->y_pos + y, color);
    				}
    			}

    			if ((numOfEndPixels > 0) && (numOfMiddlePixels > 0))
    			{
    				PATH_TESTED();
    				numOfMiddlePixels -= numOfEndPixels;
    				x = pRect->x_pos + numOfStartPixels + numOfMiddlePixels;
    				for (y=0; y < pRect->height; ++y)
    				{
    					PATH_TESTED();
						logiCVC_DrawPixel(logiCvcBufferPtr, x, pRect->y_pos + y, color);
    				}
    			}
    		}
        }
        else
        {
        	PATH_TESTED();

            switch (pLayer->BytePerPix)
            {
            case 1:
            	PATH_TESTED();
                color &= 0xff;
                color |= color << 8;
                color |= color << 16;
                break;

            case 2:
            	PATH_TESTED();
                color &= 0xffff;
                color |= color << 16;
                break;

            case 4:
            	PATH_TESTED();
                break;

            default:
                OS_ASSERT(FALSE);
                break;
            }
        }

        if (numOfMiddlePixels > 0)
        {
#define FILLRECT_BUFFSIZE 128
    		OsSizeT bytesInStride = pLayer->BytePerPix * pLayer->Stride;
    		OsSizeT bytesInLine   = pLayer->BytePerPix * numOfMiddlePixels;
    		OsSizeT pixAddr       = pBuffer->offset + pLayer->BytePerPix *
    		    (pRect->x_pos + numOfStartPixels + pRect->y_pos * pLayer->Stride);
    		OsSizeT u32sToFill;
    		OsU32   aFillColor[FILLRECT_BUFFSIZE / 4];

    		PATH_TESTED();

			u32sToFill = (bytesInLine + 3) / 4;
			if (u32sToFill > (FILLRECT_BUFFSIZE / 4))
			{
				PATH_TESTED();
				u32sToFill = FILLRECT_BUFFSIZE / 4;
			}
			for (x=0; x < u32sToFill; ++x)
			{
				PATH_TESTED();
				aFillColor[x] = color;
			}

			for (y=0; y < pRect->height; ++y)
			{
				OsSizeT bytesCopied = 0;

				PATH_TESTED();

				while (bytesCopied < bytesInLine)
				{
					OsSizeT len = bytesInLine - bytesCopied;

					PATH_TESTED();

					if (len > FILLRECT_BUFFSIZE)
					{
						PATH_TESTED();
						len = FILLRECT_BUFFSIZE;
					}

					OsRegMultiWrite8(pLayer->CvcBackPtr->VideoAccessObjPtr, pixAddr + bytesCopied, (OsU8 *)aFillColor, len);
					bytesCopied += len;
				}

				pixAddr += bytesInStride;
			}
		}
    }
}

/* @} */
