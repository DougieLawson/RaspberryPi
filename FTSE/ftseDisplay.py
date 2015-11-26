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

#from trepan.api import debug

import sys
import os
import threading
import pifacecommon
import  pifacecad_emulator as pifacecad
from time import sleep
from datetime import datetime
import sqlite3
import socket
import errno

UPDATE_INTERVAL = 60
#db = sqlite3.connect('ftse.db', check_same_thread=False)
db = sqlite3.connect('ftse.db')

db.row_factory = sqlite3.Row
db_port = db.cursor()
db_port.execute('''SELECT symbol, title from portfolio''')

SYMBOLS = []
for rows in db_port.fetchall():
  SYMBOLS.append(rows)

db_port.close()

class systemDisplay(object):
   def __init__(self, cad):
      self.cad = cad
      gw = os.popen("ip -4 route show default").read().split()
      s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      s.connect((gw[2], 0))
      self.ipaddr = s.getsockname()[0]
      self.gateway = gw[2]
      self.host = socket.gethostname() 

   def display(self, event=None):
      host = self.host
      ipaddr = self.ipaddr
      gatew = self.gateway
#      print( "{hn} @ \n{ip} via {gw}".format(hn=host,ip=ipaddr gw=gw,))
      self.cad.lcd.clear()
      self.cad.lcd.set_cursor(15,0)
      self.cad.lcd.write("{hn} @ ".format(hn=host,))
      self.cad.lcd.set_cursor(10,1)
      self.cad.lcd.write("{ip} via {gw}".format(ip=ipaddr, gw=gatew,))
      for i in range(0,20):
         self.cad.lcd.move_left()
         sleep(0.4)


class FTSEDisplay(object):
   def __init__(self, symbol, title):
     # print ("FTSEDisplay::__init__")
      self.symbol = symbol
      self.title = title
      self._prices = None

   def update_prices(self):
     # print ("FTSEDisplay::update_prices")
      f_time = datetime.now().strftime('%a %d %b @ %H:%M')
      symbol = self.symbol
      dbp = sqlite3.connect('ftse.db')
      db_live = dbp.cursor()
      db_live.execute('''SELECT symbol, stindex FROM current_prices WHERE symbol = ?;''', (symbol,))
      rows = db_live.fetchall()
      for row in rows:
         prices = dict()
         prices['now'] = f_time
         prices['title'] = self.title
         prices['symbol'] = row[0]
         prices['index'] = row[1]
      self._prices = prices

class FTSEDisplayBoard(object):
   def __init__(self, cad, price_boards, board_index = 0):
     # print ("FTSEDisplayBoard::__init__")
      sleep(1)
      self.price_boards = price_boards
      self.board_index = board_index
      self.cad = cad
      self.cad.lcd.backlight_on()
      self.cad.lcd.blink_off()
      self.cad.lcd.cursor_off()
      self.cad.lcd.write("FTSE display\nStarting")
      sleep(1)
      self.timer = threading.Timer(UPDATE_INTERVAL, self.auto_update)
      self.timer.start()

   @property
   def current_board(self):
     # print ("FTSEDisplayBoard::current_board", self.board_index)
      return self.price_boards[self.board_index]

   def update(self):
     # print ("FTSEDisplayBoard::update")
      self.cad.lcd.clear()
      self.print_loading()
      self.update_board()

   def update_board(self):
     # print ("FTSEDisplayBoard::update_board")
      symbol = self.current_board.symbol
      self.current_board.update_prices()
      self.print_prices()

   def print_loading(self):
     # print ("FTSEDisplayBoard::print_loading")
      symbol = self.current_board.symbol
      self.cad.lcd.write("Loading prices for\n{}".format(symbol))
      sleep(0.4)

   def print_prices(self):
     # print ("FTSEDisplayBoard::print_prices")
      symbol = self.current_board.symbol
      prices = self.current_board._prices
     # print("{}".format(symbol))
     # print("{no} {ti} {sy} {ix}".format(no=prices['now'], ti=prices['title'], sy=prices['symbol'], ix=prices['index']))
      self.cad.lcd.clear()
      self.cad.lcd.home()
      self.cad.lcd.set_cursor(15,0)
      self.cad.lcd.write("{no} ".format(no=prices['now']))
      self.cad.lcd.set_cursor(15,1)
      self.cad.lcd.write("{ti} ".format(ti=prices['title']))
      self.cad.lcd.write("{sy} ".format(sy=prices['symbol']))
      self.cad.lcd.write("{ix}  ".format(ix=prices['index']))
      for i in range(0,22):
         self.cad.lcd.move_left()
         sleep(0.4)

   def auto_update(self):
     # print ("FTSEDisplayBoard::auto_update")
     # print("Updating...")
      self.cad.__init__()
      self.cad.lcd.backlight_on()
      self.cad.lcd.blink_off()
      self.cad.lcd.cursor_off()
      self.update_board()
      self.timer = threading.Timer(UPDATE_INTERVAL, self.auto_update)
      self.timer.start()

   def update_price(self, event=None):
     # print ("FTSEDisplayBoard::update_price ...", self.board_index, self.current_board.symbol)
      self.update()

   def next_price(self, event=None):
     # print ("FTSEDisplayBoard::next_price ...", self.board_index, self.current_board.symbol)
      self.board_index = (self.board_index + 1) % len(self.price_boards)
     # print (self.board_index, self.current_board.symbol)
      self.update()

   def prev_price(self, event=None):
     # print ("FTSEDisplayBoard::prev_price ... ", self.board_index, self.current_board.symbol)
      self.board_index = (self.board_index - 1) % len(self.price_boards)
     # print (self.board_index, self.current_board.symbol)
      self.update()

   def reboot(self, event=None):
     # print ("FTSEDisplay::reboot")
      os.system("shutdown -r 5")
      self.cad.__init__()
      self.cad.lcd.write("RPi will reboot\nin five minutes")
      end_barrier2.wait()
 
   def shutdown(self, event=None):
     # print ("FTSEDisplay::shutdown")
      os.system("shutdown -h 5")
      self.cad.__init__()
      self.cad.lcd.write("RPi will halt\nin five minutes")
      end_barrier2.wait()

   def restart(self, event=None):
     # print ("FTSEDisplay::restart")
      os.system("/etc/init.d/ftseDis start")
      self.cad.__init__()
      self.cad.lcd.write("FTSE display will\nrestart now")
      end_barrier2.wait()

   def close(self):
      if self.timer is not None:
         self.timer.cancel()
      self.cad.lcd.clear()
      self.cad.lcd.backlight_off()

if __name__ == "__main__":

   ftsepricedisplays = [ FTSEDisplay(stuff['symbol'], stuff['title']) for stuff in SYMBOLS ]

   cad = pifacecad.PiFaceCAD()

   global systeminfo
   systeminfo = systemDisplay(cad)

   global ftsedisplay
   ftsedisplay = FTSEDisplayBoard(cad, ftsepricedisplays)
   ftsedisplay.update()

   global end_barrier1
   end_barrier1 = threading.Barrier(2)

   global listener1
   listener1 = pifacecad.SwitchEventListener(chip=cad)
   listener1.register(0, pifacecad.IODIR_ON, ftsedisplay.next_price)
   listener1.register(1, pifacecad.IODIR_ON, ftsedisplay.prev_price)
   listener1.register(2, pifacecad.IODIR_ON, systeminfo.display)
   listener1.register(4, pifacecad.IODIR_ON, end_barrier1.wait)
   listener1.register(5, pifacecad.IODIR_ON, ftsedisplay.next_price)
   listener1.register(7, pifacecad.IODIR_ON, ftsedisplay.prev_price)
   listener1.activate()

   end_barrier1.wait()
   listener1.deactivate()
   ftsedisplay.close() 

   print ("Main_line::after end_barrier1")
   os.system("logger /etc/init.d/ftseDis terminating")
   cad.lcd.write("restart, reboot or\nshutdown 1,2 or 3")

   global listener2
   listener2 = pifacecad.SwitchEventListener(chip=cad)
   listener2.register(0, pifacecad.IODIR_ON, ftsedisplay.restart)
   listener2.register(1, pifacecad.IODIR_ON, ftsedisplay.reboot)
   listener2.register(2, pifacecad.IODIR_ON, ftsedisplay.shutdown)
   listener2.activate()
   
   global end_barrier2
   end_barrier2 = threading.Barrier(2) 
   print ("Main_line::end_barrier2 started")

   end_barrier2.wait()
   listener2.deactivate()
   exit (0) 
