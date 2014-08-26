#!/usr/bin/python 

########################################################################
#
#    Matrix4:  text scrolling on the Pi Matrix
#
#    This application will let you scroll text
#    across the Pi Matrix.
#
#    To learn more, visit w8bh.net
#
#    Author:  Bruce E. Hall
#    Date  :  21 Feb 2013
#
########################################################################

import locale
import smbus             #gives us a connection to the I2C bus
import time              #for timing delays
import random            #random number generator
import sys               #for access to command-line parameters

locale.setlocale(locale.LC_ALL, 'en_GB.UTF-8')

#Definitions for the MCP23017 chip
ADDR   = 0x20            #The I2C address of our chip
DIRA   = 0x00            #PortA I/O direction, by pin. 0=output, 1=input
DIRB   = 0x01            #PortB I/O direction, by pin. 0=output, 1=input
PORTA  = 0x12            #Register address for PortA
PORTB  = 0x13            #Register address for PortB

rows   = 0x00            #starting pattern is (0,0) = all LEDS off
columns= 0x00
delay  = 0.07            #time delay between LED display transitions
                         #smaller values = faster animation

########################################################################
#
#   Lower level routines for bit-manipulation.
#   Nothing here relates to our snazzy Pi Matrix.
#


########################################################################
#
#   Lower Level LED Display routines.
#   These write data directly to the Pi Matrix board
#

def Write (register, value):
#    print "Write"
    #Abstraction of I2C bus and the MCP23017 chip
    #Call with the chip data register & value pair
    #The chip address is constant ADDR (0x20).
    bus.write_byte_data(ADDR,register,value)

def EnableLEDS ():
#    print "EnableLEDS"
    #Set up the 23017 for 16 output pins
    Write(DIRA, 0x00)           #all zeros = all outputs on PortA
    Write(DIRB, 0x00)           #all zeros = all outputs on PortB

def DisableLEDS ():
#    print "DisableLEDS"
    #Set up the 23017 for 16 output pins
    Write(DIRA, 0xff)           #all zeros = all outputs on PortA
    Write(DIRB, 0xff)           #all zeros = all outputs on PortB

    
def WriteToLED (rowPins,colPins):
#    print "WriteToLED"
    #set logic state of LED matrix pins
    Write(PORTA, 0x00)          #turn off all columns; prevent ghosting
    Write(PORTB, rowPins)       #set rows first
    Write(PORTA, colPins)       #now turn on columns


########################################################################
#
#   Intermediate-level routines for
#   LED Pixel, Row, Column, and Pattern display
#

def SetPattern (rowPattern,colPattern):
#    print "SetPattern"
    #Applies given row & column patterns to the matrix.
    #For columns, bit 0 is left-most and bit 7 is at far right.
    #For rows, bit 0 is at the top and bit 7 is at the bottom.
    #Example: (0x07,0x03) will set 3 row bits & 2 columns bits,
    #formaing a rectagle of  6 lit LEDS in upper left corner of 
    #the matrix, three rows tall and two columns wide.
    #Why?  0x07 = 0b00000111 (three lower row bits set).
    #      0x03 = 0b00000011 (two lower column bits set).

    global rows, columns                    #save current row/column
    rows = rowPattern
    columns = colPattern
    
    WriteToLED(~columns,rows)

def MultiplexDisplay (z,count):
#    print "MultiplexDisplay"
    # call this routine with Z, the image to display on the matrix
    # Z is a list containing 8 bytes (8x8=64 bits for 64 LEDs)
    # z[0] is data for the top row of the display; z[7] is bottom row.
    # count is number of times to cycle the display
    for count in range(0,count):
        for row in range(0,8): 
            SetPattern(1<<row,z[row])       #quickly display each row


########################################################################
#
#   Application Routines
#   Main Program & supporting routines go here.
#

def BinClock ():
#    print "BinClock"
    st = [0,0,0,0,0,0,0,0]
    timeStr = list(time.strftime("%X"))
    for i in range(0,8):
#        print timeStr[i]
        if timeStr[i] == ":": timeStr[i] = 0
        st[i] = int(timeStr[i]) * 4

    SPEED = 10
    MultiplexDisplay(st,SPEED)        #display current image

#
#   Main Program here.
#

bus = smbus.SMBus(1)            #initialize the I2 bus; use '0' for
                                #older Pi boards, '1' for newer ones.
EnableLEDS()                    #initialize the Pi Matrix board
try:
	while True:
		BinClock()               #Display BCD on matrix
except:
	DisableLEDS()                   #dont leave any LEDs on.
