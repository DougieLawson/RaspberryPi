/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#ifndef GPIOLCD_H
#define GPIOLCD_H

//Lcd instructions
#define LCD_DISPLAYCONTROL 0x08

void setDefaultHd44780 (hd44780 * header);
void initialiseDisplay (hd44780 * header);
void writeBytes (hd44780 * header, int byte, int mode);

#endif
