//#define F_CPU       8000000UL
#define F_TWI_SCL   27000                // desired freq of TWI SCL-line
#define BAUD        9600
#define MYUBRR      F_CPU/16/BAUD-1

#define GPS_ON    (1<<PC6)
#define SENSOR_ON (1<<PC5)
#define RADIO_ON 	(1<<PC7)

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_R  (1<<PB6)
#define LED_G  (1<<PB5)
#define R_ON (LED_PORT |= LED_R)
#define R_OFF (LED_PORT &= ~LED_R)
#define R_SWITCH (LED_PORT ^= LED_R)


#define G_ON (LED_PORT |= LED_G)
#define G_OFF (LED_PORT &= ~LED_G)
#define G_SWITCH (LED_PORT ^= LED_G)

// TWI addresses
#define GPS_CARD		0x01
#define SENSOR_CARD 	0x02
#define RADIO_CARD  	0x03
#define TEST_CARD   	0x44
#define LM_SENSOR		0x48

//#define DEBUG_ON

#define RSHUNT		5.6
#define PIN_INA		4

// defines wheather data transmitted on twi-bus normally are ascii-characters
#define ASCII_MODE
