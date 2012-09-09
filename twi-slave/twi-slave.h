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

#define TRUE            0x01
#define FALSE           0x00

#define TWI_BUSY        TWCR & (1<<TWINT)

#include <avr/io.h>
#include <util/twi.h>
#include "defines.h"
/*! \ingroup twi
Initialize the TWI slave system.
*/
void TW_Slave_Init(uint8_t twSlaveAddress/*![in] TWI address the slave respons to*/);

/*! \ingroup twi

*/
void TW_Read(uint8_t *msg/*![in] */, uint8_t msgSize/*![in] */);

/*! \ingroup twi
Prepares the TWI system with a message to send to master upon request
*/
void TW_Write(uint8_t *msg/*![in] */, uint8_t msgSize/*![in] */);
