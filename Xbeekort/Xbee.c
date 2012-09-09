#include "defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"
#include "Xbee.h"

volatile uint8_t blinkActive = 0;


void XbeeInit(void)
{
	USART_Init(MYUBRR);
	LedBlinkInit();

	XbeeEnterCmdMode();
	XbeeTxString("ATRE\r");   // restore to factory defaults
	XbeeWaitOK();
	XbeeTxString("ATPL 4\r"); // power level to 0
	XbeeWaitOK();
#ifdef FLOW_CONTROL
	XbeeTxString("ATD7 1\r"); // set up hardware CTS flow control
	XbeeWaitOK();
#endif
	XbeeTxString("ATAC\r");   // apply changes
	XbeeWaitOK();
	XbeeTxString("ATWR\r");   // save changes
	XbeeWaitOK();
	XbeeTxString("ATCN\r");   // exit command mode
	XbeeWaitOK();
}


void XbeeTxByte(char byte)
{
	BlinkLed();
#ifdef FLOW_CONTROL
	loop_until_bit_is_clear(UART_PIN, CTS); // wait if buffer full
#endif
	USART_Transmit(byte);
}


void XbeeTxString(char *string)
{
	while(*string)
		XbeeTxByte(*string++);
}


void XbeeEnterCmdMode(void)
{
	// Default AT Command Mode Sequence:
	// No characters sent for one second [GT (Guard Times) parameter = 0x3E8]
	_delay_ms(1100);
	// Input three plus characters ("+++") within one second [CC (Command Sequence Character) parameter = 0x2B.]
	XbeeTxString("+++");
	XbeeWaitOK();
}


uint8_t XbeeWaitOK(void)
{
	if( USART_Receive() == 'O' )
		if( USART_Receive() == 'K' )
			if( USART_Receive() == '\r' )
			{
				_delay_ms(10);    // need mysterious delay to function properly
				return 0;
			}
	_delay_ms(10);
	USART_Flush();
	return 1;
}


void LedBlinkInit(void)
{
	DDRD |= (1<<LED);
	OCR1A = 0x30;   // these values defines the led on/off ration
	OCR1B = 0x60;
	TCCR1B |= (5<<CS10);     // prescaler 1024
}


void BlinkLed(void)
{
	if( !blinkActive && bit_is_clear(UART_PIN, CTS) )
	{
		blinkActive = 1;
		PORTD |= (1<<LED);      // enable led
		TCNT1 = 0;              // reset blink timer
		TIMSK |= (1<<OCIE1A)|(1<<OCIE1B);    // irq on compare match
	}
}


ISR(TIMER1_COMPA_vect)
{
	PORTD &= ~(1<<LED);    // disable led
}


ISR(TIMER1_COMPB_vect)
{
	blinkActive = 0;
	TIMSK &= ~((1<<OCIE1A)|(1<<OCIE1B)); // disable new interrupts
}
