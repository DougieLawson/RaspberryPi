/*
Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
*/

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "hd44780.h"
#include "commonLcd.h"
#include "spiLcd.h"

static char *spiDevice = "/dev/spidev0.1";
static uint8_t spiBPW = 8;
static uint32_t spiSpeed = 5000000;
static uint16_t spiDelay = 0;

int spi_open(char* dev)
{
  if((spi_fd = open(dev, O_RDWR)) < 0)
  {
    printf("error opening %s\n",dev);
    return -1;
  }
  return 0;
}

void delay (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}

static void writeByte(uint8_t reg, uint8_t data)
{
  uint8_t spiBufTx [3];
  uint8_t spiBufRx [3];
  struct spi_ioc_transfer spi;
  memset (&spi, 0, sizeof(spi));
  spiBufTx [0] = CMD_WRITE;
  spiBufTx [1] = reg;
  spiBufTx [2] = data;
  spi.tx_buf =(unsigned long)spiBufTx;
  spi.rx_buf =(unsigned long)spiBufRx;
  spi.len = 3;
  spi.delay_usecs = spiDelay;
  spi.speed_hz = spiSpeed;
  spi.bits_per_word = spiBPW;
  ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
}

static void pulse(hd44780 * header)
{
	delay(1);
	gpio_reg &= (0xff -(1 << PIN_ENABLE));
        writeByte(OLATB, gpio_reg);
	delay(1);
	gpio_reg |= 1 << PIN_ENABLE;
        writeByte(OLATB, gpio_reg);
	delay(1);
	gpio_reg &= (0xff -(1 << PIN_ENABLE));
        writeByte(OLATB, gpio_reg);
	delay(1);

}

void setDefaultHd44780(hd44780 * header)
{
	return;
}

void initialiseDisplay(hd44780 * header)
{
	header->colNumber = 2;
	header->rowNumber = 16;
	spi_open(spiDevice);
        writeByte(IODIRB,0x00);
	gpio_reg |= 1 << PIN_BACKLIGHT;
        writeByte(OLATB, gpio_reg);

	writeBytes(header, 0b00110011, LCD_COMMAND_MODE);
	writeBytes(header, 0b00110000, LCD_COMMAND_MODE);
	writeBytes(header, 0b00000110, LCD_COMMAND_MODE);

	header->displayControl = (header->displayControl | LCD_DISPLAYCONTROL) | LCD_DISPLAYON;
	writeBytes(header, header->displayControl, LCD_COMMAND_MODE);
}

void writeBytes(hd44780 * header, int byte, int mode)
{
	if(!header)
		return;

	gpio_reg |= mode << PIN_RS;
	writeByte(OLATB, gpio_reg);
	delay(1);

	gpio_reg = gpio_reg | (byte >> 4);
	pulse(header);
        gpio_reg = gpio_reg & (0xff -(0x0f));
	gpio_reg = gpio_reg | (byte & 0x0f);
	pulse(header);
        gpio_reg = gpio_reg & (0xff -(1 << PIN_RS) -(0x0f));
}
