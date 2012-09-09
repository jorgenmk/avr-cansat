/*******************************************************
 *                USART.c av jkm-                      *
 *******************************************************/

#include <avr/io.h>

void USART_Init(unsigned int ubrr)
{
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void USART_Send_Char(unsigned char data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char USART_Get_Char(void)
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR;
}

void USART_TxString(unsigned char *str)
{
	while(*str)
	{
		USART_Send_Char(*str++);
	}


}

void USART_Get_Str(unsigned char *str)
{
	*str = 0;
	while(*str != '\n');
	{
			*str = USART_Get_Char();
			str++;
	}
}
