/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#ifndef COMMONLCD_H
#define COMMONLCD_H

//Lcd instructions
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_CGRAMADDRESS 0x40
#define LCD_DDRAMADDRESS 0x80

//display control flags
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_HIDECURSOR 0x00
#define LCD_SHOWCURSOR 0x02
#define LCD_CURSORBLINKON 0x01
#define LCD_CURSORBLINKOFF 0x00

//cursor/display flags
#define LCD_MOVECURSOR 0x00
#define LCD_SHIFTDISPLAY 0x08
#define LCD_LEFT 0x00
#define LCD_RIGHT 0x04

typedef enum {CURSOR_LEFT,CURSOR_RIGHT,CURSOR_HOME} cursorMovement;
typedef enum {DISPLAY_SCROLLEFT,DISPLAY_SCROLLRIGHT} displayScroll;
typedef enum {LCD_COMMAND_MODE, LCD_CHARACTER_MODE} modes;

void byte_to_binary(int x);
void moveCursor(hd44780 * header, cursorMovement movement);
void scrollDisplay(hd44780 * header, displayScroll scroll);
void printString(hd44780 * header, char * string);
void clearDisplay(hd44780 * header);
void cursorControl(hd44780 * header, int state);
void cursorBlink(hd44780 * header, int state);
void printInt32(hd44780 * header, int val);
void defineCGChars (hd44780 * header, char array[8][8]);

#endif
