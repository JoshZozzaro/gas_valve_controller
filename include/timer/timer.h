/*    |\_/|  timer.h
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

#ifndef TIMER_H
#define TIMER_H
#include <msp430.h>
#include "timer.c"

// Initializer function for servo. sets up gpio 5.0 and timer TB2
void initServo();

// Set servo to to any angle from 0-180 degrees
void setServo(int angle);

// Put system to lower power mode (LPM3) for x number of seconds using timer TB0
void sleepSeconds(unsigned int seconds);

#endif
