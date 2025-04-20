/*    |\_/|  adc.c
 *    (* *)  version 1.0
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/20/25
 *  (_,-bb   last modified 04/20/25
 *
 */

#include <msp430.h>

void initADC(){
    P1SEL0 |= BIT3 | BIT4 | BIT5;       // Set pins 1.3, 1.4, and 1.5 as inputs
    P1SEL1 |= BIT3 | BIT4 | BIT5;       // Set pins 1.3, 1.4, and 1.5 to use ADCs

    ADCCTL0 |= ADCSHT_2 | ADCON;        // Enable ADC
    ADCCTL1 |= ADCSHP;                  // Set ADC sampling time
    ADCCTL2 &= ~ADCRES;                 // Clear existing resolution settings
    ADCCTL2 |= ADCRES_2;                // Set ADC resolution to 12 bits
    ADCMCTL0 |= ADCINCH_3;              // MODIFY each time you read to specify which channel to read from
    ADCIE |= ADCIE0;                    // Enable ADC interrupts
}

unsigned int readADC(char channel){
    ADCCTL0 &= ~ADCENC;     // Disable ADC so settings can be changed
    switch(channel){        // NOTE: MUST use = instead of |= to set ALL other channels to 0. Can only read exactly 1 channel.
        case 3: ADCMCTL0 = ADCINCH_3; break;   // OVERWRITE ADC SETTINGS to read only from channel 3
        case 4: ADCMCTL0 = ADCINCH_4; break;   // OVERWRITE ADC SETTINGS to read only from channel 4
        case 5: ADCMCTL0 = ADCINCH_5; break;   // OVERWRITE ADC SETTINGS to read only from channel 5
        default: return -1;                    // Channel not supported
    }

    ADCCTL0 |= ADCENC | ADCSC;          // Begin ADC read
    while(ADCIV != ADCIV_ADCIFG);       // Wait for ADC to finish reading
    return ADCMEM0;                     // Return ADC measurement
}
