//#include <inttypes.h>

void XbeeInit(void);
void XbeeTxByte(char byte);
void XbeeTxString(char *string);
void XbeeEnterCmdMode(void);
void LedBlinkInit(void);
uint8_t XbeeWaitOK(void);
void BlinkLed(void);
