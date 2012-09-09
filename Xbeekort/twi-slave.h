/*! \file
\brief Cansat project two-wire-interface communication bus slave software
*/
/*! \defgroup twi twi.h: Cansat project two-wire-interface communication bus slave software

Example usage:

\code
#include "twi.h"
#define TWI_OWN_ADRESS	0x01

char message[] = "Message to be sent via TWI";

int main(void){
	TW_Slave_Init(TWI_OWN_ADRESS);
	TW_Write(message, sizeof(message));
}
\endcode

The example code prepares the TWI slave system with a message. Whenever a master initiates contact with the address defined at TWI_OWN_ADRESS it will return the message. The entire transfer process is interrupt driven.

*/

extern volatile char *twiBuffer;
extern volatile uint8_t twiReadFinish;

#define TWI_OWN_ADRESS  0x03

#define TRUE            0x01
#define FALSE           0x00

#define TWI_BUSY        TWCR & (1<<TWINT)

#include <avr/io.h>
#include <util/twi.h>

//extern GPS_t gpsData;


/*! \ingroup twi
Initialize the TWI slave system.
*/
void TW_Slave_Init(uint8_t twSlaveAddress/*![in] TWI address the slave responds to*/);

/*! \ingroup twi
To be completed
\todo Fix
*/
void TW_Read(char *msg/*![in] *//*, uint8_t msgSize*//*![in] */);

/*! \ingroup twi
Prepares the TWI system with a message to send to master upon request
*/
void TW_Write(uint8_t *msg/*![in] Pointer to the message to be transfered*/, uint8_t msgSize/*![in] Size of the message, use sizeof(msg)*/);


/* Protocol:
Master: Address of µC, 1 databyte on which information channel
Slave: Responds x (channel dependent?) bytes of data
*/
