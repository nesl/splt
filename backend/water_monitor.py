from pysos import pysos
import thread
import time
import os


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
		except:
			pass
		try:
			(ID,Data) = pysos.unpack('<Bf',pkt)
			print "Var", Data
		except:
			pass
			#if ID == 10:
			#	print Data
			#print ID,Data 

if(__name__ == "__main__"):
	dummy = WaterModule()
	while True:
		time.sleep(1)
