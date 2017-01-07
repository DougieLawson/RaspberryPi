/*
Copyright &copy; Dougie Lawson 2015-2017, all rights reserved.
*/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "hd44780.h"
#include "commonLcd.h"
#ifdef gpio
#include "gpioLcd.h"
#endif
#ifdef pcf
#include "pcfLcd.h"
#endif
#ifdef i2c
#include "i2cLcd.h"
#endif
#ifdef SPI
#include "spiLcd.h"
#endif
#include "ipLcd.h"

hd44780 header;
struct sigaction act;

void sig_handler(int signum, siginfo_t *info, void *ptr)
{
  initialiseDisplay(&header);
  clearDisplay(&header);
  moveCursor(&header,CURSOR_HOME);
  cursorControl(&header,0);
  cursorBlink(&header,0);

}

int main()
{
  memset(&act, 0, sizeof(act));

  act.sa_sigaction = sig_handler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGUSR1, &act, NULL);
  sigaction(SIGUSR2, &act, NULL);

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
