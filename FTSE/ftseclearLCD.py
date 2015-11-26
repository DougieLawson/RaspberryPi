#!/usr/bin/python3
# 
'''
    Copyright 2014, Dougie Lawson, all rights reserved.

    This file is part of FTSE Display.

    FTSE Display  is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>. 
''' 
#

import sys
import os
import pifacecommon
import pifacecad_emulator as pifacecad


if __name__ == "__main__":

   cad = pifacecad.PiFaceCAD()
   cad.lcd.clear()
   cad.lcd.backlight_off()
   cad.lcd.blink_off()
   cad.lcd.cursor_off()
