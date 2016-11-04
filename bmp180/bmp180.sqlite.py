#!/usr/bin/python

import time
import smbus
import bmp180
import sqlite3 as sql

time4 = ''

bus = smbus.SMBus(1)
sensor = bmp180.Bmp180(bus)
sensor.os_mode = 3

sensorData = sql.connect('/srv/bmp180/sensordata.db')
sensorData.execute("PRAGMA journal_mode=WAL")
sensorData.execute("VACUUM")
bmp_data = sensorData.cursor()

def tick():

    while True:
        global time4
        press, temp = sensor.pressure_and_temperature
        time1 = time.strftime('%M')
        if int(time1) % 3 == 0:
            time2 = time.strftime('%H:%M:%S')
            time3 = time.strftime('%S')
            if time3 == '00': # exactly on the minute
                if time2 != time4:
                    time4 = time2 # Make sure we only run once
#                   dbDate_Time = time.strftime('%Y-%m-%d %H:%M:%S')
#                   print "Time: ",time4, "Temp: ", temp, "Press: ", press
                    bmp_data.execute("insert into bmp_data (date_time, temp, pressure) values(datetime('now','localtime'), ?, ?)", (temp, press))
                    sensorData.commit()
        time.sleep(0.3)

tick()
