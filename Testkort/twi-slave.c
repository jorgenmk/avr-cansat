/***********************************************************************
  twi-slave.c
************************************************************************/

#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "twi-slave.h"

//extern GPS_t gpsData;
//volatile uint8_t *twirBuffer;
//volatile uint8_t *twiwBuffer;
volatile uint8_t twiBuffer[128];
volatile uint8_t twiReadFinish=0;

volatile uint8_t twiData;
volatile uint8_t twirBytes;
volatile uint8_t twiwBytes;
volatile uint8_t retryCnt;
//volatile uint8_t firstRun;


/***********************************************************************
  Initialize TWI-interface, set slave address
************************************************************************/
void TW_Slave_Init(uint8_t twSlaveAddress)
{
	TWAR = twSlaveAddress << 1;         // Set own TWI slave address
	TWAR |= (1<<TWGCE);					// Respond to General Calls.
	TWCR = (1<<TWEN)|(1<<TWIE);         // Enable TWI interface, enable interrupt
									 	// Not ACK, yet
	//TWCR |= (1<<TWEA);
}


/***********************************************************************
  Receive data from TWI-interface
************************************************************************/
void TW_Read(uint8_t *msg, uint8_t msgSize)
{
	while(TWI_BUSY);            		// bus is busy (or exit with error)
	//twirBuffer = data;
	twirBytes = msgSize;
	//firstRun = 0;

	TWCR |= (1<<TWEA)|(1<<TWINT);

	while(twiReadFinish==0);

	for(uint8_t i=0; i<msgSize; i++)
	{
		msg[i]=twiBuffer[i];
	}
	twiReadFinish = 0;


}


/***********************************************************************
  Function to send a prepared message via TWI-interface
************************************************************************/
void TW_Write(uint8_t *msg, uint8_t msgSize)
{
	/*if(TWI_BUSY)
		PORTA |= (1<<PA0);*/
	while(TWI_BUSY);            		// bus is busy (or exit with error)
	for(uint8_t i=0; i<(msgSize); i++)
		twiBuffer[i] = msg[i];
	//twiBuffer[0] = msgSize;
	//twiwBuffer = msg;
	twiwBytes = msgSize;
	retryCnt = 0;
	TWCR |= (1<<TWEA)|(1<<TWINT);
}


/*****************************************************
 See table 76 & 77 in ATmega32 datasheet for reference
******************************************************/ 
ISR(TWI_vect)
{
	//static uint8_t twirBufPtr;
	static uint8_t twiBufPtr;

	switch (TW_STATUS)
	{
		/*************** SLAVE RECEIVER MODES ******************************/

		case TW_SR_GCALL_ACK: // $70 - General call, ready to get data
			// Set flag GCALL
			//	twi.genCall = TRUE;

		case TW_SR_SLA_ACK: // $60 - Normal call, ready to get data
			twiBufPtr = 0;
			break;

		case TW_SR_GCALL_DATA_ACK: // $90 - General call, data received
			// Set flag GCALL

		case TW_SR_DATA_ACK: // $80 - Normal call, data received, ACK sent
			/*if(firstRun)
			{
				twirBytes = TWDR;
				firstRun = 0;
			}
			else
			{	
				twirBuffer[twirBufPtr++] = TWDR;
			}*/
			twiBuffer[twiBufPtr++] = TWDR;
			if(--twirBytes == 0)
			{
				twiReadFinish=1;
			}
			break;

		case TW_SR_GCALL_DATA_NACK: // $98 - General call, data received, NACK sent
			
		case TW_SR_DATA_NACK: // $88 - Normal call, data received, NACK sent
			// Set flag Tx Error, Return error type
			TWCR |= (1<<TWEA);
			break;

		case TW_SR_ARB_LOST_SLA_ACK: // $68 -  Single Master, never happens!

		case TW_SR_ARB_LOST_GCALL_ACK: // $78 -  Single Master, never happens!
			break;

		case TW_SR_STOP: // $A0 - Stop or repeated start condition received
			break;

		/*************** SLAVE TRANSMITTER MODES *********************************/

		case TW_ST_SLA_ACK: // $A8 - Master wants data, ACK received
			twiBufPtr = 0;          // start transmission by sending first byte
			TWDR = twiBuffer[twiBufPtr++];

			break;

		case TW_ST_DATA_ACK: // $B8 - Data sent, master wants more, ACK received
			TWDR = twiBuffer[twiBufPtr++];
			break;

		case TW_ST_DATA_NACK: // $C0 - Data sent, NACK received
			if (twiBufPtr == twiwBytes)    // Transferred all of the expected data?
			{
				// Set flag last Tx OK
			}
			else                          // Master has sent a NACK before all data were sent.
			{
				// Set flag Tx Error, Return error type
			}
			break;

		case TW_ST_LAST_DATA: // $C8 - Master asks for more data than expected
			// Send 0
			// Set flag Tx Error, Return error type
			break;

		case TW_ST_ARB_LOST_SLA_ACK: // $B0 -  Single Master, never happens!
			break;

	}

	TWCR |= (1<<TWINT);                             // clear interrupt flag

}



