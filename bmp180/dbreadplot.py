#!/usr/bin/python3

#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
import mpld3

import paho.mqtt.client as mqtt
import sys
from time import sleep

def on_connect(mqtts, obj, flags, rc):
  print("Conn rc: "+str(rc))

def on_publish(mqtts, obj, mid):
  print("mid: "+str(mid))

def on_message(mqtts, obj, msg):
  global mqttp, broker, port, x, y

  #print ("Message received on topic: "+str(msg.topic)+" with QoS: "+str(msg.qos)+" and payload: "+str(msg.payload))
  pl = msg.payload.decode('utf-8')
  ii, xxx, yyy = pl.split(':')
  ix = int(ii)
  xx = int(xxx)
  yy = float(yyy)
  x[ix] = xx
  y[ix] = yy

  if ix == 23:
    graph = plt.figure()
    plt.ylabel('Temp')
    plt.xlabel('Time')
    obj = plt.plot(x, y, 'ro-')
    mpld3.save_html(graph,'/home/dougie/public_html/nmplot.html')
    plt.close(graph)

  sleep(1)

def on_subscribe(mqtts, obj, mid, granted_qos):
  print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_log(mqtts, obj, level, string):
  print(string)

def main():

  global mqttp, broker, port, x, y
  broker = "10.1.1.11"
  port = 1883
 
  x = [0,0,0,0,0,0,
       0,0,0,0,0,0,
       0,0,0,0,0,0,
       0,0,0,0,0,0]
  y = [0,0,0,0,0,0,
       0,0,0,0,0,0,
       0,0,0,0,0,0,
       0,0,0,0,0,0]



  mqtts = mqtt.Client("ard_sub")
 
  mqtts.on_message = on_message
  mqtts.on_connect = on_connect
  mqtts.on_publish = on_publish
  mqtts.on_subscribe = on_subscribe

  mqtts.connect(broker, port, 60)
  mqtts.subscribe("dbread/out/+", 0)

  rc = 0
  while rc == 0:
    rc = mqtts.loop_forever()

  return 0 

if __name__ == "__main__":
   sys.exit(main())
