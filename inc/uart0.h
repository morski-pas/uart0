// ----------------------------------------------------------------------------
// description   The uart0 library provides functions to configure UART0 and
//               functions for sending and receiving data.
// ----------------------------------------------------------------------------
#ifndef UART0_H_
#define UART0_H_

#ifndef F_CPU
 #define F_CPU 16000000
#endif

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>


enum UartParity
{
	UART_P_NONE,            // no parity
	UART_P_EVEN,            // parity even
	UART_P_ODD              // parity odd
};

enum UartMode
{
	UART_M_RECEIVE,         // reception only
	UART_M_TRANSMIT,        // transmission only
	UART_M_TRANSCEIVE       // reception and transmission
};

// ----------------------------------------------------------------------------
// descr.   Configures UART0 for data reception and/or data transmission
//          in asynchronous mode with 8 data and 2 stop bits.
// param.   baudrate        the baudrate
//          mode            the mode
//          parity          the parity configuration
// return   -
// ----------------------------------------------------------------------------
void uart0_init(uint32_t baudrate, enum UartMode mode, enum UartParity parity);

// ----------------------------------------------------------------------------
// descr.   Waits until a character has been received by UART0 and afterwards
//          returns the character. uart0_getc is thus a blocking function.
// param.   -
// return   the received character
// ----------------------------------------------------------------------------
uint8_t uart0_getc();

// ----------------------------------------------------------------------------
// descr.   Waits until a character can be sent and sends character afterwards.
//          uart0_getc is thus a blocking function.
// param.   c       character to be sent
// return   -
// ----------------------------------------------------------------------------
void uart0_putc(char c);

// ----------------------------------------------------------------------------
// descr.   Utilizes uart0_getc(); to send a string
// param.   _str    string to be sent
// return   -
// ----------------------------------------------------------------------------
void uart0_puts(char* _str);

// ----------------------------------------------------------------------------
// descr.   sets callback function for interrupt-based uart0 reception and
//          calls sei(); to globally enable interrupts.
// param.   address of uart0 reception function
// return   -
// ----------------------------------------------------------------------------
void uart0_registerReceptionCallback(void (*pCallback)(uint8_t data));

#endif /* UART0_H_ */