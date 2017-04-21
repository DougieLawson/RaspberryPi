/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#ifndef HD44780_H
#define HD44780_H
typedef struct
{
  int registerSelect;
  int enable;

  int D4;
  int D5;
  int D6;
  int D7;

  int colNumber;
  int rowNumber;
  int displayControl;
} hd44780;
#endif
