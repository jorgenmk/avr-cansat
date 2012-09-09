#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "twi-master.h"
#ifdef DEBUG_ON
	#include "usart.h"
#endif

#ifdef ASCII_MODE
	volatile uint8_t twiAdrRW, twiBytes, retryCnt, twiStatus;
	char *twiData;
#else
	volatile uint8_t twiAdrRW, *twiData, twiBytes, retryCnt, twiStatus;
#endif


void TWI_Init(void)
{
	PORTD |= (1<<PD1) | (1<<PD0);       // enable internal pull-ups
	TWSR = 0;                           // status bits defaults to 1
	TWBR = MY_TWBR;                     // set baud rate
	TWCR |= (1<<TWEN);                  // enable TWI module
}


#ifdef ASCII_MODE
	int TWI_Write(uint8_t address,  char *data, uint8_t bytes)
#else
	int TWI_Write(uint8_t address,  uint8_t *data, uint8_t bytes)
#endif
{
	loop_until_bit_is_clear(twiStatus, BUSY); // Bus is busy, wait

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
	int TWI_Read(uint8_t address,  char *data, uint8_t bytes)
#else
	int TWI_Read(uint8_t address,  uint8_t *data, uint8_t bytes)
#endif
{
	loop_until_bit_is_clear(twiStatus, BUSY); // Bus is busy, wait

	twiAdrRW = (address<<1) + TW_READ;        // set address + data direction
	twiData = data;                           // pointer to data buffer
	twiBytes = bytes;                         // bytes in message
	retryCnt = 0;

	twiStatus |= (1<<BUSY);                   // entering twi busy state

	// generate start condition, enable twi irq
	TWCR = START_CONDITION | (1<<TWIE);

	return 0;
}


ISR(TWI_vect)                            // TWI Event has happened
{
#ifdef DEBUG_ON
	USART_TxString("ISR Entered, TW_STATUS: ");
	USART_TxNumber(TW_STATUS);
	USART_TxString("\r");
#endif
	switch(TW_STATUS)                     // check TWI-status
	{
		case TW_START:                     // 0x08 - Start condition or
		case TW_REP_START:                 // 0x10 - Repeated start condition
#ifdef DEBUG_ON
	USART_TxString("TWI Start cond, twiStatus: ");
	USART_TxNumber(twiStatus);
	USART_TxString("\r");
#endif
			if(retryCnt > 2)                // If 3 times NACK, abort
			{
				twiStatus = (twiStatus & ~(1<<BUSY)) | (1<<ERROR); // Bus no longer busy
#ifdef DEBUG_ON
	USART_TxString("retryCnt>4, Clear busy, set error, twiStatus: ");
	USART_TxNumber(twiStatus);
	USART_TxString("\r");
#endif

				TWCR = STOP_CONDITION & ~(1<<TWIE); // Generate stop condition, disable twi irq
				return;
			}
			TWDR = twiAdrRW;                // Transmit SLA + Read or Write
			TWCR = CLEAR_TWINT;
			break;

		case TW_MT_SLA_ACK:                // 0x18 - Master Tx, Slave acknowledged address
#ifdef DEBUG_ON
	USART_TxString("TWI Slave ACK address, twiStatus: ");
	USART_TxNumber(twiStatus);
	USART_TxString("\r");
#endif
			retryCnt = 0;                   // so clear retry counter
			TWDR = *twiData++;              // Transmit data, increment pointer
			TWCR = CLEAR_TWINT;             // and clear TWINT to continue
			break;

		case TW_MT_SLA_NACK:               // 0x20 - Master Tx, slave didn't acknowledge address, or
		case TW_MR_SLA_NACK:               // 0x48 - Master Rx, slave didn't acknowledge address,
#ifdef DEBUG_ON
	USART_TxString("TWI Slave NACK address, twiStatus: ");
	USART_TxNumber(twiStatus);
	USART_TxString("\r");
#endif
			retryCnt++;                     // this may mean that the slave is disconnected
			TWCR = START_CONDITION;         // Repeated Start condition
			break;

		case TW_MT_DATA_ACK:               // 0x28 - Master Tx, slave acknowledged data,
#ifdef DEBUG_ON
	USART_TxString("TWI Slave ACK data, twiStatus: ");
	USART_TxNumber(twiStatus);
	USART_TxString("\r");
#endif
			if(--twiBytes > 0)              // If there is more data to send,
			{

				TWDR = *twiData++;           // Send it, increment pointer
				TWCR = CLEAR_TWINT;
			}
			else
			{
				TWCR = STOP_CONDITION & ~(1<<TWIE); // Generate stop condition, disable twi irq
				twiStatus &= ~(1<<BUSY);     // Bus no longer busy
			}
			break;

		case TW_MT_DATA_NACK:              // 0x30 - Master Tx, Slave didn't acknowledge data
#ifdef DEBUG_ON
	USART_TxString("TWI Slave NACK data, twiStatus: ");
	USART_TxNumber(twiStatus);
	USART_TxString("\r");
#endif
			TWCR = STOP_CONDITION & ~(1<<TWIE); // Send stop condition, disable twi irq
			twiStatus = (twiStatus & ~(1<<BUSY)) | (1<<ERROR); // Bus no longer busy, error
			break;

		case TW_MT_ARB_LOST:               // 0x38 - Single master this can't be!!!
			TWCR = STOP_CONDITION & ~(1<<TWIE); // Send stop condition, disable twi irq
			twiStatus = (twiStatus & ~(1<<BUSY)) | (1<<ERROR); // Bus no longer busy, error
			break;

		case TW_MR_SLA_ACK:                // 0x40 - Slave acknowledged address
			if(--twiBytes > 0)
				TWCR = CLEAR_TWINT | (1<<TWEA); // If more than one byte to read, acknowledge
			else 
				TWCR = CLEAR_TWINT & ~(1<<TWEA); // else do not acknowledge
			break;

		case TW_MR_DATA_ACK:               // 0x50 - Master acknowledged data
			*twiData++ = TWDR;              // Store received data byte, increment pointer
			if(--twiBytes > 0)              // More data to read?
				TWCR = CLEAR_TWINT;
			else
				TWCR = CLEAR_TWINT & ~(1<<TWEA); // Don't acknowledge last byte received
			break;

		case TW_MR_DATA_NACK:              // 0x58 - Master didn't acknowledge data -> end of read process
			*twiData = TWDR;                // Read received data byte
			TWCR = STOP_CONDITION;          // Generate stop condition
			twiStatus &= ~(1<<BUSY);        // Bus no longer busy
	}
}
