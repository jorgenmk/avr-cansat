/*
 Title:    Cansat project sensorboard main program
 Filename: main.c
 Author:   Jorgen Moe Kvalvaag
 Date:     2009
 Software: AVR-GCC 4.3.3
 Hardware: ATmega16
*/


#include <avr/io.h>

#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <avr/wdt.h>

#include "defines.h"
#include "main.h"
#include "humid.h"
#include "../adc/adc.h"
#include "wdr.h"
#include "../twi-slave/twi-slave.h"
#include "../currentmonitor/currentmonitor.h"

int main(void){
	main_mcu_init();

	adc_init();
	humid_init();
	//wdr_init();
	TW_Slave_Init(TWI_OWN_ADDRESS);
	sei();

         int i = 0;
	data sensordata;
	data *sensordata_pt;
	sensordata_pt = &sensordata;
	wdt_enable(WDTO_500MS);
	avg_values list_average_values;
	currentmonitor_init(sensordata_pt->mah_sensorkort);
	for(;;){
		// Getting eight measurements from each sensor, storing results in list_average_values
		for(i = 0; i < 8; i++){
			list_average_values.temp_in[i] = adc2temp(adc_read(TEMP_IN), TEMP_RESISTOR_IN, shk_in);
			list_average_values.temp_out[i] = adc2temp(adc_read(TEMP_OUT), TEMP_RESISTOR_OUT, shk_out);
			list_average_values.pressure[i] = adc2pressure(adc_read(PRESSURE));
			list_average_values.acceleration[i] = adc2acceleration(adc_read(ACCEL));
		}

		//Humidity sensor separatly, to avoid ADC shutdown/startup delay
		for (i = 0; i < 8;i++){
			list_average_values.humidity[i] = tau2humidity(humid_read(ADC6, ADC7));
		}

		double temp = 0.0;
		// Calculating average value from last eight samples, for each sensor

		for(i = 0; i < 8; i++){
			temp += list_average_values.temp_in[i];
		}
		double2string(temp/8.0, sensordata_pt->temp_string_in);

		temp = 0;
		wdt_reset();
		for(i = 0; i < 8; i++){
			temp += list_average_values.temp_out[i];
		}
		double2string(temp/8.0, sensordata_pt->temp_string_out);

		temp = 0;

		for(i = 0; i < 8; i++){
			temp += list_average_values.pressure[i];
		}
		double2string(temp/8.0, sensordata_pt->pressure_string);

		temp = 0;


		for(i = 0; i < 8; i++){
			temp += list_average_values.acceleration[i];
		}
		double2string(temp/8.0, sensordata_pt->accel_string);

		temp = 0;

		for(i = 0; i < 8; i++){
			temp += list_average_values.humidity[i];
		}

		double2string(temp/8.0, sensordata_pt->humidity_string);
		TW_Write((uint8_t*)sensordata_pt, sizeof(sensordata));

		wdt_reset();

		LED_OFF;
	}
}

void double2string(double value, unsigned char* string){
	int temp;

	// Catching errors
	if (value <= -99.9){
		value = -55.5;
	}
	if (value >= 99.9){
		string[0] = '1';
		value -= 100;
	}
	else {
		string[0] = ' ';
	}
	// Prepare the string
	string[3] = '.';
	string[5] = '\0';

	if(value < 0){
		value *= -1;
		string[0] = '-';
	}

	temp = (int16_t)(value * 10);

//	if((value*10 - temp) >= 0.5){
//		value += 0.5;
//	}

	string[4] = ((uint8_t)(temp%10) | 0x30);
	temp /= 10;

	string[2] = ((uint8_t)(temp%10) | 0x30);
	temp /= 10;

	string[1] = ((uint8_t)(temp%10) | 0x30);
	temp /= 10;
}

double adc2temp(double Vadc, double R1, double *shk){
	/*
	if(Vadc == 1024){
	return -1;
}
	*/
	// Calculate resistance across the thermistor
	double Rt = (R1 * (Vadc/1024))/(1.0-(Vadc/1024));

	// Calculate and return the temperature in celsius
	return 1.0/(shk[0] + shk[1]*log(Rt) + shk[2]*pow(log(Rt),2) + shk[3]*pow(log(Rt),3)) - 273.15;
	//return Vadc/100.0;
}

double tau2humidity(uint16_t tau){
	if (tau == 0xFFFF){
		return 99.9;
	}
	double C = (((double)tau * TAU_FACTOR * (1.0/F_CPU))/RESISTOR_HUMIDITY)* pow(10, 12);
	return (C - 300.0) * 1.66667; // Converts 16bit tau value to humidity. See explanation in datasheet
}

double adc2pressure(uint16_t value){
	double Vout = PRESSURE_SCALE_FACTOR * (3.29/1024.0) * value;
	return (Vout + 0.475)/0.045; // Converts 16 bit adc value to pressure. See explanation in datasheet
}

double adc2acceleration(uint16_t value){
	double Vout = ACCEL_SCALE_FACTOR * (3.29/1024.0) * value;
	return (Vout/0.020)-125; // Converts 16 bit adc value to acceleration force. See datasheet for explanation
}

void main_mcu_init(void){
	DDRD = (1 << PD3);		// LED pin output
	// Setting internal pull-ups for all unused pins
	DDRA &= ~(1 << PA1);
	PORTA |= (1 << PA1);
	DDRB &= ~(1 << PB3) & ~(1 << PB1) & ~(1 << PB0);
	PORTB |= (1 << PB3) | (1 << PB1) | (1 << PB0);
	DDRC &= ~(1 << PC2) & ~(1 << PC3) & ~(1 << PC4) & ~(1 << PC5) & ~(1 << PC6) & ~(1 << PC7);
	PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5) | (1 << PC6) | (1 << PC7);
	DDRD &= ~(1 << PD0) & ~(1 << PD1) &~(1 << PD2) &~(1 << PD4) &~(1 << PD5) &~(1 << PD6) &~(1 << PD7);
	PORTD |= (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
}


