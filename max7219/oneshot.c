// (C) Copyright 2017, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "max7219.h"
#include "spiLED.h"

max7219 header;

int main()
{

  char chars[12] = "12345678";
  initialiseDisplay(&header);
  clearDisplay(&header);
  writeDigits(&header, chars);
  
  return 0;
}
