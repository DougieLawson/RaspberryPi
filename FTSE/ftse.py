import os
import time
import ftseGetSymbol

old_f4 = 0.0
asym=ftseGetSymbol.getSymbol("^FTSE")
print ("--", asym[9], "--")
print ("Symbol:", asym[0])
print ("Title: ", asym[1])
print ("Date:  ", asym[3])
fl_f4 = float(str(asym[5]))
fl_f7 = float(str(asym[8]))
try: 
  fl_f5 = float(str(asym[6]))
  fl_f6 = float(str(asym[7]))
except:
  fl_f5 = str(asym[6])
  fl_f6 = str(asym[7])
print ("Index: ", fl_f4)
print ("F5,F6: ", fl_f5, fl_f6)
print ("Change:", fl_f7)
#while True:
#   if old_f4 != fl_f4:
#      old_f4 = fl_f4
#   time.sleep(900.0)
#getSymbol("BG.L")
#getSymbol("BT-A.L")
