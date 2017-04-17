/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "hd44780.h"
#include "commonLcd.h"
#ifdef SPI
#include "spiLcd.h"
#endif
#ifdef pcf
#include "pcfLcd.h"
#endif
#ifdef i2c
#include "i2cLcd.h"
#endif
#ifdef gpio
#include "gpioLcd.h"
#endif

void byte_to_binary(int x) {
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
    printf("%s", b);
}

void moveCursor(hd44780 * header, cursorMovement movement)
{
	if(!header)
		return;

	if(movement == CURSOR_RIGHT)
		writeBytes(header, 0b00010100, LCD_COMMAND_MODE);
	else if(movement == CURSOR_LEFT)
		writeBytes(header, 0b00010000, LCD_COMMAND_MODE);
	else 
		writeBytes(header, 0b00000010, LCD_COMMAND_MODE);

}

void printString(hd44780 * header, char * string)
{
	if(!header || !string ||!*string)
		return; 

	int positionInLine = 0;

    int i;	
	for(i = 0; string[i] != '\0' ;i++)
	{
		if(positionInLine == header->rowNumber)
		{
		    writeBytes(header, LCD_DDRAMADDRESS | 0xC0, LCD_COMMAND_MODE); //jump to next line
		    positionInLine = 0;
		}

		if(string[i] == '\n' )
		{
			writeBytes(header, LCD_DDRAMADDRESS | 0xC0, LCD_COMMAND_MODE);
			positionInLine = 0;
		}
		else
		{
			writeBytes(header, string[i], LCD_CHARACTER_MODE);
			positionInLine = positionInLine + 1;
		}
	}

}

void scrollDisplay(hd44780 * header, displayScroll scroll)
{
	if(!header)
		return;

	if(scroll == DISPLAY_SCROLLEFT)
	{
		writeBytes(header,LCD_CURSORSHIFT | LCD_SHIFTDISPLAY | LCD_LEFT , LCD_COMMAND_MODE);
	}
	else if(scroll == DISPLAY_SCROLLRIGHT)
	{
		writeBytes(header,LCD_CURSORSHIFT | LCD_SHIFTDISPLAY | LCD_RIGHT , LCD_COMMAND_MODE);
	}
}

void clearDisplay(hd44780 * header)
{
	if(!header)
		return;
	writeBytes(header, LCD_CLEARDISPLAY, LCD_COMMAND_MODE);
}

void cursorControl(hd44780 * header, int state)
{
	if(!header)
		return;
	if (state)
        	header->displayControl = (header->displayControl | LCD_DISPLAYON | LCD_DISPLAYCONTROL | LCD_SHOWCURSOR);
	else
		header->displayControl = (header->displayControl | LCD_DISPLAYON | LCD_DISPLAYCONTROL | LCD_HIDECURSOR) & ~LCD_SHOWCURSOR;  
	writeBytes(header, header->displayControl, LCD_COMMAND_MODE);
}

void cursorBlink(hd44780 * header, int state)
{
	if(!header)
		return;
	if (state)
	        header->displayControl = (header->displayControl | LCD_DISPLAYON | LCD_DISPLAYCONTROL | LCD_CURSORBLINKON);
	else
        	header->displayControl = (header->displayControl | LCD_DISPLAYON| LCD_DISPLAYCONTROL | LCD_CURSORBLINKOFF) & ~LCD_CURSORBLINKON;
	writeBytes(header, header->displayControl, LCD_COMMAND_MODE);
}

void printInt32(hd44780 * header, int val)
{
	if(!header)
		return;

	int i;
	for(i = 0; i < 32; i++)
	{
		if(i == header->rowNumber)
		    writeBytes(header, LCD_DDRAMADDRESS | 0xC0, LCD_COMMAND_MODE); //jump to next line

		if((val>>i) & 0x01)
			writeBytes(header, 0x31, LCD_CHARACTER_MODE);
		else
			writeBytes(header, 0x30, LCD_CHARACTER_MODE);

	}
}

void defineCGChars (hd44780 * header, char array[8][8])
{
	char CGbyte;
	int i, j;
	for (i=0; i<8; i++)
	{
		CGbyte = LCD_CGRAMADDRESS | (i << 3);
		writeBytes(header, CGbyte , LCD_COMMAND_MODE);
		for (j=0; j<8; j++)
		{
			writeBytes(header, array[i][j], LCD_CHARACTER_MODE);
		}
	}
}
