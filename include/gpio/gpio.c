// gpio.c

#include "gpio.h"

// LED Initialization
void initLED() {
    P6DIR |= BIT0 | BIT1 | BIT2; // Set P6.0, P6.1, P6.2 as outputs (RGB)
    P6OUT |= BIT0 | BIT1 | BIT2; // Turn off all LEDs initially
}

// Set RGB LED
void setRGB(unsigned char R, unsigned char G, unsigned char B) {
    if (R) P6OUT &= ~BIT0; else P6OUT |= BIT0;
    if (G) P6OUT &= ~BIT1; else P6OUT |= BIT1;
    if (B) P6OUT &= ~BIT2; else P6OUT |= BIT2;
}

// Solenoid Initialization
void initSolenoid() {
    P2DIR |= BIT5; // P2.5 as output
    P2OUT &= ~BIT5; // Off initially
}

// Set Solenoid
void setSolenoid(unsigned char state) {
    if (state) P2OUT |= BIT5;
    else P2OUT &= ~BIT5;
}

// Igniter Initialization
void initIgniter() {
    P2DIR |= BIT0; // P2.0 as output
    P2OUT &= ~BIT0; // Off initially
}

// Set Igniter
void setIgniter(unsigned char state) {
    if (state) P2OUT |= BIT0;
    else P2OUT &= ~BIT0;
}

// Check if there is still an active calll for heat
char callForHeat(){
    return P1IN & BIT2;
}

// Configure call for heat interface
void initCallForHeat() {
    P1DIR &= ~BIT2;     // Set P2.3 as input (button)
    P1IES &= ~BIT2;     // Rising edge transition
//    P1REN |= BIT3;      // Enable pull resistor
//    P2OUT |= BIT3;       // Pull-up
//    P2IES = BIT3;       // Falling edge transition
//    P2IFG &= ~BIT3;     // Clear interrupt flag
//    P1IE  |= BIT2;      // Enable interrupt
}

// Put system to sleep (LPM3) indefinitely until awoken by a call for heat.
void sleep(){
    setRGB(0, 0, 1);                    // Set RGB LED to blue
    P1IFG &= ~BIT2;                     // Clear interrupt flag
    P1IE  |= BIT2;                      // Enable interrupt
    __bis_SR_register(LPM3_bits | GIE); // Enter LPM3 with interrupts enabled
}

/***********************************************************************
*                                                                      *
*                       -- call for heat ISR --                        *
*                                                                      *
***********************************************************************/

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1(void)
#else
#error Compiler not supported!
#endif
{
    P1IFG &= ~BIT2;                         // Clear interrupt flag
    P1IE  &= ~BIT2;                         // disable interrupt
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}
