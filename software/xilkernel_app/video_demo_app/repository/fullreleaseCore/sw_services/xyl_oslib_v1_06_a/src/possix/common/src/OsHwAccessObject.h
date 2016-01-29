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
* @file OsHwAccessObject.h
*
* Possix HW access register range typedef.
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a HJ  30/10/09  First release \n
* </pre>
*
******************************************************************************/

#ifndef INC_OsHwAccessObject_h
#define INC_OsHwAccessObject_h

#ifdef __cplusplus
extern "C"
{
#endif



/****************************** Include Files ********************************/
#include "OsTypes.h"



/********************************* Types *************************************/

/**
* This structure contains bus specific descriptor and the function pointers to the
* bus access functions. See \ref OsHwAccessInit for the initialization of this structure.
* Pointer to this structure is returned from the \ref OsHwAccessInit function, used by all access functions.
* \note This structure IS NOT TO BE TOUCHED DIRECTLY by other modules,
* i.e. treat all the members as private to oslib.
*
* \brief Returned from \ref OsHwAccessInit, used by all other access functions.
*/
typedef struct _OsHwAccessObject
{
    /** Specifies the current bus type */
    OsBusTypeE                  busType;

    /** Pointer to bus access descriptor */
    struct _OsHwAccessor *      pHwAccessor;

    /* Speed up read functions. */
    /** Function pointer called by \ref OsRegRead8. */
    OsU8  (* pfRead8)  (struct _OsHwAccessObject const * const pHwAccess,
                        OsRegOffsetT                           offset);
    /** Function pointer called by \ref OsRegRead16. */
    OsU16 (* pfRead16) (struct _OsHwAccessObject const * const pHwAccess,
                        OsRegOffsetT                           offset);
    /** Function pointer called by \ref OsRegRead32. */
    OsU32 (* pfRead32) (struct _OsHwAccessObject const * const pHwAccess,
                        OsRegOffsetT                           offset);

    /* Speed up write functions. */
    /** Function pointer called by \ref OsRegWrite8. */
    void (* pfWrite8)  (struct _OsHwAccessObject const * const pHwAccess,
                        OsRegOffsetT                           offset,
                        OsU8                                   value);
    /** Function pointer called by \ref OsRegWrite16. */
    void (* pfWrite16) (struct _OsHwAccessObject const * const pHwAccess,
                        OsRegOffsetT                           offset,
                        OsU16                                  value);
    /** Function pointer called by \ref OsRegWrite32. */
    void (* pfWrite32) (struct _OsHwAccessObject const * const pHwAccess,
                        OsRegOffsetT                           offset,
                        OsU32                                  value);

    /* FIFO read functions. */
    /** Function pointer called by \ref OsRegFifoRead8. */
    void (* pfFifoRead8)  (struct _OsHwAccessObject const * const pHwAccess,
                           OsRegOffsetT                           offset,
                           OsU8                           * const pBuffer,
                           OsSizeT                                count);
    /** Function pointer called by \ref OsRegFifoRead16. */
    void (* pfFifoRead16) (struct _OsHwAccessObject const * const pHwAccess,
                           OsRegOffsetT                           offset,
                           OsU16                          * const pBuffer,
                           OsSizeT                                count);
    /** Function pointer called by \ref OsRegFifoRead32. */
    void (* pfFifoRead32) (struct _OsHwAccessObject const * const pHwAccess,
                           OsRegOffsetT                           offset,
                           OsU32                          * const pBuffer,
                           OsSizeT                                count);

    /* FIFO write functions. */
    /** Function pointer called by \ref OsRegFifoWrite8. */
    void (* pfFifoWrite8)  (struct _OsHwAccessObject const * const pHwAccess,
                            OsRegOffsetT                           offset,
                            OsU8                     const * const pBuffer,
                            OsSizeT                                count);
    /** Function pointer called by \ref OsRegFifoWrite16. */
    void (* pfFifoWrite16) (struct _OsHwAccessObject const * const pHwAccess,
                            OsRegOffsetT                           offset,
                            OsU16                    const * const pBuffer,
                            OsSizeT                                count);
    /** Function pointer called by \ref OsRegFifoWrite32. */
    void (* pfFifoWrite32) (struct _OsHwAccessObject const * const pHwAccess,
                            OsRegOffsetT                           offset,
                            OsU32                    const * const pBuffer,
                            OsSizeT                                count);

    /* Speed up multi-read functions. */
    /** Function pointer called by \ref OsRegMultiRead8. */
    void (* pfMultiRead8)  (struct _OsHwAccessObject const * const pHwAccess,
                            OsRegOffsetT                           offset,
                            OsU8                           * const pBuffer,
                            OsSizeT                                count);
    /** Function pointer called by \ref OsRegMultiRead16. */
    void (* pfMultiRead16) (struct _OsHwAccessObject const * const pHwAccess,
                            OsRegOffsetT                           offset,
                            OsU16                          * const pBuffer,
                            OsSizeT                                count);
    /** Function pointer called by \ref OsRegMultiRead32. */
    void (* pfMultiRead32) (struct _OsHwAccessObject const * const pHwAccess,
                            OsRegOffsetT                           offset,
                            OsU32                          * const pBuffer,
                            OsSizeT                                count);

    /* Speed up multi-write functions. */
    /** Function pointer called by \ref OsRegMultiWrite8. */
    void (* pfMultiWrite8)  (struct _OsHwAccessObject const * const pHwAccess,
                             OsRegOffsetT                           offset,
                             OsU8                     const * const pBuffer,
                             OsSizeT                                count);
    /** Function pointer called by \ref OsRegMultiWrite16. */
    void (* pfMultiWrite16) (struct _OsHwAccessObject const * const pHwAccess,
                             OsRegOffsetT                           offset,
                             OsU16                    const * const pBuffer,
                             OsSizeT                                count);
    /** Function pointer called by \ref OsRegMultiWrite32. */
    void (* pfMultiWrite32) (struct _OsHwAccessObject const * const pHwAccess,
                             OsRegOffsetT                           offset,
                             OsU32                    const * const pBuffer,
                             OsSizeT                                count);

} OsHwAccessObjectT;



#ifdef __cplusplus
}
#endif


#endif /* INC_OsHwAccessObject_h */
