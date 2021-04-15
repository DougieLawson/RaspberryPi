// (C) Copyright 2018, Dougie Lawson. All rights reserved.
#ifndef SPIMATRIX_H
#define SPIMATRIX_H

#define CMD_WRITE 0x40

int spi_fd;

void writeByte(uint8_t reg, uint8_t data);
void initialiseDisplay(max7219 *header);
void writeBytes(max7219 *header, int cmd, int data);
void byte_to_binary(int x);
void clearDisplay(max7219 *header);
void digitDisplay(max7219 *header);
void setBrightness(max7219 *header, int bright);
void writeDigits(max7219 *header, char chars[12]);

#endif
