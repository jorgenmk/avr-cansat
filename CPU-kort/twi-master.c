#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "defines.h"
//#define DEBUG_ON
#include "twi-master.h"
#ifdef DEBUG_ON
	#include "usart.h"
#endif

#ifdef ASCII_MODE
	volatile uint8_t twiAdrRW, retryCnt, twiStatus;
	volatile int16_t twiBytes;
	char *twiData;
#else
	volatile uint8_t twiAdrRW, *twiData, twiBytes, retryCnt, twiStatus;
#endif


void TWI_Init(void)
{
	DDRD |= (1<<PD1) | (1<<PD0);
	PORTD |= (1<<PD1) | (1<<PD0);       // enable internal pull-ups
	TWSR = 0;                           // status bits defaults to 1
	TWBR = MY_TWBR;                     // set baud rate
	TWCR |= (1<<TWEN);                  // enable TWI module
}


#ifdef ASCII_MODE
	uint8_t TWI_Write(uint8_t address,  char *data, uint8_t bytes)
#else
	uint8_t TWI_Write(uint8_t address,  uint8_t *data, uint8_t bytes)
#endif
{
	loop_until_bit_is_clear(twiStatus, BUSY); // Bus is busy, wait
	//USART_TxString("\nBusy flag cleared");
	twiAdrRW = (address<<1) + TW_WRITE;       // set address + data direction
	twiData = data;                           // pointer to data buffer
	twiBytes = bytes;                         // bytes in message
	retryCnt = 0;

	twiStatus |= (1<<BUSY);                   // entering twi busy state

	// generate start condition, enable twi irq
	TWCR = START_CONDITION | (1<<TWIE);

	return 0;
}


#ifdef ASCII_MODE
	uint8_t TWI_Read(uint8_t address,  char *data, uint8_t bytes)
#else
	uint8_t TWI_Read(uint8_t address,  uint8_t *data, uint8_t bytes)
#endif
{
	loop_until_bit_is_clear(twiStatus, BUSY); // Bus is busy, wait
	//USART_TxString("\nBusy flag cleared");
	twiAdrRW = (address<<1) + TW_READ;        // set address + data direction
	twiData = data;                           // pointer to data buffer
	twiBytes = bytes;                         // bytes in message
	retryCnt = 0;

	twiStatus |= (1<<BUSY);                   // entering twi busy state
	// generate start condition, enable twi irq
	TWCR = (1<<TWEN)|(1<<TWSTA) | (1<<TWIE);

	return 0;
}


ISR(TWI_vect)					// TWI Event has happened
{
	#ifdef DEBUG_ON
		//USART_TxString("ISR Entered, TW_STATUS: ");
		//USART_TxNumber(TW_STATUS);
		//USART_TxString("\n");
	#endif
	switch(TW_STATUS)				// check TWI-status
	{
		case TW_START:			// 0x08 - Start condition or
			TWDR = twiAdrRW;		// Transmit SLA + Read or Write
			TWCR = CLEAR_TWINT;
			break;
		case TW_REP_START:		// 0x10 - Repeated start condition

			if(retryCnt > 3)		// If 3 times NACK, abort
			{
				//twiStatus = (twiStatus & ~(1<<BUSY)) | (1<<ERROR); // Bus no longer busy
				#ifdef DEBUG_ON
					USART_TxString("\nretryCnt>3, Clear busy, set error, twiStatus: ");
					USART_TxNumber(twiStatus);
					USART_TxString(" address: ");;
					USART_TxNumber(twiAdrRW>>1);
				#endif
				TWCR = STOP_CONDITION;	// Generate stop condition
				twiStatus &= ~(1<<BUSY);	// Bus no longer busy
				break;
			}
			TWDR = twiAdrRW;			// Transmit SLA + Read or Write
			TWCR = CLEAR_TWINT;
			#ifdef DEBUG_ON
				USART_TxString("\nRepeated start, twiStatus: ");
				USART_TxNumber(twiStatus);
				USART_TxString(" address: ");;
				USART_TxNumber(twiAdrRW>>1);
			#endif
			break;

		case TW_MT_SLA_ACK:			// 0x18 - Master Tx, Slave acknowledged address
			#ifdef DEBUG_ON
				USART_TxString("\nTWI MT Slave ACK address, twiStatus: ");
				USART_TxNumber(twiStatus);
				USART_TxString(" address: ");
				USART_TxNumber(twiAdrRW>>1);
			#endif
			retryCnt = 0;			// so clear retry counter
			TWDR = twiBytes;  //*twiData++;	// First data = number of bytes to send
			TWCR = CLEAR_TWINT;		// and clear TWINT to continue
			break;

		case TW_MT_SLA_NACK:			// 0x20 - Master Tx, slave didn't acknowledge address, or
			#ifdef DEBUG_ON
				USART_TxString("\nTWI MT Slave NACK address, twiStatus: ");
				USART_TxNumber(twiStatus);
				USART_TxString(" address: ");
				USART_TxNumber(twiAdrRW>>1);
			#endif
			retryCnt++;			// this may mean that the slave is disconnected
			TWCR = START_CONDITION;		// Repeated Start condition
			break;
		case TW_MR_SLA_NACK:			// 0x48 - Master Rx, slave didn't acknowledge address,
			#ifdef DEBUG_ON
				USART_TxString("\nTWI MR Slave NACK address, twiStatus: ");
				USART_TxNumber(twiStatus);
				USART_TxString(" address: ");
				USART_TxNumber(twiAdrRW>>1);
			#endif
			retryCnt++;			// this may mean that the slave is disconnected
			TWCR = START_CONDITION;		// Repeated Start condition
			break;

		case TW_MT_DATA_ACK:			// 0x28 - Master Tx, slave acknowledged data,
			if(--twiBytes >= 0)		// If there is more data to send,
			{
				#ifdef DEBUG_ON
					USART_TxString("\nTWI Slave ACK data, twiBytes: ");
					USART_TxNumber(twiBytes);
					USART_TxString(" address: ");
					USART_TxNumber(twiAdrRW>>1);
					USART_TxString(" data: ");
					USART_Transmit(*twiData);
				#endif
				TWDR = *twiData++;	// Send it, increment pointer
				TWCR = CLEAR_TWINT;
			}
			else
			{
				#ifdef DEBUG_ON
					USART_TxString("\nTWI Slave ACK LAST data, twiStatus: ");
					USART_TxNumber(twiStatus);
					USART_TxString(" address: ");
					USART_TxNumber(twiAdrRW>>1);
				#endif
				TWCR = STOP_CONDITION & ~(1<<TWIE); // Generate stop condition, disable twi irq
				twiStatus &= ~(1<<BUSY);	// Bus no longer busy
			}
			break;

		case TW_MT_DATA_NACK:			// 0x30 - Master Tx, Slave didn't acknowledge data
			#ifdef DEBUG_ON
				USART_TxString("\nTWI Slave NACK data, twiStatus: ");
				USART_TxNumber(twiStatus);
				USART_TxString(" address: ");
				USART_TxNumber(twiAdrRW>>1);
			#endif
			TWCR = STOP_CONDITION & ~(1<<TWIE); // Send stop condition, disable twi irq
			twiStatus = (twiStatus & ~(1<<BUSY)) | (1<<ERROR); // Bus no longer busy, error
			break;

		case TW_MT_ARB_LOST:			// 0x38 - Single master this can't be!!!
			#ifdef DEBUG_ON
				USART_TxString("\nTWI MT arb lost, twiStatus: ");
				USART_TxNumber(twiStatus);
				USART_TxString(" address: ");;
				USART_TxNumber(twiAdrRW>>1);
			#endif
			TWCR = STOP_CONDITION & ~(1<<TWIE);	// Send stop condition, disable twi irq
			twiStatus = (twiStatus & ~(1<<BUSY)) | (1<<ERROR); // Bus no longer busy, error
			break;

		case TW_MR_SLA_ACK:			// 0x40 - Slave acknowledged address
			#ifdef DEBUG_ON
				USART_TxString("\nTWI MR Slave ACK address, twiStatus: ");
				USART_TxNumber(twiStatus);
				USART_TxString(" address: ");;
				USART_TxNumber(twiAdrRW>>1);
			#endif
			if(--twiBytes > 0)
				TWCR = CLEAR_TWINT | (1<<TWEA); // If more than one byte to read, acknowledge
			else
				TWCR = CLEAR_TWINT & ~(1<<TWEA); // else do not acknowledge
			break;

		case TW_MR_DATA_ACK:			// 0x50 - Master acknowledged data
			*twiData++ = TWDR;		// Store received data byte, increment pointer
			if(--twiBytes > 0)		// More data to read?
				TWCR = CLEAR_TWINT;
			else
				TWCR = CLEAR_TWINT & ~(1<<TWEA); // Don't acknowledge last byte received
			break;

		case TW_MR_DATA_NACK:			// 0x58 - Master didn't acknowledge data -> end of read process
			#ifdef DEBUG_ON
				USART_TxString("\nTWI MR data NACK(end of read), twiStatus: ");
				USART_TxNumber(twiStatus);
				USART_TxString(" address: ");;
				USART_TxNumber(twiAdrRW>>1);
			#endif
			*twiData = TWDR;                // Read received data byte
			TWCR = STOP_CONDITION;          // Generate stop condition
			twiStatus &= ~(1<<BUSY);        // Bus no longer busy
			break;
	}
}
