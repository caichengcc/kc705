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

/**
*
* @file SwConfig_bsp.c
*
* @brief This file contains software configuration implementation in BSP mode.
*        It is used for redirection of common system operations, namely:
*        - timers: redirected to plfTimer
*        - memory: redirected to malloc/free
*        - thread locks: empty implementation (not used in BSP)
*        Any usage of above operations in other libraries, drivers and
*        application software should be redirected to this implementation.\n
*        You should pick one of swconfig implementation templates for your
*        application, and if neither of templates fits your needs, you should
*        take the best match, move it to swconfig folder, rename it and modify.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ------ ---- -------- -------------------------------------------------------
* 1.00.a AF   27/01/11 First release
* 1.01.a DJ   10/10/11 Added system timer definition and new functions for
*             time management.
* </pre>
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include <stdlib.h>
#include "Oslib.h"
#ifdef USE_XYLON_PLATFORM_LIBRARY
#include "plf_timer.h"
#else
#include "xparameters.h"
#ifdef __arm__
#include "xtime_l.h"
#include "sleep.h"
#endif
#endif

/***************** Macros (Inline Functions) Definitions ********************/

#define PRINT(...)       OsPrintfInfo(__VA_ARGS__)
#define ERROR_PRINT(...) PRINT(__VA_ARGS__)

/* Fix for ARM*/
#ifdef XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ
#undef XPAR_CPU_CORE_CLOCK_FREQ_HZ
#define XPAR_CPU_CORE_CLOCK_FREQ_HZ (XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ / 64)
#endif

#define COUNTS_PER_SECOND          (XPAR_CPU_CORE_CLOCK_FREQ_HZ)

/************************* Constant Definitions *****************************/

/**
 * System timer ID specifically designates HW timer instance which is used
 * for time management functionality (time measure, time delay).
 */
#define SYSTEM_TIMER 0

/****************************************************************************/
/*************************** TIMER MANAGEMENT *******************************/
#ifndef USE_XYLON_PLATFORM_LIBRARY
static OsSizeT tmrCnt;
#endif

/************************* Function Definitions *****************************/

/**
*
* Initialize timer subsystem.
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void swConfig_initTimerSubsystem(void)
{
#ifdef USE_XYLON_PLATFORM_LIBRARY
    plfTimer_init();
    plfTimer_start(SYSTEM_TIMER);
#else
#ifdef __arm__
    XTime t=0;
    XTime_SetTime(t);
#else
    tmrCnt=0;
#endif
#endif
}


/**
*
* Returns number of system timer ticks per millisecond.
*
* @return       Number of ticks per millisecond.
*
* @note         None.
*
*****************************************************************************/
OsSizeT swConfig_getTicksInMs(void)
{
#ifdef USE_XYLON_PLATFORM_LIBRARY
    return plfTimer_getTicksInMs();
#else
#ifdef __arm__
    return COUNTS_PER_SECOND/1000;
#else
    return 1;
#endif
#endif
}


/**
*
* Starts system timer.
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void swConfig_startTimer(void)
{
#ifdef USE_XYLON_PLATFORM_LIBRARY
    plfTimer_start(SYSTEM_TIMER);
#endif
}


/**
*
* Stops system timer.
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void swConfig_stopTimer(void)
{
#ifdef USE_XYLON_PLATFORM_LIBRARY
    plfTimer_stop(SYSTEM_TIMER);
#endif
}


/**
*
* Returns current system timer tick count.
*
* @return       Current system timer tick count.
*
* @note         None.
*
*****************************************************************************/
OsSizeT swConfig_getTickCount(void)
{
#ifdef USE_XYLON_PLATFORM_LIBRARY
    return plfTimer_getTickCount(SYSTEM_TIMER);
#else
#ifdef __arm__
    XTime t=0;
    XTime_GetTime(&t);
    return (OsSizeT)t;
#else
    return ++tmrCnt;
#endif
#endif
}


/**
*
* Performs a system time delay in miliseconds.
*
* @param        ms contains the delay value in miliseconds.
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void swConfig_sleep(OsSizeT ms)
{
#ifdef USE_XYLON_PLATFORM_LIBRARY
    plfTimer_sleep(SYSTEM_TIMER, ms);
#else
#ifdef __arm__
    //usleep(ms*1000); // doesn't work?
    OsSizeT t = swConfig_getElapsedMs(0);
    while(swConfig_getElapsedMs(t)<ms);
#else
    volatile long long cnt = XPAR_CPU_CORE_CLOCK_FREQ_HZ*ms/1000;
    while(cnt)
    {
    	--cnt;
    }
    tmrCnt+=ms;
#endif

#endif
}


/**
*
* Returns number of milliseconds elapsed from previous reading.
*
* @param        ms contains previous reading.
*
* @return       Elapsed time in milliseconds.
*
* @note         If parameter 0 is provided, returns absolute time from startup.
*
******************************************************************************/
OsSizeT swConfig_getElapsedMs(OsSizeT ms)
{
#ifdef USE_XYLON_PLATFORM_LIBRARY
    OsSizeT msTicks = plfTimer_getTicksInMs();
    return (OsSizeT)(plfTimer_getTickCount(SYSTEM_TIMER) - (ms * msTicks)) / msTicks;
#else
    OsSizeT msTicks = swConfig_getTicksInMs();
    return (OsSizeT)(swConfig_getTickCount() - (ms * msTicks)) / msTicks;
#endif
}


/****************************************************************************/
/********************** MEMORY ALLOCATION MANAGEMENT ************************/

/************************* Function Definitions *****************************/

/**
*
* Allocates a memory buffer.
*
* @param        size is the number of bytes to allocate.
*
* @return       A memory buffer pointer on success or NULL on failure.
*
* @note         None.
*
*****************************************************************************/
void *swConfig_malloc(OsSizeT size)
{
    return malloc(size);
}


/**
*
* Frees a memory buffer.
*
* @param        p is a memory pointer.
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void swConfig_free(void *p)
{
    free(p);
}


/****************************************************************************/
/************************* THREAD LOCK MANAGEMENT ***************************/

/************************* Function Definitions *****************************/

/**
*
* Creates a spin lock.
*
* @param        hParent is a spin lock parent object.
*
* @return       Pointer to a newly created spin lock; NULL in case of error.
*
*****************************************************************************/
OsSpinLockT *swConfig_spinLockConstruct(OsObjectHandleT hParent)
{
   return NULL;
}


/**
*
* Destroys a spin lock object.
*
* @param        pLock is a spin lock pointer.
*
* @return       None.
*
* @note         None.
*
******************************************************************************/
void swConfig_spinLockDestruct(OsSpinLockT *pLock)
{
}


/**
*
* Acquires a spin lock.
*
* @param        pLock is a spin lock pointer.
*
* @return       None.
*
* @note         Each call must be paired with with a call to OsSpinLockRelease.
*
******************************************************************************/
void swConfig_spinLockAcquire(OsSpinLockT *pLock)
{
}


/**
******************************************************************************
*
* Releases a spinlock.
*
* @param        pLock is a spinlock handle.
*
* @return       None.
*
* @note         Each call must be paired with with a call to OsSpinLockAcquire.
*
******************************************************************************/
void swConfig_spinLockRelease(OsSpinLockT *pLock)
{
}
