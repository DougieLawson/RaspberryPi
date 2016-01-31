// (C) Copyright 2016, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "max7219.h"
#include "spiLED.h"

int main()
{
  char chars[12] = "22:01:02";
  max7219 header;
  initialiseDisplay(&header);
  clearDisplay(&header);
  
  while (1)
  {
    clearDisplay(&header);
    sleep(2);
    writeDigits(&header, chars);
    sleep(2);
  }

  return 0;
}
