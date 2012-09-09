/***********************************************************************
  twi-slave.h
************************************************************************/

#define TWI_OWN_ADRESS  0x44
#define TRUE            0x01
#define FALSE           0x00

#define TWI_BUSY        TWCR & (1<<TWINT)

#include <avr/io.h>
#include <util/twi.h>

//extern GPS_t gpsData;


void TW_Slave_Init(uint8_t twSlaveAddress);
void TW_Read(uint8_t *msg, uint8_t msgSize);
void TW_Write(uint8_t *msg, uint8_t msgSize);


/* Protocol:
Master: Address of µC, 1 databyte on which information channel
Slave: Responds x (channel dependent?) bytes of data
*/
