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

import sys
import os
import cgi
import cgitb
import sqlite3
cgitb.enable() 
sys.path.append(os.getcwd())
from ftseGetSymbol import getSymbol
form = cgi.FieldStorage()

if form['Submit'].value == "Submit":
   print("Content-type: text/html")
   print()
   print("<html><body>")
   print ("<form action=ftseupd.py>")
   print ("<table border=0><tr><th>Action<th>Symbol<th>Name</tr>")
   for names in form:
     # print ("<tr><td>", names, "<td>", form[names].value, "</tr>")
      if names[:3] == "new":
         print ("<tr><td>New Symbol<td>", form[names].value)
         asym = getSymbol(form[names].value)
         print ("<td>", asym[1])
         print ("<input type=hidden name=", form[names].value, "value=add>")
      if names[:3] == "del":
         print ("<tr><td>Remove Symbol<td>", form[names].value)
         asym = getSymbol(form[names].value)
         print ("<td>", asym[1])
         print ("<input type=hidden name=", form[names].value, "value=delete>")

   print ("<tr colspan=3><td><input type=submit name=Submit value=Confirm>")
   print ("</table></form>")
   print("</body></html>")

if form['Submit'].value == "Confirm":
   db = sqlite3.connect('/home/pi/python/ftse/ftse.db')
   db_cur = db.cursor()
   url = "http://"+os.environ['SERVER_NAME']+":"+os.environ['SERVER_PORT']
   print ()
   print("<html><head>")
   print("<meta http-equiv=Refresh content=\"0; url="+url+"/cgi-bin/ftse.py\"")
   print("</head><body>")
   print("</body></html>")
   for names in form:
      if form[names].value == "add":
         asym = getSymbol(names)
         db_cur.execute('''INSERT INTO portfolio (symbol, title) values(?,?)''', (names, asym[1]))
         db_cur.execute('''INSERT INTO current_prices (symbol, stindex, change) values(?,?,?)''', (asym[0], float(asym[5]), float(asym[8])))
      if form[names].value == "delete":
         asym = getSymbol(names)
         db_cur.execute('''DELETE from portfolio WHERE symbol=?''', (names,))
         db_cur.execute('''DELETE from current_prices WHERE symbol=?''', (names,))
   db.commit()

