#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include <xgpio.h>
#include "xparameters.h"
#include "sleep.h"

#include "xstatus.h"
#include "xil_types.h"
#include "xil_assert.h"
#include "xuartps_hw.h"

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define UART_BASEADDR		XPAR_XUARTPS_0_BASEADDR
#define UART_CLOCK_HZ		XPAR_XUARTPS_0_CLOCK_HZ

/*
 * The following constant controls the length of the buffers to be sent
 * and received with the device. This constant must be 32 bytes or less so the
 * entire buffer will fit into the TX and RX FIFOs of the device.
 */
#define TEST_BUFFER_SIZE	16

#define CHAR_ESC		0x1b	/* 'ESC' character is used as terminator */

/*
 * The following buffers are used in this example to send and receive data
 * with the UART.
 */
u8 SendBuffer[TEST_BUFFER_SIZE];	/* Buffer for Transmitting Data */

int main2(void) {
	int Index;
	u32 Running;
	u8 RecvChar;
	u32 UartBaseAddress = UART_BASEADDR;
	u32 CntrlRegister;

	CntrlRegister = XUartPs_ReadReg(UartBaseAddress, XUARTPS_CR_OFFSET);

	/* Enable TX and RX for the device */
	XUartPs_WriteReg(UartBaseAddress, XUARTPS_CR_OFFSET,
			  ((CntrlRegister & ~XUARTPS_CR_EN_DIS_MASK) |
			   XUARTPS_CR_TX_EN | XUARTPS_CR_RX_EN));

	/*
	 * Initialize the send buffer bytes with a pattern to send and the
	 * the receive buffer bytes to zero
	 */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		SendBuffer[Index] = Index + '0';
	}

	/* Send the entire transmit buffer. */
	for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
		/* Wait until there is space in TX FIFO */
		 while (XUartPs_IsTransmitFull(UartBaseAddress));

		/* Write the byte into the TX FIFO */
		XUartPs_WriteReg(UartBaseAddress, XUARTPS_FIFO_OFFSET,
				  SendBuffer[Index]);
	}

	Running = TRUE;
	while (Running) {
		 /* Wait until there is data */
		while (!XUartPs_IsReceiveData(UartBaseAddress));

		RecvChar = XUartPs_ReadReg(UartBaseAddress,
						XUARTPS_FIFO_OFFSET);

		/* Change the capitalization */
		if (('a' <= RecvChar) && ('z' >= RecvChar)) {
			/* Convert the Capital letter to a small. */
			RecvChar = RecvChar - 'a' + 'A';
		}
		else if (('A' <= RecvChar) && ('Z' >= RecvChar)) {
			/* Convert the small letter to a Capital. */
			RecvChar = RecvChar - 'A' + 'a';
		}
		else if (CHAR_ESC == RecvChar) {
			Running = FALSE;
		}

		/* Echo the character back */
		XUartPs_WriteReg(UartBaseAddress,  XUARTPS_FIFO_OFFSET,
				  RecvChar);
	}


	return XST_SUCCESS;
}
