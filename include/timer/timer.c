/*    |\_/|  timer.c
 *    (* *)  version 1.2
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/19/25
 *  (_,-bb   last modified: 04/26/25
 *
 * This program implements the needed functions that use timers
 * including the following:
 *
 *      PWM for servo using TB2
 *      Timer interrupts for sleepSeconds()
 */

#include <msp430.h>
#include "timer.h"

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
    TB2CCR1 = 500 + (((100*angle) / 180) * 20); // Set TB2 Capture Compare register to needed time interval
}

// Put system to lower power mode (LPM3) for x number of seconds using timer TB0
void sleepSeconds(unsigned int seconds) {

    TB0CTL = TBSSEL_1 | MC__UP | TBCLR;     // ACLK, up mode, clear TAR
    TB0CCR0 = 32768 - 1;                    // Set up for 1-second delay (ACLK = 32.768 kHz)

    while (seconds--) {                     // While couting down to zero
        TB0CCTL0 = CCIE;                    // Enable interrupt for CCR0
        __bis_SR_register(LPM3_bits | GIE); // Enter LPM3 with interrupts enabled
        TB0CCTL0 &= ~CCIE;                  // Disable interrupt after waking up
    }

    TB0CTL &= ~MC__UP;                      // Stop Timer TB0
}


/***********************************************************************
*                                                                      *
*                 -- TB0 Interrupt Service Routine --                  *
*                                                                      *
***********************************************************************/

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Timer_B0(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMERB0_VECTOR))) Timer_B0 (void)
#else
#error Compiler not supported!
#endif
{
    TB0CCTL0 &= ~CCIFG; // Clear interrupt flag
    __bic_SR_register_on_exit(LPM3_bits); // Exit LPM3
}
