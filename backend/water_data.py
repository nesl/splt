from pysos import pysos
import thread
import time
import os

data = [0,0,0,0]
class WaterModule:
	def __init__(self):
		self.srv = pysos.sossrv()
		self.srv.register_trigger(self.getSOSmsg)
		#try:
		#	thread.start_new_thread(self.getSOSmsg,())
		#except thread.error:
		#	pass

	def getSOSmsg(self,msg):
		pkt = msg['data']
		try:
			(ID,Data) = pysos.unpack('<BH',pkt)
			print "WaterMeter",Data
			data[0] = Data
		except:
			pass
		try:
			(ID,Data) = pysos.unpack('<Bf',pkt)
			print "Var", ID, Data
			if ID == 11:
				data[2] = Data
			elif ID == 12:
				data[1] = Data
			elif ID == 13:
				data[3] = Data
		except:
			pass
			#if ID == 10:
			#	print Data
			#print ID,Data 

if(__name__ == "__main__"):
	dummy = WaterModule()
	file = open('data.txt','w')
	while True:
		file.write("%d,%.15f, %.15f, %.15\n"%(data[0],data[1],data[2],data[3]))
		time.sleep(0.5)
