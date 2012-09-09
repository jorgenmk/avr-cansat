#include <inttypes.h>
#include <avr/io.h>
#include "adc.h"

void ADC_Init(void)
{	
	// Prescaler 64, 3.6864MHz/64 = 57.6 kHz, ADC Enable, Start Convert(dummy convert)
	ADCSRA |= (6<<ADPS0)|(1<<ADEN)|(1<<ADSC); 
	loop_until_bit_is_clear(ADCSRA, ADSC);   // Wait for dummy conversion to complete
}

uint16_t ADC_Read(uint8_t channel)
{
	ADMUX = (ADMUX & 0xE0) | channel;         // Setting ADC channel
	ADCSRA |= (1<<ADSC);                      // start conversion (not needed if sleep_mode)
	loop_until_bit_is_clear(ADCSRA, ADSC);    // wait...
	uint16_t adc = ADCL;                      // ADCL has to be read before ADCH
	adc |= (ADCH<<8);
	return adc;
}

uint16_t ReadCurrent(double Rshunt)
{
	double Strom_mA;
	uint16_t adc;

	adc = ADC_Read(6);							//Reading ADC on channel 6
	Strom_mA = (((adc*165/Rshunt))/1024);		//Calculating current in mA
	return (uint16_t)Strom_mA;
}

