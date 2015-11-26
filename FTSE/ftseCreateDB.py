import sqlite3
db = sqlite3.connect('ftse.db')
cursor = db.cursor()
#cursor.execute(''' CREATE TABLE ftse_history (symbol TEXT, date TEXT, stindex FLOAT, yield FLOAT, p_e FLOAT, datetime TEXT)''')
cursor.execute(''' CREATE TABLE current_prices (symbol TEXT PRIMARY KEY, stindex FLOAT, change FLOAT)''')
cursor.execute(''' CREATE TABLE portfolio (symbol TEXT, title TEXT)''')
db.commit()


