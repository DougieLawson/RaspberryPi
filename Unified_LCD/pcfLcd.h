/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#ifndef PCFLCD_H
#define PCFLCD_H

#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7
#define PIN_ENABLE 2
#define PIN_RW 1
#define PIN_RS 0
#define PIN_BACKLIGHT 3

#define PCF_ADDR 0x27
int gpio_reg;
int i2c_fd;

void setDefaultHd44780 (hd44780 * header);
void initialiseDisplay (hd44780 * header);
void writeBytes (hd44780 * header, int byte, int mode);

#endif
