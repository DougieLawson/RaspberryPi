#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "hd44780.h"
#include "commonLcd.h"
#include "i2cLcd.h"
#include "ipLcd.h"

int main()
{

  hd44780 header;
  setDefaultHd44780(&header);
  initialiseDisplay(&header);
  clearDisplay(&header);
  moveCursor(&header,CURSOR_HOME);
  cursorControl(&header,0);
  cursorBlink(&header,0);
  
  while (1)
  {
    clearDisplay(&header);
    char *ifaceIP = getIPaddr();
    printString(&header, ifaceIP);
    free(ifaceIP);
    sleep(2);
  }

  return 0;
}
