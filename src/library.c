#include "uart0.h" // is located in same directory, therefore "*.h"
#include <avr/interrupt.h>
#include <string.h>

void (*pRecepionCallback)(uint8_t data) = NULL; // null indicated that no callback function has been registered yet
char sendBuffer[32] = ""; // felder san imma volatile
volatile uint8_t sendBufferIndex = 0;

void uart0_init(uint32_t baudrate, enum UartMode mode, enum UartParity parity)
{
	UCSR0A = 0x00;
	UCSR0B = 0x00; 
	
	switch(mode)
	{
		case UART_M_RECEIVE:
		{
			UCSR0B |= (1 << RXEN0);
			break;
		}

		case UART_M_TRANSMIT:
		{
			UCSR0B |= (1 << TXEN0);
			break;
		}

		case UART_M_TRANSCEIVE:
		{
			UCSR0B |= ((1 << RXEN0) | (1 << TXEN0));
			break;
		}

		default: break;
	}

	UCSR0C = ((1 << USBS0) | (0b11 << UCSZ00));

	switch(parity)
	{
		case UART_P_NONE: break;
		case UART_P_EVEN: UCSR0C |= (0b10 << UPM00); break;
		case UART_P_ODD:  UCSR0C |= (0b11 << UPM00); break;
	}

	UBRR0 = F_CPU / (16 * baudrate) - 1;
}


uint8_t uart0_getc()
{
	while(!(UCSR0A & (1 << RXC0)));
	return UDR0;
}


void uart0_putc(char c)
{
	// send c to uart0
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}


void uart0_puts(char* _str)
{
	while(*_str != '\0')
	{
		uart0_putc(*_str);
		_str++;
	}
}



void uart0_registerReceptionCallback(void (*pCallback)(uint8_t data))
{
    sei();
    
    if(pCallback)
        UCSR0B |= (1 << RXCIE0); // enable reception callback

    else
        UCSR0B &= ~(1 << RXCIE0); // disable reception callback
    
    pRecepionCallback = pCallback;
}


void uart0_puts_nb(const char* pText)
{
	strcat(sendBuffer, pText);
	sei();
	
	if(sendBuffer[sendBufferIndex] != '\0')
	{
		UCSR0B |= (1 << UDRIE0); // enable data-register-empty-interrupt
	}
}


ISR(USART0_RX_vect)
{
    if(pRecepionCallback)
    {
        (*pRecepionCallback)(UDR0);
    }
}


ISR(USART0_UDRE_vect)
{
	UDR0 = sendBuffer[sendBufferIndex++];
	
	if(sendBuffer[sendBufferIndex] == '\0')
	{
		sendBufferIndex = 0;
		UCSR0B &= ~(1 << UDRIE0); // disable data-register-empty-interrupt
		sendBuffer[0] = '\0'; // clear string
	}
}
