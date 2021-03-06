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

import smbus             #gives us a connection to the I2C bus
import time              #for timing delays
import random            #random number generator
import font0             #basic bitmapped 8x8 VGA font
import sys               #for access to command-line parameters

#Definitions for the MCP23017 chip
ADDR   = 0x20            #The I2C address of our chip
DIRA   = 0x00            #PortA I/O direction, by pin. 0=output, 1=input
DIRB   = 0x01            #PortB I/O direction, by pin. 0=output, 1=input
PORTA  = 0x12            #Register address for PortA
PORTB  = 0x13            #Register address for PortB


ORIENTATION = 180        #default viewing angle for the pi & matrix
                         #set this value according to your viewing angle
                         #values are  0 (power jack on left)
                         #           90 (power jack on bottom)
                         #          180 (power jack on right)
                         #          270 (power jack on top)

rows   = 0x00            #starting pattern is (0,0) = all LEDS off
columns= 0x00
delay  = 0.08            #time delay between LED display transitions
                         #smaller values = faster animation

########################################################################
#
#   Lower level routines for bit-manipulation.
#   Nothing here relates to our snazzy Pi Matrix.
#

def ReverseBits (byte):
    #reverse the order of bits in the byte: bit0 <->bit 7, bit1 <-> bit6, etc.
    value = 0
    currentBit = 7
    for i in range(0,8):
        if byte & (1<<i):
            value |= (0x80>>i)
            currentBit -= 1
    return value

def ROR (byte):
    #perform a 'rotate right' command on byte
    #bit 1 is rotated into bit 7; everything else shifted right
    bit1 = byte & 0x01          #get right-most bit
    byte >>= 1                  #shift right 1 bit
    if bit1:                    #was right-most bit a 1?
        byte |= 0x80            #if so, rotate it into bit 7
    return byte
    
def ROL (byte):
    #perform a 'rotate left' command on byte
    #bit 7 is rotated into bit 1; everything else shifted left
    bit7 = byte & 0x080         #get bit7
    byte <<= 1                  #shift left 1 bit
    if bit7:                    #was bit7 a 1?
        byte |= 0x01            #if so, rotate it into bit 1
    return byte


########################################################################
#
#   Lower Level LED Display routines.
#   These write data directly to the Pi Matrix board
#

def Write (register, value):
    #Abstraction of I2C bus and the MCP23017 chip
    #Call with the chip data register & value pair
    #The chip address is constant ADDR (0x20).
    bus.write_byte_data(ADDR,register,value)

def EnableLEDS ():
    #Set up the 23017 for 16 output pins
    Write(DIRA, 0x00)           #all zeros = all outputs on PortA
    Write(DIRB, 0x00)           #all zeros = all outputs on PortB

def DisableLEDS ():
    #Set all outputs to high-impedance by making them inputs
    Write(DIRA, 0xFF);          #all ones = all inputs on PortA
    Write(DIRB, 0xFF);          #all ones = all inputs on PortB

def TurnOffLEDS ():
    #Clear the matrix display
    Write(PORTA, 0x00)          #set all columns low
    Write(PORTB, 0x00)          #set all rows low
    
def TurnOnAllLEDS ():
    #Turn on all 64 LEDs
    Write(PORTA, 0xFF)          #set all columns high
    Write(PORTB, 0x00)          #set all rows low
    
def WriteToLED (rowPins,colPins):
    #set logic state of LED matrix pins
    Write(PORTA, 0x00)          #turn off all columns; prevent ghosting
    Write(PORTB, rowPins)       #set rows first
    Write(PORTA, colPins)       #now turn on columns


########################################################################
#
#   Intermediate-level routines for
#   LED Pixel, Row, Column, and Pattern display
#

def SetPattern (rowPattern,colPattern,orientation=ORIENTATION):
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
    
    if orientation==0:
        WriteToLED(~rows,ReverseBits(columns))
    elif orientation==90:
        WriteToLED(~columns,rows)
    elif orientation==180:
        WriteToLED(~ReverseBits(rows),columns)
    elif orientation==270:
        WriteToLED(~ReverseBits(columns),ReverseBits(rows))

def MultiplexDisplay (z,count):
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

def GetSomeText():
    # Get some text for scrolling on the LEDs.
    #
    # The easiest way is just to prompt for it, like this:
    # st = raw_input("Enter a string to display: ")
    #
    # Instead, let's check the command line.
    # If nothing there, then get from standard input.
    # This allows us to do fancy stuff like pipes & redirects like:
    #
    # ./myscript.py time
    # ./myscript.py "The yellow brown dog"
    # echo "once I pondered weak and weary" | ./myscript.py
    # cat poem.txt | ./myscript.py
    # ./myscript.py <poem.txt

    if len(sys.argv)>1:         #are there any command line args?
        st = sys.argv[1]        #yes, so take first one as text
    else:
        print "Enter text to display, followed by EOF (Ctrl-D)."
        st = sys.stdin.read()   #no, so get standard input instead
        
    # See if the input was either 'time' or 'date'
    # Display time and date if they were requested.
    if st=="time":
        st = 'The time is ' + time.strftime("%I:%M %p")
    elif st=="date":
        st = 'The date is ' + time.strftime("%A, %b %d, %Y")
        
    # Add a space to the end of the message - looks better for repeats
    st += ' '
    return st
    
def CharImage (ch):
    # returns the 8x8 raster image for given character.
    # the result is a list of 8 bytes, 1 byte for each raster row.
    index = ord(ch)
    image = list(font0.data[index])
    return image

def ScrollLeft (z,buf):
    # scrolls image in Z leftward, rotating in data from buf
    # data in Z & buf are both changed by this operation!
    for row in range(0,8):               #for each row in image:
        z[row] >>= 1                     #shift current image row
        if buf[row] & 0x01:              #is lsb of buffer high?
            z[row] |= 0x80               #rotate lsb of buf into msb z
        buf[row] >>= 1                   #shift buffer row, too.

def ScrollText (st):
    # This is the all-important routine!
    # It will scroll the text on the Pi Matrix, left to right.
    # st holds the string that is being scrolled.
    # ch is the character to be scrolled onto the display.
    # buf holds the character image waiting to be scrolled onto display
    # Z holds the character image now being displayed
    # Each character is 8 bits (columns wide), so we will need to scroll
    # left 8 times for each character.  Between each scroll operation,
    # we also need to quickly display (multilplex) each of the 8 rows.
    SPEED = 10                           #scroll speed: lower=faster
    z = [0,0,0,0,0,0,0,0]                #start with a blank display
    for i in range(0,len(st)):           #for each character of text
        ch  = st[i]                      #get the next character
        buf = CharImage(ch)              #get next character image
        for shiftCount in range(0,8):    #For each column in image
            MultiplexDisplay(z,SPEED)    #display current image
            ScrollLeft(z,buf)            #scroll display leftward


#
#   Main Program here.
#

print "Pi Matrix text scroller starting"
bus = smbus.SMBus(1)            #initialize the I2 bus; use '0' for
                                #older Pi boards, '1' for newer ones.
EnableLEDS()                    #initialize the Pi Matrix board
text = GetSomeText()            #get the text
print "displaying '%s'" %text   #echo text to console
ScrollText(text)                #and scroll it!
DisableLEDS()                   #dont leave any LEDs on.
print "Done."
