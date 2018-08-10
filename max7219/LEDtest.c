// (C) Copyright 2017, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "max7219.h"
#include "spiLED.h"

max7219 header;
struct sigaction act;

void sig_handler(int signum, siginfo_t *info, void *ptr)
{
  clearDisplay(&header);
  printf("Received signal %d\n", signum);
  printf("Signal originated from process %lu\n",
    (unsigned long)info->si_pid);
  exit(0);
}

int main()
{
  memset(&act, 0, sizeof(act));

  act.sa_sigaction = sig_handler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGTERM, &act, NULL);

  char chars[12] = "12:34:56";
  initialiseDisplay(&header);
  clearDisplay(&header);
  
  while (1)
  {

    clearDisplay(&header);
    writeDigits(&header, chars);
    sleep(1);
  }

  return 0;
}
