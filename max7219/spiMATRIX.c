// (C) Copyright 2018, Dougie Lawson. All rights reserved.
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "max7219.h"
#include "spiMATRIX.h"

static char *spiDevice = "/dev/spidev0.1";
static uint8_t spiBPW = 8;
static uint32_t spiSpeed = 5000000;
static uint16_t spiDelay = 0;

int spiOpen(char* dev)
{
  if((spi_fd = open(dev, O_RDWR)) < 0)
  {
    printf("error opening %s\n",dev);
    return -1;
  }
  return 0;
}

void writeByte(uint8_t reg, uint8_t data)
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

void initialiseDisplay(max7219 *header)
{
  spiOpen(spiDevice);

  writeBytes(header, MAX7219_REG_SCANLIMIT, 7);
  writeBytes(header, MAX7219_REG_DECODEMODE, 0x00);
  writeBytes(header, MAX7219_REG_SHUTDOWN, 1);
  writeBytes(header, MAX7219_REG_DISPLAYTEST, 0);
  setBrightness(header, 5);
}

void writeBytes(max7219 *header, int cmd, int data)
{
  if(!header) return;
  writeByte(cmd, data);
}

void byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }
    printf("%s", b);
}

void clearDisplay(max7219 *header)
{
  int i;
  if(!header) return; 
  for (i=0; i<8; i++) {
    header->digits[i] = 0x0F;
  }
  digitDisplay(header);
}

void digitDisplay(max7219 *header)
{
  int i;
  if(!header) return;
  for (i=0; i<8; i++)
  {
    writeBytes(header, i+1, header->digits[i]);
  }
}

void setBrightness(max7219 *header, int bright)
{
  if(!header) return;
  if (bright < 0) bright = 0;
  if (bright > 15) bright = 15;
 
  writeBytes(header, MAX7219_REG_INTENSITY, bright);
}

void writeDigits(max7219 *header, char chars[12])
{
  int i;
  uint8_t bcd_value;
  if (!header) return;
  for (i =0;i < 8;i++)
  {
	  bcd_value = (chars[i] - 48);
	  if (bcd_value > 9) bcd_value = 0;
	  header->digits[i] = bcd_value;
  }
  digitDisplay(header);
}
