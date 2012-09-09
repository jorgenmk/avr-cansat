#include "defines.h"
#include <avr/io.h>
#include "usart.h"

void USART_Init(uint16_t ubrr)
{
	UBRR0H = (uint8_t)(ubrr>>8);         // Set baud rate
	UBRR0L = (uint8_t)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);       // Enable receiver and transmitter
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	  // 8-bit 2560
}

void USART_Transmit(uint8_t data)
{
	while ( !( UCSR0A & (1<<UDRE0)) );   // Wait for empty transmit buffer
	UDR0 = data;                         // Put data into buffer, sends the data
}

uint8_t USART_Receive(void)
{
	while ( !(UCSR0A & (1<<RXC0)) );      // Wait for data to be received
	return UDR0;                         // Get and return received data from buffer
}


void USART_TxString(char *str){
	while(*str){
		USART_Transmit(*str++);
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

void USART_TxDouble(double number){
	number *= 10;
	char temp[] = "xxx.x\0";
	temp[0] = (((uint8_t)number%10) | 0x30);
	number /= 10;
	temp[1] = (((uint8_t)number%10) | 0x30);
	number /= 10;
	temp[2] = (((uint8_t)number%10) | 0x30);
	number /= 10;
	temp[4] = (((uint8_t)number%10) | 0x30);
	USART_TxString(temp);
}