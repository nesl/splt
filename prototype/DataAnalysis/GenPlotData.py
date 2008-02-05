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
	SuccessCho = 0
	TotalCount = 0



	fileTVEnergy = open('TVEnergy.dat','w')
	fileLivLampEnergy = open('LivLampEnergy.dat','w')
	fileBedLampEnergy = open('BedLampEnergy.dat','w')
	fileKimEnergy = open('KimEnergy.dat','w')
	fileChoEnergy = open('ChoEnergy.dat','w')


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


			TVEnergy = float(data.TV[index])
			LivEnergy = float(data.LampLiving[index])
			BedEnergy = float(data.LampBed[index])

			# write Energy dat file
			fileTVEnergy.write("%f\n"%(TVEnergy))
			fileLivLampEnergy.write("%f\n"%(LivEnergy))
			fileBedLampEnergy.write("%f\n"%(BedEnergy))

			#Calculate Energy per person and write down

			#Ground Truth
                        GTVDiv = ChoTemp[0]+KimTemp[0]
			GLivDiv = ChoTemp[1]+KimTemp[1]
			GBedDiv = ChoTemp[3]+KimTemp[3]

			if GTVDiv != 0:
				TVperOne = TVEnergy/GTVDiv
			else:
				TVperOne = 0


                        if GLivDiv != 0:
                                LivperOne = LivEnergy/GLivDiv
                        else:
                                LivperOne = 0

                        if GBedDiv != 0:
                                BedperOne = BedEnergy/GBedDiv
                        else:
                                BedperOne = 0

			GTVKim = KimTemp[0]*TVperOne
			GTVCho = ChoTemp[0]*TVperOne
			GLivKim = KimTemp[1]*LivperOne
			GLivCho = ChoTemp[1]*LivperOne
			GBedKim = KimTemp[3]*BedperOne
			GBedCho = ChoTemp[3]*BedperOne

			



			#Basic

                        GTVDiv = base.Chotoken[0]+base.Kimtoken[0]
                        GLivDiv = base.Chotoken[1]+base.Kimtoken[1]
                        GBedDiv = base.Chotoken[3]+base.Kimtoken[3]

                        if GTVDiv != 0:
                                TVperOne = TVEnergy/GTVDiv
                        else:
                                TVperOne = 0


                        if GLivDiv != 0:
                                LivperOne = LivEnergy/GLivDiv
                        else:
                                LivperOne = 0

                        if GBedDiv != 0:
                                BedperOne = BedEnergy/GBedDiv
                        else:
                                BedperOne = 0

                        BTVKim = base.Kimtoken[0]*TVperOne
                        BTVCho = base.Chotoken[0]*TVperOne
                        BLivKim = base.Kimtoken[1]*LivperOne
                        BLivCho = base.Chotoken[1]*LivperOne
                        BBedKim = base.Kimtoken[3]*BedperOne
                        BBedCho = base.Chotoken[3]*BedperOne

			#Calibration             
                        GTVDiv = cali.Chotoken[0]+cali.Kimtoken[0]
                        GLivDiv = cali.Chotoken[1]+cali.Kimtoken[1]
                        GBedDiv = cali.Chotoken[3]+cali.Kimtoken[3]
                        
                        if GTVDiv != 0:
                                TVperOne = TVEnergy/GTVDiv
                        else:
                                TVperOne = 0


                        if GLivDiv != 0:
                                LivperOne = LivEnergy/GLivDiv
                        else:
                                LivperOne = 0

                        if GBedDiv != 0:
                                BedperOne = BedEnergy/GBedDiv
                        else:
                                BedperOne = 0

                        CTVKim = cali.Kimtoken[0]*TVperOne
                        CTVCho = cali.Chotoken[0]*TVperOne
                        CLivKim = cali.Kimtoken[1]*LivperOne
                        CLivCho = cali.Chotoken[1]*LivperOne
                        CBedKim = cali.Kimtoken[3]*BedperOne
                        CBedCho = cali.Chotoken[3]*BedperOne

			#Hysteresis
                        GTVDiv = hys.Chotoken[0]+hys.Kimtoken[0]
                        GLivDiv = hys.Chotoken[1]+hys.Kimtoken[1]
                        GBedDiv = hys.Chotoken[3]+hys.Kimtoken[3]
                        
                        if GTVDiv != 0:
                                TVperOne = TVEnergy/GTVDiv
                        else:
                                TVperOne = 0


                        if GLivDiv != 0:
                                LivperOne = LivEnergy/GLivDiv
                        else:
                                LivperOne = 0

                        if GBedDiv != 0:
                                BedperOne = BedEnergy/GBedDiv
                        else:
                                BedperOne = 0

                        HTVKim = hys.Kimtoken[0]*TVperOne
                        HTVCho = hys.Chotoken[0]*TVperOne
                        HLivKim = hys.Kimtoken[1]*LivperOne
                        HLivCho = hys.Chotoken[1]*LivperOne
                        HBedKim = hys.Kimtoken[3]*BedperOne
                        HBedCho = hys.Chotoken[3]*BedperOne
                
		
			
			fileKimEnergy.write("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n"%(GTVKim,GLivKim,GBedKim,BTVKim,BLivKim,BBedKim,CTVKim,CLivKim,CBedKim,HTVKim,HLivKim,HBedKim))
			fileChoEnergy.write("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n"%(GTVCho,GLivCho,GBedCho,BTVCho,BLivCho,BBedCho,CTVCho,CLivCho,CBedCho,HTVCho,HLivCho,HBedCho))


        fileTVEnergy.close()# = open('TVEnergy.dat','w')
        fileLivLampEnergy.close()#  = open('LivLampEnergy.dat','w')
        fileBedLampEnergy.close()#  = open('BedLampEnergy.dat','w')
        fileKimEnergy.close()#  = open('KimEnergy.dat','w')
        fileChoEnergy.close()#  = open('ChoEnergy.dat','w')

			#print ChoToken[index].split(':')+KimToken[index].split(':')

			#print ChoTemp+KimTemp
			#print base.Chotoken+base.Kimtoken
			#print cali.Chotoken+cali.Kimtoken
			#print hys.Chotoken+hys.Kimtoken

