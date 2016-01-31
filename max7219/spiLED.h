#ifndef SPILED_H
#define SPILED_H

#define CMD_WRITE 0x40

int spi_fd;
void initialiseDisplay(max7219 *header);
void writeBytes(max7219 *header, int cmd, int data);
void writeDigits(max7219 *header, char chars[12]);
void clearDisplay(max7219 *header);
void writeByte(uint8_t reg, uint8_t data);
uint8_t asciiToBCD (uint8_t chr);
void byte_to_binary(int x);
void digitDisplay(max7219 *header);
void setBrightness(max7219 *header, int bright);

#endif
