/*
Copyright (C) Dougie Lawson 2015-2017, 2019 all rights reserved.
*/

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/ioctl.h>
#include "linux/i2c-dev.h"
#include "i2c/smbus.h"
#include "hd44780.h"
#include "commonLcd.h"
#include "pcfLcd.h"

static char *i2cDevice = "/dev/i2c-1";
int
i2c_open (char *dev)
{
  if ((i2c_fd = open (dev, O_RDWR)) < 0)
    {
      printf ("error opening %s\n", dev);
      return -1;
    }
  if (ioctl (i2c_fd, I2C_SLAVE, PCF_ADDR) < 0)
    {
      printf ("Failed to set slave address");
      return -2;
    }
  return 0;
}

void
delay (unsigned int howLong)
{
  struct timespec sleeper, dummy;

  sleeper.tv_sec = (time_t) (howLong / 1000);
  sleeper.tv_nsec = (long) (howLong % 1000) * 1000000;

  nanosleep (&sleeper, &dummy);
}

static void
writeByte (uint8_t data)
{
  i2c_smbus_write_byte (i2c_fd, data);
}

static void
pulse (hd44780 * header)
{
  delay (1);
  gpio_reg &= (0xff - (1 << PIN_ENABLE));
  writeByte (gpio_reg);
  delay (1);
  gpio_reg |= 1 << PIN_ENABLE;
  writeByte (gpio_reg);
  delay (1);
  gpio_reg &= (0xff - (1 << PIN_ENABLE));
  writeByte (gpio_reg);
  delay (1);

}

void
setDefaultHd44780 (hd44780 * header)
{
  return;
}

void
initialiseDisplay (hd44780 * header)
{
  header->colNumber = 2;
  header->rowNumber = 16;
  i2c_open (i2cDevice);
  gpio_reg |= 1 << PIN_BACKLIGHT;
  writeByte (gpio_reg);

  writeBytes (header, 0b00110011, LCD_COMMAND_MODE);
  writeBytes (header, 0b00110000, LCD_COMMAND_MODE);
  writeBytes (header, 0b00000110, LCD_COMMAND_MODE);

  header->displayControl =
    (header->displayControl | LCD_DISPLAYCONTROL) | LCD_DISPLAYON;
  writeBytes (header, header->displayControl, LCD_COMMAND_MODE);
}

void
writeBytes (hd44780 * header, int byte, int mode)
{
  if (!header)
    return;
  gpio_reg |= mode << PIN_RS;
  writeByte (gpio_reg);
  delay (1);
  gpio_reg = gpio_reg | (byte >> 4) << 4;
  pulse (header);
  gpio_reg = gpio_reg & (0xff - (0xf0));
  gpio_reg = gpio_reg | (byte & 0x0f) << 4;
  pulse (header);
  gpio_reg = gpio_reg & (0xff - (1 << PIN_RS) - (0xf0));
}
