#include "defines.h"

#ifndef F_CPU
	#error "F_CPU not defined for twi-master.h"
#endif

#ifndef F_TWI_SCL
	#error "F_TWI_SCL not defined for twi-master.h"
#endif

#if F_CPU/F_TWI_SCL < 16
	#error "TWI freq has to be at least 16 times lower than F_CPU"
#endif
#define MY_TWBR   ((F_CPU/F_TWI_SCL - 16) / 2)
#if MY_TWBR < 10
	#error "MY_TWBR too low, has to be larger than 10"
#endif


#define START_CONDITION  ( (TWCR & ~(1<<TWSTO)) | (1<<TWINT) | (1<<TWSTA) )
#define CLEAR_TWINT      ( (TWCR & ~((1<<TWSTA) | (1<<TWSTO))) | (1<<TWINT) )
#define STOP_CONDITION   ( (TWCR & ~(1<<TWSTA)) | (1<<TWINT) | (1<<TWSTO) )


/* Bit definitions for the twStatus register */
#define BUSY 7
#define ERROR 4

void TWI_Init(void);

#ifdef ASCII_MODE
	uint8_t TWI_Write(uint8_t address, char *data, uint8_t bytes);
	uint8_t TWI_Read(uint8_t address, char *data, uint8_t bytes);
#else
	uint8_t TWI_Write(uint8_t address, uint8_t *data, uint8_t bytes);
	uint8_t TWI_Read(uint8_t address, uint8_t *data, uint8_t bytes);
#endif

