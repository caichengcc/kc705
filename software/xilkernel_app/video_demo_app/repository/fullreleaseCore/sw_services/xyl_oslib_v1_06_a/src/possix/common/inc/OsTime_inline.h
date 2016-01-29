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
* @file OsTime_inline.h
*
* Possix time handling inline functions.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a AF   7/01/11  First release \n
* </pre>
*
******************************************************************************/

#ifndef INC_OsTime_inline_h
#define INC_OsTime_inline_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibDebug.h"
#include "OsTime.h"
#include "SwConfig.h"
#include "OsTypes.h"


/************************** Function Definitions *****************************/

/**
******************************************************************************
*
* Initializes time services.
*
* @param        None.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsTimeInit(void)
{
    PATH_TESTED();
    swConfig_initTimerSubsystem();
    swConfig_startTimer();
}


/**
******************************************************************************
*
* Performs a time delay in miliseconds.
*
* @param        ms contains the delay value in miliseconds.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
void
OsMsDelay(
    OsSizeT ms)
{
    PATH_TESTED();
    swConfig_sleep(ms);
}


/**
******************************************************************************
*
* Returns current system timer tick count.
*
* @param        None.
*
* @return       Current tick count.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
OsSizeT
OsTimeGetTickCount(void)
{
    PATH_TESTED();
    return swConfig_getTickCount();
}


/**
******************************************************************************
*
* Returns number of system timer ticks per millisecond.
*
* @param        None.
*
* @return       Number of ticks per millisecond.
*
* @note         None.
*
******************************************************************************/
OS_INLINE
OsSizeT
OsTimeTicksPerMs(void)
{
    PATH_TESTED();
    return swConfig_getTicksInMs();
}


/**
******************************************************************************
*
* Returns number of milliseconds elapsed from previous reading.
*
* @param        ms is value returned from previous reading.
*
* @return       Elapsed time in milliseconds.
*
* @note         If parameter 0 is provided, returns absolute time from startup.
*
******************************************************************************/
OS_INLINE
OsSizeT
OsTimeGetElapsedMs(OsSizeT ms)
{
    PATH_TESTED();
    return swConfig_getElapsedMs(ms);
}


#ifdef __cplusplus
}
#endif


#endif /* INC_OsTime_inline_h */
