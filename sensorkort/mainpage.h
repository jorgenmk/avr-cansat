/*! \mainpage Sensorboard software documentation

This is the documentation for the sensorboard AVR microcontroller software for the cansat sensorboard hardware.

This document is provided as an aid to comprehend the sensorboard source code.

The sensorboard contains the following elements:
 - Two temperature sensors (thermistors)
 - Pressure sensor
 - Humidity sensor
 - Acceleration sensor
 - ATMega16 microcontroller

The pressure and acceleration sensors have only one output pin and are therefore quite simple to interface. The voltage output on this pin changes linearly and represents the measurement result. The AVR microcontroller utilises its analog to digital convert(ADC) circuitry to read output values from these sensors.

There are many ways to measure the resistance in a thermistor. One simple and cost-efficient method, yet precise enough for our needs, is to connect the thermistor in series with an known resistor and connect these to Vcc and ground. The ADC then measures the voltage between the known resistor and the thermistor, from this value the resistance within the thermistor is calculated. The temperature is then calculated from the resistance using Steinhart-Harts formula.

The humidity sensor is a two-terminal variable capacitor where the capacitance changes with the ambient humidity level. One very simple way to measure this capacitance is to apply a voltage jump from 0V to Vcc to the capacitor via a known large resistor. The voltage across the capacitor will rise according to the formula V(t) = Vcc*(1-e^-t/tau).

There are two interesting values of t
 - t = tau the voltage across the capacitor will be Vcc*(1-e*^-1) = Vcc*63\%
 - t = 0.5*tau the voltage across the capacitor will be Vcc*(1-e*^-0.5) = Vcc*39.3\%

These reference voltages are constructed from two voltage dividers and connected to the analog comparator within the microcontroller.

Due to electrical characteristics within the AVR the output pin on the AVR connected the the capacitor will never reach zero, but float between 0 and 0.3 volts. Getting tau by measuring the time it takes to go from 0V to Vcc*63.5\% will therefore give inconsistent and inaccurate results. Measuring tau as shown below will eliminate this error given the voltage at t = 0 is well below tau/2

Voltage is applied to the capacitor and a 16bit timer starts counting. An interrupt routine in the microcontroller will trigger as soon as the voltage rises above Vcc*39.3\% and store the counter value in a variable. As the voltage continues to rise above Vcc*63.3\% a second counter value is stored by the microcontroller. If the first value is subtracted from the second the result will be the number of clock cycles between tau/2 and tau. The microcontroller then calculates a precise value of tau.

\note Creating the voltage references with enough precision can prove difficult unless one has access to very precise resistors. To work around this limitation one may construct a voltage divider giving an approximate voltage, for instance the voltage expected at t=0.47*tau and t=1.12*tau. The definition TAU_FACTOR in main.h should then be changed to (1.12-0.47)*2 = 1.3

Voltage is applied to the capacitor and a 16bit timer starts counting. An interrupt routine in the microcontroller will trigger as soon as the voltage rises above Vcc*39.3\% and store the counter value in a variable. As the voltage continues to rise above Vcc*63.3\% the comparator triggers again and a second counter value is stored by the microcontroller. If the first value is subtracted from the second the result will be the number of clock cycles between tau/2 and tau. Knowing the number of cycles per second a value of tau can be calculated.

Now the formula C = tau/R gives the value of the capacitor, and from this the relative humidity can be found from the humidity sensors datasheet.

Access to the recorded data is provided via a two-wire interface connected to the bus. The AVR is initialised as a slave reciever. A master on the bus contacting the AVR will recieve the data as a long list of strings on the form "+xx.x".

*/

