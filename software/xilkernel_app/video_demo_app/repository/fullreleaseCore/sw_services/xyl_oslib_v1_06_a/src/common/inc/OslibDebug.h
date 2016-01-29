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
* @file OslibDebug.h
*
* Contains relevant OS specific debug wrappers, as well as the debug
* functionality common to all the OSs.
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

#ifndef INC_OslibDebug_h
#define INC_OslibDebug_h

#ifdef __cplusplus
extern "C"
{
#endif

/*  - if NDEBUG is not defined (debug build  ) we have full debug support
    - if NDEBUG is     defined (release build) we have no   debug support
    - if NDEBUG and OSLIB_ADD_DEBUG are defined have print but no assert/PathTested
   */
#if (!defined NDEBUG) || (defined OSLIB_ADD_DEBUG)
#define OS_ADD_DEBUG_SUPPORT
#endif

/****************************** Include Files ********************************/
#include "OslibTypes.h"         /* Import TEXT() */
#include "OsDebug.h"



/******************************* Definitions *********************************/
#ifndef NDEBUG

/* Multi-level macros needed to launder __LINE__ */
#define FILENAME_AND_LINENUMBER_STRING2(file, line) TEXT(file) TEXT(":") TEXT(#line)
#define FILENAME_AND_LINENUMBER_STRING(file, line)  FILENAME_AND_LINENUMBER_STRING2(file, line)

/** Oslib assert macro. */
#define OS_ASSERT(e)            \
    ((void)((e) || (OsPrintfError(TEXT("ASSERT ") FILENAME_AND_LINENUMBER_STRING(__FILE__, __LINE__) TEXT("\r\n")), 0)))

/** Debug code-coverage macro. Use this macro when first writting some part of code */
#define PATH_NOT_YET_TESTED()   \
    OsPrintfInfo(TEXT("PNT ") FILENAME_AND_LINENUMBER_STRING(__FILE__, __LINE__) TEXT("\r\n"))

/** Debug code-coverage macro. Use this macro when some part of code past initial tests.
    Rename PATH_NOT_YET_TESTED to PATH_ALMOST_TESTED */
#define PATH_ALMOST_TESTED()    \
    OsPrintfInfo(TEXT("PAT ") FILENAME_AND_LINENUMBER_STRING(__FILE__, __LINE__) TEXT("\r\n"))

/** Debug code-coverage macro. Use this macro when some part of code passed testing
    Rename PATH_ALMOST_TESTED to PATH_TESTED */
#define PATH_TESTED()           do { } while(0)

#else

/** Oslib assert macro. */
#define OS_ASSERT(e)            ((void) 0)

/** Debug code-coverage macro. Use this macro when first writting some part of code */
#define PATH_NOT_YET_TESTED()   do { } while(0)

/** Debug code-coverage macro. Use this macro when some part of code past initial tests.
    Rename PATH_NOT_YET_TESTED to PATH_ALMOST_TESTED */
#define PATH_ALMOST_TESTED()    do { } while(0)

/** Debug code-coverage macro. Use this macro when some part of code passed testing
    Rename PATH_ALMOST_TESTED to PATH_TESTED */
#define PATH_TESTED()           do { } while(0)

#endif



/************************** Function Declarations ****************************/

/**
******************************************************************************
*
* Initializes debug support.
*
* @return       None.
*
* @note         This function must be called before any other debug functions
*               (e.g. \ref OsPrintfInfo) or macros (e.g. \ref PATH_NOT_YET_TESTED)
*               are called.
*
******************************************************************************/
void
OsDebugInit(void);



/**
******************************************************************************
*
* Sets debug level and flags.
*
* @param        traceLevel specifies the max message importance level to be
*               printed out.
* @param        traceFlags specifies the components to be printed out.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void
OsDebugSet(
    OsTraceLevelT traceLevel,
    OsU32         traceFlags);



/**
******************************************************************************
*
* Formats and prints a series of wchars and values to the debug output
* depending on the trace level and flags.
*
* @param        traceLevel specifies the message importance level.
* @param        traceFlags specifies the component calling the function.
* @param        pFormat specifies the format control.
* @param        ... are optional arguments.
*
* @return       None.
*
* @note         See printf for more info on pFormat.
*
******************************************************************************/
void
OsPrintfTrace(
    OsTraceLevelT traceLevel,
    OsU32         traceFlags,
    OsCharT     * pFormat,
    ...);



/**
******************************************************************************
*
* Formats and prints a series of wchars and values to the debug output
* depending on the trace level and flags, as well as on a specified condition.
*
* @param        traceLevel specifies the message importance level.
* @param        traceFlags specifies the component calling the function.
* @param        bCond is FALSE if the message is not to be output.
* @param        pFormat specifies the format control.
* @param        ... are optional arguments.
*
* @return       None.
*
* @note         See printf for more info on pFormat.
*
******************************************************************************/
void
OsPrintfTraceCond(
    OsTraceLevelT traceLevel,
    OsU32         traceFlags,
    BOOLEAN       bCond,
    OsCharT     * pFormat,
    ...);



/****************************** Include Files ********************************/
#include "OsDebug_inline.h"



#ifdef __cplusplus
}
#endif


#endif /* INC_OslibDebug_h */
