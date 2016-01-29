/**
** This confidential and proprietary code may be used only as authorized
** by licensing agreement from Xylon d.o.o.
** In the event of publication, the following notice is applicable:
**
** Copyright 2010 - Xylon d.o.o.
** All rights reserved.
**
** The entire notice above must be reproduced on all authorized copies.
******************************************************************************/
/**
*
* @file logiSDHC_fatfs_example.c
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver    Who  Date     Changes \n
* -----  ---- -------- ------------------------------------------------------- \n
* 1.00.a GP   30/06/08 First release \n
* 1.10.a DJ   10/11/10 Changed code according to FAT FS library \n
* </pre>
*
******************************************************************************/
#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xparameters.h"
#include "sd_test.h"
#include "fatfs.h"
//#include "file.h"

#undef MEASURE_SPEED
#ifdef MEASURE_SPEED
#include "Oslib.h"
#endif

#ifndef PRINT
#define PRINT(...)       printf(__VA_ARGS__)
#define ERROR_PRINT(...) PRINT
#define DEBUG_PRINT(...) PRINT
#endif

#define COPY_FILES
#define VERIFY_DATA
#define MAX_NUM_OF_FILES_TO_COPY 128
#define MAX_FILE_SIZE 10000000

static FATFS fs;
/* Buffers to store data */
//static BYTE buffer[1000000];
//static char read_buffer[MAX_FILE_SIZE];
static char compare_buffer[MAX_FILE_SIZE];
/* Total file counter */
static int file_counter = 0;
int filter_file_counter = 0;
/* In/Out filename arrays */
static char in_filename_string[MAX_NUM_OF_FILES_TO_COPY][32];
static char out_filename_string[MAX_NUM_OF_FILES_TO_COPY][32];
extern char* filter_filename_string[];
extern char read_buffer[];
int sd_file_read(char *filename, char *read_buffer);

#ifdef MEASURE_SPEED
static float top_write_speed, top_read_speed;
static long top_write_data_size, top_read_data_size;
#endif

static void filter_file_list( char* filters[], void* file_list, int rows, int cols )
{
	int i,j;
	char* tmp;

	filter_file_counter = 0;

	// Limit the file list to max rows set by caller
	if(file_counter > rows)
		file_counter = rows;

	for(i=0; i < file_counter; i++)
	{
		for(j=0; filters[j] != NULL; j++)
		{
			tmp = strstr(in_filename_string[i], filters[j]);
			if(tmp != NULL)
			{
				// This filename matches to one of the filters. Pick it.

				xil_printf("rows=%d, cols=%d, filtercntr=%d, ptr=0x%x\n",
						rows, cols, filter_file_counter,
						(unsigned int)(((char *)file_list) + (filter_file_counter * cols) + 0) );
				strcpy( (((char *)file_list) + (filter_file_counter * cols) + 0),
						in_filename_string[i]);
				filter_file_counter++;
				break;
			}
		}
	}

	xil_printf("========== FILTERED FILE LIST: # %d ==========\n", filter_file_counter);
	for(i=0; i<filter_file_counter; i++)
		xil_printf("%s\n", (((char *)file_list) + (i * cols) + 0) );
}

static void scan_files_recursive(char* path)
{
    FILINFO finfo;
    DIR dirs;
    char out_filename[64] = "c";

    PRINT("========== scan_files_recursive: %s ==========\n", path);

    if (f_opendir(&dirs, path) == FR_OK)
    {
        while ((f_readdir(&dirs, &finfo) == FR_OK) && finfo.fname[0])
        {
            if (finfo.fattrib & AM_DIR)
            {
            	//strcat(path, "\\");
            	//strcat(path, finfo.fname);
            	//PRINT("==== new scan path: %s\n", path);
                //scan_files_recursive(path);
                //path[i] = 0;
            }
            else
            {
                PRINT("%s/%s\n", path, &finfo.fname[0]);
#ifdef COPY_FILES
                /* Create output filenames */
                if(file_counter < MAX_NUM_OF_FILES_TO_COPY)
                {
                    sprintf(out_filename,"c%s", &finfo.fname[0]);
                    strcpy(in_filename_string[file_counter], &finfo.fname[0]);
                    strcpy(out_filename_string[file_counter], out_filename);
                    file_counter++;
                }
#endif
            }
        }
    }
    else
    {
    	PRINT("========== Unable to open the path: %s ==========\n", path);
	}
}

static void scan_files(char* path)
{
    FILINFO finfo;
    DIR dirs;
    int i;
    char out_filename[64] = "c";

    if (f_opendir(&dirs, path) == FR_OK)
    {
        i = strlen(path);

        while ((f_readdir(&dirs, &finfo) == FR_OK) && finfo.fname[0])
        {
            if (finfo.fattrib & AM_DIR)
            {
                sprintf(&path[i], "/%s", &finfo.fname[0]);
                scan_files(path);
                path[i] = 0;
            }
            else
            {
                PRINT("%s/%s\n", path, &finfo.fname[0]);
#ifdef COPY_FILES
                /* Create output filenames */
                if(file_counter < MAX_NUM_OF_FILES_TO_COPY)
                {
                    sprintf(out_filename,"c%s", &finfo.fname[0]);
                    strcpy(in_filename_string[file_counter], &finfo.fname[0]);
                    strcpy(out_filename_string[file_counter], out_filename);
                    file_counter++;
                }
#endif
            }
        }
    }
}


/* Copy files using PLF wrappers */
void copy_file(char *in_filename, char *out_filename)
{
    FIL f, *fp;
#ifdef MEASURE_SPEED
    /* Time measuring variables */
    unsigned long timer_start, timer_end, timer_diff;
    float current_speed;
#endif
    unsigned int file_size, bytes_read, bytes_written, j;
    int read, ret;

    fp = &f;

    /* Open file for read */
    ret = f_open(fp, in_filename, FA_READ);
    if (ret != 0)
    {
        PRINT("Input file open error!\n\rError %d\n\r", ret);
        return;
    }
    file_size = f_size(fp);
    if (file_size > MAX_FILE_SIZE)
    {
        PRINT("Input file to big for test!\n\rFile size %d\n\r", file_size);
        return;
    }

#ifdef MEASURE_SPEED
    /* Start time measure */
    timer_start = swConfig_getTickCount();
#endif

    ret = f_read(fp, (void *)read_buffer, file_size, &bytes_read);
    if (bytes_read != file_size)
    {
        PRINT("Input file read error!\n\rBytes read %d : Bytes needed %d\n\r", bytes_read, file_size);
        return;
    }

#ifdef MEASURE_SPEED
    /* Stop time measure */
    timer_end = swConfig_getTickCount();
    timer_diff = timer_end - timer_start;
    //printf("time_diff = %d\n\r",timer_diff);
    PRINT("%s (%.3f KB)\n\r", in_filename, (float)bytes_read/1000);
    current_speed = ((float)bytes_read/((float)timer_diff/(float)swConfig_getTicksInMs()));
    PRINT("File read speed  = %.3f KB/s\n\r", current_speed);
    if(current_speed > top_read_speed)
    {
        top_read_speed = current_speed;
        top_read_data_size = bytes_read;
    }
#endif
    /* Close the input file */
    ret = f_close(fp);
    if (ret != 0)
    {
        PRINT("Input file close error!\n\rError %d\n\r", ret);
        return;
    }

    /* Write to output file */
    ret = f_open(fp, out_filename, (FA_CREATE_ALWAYS | FA_WRITE));
    if (ret != 0)
    {
        PRINT("Output file open error!\n\rError %d\n\r", ret);
        return;
    }

#ifdef MEASURE_SPEED
    timer_start = swConfig_getTickCount();
#endif

    ret = f_write(fp, (void *)read_buffer, bytes_read, &bytes_written);
    if (bytes_written != bytes_read)
    {
        PRINT("Output file write error!\n\rError %d\n\r", ret);
        return;
    }

#ifdef MEASURE_SPEED
    timer_end = swConfig_getTickCount();
    timer_diff = timer_end - timer_start;
    current_speed = ((float)bytes_written/((float)timer_diff/(float)swConfig_getTicksInMs()));
    PRINT("File write speed = %.3f KB/s\n\r", current_speed);
    if(current_speed > top_write_speed)
    {
        top_write_speed = current_speed;
        top_write_data_size = bytes_written;
    }
#endif
    /* Close the output file */
    ret = f_close(fp);
    if (ret != 0)
    {
        PRINT("Output file close error!\n\rError %d\n\r", ret);
//        return;
    }

#ifdef VERIFY_DATA
    /* verify data */
    PRINT("Verifying data...\n\r");
    bytes_read = 0;

    ret = f_open(fp, out_filename, FA_READ);
    if (ret != 0)
    {
        PRINT("Output file open error!\n\rError %d\n\r", ret);
        return;
    }

    ret = f_read(fp, compare_buffer, file_size, &bytes_read);
    if (bytes_read != file_size)
    {
        PRINT("Input file read error!\n\rBytes read %d : Bytes needed %d\n\r", bytes_read, file_size);
        return;
    }

    ret = f_close(fp);
    if (ret != 0)
    {
        PRINT("Input file close error!\n\rError %d\n\r", ret);
        return;
    }

    /* Compare data */
    for(j = 0; j < bytes_read; j++)
    {
        if(read_buffer[j] != compare_buffer[j])
        {
            PRINT("Data diff: %d. %c != %c\n\r", j, read_buffer[j], compare_buffer[j]);
        }
    }

    PRINT("%s - Verified\n\r\n\r", out_filename);
#endif
}

/* Copy files using PLF wrappers */
int sd_file_read(char *filename, char *read_buffer)
{
    FIL f, *fp;
    unsigned int file_size, bytes_read, bytes_written, j;
    int read, ret;

    PRINT("sd_file_read: %s\n", filename);
    fp = &f;

    /* Open file for read */
    ret = f_open(fp, filename, FA_READ);
    if (ret != 0)
    {
        PRINT("Input file open error!\n\rError %d\n\r", ret);
        return -1;
    }

    file_size = f_size(fp);
    PRINT("file size: %d bytes\n", file_size);
    if (file_size > MAX_FILE_SIZE)
    {
        PRINT("Input file to big for test!\n\rFile size %d\n\r", file_size);
        return -1;
    }

    ret = f_read(fp, (void *)read_buffer, file_size, &bytes_read);
    if (bytes_read != file_size)
    {
        PRINT("Input file read error!\n\rBytes read %d : Bytes needed %d\n\r", bytes_read, file_size);
        return -1;
    }

    /* Close the input file */
    ret = f_close(fp);
    if (ret != 0)
    {
        PRINT("Input file close error!\n\rError %d\n\r", ret);
        return -1;
    }

    return file_size;	// SUCCESS
}

int is_sd_accessible(void)
{
	volatile unsigned int present_state;
	int timer=0;

	xil_printf("check SD card accessibility \n");
	#if 0
	IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2F) = 0x01;  	// Soft reset
	IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x2C) = 0x205; 	// Clock ctrl reg; SD clock frequency = 50MHz/2, SD clk en, Internal clk en
	IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x28) = 0x00;  	// Host ctrl reg; High speed disabled, Data trans width = 1, (0x06; Host ctrl reg; High speed enabled, Data trans width = 4)  ; 0x04 High speed
	IN8(XPAR_LOGISDHC_0_REGS_BASEADDR+0x29) = 0x0F;  	// Power ctrl reg; SD Bus voltage select, SD Bus Power
	IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x36) = 0xFFFF;  // Error interrupt status en reg; Enable all
	IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x34) = 0xFFFF;  // Normal interrupt status en reg; Enable all
	IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x38) = 0x0008;  // Normal interrupt signal en reg; Enable all
	IN16(XPAR_LOGISDHC_0_REGS_BASEADDR+0x3A) = 0x0000;  // Error interrupt signal en reg; dis Enable all
	#endif

   for(timer=0; timer < 100; timer++)
   {
	   present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1) before doing anything
	   if((present_state & 0x3) == 0)
		   break;
   }
   present_state = IN32(XPAR_LOGISDHC_0_REGS_BASEADDR+0x24+0);  //Check Present state reg (bit 0 and 1) before doing anything
   if((present_state & 0x3) != 0)
	   return -1;	// BAD, SD card is not accessible.

   return 0;		// GOOD, SD card is accessible.
}

int main_SDHCExample()
{
    unsigned int j;
    char *filters[] = {".JPG", ".PNG", NULL};
    int status;

#if 0
#ifdef XPAR_MICROBLAZE_USE_ICACHE
    microblaze_invalidate_icache();
    microblaze_enable_icache();
#endif
#ifdef XPAR_MICROBLAZE_USE_DCACHE
    microblaze_invalidate_dcache();
    microblaze_enable_dcache();
#endif
#endif

    status = is_sd_accessible();
    if(status == -1)
    {
    	xil_printf("SD card is not accessible.. insert properly and rerun\n");
    	return -1;
    }

    /* Initialize FATFS and timer */
    f_mount(0, &fs);
#ifdef MEASURE_SPEED
    swConfig_initTimerSubsystem();
#endif

    /* Scan file structure */
    scan_files_recursive("");	// param: parent path
    xil_printf("total files: %d\n", file_counter);
    filter_file_list(filters, filter_filename_string, MAX_NUM_OF_FILES_TO_COPY,32);

#ifdef COPY_FILES
    /* Copy and verify all scanned files */
    for(j = 0;j < file_counter; j++)
    {
    	//xil_printf("%s\n", in_filename_string[j]);

		#if 0
        PRINT("\n\rCopying file %d of %d...\n\r", (j + 1), file_counter);
        if((strlen(in_filename_string[j]) > (8 + 3 + 1)) || (strlen(out_filename_string[j]) > (8 + 3 + 1)))
        {
            PRINT("\n\rFilename %s is to long for filesystem!\n\r", out_filename_string[j]);
            PRINT("Only filenames of max 8 + 3 characters(xxxxxxxx.yyy) are supported\n\r\n\r");
        }
        else
        {
            copy_file(in_filename_string[j], out_filename_string[j]);
        }
		#endif
    }
#endif

#ifdef MEASURE_SPEED
    printf("Top write speed = %8.2f for data size = %8d\n\r", top_write_speed, top_write_data_size);
    printf("Top read speed = %8.2f for data size = %8d\n\r", top_read_speed, top_read_data_size);
#endif

    f_mount(0, (FATFS *)0);

    PRINT("SD Card test end\n\r");
    //while(1);

    return 0;
}

static int mounted=0;
int get_SDFS_imageList(void* file_list, int rows, int cols)
{
    unsigned int j, status;
    char *filters[] = {".JPG", ".PNG", NULL};

    /* Initialize FATFS and timer */
    filter_file_counter=0;
   	if(mounted == 0)
    {
        status = is_sd_accessible();
        if(status != 0)
        {
        	xil_printf("SD card is not accessible.. insert properly and rerun\n");
        	return -1;
        }

        f_mount(0, &fs);
    	mounted = 1;
    }

    /* Scan file structure */
    scan_files_recursive("");	// param: root path
    filter_file_list(filters, file_list, rows, cols);

    return 0;
    //f_mount(0, (FATFS *)0);
}

#endif	// 0
