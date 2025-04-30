/*    |\_/|  main.c
 *    (* *)  version 1.1
 *  ) /  T   created by
 * ( /  ||   Joshua Zozzaro
 *  (_,-bb   04/29/25
 *
 * This is the main program which directly controls
 * the operation of the gas valve control unit. All
 * functions and macros are defined in the included
 * header files.
 */

#include "include/gpio/gpio.h"
#include "include/adc/adc.h"
#include "include/timer/timer.h"
#include <msp430.h>

volatile unsigned int adcResult = 0;

void setup(){
    WDTCTL = WDTPW | WDTHOLD;   // Disable watchdog timer
    initCallForHeat();          // Configure call for heat interface
    initServo();                // Configure servo timer and gpio
    initADC();                  // Configure adc settings
    initLED();                  // Configure RGB LED interface
    initSolenoid();             // Configure solenoid interface
    initIgniter();              // Configure igniter interface
    PM5CTL0 &= ~LOCKLPM5;       // Enable GPIO 
}

int main(){
    
    setup();

    char error = 0;         // Initialize variable for tracking error status
    char retry = 0;         // Initialize variable for determining if system should go to sleep after each iteration of main while loop
    char flameLoss = 0;     // Initialize variable for counting number of times flame is lost during each call for heat
    char failToIgnite = 0;  // Initialize variable for counting number of times the pilot light failed to ignite during each call for heat
    
    sleep();    // Go to sleep (LPM3) until call for heat

    while(1){

        error = 0;  // Reset error status
        retry = 0;  // Reset loop status

        // Try to ignite
        while(1){
            setRGB(0, 1, 0);                // Set RGB LED to green
            setIgniter(1);                  // Turn on igniter
            setSolenoid(1);                 // Open pilot valve
            sleepSeconds(1);                // Wait one second
            if (flameProved()){             // If flame detected
                break;                      // Break out of while loop
            } else {                        // If flame NOT detected
                setSolenoid(0);             // Close pilot valve
                setIgniter(0);              // Turn off igniter
                setRGB(1, 0, 0);            // Set RGBLED to red
                if (++failToIgnite > 5){    // If pilot light has failed to ignite more than 5 times
                    sleepSeconds(5*60);     // pause for 5 minutes
                    failToIgnite = 0;       // Restart count
                } else {
                    sleepSeconds(2);        // wait 2 seconds
                }                    
            }
        }

        // Flame proved. Turn off igniter and open main valve
        setIgniter(0);      // Turn off igniter
        setServo(180);      // Open main valve
        sleepSeconds(2);    // Wait for 2 seconds
        setSolenoid(0);     // Close pilot valve

        // Run sequence
        while(callForHeat()){
            if (!flameProved()){            // If flame is not detected
                setRGB(1, 0, 0);            // Set RGB LED to red
                flameLoss++;                // Increment number of flame losses this heat cycle
                error = 1;                  // Set error status
                break;                      // Exit loop
            } else if (boilerOverTemp()){   // If boiler temperature exceeds set point
                setRGB(1, 0, 0);            // Set RGB LED to red
                error = 2;                  // Set error status
                break;                      // Exit loop
            }
        }

        setServo(0);    // Close main valve

        // Determine if system shut down because of an error
        if (error == 1){            // If system shut down because of a flame loss
            if (++flameLoss > 5){   // If system has had more than 5 flame losses this heat cycle
                sleepSeconds(5*60); // Pause for 5 minutes
            } else {
                retry = 1;          // Re-ignite and try again. Do NOT clear counter variables or go to sleep.
            }
        } else if (error == 2){     // If system shut down due to boiler temp exceeding set point
            sleepSeconds(5*60);     // Pause for 5 minutes
        }

        if (! retry){               // If no error or system has paused for 5 minutes due to error
            flameLoss = 0;          // Reset flame loss counter
            failToIgnite = 0;       // Reset fail to ignite counter
            sleep();                // Go to sleep (will re-awaken immedietely if call for heat is still active)
        }
    }
}
