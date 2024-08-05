/******************************************************************************
* Copyright (C) 2002 - 2020 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
* Edited By Linn
******************************************************************************/

/*****************************************************************************/
/**
* @file led_main.c
*
* This file contains a design example using the AXI GPIO driver (XGpio) and
* hardware device.  It only uses channel 1 of a GPIO device and assumes that
* the bit 0 of the GPIO is connected to the LED on the HW board.
*
* PS可以通过AXI总线控制PL，但视乎没有体现出ZYNQ的优势，因为对于控制LED灯，无论是ARM还是FPGA，都可以轻松完成
* 但是如果把LED换成串口呢，控制100路串口通信，8路以太网等应用，还没有哪个SOC能完成这种功能，只有ZYNQ可以
* 这就是ZYNQ和普通SOC的不同之处。
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rmm  03/13/02 First release
* 1.00a rpm  08/04/03 Removed second example and invalid macro calls
* 2.00a jhl  12/15/03 Added support for dual channels
* 2.00a sv   04/20/05 Minor changes to comply to Doxygen and coding guidelines
* 3.00a ktn  11/20/09 Minor changes as per coding guidelines.
* 4.1   lks  11/18/15 Updated to use canonical xparameters and
*		      clean up of the comments and code for CR 900381
* 4.3   sk   09/29/16 Modified the example to make it work when LED_bits are
*                     configured as an output. CR# 958644
*       ms   01/23/17 Added xil_printf statement in main function to
*                     ensure that "Successfully ran" and "Failed" strings
*                     are available in all examples. This is a fix for
*                     CR-965028.
* 4.5  sne   06/12/19 Fixed IAR compiler warning.
*
* </pre>
******************************************************************************/

/***************************** Include Files *********************************/

#include "xparameters.h"
#include "xgpio.h"
#include "xil_printf.h"

/************************** Constant Definitions *****************************/

//#define LED 0x01   /* Assumes bit 0 of GPIO is connected to an LED  */
#define LED 0x0F /* 四个LED对应b1111 */

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define GPIO_LED_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID
#define GPIO_SW_DEVICE_ID	XPAR_GPIO_1_DEVICE_ID

/*
 * The following constant is used to wait after an LED is turned on to make
 * sure that it is visible to the human eye.  This constant might need to be
 * tuned for faster or slower processor speeds.
 */
#define LED_DELAY     10000000

/*
 * The following constant is used to determine which channel of the GPIO is
 * used for the LED if there are 2 channels supported.
 */
#define LED_CHANNEL 1 //取决于IP核配置时选择GPIO还是GPIO2，若为GPIO则Channel为1
#define SW_CHANNEL	1

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

#ifdef PRE_2_00A_APPLICATION

/*
 * The following macros are provided to allow an application to compile that
 * uses an older version of the driver (pre 2.00a) which did not have a channel
 * parameter. Note that the channel parameter is fixed as channel 1.
 */
#define XGpio_SetDataDirection(InstancePtr, DirectionMask) \
        XGpio_SetDataDirection(InstancePtr, LED_CHANNEL, DirectionMask)

#define XGpio_DiscreteRead(InstancePtr) \
        XGpio_DiscreteRead(InstancePtr, LED_CHANNEL)

#define XGpio_DiscreteWrite(InstancePtr, Mask) \
        XGpio_DiscreteWrite(InstancePtr, LED_CHANNEL, Mask)

#define XGpio_DiscreteSet(InstancePtr, Mask) \
        XGpio_DiscreteSet(InstancePtr, LED_CHANNEL, Mask)

#endif

/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/

/*
 * The following are declared globally so they are zeroed and so they are
 * easily accessible from a debugger
 */
u32 sw_val;

/* The Instance of the GPIO Driver */
XGpio GpioInput;
XGpio GpioOutput;

/*****************************************************************************/
/**
*
* The purpose of this function is to illustrate how to use the GPIO
* driver to turn on and off an LED.
*
* @param	None
*
* @return	XST_FAILURE to indicate that the GPIO Initialization had
*		failed.
*
* @note		This function will not return if the test is running.
*
******************************************************************************/
int init_gpio();

int main(void)
{
	volatile int Delay;
    unsigned char led_states;  // 存储灯的状态

	/* Initialize */
	init_gpio();

	/* Loop forever blinking the LED */

	while (1) {
		sw_val = XGpio_DiscreteRead(&GpioInput, SW_CHANNEL);

		if (sw_val) {
			// 闪烁模式
			print("LED state: 0x0F\n\r");
			/* Set the LED to High */
			XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, LED);

			/* Wait a small amount of time so the LED is visible */
			for (Delay = 0; Delay < LED_DELAY; Delay++);

			/* Clear the LED bit */
			XGpio_DiscreteClear(&GpioOutput, LED_CHANNEL, LED);

			/* Wait a small amount of time so the LED is visible */
			for (Delay = 0; Delay < LED_DELAY; Delay++);
		} else {
			// 流水灯模式
			for (led_states = 0; led_states < 4; led_states++) {  // 遍历四个位置
				xil_printf("LED state: 0x%02X\n\r", led_states+1);  // 输出灯的位置
				XGpio_DiscreteWrite(&GpioOutput, LED_CHANNEL, (1 << led_states));
				for (Delay = 0; Delay < LED_DELAY; Delay++);
				XGpio_DiscreteClear(&GpioOutput, LED_CHANNEL, (1 << led_states));
				for (Delay = 0; Delay < LED_DELAY; Delay++);
			}
		}
	}

}

/* Initialize the GPIO driver */
int init_gpio() {
	int Status;
	print("Wait to initialize\n\r");

	// SW_INIT
	Status = XGpio_Initialize(&GpioInput, GPIO_SW_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	} else {
		print("SW_DEVICE well!\n\r");
	}

	XGpio_SetDataDirection(&GpioInput, SW_CHANNEL, 1);

	// LED_INIT
	Status = XGpio_Initialize(&GpioOutput, GPIO_LED_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	} else {
		print("Glad to see u! Bling!\n\r");
	}

	/* Set the direction for all signals as inputs except the LED output */
	XGpio_SetDataDirection(&GpioOutput, LED_CHANNEL, ~LED); //第三位参数为方向设置掩码，0为输出 1为输入

    return XST_SUCCESS;
}
