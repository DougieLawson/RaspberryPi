#!/usr/bin/python

import math
import sqlite3 as sql
import time
import datetime
import plotly.plotly as py
import plotly.tools as tls
from plotly.graph_objs import *
stream_ids = tls.get_credentials_file()['stream_ids']

sensorData = sql.connect('/srv/bmp180/sensordata.db')
bmp_data = sensorData.cursor()
time3 = ''
startTime = datetime.datetime.now() - datetime.timedelta(minutes=1440)

temp_trace = Scatter(
    x = [],
    y = [],
    mode = 'lines+markers',
    name = 'Temp',
    stream = Stream(
        token=stream_ids[0]
    )
)
press_trace = Scatter(
    x = [],
    y = [],
    mode = 'lines+markers',
    name = 'Pressure (QFE)',
    yaxis='y2',
    stream = Stream(
        token=stream_ids[1]
    )
)
msl_trace = Scatter(
    x = [],
    y = [],
    mode = 'lines+markers',
    name = 'MSL (QNH)',
    yaxis='y2',
    stream = Stream(
        token=stream_ids[2]
    )
)

data = Data([temp_trace, press_trace, msl_trace])
layout = Layout(
    title = 'Indoor Temp & Pressure',
    yaxis = YAxis(
        title = 'Temp'
    ),
    yaxis2 = YAxis(
        title = 'Pressure',
        overlaying = 'y',
        side = 'right'
    ),
    showlegend=True,
    legend=Legend(
        x=0,
        y=0
    )
)
fig = Figure(data = data, layout = layout)
unique_url = py.plot(fig, filename='Indoor_temp_pressure', auto_open=False)
#print unique_url
s1 = py.Stream(stream_ids[0])
s2 = py.Stream(stream_ids[1])
s3 = py.Stream(stream_ids[2])

def msl(pressure,altitude=112.2):
    msl = pressure/pow(1-(altitude/44330.0),5.255)
    return "%4.2f"  % (msl)

def tick():
    while True:
        global time3, startTime
        time1 = time.strftime('%S')
        time2 = time.strftime('%M')
        if (time1 == "00"):
	    if (int(time2) % 12) == 0: # every twelve minutes
#	    if (int(time2) % 1) == 0: # every twelve minutes
                 endTime = datetime.datetime.now()
                 sqlEnd = endTime.strftime('%Y-%m-%d %H:%M:59')
                 if time3 != sqlEnd:
                     time3 = sqlEnd
                     s1.open()
                     s2.open()
                     s3.open()
                     sqlStart = startTime.strftime('%Y-%m-%d %H:%M:00')
#print "sqlStart: ", sqlStart
#print "sqlEnd: ", sqlEnd        
                     bmp_data.execute("select date_time, temp, pressure from bmp_data where date_time between ? and ?",(sqlStart, sqlEnd))
                     while True:
                         row = bmp_data.fetchone()
                         if row == None:
                             break
#                        print row[0], row[1], row[2], msl(row[2])
    
                         s1.write(dict(x=row[0], y=row[1]))
                         s2.write(dict(x=row[0], y=row[2]))
                         s3.write(dict(x=row[0], y=msl(row[2])))
                         startTime = endTime + datetime.timedelta(seconds=30)
                         tempStart = startTime.strftime('%Y-%m-%d %H:%M:00')
#print "tempStart: ", tempStart
#                     s1.write(dict(x=x, y=temp))
#                     s2.write(dict(x=x, y=press))
#                     s3.write(dict(x=x, y=msl_press))
            if (time2 == "00"): # Once an hour
#                pass
                 s1.close()
                 s2.close()
                 s3.close()
        else:
            pass
        time.sleep(0.3)

tick()
