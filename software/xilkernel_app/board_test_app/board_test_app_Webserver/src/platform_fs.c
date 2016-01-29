/*
 * Copyright (c) 2008, 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Created on 2,Feb,2012 srinivasa attili; for Kintex7 Demo
 *
 */

#include "mfs_config.h"
#include "lwip/sys.h"

/*
 * The MFS library does not appear to be thread safe.
 * Provide big-hammer style mutual exclusion 
 */
static sys_sem_t mfs_mutex;
static sys_sem_t *mfs_mutexp = &mfs_mutex;	// LWIP140

/*
 * The xilmfs library is not thread safe. When used with 
 * xilkernel the mfs_mutex is used to provide mutual exclusion.
 */
void
mfs_lock()
{
    /* Lock mutex */
    //sys_arch_sem_wait(mfs_mutex, 0);	// LWIP130
    sys_arch_sem_wait(mfs_mutexp, 0);	// LWIP140
}

/*
 * The xilmfs library is not thread safe. When used with 
 * xilkernel the mfs_mutex is used to provide mutual exclusion.
 */
void mfs_unlock()
{
    /* unlock mutex */
    //sys_sem_signal(mfs_mutex);	// LWIP130
    sys_sem_signal(mfs_mutexp);		// LWIP140
}

int platform_init_fs() 
{
    extern char _binary_image_mfs_start, _binary_image_mfs_size;

    /* initialize the memory file system (MFS) image pre-loaded into memory */
    xil_printf("Initializing MFS at 0x%08x\n\r", &_binary_image_mfs_start);
    mfs_init_fs((int)&_binary_image_mfs_size,
                (char *)(&_binary_image_mfs_start + 4),
                MFSINIT_IMAGE);

    xil_printf("Done.\n\r");

    /* check if we can access index.html */
    if (mfs_exists_file("index.html") == 0) {
        xil_printf("%s: ERROR: unable to locate index.html in MFS\n\r", __FUNCTION__);
        xil_printf("One of your applications requires a Memory File System to be loaded.\n\r");
        xil_printf("Please check if Memory File System has been loaded,\n\r"
                   "and it has index.html file in root directory\n\r");
        return -1;
    }

    xil_printf("Located index.html\n\r");

    //mfs_mutex = sys_sem_new(1);	// LWIP130
    sys_sem_new(mfs_mutexp, 1);		// LWIP140

    return 0;
}
