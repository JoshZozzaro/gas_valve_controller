/*    |\_/|  adc.h
 *    (* *)  version 1.3
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/20/25
 *  (_,-bb   last modified: 04/29/25
 *
 */

extern volatile unsigned int adcResult;

#define THERMOCOUPLE 3
#define THERMISTOR   4
#define POT          5
#define ADCMAX       4095

#define THERMOCOUPLE_THRESHOLD 7
#define THERMISTOR_THRESHOLD    2797

#ifndef ADC_H
#define ADC_H

#include "adc.c"

void initADC();
unsigned int readADC(char channel);
char flameProved();
char boilerOverTemp();

#endif
