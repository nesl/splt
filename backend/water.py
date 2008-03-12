from pysos import pysos
import thread
import time
import os


class WaterModule:
	def __init__(self):
		self.srv = pysos.sossrv()
		try:
			thread.start_new_thread(self.getSOSmsg,())
		except thread.error:
			pass

	def getSOSmsg(self):
		while True:
			msg = self.srv.listen()
			pkt = msg[0]['data']
			(ID,Data) = pysos.unpack('<Bf',pkt)
			if ID == 10:
				print Data
			#print ID,Data 

if(__name__ == "__main__"):
	dummy = WaterModule()
	while True:
		time.sleep(1)
