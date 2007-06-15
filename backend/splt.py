#!/usr/bin/python

import sys
import time
import os
import Queue
import thread
from pysos import pysos
from Slog import spltSlog


# Message Queue
RFIDqueue = Queue.Queue(200)
PMqueue = Queue.Queue(200)




class spltModule:
	def __init__(self):
		try:
			self.srv = pysos.sossrv()
		except:
			print "Please run sos_server first"
		try:
			thread.start_new_thread(self.getSOSmsg,())
		except thread.error:
			pass
		try:
			thread.start_new_thread(self.getSloggingRFID, ())
		except thread.error:
			pass
		try:
			thread.start_new_thread(self.getSloggingPM, ())
		except thread.error:
			pass

		self.TID=['00000000','00000000','00000000','00000000','000000000','00000000','00000000','00000000']
		self.PM = ['000000','00000','000000','000000','000000','000000','000000']

		try:
			thread.start_new_thread(self.timerOUT, ())
		except thread.error:
			pass

	def getSOSmsg(self):
		while True:
			try:
				msg = self.srv.listen()
				pkt = msg[0]['data']
				SpltMsg = pysos.unpack('<BHLB',pkt[10:18])
				if SpltMsg[0] == 1: # PM
					if self.PM[SpltMsg[1]] != pkt[18:]:
						self.PM[SpltMsg[1]]=pkt[18:]
						temp =  pkt[18:].split(',')
						Data = (('A',temp[5]),('TimeStamp',time.time()),('V',temp[4]),('W',temp[3]),('AID',SpltMsg[1]))
						PMqueue.put(Data,True,0.5)
				if SpltMsg[0] == 2: # RFID
					if self.TID[SpltMsg[1]] != pkt[18:]:
						print pkt[18:]
						self.TID[SpltMsg[1]]=pkt[18:]
						Data = (('TID',self.TID[SpltMsg[1]].strip()),('TimeStamp',time.time()),('UID',SpltMsg[1]),('UserName','YH'))
						RFIDqueue.put(Data,True,0.5)				
			except:
				pass

		

	def getSloggingRFID(self):
		while True:
			try:
				data = RFIDqueue.get(False,1)
				spltSlog.spltSlog('RFID',data)
			except:
				pass


	def getSloggingPM(self):
		while True:
			try:
				data2 = PMqueue.get(False,1)
				spltSlog.spltSlog('PowerMeasure',data2)
			except:
				pass

	def timerOUT(self):
		while True:
			time.sleep(10)
			self.TID=['00000000','00000000','00000000','00000000','000000000','0000000','000000','000000']


if(__name__ == "__main__"):
	dummy = spltModule()
	while True:
		pass
