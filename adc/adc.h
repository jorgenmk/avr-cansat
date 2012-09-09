/*! \file
\brief Cansat project sensorboard ADC converter
*/

/*! \defgroup adc adc.h: Cansat project sensorboard ADC converter

Example usage:

\code
#include "adc.h"
#define ADC_CHANNEL 0
uint16_t adc_result;
int main(void){
	adc_init();
	adc_result = adc_read(ADC_CHANNEL);
}
\endcode

A simple library for reading voltage outputs from different sensors.

*/

/*! \ingroup adc
 ADC initialization routine

 This routine initialises the ADC with the following parameters
  - 10bit precision
  - Prescaler at CLKio/128 (62.5kHz @ 8 MHz)
  - Not interrupt driven
  -
  - AREF capacitor to ground
  - AVCC connected to Vcc
  - One dummy convert is issued on channel 0

*/
void adc_init(void);

/*! \ingroup adc
Initializes an analog-to-digital convert. The 10bit ADC result is returned as a 16bit unsigned integer.
*/
uint16_t adc_read(uint8_t channel/*! [in] Selects which ADC circuitry input pin the conversion is run on*/);

/*! \ingroup adc
Reads the current passing through the shunt resistor in the power supply
*/
uint16_t ReadCurrent(double Rshunt/*! [in] Value of the shunt resistor in ohms*/, uint8_t pin_ina/*![in] ADC pin connected to the INA*/);