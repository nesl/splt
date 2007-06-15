from Retrieve import retrieve
import time
# input values are (table_name, fields, condition, data_from, data_to)
# return values are csv data
#14E00401000042B1CE : Laura
#14E004010000429DF7 : Akhi
#14E0040100004288B9 : Younghun
#14E004010000429DA9 : Zainul


#condition = "TID=\"14E004010000429DA9\""
#condition = "TID=\"14E0040100004288B9\""
#condition = "TID=\"14E004010000429DF7\""
condition = "TID=\"14E00401000042B1CE\""

file = open('laura.dat','w+')
data =  retrieve.retrieve('RFID',"TimeStamp,UID",condition,0,10000)
file.write(data)
file.close()


#file = open('DataForm.txt','w+')
#for i in data.split(','):#
#	try:
#		print time.localtime(i)
#	except:
#		pass

#time = time.time()
#condition = "AID=\"%s\""%(4) # AND "TimeStamp=%d""%(time)
#condition = "AID=4"
#condition = "TimeStamp>1181771990"
#print condition
#condition = 1
#file = open('power.dat','w+')
#for i in range(10):#
#	data = retrieve.retrieve("PowerMeasure","TimeStamp,W,AID",condition, 510000+(i-1)*10000,10000)
#	data = retrieve.retrieve("PowerMeasure","TimeStamp,W",condition, 490000,10000)

#	print data

#	try:
#		file.write(data)
#	except:
#		pass
#file.close()
    
    
		
