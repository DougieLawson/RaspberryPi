#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "max7219.h"
#include "spiLED.h"

static char *spiDevice = "/dev/spidev0.0";
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

void initialiseDisplay(max7219 * header)
{
  spiOpen(spiDevice);

  writeBytes(header, MAX7219_REG_SCANLIMIT, 7);
  writeBytes(header, MAX7219_REG_DECODEMODE, 0xff);
  writeBytes(header, MAX7219_REG_SHUTDOWN, 1);
  writeBytes(header, MAX7219_REG_DISPLAYTEST, 0);
}

void writeBytes(max7219 * header, int cmd, int data)
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

void clearDisplay(max7219 * header)
{
  int i;
  if(!header) return; 
  for (i=0; i<8; i++) {
    header->digits[i] = 0x0F;
  }
  digitDisplay(header);
}

void writeDigits(max7219 * header, char chars[12])
{
  int i,j;
  if (!header) return;
  i = 7;
  j = 0;
  while (j <= 11 && i >= 0)
  {
    if ((int)chars[j] >= 0x30 && (int)chars[j] <= 0x39 )
    {
      // chars[j] between 0 and 9
      header->digits[i] = asciiToBCD(chars[j]);
      j++;
      i--;
    }
    else if ((int)chars[j] == 0x20 ) 
    {
      // chars[j] is space
      header->digits[i] = 0x0F;
      j++;
      i--;
    }
    else if ((int)chars[j] == 0x2d)
    {
      // chars[j] is minus (-)
      header->digits[i] = 0x0a;
      j++;
      i--;
    }
    else if ((int)chars[j] == 0x3a)
    {
      // chars[j] is colon (:)
      // set it to space + DP
      header->digits[i] = 0x8f;
      j++;
      i--;
    } 
    else if ((int)chars[j] == 0x2e)
    {
      // chars[j] is full stop (.)
      // so activate the DP (bit 7) on left hand digit
      header->digits[i+1] = header->digits[i+1] + 0x80;
      j++;
      // digits index isn't incremented
    }
    else if ((int)chars[j] == 0x00)
    {
      // chars[j] is a NULL
      // That's it we're done, set both indices so we terminate the loop
      j = 12;
      i = -1;
    }
    else 
    {
      j++;
    }
  }

  digitDisplay(header);
}

void digitDisplay(max7219 * header)
{
  int i;
  if(!header) return;
  for (i=0; i<8; i++)
  {
    writeBytes(header, i+1, header->digits[i]);
  }

}

uint8_t asciiToBCD (uint8_t chr) 
{
  uint8_t bcd_value;
  bcd_value = (chr - 48);
  return bcd_value;
}
