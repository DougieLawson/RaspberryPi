Unified LCD driver
==================

Copyright &copy; Dougie Lawson 2015-2020, all rights reserved.
 
This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 
 To view a copy of this license, visit [creativecommons.org]
   
[creativecommons.org]:http://creativecommons.org/licenses/by-nc-sa/4.0/deed.en_GB.  

To build
```
sudo apt-get install libi2c-dev
cmake .
make
```
The GPIO LCD needs to be wired as

* EN == GPIO16
* RS == GPIO26
* D7 == GPIO12
* D6 == GPIO5
* D5 == GPIO6
* D4 == GPIO13
* BL &lt;Not connected&gt; you could wire it to GPIO18 for PWM control
 
For SPI MCP23S17 GPIO chips the LCD needs to be wired as

* BL == B7
* RS == B6
* EN == B4
* D7 == B3
* D6 == B2
* D5 == B1
* D4 == B0

For I²C MCP20S17 GPIO chips the LCD needs to be wired as

* BL == A7
* RS == A6
* EN == A4
* D7 == A3
* D6 == A2
* D5 == A1
* D4 == A0

Note: it should be a trivial change to use an MCP23008.

For PCF8574 GPIO chips the LCD needs to match the normal backpacks

* RS == P0
* RW == P1
* EN == P2
* BL == P3
* D4 == P4
* D5 == P5
* D6 == P6
* D7 == P7

The latest Jan 2020 update is to remove any dependency on wiringPi. 
