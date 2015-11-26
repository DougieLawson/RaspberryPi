import sys
import sqlite3

db = sqlite3.connect('ftse.db')
db.row_factory = sqlite3.Row
db_port = db.cursor()
db_port.execute('''SELECT symbol, title from portfolio''')

symbols = [ db_port.fetchall() ]

SYMBOLS = [[stuff for stuff in rows] for rows in symbols]

for x in SYMBOLS:
  print (x[0]['symbol'])
