/*******************************************************
 *                USART.h av jkm-                      *
 *******************************************************/

#define FOSC 3686400// Clock Speed
//#define FOSC 1000000
#define BAUD 4800
#define MYUBRR FOSC/16/BAUD-1

void USART_Init(unsigned int ubrr);
void USART_Send_Char(unsigned char data);
char USART_Get_Char(void);
void USART_Send_Str(unsigned char *str);
void USART_Get_Str(unsigned char *str);

