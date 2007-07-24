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


	def getSOSmsg(self):
		while True:
			try:
				msg = self.srv.listen()
				pkt = msg[0]['data']
				photoMsg = pysos.unpack('<BH',pkt)
				if photoMsg[0]==1:
					print photoMsg
			except:
				pass
		


if(__name__ == "__main__"):

	dummy = photoModule()
	while True:
		pass
