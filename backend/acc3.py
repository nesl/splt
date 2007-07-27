import sys
import time
import os
import Queue
import thread
from pysos import pysos
from Slog import spltSlog




class photoModule:
	def __init__(self):
		try:
			self.srv = pysos.sossrv()
		except:
			print "Please run sos_server first"
		try:
			thread.start_new_thread(self.getSOSmsg,())
		except thread.error:
			pass
		self.middle = 511.735
		self.position = 0
		self.velocity = 0
		self.samplingtime = 0.02
		self.constant = 10

	def getSOSmsg(self):
		while True:
			try:
				msg = self.srv.listen()
				pkt = msg[0]['data']
				photoMsg = pysos.unpack('<BH',pkt)
				if photoMsg[0]==3:
					deltaV = self.constant*self.samplingtime*(photoMsg[1]-self.middle)
					self.velocity = self.velocity+deltaV
					deltaP = self.velocity*self.samplingtime
					self.position = self.position+deltaP
					#print photoMsg[1]-self.middle
					print photoMsg[1]
			except:
				pass
		


if(__name__ == "__main__"):

	dummy = photoModule()
	while True:
		pass
