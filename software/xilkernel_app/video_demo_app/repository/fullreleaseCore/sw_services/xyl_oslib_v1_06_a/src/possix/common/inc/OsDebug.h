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
* @file OsDebug.h
*
* Contains required debug functionality for the Possix wrapper.
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

#ifndef INC_OsDebug_h
#define INC_OsDebug_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include <stdio.h>

/******************************* Definitions *********************************/
#ifndef OS_ADD_DEBUG_SUPPORT
#define OS_PRINT_INTERNAL(...)
#else
#ifdef __arm__
#define OS_PRINT_INTERNAL(...)       printf(__VA_ARGS__)
#else
#define OS_PRINT_INTERNAL(...)       xil_printf(__VA_ARGS__)
#endif /* #ifdef __arm__ */
#endif /* #ifndef OS_ADD_DEBUG_SUPPORT */

/********************************* Types *************************************/
/**
 *  Trace levels.
 */
typedef enum
{
    OS_TRACE_LEVEL_NONE,
    OS_TRACE_LEVEL_CRITICAL,
    OS_TRACE_LEVEL_FATAL,
    OS_TRACE_LEVEL_ERROR,
    OS_TRACE_LEVEL_WARNING,
    OS_TRACE_LEVEL_INFORMATION,
    OS_TRACE_LEVEL_VERBOSE

} OsTraceLevelT;



/**
******************************************************************************
*
* Formats and prints a series of chars and values to the debug output.
*
* @param        pFormat specifies the format control.
* @param        ... are optional arguments.
*
* @return       None.
*
* @note         See printf for more info.
*
******************************************************************************/
#define OsPrintfError(...) OS_PRINT_INTERNAL(__VA_ARGS__)
#define OsPrintfInfo(...)  OS_PRINT_INTERNAL(__VA_ARGS__)



#ifdef __cplusplus
}
#endif


#endif /* INC_OsDebug_h */
