/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <time.h>
#include <stdlib.h>
#include "get31855.h"
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

static const char *device = "/dev/spidev0.0";
static uint8_t mode = SPI_MODE_0;
static uint8_t bits = 8;
static uint32_t speed = 1000000;
static uint16_t delay = 0;


static void
transfer (int fd, char *rd31855)
{
  int ret;
  uint8_t wr_buf[] = { 0x00, 0x00, 0x00, 0x00 };
  uint8_t rd_buf[4];
  struct spi_ioc_transfer tr = {
    .tx_buf = (unsigned long) wr_buf,
    .rx_buf = (unsigned long) rd_buf,
    .len = ARRAY_SIZE (wr_buf),
    .delay_usecs = delay,
    .speed_hz = speed,
    .bits_per_word = bits,
  };

  time_t rawtime;
  struct tm *timeinfo;
  char time_of_day[16];
  time (&rawtime);
  timeinfo = localtime (&rawtime);

  if (fd <= 0)
    {
      sprintf (rd31855, "Device %s not found\n", device);
      return;
    }

  if ((ret = ioctl (fd, SPI_IOC_WR_MODE, &mode)) == -1)
    {
      sprintf (rd31855, "Can't set mode");
      return;
    }

  if ((ret = ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &bits)) == -1)
    {
      sprintf (rd31855, "Can't set number of bits");
      return;
    }

  if ((ret = ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed)) == -1)
    {
      sprintf (rd31855, "Can't set write speed");
      return;
    }

  if ((ret = ioctl (fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed)) == -1)
    {
      sprintf (rd31855, "Can't set read speed");
      return;
    }

  ret = ioctl (fd, SPI_IOC_MESSAGE (1), &tr);
  if (ret == 1)
    {
      sprintf (rd31855, "Can't send API message");
      return;
    }

  strftime (time_of_day, 16, "%T %b,%d", timeinfo);
  sprintf (rd31855, "%s\n%.4f %.4f", time_of_day,
	   (float) ((rd_buf[0] << 8) + rd_buf[1]) / 16,
	   (float) ((rd_buf[2] << 8) + rd_buf[3]) / 256);

}

char *
get31855 ()
{
  char *rd31855 = malloc (40);
  int fd;

  fd = open (device, O_RDWR);
  transfer (fd, rd31855);
  close (fd);
  return rd31855;
}
