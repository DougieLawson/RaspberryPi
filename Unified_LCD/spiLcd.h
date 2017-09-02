/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#ifndef SPILCD_H
#define SPILCD_H

//Lcd instructions
#define LCD_DISPLAYCONTROL 0x08

//display control flags
#define LCD_DISPLAYON      0x04

#define PIN_ENABLE 4
#define PIN_RS 6
#define PIN_BACKLIGHT 7

#define CMD_WRITE 0x40

#define IODIRB   0x01
#define OLATB    0x15

int gpio_reg;
int spi_fd;

void setDefaultHd44780 (hd44780 * header);
void initialiseDisplay (hd44780 * header);
void writeBytes (hd44780 * header, int byte, int mode);
void delay (unsigned int howLong);

#endif
