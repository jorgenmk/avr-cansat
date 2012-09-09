#include "defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "usart.h"
#include "Xbee.h"
#include "twi-slave.h"
#include "main.h"


int main(void)
{
	TW_Slave_Init(TWI_OWN_ADRESS);
	//wdt_enable(WDTO_2S);   // XbeeInit() requires 1s delay
	XbeeInit();   // needs interrupts to blink led
	//wdt_reset();
	//wdt_enable(WDTO_500MS);

	XbeeTxString("\nOnline");



	data xbeedata;
	data* xbee_pt;
	xbee_pt = &xbeedata;

	sei();
	XbeeTxString("\n\nPrinting data:");
	for(;;)
	{
		TW_Read((char*)xbee_pt);
		/*
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
*/
		USART_TxString(xbee_pt->FixTime);
		USART_Transmit(',');
		USART_TxString(xbee_pt->Lat);
		USART_Transmit(',');
		USART_TxString(xbee_pt->LatDir);
		USART_Transmit(',');
		USART_TxString(xbee_pt->Lon);
		USART_Transmit(',');
		USART_TxString(xbee_pt->LonDir);
		USART_Transmit(',');
		USART_TxString(xbee_pt->FixQuality);
		USART_Transmit(',');
		USART_TxString(xbee_pt->SatsUsed);
		USART_Transmit(',');
		USART_TxString(xbee_pt->Alt);
		USART_Transmit(',');
		USART_TxString(xbee_pt->AltUnit);
		USART_Transmit(',');
		USART_TxString(xbee_pt->Speed);
		USART_Transmit(',');
		USART_TxString(xbee_pt->UnitN);
		USART_Transmit(',');
		USART_TxString(xbee_pt->SpeedKPH);
		USART_Transmit(',');
		USART_TxString(xbee_pt->UnitK);
		USART_Transmit(',');
		USART_TxString(xbee_pt->Tcourse);
		USART_Transmit(',');
		USART_TxString(xbee_pt->TcourseRef);
		USART_Transmit(',');
		USART_TxString(xbee_pt->Mcourse);
		USART_Transmit(',');
		USART_TxString(xbee_pt->McourseRef);
		USART_Transmit(',');
		USART_TxString(xbee_pt->temp_string_in);
		USART_Transmit(',');
		USART_TxString(xbee_pt->temp_string_out);
		USART_Transmit(',');
		USART_TxString(xbee_pt->humidity_string);
		USART_Transmit(',');
		USART_TxString(xbee_pt->pressure_string);
		USART_Transmit(',');
		USART_TxString(xbee_pt->accel_string);
		USART_TxString("\n");

	}


	return 0;
}




