import sqlite3
import ftseGetSymbol
db = sqlite3.connect('ftse.db')
cursor = db.cursor()
symlist = ['^FTSE','BG.L','BT-A.L']
fl_f4 = 0.0
fl_f5 = -1.0
fl_f6 = -1.0
fl_f7 = 0.0
f3 = "01/01/2014"
f_time = "2010-03-01 00:00:00"
for sym in symlist:
  symdata = ftseGetSymbol.getSymbol(sym)
  f1 = symdata[0]
  f2 = symdata[1]
  cursor.execute('''INSERT INTO current_prices (symbol, stindex, change) values(?,?,?)''', (f1, fl_f4, fl_f7))
 # cursor.execute('''INSERT INTO ftse_history (symbol, date, stindex, yield, p_e, datetime) VALUES(?,?,?,?,?,?) ''', (f1, f3, fl_f4, fl_f5, fl_f6, f_time))
  cursor.execute('''INSERT INTO portfolio (symbol, title) VALUES(?,?) ''', (f1, f2))
  db.commit()
