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
* @file SwConfig_xilkernel.c
*
* @brief This file contains software configuration implementation with
*        xilkernel. It is used for redirection of common system operations,
*        namely:
*        - timers: redirected to xilkernel timer
*        - memory: redirected to malloc/free
*        - thread locks: redirected to xilkernel mutexes
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
* ----- ---- -------- -------------------------------------------------------
* 1.00  AF   27/01/11 First release
* </pre>
*
* modified on 2,Feb,2012 srinivasa attili; for Kintex7 Demo TRD
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xmk.h"
#include "sys/timer.h"
#include <pthread.h>
#include <stdlib.h>
#include "board_test_app.h"
#include "os_config.h"
#include "Oslib.h"


/***************** Macros (Inline Functions) Definitions ********************/

#define PRINT(...)       xil_printf(__VA_ARGS__)
#define ERROR_PRINT(...) PRINT(__VA_ARGS__)


/****************************************************************************/
/*************************** TIMER MANAGEMENT *******************************/

/************************* Function Definitions *****************************/

/**
*
* Initializes timer subsystem.
*
* @param        None.
*
* @return       None.
*
* @note         None.
*
*****************************************************************************/
void swConfig_initTimers()
{
}


/**
*
* Performs a time delay in miliseconds.
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
    sleep(ms);
}


/**
*
* Returns current system timer tick count.
*
* @param        None.
*
* @return       Current tick count.
*
* @note         None.
*
*****************************************************************************/
OsSizeT swConfig_getTickCount()
{
    return xget_clock_ticks();
}


/**
*
* Returns number of system timer ticks per millisecond.
*
* @param        None.
*
* @return       Number of ticks per millisecond.
*
* @note         None.
*
*****************************************************************************/
OsSizeT swConfig_ticksInMs()
{
    return (SYSTMR_CLK_FREQ/SYSTMR_INTERVAL/1000);
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
    return xget_clock_ticks() - ms;
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
******************************************************************************/
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
******************************************************************************/
void swConfig_free(void *p)
{
    free(p);
}


/****************************************************************************/
/************************* THREAD LOCK MANAGEMENT ****************************/

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
    pthread_mutexattr_t attr;
    void *pSpinLock;

    if (pthread_mutexattr_init(&attr) != 0)
		return NULL;

    pSpinLock = OsMemoryAllocate(sizeof(pthread_mutex_t), OS_MEMORY_TYPE__NON_PAGED_POOL);

    if (pSpinLock == NULL)
		return NULL;

    if (pthread_mutex_init((pthread_mutex_t *)pSpinLock, &attr) != 0)
    {
		OsMemoryFree(pSpinLock);
        return NULL;
    }

   return pSpinLock;
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
    if (pLock == NULL)
		return;

    if (pthread_mutex_destroy((pthread_mutex_t *)pLock) != 0)
    {
        //error ?FIX
    }

    OsMemoryFree(pLock);
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
    if (pLock == NULL)
		return;

    pthread_mutex_lock((pthread_mutex_t *)pLock);
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
    if (pLock == NULL)
		return;

    pthread_mutex_unlock((pthread_mutex_t *)pLock);
}
