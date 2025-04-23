/*    |\_/|  timer.h
 *    (* *)  version 1.1
 *  ) /  T   author: Joshua Zozzaro
 * ( /  ||   created: 04/19/25
 *  (_,-bb   last modified: 04/22/25
 *
 */

#ifndef TIMER_H
#define TIMER_H
#include <msp430.h>
#include "timer.c"

// Initializer function for servo. sets up gpio 5.0 and timer TB2
void initServo();

// Set servo to to any angle from 0-180 degrees
void setServo(int angle);

#endif
