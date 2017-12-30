using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace I2C_LCD
{
    class Program
    {
        private static readonly int PIN_BACKLIGHT = 3;
        private static readonly int PIN_ENABLE = 2;
        private static readonly int PIN_RS = 0;
        private static readonly int CURSOR_LEFT = 0;
        private static readonly int CURSOR_RIGHT = 1;
        private static readonly int CURSOR_HOME = 2;
        private static readonly int LCD_COMMAND_MODE = 0x00;
        private static readonly int LCD_CLEARDISPLAY = 0x01;
        private static readonly int LCD_CHARACTER_MODE = 0x01;
        private static readonly int LCD_DISPLAYCONTROL = 0x08;
        private static readonly int LCD_DISPLAYON = 0x04;
        private static readonly int LCD_SHOWCURSOR = 0x02;
        private static readonly int LCD_HIDECURSOR = 0x00;
        private static readonly int LCD_CURSORBLINKOFF = 0x00;
        private static readonly int LCD_CURSORBLINKON = 0x01;
        private static readonly int LCD_DDRAMADDRESS = 0x80;

        private static readonly int PCF_ADDR = 0x27;

        private static int displayControl;
        private static int gpio_reg;
        private static int colNumber;
        private static int rowNumber;

        static void Main(string[] args)
        {
            using (var bus = RPi.I2C.Net.I2CBus.Open("/dev/i2c-1"))
            {
                InitialiseLCD(bus);
                ClearDisplay(bus);
                MoveCursor(bus, CURSOR_HOME);
                CursorControl(bus, false);
                CursorBlink(bus, false);
                while (true)
                {

                    ClearDisplay(bus);
                    char[] text = new char[40];
                    text[0] = 'H';
		            text[1] = 'e';
		            text[2] = 'l';
		            text[3] = 'l';
		            text[4] = 'o';
		            text[5] = ' ';
		            text[6] = 'C';
		            text[7] = '#';
		            text[8] = ' ';
		            text[9] = 'M';
		            text[10] = 'o';
		            text[11] = 'n';
		            text[12] = 'o';
		            text[13] = '\0'; 
                    PrintString(bus, text);
		            Thread.Sleep(2000);
                }
            }
        }

        static void Pulse(RPi.I2C.Net.I2CBus bus)
        {
            Thread.Sleep(1);
            gpio_reg &= (0xff - (1 << PIN_ENABLE));
            bus.WriteByte(PCF_ADDR, (byte)gpio_reg);
            Thread.Sleep(1);
            gpio_reg |= 1 << PIN_ENABLE;
            bus.WriteByte(PCF_ADDR, (byte)gpio_reg);
            Thread.Sleep(1);
            gpio_reg &= (0xff - (1 << PIN_ENABLE));
            bus.WriteByte(PCF_ADDR, (byte)gpio_reg);
            Thread.Sleep(1);
        }

        static void WriteLCD(RPi.I2C.Net.I2CBus bus, int address, byte reg, int mode)
        {
            gpio_reg |= mode << PIN_RS;
            bus.WriteByte(PCF_ADDR, (byte)gpio_reg);
            Thread.Sleep(1);
            gpio_reg = gpio_reg | (reg >> 4) << 4;
            Pulse(bus);
            gpio_reg = gpio_reg & (0xff - (0xf0));
            gpio_reg = gpio_reg | (reg & 0x0f) << 4;
            Pulse(bus);
            gpio_reg = gpio_reg & (0xff - (1 << PIN_RS) - (0xf0));
        }

        static void InitialiseLCD(RPi.I2C.Net.I2CBus bus)
        {
            colNumber = 2;
            rowNumber = 16;
            gpio_reg = 0;

            gpio_reg = gpio_reg | (1 << PIN_BACKLIGHT);
            bus.WriteByte(PCF_ADDR, (byte)gpio_reg);

            WriteLCD(bus, PCF_ADDR, (byte)0x33, (byte)LCD_COMMAND_MODE);
            WriteLCD(bus, PCF_ADDR, (byte)0x30, (byte)LCD_COMMAND_MODE);
            WriteLCD(bus, PCF_ADDR, (byte)0x06, (byte)LCD_COMMAND_MODE);

            displayControl =
              (displayControl | LCD_DISPLAYCONTROL) | LCD_DISPLAYON;

            WriteLCD(bus, PCF_ADDR, (byte)displayControl, (byte)LCD_COMMAND_MODE);
        }

        static void ClearDisplay(RPi.I2C.Net.I2CBus bus)
        {
            WriteLCD(bus, PCF_ADDR, (byte)LCD_CLEARDISPLAY, (byte)LCD_COMMAND_MODE);
        }

        static void MoveCursor(RPi.I2C.Net.I2CBus bus, int movement)
        {
            if (movement == CURSOR_RIGHT)
                WriteLCD(bus, PCF_ADDR, (byte)0x14, (byte)LCD_COMMAND_MODE);
            else if (movement == CURSOR_LEFT)
                WriteLCD(bus, PCF_ADDR, (byte)0x10, (byte)LCD_COMMAND_MODE);
            else
                WriteLCD(bus, PCF_ADDR, (byte)0x02, (byte)LCD_COMMAND_MODE);
        }

        static void CursorControl(RPi.I2C.Net.I2CBus bus, bool state)
        {
            if (state)
                displayControl =
                  (displayControl | LCD_DISPLAYON | LCD_DISPLAYCONTROL | LCD_SHOWCURSOR);
            else
                displayControl =
                  (displayControl | LCD_DISPLAYON | LCD_DISPLAYCONTROL | LCD_HIDECURSOR) &
                  ~LCD_SHOWCURSOR;

            WriteLCD(bus, PCF_ADDR, (byte)displayControl, (byte)LCD_COMMAND_MODE);
        }

        static void CursorBlink(RPi.I2C.Net.I2CBus bus, bool state)
        {
            if (state)
                displayControl =
                  (displayControl | LCD_DISPLAYON | LCD_DISPLAYCONTROL | LCD_CURSORBLINKON);
            else
                displayControl =
                  (displayControl | LCD_DISPLAYON | LCD_DISPLAYCONTROL | LCD_CURSORBLINKOFF) & ~LCD_CURSORBLINKON;

            WriteLCD(bus, PCF_ADDR, (byte)displayControl, (byte)LCD_COMMAND_MODE);
        }

        static void PrintInt32(RPi.I2C.Net.I2CBus bus, int val)
        {
            int dram;
            int i;
            for (i = 0; i < 32; i++)
            {
                if (i == rowNumber)
                {
                    dram = 0xC0 | LCD_DDRAMADDRESS;
                    WriteLCD(bus, PCF_ADDR, (byte)dram, (byte)LCD_COMMAND_MODE); //jump to next line
                }
                if (((val >> i) & 0x01) == 1)
                    WriteLCD(bus, PCF_ADDR, (byte)0x31, (byte)LCD_CHARACTER_MODE);
                else
                    WriteLCD(bus, PCF_ADDR, (byte)0x30, (byte)LCD_CHARACTER_MODE);
            }
        }

        static void PrintString(RPi.I2C.Net.I2CBus bus, char[] text)
        {
            int positionInLine = 0;
            int dram;
            int i;
            for (i = 0; text[i] != '\0'; i++)
            {
                //      byte_to_binary(string[i]);
                if (positionInLine == rowNumber)
                {
                    dram = 0xC0 | LCD_DDRAMADDRESS;
                    WriteLCD(bus, PCF_ADDR, (byte)dram, (byte)LCD_COMMAND_MODE);       //jump to next line
                    positionInLine = 0;
                }
                if (text[i] == '\n')
                {
                    dram = 0xC0 | LCD_DDRAMADDRESS;
                    WriteLCD(bus, PCF_ADDR, (byte)dram, (byte)LCD_COMMAND_MODE);
                    positionInLine = 0;
                }
                else
                {
                    WriteLCD(bus, PCF_ADDR, (byte)text[i], (byte)LCD_CHARACTER_MODE);
                    positionInLine = positionInLine + 1;
                }
            }
        }

        static void SetPrintPosn(RPi.I2C.Net.I2CBus bus, int offset)
        {
            int posn = LCD_DDRAMADDRESS | offset;
            WriteLCD(bus, PCF_ADDR, (byte)posn, (byte)LCD_COMMAND_MODE);
        }
    }
}
