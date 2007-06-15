import time

file = open('younghun.dat','r')

data = file.read()
data = data.split("\n")
for line in data:
	temp = line.split(',')
	try:
		if temp[1]=="3":
			print time.localtime(int(temp[0]))
			print temp[0]
	except:
		pass
	
file.close()