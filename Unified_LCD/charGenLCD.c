/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hd44780.h"
#include "commonLcd.h"
#ifdef pcf
#include "pcfLcd.h"
#endif
#ifdef i2c
#include "i2cLcd.h"
#endif
#ifdef SPI
#include "spiLcd.h"
#endif
#ifdef gpio
#include "gpioLcd.h"
#endif

char customChars[8][8] = {
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
{ 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
{ 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 }, 
{ 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c }, 
{ 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e, 0x1e },
{ 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
};

char displayStr[40];
int i,j;
int main()
{
  char *jString = malloc(40);

  hd44780 header;
  setDefaultHd44780(&header);
  initialiseDisplay(&header);
  clearDisplay(&header);
  moveCursor(&header,CURSOR_HOME);
  cursorControl(&header,0);
  cursorBlink(&header,0);
  defineCGChars (&header, customChars);
 
  for (i=0; i<40; i++) 
  {
    for (j=1; j<6; j++)
    {
      sprintf(jString, "%c", j);
      displayStr[i] = jString[0];
      clearDisplay(&header);
      printString(&header, displayStr);
      sleep(2);
    }
  }

  return 0;
}
