
class SpotBase:
    def __init__(self):

	self.Kim = [0,0,0,0]
	self.Cho = [0,0,0,0]

        self.Kimtoken=[0,0,0,0]
        self.Chotoken=[0,0,0,0]	
	pass

    def Token(self,Kim,Cho): 
#TV

        try:
                if Kim[0]  == 1 and Cho[0]  == 1:
                        self.Kimtoken[0] = 1
                        self.Chotoken[0] = 1
                elif Kim[0]  == 1 and (Cho[0] == 0 or Cho[0] == 2):
                        self.Kimtoken[0] = 1
                        self.Chotoken[0] = 0
                elif (Kim[0] == 0 or Kim[0] == 2) and Cho[0]  == 1:
                        self.Kimtoken[0] = 0
                        self.Chotoken[0] = 1
        except:
                pass

#Lamp1
        try:
                if Kim[1]  == 1 and Cho[1]  == 1:
                        self.Kimtoken[1] = 1
                        self.Chotoken[1] = 1
                elif Kim[1]  == 1 and Cho[1] == 0:
                        self.Kimtoken[1] = 1
                        self.Chotoken[1] = 0
                elif Kim[1] == 0 and Cho[1]  == 1:
                        self.Kimtoken[1] = 0
                        self.Chotoken[1] = 1

        except:
                pass



#Lamp2
        try:
                if Kim[3]  == 1 and Cho[3]  == 1:
                        self.Kimtoken[3] = 1
                        self.Chotoken[3] = 1
                elif Kim[3]  == 1 and Cho[3] == 0:
                        self.Kimtoken[3] = 1
                        self.Chotoken[3] = 0
                elif Kim[3] == 0 and Cho[3]  == 1:
                        self.Kimtoken[3] = 0
                        self.Chotoken[3] = 1

        except:
                pass


        #print "[TV,LampLiving,Coffee,LampBed]"
        #print self.Kimtoken
        #print self.Chotoken
        #print self.Chorssi
        #print self.Kimrssi



if __name__ == '__main__':
	import time
	base = SpotBase()

	phase1 = open('groundtruth/phase1.txt')
	count = 0
	for Line in phase1:
		temp = Line.split(',')
		Kim = temp[2].split(':')
		Kim = [int(Kim[0]),int(Kim[1]),int(Kim[2]),int(Kim[3])]
                Cho = temp[1].split(':')
                Cho = [int(Cho[0]),int(Cho[1]),int(Cho[2]),int(Cho[3])]

		try:
			base.Token(Kim,Cho)
			print "======================================="
			count = count+1
			print count
			print Cho+Kim
			print base.Chotoken + base.Kimtoken
			#print base.Kimtoken
			#print base.Chotoken
			time.sleep(10)
		except:
			pass
