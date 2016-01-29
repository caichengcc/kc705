#define TESTAPP_GEN

/* $Id: xgpio_tapp_example.c,v 1.1 2008/09/01 10:16:46 sadanan Exp $ */
/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2005 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file xgpio_tapp_example.c
*
* This file contains a example for using GPIO hardware and driver.
* This example assumes that there is a UART Device or STDIO Device in the
* hardware system.
*
* This example can be run on the Xilinx ML300 board with either the PowerPC or
* the MicroBlaze processor using the Prototype Pins & LEDs of the board
* connected to the GPIO and the Push Buttons connected.
*
* @note
*
* None
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a sv   04/15/05 Initial release for TestApp integration.
* </pre>
*
* modified on 2,Feb,2012 srinivasa attili; modified for Kintex7 Demo
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xparameters.h"
#include "xuartns550_l.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xbasic_types.h"
#include "board_test_app.h"

#if !SIM
#include "stdio.h"
#endif

/************************** Constant Definitions ****************************/

/*
 * The following constant is used to wait after an LED is turned on to make
 * sure that it is visible to the human eye.  This constant might need to be
 * tuned for faster or slower processor speeds.
 */
#define LED_DELAY     1000000

/* following constant is used to determine which channel of the GPIO is
 * used if there are 2 channels supported in the GPIO.
 */
#define LED_CHANNEL 1

#define LED_MAX_BLINK   0x1     /* Number of times the LED Blinks */

#define GPIO_BITWIDTH   16      /* This is the width of the GPIO */

#define printf xil_printf       /* A smaller footprint printf */

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
//#ifndef TESTAPP_GEN
#define GPIO_OUTPUT_DEVICE_ID  XPAR_GPIO_INTERFACE_LEDS_8BITS_DEVICE_ID
#define GPIO_INPUT_DEVICE_ID   XPAR_GPIO_INTERFACE_DIP_SWITCHES_4BITS_DEVICE_ID
//#endif /* TESTAPP_GEN */

#define GPIO_0_BASEADDR XPAR_LEDS_8BIT_BASEADDR

/************************** Constant Definitions *****************************/


/************************** Function Prototypes ******************************/
#if SIM
static void printf(char *s)
{
  while (*s) {
    XUartNs550_SendByte(UART_BASEADDR, *s);
    s++;
  }
}
#endif

/**************************** Type Definitions ******************************/


/***************** Macros (Inline Functions) Definitions *******************/


/************************** Function Prototypes ****************************/

void GpioOutputExample(Xuint16 DeviceId, Xuint32 GpioWidth);

XStatus GpioInputExample(Xuint16 DeviceId, Xuint32 *DataRead);

void GpioDriverHandler(void *CallBackRef);



/************************** Variable Definitions **************************/

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */
XGpio GpioOutput;   /* The driver instance for GPIO Device configured as O/P */
XGpio GpioInput;    /* The driver instance for GPIO Device configured as I/P */

int hello_led (void)
{

	xil_printf("\n\r********************************************************");
	xil_printf("\n\r********************************************************");
	xil_printf("\n\r**     KC705 - GPIO LED Test                          **");
	xil_printf("\n\r********************************************************");
	xil_printf("\n\r********************************************************\r\n");
	xil_printf("Watch the LEDs\r\n");

	GpioOutputExample(GPIO_OUTPUT_DEVICE_ID, 8);

	return 0;
}

/*****************************************************************************/
/**
* Main function to call the example.This function is not included if the
* example is generated from the TestAppGen test tool.
*
* @param    None
*
* @return   XST_SUCCESS if successful, XST_FAILURE if unsuccessful
*
* @note     None
*
******************************************************************************/
int hello_switch (void)
{
    XStatus Status;
    Xuint32 InputData;

    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r**     KC705 - GPIO Switch Test                       **");
    xil_printf("\n\r********************************************************");
    xil_printf("\n\r********************************************************\r\n");

    Status = GpioInputExample((Xuint16)GPIO_INPUT_DEVICE_ID, (Xuint32 *)&InputData);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }

    InputData &= 0x0F;	// Kintex-7: Lower 4 bits are valid
    printf("\nData read from GPIO Input is 0x%x \r\n", (int)InputData);
    printf("    Switch 1: %s\r\n", (InputData & 0x1) ? "ON" : "OFF" );
    printf("    Switch 2: %s\r\n", (InputData & 0x2) ? "ON" : "OFF" );
    printf("    Switch 3: %s\r\n", (InputData & 0x4) ? "ON" : "OFF" );
    printf("    Switch 4: %s\r\n\r\n", (InputData & 0x8) ? "ON" : "OFF" );

    return XST_SUCCESS;
}


/*****************************************************************************/
/**
*
* This function does a minimal test on the GPIO device configured as OUTPUT
* and driver as a  example.
*
*
* @param    DeviceId is the XPAR_<GPIO_instance>_DEVICE_ID value from
*           xparameters.h
* @param    GpioWidth is the width of the GPIO
*
* @return   XST_SUCCESS if successful, XST_FAILURE if unsuccessful
*
* @note     None
*
****************************************************************************/
void GpioOutputExample(Xuint16 DeviceId, Xuint32 GpioWidth)
{
  volatile int Delay;
  Xuint32 LedBit;
  Xuint32 LedLoop;
  XStatus Status;
  int numTimes = 1;

  /*
   * Initialize the GPIO driver so that it's ready to use,
   * specify the device ID that is generated in xparameters.h
   */
  Status = XGpio_Initialize(&GpioOutput, DeviceId);
  if (Status != XST_SUCCESS)
    {
      print("Gpio Instance Didn't Initialize!\r\n");
    }


  /*
   * Set the direction for all signals to be outputs
   */
  XGpio_SetDataDirection(&GpioOutput, 1, 0x0);

  /*
   * Set the GPIO outputs to low
   */
  XGpio_DiscreteWrite(&GpioOutput, 1, 0x0);
  while (numTimes > 0) {
    for (LedBit = 0x0; LedBit < GpioWidth; LedBit++) {
        for (LedLoop = 0; LedLoop < 1; LedLoop++) {

            /*
             * Set the GPIO Output to High
             */
            XGpio_DiscreteWrite(&GpioOutput, 1, 1 << LedBit);

            /*
             * Wait a small amount of time so the LED is visible
             */
            for (Delay = 0; Delay < (500000/2); Delay++)
            {
            	// Dummy read; to ensure the delay has no Cache effect
            	XGpio_DiscreteRead(&GpioOutput, 1);
            }
            /*
             * Clear the GPIO Output
             */
            XGpio_DiscreteClear(&GpioOutput, 1, 1 << LedBit);
	  	}
    }
    numTimes--;
  }
}

/******************************************************************************/
/**
*
* This function  performs a test on the GPIO driver/device with the GPIO
* configured as INPUT
*
* @param    DeviceId is the XPAR_<GPIO_instance>_DEVICE_ID value from
*           xparameters.h
* @param    DataRead is the pointer where the data read from GPIO Input is
*           returned
*
* @return   XST_SUCCESS if the Test is successful, otherwise XST_FAILURE
*
* @note     None.
*
******************************************************************************/
XStatus GpioInputExample(Xuint16 DeviceId, Xuint32 *DataRead)
{
    XStatus Status;

    /*
     * Initialize the GPIO driver so that it's ready to use,
     * specify the device ID that is generated in xparameters.h
     */
    Status = XGpio_Initialize(&GpioInput, DeviceId);
    if (Status != XST_SUCCESS)
    {
		print("Gpio Instance Didn't Initialize!\r\n");
        return XST_FAILURE;
    }

    /*
     * Set the direction for all signals to be inputs
     */
    XGpio_SetDataDirection(&GpioInput, LED_CHANNEL, 0xFFFFFFFF);

    /*
     * Read the state of the data so that it can be  verified
     */
    *DataRead = XGpio_DiscreteRead(&GpioInput, LED_CHANNEL);

    return XST_SUCCESS;

}


