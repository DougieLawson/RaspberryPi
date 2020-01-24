/*
 *  Mock wiringPi.h
 *  Copyright (C) Dougie Lawson 2020, all rights reserved.
 */

#ifndef __WIRING_PI_H__
#define __WIRING_PI_H__

#ifndef TRUE
#define TRUE (1=1)
#define FALSE (!TRUE)
#endif

#define INPUT	0
#define OUTPUT	1
#define LOW	0
#define HIGH	1

#define GPSET0	7
#define GPCLR0	10

#define PI_BANK	(pin>>5)
#define PI_BIT	(1<<(pin&0x1f))

void delay (unsigned int howLong);
int  wiringPiSetupGpio (void);
void pinMode (int pin, int mode);
void digitalWrite (int pin, int value);

#endif
