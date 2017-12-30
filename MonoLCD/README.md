Raspberry Pi Mono 1602 LCD 
==========================

This needs the I2C library from MagPi Mag Issue #30 as a basis. Most of the code is built on that as a base level.

You need the DLL (*.so library) from https://github.com/mshmelev/RPi.I2C.Net
.git in /usr/local/lib


Install mono on your RPi with 
``` sudo apt install mono-complete```


Build both pieces with
 xbuild /p:TargetFrameworkVersion="v4.5" 


&copy; 2017 Dougie Lawson, All rights reserved
Licence: CC 4.0 BY-NC-SA
