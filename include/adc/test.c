/*    |\_/|  test.c
 *    (* *)  version 1.0
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/20/25
 *  (_,-bb   last modified: 04/20/25
 *
 */

// P1.3 thermocouple    ADC A3
// P1.4 thermistor      ADC A4
// P1.5 pot             ADC A5

#include <msp430.h>
#include "adc.h"

int main(){
    WDTCTL = WDTPW | WDTHOLD;   // Disable watchdog timer
    initADC();                  // Configure ADC
    P6OUT &= ~BIT0;             // Set LED P6.0 to low state
    P6DIR |=  BIT0;             // Set LED P6.0 to output
    P6OUT &= ~BIT1;             // Set LED P6.1 to low state
    P6DIR |=  BIT1;             // Set LED P6.1 to output
    P6OUT &= ~BIT2;             // Set LED P6.2 to low state
    P6DIR |=  BIT2;             // Set LED P6.2 to output
    PM5CTL0 &= ~LOCKLPM5;       // enable GPIO

    unsigned int bits;

    while(1){
        bits = flameProved();
        if (bits > 0){
            P6OUT |= BIT0;      // Turn on red LED
        } else {
            P6OUT &= ~BIT0;     // Turn off red LED
        }

        bits = readADC(THERMISTOR);
        if (bits >= 250){
            P6OUT |= BIT1;      // Turn on green LED
        } else {
            P6OUT &= ~BIT1;     // Turn off green LED
        }

        bits = readADC(POT);
        if (bits >= 250){
            P6OUT |= BIT2;      // Turn on blue LED
        } else {
            P6OUT &= ~BIT2;     // Turn off blue LED
        }
    }
}
