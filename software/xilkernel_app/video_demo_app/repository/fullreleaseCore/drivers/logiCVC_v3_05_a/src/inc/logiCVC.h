/*****************************************************************************
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2012 - Xylon d.o.o.
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
* @file logiCVC.h
* \mainpage
* Driver for the Xylon logiCVC-ML device.
*
* - \subpage p1
* - \subpage p2
* - \subpage init "Driver initialization"
* - \subpage notes "Driver notes"
* - \subpage p3 "logiCVC example details"
* - Driver API: \ref logiCVCmid
* - Driver XPS higher layer API: \ref logiCVC_XPS_osdrv
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  DD/MM/YY Changes
* ------ ---- -------- -------------------------------------------------------
* 1.00.a GP   12/04/06 First release.
* 1.02.a JI   07/09/06 Changes related to widening CVC registers and adding
*                      interrupt control for switching buffers
*                      Added layer control register and enable layer function
* 1.02.b JI   28/06/07 LOGICVC_PIXELS_IN_HORIZONTAL_LINE increased from 1024
*                      to 2048 pixels.
* 1.03.a JI   07/01/08 Added triple buffer support
*                      Changed DOUBLE_VBUFF and DOUBLE_CLUT register names to
*                      VBUFF_SEL and CLUT_SEL.
* 1.04.a KM   27/05/10 Added logiCvcDisplay structure
* 2.02.b AF   27/05/10 Added osdrv level files for XPS (logiCVC_init), added version log
* 2.03.a AF   29/03/11 added version log
* 2.04.a GP   14/04/11 Changed XPS osdrv interface, added logiCVC_plfInitCallbackT to logiCVC_init function
*                      changed behavior of functions logiCVC_LayerEnDis, logiCVC_SetTransparentColor to check if regs
*                      are readable or cVC was started before driver. Added functions logiCVC_LayerExternalBufferSwitchingCtrl and
*                      logiCVC_SetLayerControlFlags
* 2.05.a GP   01/06/11 Functional change: PAL and NTSC clock set to 27000(ITU), removed old disp init data,
*                      use new functions logiCVC_Start and logiCVC_GetDispInitData
*                      Cosmetics: Doxy update, split logiCVC.c in several files
* 3.00.a GP   24/10/11 Major interface change:
*                      Hidden logiCVC_instance struct from interface, added logiCVC_GetCommonParam
*                      and logiCVC_GetLayerParam functions, pass core generics configuration in logiCVC_Initialize.
*                      Changed the logiCVC_plfInitCallbackT to allow better display control.
*                      Added internal copy of driver configuration structure.
*                      Cosmetics: Changed function arguments and variables types to OsLib types.
* 3.01.a HJ   16/12/11 A new interface that uses logiCvcLayerPointerT, logiCvcBufferPointerT and logiCvcClutPointerT throughout.
*                      logiCvcDisplayT now specifies HorizontalRes and VerticalRes incremented by 1.
*                      fixed logiCVC_SetBackgroundColor() - corrected layer pointer
* 3.02.a HJ   21/12/11 logiCVC_AbortChange() added and called when disabling a layer.
* 3.03.a HJ   21/02/12 Endian flippable.
*                      Revisions added.
*                      logiCVC_Initialize, logiCvcConfigPtr is a const.
* 3.04.a GP   21/05/12 LOGICVC_DT_X are now logiCvcDisplayTypeE.logiCVC_GetDisplayParamName() added.
*                      Fixed internal video modes timig parameters in logiCVC_disp_i.c.
*                      CVC_VRAM_SIZE_E calculation fixed. Osdrv updated to new oslib, sw_apps examples.
* 3.05.a HJ   03/10/12 Support for logiCVC core v3_00_a (YCbCr, alpha layer, layer type AlphaLX_CTRL_PIXEL_FORMAT runtime color components
*                      swap), color utilty functions, renamed int functions
* </pre>
*
*****************************************************************************/


#ifndef XLOGICVC_H /* prevent circular inclusions */
#define XLOGICVC_H /* by using protection macros */

/***************************** Include Files ********************************/

#include "Oslib.h"


/**
* logiCVC driver version number
*
* bits          represent           range           values
* [31 - 24]     custom revision     (0 : 31)        0 = standard Xylon driver
* [23 - 16]     major revision      (1 : 31)
* [15 - 8]      minor revision      (0 : 31)
* [ 7 - 0]      patch level         (0 : 25)        0=a,  1=b,  2=c,  3=d,  4=e,
                                                    5=f,  6=g,  7=h,  8=i,  9=j,
                                                    10=k, 11=l, 12=m, 13=n, 14=o,
                                                    15=p, 16=q, 17=r, 18=s, 19=t,
                                                    20=u, 21=v, 22=w, 23=x, 24=y,
                                                    25=z
*/
#define CVC_DRIVER_VER 0x00030500   /**< v3_04_a standard Xylon driver*/


/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/**
 * This typedef contains configuration information for the device.
 */

/**  Driver Configuration  Begin @{ ****/
#define LOGICVC_MAX_LAYERS_PER_CVC  5
#define LOGICVC_NUM_BUFFERS         3
/**  Driver Configuration  End @} ****/

#define LOGICVC_ALPHA_MODE_LAYER                    0
#define LOGICVC_ALPHA_MODE_PIXEL                    1
#define LOGICVC_ALPHA_MODE_CLUT_INTERNAL_ARGB_6565  2
#define LOGICVC_ALPHA_MODE_CLUT_INTERNAL_ARGB_8888  3


/**
 * Layer Types
 */
typedef enum
{
    CVC_LAYER_TYPE__RGB,    /**< RGB   layer  (e.g. ARGB8888, RGB565 ...) */
    CVC_LAYER_TYPE__YCBCR,  /**< YCBCR layer  ( YCbCr 16bpp (4:2:2) and YCbCr 24bpp (4:4:4)) */
    CVC_LAYER_TYPE__ALPHA,  /**< Alpha layer type: special 8bit alpha layer that defines
                                 pixel blending between two surrounding layers, e.g. if configured
                                 on layer 1 it defines blending between layers 0 and 2, if configured
                                 on layer 3 it defines blending between layers 2 and 4 */

    CVC_LAYER_TYPE__OUT_OF_RANGE

} logiCvcLayerTypeE;


/** Defines per-layer interupts */
typedef enum
{
 LOGICVC_BOTH_INT = 0,
 LOGICVC_VBUFF_INT,
 LOGICVC_CLUT_INT
}logiCVC_layerInterruptModeE;

/* logiCVC interrupt status and interrupt enable register  bitmasks */
#define LOGICVC_INT_L0_VBUFF_SW_MSK        (1<<0)  /**< layer 0 vbuff switch interrupt mask  */
#define LOGICVC_INT_L1_VBUFF_SW_MSK        (1<<1)  /**< layer 1 vbuff switch interrupt mask  */
#define LOGICVC_INT_L2_VBUFF_SW_MSK        (1<<2)  /**< layer 2 vbuff switch interrupt mask  */
#define LOGICVC_INT_L3_VBUFF_SW_MSK        (1<<3)  /**< layer 3 vbuff switch interrupt mask  */
#define LOGICVC_INT_L4_VBUFF_SW_MSK        (1<<4)  /**< layer 4 vbuff switch interrupt mask  */
#define LOGICVC_INT_V_SYNC_MSK             (1<<5)  /**< v-sync interrupt mask  */
#define LOGICVC_INT_E_VIDEO_VALID_MSK      (1<<6)  /**< External RGB input valid  */
#define LOGICVC_INT_L0_CLUT_SW_MSK         (1<<8)  /**< layer 0 clut switch interrupt mask */
#define LOGICVC_INT_L1_CLUT_SW_MSK         (1<<9)  /**< layer 0 clut switch interrupt mask */
#define LOGICVC_INT_L2_CLUT_SW_MSK         (1<<10) /**< layer 0 clut switch interrupt mask */
#define LOGICVC_INT_L3_CLUT_SW_MSK         (1<<11) /**< layer 0 clut switch interrupt mask */
#define LOGICVC_INT_L4_CLUT_SW_MSK         (1<<12) /**< layer 0 clut switch interrupt mask */



#define DIPSWITCH_CLOCK_INVERT  (1<<0)
#define DIPSWITCH_LCD           (1<<1)
#define DIPSWITCH_NEW_BOARD     (1<<2)
#define DIPSWITCH_LCD2          (1<<3)

#define PARAMETER_INVALID   0xFFFFFFFF
#define PARAMETER_NOT_USED (-1)

/**
 * This structure contains the logiCVC information gained from hardware configuration.
 * All the elements are OsU32 to simplify usage.
 */
typedef struct logiCvcConfig
{
    OsU32 ipMajorRevision;  /**< logiCVC IP core major revision, range (0:31) */
    OsU32 ipMinorRevision;  /**< logiCVC IP core minor revision, range (0:31) */
    OsU32 ipPatchRevision;  /**< logiCVC IP core patch revision, range (0:25), VALUES = (0=a,1=b,2=c,3=d,4=e,5=f,6=g,7=h,8=i,9=j,10=k,11=l,12=m,13=n,14=o,15=p,16=q,17=r,18=s,19=t,20=u,21=v,22=w,23=x,24=y,25=z) */
    OsU32 RegBaseAddr;      /**< logiCVC Register base address */
    OsU32 RegHighAddr;      /**< logiCVC Last register address */
    OsU32 MemoryLowAddr;    /**< Lowest memory address visible from logiCVC device */
    OsU32 MemoryHighAddr;   /**< Highest memory address visible from logiCVC device */
    OsU32 RowStride;        /**< Row stride in number of pixels */
    OsU32 NumLayers;        /**< Number of layes supported on this CVC instance, currently CVC can support up to max 5 layers */
    OsU32 UseBackground;    /**< Is last layer configured as background layer: 0- no, 1 -yes */
    OsU32 ReadableRegs;     /**< If !=0 device registers are readable, if 0 only interrupt status
                                 register and double/triple video/CLUT buffer registers are readable */
    OsU32 Layer_DataWidth[LOGICVC_MAX_LAYERS_PER_CVC];      /**< Number of bits per pixel on a layer */
    OsU32 Layer_Type[LOGICVC_MAX_LAYERS_PER_CVC];           /**< Layer type, i.e. color format: RGB, YCbCr */
    OsU32 Layer_AlphaMode[LOGICVC_MAX_LAYERS_PER_CVC];      /**< Alpha blending mode on a layer, 0-layer, 1- pixel, 2- clut */
    OsU32 Layer_BufferA_Offset[LOGICVC_MAX_LAYERS_PER_CVC]; /**< Buffer A offset in expressed in horizontal lines starting from MemoryLowAddr */
    OsU32 Layer_Buffer_Offset[LOGICVC_MAX_LAYERS_PER_CVC];  /**< Buffer offset expressed in horizontal lines starting from Buffer A offset, calculation:
                                                                 BufferB = BufferA + Buffer_offset, BufferC = BufferA + 2*Buffer_offset */
} logiCvcConfigT;


#ifndef XRECT_TYPE
#define XRECT_TYPE
typedef struct
{
    OsU16 x_pos;
    OsU16 y_pos;
    OsU16 width;
    OsU16 height;
} XRECT, *HXRECT;
#endif


/**
 * Pointer to logiCVC driver instance, used in driver interface functions (hidden struct definition)
 */
typedef struct logiCVC_Instance * logiCvcPointerT;

/**
 * Pointer to logiCVC layer structure, used in driver interface functions (hidden struct definition)
 */
typedef struct logiCVC_Layer * logiCvcLayerPointerT;

/**
 * Pointer to logiCVC buffer structure, used in driver interface functions (hidden struct definition)
 */
typedef struct logiCVC_Buffer * logiCvcBufferPointerT;

/**
 * Pointer to logiCVC CLUT structure, used in driver interface functions (hidden struct definition)
 */
typedef struct logiCVC_Clut * logiCvcClutPointerT;

/**
 * Structure that contains detailed data about the particular display or standard VGA resolution type.
 */
typedef struct logiCvcDisplay
{
    OsU32 HorizontalFrontPorch; /**< Display horizontal front porch */
    OsU32 HorizontalSync;       /**< Display horizontal sync */
    OsU32 HorizontalBackPorch;  /**< Display horizontal back porch */
    OsU32 HorizontalRes;        /**< Display horizontal resolution */
    OsU32 VerticalFrontPorch;   /**< Display vertical front porch */
    OsU32 VerticalSync;         /**< Display vertical sync */
    OsU32 VerticalBackPorch;    /**< Display vertical back porch */
    OsU32 VerticalRes;          /**< Display vertical resolution */
    OsU32 ControlRegister;      /**< Display control register */
    OsU32 TypeRegister;         /**< Display type register */
    OsU32 PixelClock;           /**< Display pixel clock */
    OsU32 PowerOnDelay;         /**< Delay after applying display power and before applying display signals */
    OsU32 SignalOnDelay;        /**< Delay after applying display signal and before applying display backlight power supply */
} logiCvcDisplayT;

/**
 * Function return values.
 */
typedef enum
{
    cvcRetSuccessE = 0,
    cvcRetFailureE = -1
} cvcRetT;

/**
 * Layer control bitmasks.
 */
typedef enum
{
    LX_CTRL_EN_LAYER_MSK        = (1<<0),  /**< When set to 1 enables a layer. By disabling it, layer is not fetching
                                             data from memory and acts as a transparent layer. */
    LX_CTRL_DIS_TRANSP_MSK      = (1<<1),  /**< When set to 1 disables colour key transparency per layer. By setting this bit,
                                             logiCVC-ML will not perform a comparison between pixel colour and
                                             the value stored in the Transparent Colour register and pixel will
                                             be seen (if alpha value is set appropriately). */
    LX_CTRL_EN_EXT_VBUFF_SW_MSK = (1<<2),  /**< When set to 1 enables external switching of video buffers. This functionality
                                             is used when there is an external video source writing to logiCVC-ML
                                             layer video buffer.*/
    LX_CTRL_EN_INTERLACED_MSK   = (1<<3),  /**< This bit is used to control the memory-reading mode when logiCVC-ML is configured
                                             to output ITU656 standard. If set to ‘1’ logiCVC-ML toggles between
                                             reading even and odd lines from the memory depending on the current frame
                                             it is outputting. When set to ‘0’, i.e. progressive mode, every frame
                                             consists of continuous lines from memory.*/
    LX_CTRL_PIXEL_FORMAT_MSK    = (7<<4),  /**< Pixel format mask. */
    LX_CTRL_PIXEL_FORMAT_0      = (0<<4),  /**< Pixel format 0: ARGB, AYCbCr, CbY0CrY1. */
    LX_CTRL_PIXEL_FORMAT_1      = (1<<4)   /**< Pixel format 1: ABGR, ACrCbY, Y0CbY1Cr. */

} cvcLayerControlBitMaskE;

/**
 * Input parameter used in /ref logiCVC_GetCommonParam
 */
typedef enum
{
    CVC_HRES_E = 0,             /**< logiCVC output horizontal resolution */
    CVC_VRES_E,                 /**< logiCVC output vertical resolution */
    CVC_NUM_LAYERS_E,           /**< Number of logiCVC layers,  NOTE: This doesnt count also
                                     the background layer, only the layers reading from video memory. */
    CVC_USE_BACKGROUND_E,       /**< The background layer is used. */
    CVC_STRIDE_E,               /**< Stride in pixels, common for all layers */
    CVC_VRAM_START_PHY_ADDR_E,  /**< Start of video ram, physical address */
    CVC_VRAM_START_VIRT_ADDR_E, /**< Start of video ram, virtual address */
    CVC_VRAM_SIZE_E             /**< Size of video ram */
} cvcCommonParamE;

/**
 * Input parameter used in /ref logiCVC_GetLayerParam
 */
typedef enum
{
    CVC_LAYER_BYTE_PER_PIX_E = 0,           /**< Bytes per pixel in layer, values: 1,2,4*/
    CVC_LAYER_COLOR_BITS_E,                 /**< Number of bits per color, values: 8,16,24 */
    CVC_LAYER_TYPE_E,                       /**< Layer type, values: 0-RGB, 1-YCbCr, 2-Alpha */
    CVC_LAYER_ALPHA_MODE_E,                 /**< Alpha mode of layer (Alpha blending mode on a layer, 0-layer, 1- pixel, 2- clut)*/
    CVC_LAYER_BUFF_A_BYTE_OFFSET_E,         /**< Buffer A byte memory offset from the start of logiCVC video memory */
    CVC_LAYER_BUFF_B_BYTE_OFFSET_E,         /**< Buffer B byte memory offset from the start of logiCVC video memory */
    CVC_LAYER_BUFF_C_BYTE_OFFSET_E,         /**< Buffer C byte memory offset from the start of logiCVC video memory */
    CVC_LAYER_BUFF_BYTE_OFFSET_E,           /**< Byte memory offset between (buffB and buffA) (buffC and buffB) */
    CVC_LAYER_SIZE_BYTES_TO_NEXT_LAYER_E,   /**< Number of bytes between this buffA of this layer and buffA of next
                                                 layer (as they are ordered in memory), or end of video mem. */
    CVC_LAYER_MAX_VRES_NO_BUFFERING_E,      /**< Maximal vertical resolution that this layer can show without
                                                 overlapping with the next layer in memory. When buffering is not used then we can
                                                 use all available mem until next layer in memory or end of memory.
                                                 determined by buffer offset. */
    CVC_LAYER_MAX_VRES_USE_BUFFERING_E,     /**< Maximal vertical resolution that this layer can show without
                                                 overlapping with the next layer in memory. When using buffering vres is
                                                 determined by buffer offset. */
    CVC_LAYER_NUM_BUFFERS_USABLE_E,         /**< Maximal number of buffers that you can use with respect to the next layer in memory. This can be:
                                                 0 - there is next layer at the same offset, 1 - no buffering, 2 - double buffering, 3 - tripple buffering */
    CVC_LAYER_GET_PREV_LAYER_IN_MEM_IDX_E,  /**< Returns index of previous layer in memory, layer whose bufferA is located before the bufferA of that layer */
    CVC_LAYER_GET_NEXT_LAYER_IN_MEM_IDX_E,  /**< Returns index on next layer in memory, layer whose bufferA is located  after the bufferA of that layer */
    CVC_LAYER_GET_OUTPUT_POSITION_X_E,      /**< Layer output rectangle x position */
    CVC_LAYER_GET_OUTPUT_POSITION_Y_E,      /**< Layer output rectangle y position */
    CVC_LAYER_GET_OUTPUT_POSITION_WIDTH_E,  /**< Layer output rectangle width */
    CVC_LAYER_GET_OUTPUT_POSITION_HEIGHT_E, /**< Layer output rectangle height */
    CVC_LAYER_GET_TRANSPARENT_COLOR_E,      /**< Transparent color */
    CVC_LAYER_IS_ENABLED_E,                 /**< Returns 1 if layer is enabled, 0 if disabled */
    CVC_LAYER_GET_X_MEM_OFFSET_E,           /**< Returns horizontal memory layer offset */
    CVC_LAYER_GET_Y_MEM_OFFSET_E            /**< Returns vertical memory layer offset */
} cvcLayerParamE;


/**
 * Aditional param in function /ref logiCVC_Start
 */
typedef enum
{
    CVC_START_NORMAL_E          =  0,       /**< No special flag set */
    CVC_START_SKIP_REGS_WRITE_E = (1<<1)    /**< Skip writting into logiCVC registers,
                                                 this is used when registers were initialised previously */
} cvcStartFlagsE;


/**
 * Power signal flags used to perform proper LCD display power sequencing.\n
 * NOTE that for VGA/DVI monitors you must enable only CVC_PWR_SIGNALS.\n
 * In general the power sequencing is as follows:\n
 * 1. CVC_PWR_VDD - enable display power supply. (prior to this clock and display power might need to be enabled on the hardware board)\n
 * 2. Wait for VDD to become stable (display specific, this value is stored in PowerOnDelay element of \ref logiCvcDisplayT).\n
 * 3. CVC_PWR_SIGNALS - enable display control and data signals.\n
 * 4. Wait for display to initalize (display specific, this value is stored in SignalOnDelay element of \ref logiCvcDisplayT).\n
 * 5. CVC_PWR_BACKLIGHT - enable backlight.\n
 */
typedef enum
{
    CVC_PWR_OFF = 0,
    CVC_PWR_VDD,               /**< Vdd display power enable */
    CVC_PWR_VEE,               /**< Vee power enable (for passive displays - this function is depricated in logiCVC but the
                                    signals are still connected to logiCVC output) */
    CVC_PWR_SIGNALS,           /**< Enable display control and data signals */
    CVC_PWR_BACKLIGHT          /**< Enable display backlight (backlight is turned on last to avoid seeing displey powerup) */
} cvcPowerSignalFlagsE;


/**
 * Supported display types
 */
typedef enum
{
    LOGICVC_DT_SIMULATION,
    LOGICVC_DT_512_512,
    LOGICVC_DT_1024_1024,
    LOGICVC_DT_2048_2048,
    LOGICVC_DT_512_384,
    LOGICVC_DT_2048_1536,
    LOGICVC_DT_2048_512,
    LOGICVC_DT_VGA_640_480,
    LOGICVC_DT_VGA_800_480,
    LOGICVC_DT_VGA_800_600,
    LOGICVC_DT_VGA_1024_768,
    LOGICVC_DT_VGA_RBT_1024_768,
    LOGICVC_DT_VGA_1280_1024,
    LOGICVC_DT_VGA_1680_1050,
    LOGICVC_DT_VGA_1920_1080,
    LOGICVC_DT_VGA_1920_1200,
    LOGICVC_DT_PAL,
    LOGICVC_DT_NTSC,
    LOGICVC_DT_1280_480,
    LOGICVC_DT_TD070WGCB2,
    LOGICVC_DT_LQ065T9DR51U,
    LOGICVC_DT_LTL652T,
    LOGICVC_DT_LTA035B,
    LOGICVC_DT_TD080WGCA1,
    LOGICVC_DT_LQ070Y5DE01,
    LOGICVC_DT_LLL652T,
    LOGICVC_DT_UMSH_8013MD,
    LOGICVC_DT_MTF_TW70SP,
    LOGICVC_DT_LTA149B780F,
    LOGICVC_DT_XTRONIC_800_480,
    LOGICVC_DT_XTRONIC_400_240,
    LOGICVC_DT_DLF1095,
    LOGICVC_DT_LAJ123T001_JLR,
    LOGICVC_DT_TCG062HVLDA_G20,
    LOGICVC_DT_T_55382GD050JU_LW_A_ABN,
    LOGICVC_DT_G065VN01V2,
    LOGICVC_DT_TM050RBH01,
    LOGICVC_DT_1920_1080

} logiCvcDisplayTypeE;



/************************** Function Prototypes *****************************/

/*
 * Required functions, in file logiCVC.c
 */

OsCharT const * logiCVC_GetConfigParamName(OsSizeT index);
OsCharT const * logiCVC_GetDisplayParamName(OsSizeT index);

logiCvcPointerT logiCVC_Initialize(logiCvcConfigT const * const pConfig,
                                   OsHwResourceT  const * const pOsHwRegResource,
                                   OsHwResourceT  const * const pOsHwVideoMemResource);
void            logiCVC_Deinitialize(logiCvcPointerT logiCvcPtr);

cvcRetT logiCVC_Start(logiCvcPointerT logiCvcPtr, logiCvcDisplayT const *pDisplayData, cvcStartFlagsE flags);
void    logiCVC_PowerControl(logiCvcPointerT logiCvcPtr, cvcPowerSignalFlagsE pwrSignal, OsBoolT bEnable);

OsU32   logiCVC_GetCommonParam(logiCvcPointerT logiCvcPtr, cvcCommonParamE param);

void    logiCVC_Sync(logiCvcPointerT logiCvcPtr, OsBoolT bWait);
OsBoolT logiCVC_IsSynced(logiCvcPointerT logiCvcPtr);

cvcRetT logiCVC_SetBackgroundColor(logiCvcPointerT logiCvcPtr, OsU32 color);

OsBoolT logiCVC_GetVsyncState(logiCvcPointerT logiCvcPtr);
void    logiCVC_ClearVsyncState(logiCvcPointerT logiCvcPtr);
void    logiCVC_WaitVsync(logiCvcPointerT logiCvcPtr);

OsU32   logiCVC_GetIntStatus(logiCvcPointerT logiCvcPtr );
void    logiCVC_ClearIntStatus(logiCvcPointerT logiCvcPtr, OsU32 mask);
void    logiCVC_WriteIntMask(logiCvcPointerT logiCvcPtr, OsU32 mask);

logiCvcLayerPointerT logiCVC_GetLayerPointer(logiCvcPointerT logiCvcPtr, OsU16 numLayer);

OsU32   logiCVC_GetLayerParam(logiCvcLayerPointerT logiCvcLayerPtr, cvcLayerParamE param);
void    logiCVC_LayerEnDis(logiCvcLayerPointerT logiCvcLayerPtr, OsBoolT bEnable);

cvcRetT logiCVC_SetTransparentColor(logiCvcLayerPointerT logiCvcLayerPtr, OsU32 color);
cvcRetT logiCVC_DisableTransparentColor(logiCvcLayerPointerT logiCvcLayerPtr);

cvcRetT logiCVC_SetOutputPosition(logiCvcLayerPointerT logiCvcLayerPtr, XRECT *outputPosition);
void    logiCVC_SetMemoryOffset(logiCvcLayerPointerT logiCvcLayerPtr, OsU16 x_pos, OsU16 y_pos);

cvcRetT logiCVC_SetLayerControlFlags(logiCvcLayerPointerT logiCvcLayerPtr, cvcLayerControlBitMaskE mask);
void    logiCVC_EnLayerInt(logiCvcLayerPointerT logiCvcLayerPtr, logiCVC_layerInterruptModeE mode);
void    logiCVC_DisLayerInt(logiCvcLayerPointerT logiCvcLayerPtr, logiCVC_layerInterruptModeE mode);

cvcRetT logiCVC_GetMaxLayerAlphaValue(logiCvcLayerPointerT logiCvcLayerPtr, OsU32 * pMaxLayerAlphaValue);
cvcRetT logiCVC_SetLayerAlphaValue(logiCvcLayerPointerT logiCvcLayerPtr, OsU32 layerAlphaValue);

cvcRetT logiCVC_SetExternalBufferSwitchingCtrl(logiCvcLayerPointerT logiCvcLayerPtr, OsBoolT bEnable);
OsSizeT logiCVC_GetCurrentBufferNumber(logiCvcLayerPointerT logiCvcLayerPtr);
cvcRetT logiCVC_ChangeBuffer(logiCvcLayerPointerT logiCvcLayerPtr, OsSizeT bufNum);

OsSizeT logiCVC_GetCurrentClutNumber(logiCvcLayerPointerT logiCvcLayerPtr);
cvcRetT logiCVC_ChangeClut(logiCvcLayerPointerT logiCvcLayerPtr, OsSizeT clutNum);

void logiCVC_AbortChange(logiCvcLayerPointerT logiCvcLayerPtr, OsBoolT bWait);

cvcRetT logiCVC_SetLayerPixelFormat(logiCvcLayerPointerT logiCvcLayerPtr, cvcLayerControlBitMaskE pixelFormat);
OsU32   logiCVC_ConvertArgb32ToLayerColorFormat(logiCvcLayerPointerT logiCvcLayerPtr, OsU32 argb);


OsU32   logiCVC_ConvertAycbcr32ToArgb32(OsU32 aycbcr);
OsU32   logiCVC_ConvertArgb32ToAycbcr32(OsU32 argb);


logiCvcBufferPointerT logiCVC_GetBufferPointer(logiCvcLayerPointerT logiCvcLayerPtr, OsSizeT bufNum);

OsU32   logiCVC_GetOffset(logiCvcBufferPointerT logiCvcBufferPtr);
void    logiCVC_DrawPixel(logiCvcBufferPointerT logiCvcBufferPtr, OsU16 x_off, OsU16 y_off, OsU32 color);
OsU32   logiCVC_GetPixel(logiCvcBufferPointerT logiCvcBufferPtr, OsU16 x_off, OsU16 y_off);
void    logiCVC_FillRectangle(logiCvcBufferPointerT logiCvcBufferPtr, XRECT *rectangle, OsU32 color);
void    logiCVC_CopyRectangle(logiCvcBufferPointerT logiCvcBufferPtr, XRECT *destRect, OsU8 *srcPtr, OsU16 srcDataStride);


logiCvcClutPointerT logiCVC_GetClutPointer(logiCvcLayerPointerT logiCvcLayerPtr, OsSizeT clutNum);

void    logiCVC_ClutWrite(logiCvcClutPointerT logiCvcClutPtr, OsU8 index, OsU32 value);
OsU32   logiCVC_ClutRead(logiCvcClutPointerT logiCvcClutPtr, OsU8 index);


logiCvcDisplayT const *logiCVC_GetDispInitData(logiCvcDisplayTypeE displayType);

#endif  /* end of protection macro */

/* @} */
