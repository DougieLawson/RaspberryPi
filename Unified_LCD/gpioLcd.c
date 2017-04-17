/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "hd44780.h"
#include "commonLcd.h"
#include "gpioLcd.h"
#include "wiringPi.h"

void setDefaultHd44780(hd44780 * header)
{
	if(!header)
		return;
	header->D4 = 13;
	header->D5 = 6;
	header->D6 = 5;
	header->D7 = 12;

	header->registerSelect = 26;
	header->enable = 16;

	header->colNumber = 2;
	header->rowNumber = 16;

        setenv("WIRINGPI_GPIOMEM", "1", 1);
	wiringPiSetupGpio();

	pinMode(header->D4,OUTPUT);
	pinMode(header->D5,OUTPUT);
	pinMode(header->D6,OUTPUT);
	pinMode(header->D7,OUTPUT);
	pinMode(header->registerSelect,OUTPUT);
	pinMode(header->enable,OUTPUT);

}

void initialiseDisplay(hd44780 * header)
{
	if(!header)
		return;

	writeBytes(header, 0b00110011, LCD_COMMAND_MODE);
	writeBytes(header, 0b00110000, LCD_COMMAND_MODE);
	writeBytes(header, 0b00000110, LCD_COMMAND_MODE);

	header->displayControl = (header->displayControl | LCD_DISPLAYCONTROL) | LCD_DISPLAYON;
	writeBytes(header, header->displayControl, LCD_COMMAND_MODE);
}

static void pulse(hd44780 * header)
{
	delay(1);
	digitalWrite(header->enable,LOW);
	delay(1);
	digitalWrite(header->enable,HIGH);
	delay(1);
	digitalWrite(header->enable,LOW);
	delay(1);

}

void writeBytes(hd44780 * header, int byte, int mode)
{
	if(!header)
		return;
	digitalWrite(header->registerSelect, mode);
	delay(1);

	digitalWrite(header->D7,(byte & 0x80));
	digitalWrite(header->D6,(byte & 0x40));
	digitalWrite(header->D5,(byte & 0x20));
	digitalWrite(header->D4,(byte & 0x10));

	pulse(header);

	digitalWrite(header->D7,(byte & 0x08));
	digitalWrite(header->D6,(byte & 0x04));
	digitalWrite(header->D5,(byte & 0x02));
	digitalWrite(header->D4,(byte & 0x01));

	pulse(header);
}
