// gpio.h

#ifndef GPIO_H
#define GPIO_H

#include <msp430.h>
#include "gpio.c"

void initLED();   //pin initialisation for RGB LED , P 6.0- 6.2
void setRGB(unsigned char R, unsigned char G, unsigned char B);     //What LED to have on/off

void initSolenoid();       //pin initialisation for Solenoid P2.5
void setSolenoid(unsigned char state);      //control if Solenoid on/off

void initIgniter();        //pin initialisation for Igniter P2.0
void setIgniter(unsigned char state);         //control if Igniter on/off

void initCallForHeat();          //pin initialisation for Call for Heat P1.2
char callForHeat();             // checks if there is an active call for heat
void sleep();   // put system to sleep (LPM3) indefinitely until awoken by a call for heat

// NOTE: gpio.c also contains call for heat ISR

#endif
