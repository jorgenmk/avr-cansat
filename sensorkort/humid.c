#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "humid.h"

volatile uint8_t run = 0;
volatile uint16_t tau_half;
volatile uint8_t adc_mplex2;

void humid_init(void){
	DDRB |= (1 << PB4);		// Port B pin 4 output, used to apply voltage to the RC-network
	ACSR |= (1 << ACIC);		// Analog Comparator Input Capture on TimerCounter1
	TCCR1B |= (1 << ICES1);		// Comparator trigger on rising edge
}

uint16_t humid_read(uint8_t adc_multiplex_1, uint8_t adc_multiplex_2){
	adc_mplex2 = adc_multiplex_2;	// Store second multiplex input
	ADCSRA &= ~(1 << ADEN);		// ADC Disable
	ADMUX &= 0xF8;
	ADMUX |= adc_multiplex_1;		// Setting adc_multiplex_1 as negative input on the comparator
	TCNT1 = 0;			// Reset TimerCounter1
	SFIOR |= (1 << ACME);		// Analog Comparator Multiplex Enable, negative input to the comparator is connected to ADC
	TIMSK |= (1 << TICIE1); 		// Timer1 Input Capture Interrupt Enable
	run = 0;
	TCCR1B |= (1 << CS10);		// Start TimerCounter1 with no prescaler
	PORTB |= (1 << PB4);		// Apply voltage to RC-network
	_delay_us(500);
	PORTB &= ~(1 << PB4);		// Ground RC-network after delay, in case something went wrong
	TIMSK &= ~(1 << TICIE1); 		// Timer1 Input Capture Interrupt disable
	SFIOR &= ~(1 << ACME);		// Analog Comparator Multiplex disabled
	if (run == 2){
		return (2*(ICR1 - tau_half));
	}
	else return 0xFFFF;
}

ISR(TIMER1_CAPT_vect){
	switch (run){
		case 0:
		tau_half = ICR1;		// Store tau/2
		ADMUX &= 0xF8;
		ADMUX |= adc_mplex2;		// Setting adc_mplex_2 as negative input on the comparator
		run++;
		break;

		case 1:
		TCCR1B &= ~(1 << CS10);		// Stop Counter1
		PORTB &= ~(1 << PB4);		// Remove voltage from RC-network
		run++;
		break;
	}
}

