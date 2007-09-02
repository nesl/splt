if __name__ == '__main__':
	import data
	import time

 	# Order of columns in data file is:
   # PowerTV PowerLamp PowerCoffee PowerLamp2

	for i in range(1877):
 		print data.TV[i]+"\t"+data.LampLiving[i]+"\t"+data.Coffee[i]+"\t"+data.LampBed[i]
