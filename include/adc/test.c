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
    P1OUT &= ~BIT0;             // Set LED P1.0 to low state
    P1DIR |=  BIT0;             // Set LED P1.0 to output
    P6OUT &= ~BIT6;             // Set LED P6.6 to low state
    P6DIR |=  BIT6;             // Set LED P6.6 to output
    PM5CTL0 &= ~LOCKLPM5;       // enable GPIO

    unsigned int temp;

    while(1){
        temp = readADC(4);
        if (temp >= (0x7FF)){
            P1OUT |= BIT0;      // Turn on LED P1.0
        } else {
            P1OUT &= ~BIT0;     // Turn off LED P1.0
        }

        temp = readADC(5);
        if (temp >= (0x7FF)){
            P6OUT |= BIT6;      // Turn on LED P6.6
        } else {
            P6OUT &= ~BIT6;     // Turn off LED P6.6
        }
    }
}
