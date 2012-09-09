/*******************************************************
 *                USART.h av jkm-                      *
 *******************************************************/

#define FOSC 3686400// Clock Speed
//#define FOSC 1000000
//#define BAUD 9600
//#define MYUBRR FOSC/16/BAUD-1
#include "defines.h"

void USART_Init(unsigned int ubrr);
void USART_Send_Char(unsigned char data);
unsigned char USART_Get_Char(void);
void USART_TxString(unsigned char *str);
void USART_Get_Str(unsigned char *str);

