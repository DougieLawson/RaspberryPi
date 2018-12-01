// (C) Copyright 2016, Dougie Lawson. All rights reserved.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  FILE *fp;
  const char *delim = " ";
  char lightordark[40];
  char *stringp = lightordark;
  char *token;

  char filename[21] = "/var/run/lightordark";

  fp = fopen(filename,"r");
  fgets(lightordark, sizeof lightordark, fp); 
  token = strsep(&stringp, delim);
  printf("LoD: %s", token);

  if (strcmp(token, "dark") == 0) {
    printf("Dark Token: %s\r\n", token);
  }
  else
  {
    printf("Light Token: %s\r\n", token);
  }

  token = strsep(&stringp, delim);
  token = strsep(&stringp, delim);
  printf("Time: %s\r\n", token);

  fclose(fp);
  return 0;
}
