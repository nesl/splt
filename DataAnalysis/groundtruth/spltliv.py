#!/usr/bin/python

import time

a=open('spltliv.txt')

for line in a:
	b=line.strip()
	b=b.split(',')
	try:
		epoch = time.mktime(time.strptime(b[2],"%Y-%m-%d %H:%M:%S"))
		print epoch
		time.sleep(0.05)
	except:
		print "Maybe the first line"


a.close()	
