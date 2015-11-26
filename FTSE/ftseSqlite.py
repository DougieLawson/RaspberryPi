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

import os
import time
from datetime import datetime
import csv
from urllib.request import urlopen
import io
import sqlite3
import sys
sys.path.append('/home/pi/python/ftse')
from ftseGetSymbol import getSymbol

db = sqlite3.connect('ftse.db')
db_port = db.cursor()
db_live = db.cursor()

db_port.execute('''SELECT symbol, title from portfolio''')

symbols = db_port.fetchall()

while True:
   for symbol in symbols:

      asym=getSymbol(symbol[0])
      fl_price = float(asym[5])
      fl_change = float(asym[8])
      db_live.execute('''UPDATE current_prices SET stindex = ?, change = ? WHERE symbol = ?''', (fl_price, fl_change, asym[0]))
      db.commit()
   time.sleep(60.0)
