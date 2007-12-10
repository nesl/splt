from Retrieve import retrieve
import time
import thread

Condition = "PosDescription=\"LivingRoom\" and ID = 201"
#TimeStamp = retrieve.retrieve('HomeTokens',"TimeStamp",Condition,0,10000)
#PosDescription = retrieve.retrieve('HomeTokens',"PosDescription",Condition,0,10000)

LivBaseToken = retrieve.retrieve('HomeTokens',"BaseToken",Condition,0,10000).split('\n')
LivCaliToken = retrieve.retrieve('HomeTokens',"CaliToken",Condition,0,10000).split('\n')
LivHysToken = retrieve.retrieve('HomeTokens',"HysToken",Condition,0,10000).split('\n')
LivRSSI = retrieve.retrieve('HomeTokens',"RSSI",Condition,0,10000).split('\n')


Condition = "PosDescription=\"WorkSpace3\""
WorkBaseToken = retrieve.retrieve('HomeTokens',"BaseToken",Condition,0,10000).split('\n')
WorkCaliToken = retrieve.retrieve('HomeTokens',"CaliToken",Condition,0,10000).split('\n')
WorkHysToken = retrieve.retrieve('HomeTokens',"HysToken",Condition,0,10000).split('\n')
WorkRSSI = retrieve.retrieve('HomeTokens',"RSSI",Condition,0,10000).split('\n')

Condition = "PosDescription=\"Kitchen1\""
Kit1BaseToken = retrieve.retrieve('HomeTokens',"BaseToken",Condition,0,10000).split('\n')
Kit1CaliToken = retrieve.retrieve('HomeTokens',"CaliToken",Condition,0,10000).split('\n')
Kit1HysToken = retrieve.retrieve('HomeTokens',"HysToken",Condition,0,10000).split('\n')
Kit1RSSI = retrieve.retrieve('HomeTokens',"RSSI",Condition,0,10000).split('\n')

Condition = "PosDescription=\"Kitchen2\""
Kit2BaseToken = retrieve.retrieve('HomeTokens',"BaseToken",Condition,0,10000).split('\n')
Kit2CaliToken = retrieve.retrieve('HomeTokens',"CaliToken",Condition,0,10000).split('\n')
Kit2HysToken = retrieve.retrieve('HomeTokens',"HysToken",Condition,0,10000).split('\n')
Kit2RSSI = retrieve.retrieve('HomeTokens',"RSSI",Condition,0,10000).split('\n')

Condition = "PosDescription=\"BedRoom\""
BedBaseToken = retrieve.retrieve('HomeTokens',"BaseToken",Condition,0,10000).split('\n')
BedCaliToken = retrieve.retrieve('HomeTokens',"CaliToken",Condition,0,10000).split('\n')
BedHysToken = retrieve.retrieve('HomeTokens',"HysToken",Condition,0,10000).split('\n')
BedRSSI = retrieve.retrieve('HomeTokens',"RSSI",Condition,0,10000).split('\n')






temp = 0
a = "\n"
b = ['0','0','0','0','0','0','0','0','0','0']
c = ['0','0','0','0','0','0','0','0','0','0']
flip = 0
index = 0

file = open('LivBaseToken.dat','w')
for i in range(len(LivBaseToken)):


	#if((i+1)%20==0):
	#	a=a+'\n'

	if((i)%50==0 and (i)%100==0):
		print b
		#a=a+b+'\n'
	elif((i)%50==0 and (i)%100!=0):
		#a=a+c+'\n'
		print c


	if((i+1)%5==0):
		file.write("%i\n"%(temp))
		if(temp>0):
			Temp = '1'	
		else:
			Temp = '0'
		temp = 0
		index = index + 1 

	        if (flip==0):
       	        	b[index%10] = Temp
        	elif(flip==1):
                	c[-index%10] = Temp

	else:

		try:
			temp = temp+int(LivBaseToken[i].split(':')[0])
		except:
			pass

	if((i)%50==0 and (i)%100==0):
		flip = 0
			
	elif((i)%50==0 and (i)%100!=0):
		flip = 1



file.close()


print a

#print LivRSSI



#for j in range(12):
	#for i in range(10):
	



