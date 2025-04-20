/*    |\_/|  test.c
 *    (* *)  version 1.0
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/19/25
 *  (_,-bb   last modified: 04/19/25
 *
 */

#include <msp430.h>
#include "timer.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Disable watchdog timer
    initServo();                // configure servo
    PM5CTL0 &= ~LOCKLPM5;       // enable GPIO

    int angle[4] = {0, 90, 180, 90};    // Array of angles (in degrees)
    while (1){
        for (int i=0; i<4; i++){        // Iterate through each angle
            setServo(angle[i]);         // Set sero to angle
            __delay_cycles(500000);     // Wait for 500000 clock cycles
        }
    }
}
