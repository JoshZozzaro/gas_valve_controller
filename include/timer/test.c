/*    |\_/|  test.c
 *    (* *)  version 1.1
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/19/25
 *  (_,-bb   last modified: 04/26/25
 *
 * This program tests all of the functions
 * in timer.h and timer.c
 */

#include <msp430.h>
#include "timer.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;           // Disable watchdog timer
    initServo();                        // configure servo
    PM5CTL0 &= ~LOCKLPM5;               // enable GPIO

    int angle[4] = {0, 90, 180, 90};    // Array of angles (in degrees)
    while (1){
        for (int i=0; i<4; i++){        // Iterate through each angle
            setServo(angle[i]);         // Set sero to angle
            sleepSeconds(2);            // put system to sleep for 2 seconds
        }
    }
}
