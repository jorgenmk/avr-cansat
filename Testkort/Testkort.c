#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "twi-slave.h"


#define LED1_DDR DDRB
#define LED1_PORT PORTB
#define LED1 (1<<PB3)

#define LED2_DDR DDRD
#define LED2_PORT PORTD
#define LED2 (1<<PD5)

#define LED3_DDR DDRD
#define LED3_PORT PORTD
#define LED3 (1<<PD7)

/*typedef struct {		char Tulle[6];
						char Rulle[6];
						char Og[3];
						char Knulle[7];
						} Tulle_t;

Tulle_t  Tulle_Data;*/

int main(void)
{	
	uint8_t Tulle_Data[]="Tulle rulle og knulle";

	LED1_DDR |= LED1;
	LED2_DDR |= LED2;
	LED3_DDR |= LED3;
	
	TW_Slave_Init(TWI_OWN_ADRESS);

	sei();

	TW_Write(Tulle_Data, sizeof(Tulle_Data));

	for(;;)
	{
		LED1_PORT |= LED1;
		_delay_ms(200);
		LED1_PORT &= ~LED1;

		LED2_PORT |= LED2;
		_delay_ms(200);
		LED2_PORT &= ~LED2;

		LED3_PORT |= LED3;
		_delay_ms(200);
		LED3_PORT &= ~LED3;
	}
	return 0;
}
