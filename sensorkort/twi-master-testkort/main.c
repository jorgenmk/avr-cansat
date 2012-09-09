#include "defines.h"
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"
#include "twi-master.h"

//#define DEBUG

typedef struct {
	uint8_t temp_string_in[6];		//!< Contains a string representing the temperature inside the cansat
	uint8_t temp_string_out[6];	//!< Contains a string representing the temperature outside the cansat
	uint8_t humidity_string[6];	//!< Contains a string representing the humidity level outside the cansat
	uint8_t pressure_string[6];	//!< Contains a string representing the pressure outside the cansat
	uint8_t accel_string[6];		//!< Contains a string representing the acceleration forces applied to cansat
} GGA_t;

GGA_t GGA_Data;

int main(void)
{
	//ADC_Init();
	USART_Init(BAUD);
	USART_TxString("\n\rPower On!");

	TWI_Init();

	sei();
	_delay_ms(100);
	while(1)
	{
		//TWI_Write(GPS_CARD, sendStr, sizeof(sendStr));
		USART_TxString("\nReading...");
		TWI_Read(SENSOR_CARD, &GGA_Data, sizeof(GGA_Data));
		//TWI_Read(GPS_CARD, tempGPS, sizeof(tempGPS));

		//while(status & (1<<BUSY));
		USART_TxString("\ntemp_string_in: ");
		USART_TxString(GGA_Data.temp_string_in);
		USART_TxString("\ntemp_string_out: ");
		USART_TxString(GGA_Data.temp_string_out);
		USART_TxString("\nhumidity_string: ");
		USART_TxString(GGA_Data.humidity_string);
		USART_TxString("\npressure_string: ");
		USART_TxString(GGA_Data.pressure_string);
		USART_TxString("\naccel_string: ");
		USART_TxString(GGA_Data.accel_string);
		USART_TxString("\n");
		//USART_TxString(tempGPS);
/*		TWI_Read(0x02, sensorData);
		while(status & (1<<BUSY));
		USART_TxString(sensorData); */
		USART_TxString("\n\rWaiting...\n\r");
		_delay_ms(2000);
	}
}
