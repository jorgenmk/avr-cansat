/*! \file
\brief Cansat project sensorboard humidity sensor software
*/
/*! \defgroup humid humid.h: Cansat project sensorboard humidity sensor software

Example usage:

\code
#include "humid.h"
#define ADC_MULTIPLEX_1	1
#define ADC_MULTIPLEX_2	2
uint16_t humid_result;

int main(void){
	humid_init();
	humid_result = humid_read(ADC_MULTIPLEX_1, ADC_MULTIPLEX_2);
}
\endcode

These functions are used to measure relative humidity from a HCH-1000 humidity sensor from Honeywell S&C.

*/

/*! \ingroup humid
	Relative humidity sensing system initialisation routine.
*/
void humid_init(void);

/*! \ingroup humid
	Starts the humidity sensing system. The result is returned as a 16bit integer representing tau as the number of CPU cycles. The capacitance can then be calculated with the formula C = tau/R.
*/
uint16_t humid_read(uint8_t adc_multiplex_1/*![in] The first ADC channel, connected to a voltage reference at tau/2*/, uint8_t adc_multiplex_2)/*![in] The second ADC channel, connected to a voltage reference at tau*/;
