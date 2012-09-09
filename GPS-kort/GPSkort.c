/** @mainpage GPS-module Documentation
	@author Joakim Andre Tonnesen
	@date  23.04.2009
	
	This program reads NMEA 0183 strings from the GPS using UART, reads how much current it draws and sends the data to the main-board.
	<BR><BR>1. Read one string from the GPS.
	<BR>2. Determine wich string it is.
	<BR>3. Save corresponding data.
	<BR>4. Read how much current it draws.
	<BR>5. Send data through TWI to the mainboard on request.

	 

**/

#define F_CPU 3686400UL
//#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "usart.h"
#include "twi-slave.h"
#include "adc.h"

#define LED_DDR		DDRA
#define LED_PORT	PORTA
#define LED 		(1<<PA0)

#define GPS_SLEEP_DDR	DDRD
#define GPS_SLEEP_PORT	PORTD
#define GPS_SLEEP		(1<<PD4)

void GPS_Get_Str(char *str);
void GPS_Send_Cmd(char *cmd);

typedef struct {		char FixTime[11];
						char Lat[10];
						char LatDir[2];
						char Lon[11];
						char LonDir[2];
						char FixQuality[2];
						char SatsUsed[3];
						char Alt[11];
						char AltUnit[2];
						char Speed[10];
						char UnitN[2];
						char SpeedKPH[10];
						char UnitK[2];
						char Tcourse[6];
						char TcourseRef[2];
						char Mcourse[6];
						char McourseRef[2];
						}GPS_t;

GPS_t  GPS_Data;

//extern volatile uint8_t *twirBuffer;
//extern volatile uint8_t *twiwBuffer;
int main(void)
{
	uint16_t strom;	

	ADC_Init();
	
	strom = ReadCurrent(1.5);
	
	LED_DDR |= LED;			//Setter pinnen som LED'en er koblet til som utgang.

	GPS_SLEEP_DDR |= GPS_SLEEP;	//Setter pinnen som GPS-signalet ON/OFF er koblet til som utgang.
	GPS_SLEEP_PORT &= ~GPS_SLEEP;	//Sikrer at GPS'en ikke går i hibernate-state.

	char GPSstr[128]={0xFF};

	
	//char datum_conf[]="$PSRF106,65,*13\r\n";
	//char output_conf[]="$PSRF100,1,4800,8,1,0*0D\r\n";
	//char lla_nav_init[]="$PSRF104,63.42842,10.38901,30,0,170340,1043,20,1*1C\r\n";
	//char lla_nav_init[]="$PSRF104,63.42842,10.38901,30,0,170340,1043,20,1*2A\r\n";
	//char nav_init[]="$PSRF101,

	USART_Init(MYUBRR);   // Initialiserer USART som skal kommunisere med GPS'en
	//USART_Send_Str(lla_nav_init);
	//USART_Send_Str(output_conf);
	TW_Slave_Init(TWI_OWN_ADRESS);	//Initialiserer TWI som skal kommunisere med hovedprosessoren

	sei(); //Enable Global Interrupt
		
	for(;;)
	{
		GPS_Get_Str(GPSstr);
		TW_Write(GPS_Data.FixTime,sizeof(GPS_Data));
	}
	return 0;	
}

void GPS_Get_Str(char *str)
{
	uint8_t i;
	do 
	{
		str[0]=USART_Get_Char();
	} while(str[0]!= '$');
	
	for(i=1;str[i-1] != '\n'; i++)
	{
		str[i]=USART_Get_Char();
	}	
	str[i]=0;				
	//str[0]=i+2;			//First Byte is the string lenght

	if(str[3]=='G' && str[4]=='G' && str[5]=='A')
	{
		uint8_t i=0,j=0;
		for(; str[i] != ','; j++,i++){}		//!MsgID
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!FixTime
		{
			GPS_Data.FixTime[j]=str[i];
		}
		GPS_Data.FixTime[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Latitude
		{
			GPS_Data.Lat[j]=str[i];
		}
		GPS_Data.Lat[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Latitude Direction
		{
			GPS_Data.LatDir[j]=str[i];
		}
		GPS_Data.LatDir[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Lonitude
		{
			GPS_Data.Lon[j]=str[i];
		}
		GPS_Data.Lon[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Longitude Direction
		{
			GPS_Data.LonDir[j]=str[i];
		}
		GPS_Data.LonDir[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Fix Quality
		{
			GPS_Data.FixQuality[j]=str[i];
		}
		GPS_Data.FixQuality[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Sats Used
		{
			GPS_Data.SatsUsed[j]=str[i];
		}
		GPS_Data.SatsUsed[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++){}	//!HDOP
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Altitude
		{
			GPS_Data.Alt[j]=str[i];
		}
		GPS_Data.Alt[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Altitude Unit
		{
			GPS_Data.AltUnit[j]=str[i];
		}
		GPS_Data.AltUnit[j+1]=0;
		i++;
	}
	
	if(str[3]=='V' && str[4]=='T' && str[5]=='G')
	{
		uint8_t i=0,j=0;
		for(; str[i] != ','; j++,i++){}		//!MsgID
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!True Course
		{
			GPS_Data.Tcourse[j]=str[i];	
		}
		GPS_Data.Tcourse[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)		//!Reference T
		{
			GPS_Data.TcourseRef[j]=str[i];
		}
		GPS_Data.TcourseRef[j+1]=0;		
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Magnetic Course
		{
			GPS_Data.Mcourse[j]=str[i];	
		}
		GPS_Data.Mcourse[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)		//!Reference M
		{
			GPS_Data.McourseRef[j]=str[i];
		}
		GPS_Data.McourseRef[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Speed in knots
		{
			GPS_Data.Speed[j]=str[i];	
		}
		GPS_Data.Speed[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Unit N
		{
			GPS_Data.UnitN[j]=str[i];
		}
		GPS_Data.UnitN[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!SpeedKPH
		{
			GPS_Data.SpeedKPH[j]=str[i];	
		}
		GPS_Data.SpeedKPH[j+1]=0;
		i++;
		for(j=0; str[i] != ','; j++,i++)	//!Unit K
		{
			GPS_Data.UnitK[j]=str[i];	
		}
		GPS_Data.UnitK[j+1]=0;

	}
}





