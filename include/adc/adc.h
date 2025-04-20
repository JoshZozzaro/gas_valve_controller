/*    |\_/|  adc.h
 *    (* *)  version 1.0
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/20/25
 *  (_,-bb   last modified: 04/20/25
 *
 */

#include <msp430.h>
#include "adc.c"

#define THERMOCOUPLE 3
#define THERMISTOR   4
#define POT          5
#define ADCMAX       4095

void initADC();
unsigned int readADC(char channel);
