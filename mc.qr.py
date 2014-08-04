#!/usr/bin/python
#
# Build a QR code in Minecraft Pi using snow & obsidian.
#
# You will need the qrencode package
#   sudo apt-get install qrencode
#
# The input file is generated with qrencode and sed (to trim double chars down
# to single chars)
#   qrencode -t ASCII "Text for QR" | sed 's/  / /g' | sed 's/##/#/g' > ~/mc.qr.txt
#
# (C) Copyright 2014 Dougie Lawson, All rights reserved
#     Creative commons V2.0 BY-NC-SA
#     Licenced under GPL V3

import sys
sys.path.insert(1,'/shared/Raspi/minecraft_pi/api/python')
import mcpi.minecraft as mine
import mcpi.block as block

mc = mine.Minecraft.create()
pPos = mc.player.getTilePos()
print "Player point:", pPos.x, pPos.y, pPos.z
mc.player.setTilePos(pPos.x -20, pPos.y -40, pPos.z -20)

nPos = mc.player.getTilePos()

wifi = open('mc.qr.txt', 'r')
arrayW = []
for line in wifi:
  arrayW.append(line)
wifi.close()

print "Starting point:", nPos.x, nPos.y, nPos.z
x = nPos.x
y = nPos.y 
z = nPos.z

for i in reversed(arrayW):
  y = pPos.y + len(i)

  for j in range(0,len(i)): 
    if (i[j] == " "):
      block = 80 
    if (i[j] == "#"):
      block = 49 
    y = y - 1
#    print "X:", x, "Y:", y, "Z:", z, "Value:", block 
    mc.setBlock(x,y,z,block)
 
  x = x + 1

mc.player.setPos(pPos.x,pPos.y,pPos.z)
