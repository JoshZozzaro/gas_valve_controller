#include <msp430.h>
#include "timer.c"

// Initializer function for servo. sets up gpio 5.0 and timer TB2
void initServo();

// Set servo to to any angle from 0-180 degrees
void setServo(int angle);
