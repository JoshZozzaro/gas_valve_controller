/*    |\_/|  adc.c
 *    (* *)  version 1.2
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/20/25
 *  (_,-bb   last modified 04/29/25
 *
 */

#include <msp430.h>
#include "adc.h"

void initADC(){
    P1SEL0 |= BIT3 | BIT4 | BIT5;       // Set pins 1.3, 1.4, and 1.5 as inputs
    P1SEL1 |= BIT3 | BIT4 | BIT5;       // Set pins 1.3, 1.4, and 1.5 to use ADCs

    ADCCTL0 |= ADCSHT_2 | ADCON;        // Enable ADC
    ADCCTL1 |= ADCSHP;                  // Set ADC sampling time
    ADCCTL2 &= ~ADCRES;                 // Clear existing resolution settings
    ADCCTL2 |= ADCRES_2;                // Set ADC resolution to 12 bits
    ADCMCTL0 = ADCINCH_3;              // MODIFY each time you read to specify which channel to read from
    ADCIE |= ADCIE0;                    // Enable ADC interrupts
}

unsigned int readADC(char channel) {

    ADCCTL0 &= ~ADCENC; // Disable ADC so settings can be changed

    // Select the ADC channel. MUST use '=' instead of '|=' for assignment.
    switch(channel) {
        case 3: ADCMCTL0 = ADCINCH_3; break;
        case 4: ADCMCTL0 = ADCINCH_4; break;
        case 5: ADCMCTL0 = ADCINCH_5; break;
        default: return 0; // Unsupported channel
    }
    
    ADCIE |= ADCIE0;                    // Enable ADC
    ADCCTL0 |= ADCENC | ADCSC;          // Measure ADC
    
    __bis_SR_register(LPM3_bits + GIE); // Go to sleep (LPM3) with interrupts enabled
                                        // Will be awoken by ADC ISR

    ADCIE &= ~ADCIE0;                   // Disable ADC interrupts
    return adcResult;                   // Return ADC value stored in global variable adcResult by ADC ISR
}

char flameProved(){
    unsigned int bits = readADC(THERMOCOUPLE);
    if (bits >= THERMOCOUPLE_THRESHOLD){
        return 1;
    } else {
        return 0;
    }
}

char boilerOverTemp(){
    return readADC(THERMISTOR) > readADC(POT);
}


/***********************************************************************
*                                                                      *
*              -- ADC Interrupt Service Routine (ISR) --               *
*                                                                      *
***********************************************************************/

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR(void)
#else
#error Compiler not supported!
#endif
{
    /* 
     * Use __even_in_range on ADCIV to keep the switch-case efficient. The 
     * ADCIV register holds the highest pending interrupt flag; here we handle the 
     * conversion complete interrupt.
     */
    switch(__even_in_range(ADCIV, ADCIV_ADCIFG)) {
        case ADCIV_ADCIFG:    // ADC conversion complete
            adcResult = ADCMEM0;  // Capture the ADC conversion result
            // Exit LPM3 on ISR exit so the CPU resumes execution in readADC()
            __bic_SR_register_on_exit(LPM3_bits);
            break;
        default: break;
    }
}
