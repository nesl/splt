import data


class SpotCali:
    def __init__(self):
        self.Chorssi=[-200,-200,-200,-200]
	self.Kimrssi=[-200,-200,-200,-200]

        self.Kimtoken=[0,0,0,0]
        self.Chotoken=[0,0,0,0]	

# will be first calibration constant

	self.TVKim = -17
	self.TVCho = -18
	self.LampKim = -22
	self.LampCho = -23
	self.CoffeeKim = -8
	self.CoffeeCho = -10
	self.Lamp2Kim = -23
	self.Lamp2Cho = -18




    def Token(self,RSSI1,RSSI2): 
	try:
		self.Chorssi=RSSI1
	except:
		self.Chorssi=[-200,-200,-200,-200]
	try:
		self.Kimrssi=RSSI2
	except:
		self.Kimrssi=[-200,-200,-200,-200]

#self.TV
        try:
                if self.Kimrssi[0] > self.TVKim and self.Chorssi[0] > self.TVCho:
                        self.Kimtoken[0] = 1
                        self.Chotoken[0] = 1
                elif self.Kimrssi[0] > self.TVKim and self.Chorssi[0] < self.TVCho:
                        self.Kimtoken[0] = 1
                        self.Chotoken[0] = 0
                elif self.Kimrssi[0] < self.TVKim and self.Chorssi[0] > self.TVCho:
                        self.Kimtoken[0] = 0
                        self.Chotoken[0] = 1
        except:
                pass

#Lamp1
        try:
                if self.Kimrssi[1] > self.LampKim and self.Chorssi[1] > self.LampCho:
                        self.Kimtoken[1] = 1
                        self.Chotoken[1] = 1
                elif self.Kimrssi[1] > self.LampKim and self.Chorssi[1] < self.LampCho:
                        self.Kimtoken[1] = 1
                        self.Chotoken[1] = 0
                elif self.Kimrssi[1] < self.LampKim and self.Chorssi[1] > self.LampCho:
                        self.Kimtoken[1] = 0
                        self.Chotoken[1] = 1
        except:
                pass


#Coffee
        try:
                if self.Kimrssi[2] > self.CoffeeKim and self.Chorssi[2] > self.CoffeeCho:
                        self.Kimtoken[2] = 1
                        self.Chotoken[2] = 1
                elif self.Kimrssi[2] > self.CoffeeKim and self.Chorssi[2] < self.CoffeeCho:
                        self.Kimtoken[2] = 1
                        self.Chotoken[2] = 0
                elif self.Kimrssi[2] < self.CoffeeKim and self.Chorssi[2] > self.CoffeeCho:
                        self.Kimtoken[2] = 0
                        self.Chotoken[2] = 1

        except:
                pass

#Lamp2
        try:
                if self.Kimrssi[3] > self.Lamp2Kim and self.Chorssi[3] > self.Lamp2Cho:
                        self.Kimtoken[3] = 1
                        self.Chotoken[3] = 1
                elif self.Kimrssi[3] > self.Lamp2Kim and self.Chorssi[3] < self.Lamp2Cho:
                        self.Kimtoken[3] = 1
                        self.Chotoken[3] = 0
                elif self.Kimrssi[3] < self.Lamp2Kim and self.Chorssi[3] > self.Lamp2Cho:
                        self.Kimtoken[3] = 0
                        self.Chotoken[3] = 1
        except:
                pass


        print "[TV,LampLiving,Coffee,LampBed]"
        #print self.Kimtoken
        #print self.Chotoken
        #print self.Chorssi
        #print self.Kimrssi



if __name__ == '__main__':
	import time
	base = SpotCali()

	for i in range(3424):
		RSSI1=data.User200RSSI[i].split(':')
		RSSI2=data.User201RSSI[i].split(':')
		RSSI1 = [float(RSSI1[0]),float(RSSI1[1]),float(RSSI1[2]),float(RSSI1[3])]
		RSSI2 = [float(RSSI2[0]),float(RSSI2[1]),float(RSSI2[2]),float(RSSI2[3])]
		base.Token(RSSI1,RSSI2)
		print base.Kimtoken
		print base.Chotoken
		time.sleep(0.1)
