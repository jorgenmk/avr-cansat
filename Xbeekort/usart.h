
#include <inttypes.h>

/*! \file
\brief Cansat project usart communication software
*/
/*! \defgroup usart usart.h: Cansat project usart communication software

Example usage:

\code
#include "usart.h"
#define BAUD	9600

char message[] = "Message to be sent via UART";

int main(void){
	USART_Init(BAUD);
	USART_TxString(message);
}
\endcode

The example code sends the string 'message' via UART immediately.
*/
#define F_CPU       1000000UL
#define BAUD        9600
#define MYUBRR      F_CPU/16/BAUD-1



/*! \ingroup usart
Initialize UART-interface
*/
void USART_Init(uint16_t baud/*![in] Sets the BAUD rate of the UART interface*/);

/*!Transmits byte over UART-interface*/
void USART_Transmit(uint8_t data);

/*! \ingroup usart
Transmits byte via UART-interface
*/
void USART_Transmit(uint8_t data/*![in] Byte to be transmitted*/)/* BOOTLOADER_SECTION*/;

/*! \ingroup usart
Receive byte from UART-interface
*/
char USART_Receive(void);

void USART_Flush(void);

/*!Transmits string over UART-interface*/
void USART_TxString(char *string);

/*! \ingroup usart
Transmit a string via UART-interface
*/
void USART_TxString(char *string/*![in] Pointer to transmit string*/) /*BOOTLOADER_SECTION*/;

/*! \ingroup usart
Transmits a number via UART-interface
*/
void USART_TxNumber(uint16_t number/*![in] Number to be transfered via UART*/);

