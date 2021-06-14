/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This file serves as a gateway to interfacing with the test harness.
 * This example sets enable bits to the PL and reads data back using the AXI Gpio
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include <xgpio.h>
#include "xparameters.h"
#include "sleep.h"

/*
 * Todo: Add this for interrupts
 * 		 using polling for now
 */
//#include "Xscugic.h"

/*
 * Todo: Add this for UART input
 * 		 using physical switches for now
 */

//#include "xuartps_hw.h"

int main() {
	XGpio buttonBus, ledBus, hardwareBus;
	int newData, oldData, leds;
	
	// Initialize the XGPio variables
	XGpio_Initialize(&buttonBus, XPAR_AXI_GPIO_0_DEVICE_ID);	//initialize switches XGpio variable
	XGpio_Initialize(&ledBus, XPAR_AXI_GPIO_1_DEVICE_ID);	//initialize leds XGPio variable
	XGpio_Initialize(&hardwareBus, XPAR_AXI_GPIO_2_DEVICE_ID);	//initialize hardware XGpio variable

	// Set bus channel directions
	XGpio_SetDataDirection(&buttonBus, 1, 0xF);		//set first channel tristate buffer to input
	XGpio_SetDataDirection(&hardwareBus, 1, 0xF);	//set first channel tristate buffer to input
	XGpio_SetDataDirection(&hardwareBus, 2, 0x0);	//set second channel tristate buffer to output
	XGpio_SetDataDirection(&ledBus, 1, 0x0);		//set first channel tristate buffer to output
	
    init_platform();
    print("Hello World\n\r");

    XGpio_DiscreteWrite(&hardwareBus, 1, oldData);

	// Main loop - If button 2 is on, the leds will turn on and according to their frequencies
	while(1) {
		newData = XGpio_DiscreteRead(&buttonBus, 1);

		if (newData != oldData) {
			sleep(50);
			newData = XGpio_DiscreteRead(&buttonBus, 1);
			if (newData != oldData) {
				oldData = newData;
				XGpio_DiscreteWrite(&hardwareBus, 1, oldData);
			}
		}

		leds = XGpio_DiscreteRead(&hardwareBus, 2);
		XGpio_DiscreteWrite(&ledBus, 1, leds);
	}

    cleanup_platform();
    return 0;
}
