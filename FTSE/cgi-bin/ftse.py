#!/usr/bin/env python3
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

print("Content-type: text/html\r\n")
from datetime import datetime

import cgi
import cgitb
import sqlite3
db = sqlite3.connect('/home/pi/python/ftse/ftse.db')
cursor = db.cursor()
db_port = db.cursor()
db_live = db.cursor()

db_port.execute('''SELECT symbol, title from portfolio''')

symbols = db_port.fetchall()
cgitb.enable() 
print("<html><body>")
print("<form action=ftseupd.py>")
print("<table>")
f_time = datetime.now().strftime('%a %d %b @ %H:%M')
print ("<tr><td colspan=5>", f_time)
print ("</td></tr>")
print("<tr><th>DEL</th><th>Symbol</th><th>Title</th><th>Price</th><th>Change</th></tr>")

rnum = int(0)
for symbol in symbols:
   db_live.execute('''SELECT symbol, stindex, change FROM current_prices WHERE symbol = ?;''', (symbol[0],)) 
   rows = db_live.fetchall()
   rnum = rnum + 1
   for row in rows:
      print ("<tr>")
      f1 = row[0]
      fl_f4 = row[1]
      fl_f7 = row[2]
      if row[0] != "^FTSE":
         print ("<td><input type=checkbox name=delSym[{nu}] value={sy}></td>".format(nu=rnum, sy=row[0],))
      else:
         print ("<td>&nbsp;</td>")
      print ("<td>", f1)
      print ("</td><td>", symbol[1])
      print ("</td><td>" + format(fl_f4, ',.2f'))
      print ("</td><td>" + format(fl_f7, ',.2f'), "</td></tr>")

print("<tr><td colspan=2>Add new symbol</td><td colspan=3><input type=text name=newSym></td></tr>")
print("</table>")
print("<input type=submit name=Submit value=Submit>")
print("</form></body></html>")
