/*
Copyright (C) Dougie Lawson 2017, all rights reserved.
*/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "hd44780.h"
#include "commonLcd.h"
#include "pcfLcd.h"
#include "get31855.h"

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
    char *tempRead = get31855();
    printString(&header, tempRead);
    free(tempRead);
    sleep(2);
  }

  return 0;
}
