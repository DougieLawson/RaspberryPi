// (C) Copyright 2017, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "max7219.h"
#include "spiLED.h"
#include <time.h>

max7219 header;
struct sigaction act;

void sig_handler(int signum, siginfo_t *info, void *ptr)
{
//clearDisplay(&header);
  int i;
  for (i=0; i<8; i++) {
    header.digits[i] = 0;
  }

  digitDisplay(&header);

  printf("Received signal %d\n", signum);
  printf("Signal originated from process %lu\n",
    (unsigned long)info->si_pid);
  exit(0);
}

int main()
{

  FILE *fp;
  int bright;
  char lightordark[40];
  char *LoDstringp = lightordark;
  char *LoDtoken;
  char *LoDtime;
 
  const char filename[21] = "/var/run/lightordark";
  const char *delim = " ";

  memset(&act, 0, sizeof(act));

  act.sa_sigaction = sig_handler;
  act.sa_flags = SA_SIGINFO;
  sigaction(SIGTERM, &act, NULL);

  char chars[12];
  char hour_c[3];
  char min_c[3];
  char sec_c[3];

  int hour, min, sec;
  int hour_10, hour_1, min_10, min_1, sec_10, sec_1;

  initialiseDisplay(&header);

  writeBytes(&header, MAX7219_REG_DECODEMODE, 0x00);

  clearDisplay(&header);
  
  while (1)
  {

    time_t rawtime;
    struct tm * timeinfo;

    clearDisplay(&header);
    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (chars,12,"%T",timeinfo);
    strftime(hour_c,3,"%H",timeinfo);
    hour = atoi(hour_c);
    hour_10 = hour / 10;
    hour_1 = hour % 10;
    strftime(min_c,3,"%M",timeinfo);
    min = atoi(min_c);
    min_10 = min / 10;
    min_1 = min % 10;
    strftime(sec_c,3,"%S",timeinfo);
    sec = atoi(sec_c);
    sec_10 = sec / 10;
    sec_1 = sec % 10;

    header.digits[7] = hour_10 << 2;
    header.digits[6] = hour_1 << 2;
    header.digits[5] = 0;
    header.digits[4] = min_10 << 2;
    header.digits[3] = min_1 << 2;
    header.digits[2] = 0;
    header.digits[1] = sec_10 << 2;
    header.digits[0] = sec_1 << 2;

    /* Set the display brightness at dawn / dusk */
    fp = fopen(filename,"r");
    fgets(lightordark, sizeof lightordark, fp); 
    LoDtoken = strsep(&LoDstringp, delim);
    LoDtime = strsep(&LoDstringp, delim); /* skip @ */
    LoDtime = strsep(&LoDstringp, delim); /* got Time */

    if (strcmp(LoDtoken, "dark") == 0) bright = 7;
    else bright = 3;

    setBrightness(&header, bright);
    fclose(fp);
    LoDstringp = lightordark;

    digitDisplay(&header);
    sleep(1);
  }

  return 0;
}
