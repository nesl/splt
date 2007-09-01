if __name__ == '__main__':
	import data
	import time

 	# Order of columns in data file is:
   # RSSI1TV  RSSI1Lamp  RSSI1Coffee  RSSI1Lamp2,
   # RSSI2TV  RSSI2Lamp  RSSI2Coffee  RSSI2Lamp2,

	for i in range(1877):
		RSSI1=data.User200RSSI[i].split(':')
 		print RSSI1[0]+"\t"+RSSI1[1]+"\t"+RSSI1[2]+"\t"+RSSI1[3],
		RSSI2=data.User201RSSI[i].split(':')
 		print RSSI2[0]+"\t"+RSSI2[1]+"\t"+RSSI2[2]+"\t"+RSSI2[3]
