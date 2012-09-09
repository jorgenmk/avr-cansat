

/*!Initialize UART-interface*/
void USART_Init(uint16_t ubrr);

/*!Transmits byte over UART-interface*/
void USART_Transmit(uint8_t data);/* BOOTLOADER_SECTION*/;

/*!Receive byte from UART-interface*/
uint8_t USART_Receive(void); /*BOOTLOADER_SECTION*/;

/*!Transmits string over UART-interface*/
void USART_TxString(char *str/*!Pointer to transmit string*/);

void USART_TxNumber(uint16_t number);

void USART_TxDouble(double number);