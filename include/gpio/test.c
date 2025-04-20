//test.c

#include <msp430.h>
#include "gpio.h"

void delay() {
    volatile unsigned int i;
    for (i = 0; i < 50000; i++);  // crude delay loop
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    // === Initialize all components ===
    initLED();
    initSolenoid();
    initIgniter();
    initCallForHeat();

    PM5CTL0 &= ~LOCKLPM5;       // Enable GPIOs

    // === Test 1: RGB LED ===
    while(1){
        setRGB(1, 0, 0);   // Red
        delay();
        setRGB(0, 1, 0);   // Green
        delay();
        setRGB(0, 0, 1);   // Blue
        delay();
        setRGB(1, 1, 1);   // White (all ON)
        delay();
        setRGB(0, 0, 0);   // OFF
        delay();
    
    // === Test 2: Solenoid ===
    setSolenoid(1);  // ON
    delay();
    setSolenoid(0);  // OFF
    delay();
    
    // === Test 3: Igniter ===
    setIgniter(1);   // ON
    delay();
    setIgniter(0);   // OFF
    delay();
    }

    // === Test 4: Call for heat ISR trigger simulation ===
    // Simulated manually by pressing button on P1.2 (real ISR)

    while (1) {
        // Sit idle here to allow CallForHeat ISR to be triggered via button
        __bis_SR_register(LPM3_bits | GIE);  // Sleep until ISR wakes
    }

}
