#define F_TWI_SCL   27000                // desired freq of TWI SCL-line
#define BAUD        9600
#define MYUBRR      F_CPU/16/BAUD-1

// TWI addresses
#define GPS_CARD    0x01
#define SENSOR_CARD 0x02
#define RADIO_CARD  0x03
#define TEST_CARD   0x44

//#define DEBUG_ON

// defines wheather data transmitted on twi-bus normally are ascii-characters
#define ASCII_MODE
