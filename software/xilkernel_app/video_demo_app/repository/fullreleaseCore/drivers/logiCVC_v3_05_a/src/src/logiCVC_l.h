/*****************************************************************************
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2006 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
*****************************************************************************/

/****************************************************************************/
/**
*
* @file logiCVC_l.h
*
* This header file contains identifiers and low-level driver functions (or
* macros) that can be used to access the device.  High-level driver functions
* are defined in logiCVC.h.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes
* ------ ---- -------- -------------------------------------------------------
* 1.00.a GP   12/04/06 First release
* 1.02.a JI   07/09/06 Changes related to widening CVC registers and adding
*                      interrupt control for switching buffers.
*                      Added layer control register and enable layer function.
*                      Changed transparent color register to be per layer.
* 1.03.a JI   07/01/08 Added triple buffer support
*                      Changed DOUBLE_VBUFF and DOUBLE_CLUT register names to
*                      VBUFF_SEL and CLUT_SEL.
* 2.06.a DJ   27/09/11 Renamed and reorganised according to new logiCVC driver structure.
*
* </pre>
*
*****************************************************************************/

#ifndef LOGICVC_L_H /* prevent circular inclusions */
#define LOGICVC_L_H /* by using protection macros */

/***************************** Include Files ********************************/

#include "Oslib.h"


/************************** Constant Definitions ****************************/



/** \addtogroup logiCVCreg logiCVC low-level driver - Registers macros
 *  LogiCVC registers description  @{
 */
/** All logiCVC registers are 32 bit registers, at distance of 64 bit */
#define CVC_REG_DIST_USED  8   /**<  All logicvc registers are spaced at 8 bytes       */
#define CVC_SHSY_FP_ROFF       ( 0   * CVC_REG_DIST_USED )  /**<  R_HSY_FP             */
#define CVC_SHSY_ROFF          ( 1   * CVC_REG_DIST_USED )  /**<  R_HSY                */
#define CVC_SHSY_BP_ROFF       ( 2   * CVC_REG_DIST_USED )  /**<  R_HSY_BP             */
#define CVC_SHSY_RES_ROFF      ( 3   * CVC_REG_DIST_USED )  /**<  R_HSY_RES            */
#define CVC_SVSY_FP_ROFF       ( 4   * CVC_REG_DIST_USED )  /**<  R_VSY_FP             */
#define CVC_SVSY_ROFF          ( 5   * CVC_REG_DIST_USED )  /**<  R_VSY                */
#define CVC_SVSY_BP_ROFF       ( 6   * CVC_REG_DIST_USED )  /**<  R_VSY_BP             */
#define CVC_SVSY_RES_ROFF      ( 7   * CVC_REG_DIST_USED )  /**<  R_VSY_RES            */
#define CVC_SCTRL_ROFF         ( 8   * CVC_REG_DIST_USED )  /**<  R_CTRL               */
#define CVC_SDTYPE_ROFF        ( 9   * CVC_REG_DIST_USED )  /**<  R_DTYPE              */
#define CVC_BACKCOL_ROFF       ( 10  * CVC_REG_DIST_USED )  /**<  R_BACKGROUND         */
#define CVC_DOUBLE_VBUFF_ROFF  ( 11  * CVC_REG_DIST_USED )  /**<  R_DOUBLE_VBUFF       */
#define CVC_DOUBLE_CLUT_ROFF   ( 12  * CVC_REG_DIST_USED )  /**<  R_DOUBLE_CLUT        */
#define CVC_INT_ROFF           ( 13  * CVC_REG_DIST_USED )  /**<  R_INT                */
#define CVC_INT_MASK_ROFF      ( 14  * CVC_REG_DIST_USED )  /**<  R_INT_MASK           */
#define CVC_SPWRCTRL_ROFF      ( 15  * CVC_REG_DIST_USED )  /**<  R_PWRCTRL            */
#define CVC_IPVERSION_ROFF     ( 31  * CVC_REG_DIST_USED )  /**<  R_IPVERSION          */
/** @} */

/** \addtogroup group_layer layer Registers
 * \ingroup logiCVCreg
 *  @{
 */

/* CVC layer registers base and distance between the layers */
#define CVC_LAYER0_BASE_ROFF   ( 32  * CVC_REG_DIST_USED )                        /**<  offset to the beginning of layer 0 registers */
#define CVC_LAYER_DISTANCE     ( 16  * CVC_REG_DIST_USED )                        /**<  distance between groups of layer registers */
#define CVC_LAYER1_BASE_ROFF   ( CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE * 1 )  /**<  offset to the beginning of layer 1 registers */
#define CVC_LAYER2_BASE_ROFF   ( CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE * 2 )  /**<  offset to the beginning of layer 2 registers */
#define CVC_LAYER3_BASE_ROFF   ( CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE * 3 )  /**<  offset to the beginning of layer 3 registers */
#define CVC_LAYER4_BASE_ROFF   ( CVC_LAYER0_BASE_ROFF + CVC_LAYER_DISTANCE * 4 )  /**<  offset to the beginning of layer 4 registers */

/* CVC layer registers offsets (common for each layer) */
#define CVC_LAYER_HOR_OFF_ROFF    (0 * CVC_REG_DIST_USED )   /**<  LH_OFFSET      */
#define CVC_LAYER_VER_OFF_ROFF    (1 * CVC_REG_DIST_USED )   /**<  LV_OFFSET      */
#define CVC_LAYER_HOR_POS_ROFF    (2 * CVC_REG_DIST_USED )   /**<  LH_POSITION    */
#define CVC_LAYER_VER_POS_ROFF    (3 * CVC_REG_DIST_USED )   /**<  LV_POSITION    */
#define CVC_LAYER_WIDTH_ROFF      (4 * CVC_REG_DIST_USED )   /**<  LH_WIDTH       */
#define CVC_LAYER_HEIGHT_ROFF     (5 * CVC_REG_DIST_USED )   /**<  LV_HEIGHT      */
#define CVC_LAYER_ALPHA_ROFF      (6 * CVC_REG_DIST_USED )   /**<  ALPHA          */
#define CVC_LAYER_CTRL_ROFF       (7 * CVC_REG_DIST_USED )   /**<  CTRL           */
#define CVC_LAYER_TRANSP_ROFF     (8 * CVC_REG_DIST_USED )   /**<  TRANSPARENT    */

/** @} */

/** CLUT definitions @{ */
#define LOGICVC_CLUT_SIZE 256
#define LOGICVC_CLUT_ELEM_BYTE_DIST 8

#define LOGICVC_CLUT_0_A_OFFSET   ( 0x200 * LOGICVC_CLUT_ELEM_BYTE_DIST )
#define LOGICVC_CLUT_0_B_OFFSET   ( 0x300 * LOGICVC_CLUT_ELEM_BYTE_DIST )
#define LOGICVC_CLUT_1_A_OFFSET   ( 0x400 * LOGICVC_CLUT_ELEM_BYTE_DIST )
#define LOGICVC_CLUT_1_B_OFFSET   ( 0x500 * LOGICVC_CLUT_ELEM_BYTE_DIST )
#define LOGICVC_CLUT_2_A_OFFSET   ( 0x600 * LOGICVC_CLUT_ELEM_BYTE_DIST )
#define LOGICVC_CLUT_2_B_OFFSET   ( 0x700 * LOGICVC_CLUT_ELEM_BYTE_DIST )
#define LOGICVC_CLUT_3_A_OFFSET   ( 0x800 * LOGICVC_CLUT_ELEM_BYTE_DIST )
#define LOGICVC_CLUT_3_B_OFFSET   ( 0x900 * LOGICVC_CLUT_ELEM_BYTE_DIST )
#define LOGICVC_CLUT_4_A_OFFSET   ( 0xA00 * LOGICVC_CLUT_ELEM_BYTE_DIST )
#define LOGICVC_CLUT_4_B_OFFSET   ( 0xB00 * LOGICVC_CLUT_ELEM_BYTE_DIST )
/** @} */

/** SPWRCTRL register bitmasks @{ */
#define CVC_EN_BLIGHT_MSK           0x01
#define CVC_EN_VDD_MSK              0x02
#define CVC_EN_VEE_MSK              0x04
#define CVC_V_EN_MSK                0x08
/** @} */

/** SCTRL register bitmasks */
#define CVC_SCTRL_CLOCK_MSK         0x100


/* control register bit positions */

/**************************** Type Definitions ******************************/



/***************** Macros (Inline Functions) Definitions ********************/

/** \addtogroup logiCVClow logiCVC low-level driver - Macros
 *  LogiCVC Macros description  @{
 */


/****************************************************************************/
/**
*
* Writes to any logiCVC register
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    RegisterOffset is the byte offset of the target logiCVC register
* @param    Mask is the 32-bit value to write
*
* @return   None.
*
* @note     None.
*
*****************************************************************************/
#ifdef CVC_FLIP_REG_ENDIAN
 #define CVC_mWriteReg(pRegRange, RegisterOffset,  Mask) \
    OsRegWrite32(pRegRange, RegisterOffset, OsChangeEndian32(Mask))
#else
 #define CVC_mWriteReg(pRegRange, RegisterOffset,  Mask) \
    OsRegWrite32(pRegRange, RegisterOffset, Mask)
#endif


/****************************************************************************/
/**
*
* Reads from any logiCVC register
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    RegisterOffset is the byte offset of the target logiCVC register
*
* @return   Value read.
*
* @note     None.
*
*****************************************************************************/
#ifdef CVC_FLIP_REG_ENDIAN
 #define CVC_mReadReg(pRegRange, RegisterOffset) \
    OsChangeEndian32(OsRegRead32(pRegRange, RegisterOffset))
#else
 #define CVC_mReadReg(pRegRange, RegisterOffset) \
    OsRegRead32(pRegRange, RegisterOffset)
#endif


/****************************************************************************/
/**
*
* Write into CLUT
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    ClutBaseAddress is the base address of the CLUT table
* @param    Index is the CLUT index
* @param    Value is the 32bit value that will be written into CLUT
*
* @return   None.
*
* @note     None.
*
*****************************************************************************/
#define CVC_mCLUTWrite(pRegRange, ClutBaseAddress, Index, Value) \
    CVC_mWriteReg(pRegRange, (ClutBaseAddress) + (Index) * LOGICVC_CLUT_ELEM_BYTE_DIST, Value)


/****************************************************************************/
/**
*
* Read from CLUT
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    ClutBaseAddress is the base address of the CLUT table
* @param    Index is the CLUT index
*
* @return   32 bit Value read
*
* @note     None.
*
*****************************************************************************/
#define CVC_mCLUTRead(pRegRange, ClutBaseAddress, Index) \
    CVC_mReadReg(pRegRange, (ClutBaseAddress) + (Index) * LOGICVC_CLUT_ELEM_BYTE_DIST)


/****************************************************************************/
/**
*
* Write into Layer Alpha register
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    LayerIndex is the number of layer
* @param    AlphaValue is the 8bit value that will be written into layer alpha register
*
* @note     None.
*
*****************************************************************************/
#define CVC_mLayerAlphaWrite(pRegRange, LayerIndex, AlphaValue) \
    CVC_mWriteReg(pRegRange, CVC_LAYER0_BASE_ROFF + (LayerIndex) * CVC_LAYER_DISTANCE + CVC_LAYER_ALPHA_ROFF, AlphaValue)


/****************************************************************************/
/**
*
* Write into Layer Horisontal position registers.
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    LayerIndex is the number of layer
* @param    x_pos is the 16bit value (short) that will be written into layer
*           CVC_LAYER_HOR_POSL_ROFF and CVC_LAYER_HOR_POSH_ROFF registers
* @param    hres is 16bit value (short) needed for the calculation of x_pos value
*
* @note     None.
*
*****************************************************************************/
#define CVC_mLayerPositionHorWrite(pRegRange, LayerIndex, x_pos, hres) \
{ \
    OsU16 pos = (hres) - (x_pos); \
    CVC_mWriteReg(pRegRange, CVC_LAYER0_BASE_ROFF + (LayerIndex) * CVC_LAYER_DISTANCE + CVC_LAYER_HOR_POS_ROFF, pos); \
}


/****************************************************************************/
/**
*
* Write into Layer y position  registers..
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    LayerIndex is the number of layer
* @param    y_pos is the 16bit value (short) that will be written into layer
*           CVC_LAYER_HOR_POSL_ROFF and CVC_LAYER_HOR_POSH_ROFF registers
* @param    vres is 16bit value (short) needed for the calculation of x_pos value
*
* @note     LAST written when changing size, position or offset must be
*           CVC_LAYER_VER_POSL_ROFF register !!!
*
*****************************************************************************/
#define CVC_mLayerPositionVerWrite(pRegRange, LayerIndex, y_pos, vres) \
{ \
    OsU16 pos = (vres) - (y_pos); \
    CVC_mWriteReg(pRegRange, CVC_LAYER0_BASE_ROFF + (LayerIndex) * CVC_LAYER_DISTANCE + CVC_LAYER_VER_POS_ROFF, pos); \
}


/****************************************************************************/
/**
*
* Write into Layer Width registers
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    LayerIndex is the number of layer
* @param    width is the 16bit value (short) that will be written into layer
*            CVC_LAYER_WIDTHL_ROFF and CVC_LAYER_WIDTHH_ROFF registers
*
* @note     None.
*
*****************************************************************************/
#define CVC_mLayerWidthWrite(pRegRange, LayerIndex, width) \
    CVC_mWriteReg(pRegRange, CVC_LAYER0_BASE_ROFF + (LayerIndex) * CVC_LAYER_DISTANCE + CVC_LAYER_WIDTH_ROFF, width);


/****************************************************************************/
/**
*
* Write into Layer Height registers
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    LayerIndex is the number of layer
* @param    height is the 16bit value (short) that will be written into layer
*            CVC_LAYER_HEIGHTL_ROFF and CVC_LAYERHEIGHTH_ROFF registers
*
* @note     None.
*
*****************************************************************************/
#define CVC_mLayerHeightWrite(pRegRange, LayerIndex, height) \
    CVC_mWriteReg(pRegRange, CVC_LAYER0_BASE_ROFF + (LayerIndex) * CVC_LAYER_DISTANCE + CVC_LAYER_HEIGHT_ROFF, height);


/****************************************************************************/
/**
*
* Write into Layer Memory Offset Horisontal registers. Sets the upper left corner offset in the logiCVC memory, from which the image is read.
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    LayerIndex is the number of layer
* @param    x_pos is the 16bit value (short) that will be written into layer
*           CVC_LAYER_HOR_OFFL_ROFF and CVC_LAYER_HOR_OFFH_ROFF registers
*
* @note     None.
*
*****************************************************************************/
#define CVC_mLayerMemOffsetHorWrite(pRegRange, LayerIndex, x_pos) \
    CVC_mWriteReg(pRegRange, CVC_LAYER0_BASE_ROFF + (LayerIndex) * CVC_LAYER_DISTANCE + CVC_LAYER_HOR_OFF_ROFF, x_pos);


/****************************************************************************/
/**
*
* Write into Layer Memory Offset Vertical registers. Sets the upper left corner offset in the logiCVC memory, from which the image is read.
*
* @param    pRegRange is a pointer to the device register range descriptor
* @param    LayerIndex is the number of layer
* @param    y_off is the 16bit value (short) that will be written into layer
*           CVC_LAYER_VER_OFFL_ROFF and CVC_LAYER_VER_OFFH_ROFF registers
*
* @note     None.
*
*****************************************************************************/
#define CVC_mLayerMemOffsetVerWrite(pRegRange, LayerIndex, y_off) \
    CVC_mWriteReg(pRegRange, CVC_LAYER0_BASE_ROFF + (LayerIndex) * CVC_LAYER_DISTANCE + CVC_LAYER_VER_OFF_ROFF, y_off);

/** @} */

#endif  /* end of protection macro */
