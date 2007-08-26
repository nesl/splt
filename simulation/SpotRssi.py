import pysos
import math
import thread
import time
import Queue
 
class SpotLight:
	def __init__(self):
		try:
			self.srv = pysos.sossrv()
		except:
			print "Please run sos_server first"
		try:
			thread.start_new_thread(self.parseSOSmsg,())
		except thread.error:
			pass

		try: 
			self.srv.register_trigger(self.getSOSmsg)
		except:
			pass
		try:
			thread.start_new_thread(self.TimeOut,())
		except thread.error:
			pass

		self.queue = Queue.Queue(200)

		self.Kimrssi=[-200,-200,-200,-200]
		self.Chorssi=[-200,-200,-200,-200]
		self.Kimcount = [0,0,0,0]

	def parse(self,msg):
		print msg['data']

	def TimeOut(self):
		while True:
			time.sleep(10)
			for i in range(4):
				if self.Kimcount[i]==0:
					self.Kimrssi[i]=-200
			#print self.Kimrssi
			print self.Kimcount
			self.Kimcount = [0,0,0,0]
			

	def getSOSmsg(self,msg):
		self.queue.put(msg)

	def parseSOSmsg(self):
		while True:
			try:
				msg = self.queue.get(True,10)
				pkt = msg['data']
			except:
				pass

			try:
				data = pysos.unpack('<BbB',pkt)
				temp = float(data[1])
# Moving average
                                alpha = 0.8
                                if data[0] == 2 and data[2] == 200:
                                        self.Chorssi[0] = self.Chorssi[0]*(1-alpha)+alpha*temp
                                elif data[0] == 3 and data[2] == 200:
                                        self.Chorssi[1] = self.Chorssi[1]*(1-alpha)+alpha*temp
                                elif data[0] == 7 and data[2] == 200:
                                        self.Chorssi[2] = self.Chorssi[2]*(1-alpha)+alpha*temp
                                elif data[0] == 8 and data[2] == 200:
                                        self.Chorssi[3] = self.Chorssi[3]*(1-alpha)+alpha*temp


                                elif data[0] == 2 and data[2] == 201:
                                        self.Kimrssi[0] = self.Kimrssi[0]*(1-alpha)+alpha*temp
					self.Kimcount[0] = self.Kimcount[0]+1
                                elif data[0] == 3 and data[2] == 201:
                                        self.Kimrssi[1] = self.Kimrssi[1]*(1-alpha)+alpha*temp
					self.Kimcount[1] = self.Kimcount[1]+1
                                elif data[0] == 7 and data[2] == 201:
                                        self.Kimrssi[2] = self.Kimrssi[2]*(1-alpha)+alpha*temp
					self.Kimcount[2] = self.Kimcount[2]+1
                                elif data[0] == 8 and data[2] == 201:
                                        self.Kimrssi[3] = self.Kimrssi[3]*(1-alpha)+alpha*temp
					self.Kimcount[3] = self.Kimcount[3]+1
			#	print data
			except:
			#	pass
				try:
					data = pysos.unpack('<B',pkt[0])
			#		print data
			#		print pkt[1:]
				except:
					pass



if __name__ == '__main__':               
    rssi = SpotLight()
    while True:
	pass                  
