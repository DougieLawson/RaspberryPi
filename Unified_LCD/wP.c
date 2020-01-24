/*
 *  Mock wiringPi.c
 *  Copyright (C) Dougie Lawson 2020, all rights reserved.
 */
#include "wP.h"
#include <time.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

unsigned address;
unsigned size;
static volatile uint32_t *gpioReg = MAP_FAILED;

static unsigned get_dt_ranges(unsigned offset)
{
   unsigned addr8 = ~0;
   FILE *fp = fopen("/proc/device-tree/soc/ranges", "rb");
   if (fp)
   {
      unsigned char buf[4];
      fseek(fp, offset, SEEK_SET);
      if (fread(buf, 1, sizeof buf, fp) == sizeof buf)
         addr8 = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0;
      fclose(fp);
   }
   return addr8;
}

void delay (unsigned int howLong)
{
	struct timespec sleeper, dummy;
	sleeper.tv_sec = (time_t) (howLong / 1000);
	sleeper.tv_nsec = (long) (howLong % 1000) * 1000000;

	nanosleep (&sleeper, &dummy);
}

int  wiringPiSetupGpio (void)
{
	int fd;
	unsigned gpioBase;
	address = get_dt_ranges(4);
	if (address == 0) 
       	address = get_dt_ranges(8); 
	address == ~0 ? 0x20000000 : address;
	size = get_dt_ranges(4);
	size = get_dt_ranges((size == 0) ? 12 : 0);
	size == ~0 ? 0x01000000 : size;

	//Testing only
	//printf("address: %08x size: %08x \r\n", address, size);
	
	gpioBase = address + 0x00200000;
	fd = open("/dev/gpiomem", O_RDWR | O_SYNC | O_CLOEXEC);
	if (fd < 0)
	{
		fprintf(stderr, "failed to open /dev/gpiomem\n");
		return -1;
	}
	gpioReg = (uint32_t *)mmap(NULL, 0xB4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, gpioBase);
	close(fd);
	if (gpioReg == MAP_FAILED)
	{
		fprintf(stderr, "Bad mmap failed\n");
		return -1;
	}
	return 0;

}

void pinMode (int pin, int mode)
{
	int reg, shift;
	reg = pin/10;
	shift = (pin%10) * 3;

	// Testing only
	// printf("pinMode called: %08x, %08x reg %d shift %d\r\n", pin, mode, reg, shift);
	gpioReg[reg] = (gpioReg[reg] & ~(7<<shift)) | (mode<<shift);
}

void digitalWrite (int pin, int value)
{
	// Testing only
	//printf("digitalWrite called: %08x, %08x gpioReg %08x %08x\r\n", pin, value, *(gpioReg + GPCLR0 + PI_BANK), *(gpioReg + GPSET0 + PI_BANK));
	if (value == 0) *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
	else            *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
}

