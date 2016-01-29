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

/**
*
* @file logiCVC_instance.h
*
* @brief This header file contains structures  logiCVC instance and layer.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 2.06  GP   27/09/11 Initial release.
*
* </pre>
*
*****************************************************************************/

#ifndef XLOGICVC_INSTANCE_H /* prevent circular inclusions */
#define XLOGICVC_INSTANCE_H /* by using protection macros */

/***************************** Include Files ********************************/

#include "Oslib.h"

#define MAX_NUMBER_OF_BUFFERS       3
#define MAX_NUMBER_OF_CLUTS         2


/**
 * The logiCVC buffer descriptor.
 */
typedef struct logiCVC_Buffer
{
    struct logiCVC_Layer * pLayer;  /**< Backpointer to the parent logiCVC_LayerT info */
    OsU32                  offset;  /**< Buffer offset */
    OsSizeT                index;   /**< Buffer index  */

} logiCVC_BufferT;


/**
 * The logiCVC CLUT descriptor.
 */
typedef struct logiCVC_Clut
{
    struct logiCVC_Layer * pLayer;  /**< Backpointer to the parent logiCVC_LayerT info */
    OsU32                  offset;  /**< CLUT offset */
    OsSizeT                index;   /**< CLUT index  */

} logiCVC_ClutT;


/**
 * The logiCVC layer information data.
 */
typedef struct logiCVC_Layer
{
    struct logiCVC_Instance *CvcBackPtr;            /**< Backpointer to the parent logiCVC info */
    logiCVC_BufferT aBuffer[MAX_NUMBER_OF_BUFFERS]; /**< Buffer descriptors */
    logiCVC_ClutT aClut[MAX_NUMBER_OF_CLUTS];       /**< CLUT descriptors */
    OsU32 MemoryXoff;        /**< Memory offsets of layer, settable by user: Horizontal offset */
    OsU32 MemoryYoff;        /**< Memory offsets of layer, settable by user: Vertical offset */
    XRECT OutputPosition;    /**< Output information of layer, settable by user: (all in pixels) */
    OsU32 Stride;            /**< Stride of layer (in pixels); hardcoded in HW */
    OsU32 BitPerPix;         /**< Real bits per pixel, for color  (values:8,16,24); hardcoded in HW */
    OsU32 BytePerPix;        /**< Real bytes per pixel including alpha byte if supported (values:1,2,4); hardcoded in HW */
    OsU32 AlphaMode;         /**< Alpha mode of layer ( Alpha blending mode on a layer, 0-layer, 1- pixel, 2- clut; hardcoded in HW */
    OsU32 LayerAlpha;        /**< Layer alpha value */
    OsU32 TransparentColor;  /**< Transparent color of layer  */
    OsU32 LxCtrl;            /**< Contents of LX_CTRL register (as it may be unreadable)  */
    OsU32 LayerNumber;       /**< Sequential number of layer, set at init time*/
    logiCvcLayerTypeE Type;  /**< Color format: RGB, YCbCr, Alpha */
} logiCVC_LayerT;


/**
 * The logiCVC driver instance data. The user is required to allocate a
 * variable of this type for every logiCVC device in the system. A pointer
 * to a variable of this type is then passed to the driver API functions.
 */
typedef struct logiCVC_Instance
{
    OsBoolT IsReady;        /**< Device is initialized and ready */
    OsBoolT ReadableRegs;   /**< If TRUE device registers are readable, if FALSE only interrupt status 
                                 register and double/triple video/CLUT buffer registers are readable */
    OsU32 RegBaseAddr;      /**< Base address of registers */
    OsU32 RegHighAddr;      /**< Last register address */
    OsU32 MemoryLowAddr;    /**< Lowest memory address visible from logiCVC device */
    OsU32 MemoryHighAddr;   /**< Highest memory address visible from logiCVC device  */
    OsU32 BackgroundColor;  /**< The color of the background layer, color is 3byte RGB  */
    OsU32 HorizontalRes;    /**< Horizontal resolution of logiCVC, set depending on display type  */
    OsU32 VerticalRes;      /**< Vertical resolution of logiCVC, set depending on display type  */
    OsU32 NumLayers;        /**< Number of memory layes (not counting background) supported on this CVC instance, currently CVC can support up to max 5 layers  */
    OsBoolT UseBackground;  /**< Is last layer configured as background layer  */
    OsU32 IntMask;          /**< Contents of INT_MASK register (as it may be unreadable)  */
    logiCVC_LayerT LayerInfo[LOGICVC_MAX_LAYERS_PER_CVC];   /**< Layer information structures **/
    logiCvcConfigT CvcConfig;               /**< logiCVC configuration structure */
    OsHwAccessObjectT * RegAccessObjPtr;    /**< Register range descriptor */
    OsHwAccessObjectT * VideoAccessObjPtr;  /**< Video memory descriptor */
    OsBoolT CoreInitBeforeDriverStart;      /**< When true this flag indicates that CVC driver is started but CVC IP core
                                                 registers were already initialised before, e.g. from bootloader,
                                                 in this case when ReadableRegs==FALSE we can't support some of the functions */
    int memSortedLayerIndices[LOGICVC_MAX_LAYERS_PER_CVC];  /**< Array containing layer indices (0-NUM_LAYER-1) sorted by layer A buffers address */
    OsU32 accBufferSelect;  /**< Accumulated buffer select requests */
    OsU32 wrBufferSelect;   /**< Written buffer select requests */
    OsU32 accClutSelect;    /**< Accumulated CLUT select requests */
    OsU32 wrClutSelect;     /**< Written CLUT select requests */

    OsU32 ipMajorRevision;  /**< logiCVC IP core major revision, range (0:31) */
    OsU32 ipMinorRevision;  /**< logiCVC IP core minor revision, range (0:31) */
    OsU32 ipPatchRevision;  /**< logiCVC IP core patch revision, range (0:25), VALUES = (0=a,1=b,2=c,3=d,4=e,5=f,6=g,7=h,8=i,9=j,10=k,11=l,12=m,13=n,14=o,15=p,16=q,17=r,18=s,19=t,20=u,21=v,22=w,23=x,24=y,25=z) */
    OsU32 ipLicenseType;    /**< IP license type: 0 – source code, 1 – evaluation, 2 – release */

} logiCVC_InstanceT;


#endif  /* end of protection macro */

/* @} */
