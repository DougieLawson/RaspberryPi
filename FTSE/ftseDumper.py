import os
import time
from datetime import datetime
import csv
import sqlite3

db = sqlite3.connect('ftse.db')
db_live = db.cursor()

for line in db.iterdump():
   print ("%s\n", line)
