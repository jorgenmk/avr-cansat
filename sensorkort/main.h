/*! \file
\brief Cansat project main program header file
*/
/*! \defgroup main main.h: Cansat project main program header file

This module describes how the cansat sensorboard main program works.

 */
/*! \ingroup main
Connection definition: Inside temperature sensor is connected to ADC pin 2
*/
#define TEMP_IN		2

/*! \ingroup main
Connection definition: Outside temperature sensor is connected to ADC pin 0
*/
#define TEMP_OUT 		0

/*! \ingroup main
Connection definition: Pressure sensor is connected to ADC pin 3
*/
#define PRESSURE 		3

/*! \ingroup main
Connection definition: Accelerometer is connected to ADC pin 5
*/
#define ACCEL		5

/*! \ingroup main
Connection definition: Current monitor INA193 connnected to ADC pin 4
*/
#define CURRENT_MONITOR 	4

/*! \ingroup main
Connection definition: ADC6 on pin 6
*/
#define ADC6		6

/*! \ingroup main
Connection definition: ADC7 on pin 7
*/
#define ADC7		7

/*! \ingroup main
The tau measuring system realized in humid.c is dependent on two reference voltages, representing the voltage at t=tau/2 and t=tau. Since these are created from voltage dividers these voltages tend to vary according to the resistor error tolerance, common values range from 0.1 to 5\% (using low tolerance resistors is highy recommended).

These reference voltages will therefore not trigger the comparator at exactly t=tau/2 and t=tau, they might instead trigger at t=0.47*tau and t=1.12*tau. In this exaple TAU_FACTOR should be changed to (1.12-0.47)*2 = 1.3
*/
#define TAU_FACTOR	1.01772508


/*! \ingroup main
Value of the resistor in the humidity sensing RC-network(exact value required for optimal accuracy)
*/
#define RESISTOR_HUMIDITY 179200

/*! \ingroup main
Value of the resitor in series with the outside temperature sensor (exact value required for optimal accuracy)
*/
#define TEMP_RESISTOR_OUT 99700

/*! \ingroup main
Value of the resistor in series with the inside temperature sensor(exact value required for optimal accuracy)
*/
#define TEMP_RESISTOR_IN 101200

/*! \ingroup main
The pressure sensor outputs voltage between 0 - 5 volts, but the ADC can only read 0 - 3.3 volts. A voltage divider is placed between the sensor output and the ADC input to scale down

This value represents the scaling effect of the voltage divider.

The formula used to calculate this value: PRESSURE_SCALE_FACTOR = Vsensor/Vadc.

This allows construction of the circuit using low-precision resistors (5\%-10\%). The values should be measured with a precision voltmeter on the actual hardware and inserted into the definition

\note Make sure the voltage measured by the AVR doesn't rise above 3.3 volts when the sensor output reaches 5 volts!  This means that the scaling factor should not be lower than 1.5152
*/
#define PRESSURE_SCALE_FACTOR  1.51504

/*! \ingroup main
The accelerometer outputs voltage between 0 - 5 volts, but the ADC can only read 0 - 3.3 volts. A voltage divider is placed between the sensor output and the ADC input.

This value represents the scaling effect of the voltage divider.

The formula used to calculate this value: ACCEL_SCALE_FACTOR = Vsensor/Vadc.

This allows construction of the circuit using low-precision resistors (5\%-10\%). The values should be measured with a precision voltmeter on the actual hardware and inserted into the definition

\note Make sure the voltage measured by the AVR doesn't rise above 3.3 volts when the sensor output reaches 5 volts! This means that the scaling factor should not be lower than 1.5152
*/
#define ACCEL_SCALE_FACTOR  1.51534

/*! \ingroup main
 *
 * The arrays in this data structure contains the average values from the last eight samples from the sensors. This is the data sent to the mainboard via the two-wire interface.
 *
 * The values are stored as six byte long strings "+xx.x\0"
 *
 */

typedef struct {
	char temp_string_in[6];		//!< Contains a string representing the temperature inside the cansat
	char temp_string_out[6];	//!< Contains a string representing the temperature outside the cansat
	char humidity_string[6];	//!< Contains a string representing the humidity level outside the cansat
	char pressure_string[6];	//!< Contains a string representing the pressure outside the cansat
	char accel_string[6];		//!< Contains a string representing the acceleration forces applied to cansat
	double mah_sensorkort;
} data;

/*! \ingroup main
 *
 * This data structure is used to store eight consecutive measurements from the sensors. This data is used to calculate the average values sent to the mainboard
 *
 */
typedef struct {
	double temp_in[8];		//!< Contains the last eight recorded inside temperatures
	double temp_out[8];		//!< Contains the last eight recorded outside temperatures
	double humidity[8];		//!< Contains the last eight recorded humidity levels
	double pressure[8];		//!< Contains the last eight pressure readings
	double acceleration[8];	//!< Contains the last eight acceleration force readings
} avg_values;

/*! \ingroup main
 *
 *  These are the Steinhart-Hart coefficients for the Vishay SMD thermistor
 *
 */
double shk_in[] = { 0.001602555859338,
		0.000184675766183,
		0.000005664300751,
		-0.000000000017122};

/*! \ingroup main
 *
 * These are the Steinhart-Hart coefficients for Betatherm precision thermistor
 *
 */

double shk_out[] = {0.001525768873324,
  		 0.000217362722462,
  		 0.000002500750142,
  		 0.000000000003197};


/*! \ingroup main
 *
 * Initialise microcontroller. All unused pins get their internal pull-ups activated.
 *
 */
void main_mcu_init(void);


/*! \ingroup main
 *
 * Converts the ADC voltage to a temperature value in Celcius
 *
 */
double adc2temp(double Vadc /*!<[in] 10 bit voltage value from the ADC */, double R1 /*![in] The value of the resistor in series with the thermistor */, double shk[] /*!<[in] Array containing the Steinhart-Hart coefficients for the thermistor*/);

/*! \ingroup main
 *
 * Coverts the ADC voltage to a g-force value. The g-force is measured in the Z-direction
 *
 */
double adc2acceleration(uint16_t value /*!<[in] 10 bit voltage value from the ADC */);

/*! \ingroup main
 *
 * Converts the ADC voltage to pressure in kPa
 */
double adc2pressure(uint16_t value /*!<[in] 10 bit voltage value from the ADC */);

/*! \ingroup main
 *
 * Converts the tau reading from humid_read() to a relative humidity value between 0 - 100\%RH
 *
 */
double tau2humidity(uint16_t tau /*!<[in] tau represented by number of clock cycles */);

/*! \ingroup main
 *
 * Converts a double to a string
 *
 */
void double2string(double value /*!<[in] The value to be converted*/, uint8_t* string /*!< [out] The resulting string, the format is "xx.x"*/);

#define LED_ON	PORTD &= ~(1 << PD3)
#define LED_OFF	PORTD |= (1 << PD3)

