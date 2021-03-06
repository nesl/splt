splt.py                                                                                             0100700 0000000 0000000 00000004061 00000000000 011044  0                                                                                                    ustar   root                            root                                                                                                                                                                                                                   #!/usr/bin/python

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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                               pysos/                                                                                              0040700 0000000 0000000 00000000000 00000000000 010664  5                                                                                                    ustar   root                            root                                                                                                                                                                                                                   pysos/__init__.py                                                                                   0100600 0000000 0000000 00000000025 00000000000 012771  0                                                                                                    ustar   root                            root                                                                                                                                                                                                                   from pysos import * 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           pysos/pysos.py                                                                                      0100600 0000000 0000000 00000041530 00000000000 012415  0                                                                                                    ustar   root                            root                                                                                                                                                                                                                   """
PySOS version 1.51 - March 22 2007
Authors: Thiago Teixeira and Neeraj Singh.

This module allows you to connect to a SOS server on any host
and port, as well as send and receive SOS messages.


INITIALIZATION:
===============

Make sure you are running the SOS server, then create a new instance:

>>> import pysos
>>> srv = pysos.sossrv()    # you may enter host=... and port=... here
>>>                         # localhost and 7915 are default


SENDING MESSAGES:
=================

There are 3 different ways to send messages. They aren't too 
different, and it's up to your personal preference which one to use:


1ST METHOD:
-----------

>>> data = pysos.pack('<BBBB', 0, 1, 2, 3)
>>> 
>>> srv.post(daddr = 5,   saddr = 3,  did  = 128, 
...          sid   = 128, type  = 32, data = data)

Any of these can be omitted, in which case the defaults specified 
with set_message_defaults() are utilized.


2ND METHOD:
-----------

This method is largely the same as the previous, but it separates
the message creation from the act of sending it:

>>> m = pysos.msg(daddr = 5,   saddr = 3,  did  = 128, 
...               sid   = 128, type  = 32, data = data)
>>>
>>> srv.post_msg(m)


3RD METHOD:
-----------

If you prefer to use SOS's post_net syntax, you may do so like this:

>>> srv.post_net(128, 128, 32, 4, data, 0, 5)

In this case, saddr is the one specified with set_message_defaults(),
or 0xFFFE by default. This is because post_net does not let you specify
your saddr in SOS.

Also note that the "length" and "flags" parameters are ignored.


RECEIVING MESSAGES:
===================

There are 2 different methods you can use. The first one is 
synchronous (blocking) and the 2nd asynchronous -- it allows you to 
register listeners and then run a non-blocking method to start listening 
for messages. You can use both of these methods with the same sossrv.
 

1ST METHOD (synchronous):
-------------------------

>>> msg = srv.listen(did   = 128,  sid   = 128, 
...                  daddr = 0x14, saddr = 0x16, 
...                  type  = 32,   nreplies = 5, timeout = 3.0)

This method returns the first matching messages. It returns the message as
a dictionary with keys 'did', 'sid', 'daddr', 'saddr', 'type, 'length',
and 'data'. To cast msg['data'] into a tuple, you may use the unpack()
method, as such:

>>> data = pysos.unpack('<LHB', msg['data'])

Where '<LHB' stands for a little endian ('<') struct composed of a
uint32 ('L'), uint16 ('H') and uint8 ('B'). For signed integers, use
lowercase letters instead. Of course, the string you feed into the unpack()
method depends on your particular data struct.


2ND METHOD (asynchronous):
--------------------------

For this method you register a trigger (i.e. listener). Then a thread
in the background will call a function of your choosing
when the trigger fires.

This is how you specify a trigger:

>>> srv.register_trigger(func,         did   = 128, 
...                      sid   = 128,  daddr = 0x14,
...                      saddr = 0x16, type  = 32)

Where you may omit any parameter (except func) to match all messages,
irrespective of that parameter's value. That is, None is a wildcard.

At any point, you may use the deregister_trigger() method to remove
triggers from the pysos instance. When deregistering a trigger, None
is once again used as wildcard.


RPC-STYLE COMMUNICATIONS:
=========================

You can also do an RPC-style call, which posts a message to the network 
and returns the response message(s):

>>> replylist = srv.post_rpc_msg(m, rtype=36, nreplies=10, timeout=5.0)

The above command creates a message dictionary (through sossrv.msg_dict)
which is sent to all the nodes. We collect up to 10 replies with message
type 36 in the variable msglist. The replies are in a list of message
dicts. If 5 seconds elapse, we just return the messages obtained thus 
far.

For those who do not wish to first create a message dict (the variable
called 'm' in the example above), there is the post_rpc() method:

>>> post_rpc(did    = 0x97, daddr  = 13, type   = 32,
...          rsid   = 0x97, rsaddr = 13, rtype  = 40,
...          timeout = 3, nreplies = 5)


MORE INFORMATION:
=================

Use each method's help function for more details.
"""

from threading import Thread, Lock, Condition
from struct import pack, unpack
import socket
from time import strftime

BCAST_ADDRESS = 0xFFFF  #move this to a higher scope, since it's useful

class sossrv:
    
	SOS_HEADER_SIZE = 8
	BCAST_ADDRESS   = 0xFFFF
	MOD_MSG_START   = 32

	def __init__(self, host=None, port=None, nid=0xFFFE, pid=128, verbose=False):

		if host: self.host = host
		else: self.host = 'localhost'

		if port: self.port = port 
		else: self.port = 7915

		self.daddr     = self.BCAST_ADDRESS
		self.saddr     = nid
		self.did       = pid
		self.sid       = pid
		self.type      = self.MOD_MSG_START
		self.verbose   = verbose 

		self._retries  = 0

		self._listening     = False
		self._listenthread  = None
		self._leftoverbytes = ''

		self._syncPkt  = []           # variable for passing packets to a sync listener
		self._syncTrig = None
		self._syncCV   = Condition()  # condition variable for sync receive

		self._triggers = []
		self._trgLock  = Lock()       # synchronizes reg/dereg against matching

		self._sock = None

		if self.verbose: print('Connecting to sossrv at %s:%d' % (self.host, self.port))
		self._connect()

		# necessary when running in fast systems to ensure that
		# pysos is connected to sossrv when the constructor returns
		while True:
			if self._listening: break


	def __del__(self):

		self.disconnect()


	def disconnect(self):

		if self.verbose: print('Disconnecting from sossrv...')

		if self._listening:
			self._sock.shutdown(socket.SHUT_RDWR)
			self._listening = False
			self._sock.close()

		if self._listenthread: self._listenthread.join()
	
	
	def reconnect(self, host=None, port=None):
		"""
		Run this if you restart sossrv and need to reconnect.
		"""
	
		if host == None : host = self.host
		if port == None : port = self.port
	
		self._connect(host=host, port=port, retries=self.retries)


	def set_msg_defaults(self, did=None, sid=None, daddr=None, saddr=None, type=None):
		"""
		Use this method to specify the default parameters for sending
		messages. This allows you to type less arguments when calling
		methods such as post() or msg().
		"""
	
		if did   == None : did   = self.did
		if sid   == None : sid   = self.sid
		if daddr == None : daddr = self.daddr
		if saddr == None : saddr = self.saddr
		if type  == None : type  = self.type


	def post(self, daddr=None, saddr=None, did=None, sid=None, type=None, data=None):
		"""
		Sends a message. Unspecified fields will be sent using the 
		defaults set when instantiating sossrv or by use of 
		set_message_defaults() method.

		The data argument must be a str (use the pack() method to 
		build strings from tuples).
		"""

		if did   == None : did   = self.did
		if sid   == None : sid   = self.sid
		if daddr == None : daddr = self.daddr
		if saddr == None : saddr = self.saddr
		if type  == None : type  = self.type
		if data  == None : data  = ''
		
		self._sock.send(pack('<BBHHBB', did, sid, daddr, saddr, type, len(data)) + data) 				


	def post_msg(self, msg):
			"""
			Sends a message dictionary, which can be produced by
			sossrv.msg()
			"""
			self._sock.send(pack('<BBHHBB', msg['did'] , msg['sid'], msg['daddr'],
			                msg['saddr']  , msg['type'], msg['len']) + msg['data'])


	def post_net(self, did, sid, type, length, data, flags, daddr):
		"""
		Sends a message where saddr and sid are the ones set when 
		initializing this instance or with set_msg_defaults().
		If none was specified in either of those times, saddr (nid) 
		defaults to 0xFFFE, and sid (pid) defaults to 128.

		The data argument must be a str (use the pack() method to 
		build strings from tuples).

		The flags argument is currently not used, and is only there
		for keeping the same syntax as SOS.
		"""

		if daddr == None: daddr = BCAST_ADDRESS
		self._sock.send(pack('<BBHHBB', did, sid, daddr, self.saddr, type, length) + data)


	# The r's are for "reply" to distinguish the trigger from the outgoing message
	def post_rpc(self, did    = None, sid    = None, daddr  = None, 
	                   saddr  = None, type   = None, data   = '',
	                   rdid   = None, rsid   = None, rdaddr = None, 
	                   rsaddr = None, rtype  = None,
	                   timeout = None, nreplies = 1, **etc):
		"""
		post_rpc(did    = 0x97, sid    = None, daddr  = None, 
		         saddr  = None, type   = None, data   = '',
		         rdid   = None, rsid   = None, rdaddr = None, 
		         rsaddr = None, rtype  = None,
				 timeout = 3, nreplies = 5)

		returns [msg0, msg1, ...]

		This method allows you to send a message and wait for the
		replies. The parameters starting with the letter 'r' describe
		what the expected reply looks like. The timeout parameter is 
		required when nreplies != 1.

		Although this method has an aweful lot of parameters, in 
		real-world scenarios only a handful will be used at a time.
		"""

		# We do the post_net inside the condition lock to prevent the (unlikely) race condition
		# that the packet comes back before we get a chance to read the reply packet

		if not self._listening:
			raise Exception, 'The sossrv instance is disconnected.'

		if nreplies > 1 and (not timeout):
			raise Exception, 'You should provide a timeout if you want multiple replies.'

		# Only one thread can be synchronously listening at a time
		assert self._syncTrig == None and self._syncPkt == [] 
		
		ret = []
		
		self._syncCV.acquire()
		self._syncTrig = (rdid, rsid, rdaddr, rsaddr, rtype, None)  # make a trigger packet

		self.post(daddr=daddr, saddr=saddr, did=did, sid=sid, type=type, data=data)

		while nreplies > 0:
			self._syncCV.wait(timeout)

			if len(self._syncPkt):
				# this is None on timeout or error. (hdr, data) otherwise
				nreplies -= len(self._syncPkt)
				msgdict = [_make_msg_dict(*pkt) for pkt in self._syncPkt]
				ret.extend(msgdict)
				self._syncPkt = []
			else: break

		self._syncTrig = None
		self._syncCV.release()

		return ret		
				

	def post_rpc_msg(self, msgdict,
	                 rdid   = None, rsid  = None, rdaddr  = None, 
	                 rsaddr = None, rtype = None, timeout = None, 
					 nreplies = 1):
		"""
		post_rpc_msg(msgdict,
	                 rdid   = None, rsid  = None, rdaddr  = None, 
	                 rsaddr = None, rtype = None, timeout = None,
					 nreplies = 1)

		returns [msg0, msg1, ...]

		This method allows you to send a message and wait for the
		replies. The parameters starting with the letter 'r' describe
		what the expected reply looks like. The timeout parameter is 
		required when nreplies != 1.

		msgdict is a dictionary rescribing the message and can be
		created with the msg() method.
		"""

		return self.post_rpc(
				did    = msgdict['did'], 
				sid    = msgdict['sid'],
				daddr  = msgdict['daddr'], 
				saddr  = msgdict['saddr'],
				type   = msgdict['type'],
				data   = msgdict['data'],
				rdid   = rdid,   rsid  = rsid,  rdaddr  = rdaddr, 
				rsaddr = rsaddr, rtype = rtype, timeout = timeout,
				nreplies = nreplies)


	def listen(self, did=None, sid=None, daddr=None, saddr=None, type=None, timeout=None, nreplies=1):
		"""
		listen(did=None, sid=None, daddr=None, saddr=None,
		       type=None, timeout=None, nreplies=1) 
			   
		returns [msg0, msg1, ...]

		This is a blocking method that returns the first matching message.
		Timeout is specified in floating point seconds.
		"""

		t = (did, sid, daddr, saddr, type)

		if not self._listening:
			raise Exception, 'The sossrv instance is disconnected.?'

		if nreplies > 1 and (not timeout):
			raise Exception, 'You should provide a timeout if you want multiple replies.'

		# Only one thread can be synchronously listening at a time
		assert self._syncTrig == None and self._syncPkt == []
		
		ret = []

		self._syncCV.acquire()
		self._syncTrig = t
		
		while nreplies > 0:
			self._syncCV.wait(timeout)
			nreplies -= len(self._syncPkt)
			if len(self._syncPkt):
				# this is None on timeout or error. (hdr, data) otherwise
				msgdict = [_make_msg_dict(*pkt) for pkt in self._syncPkt]
				ret.extend(msgdict)
				self._syncPkt = []

			else: break

		self._syncTrig = None
		self._syncCV.release()

		return ret
                
	
	def register_trigger(self, func, did=None, sid=None, daddr=None, saddr=None, type=None):
		"""
		Register a trigger (i.e. listener) to execute 'func(hdr, data)' upon
		receiving a matching message.'.

		NOTE: Because of the way Python works, if you change a function *after* registering 
		it, you must deregister then re-register it!
		"""
		
		self._trgLock.acquire()
		self._triggers.append((did, sid, daddr, saddr, type, func))
		self._trgLock.release()


	def deregister_trigger(self, func=None, did=None, sid=None, daddr=None, saddr=None, type=None):
		"""
		Deregisters ALL matching triggers, so if no arguments are provided
		all triggers are deregistered. 
		"""

		pattern = (did, sid, daddr, saddr, type, func)
		newtriggers = []

		self._trgLock.acquire()

		for t in self._triggers:
			if not _match_trigger(t, pattern): newtriggers.append(t)

		self._triggers = newtriggers
		self._trgLock.release()


	def msg(self, daddr=None, saddr=None, did=None, sid=None, type=None, data=''):
		"""
		Returns a properly-formatted message dictionary with default values 
		for any parameters not provided. Defaults can be set with set_defaults().
		"""

		if did == None   : did   = self.did
		if sid == None   : sid   = self.sid
		if daddr == None : daddr = self.daddr
		if saddr == None : saddr = self.saddr
		if type == None  : type  = self.type

		return {'did'   : did,     'sid'   : sid,  'daddr'  : daddr,
		        'saddr' : saddr,   'type'  : type, 'length' : len(data),
		        'data'  : data,    'flags' : 0}


	def _connect(self, host=None, port=None, retries=0):
		"""
		Internal: connects to the sossrv
		"""

		if host == None: host = self.host
		else: self.host = host
		
		if port == None: port = self.port
		else: self.port = port
		
		self.retries = retries

		self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		
		while retries >= 0:
			try:
				self._sock.connect((host, port))
				retries = -1
			except Exception, e:
				if retries <= 0: raise IOError, "Can't connect to SOS server: " + str(e)
				retries -= 1
				continue

		self._start_listening()



	def _start_listening(self):
		"""
		Internal: Starts a thread to listen to all incomming messages. This must 
		be called in order to receive any data.
		"""
		if (self._listenthread == None) or (not self._listenthread.isAlive()):
			self._listenthread = Thread(name='SOSSRVpktrecv', target=self._listen_for_triggers)
			self._listenthread.setDaemon(True)
			self._listenthread.start()


	def _receive_message_tuple(self):
		"""
		Internal: Returns the next packet received by the SOS server as a header
		tuple and data str. Use the unpack() method to cast the data 
		into the required format.
		"""
		
		hdrstr = self._leftoverbytes
		tmpbuf = ''
		
		while len(hdrstr) < self.SOS_HEADER_SIZE :
			tmpbuf = self._sock.recv(self.SOS_HEADER_SIZE - len(tmpbuf))
			if tmpbuf != '' : hdrstr += tmpbuf
			else : raise socket.error, 'Socket reset by peer.'
		
		# we do this in case we get a bit of the data string along with hdrstr
		hdrstr = hdrstr[:self.SOS_HEADER_SIZE]
		data   = hdrstr[self.SOS_HEADER_SIZE:]
		
		hdr = unpack('<BBHHBB', hdrstr)
		
		datalen = hdr[5]
		
		while len(data) < datalen:
			tmpbuf = self._sock.recv(datalen - len(data))
			if tmpbuf == '' : raise socket.error, 'Socket reset by peer.'
			data += tmpbuf 

		data = data[:datalen]
		self._leftoverbytes = data[datalen:]


		return hdr, data


	def _listen_for_triggers(self):
		"""
		Internal listener thread procedure.
		"""

		self._listening = True
		self._leftoverbytes = ''

		while self._listening:

			try: hdr, data = self._receive_message_tuple()

			except socket.error:
				if self.verbose: print('Connection to sossrv at %s:%d broken.' % (self.host, self.port))
				break

			self._trgLock.acquire()

			for t in self._triggers:
				if _match_packet(hdr, t):
					t[5](_make_msg_dict(hdr, data))

			self._trgLock.release()
			self._syncCV.acquire()

			if self._syncTrig != None:
				if _match_packet(hdr, self._syncTrig):
					self._syncPkt.append((hdr, data))
					self._syncCV.notify()

			self._syncCV.release()

		self._listening = False


def _make_msg_dict(hdr, data):
	"""
	Gets the header and data string as a dictionary for easier display and parsing.
	"""
	return {'did'   : hdr[0], 'sid'  : hdr[1], 'daddr'  : hdr[2],
	        'saddr' : hdr[3], 'type' : hdr[4], 'length' : hdr[5],
	        'data'  : data}


def _match_packet(hdr, pattern):
	"""
	Returns True if the hdr matches the pattern.
	"""
	for i in xrange(5):
		if (pattern[i]) and (hdr[i] != pattern[i]): return False

	return True


def _match_trigger(trigger, pattern):
	"""
	Returns True if the trigger matches the pattern.
	"""
	for i in xrange(6):
		if (pattern[i]) and (trigger[i] != pattern[i]): return False

	return True

                                                                                                                                                                        pysos/__init__.pyc                                                                                  0100600 0000000 0000000 00000000203 00000000000 013132  0                                                                                                    ustar   root                            root                                                                                                                                                                                                                   mò
    c           @   s   d  k  Td S(   (   t   *N(   t   pysos(    (    (    t   /root/pysos/__init__.pyt   ?   s                                                                                                                                                                                                                                                                                                                                                                                                 pysos/pysos.pyc                                                                                     0100600 0000000 0000000 00000042507 00000000000 012565  0                                                                                                    ustar   root                            root                                                                                                                                                                                                                   mò
    c           @   s   d  Z  d k l Z l Z l Z d k l Z l Z d k Z d k	 l
 Z
 d Z d f  d     YZ d   Z d	   Z d
   Z d S(   s  
PySOS version 1.51 - March 22 2007
Authors: Thiago Teixeira and Neeraj Singh.

This module allows you to connect to a SOS server on any host
and port, as well as send and receive SOS messages.


INITIALIZATION:
===============

Make sure you are running the SOS server, then create a new instance:

>>> import pysos
>>> srv = pysos.sossrv()    # you may enter host=... and port=... here
>>>                         # localhost and 7915 are default


SENDING MESSAGES:
=================

There are 3 different ways to send messages. They aren't too 
different, and it's up to your personal preference which one to use:


1ST METHOD:
-----------

>>> data = pysos.pack('<BBBB', 0, 1, 2, 3)
>>> 
>>> srv.post(daddr = 5,   saddr = 3,  did  = 128, 
...          sid   = 128, type  = 32, data = data)

Any of these can be omitted, in which case the defaults specified 
with set_message_defaults() are utilized.


2ND METHOD:
-----------

This method is largely the same as the previous, but it separates
the message creation from the act of sending it:

>>> m = pysos.msg(daddr = 5,   saddr = 3,  did  = 128, 
...               sid   = 128, type  = 32, data = data)
>>>
>>> srv.post_msg(m)


3RD METHOD:
-----------

If you prefer to use SOS's post_net syntax, you may do so like this:

>>> srv.post_net(128, 128, 32, 4, data, 0, 5)

In this case, saddr is the one specified with set_message_defaults(),
or 0xFFFE by default. This is because post_net does not let you specify
your saddr in SOS.

Also note that the "length" and "flags" parameters are ignored.


RECEIVING MESSAGES:
===================

There are 2 different methods you can use. The first one is 
synchronous (blocking) and the 2nd asynchronous -- it allows you to 
register listeners and then run a non-blocking method to start listening 
for messages. You can use both of these methods with the same sossrv.
 

1ST METHOD (synchronous):
-------------------------

>>> msg = srv.listen(did   = 128,  sid   = 128, 
...                  daddr = 0x14, saddr = 0x16, 
...                  type  = 32,   nreplies = 5, timeout = 3.0)

This method returns the first matching messages. It returns the message as
a dictionary with keys 'did', 'sid', 'daddr', 'saddr', 'type, 'length',
and 'data'. To cast msg['data'] into a tuple, you may use the unpack()
method, as such:

>>> data = pysos.unpack('<LHB', msg['data'])

Where '<LHB' stands for a little endian ('<') struct composed of a
uint32 ('L'), uint16 ('H') and uint8 ('B'). For signed integers, use
lowercase letters instead. Of course, the string you feed into the unpack()
method depends on your particular data struct.


2ND METHOD (asynchronous):
--------------------------

For this method you register a trigger (i.e. listener). Then a thread
in the background will call a function of your choosing
when the trigger fires.

This is how you specify a trigger:

>>> srv.register_trigger(func,         did   = 128, 
...                      sid   = 128,  daddr = 0x14,
...                      saddr = 0x16, type  = 32)

Where you may omit any parameter (except func) to match all messages,
irrespective of that parameter's value. That is, None is a wildcard.

At any point, you may use the deregister_trigger() method to remove
triggers from the pysos instance. When deregistering a trigger, None
is once again used as wildcard.


RPC-STYLE COMMUNICATIONS:
=========================

You can also do an RPC-style call, which posts a message to the network 
and returns the response message(s):

>>> replylist = srv.post_rpc_msg(m, rtype=36, nreplies=10, timeout=5.0)

The above command creates a message dictionary (through sossrv.msg_dict)
which is sent to all the nodes. We collect up to 10 replies with message
type 36 in the variable msglist. The replies are in a list of message
dicts. If 5 seconds elapse, we just return the messages obtained thus 
far.

For those who do not wish to first create a message dict (the variable
called 'm' in the example above), there is the post_rpc() method:

>>> post_rpc(did    = 0x97, daddr  = 13, type   = 32,
...          rsid   = 0x97, rsaddr = 13, rtype  = 40,
...          timeout = 3, nreplies = 5)


MORE INFORMATION:
=================

Use each method's help function for more details.
(   s   Threads   Locks	   Condition(   s   packs   unpackN(   s   strftimeiÿÿ  t   sossrvc           B   s  t  Z d Z d Z d Z d  d  d d e d  Z d   Z d   Z	 d  d  d	  Z
 d  d  d  d  d  d
  Z d  d  d  d  d  d  d  Z d   Z d   Z d  d  d  d  d  d d  d  d  d  d  d  d d  Z d  d  d  d  d  d  d d  Z d  d  d  d  d  d  d d  Z d  d  d  d  d  d  Z d  d  d  d  d  d  d  Z d  d  d  d  d  d d  Z d  d  d d  Z d   Z d   Z d   Z RS(   Ni   iÿÿ  i    iþÿ  i   c         C   s%  | o | |  _  n
 d |  _  | o | |  _ n
 d |  _ |  i |  _ | |  _ | |  _ | |  _	 |  i
 |  _ | |  _ d |  _ t |  _ d  |  _ d |  _ g  |  _ d  |  _ t   |  _ g  |  _ t   |  _ d  |  _ |  i o d |  i  |  i f GHn |  i   x t o |  i o PqqWd  S(   Nt	   localhostië  i    t    s   Connecting to sossrv at %s:%d(   t   hostt   selft   portt   BCAST_ADDRESSt   daddrt   nidt   saddrt   pidt   didt   sidt   MOD_MSG_STARTt   typet   verboset   _retriest   Falset
   _listeningt   Nonet   _listenthreadt   _leftoverbytest   _syncPktt	   _syncTrigt	   Conditiont   _syncCVt	   _triggerst   Lockt   _trgLockt   _sockt   _connectt   True(   R   R   R   R   R
   R   (    (    t   /root/pysos/pysos.pyt   __init__   s:     	 													
 
 
 c         C   s   |  i   d  S(   N(   R   t
   disconnect(   R   (    (    R    t   __del__Â   s    c         C   si   |  i o	 d GHn |  i o- |  i i t i  t |  _ |  i i   n |  i	 o |  i	 i
   n d  S(   Ns   Disconnecting from sossrv...(   R   R   R   R   t   shutdownt   sockett	   SHUT_RDWRR   t   closeR   t   join(   R   (    (    R    R"   Ç   s    
 	
	
 c         C   sW   | d j o |  i  } n | d j o |  i } n |  i d | d | d |  i  d S(   s;   
		Run this if you restart sossrv and need to reconnect.
		R   R   t   retriesN(   R   R   R   R   R   R)   (   R   R   R   (    (    R    t	   reconnectÓ   s       c         C   s   | d j o |  i  } n | d j o |  i } n | d j o |  i } n | d j o |  i } n | d j o |  i } n d S(   s¦   
		Use this method to specify the default parameters for sending
		messages. This allows you to type less arguments when calling
		methods such as post() or msg().
		N(   R   R   R   R   R   R	   R   (   R   R   R   R   R	   R   (    (    R    t   set_msg_defaultsÞ   s          c      
   C   sÏ   | d j o |  i  } n | d j o |  i } n | d j o |  i } n | d j o |  i } n | d j o |  i } n | d j o
 d } n |  i i	 t
 d | | | | | t |   |  d S(   sõ   
		Sends a message. Unspecified fields will be sent using the 
		defaults set when instantiating sossrv or by use of 
		set_message_defaults() method.

		The data argument must be a str (use the pack() method to 
		build strings from tuples).
		R   s   <BBHHBBN(   R   R   R   R   R   R	   R   t   dataR   t   sendt   packt   len(   R   R   R	   R   R   R   R,   (    (    R    t   postì   s           
c      
   C   sL   |  i i t d | d | d | d | d | d | d  | d  d	 S(
   sL   
			Sends a message dictionary, which can be produced by
			sossrv.msg()
			s   <BBHHBBR   R   R   R	   R   R/   R,   N(   R   R   R-   R.   t   msg(   R   R1   (    (    R    t   post_msg   s     $c      	   C   sJ   | d j o
 t } n |  i i t d | | | |  i	 | |  |  d S(   s®  
		Sends a message where saddr and sid are the ones set when 
		initializing this instance or with set_msg_defaults().
		If none was specified in either of those times, saddr (nid) 
		defaults to 0xFFFE, and sid (pid) defaults to 128.

		The data argument must be a str (use the pack() method to 
		build strings from tuples).

		The flags argument is currently not used, and is only there
		for keeping the same syntax as SOS.
		s   <BBHHBBN(   R   R   R   R   R   R-   R.   R   R   R	   R   t   lengthR,   (   R   R   R   R   R3   R,   t   flagsR   (    (    R    t   post_net	  s      
R   i   c         K   sc  |  i p t d  n | d j o | o t d  n |  i d j o |  i g  j p t  g  } |  i
 i   | | |	 |
 | d f |  _ |  i d | d | d | d | d | d	 |  x | d
 j o| |  i
 i |  t |  i  oW | t |  i  8} g  } |  i D] } | t |   q~ } | i |  g  |  _ q¿ Pq¿ Wd |  _ |  i
 i   | S(   sh  
		post_rpc(did    = 0x97, sid    = None, daddr  = None, 
		         saddr  = None, type   = None, data   = '',
		         rdid   = None, rsid   = None, rdaddr = None, 
		         rsaddr = None, rtype  = None,
				 timeout = 3, nreplies = 5)

		returns [msg0, msg1, ...]

		This method allows you to send a message and wait for the
		replies. The parameters starting with the letter 'r' describe
		what the expected reply looks like. The timeout parameter is 
		required when nreplies != 1.

		Although this method has an aweful lot of parameters, in 
		real-world scenarios only a handful will be used at a time.
		s$   The sossrv instance is disconnected.i   s:   You should provide a timeout if you want multiple replies.R   R	   R   R   R   R,   i    N(    R   R   t	   Exceptiont   nrepliest   timeoutR   R   R   t   AssertionErrort   retR   t   acquiret   rdidt   rsidt   rdaddrt   rsaddrt   rtypeR0   R   R	   R   R   R   R,   t   waitR/   t   _[1]t   pktt   _make_msg_dictt   msgdictt   extendt   release(   R   R   R   R   R	   R   R,   R<   R=   R>   R?   R@   R8   R7   t   etcRC   RB   R:   RE   (    (    R    t   post_rpc  s,     
'. *	c	   	      C   sp   |  i d | d d | d d | d d | d d | d d | d d | d | d	 | d
 | d | d | d |  S(   s  
		post_rpc_msg(msgdict,
	                 rdid   = None, rsid  = None, rdaddr  = None, 
	                 rsaddr = None, rtype = None, timeout = None,
					 nreplies = 1)

		returns [msg0, msg1, ...]

		This method allows you to send a message and wait for the
		replies. The parameters starting with the letter 'r' describe
		what the expected reply looks like. The timeout parameter is 
		required when nreplies != 1.

		msgdict is a dictionary rescribing the message and can be
		created with the msg() method.
		R   R   R   R	   R   R,   R<   R=   R>   R?   R@   R8   R7   N(
   R   RI   RE   R<   R=   R>   R?   R@   R8   R7   (	   R   RE   R<   R=   R>   R?   R@   R8   R7   (    (    R    t   post_rpc_msgW  s     




c         C   s8  | | | | | f } |  i p t d  n | d j o | o t d  n |  i d j o |  i g  j p t  g  }
 |  i i   | |  _ x | d j o| |  i i |  | t |  i  8} t |  i  oD g  } |  i D] }	 | t |	   qâ ~ } |
 i |  g  |  _ q Pq Wd |  _ |  i i   |
 S(   s  
		listen(did=None, sid=None, daddr=None, saddr=None,
		       type=None, timeout=None, nreplies=1) 
			   
		returns [msg0, msg1, ...]

		This is a blocking method that returns the first matching message.
		Timeout is specified in floating point seconds.
		s%   The sossrv instance is disconnected.?i   s:   You should provide a timeout if you want multiple replies.i    N(   R   R   R   R	   R   t   tR   R   R6   R7   R8   R   R   R   R9   R:   R   R;   RA   R/   RB   RC   RD   RE   RF   RG   (   R   R   R   R   R	   R   R8   R7   RE   RC   R:   RB   RK   (    (    R    t   listenx  s,    	 
'	 *	c         C   s@   |  i i   |  i i | | | | | | f  |  i i   d S(   só   
		Register a trigger (i.e. listener) to execute 'func(hdr, data)' upon
		receiving a matching message.'.

		NOTE: Because of the way Python works, if you change a function *after* registering 
		it, you must deregister then re-register it!
		N(   R   R   R;   R   t   appendR   R   R   R	   R   t   funcRG   (   R   RN   R   R   R   R	   R   (    (    R    t   register_trigger¤  s     "c   
      C   sz   | | | | | | f } g  } |  i	 i
   x2 |  i D]' }	 t |	 |  p | i |	  q5 q5 W| |  _ |  i	 i   d S(   sj   
		Deregisters ALL matching triggers, so if no arguments are provided
		all triggers are deregistered. 
		N(   R   R   R   R	   R   RN   t   patternt   newtriggersR   R   R;   R   RK   t   _match_triggerRM   RG   (
   R   RN   R   R   R   R	   R   RP   RQ   RK   (    (    R    t   deregister_trigger²  s     
  	c         C   sÔ   | d
 j o |  i  } n | d
 j o |  i } n | d
 j o |  i } n | d
 j o |  i } n | d
 j o |  i } n h  d | <d | <d | <d | <d | <d t |  <d | <d d	 <S(   s   
		Returns a properly-formatted message dictionary with default values 
		for any parameters not provided. Defaults can be set with set_defaults().
		R   R   R   R	   R   R3   R,   R4   i    N(	   R   R   R   R   R   R	   R   R/   R,   (   R   R   R	   R   R   R   R,   (    (    R    R1   Ä  s          i    c         C   sö   | d j o |  i  } n
 | |  _  | d j o |  i } n
 | |  _ | |  _ t i t i t i  |  _ x{ | d j om y  |  i i	 | | f  d } Wqm t
 j
 o: } | d j o t d t |   n | d 8} qm qm Xqm W|  i   d S(   s&   
		Internal: connects to the sossrv
		i    iÿÿÿÿs   Can't connect to SOS server: i   N(   R   R   R   R   R)   R%   t   AF_INETt   SOCK_STREAMR   t   connectR6   t   et   IOErrort   strt   _start_listening(   R   R   R   R)   RW   (    (    R    R   Õ  s(      	 		 
 
c         C   sa   |  i d j p |  i i   o< t d d d |  i  |  _ |  i i t  |  i i   n d S(   sy   
		Internal: Starts a thread to listen to all incomming messages. This must 
		be called in order to receive any data.
		t   namet   SOSSRVpktrecvt   targetN(	   R   R   R   t   isAlivet   Threadt   _listen_for_triggerst	   setDaemonR   t   start(   R   (    (    R    RZ   ñ  s
     !c         C   s%  |  i } d } xa t |  |  i j  oJ |  i i |  i t |   } | d j o | | 7} q t i	 d  q W| |  i  } | |  i } t d |  } | d } x[ t |  | j  oG |  i i | t |   } | d j o t i	 d  n | | 7} q© W| |  } | | |  _ | | f S(   sª   
		Internal: Returns the next packet received by the SOS server as a header
		tuple and data str. Use the unpack() method to cast the data 
		into the required format.
		R   s   Socket reset by peer.s   <BBHHBBi   N(   R   R   t   hdrstrt   tmpbufR/   t   SOS_HEADER_SIZER   t   recvR%   t   errorR,   t   unpackt   hdrt   datalen(   R   R,   Rc   Rj   Rd   Ri   (    (    R    t   _receive_message_tupleü  s,     	  
  
c         C   sE  t  |  _ d |  _ x#|  i oy |  i   \ } } Wn: t i j
 o+ |  i	 o d |  i
 |  i f GHn Pn X|  i i   x< |  i D]1 } t | |  o | d t | |   q q W|  i i   |  i i   |  i d j o> t | |  i  o' |  i i | | f  |  i i   q&n |  i i   q Wt |  _ d S(   s)   
		Internal listener thread procedure.
		R   s%   Connection to sossrv at %s:%d broken.i   N(   R   R   R   R   Rk   Ri   R,   R%   Rg   R   R   R   R   R;   R   RK   t   _match_packetRD   RG   R   R   R   R   RM   t   notifyR   (   R   Ri   RK   R,   (    (    R    R`     s0     		 
 
 
 (   t   __name__t
   __module__Re   R   R   R   R   R!   R#   R"   R*   R+   R0   R2   R5   RI   RJ   RL   RO   RS   R1   R   RZ   Rk   R`   (    (    (    R    R       s*   '					0;!,		#c         C   s[   h  d |  d <d |  d <d |  d <d |  d <d	 |  d
 <d |  d <d | <S(   sS   
	Gets the header and data string as a dictionary for easier display and parsing.
	R   i    R   i   R   i   R	   i   R   i   R3   i   R,   N(   Ri   R,   (   Ri   R,   (    (    R    RD   B  s     c         C   sC   x< t  d  D]. } | | o |  | | | j o t Sq q Wt S(   s0   
	Returns True if the hdr matches the pattern.
	i   N(   t   xranget   iRP   Ri   R   R   (   Ri   RP   Rq   (    (    R    Rl   K  s        c         C   sC   x< t  d  D]. } | | o |  | | | j o t Sq q Wt S(   s4   
	Returns True if the trigger matches the pattern.
	i   N(   Rp   Rq   RP   t   triggerR   R   (   Rr   RP   Rq   (    (    R    RR   U  s        (   t   __doc__t	   threadingR_   R   R   t   structR.   Rh   R%   t   timet   strftimeR   R    RD   Rl   RR   (   R    R%   R_   R   Rl   R   RR   RD   Rw   Rh   R   R.   (    (    R    t   ?   s   	ÿ ®			
                                                                                                                                                                                         Slog/                                                                                               0040755 0001750 0001750 00000000000 00000000000 011561  5                                                                                                    ustar   default                         default                                                                                                                                                                                                                Slog/XMLwrap.pyc                                                                                    0100600 0000000 0000000 00000001510 00000000000 012456  0                                                                                                    ustar   root                            root                                                                                                                                                                                                                   mò
    c           @   s(   d    Z  d   Z d   Z d   Z d S(   c         C   s   d |  d S(   Ns   	<row>
	s   
	</row>(   t   string(   R    (    (    t   /root/Slog/XMLwrap.pyt   Row   s    c         C   s   d |  d S(   Ns   <table>
s	   
</table>(   R    (   R    (    (    R   t   Table   s    c         C   s7   d } x* |  D]" } | d | d | d f } q W| S(   Nt    s   <field name ="%s">%s</field>i    i   (   t   datat
   Name_Valuet   items(   R   R   R   (    (    R   t   Field   s
      c         C   s3   d } t |   } t |  } t |  } | GH| S(   NR   (   R   R   R   R   R   (   R   R   (    (    R   t   Wrap   s    N(   R   R   R   R	   (   R   R   R	   R   (    (    R   t   ?   s   			                                                                                                                                                                                        Slog/spltSlog.pyc                                                                                   0100600 0000000 0000000 00000001170 00000000000 012735  0                                                                                                    ustar   root                            root                                                                                                                                                                                                                   mò
    c           @   s7   d  k  Z  d  k Z d Z d Z d Z d Z d   Z d  S(   Ns   kimyh@ucla.edut   passwordt   154t   PowerMeasurec         C   sL   t  i   } t i |  } | i t t	 t
 |   | i |  | i   d  S(   N(   t
   SlogModulet   DataSlogt   spltSlogt   XMLwrapt   Wrapt   stringt   datat   ChangeDBt   emailR    t
   project_idt	   tablenamet	   ChangeXMLt   Slog(   R   R   R   R	   (    (    t   /root/Slog/spltSlog.pyR      s
    (   R   R   R   R    R   t   tableR   (   R    R   R   R   R   R   R   (    (    R   t   ?   s   		                                                                                                                                                                                                                                                                                                                                                                                                        Slog/XMLwrap.py                                                                                     0100644 0001750 0001750 00000000603 00000000000 013461  0                                                                                                    ustar   default                         default                                                                                                                                                                                                                def Row(string):
	return "\t<row>\n\t"+string+"\n\t</row>"

def Table(string):
	return "<table>\n"+string+"\n</table>"

def Field(Name_Value):
	data = ''
	for items in Name_Value:
		data = data+ "<field name =\"%s\">%s</field>"%(items[0],items[1]) 
	return data

def Wrap(Name_Value):
	data = ''
	data = Field(Name_Value)
	data = Row(data)
	data = Table(data)
	print data
	return data
	
                                                                                                                             Slog/SlogModule.pyc                                                                                 0100600 0000000 0000000 00000003546 00000000000 013211  0                                                                                                    ustar   root                            root                                                                                                                                                                                                                   mò
    c           @   sM   d  k  Z  d  k Z d  k Z d  k Z d  k Z d  k Z d f  d     YZ d  S(   Nt   DataSlogc           B   s5   t  Z d   Z d   Z d   Z d   Z d   Z RS(   Nc         C   s?   d |  _ d |  _ d |  _ d |  _ d |  _ y Wn n Xd  S(   Nt    (   t   selft   sb_emailt   sb_passwordt   sb_project_idt   sb_tablet   xml(   R   (    (    t   /root/Slog/SlogModule.pyt   __init__   s    					c         C   s·   d } h  d |  i <d |  i <d |  i <d |  i <d d <d |  i <} | GHyN t i	 |  } t i | |  } t i |  } d	 | i   } | i   Wn n X| GHd  S(
   Ns&   http://sensorbase.org/alpha/upload.phpt   emailt   pwt
   project_idt   data_stringt   typeR   t	   tableNames   DATA POST result: (   t   sb_apiR   R   R   R   R   R   t   paramt   urllibt	   urlencodet   datat   urllib2t   Requestt   reqt   urlopent   responset   readt
   SlogResultt   close(   R   R   R   R   R   R   R   (    (    R   t   Slog   s    Kc         C   s(   | |  _  | |  _ | |  _ | |  _ d  S(   N(   R   R   R   R   R   (   R   R   R   R   R   (    (    R   t   ChangeDB.   s    			c         C   s   | |  _ d  S(   N(   t   XMLR   R   (   R   R   (    (    R   t	   ChangeXML8   s    c         C   sU   | d } | d } |  i | d | d | d | d  |  i |  |  i   d  S(   Ni    i   i   i   (   t   DBt   DBentryt   DBdataR   R   R    R   (   R   R!   R#   R"   (    (    R   t   WrapSlog<   s
    

&(   t   __name__t
   __module__R	   R   R   R    R$   (    (    (    R   R       s
   			
	(   t   threadt   syst   osR   R   t   timeR    (   R'   R    R   R   R(   R*   R)   (    (    R   t   ?   s
   				                                                                                                                                                          Slog/__init__.py                                                                                    0100644 0001750 0001750 00000000000 00000000000 013655  0                                                                                                    ustar   default                         default                                                                                                                                                                                                                Slog/SlogModule.py                                                                                  0100644 0001750 0001750 00000003201 00000000000 014176  0                                                                                                    ustar   default                         default                                                                                                                                                                                                                #!/usr/bin/python

import thread
import sys,os
import urllib
import urllib2
import time



## Assumption : There is only one type

class DataSlog:
    def __init__(self):
        self.sb_email = ''
        self.sb_password = ''
        self.sb_project_id = ''
        self.sb_table = ''
	self.xml = ''
        try:
            pass
        except:
	    pass

    def Slog(self):
        
        sb_api = 'http://sensorbase.org/alpha/upload.php' # the interface of sensorbase used for uploading data
        param = {'email' : self.sb_email,
                      'pw' : self.sb_password,
                      'project_id' : self.sb_project_id,
                      'data_string': self.xml,
                      'type':'xml',
                      'tableName': self.sb_table}
	print param
        try: 
		data = urllib.urlencode(param)
	        req = urllib2.Request(sb_api, data)
	        response = urllib2.urlopen(req)
	        SlogResult = "DATA POST result: " + response.read()
	        response.close()
        except:
		pass
	print SlogResult


    def ChangeDB(self,sb_email,sb_password,sb_project_id,sb_table):
        self.sb_email = sb_email
        self.sb_password = sb_password
        self.sb_project_id = sb_project_id
        self.sb_table = sb_table
        #print self.sb_email
        #print self.sb_password
        #print self.sb_project_id
        #print self.sb_table

    def ChangeXML(self,XML):
	self.xml = XML


    def WrapSlog(self,DB):
	DBentry = DB[0]
	DBdata = DB[1]
	self.ChangeDB(DBentry[0],DBentry[1],DBentry[2],DBentry[3])
	self.ChangeXML(DBdata)
	self.Slog()
                                                                                                                                                                                                                                                                                                                                                                                               Slog/__init__.pyc                                                                                   0100600 0000000 0000000 00000000146 00000000000 012667  0                                                                                                    ustar   root                            root                                                                                                                                                                                                                   mò
    c           @   s   d  S(   N(    (    (    (    t   /root/Slog/__init__.pyt   ?    s                                                                                                                                                                                                                                                                                                                                                                                                                              Slog/spltSlog.py                                                                                    0100644 0001750 0001750 00000000617 00000000000 013743  0                                                                                                    ustar   default                         default                                                                                                                                                                                                                import SlogModule
import XMLwrap

#Account information
email='kimyh@ucla.edu'
password = 'password'
project_id = '154'
table = 'PowerMeasure'

#Date and Time / follows the format YYYY-MM-DD HH:MM:SS



def spltSlog(tablename,string):
	spltSlog = SlogModule.DataSlog()
	data = XMLwrap.Wrap(string)
	spltSlog.ChangeDB(email,password,project_id,tablename)
	spltSlog.ChangeXML(data)
	spltSlog.Slog()

	
                                                                                                                                                        