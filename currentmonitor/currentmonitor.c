#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>
#include "defines.h"
#include "currentmonitor.h"
#include "../adc/adc.h"



uint16_t counter = 0;

char* mah_used;

void currentmonitor_init(char* mah_pt){
	mah_used = mah_pt;
	TIMSK |= (1 << TOIE0);		// Enable Overflow Interrupt for TC0
	TCCR0 |= (1 << CS02)|(1 << CS00);	//Start TC0 with prescaler = 1024

}

ISR(TIMER0_OVF_vect){
	if(counter++ == ONE_SECOND){
		//mah_used += U_shunt/R_shunt * 1/3600
		mah_used += (((adc_read(INA_PIN)/1024)*(3.3/20)/RSHUNT)/3600);
		counter = 0;
		while(!ERROR){
			LED_ON;
		}
		LED_ON;
	}


}