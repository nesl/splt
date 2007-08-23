#!/usr/bin/python
import pysos

try:
  srv = pysos.sossrv()
except:
  print "Please run sos_server first"

while True:
  try:
    msg = srv.listen(type=36)
    pkt = msg[0]['data']
    addr = pysos.unpack('<b',pkt[0])
    print addr[0]
  except:
    pass

