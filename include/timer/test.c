//    |\_/|  test.c
//    (* *)  version 1.0
//  ) /  T   created by
// ( /  ||   Joshua Zozzaro
//  (_,-bb   04/19/25

#include <msp430.h>
#include "timer.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Disable watchdog timer
    initServo();                // configure servo
    PM5CTL0 &= ~LOCKLPM5;       // enable GPIO

    int angle[4] = {0, 90, 180, 90};
    while (1){
        for (int i=0; i<4; i++){
            setServo(angle[i]);
            __delay_cycles(500000);
        }
    }
}
