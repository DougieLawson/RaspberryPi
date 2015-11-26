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

def getSymbol(symbol):
   url = 'http://finance.yahoo.com/d/quotes.csv?s={0}&f=snd1l1yrc1'.format(symbol)
   symdata = urlopen(url)
   response = str(symdata.read().decode('utf-8'))
   comsep = csv.reader(response.split('\r\n'), delimiter='"')
   for row in comsep:
      try:
         row[0] = row[0].split(',')[0]
         row.append(row[4].split(',')[1])
         row.append(row[4].split(',')[2])
         row.append(row[4].split(',')[3])
         row.append(row[4].split(',')[4])
         row.append(datetime.now())
         symrow = row
      except:
         pass
   return symrow

