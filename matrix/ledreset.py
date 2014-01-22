#!/usr/bin/python


import sys
sys.path.append("/usr/local/lib/python2.7/site-packages/Adafruit_I2C")
from Adafruit_I2C import Adafruit_I2C 

import smbus 
import time

MCP23008_IODIRA = 0x00

MCP23017_IODIRA = 0x00
MCP23017_IODIRB = 0x01
MCP23017_GPIOA = 0x12
MCP23017_GPIOB = 0x13
MCP23017_GPPUA = 0x0C
MCP23017_GPPUB = 0x0D
MCP23017_OLATA = 0x14
MCP23017_OLATB = 0x15

OUTPUT = 0
INPUT = 1
    
class Adafruit_MCP230XX(object):

    def __init__(self, busnum, address, num_gpios):
        assert num_gpios >= 0 and num_gpios <= 16, "Number of GPIOs must be between 0 and 16"
        self.i2c = Adafruit_I2C(address, smbus.SMBus(busnum))
        self.address = address
        self.num_gpios = num_gpios

        # set defaults

        if num_gpios <= 8:
            self.i2c.write8(MCP23008_IODIRA, 0xFF)  # all inputs on port A
            self.direction = self.i2c.readU8(MCP23008_IODIRA)
            self.i2c.write8(MCP23008_GPPU, 0x00)
        elif num_gpios > 8 and num_gpios <= 16:
            self.i2c.write8(MCP23017_IODIRA, 0xFF)  # all inputs on port A
            self.i2c.write8(MCP23017_IODIRB, 0xFF)  # all inputs on port B
            self.direction = self.i2c.readU8(MCP23017_IODIRA)
            self.direction |= self.i2c.readU8(MCP23017_IODIRB) << 8
            self.i2c.write8(MCP23017_GPPUA, 0x00)
            self.i2c.write8(MCP23017_GPPUB, 0x00)
    
    def _changebit(self, bitmap, bit, value):
        assert value == 1 or value == 0, "Value is %s must be 1 or 0" % value
        if value == 0:
            return bitmap & ~(1 << bit)
        elif value == 1:
            return bitmap | (1 << bit)

    def _readandchangepin(self, port, pin, value, currvalue = None):
        assert pin >= 0 and pin < self.num_gpios, "Pin number %s is invalid, only 0-%s are valid" % (pin, self.num_gpios)
        #assert self.direction & (1 << pin) == 0, "Pin %s not set to output" % pin
        if not currvalue:
             currvalue = self.i2c.readU8(port)
        newvalue = self._changebit(currvalue, pin, value)
        self.i2c.write8(port, newvalue)
        return newvalue


    def pullup(self, pin, value):
        if self.num_gpios <= 8:
            return self._readandchangepin(MCP23008_GPPU, pin, value)
        if self.num_gpios <= 16:
            if (pin < 8):
                return self._readandchangepin(MCP23017_GPPUA, pin, value)
            else:
                return self._readandchangepin(MCP23017_GPPUB, pin-8, value)

    # Set pin to either input or output mode
    def config(self, pin, mode):        
        if self.num_gpios <= 8:
            self.direction = self._readandchangepin(MCP23008_IODIR, pin, mode)
        if self.num_gpios <= 16:
            if (pin < 8):
                self.direction = self._readandchangepin(MCP23017_IODIRA, pin, mode)
            else:
                self.direction = self._readandchangepin(MCP23017_IODIRB, pin-8, mode)

        return self.direction

    def output(self, pin, value):
        # assert self.direction & (1 << pin) == 0, "Pin %s not set to output" % pin
        if self.num_gpios <= 8:
            self.outputvalue = self._readandchangepin(MCP23008_GPIO, pin, value. self.i2c.readU8(MCP23008_OLAT))
        if self.num_gpios <= 16:
            if (pin < 8):
                self.outputvalue = self._readandchangepin(MCP23017_GPIOA, pin, value, self.i2c.readU8(MCP23017_OLATA))
            else:
                self.outputvalue = self._readandchangepin(MCP23017_GPIOB, pin-8, value, self.i2c.readU8(MCP23017_OLATB))

        return self.outputvalue


        self.outputvalue = self._readandchangepin(MCP23017_IODIRA, pin, value, self.outputvalue)
        return self.outputvalue
        
    def input(self, pin):
        assert pin >= 0 and pin < self.num_gpios, "Pin number %s is invalid, only 0-%s are valid" % (pin, self.num_gpios)
        assert self.direction & (1 << pin) != 0, "Pin %s not set to input" % pin
        if self.num_gpios <= 8:
            value = self.i2c.readU8(MCP23008_GPIO)
        elif self.num_gpios > 8 and self.num_gpios <= 16:
            value = self.i2c.readU16(MCP23017_GPIOA)
            temp = value >> 8
            value <<= 8
            value |= temp
        return value & (1 << pin)

        

# RPi.GPIO compatible interface for MCP23017 and MCP23008

class MCP230XX_GPIO(object):
    OUT = 0
    IN = 1
    BCM = 0
    BOARD = 0
    def __init__(self, busnum, address, num_gpios):
        self.chip = Adafruit_MCP230XX(busnum, address, num_gpios)
    def setmode(self, mode):
        # do nothing
        pass
    def setup(self, pin, mode):
        self.chip.config(pin, mode)
    def input(self, pin):
        return self.chip.input(pin)
    def output(self, pin, value):
        self.chip.output(pin, value)
    def pullup(self, pin, value):
        self.chip.pullup(pin, value)
        

if __name__ == '__main__':
    # Use busnum = 0 for older Raspberry Pi's (pre 512MB)
    
 #   mcp2 = Adafruit_MCP230XX(busnum = 0, address = 0x21, num_gpios = 16)
    mcp = Adafruit_MCP230XX(busnum = 1, address = 0x20, num_gpios = 16)
 #   mcp2.config(0, OUTPUT)
    mcp.config(0, OUTPUT)
    mcp.config(1, OUTPUT)
    mcp.config(2, OUTPUT)
mcp.config(3, OUTPUT)
mcp.config(4, OUTPUT)
mcp.config(5, OUTPUT)
mcp.config(6, OUTPUT)
mcp.config(7, OUTPUT)
mcp.config(8, OUTPUT)
mcp.config(9, OUTPUT)
mcp.config(10, OUTPUT)
mcp.config(11, OUTPUT)
mcp.config(12, OUTPUT)
mcp.config(13, OUTPUT)
mcp.config(14, OUTPUT)
mcp.config(15, OUTPUT)
#mcp2.output(0, 1) 
for i in range(0,8):
	mcp.output(i,0)
for i in range(8,16):
	mcp.output(i,0)

