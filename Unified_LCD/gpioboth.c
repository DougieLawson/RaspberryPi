/*
Copyright (C) Dougie Lawson 2017, 2020, all rights reserved.
*/

#include "wP.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "hd44780.h"
#include "commonLcd.h"
#include "gpioLcd.h"
#include "get31855.h"
#include "get6675.h"

hd44780 header;
struct sigaction act;

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
  int ret_tod = 0;
  char tempRead[40];

  memset (&act, 0, sizeof (act));
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
      clearDisplay (&header);
      char *t31855rd = get31855 (ret_tod);
      char *t6675rd = get6675 (ret_tod);
      sprintf(tempRead, "%s\n%s", t31855rd, t6675rd);
      printString (&header, tempRead);
      free (t31855rd);
      free (t6675rd);
      sleep (2);
    }

  return 0;
}
