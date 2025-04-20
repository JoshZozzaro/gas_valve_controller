/*    |\_/|  timer.c
 *    (* *)  version 1.0
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/19/25
 *  (_,-bb   last modified: 04/19/25
 *
 */

#include <msp430.h>

// Initializer function for servo. sets up gpio 5.0 and timer TB2
void initServo(){

    P5DIR  |=  BIT0;        // Set pin 5.0 as output.
    P5SEL0 |=  BIT0;        // Set pin 5.0 to be directly controlled by timer

    TB2CCR0 = 20000 - 1;    // Set the PWM period to 20ms
    TB2CCR1 = 500;          // Initial position = 0 degrees (valve closed)
    TB2CCTL1 = OUTMOD_7;    // configure TB2.1 to reset/set mode
                            // 1 when TB2CCR0 is reached
                            // 0 when TB2CCR1 is reached

    TB2CTL = TBSSEL__SMCLK | MC__UP | TBCLR;    // Configure timer B2
                                                // TBSSEL__SMCLK: use 1 MHz clock
                                                // MC__UP: count up from 0 to TB2CCR0
                                                // TBCLR: Clear existing count in timer
}

// Set servo to to any angle from 0-180 degrees
void setServo(int angle){
    TB2CCR1 = 500 + (((100*angle) / 180) * 20);
}
