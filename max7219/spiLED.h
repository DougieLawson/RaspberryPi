#ifndef SPILED_H
#define SPILED_H

#define MAX7219_REG_NOOP 00x0
#define MAX7219_REG_DIGIT0 0x01
#define MAX7219_REG_DIGIT1 0x02
#define MAX7219_REG_DIGIT2 0x03
#define MAX7219_REG_DIGIT3 0x04
#define MAX7219_REG_DIGIT4 0x05
#define MAX7219_REG_DIGIT5 0x06
#define MAX7219_REG_DIGIT6 0x07
#define MAX7219_REG_DIGIT7 0x08
#define MAX7219_REG_DECODEMODE 0x09
#define MAX7219_REG_INTENSITY 0x0A
#define MAX7219_REG_SCANLIMIT 0x0B
#define MAX7219_REG_SHUTDOWN 0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

#define CMD_WRITE 0x40

int spi_fd;
void initialiseDisplay(max7219 * header);
void writeBytes(max7219 * header, int cmd, int data);
void writeDigits(max7219 * header, char chars[12]);
void clearDisplay(max7219 * header);
void writeByte(uint8_t reg, uint8_t data);
uint8_t asciiToBCD (uint8_t chr);
void byte_to_binary(int x);
void digitDisplay(max7219 * header);

#endif
