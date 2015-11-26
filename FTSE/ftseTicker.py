import os
import time
from datetime import datetime
import csv
import sqlite3

db = sqlite3.connect('ftse.db')
db_port = db.cursor()
db_live = db.cursor()

db_port.execute('''SELECT symbol, title from portfolio''')

symbols = db_port.fetchall()

while True:
   for symbol in symbols:
      f_time = datetime.now()
      db_live.execute('''SELECT symbol, stindex FROM current_prices WHERE symbol = ?;''', (symbol[0],)) 
      rows = db_live.fetchall()
      for row in rows:
         f1 = row[0]
         fl_f4 = row[1]
         print ("Now:   ", f_time)
         print ("Title: ", symbol[1])
         print ("Symbol:", f1)
         print ("Index: ", fl_f4)
   time.sleep(60)
