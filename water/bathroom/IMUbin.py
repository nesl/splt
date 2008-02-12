import time
import serial
from struct import *

#GPS=gps.GpsThread()
#GPS.start()
ser=serial.Serial('/dev/cu.SparkFun-BT-SerialPort-1',57600,timeout=1)
data = ""
for i in range(100000):
        read=ser.read(1)
	if read == 'A' :
		start = 1
	elif read == 'Z' :
		start = 0
		try:
			values = unpack('hhh',data)
			print "%i, %i, %i,"%(values), '%f'%(time.time())
#			print "%i %i %i %f"%(values[0], value[1], value[2], time.time())
			data = ""
		except:
			data = ""
	else:
		data = data+read
		
ser.close()


