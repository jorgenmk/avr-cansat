#include "defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "twi-master.h"
#include "main.h"
#include "../adc/adc.h"

//#define DEBUG
#include "usart.h"

int main(void){
	USART_Init(MYUBRR);
	TWI_Init();
	sei();
	DDRC |= SENSOR_ON | GPS_ON | RADIO_ON;
	PORTC |= SENSOR_ON | GPS_ON | RADIO_ON;

	/*sensor_data sensordata;
	sensor_data *sensordata_pt;
	sensordata_pt = &sensordata;

	gps_data gpsdata;
	gps_data *data_pt;
	data_pt = &gpsdata;
	*/

	all_data data;
	all_data *data_pt;
	data_pt = &data;

	char* sensor_pt;
	sensor_pt = &data.temp_string_in;

	double mah_remaining = 860;

	char* gps_pt;
	gps_pt = &data.FixTime;

	#ifdef DEBUG
		USART_TxString("\n\rPower On!\n");
		LED_DDR |= LED_G | LED_R;
		uint8_t lmsensor[2];
		lmsensor[0] = 0xFE;
		lmsensor[1] = 0xFE;
		uint8_t text = 1;

	#endif
	_delay_ms(3000);
	for(;;){

		//TWI_Read(LM_SENSOR, (char*)lmsensor, sizeof(lmsensor));
		//USART_TxString("\nLM lest");
		TWI_Read(SENSOR_CARD, (char*)sensor_pt, 30);
		//USART_TxString("\nSensorkort lest");
		TWI_Read(GPS_CARD, (char*)gps_pt, 94);
		//USART_TxString("\nGPS lest");
		TWI_Write(RADIO_CARD, (char*)data_pt, sizeof(data));
		//USART_TxString("\nRadio sendt");
		#ifdef DEBUG
			R_SWITCH;
			//USART_TxNumber(sizeof(data));
			if(text == 1){
				USART_TxString("\n\nPrinting data:");
				USART_TxString("\nFixTime,");
				USART_TxString("Latitude,");
				USART_TxString("Latitude Direction,");
				USART_TxString("Longditude,");
				USART_TxString("Longditude Direction,");
				USART_TxString("Fix Quality,");
				USART_TxString("Satelittes used,");
				USART_TxString("Altitude,");
				USART_TxString("Altitude Unit,");
				USART_TxString("Speed (Knots),");
				USART_TxString("Speed Unit (N=Knots),");
				USART_TxString("Speed (KPH),");
				USART_TxString("Speed Unit (K=KPH),");
				USART_TxString("True Course (deg),");
				USART_TxString("Magnetic Course (deg),");
				USART_TxString("Temp(in),");
				USART_TxString("Temp(out),");
				USART_TxString("RH,");
				USART_TxString("kPa,");
				USART_TxString("Aks\n\n");
				text = 0;
			}

			USART_TxString(data_pt->FixTime);
			USART_Transmit(',');
			USART_TxString(data_pt->Lat);
			USART_Transmit(',');
			USART_TxString(data_pt->LatDir);
			USART_Transmit(',');
			USART_TxString(data_pt->Lon);
			USART_Transmit(',');
			USART_TxString(data_pt->LonDir);
			USART_Transmit(',');
			USART_TxString(data_pt->FixQuality);
			USART_Transmit(',');
			USART_TxString(data_pt->SatsUsed);
			USART_Transmit(',');
			USART_TxString(data_pt->Alt);
			USART_Transmit(',');
			USART_TxString(data_pt->AltUnit);
			USART_Transmit(',');
			USART_TxString(data_pt->Speed);
			USART_Transmit(',');
			USART_TxString(data_pt->UnitN);
			USART_Transmit(',');
			USART_TxString(data_pt->SpeedKPH);
			USART_Transmit(',');
			USART_TxString(data_pt->UnitK);
			USART_Transmit(',');
			USART_TxString(data_pt->Tcourse);
			USART_Transmit(',');
			USART_TxString(data_pt->TcourseRef);
			USART_Transmit(',');
			USART_TxString(data_pt->Mcourse);
			USART_Transmit(',');
			USART_TxString(data_pt->McourseRef);
			USART_Transmit(',');
			USART_TxString(data_pt->temp_string_in);
			USART_Transmit(',');
			USART_TxString(data_pt->temp_string_out);
			USART_Transmit(',');
			USART_TxString(data_pt->humidity_string);
			USART_Transmit(',');
			USART_TxString(data_pt->pressure_string);
			USART_Transmit(',');
			USART_TxString(data_pt->accel_string);
			USART_Transmit(',');
			USART_TxNumber(lm2temp(lmsensor));
			USART_TxString("\n");
		#endif
		_delay_ms(1000);

		/*
		USART_TxString("\n\nVirkelig Temperatur: ");
		uint16_t temp = ((lmsensor[0] << 8) | lmsensor[1]);
		USART_TxNumber((uint16_t)temp);

		USART_TxString("lmsensor[0]: ");
		USART_TxNumber((uint16_t)lmsensor[0]);
		USART_TxString("lmsensor[1]: ");
		USART_TxNumber((uint16_t)lmsensor[1]);
		*/

		//mah_remaining -= ReadCurrent(RSHUNT, PIN_INA);	// * time_since_last_subtract


	}

	return 0;
}

uint16_t lm2temp(uint8_t *value){
	uint16_t temp = (*value++ << 8);
	temp |= *value;
	return ((uint16_t)(temp * 0.0625));
}
