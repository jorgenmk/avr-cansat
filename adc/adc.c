#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

#include "adc.h"

void adc_init(void){
	ADMUX  |= (1 << REFS0); 			// AREF cap + AVCC connected to Vcc(3.3V)
	ADCSRA |= (1 << ADPS2)|
		(1 << ADPS1)|
		(1 << ADPS0);			// Prescaler CLK/128 (8MHz/64 = 62.5 kHz ADC)
//		(1 << ADIE);			// ADC Interrupt Enable
//	MCUCR  |= (1 << SM0);			// Enable ADC noise reduction sleep mode
	ADCSRA |= (1 << ADEN);			// ADC Enable
	ADCSRA |= (1 << ADSC);			// ADC Start Convert(dummy init)
	while (ADCSRA & (1 << ADSC));		// Wait for ADC conversion to complete
}

uint16_t adc_read(uint8_t channel){
	ADMUX &= 0xE0;
	ADMUX |= channel;				// Setting ADC channel
	ADCSRA |= (1 << ADEN);			// ADC Enable
	ADCSRA |= (1 << ADSC);			// ADC Start Convert
	while (ADCSRA & (1 << ADSC));		// Wait for ADC conversion to complete
	return (ADCL|(ADCH << 8));			// Return 10bit result
}

uint16_t ReadCurrent(double Rshunt, uint8_t pin_ina){

	return (uint16_t)((((double)adc_read(pin_ina)*165.0/Rshunt))/1024.0); //Returns the current used by the entire card in mA
}