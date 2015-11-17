#ifndef I2CLCD_H
#define I2CLCD_H

//Lcd instructions
//#define LCD_CLEARDISPLAY   0x01
//#define LCD_RETURNHOME     0x02
//#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
//#define LCD_CURSORSHIFT    0x10
//#define LCD_FUNCTIONSET    0x20
//#define LCD_CGRAMADDRESS   0x40
//#define LCD_DDRAMADDRESS   0x80

//display entry mode flags
//#define LCD_SHIFTENTRY     0x01 //the DDRAM address is incremented upon DDRAM write/read
//#define LCD_NOSHIFTENTRY   0x00 //the DDRAM address is decremented upon DDRAM write/read
//#define LCD_ENTRYDECREMENT 0x00 //the display is not shifted
//#define LCD_ENTRYINCREMENT 0x02 //the the display is shifted left if LCD_SHIFTENTRY is set or right if LCD_NOSHIFTENTRY is set

//display control flags
#define LCD_DISPLAYON      0x04
//#define LCD_DISPLAYOFF     0x00
//#define LCD_HIDECURSOR     0x00
//#define LCD_SHOWCURSOR     0x02
//#define LCD_CURSORBLINKON  0x01
//#define LCD_CURSORBLINKOFF 0x00

//cursor/display flags
//#define LCD_MOVECURSOR     0x00
//#define LCD_SHIFTDISPLAY   0x08
//#define LCD_LEFT           0x00
//#define LCD_RIGHT          0x04

//function set flags
//#define LCD_5x10FONT       0x04
//#define LCD_5x8FONT        0x00
//#define LCD_LINES          0x08
//#define LCD_LINE           0x00
//#define LCD_8BIT           0x10
//#define LCD_4BIT           0x00

// mcp23s17 GPIOB to HD44780 pin map
//#define HIGH 1
//#define LOW 0

//#define PIN_D4 0
//#define PIN_D5 1
//#define PIN_D6 2
//#define PIN_D7 3
#define PIN_ENABLE 4
//#define PIN_RW 5
#define PIN_RS 6
#define PIN_BACKLIGHT 7

//#define CMD_WRITE 0x40
//#define CMD_READ  0x41

#define I2C_ADDR 0x20
//#define IODIRA   0x00
#define IODIRB   0x01
//#define IPOLA    0x02
//#define IPOLB    0x03
//#define GPINTENA 0x04
//#define GPINTENB 0x05
//#define DEFVALA  0x06
//#define DEFVALB  0x07
//#define INTCONA  0x08
//#define INTCONB  0x09
//#define IOCONA   0x0A
//#define IOCONB   0x0B
//#define GPPUA    0x0C
//#define GPPUB    0x0D
//#define INTFA    0x0E
//#define INTFB    0x0F
//#define INTCAPA  0x10
//#define INTCAPB  0x11
//#define GPIOA    0x12
//#define GPIOB    0x13
//#define OLATA    0x14
#define OLATB    0x15

int gpio_reg;
int i2c_fd;

//typedef enum{FALSE,TRUE} bool;
//typedef enum {CURSOR_LEFT,CURSOR_RIGHT,CURSOR_HOME} cursorMovement;
//typedef enum {DISPLAY_SCROLLEFT,DISPLAY_SCROLLRIGHT} displayScroll;
//typedef enum {LCD_COMMAND_MODE, LCD_CHARACTER_MODE} modes;

//void setDefaultHd44780(hd44780 * toDefault);
void initializeDisplay(hd44780 * header);
void writeBytes(hd44780 * header, int byte, int mode);

#endif
