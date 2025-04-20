// gpio.c

// LED Initialization
void initLED() {
    P6DIR |= BIT0 | BIT1 | BIT2; // Set P6.0, P6.1, P6.2 as outputs (RGB)
    P6OUT &= ~(BIT0 | BIT1 | BIT2); // Turn off all LEDs initially
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

// Call for Heat Initialization
static volatile int callForHeatState = 0;

void initCallForHeat() {
    P2DIR &= ~BIT3;      // Set P2.3 as input (button)
    P2REN |= BIT3;       // Enable pull resistor
    P2OUT |= BIT3;       // Pull-up
    P2IES |= BIT3;       // High-to-low transition
    P2IFG &= ~BIT3;      // Clear interrupt flag
    P2IE  |= BIT3;       // Enable interrupt

    P1DIR |= BIT0;       // P1.0 as output (LED)
    P1OUT &= ~BIT0;      // Start OFF
}

// Call for Heat ISR
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2(void)
#else
#error Compiler not supported!
#endif
{
    P2IFG &= ~BIT3;                     // Clear interrupt flag
    callForHeatState ^= 1;             // Toggle state
    if (callForHeatState)
        P1OUT |= BIT0;                 // Turn ON LED
    else
        P1OUT &= ~BIT0;                // Turn OFF LED
}
