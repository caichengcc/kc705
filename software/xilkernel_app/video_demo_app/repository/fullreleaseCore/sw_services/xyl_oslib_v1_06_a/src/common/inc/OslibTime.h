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
* @file OslibTime.h
*
* Contains relevant OS specific time(r) wrappers, as well as the time
* handling functionality common to all the OSs.
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

#ifndef INC_OslibTime_h
#define INC_OslibTime_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OslibTypes.h"
#include "OsTime.h"



/************************** Function Declarations ****************************/

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
void
OsTimeInit(void);


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
void
OsMsDelay(
    OsSizeT ms);


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
OsSizeT
OsTimeGetTickCount(void);


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
OsSizeT
OsTimeTicksPerMs(void);


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
OsSizeT
OsTimeGetElapsedMs(
    OsSizeT ms);



/****************************** Include Files ********************************/
#include "OsTime_inline.h"



#ifdef __cplusplus
}
#endif


#endif /* INC_OslibTime_h */
