Unified LCD driver
==================

Copyright &copy; Dougie Lawson 2015, all rights reserved.
 
This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
 
 To view a copy of this license, visit [creativecommons.org]
   
[creativecommons.org]:http://creativecommons.org/licenses/by-nc-sa/4.0/deed.en_GB.  

To build for GPIO use
```
make
```
The LCD needs to be wired as

* EN == GPIO16
* RS == GPIO26
* D7 == GPIO12
* D6 == GPIO5
* D5 == GPIO6
* D4 == GPIO13
* BL &lt;Not connected&gt; you could wire it to GPIO18 for PWM control
 
To build for SPI (MCP23S17) use
```
SPI=Y make
```

To build for I2C (MCP23017) use
```
sudo apt-get install libi2c-dev
I2C=Y make
```

For both MCP23x17 GPIO chips the LCD needs to be wired as

* BL == B7
* RS == B6
* EN == B4
* D7 == B3
* D6 == B2
* D5 == B1
* D4 == B0
