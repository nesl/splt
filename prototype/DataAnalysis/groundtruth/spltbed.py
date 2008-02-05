#!/usr/bin/python

import time

#a=open('spltbed.txt')
a=open('spltliv.txt')
temp = 0
for line in a:
	b=line.strip()
	b=b.split(',')
	try:
		epoch = time.mktime(time.strptime(b[2],"%Y-%m-%d %H:%M:%S"))
		if (temp-epoch) < -30:
			print temp
			print epoch
			print b[2]
		temp = epoch
	except:
		print "Maybe the first line"


a.close()	
