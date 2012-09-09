/*! \file
\brief Cansat project sensorboard watchdog reset
*/
/*! \defgroup wdr wdr.h: Cansat project sensorboard watchdog reset

Example usage:

\code
#include "wdr.h"

int main(void){
	wdr_init();

}
\endcode

The wdr module will reset the processor if it hangs for more than 0.14 seconds.

*/

/*! \ingroup wdr
 * Intitialise the watchdog reset function in the AVR
 */
void wdr_init(void);
