/*
Copyright (C) Dougie Lawson 2015-2017, 2019 all rights reserved.
*/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "hd44780.h"
#include "commonLcd.h"
#ifdef gpio
#include "gpioLcd.h"
#include "wiringPi.h"
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
time_t rawtime;
struct tm *timeinfo;
char time_of_day[16];
int offset = 0;
int len;
char fortySP[41];

void
sig_handler (int signum, siginfo_t * info, void *ptr)
{
  initialiseDisplay (&header);
  clearDisplay (&header);
  moveCursor (&header, CURSOR_HOME);
  cursorControl (&header, 0);
  cursorBlink (&header, 0);

}

int
main ()
{
  memset (&act, 0, sizeof (act));

  strcpy(fortySP, "                                        ");

  act.sa_sigaction = sig_handler;
  act.sa_flags = SA_SIGINFO;
  sigaction (SIGUSR1, &act, NULL);
  sigaction (SIGUSR2, &act, NULL);

  setDefaultHd44780 (&header);
  initialiseDisplay (&header);
  clearDisplay (&header);
  moveCursor (&header, CURSOR_HOME);
  cursorControl (&header, 0);
  cursorBlink (&header, 0);

  while (1)
    {

      time (&rawtime);
      timeinfo = localtime (&rawtime);

      strftime (time_of_day, 16, "%T %b,%d\n", timeinfo);

      clearDisplay (&header);
      char *ifaceIP = getIPaddr ();
      len = strlen(ifaceIP);
      ifaceIP[len+1] = '\0';
      moveCursor (&header, CURSOR_HOME);
      printString (&header, time_of_day);
      printString (&header, "\n");
      printString (&header, ifaceIP);
      if (len > 16) 
      {
        len=len-16;
        for (offset = 0; offset <= len; offset++)
        {
          delay(80);
          setPrintPosn (&header, 40); // move to start of second line
          printString (&header, fortySP);
          setPrintPosn (&header, 40); // move to start of second line
          printString (&header, &ifaceIP[offset]);
        }
      }
      free (ifaceIP);
      sleep (2);
    }

  return 0;
}
