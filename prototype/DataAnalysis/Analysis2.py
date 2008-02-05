from display import data
from display import hysAlgorithm
from display import caliAlgorithm
from display import baseAlgorithm



if __name__ == '__main__':
        import time
	import Truth


	phase1Kimtoken = [[],[]]
	phase1Chotoken = [[],[]]
	phase2Kimtoken = [[],[]]
	phase2Chotoken = [[],[]]

	phase1 = Truth.SpotBase()
	phase2 = Truth.SpotBase()
	file = open('groundtruth/phase1.txt')
	fileKim = open('phase1Kimtoken.txt','w')
	fileCho = open('phase1Chotoken.txt','w')

	for Line in file:

                temp = Line.split(',')
                Kim = temp[2].split(':')
                Kim = [int(Kim[0]),int(Kim[1]),int(Kim[2]),int(Kim[3])]
                Cho = temp[1].split(':')
                Cho = [int(Cho[0]),int(Cho[1]),int(Cho[2]),int(Cho[3])]


                
                try:
                        phase1.Token(Kim,Cho)
			#phase1Kimtoken.insert(count,phase1.Kimtoken)
			#phase1Chotoken.insert(count,phase1.Chotoken)
			#print phase1Chotoken[count]+phase1Kimtoken[count]
			
			fileKim.write("%d:%d:%d:%d\n"%(phase1.Kimtoken[0],phase1.Kimtoken[1],phase1.Kimtoken[2],phase1.Kimtoken[3]))
			fileCho.write("%d:%d:%d:%d\n"%(phase1.Chotoken[0],phase1.Chotoken[1],phase1.Chotoken[2],phase1.Chotoken[3]))

                except:
			pass
                        #print "haha"

	fileKim.close()
	fileCho.close()


	file.close()

	#print phase1Chotoken[1]+phase1Kimtoken[1]
	#print phase1Kimtoken





	file = open('groundtruth/phase2.txt')
        fileKim = open('phase2Kimtoken.txt','w')
        fileCho = open('phase2Chotoken.txt','w')

        for Line in file:

                temp = Line.split(',')
                Kim = temp[2].split(':')
                Kim = [int(Kim[0]),int(Kim[1]),int(Kim[2]),int(Kim[3])]
                Cho = temp[1].split(':')
                Cho = [int(Cho[0]),int(Cho[1]),int(Cho[2]),int(Cho[3])]


                try:
                        phase2.Token(Kim,Cho)
                        fileKim.write("%d:%d:%d:%d\n"%(phase2.Kimtoken[0],phase2.Kimtoken[1],phase2.Kimtoken[2],phase2.Kimtoken[3]))
                        fileCho.write("%d:%d:%d:%d\n"%(phase2.Chotoken[0],phase2.Chotoken[1],phase2.Chotoken[2],phase2.Chotoken[3]))
                
                except: 
                        pass
                        #print "haha"
        
        fileKim.close()
        fileCho.close()
        file.close()

        base = baseAlgorithm.SpotBase()
	cali = caliAlgorithm.SpotCali()
	hys = hysAlgorithm.SpotCali()



#Phase 1 Truth value
	fileKim = open('phase1Kimtoken.txt')
	fileCho = open('phase1Chotoken.txt')

	KimToken = fileKim.read()
	ChoToken = fileCho.read()

	fileKim.close()
	fileCho.close()

	KimToken = KimToken.split('\n')
	ChoToken = ChoToken.split('\n')


	SuccessKim = 0
	FPKim = 0
	FNKim = 0
	SuccessCho = 0
	FPCho = 0
	FNCho = 0
	TotalCount = 0
        for i in range(1877):
                RSSI1=data.User200RSSI[i].split(':')
                RSSI2=data.User201RSSI[i].split(':')
                RSSI1 = [float(RSSI1[0]),float(RSSI1[1]),float(RSSI1[2]),float(RSSI1[3])]
                RSSI2 = [float(RSSI2[0]),float(RSSI2[1]),float(RSSI2[2]),float(RSSI2[3])]
                base.Token(RSSI1,RSSI2)
		cali.Token(RSSI1,RSSI2)
		hys.Token(RSSI1,RSSI2)

#index correlation for time sync

		SET = 33

		if (i-SET)%10 == 0 and i >= SET:
			#print i
			index=int((i-SET)/10)
			#print index
			#print "=========================="
			#print data.User200RSSI[i]+data.User201RSSI[i]
			ChoTemp = ChoToken[index].split(':')
			KimTemp = KimToken[index].split(':')
			ChoTemp = [int(ChoTemp[0]),int(ChoTemp[1]),int(ChoTemp[2]),int(ChoTemp[3])]
			KimTemp = [int(KimTemp[0]),int(KimTemp[1]),int(KimTemp[2]),int(KimTemp[3])]

			#print ChoToken[index].split(':')+KimToken[index].split(':')

			#print base.Chotoken+base.Kimtoken
			#print ChoTemp+KimTemp
			#print cali.Chotoken+cali.Kimtoken
			#print hys.Chotoken+hys.Kimtoken
			TotalCount = TotalCount + 1
#			if KimTemp[3] == base.Kimtoken[3]:# and KimTemp[1] == base.Kimtoken[1] and KimTemp[3] == base.Kimtoken[3]:
#				SuccessKim = SuccessKim+1
#			if ChoTemp[3]==base.Chotoken[3]:# and ChoTemp[1]==base.Chotoken[1] and ChoTemp[3]==base.Chotoken[3]:
#				SuccessCho = SuccessCho+1

#                        if KimTemp[0] == cali.Kimtoken[0] and KimTemp[1] == cali.Kimtoken[1] and KimTemp[3] == cali.Kimtoken[3]:
#                                SuccessKim = SuccessKim+1
#                        if ChoTemp[0]==cali.Chotoken[0] and ChoTemp[1]==cali.Chotoken[1] and ChoTemp[3]==cali.Chotoken[3]:
#                                SuccessCho = SuccessCho+1


			index = 0
                        if KimTemp[index] == base.Kimtoken[index]:# and KimTemp[1] == hys.Kimtoken[1] and KimTemp[3] == hys.Kimtoken[3]:
                                SuccessKim = SuccessKim+1
			if KimTemp[index] == 1 and base.Kimtoken[index]==0:
				FNKim = FNKim+1
			if KimTemp[index] == 0 and base.Kimtoken[index]==1:
				FPKim = FPKim+1
                        if ChoTemp[index]==base.Chotoken[index]:# and ChoTemp[1]==hys.Chotoken[1] and ChoTemp[3]==hys.Chotoken[3]:
                                SuccessCho = SuccessCho+1
                        if ChoTemp[index] == 1 and base.Chotoken[index]==0:
                                FNCho = FNCho+1
                        if ChoTemp[index] == 0 and base.Chotoken[index]==1:
                                FPCho = FPCho+1


			#print SuccessCho
	print SuccessCho
	print FPCho
	print FNCho
        print SuccessKim
        print FPKim
        print FNKim

	print TotalCount 
		#print "====================================="
                #print base.Kimtoken+base.Chotoken
		#print cali.Kimtoken+cali.Chotoken
                #print hys.Kimtoken+hys.Chotoken



