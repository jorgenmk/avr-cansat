#include "defines.h"
#include <avr/io.h>
//#include <avr/interrupt.h>
#include "usart.h"

/*
volatile char uartBuffer[32];
volatile uint8_t uartBufferPtr=0, uartRxComplete;
*/

void USART_Init(uint16_t ubrr)
{
	UBRRH = (uint8_t)(ubrr>>8);         // Set baud rate
	UBRRL = (uint8_t)ubrr;
	UCSRB = /*(1<<RXCIE)|*/(1<<RXEN)|(1<<TXEN);   // Enable receiver and transmitter + irq
}

void USART_Transmit(uint8_t data)
{
	while ( !( UCSRA & (1<<UDRE)) ); // Wait for empty transmit buffer
	UDR = data;                      // Put data into buffer, sends the data
}

char USART_Receive(void)
{
	loop_until_bit_is_set(UCSRA, RXC); // Wait for data to be received
	return UDR;                        // Return received data from buffer
}

void USART_Flush(void)
{
	unsigned char dummy;
	while ( UCSRA & (1<<RXC) ) 
		dummy = UDR;
}

void USART_TxString(char *string)
{
	uint8_t i=0;
	while(string[i])
	{
		USART_Transmit(string[i]);
		i++;
	}
}

void USART_TxNumber(uint16_t number)
{
	USART_Transmit(number/10000+48);
	number = number%10000;
	USART_Transmit(number/1000+48);
	number = number%1000;
	USART_Transmit(number/100+48);
	number = number%100;
	USART_Transmit(number/10+48);
	USART_Transmit(number%10+48);
}


/*
ISR(USART_RXC_vect)
{
	if(uartRxComplete == 1)
		uartRxComplete = uartBufferPtr = 0;

	uartBuffer[uartBufferPtr] = UDR;

	if( (uartBuffer[uartBufferPtr] == '\r') )  // nullterminerer
		 uartBuffer[++uartBufferPtr] = 0;

	if( (uartBuffer[uartBufferPtr] == 0) )
	{
		uartBufferPtr = 0;
		uartRxComplete = 1;
	}
	else
		uartBufferPtr++;
}
*/

