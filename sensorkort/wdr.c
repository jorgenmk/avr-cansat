/*
 Title:    Cansat project sensorboard watchdog reset
 Filename: wdr.c
 Author:   Jorgen Moe Kvalvaag
 Date:     2009
 Software: AVR-GCC 4.3.3
 Hardware: ATmega16
*/
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "wdr.h"

void wdr_init(void){
	WDTCR |= (1 << WDTOE) | (1 << WDE);		// Initialise change, enable watchdog
	WDTCR |= (1 << WDE)|(1 << WDP1)|(1 << WDP0);	// WDR enabled, 0.14s timeout
}
