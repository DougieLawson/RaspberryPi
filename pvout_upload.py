#!/usr/bin/env python
# -*- coding: utf-8 -*-
import subprocess
from time import strftime
import time
from configobj import ConfigObj
from pyowm import OWM
from pymodbus.client.sync import ModbusSerialClient as ModbusClient

# read settings from config file
config = ConfigObj("/boot/pvoutput.txt")
SYSTEMID = config['SYSTEMID']
APIKEY = config['APIKEY']
OWMKey = config['OWMKEY']
OWMLon = float(config['Longitude'])
OWMLat = float(config['Latitude'])
NoInvert = int(config['Inverters'])

t_date = format(strftime('%Y%m%d'))
t_time = format(strftime('%H:%M'))

pv_volts=0.0
pv_power=0.0
Wh_today=0
current_temp=0.0
cmo_str='None'

for i in range(NoInvert):
# Read data from inverter
  inverter = ModbusClient(method='rtu', port='/dev/ttyUSB'+str(i), baudrate=9600, stopbits=1, parity='N', bytesize=8, timeout=1)
  inverter.connect()
  rr = inverter.read_input_registers(1,27)
  inverter.close()
  value=rr.registers[2]
  pv_volts=pv_volts+(float(value)/10)
  value=rr.registers[11]
  pv_power=pv_power+(float(value)/10)
  value=rr.registers[26]
  Wh_today=Wh_today+(float(value)*100)

if OWMKey<>'':
  owm = OWM(OWMKey)
  if owm.API_online:
    obs = owm.weather_at_coords(OWMLat, OWMLon)
    w = obs.get_weather()
    w_stat = w.get_detailed_status()
    temp = w.get_temperature(unit='celsius')
    current_temp = temp['temp']
    cloud_pct = w.get_clouds()
    com_str= ('%s with a cloud coverage of %s percent' %(w_stat,cloud_pct))

cmd=('curl -d "d=%s" -d "t=%s" -d "v1=%s" -d "v2=%s" -d "v5=%s" -d "v6=%s" -d "c1=0" -H \
"X-Pvoutput-Apikey: %s" -H \
"X-Pvoutput-SystemId: %s" \
http://pvoutput.org/service/r2/addstatus.jsp'\
%(t_date, t_time, Wh_today, pv_power, current_temp, pv_volts,\
APIKEY, SYSTEMID))
ret = subprocess.call(cmd, shell=True)

time.sleep(20)

if Wh_today>0:
  cmd=('curl -d "data=%s,%s,,,,,,,%s,,,,," -H \
  "X-Pvoutput-Apikey: %s" -H \
  "X-Pvoutput-SystemId: %s" http://pvoutput.org/service/r2/addoutput.jsp' \
  %(t_date, Wh_today, com_str,\
  APIKEY,SYSTEMID))
  ret = subprocess.call(cmd, shell=True)

#print 'pv_volts %s' %pv_volts
#print 'pv_power %s' %pv_power
#print 'Wh_today %s' %Wh_today
#print com_str
#print current_temp

