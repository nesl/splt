from Retrieve import retrieve
import time
import thread



TV = retrieve.retrieve('SpotHome2',"TV",1,0,10000)
Coffee = retrieve.retrieve('SpotHome2',"Coffee",1,0,10000)
LampLiving = retrieve.retrieve('SpotHome2',"LampLiving",1,0,10000)
LampBed = retrieve.retrieve('SpotHome2',"LampBed",1,0,10000)
User200 = retrieve.retrieve('SpotHome2',"200_Consumption",1,0,10000)
User201 = retrieve.retrieve('SpotHome2',"201_Consumption",1,0,10000)

User200Occu = retrieve.retrieve('SpotHome2',"200_Occupancy",1,0,10000)
User201Occu = retrieve.retrieve('SpotHome2',"201_Occupancy",1,0,10000)

User200RSSI = retrieve.retrieve('SpotHome2',"200_RSSI",1,0,10000)
User201RSSI = retrieve.retrieve('SpotHome2',"201_RSSI",1,0,10000)


TV = TV.split()
Coffee = Coffee.split()
LampLiving = LampLiving.split()
LampBed = LampBed.split()
User200 = User200.split()
User201 = User201.split()
User200Occu = User200Occu.split()
User201Occu = User201Occu.split()
User200RSSI = User200RSSI.split()
User201RSSI = User201RSSI.split()


import wx
from plot import *

import hysAlgorithm

hysAlgo = hysAlgorithm.SpotCali()


class SpotDisplay(wx.Frame):
    def __init__(self):
	wx.Frame.__init__(self, None, -1, "Occupancy", size=(400, 200))
	self.SetPosition((0,622))
	wx.StaticText(self, -1, "TV:LampLiving:Coffee:LampBed", pos = (100,12))
	wx.StaticText(self, -1, "User200", pos = (20,42))
	wx.StaticText(self, -1, "User201", pos = (20,72))
	self.U200TV = wx.TextCtrl(self, -1, "", pos=(100,35),size=(20,20))
	self.U200LampLiving = wx.TextCtrl(self, -1, "", pos=(140,35),size=(20,20))
	self.U200Coffee = wx.TextCtrl(self, -1, "", pos=(200,35),size=(20,20))
	self.U200LampBed = wx.TextCtrl(self, -1, "", pos=(250,35),size=(20,20))
	self.U201TV = wx.TextCtrl(self, -1, "", pos=(100,72),size=(20,20))
	self.U201LampLiving = wx.TextCtrl(self, -1, "", pos=(140,72),size=(20,20))
	self.U201Coffee = wx.TextCtrl(self, -1, "", pos=(200,72),size=(20,20))
	self.U201LampBed = wx.TextCtrl(self, -1, "", pos=(250,72),size=(20,20))




        self.TVFig = AppEnergy("TV Energy",(0,20),(400,300),"red")  
        self.CoffeeFig = AppEnergy("Coffee Energy",(401,20),(400,300),"blue")  
        self.LampLivingFig = AppEnergy("LampLiving Energy",(802,20),(400,300),"black")  
        self.LampBedFig = AppEnergy("LampBed Energy",(0,321),(400,300),"magenta")  
        self.User200Fig = AppEnergy("User200 Energy",(401,321),(400,300),"green")  
        self.User201Fig = AppEnergy("User201 Energy",(802,321),(400,300),"orange")  

	#self.User200T = AppEnergy("User200 Accum energy", (401,600),(400,300),"green")
	#self.User201T = AppEnergy("User201 Accum energy", (802,600),(400,300),"blue")

	try:
		thread.start_new_thread(self.Update,())
	except thread.error:
		pass
	self.index = 0

	self.KimTotal = 0
	self.ChoTotal = 0

    def Update(self):
	while True:
		try:
			self.TVFig.Update(float(TV[self.index]))
			self.CoffeeFig.Update(float(Coffee[self.index]))
			self.LampLivingFig.Update(float(LampLiving[self.index]))
			self.LampBedFig.Update(float(LampBed[self.index]))
			#self.User200Fig.Update(float(User200[self.index]))
			#self.User201Fig.Update(float(User201[self.index]))
		except:
			pass


                RSSI1=User200RSSI[self.index].split(':')
                RSSI2=User201RSSI[self.index].split(':')
                RSSI1 = [float(RSSI1[0]),float(RSSI1[1]),float(RSSI1[2]),float(RSSI1[3])]
                RSSI2 = [float(RSSI2[0]),float(RSSI2[1]),float(RSSI2[2]),float(RSSI2[3])]
                hysAlgo.Token(RSSI1,RSSI2)
                print hysAlgo.Kimtoken
                print hysAlgo.Chotoken

		try:
		        self.U200TV.SetValue("%s"%(hysAlgo.Kimtoken[0]))
	        	self.U200LampLiving.SetValue("%s"%(hysAlgo.Kimtoken[1]))
		        self.U200Coffee.SetValue("%s"%(hysAlgo.Kimtoken[2]))
		        self.U200LampBed.SetValue("%s"%(hysAlgo.Kimtoken[3]))

                        self.U201TV.SetValue("%s"%(hysAlgo.Chotoken[0])) 
                        self.U201LampLiving.SetValue("%s"%(hysAlgo.Chotoken[1]))
                        self.U201Coffee.SetValue("%s"%(hysAlgo.Chotoken[2]))
                        self.U201LampBed.SetValue("%s"%(hysAlgo.Chotoken[3]))
		except:
			print "haha"

	
        	divider = [0,0,0,0]
        	divider[0] = hysAlgo.Kimtoken[0] + hysAlgo.Chotoken[0]
        	divider[1] = hysAlgo.Kimtoken[1] + hysAlgo.Chotoken[1]
        	divider[2] = hysAlgo.Kimtoken[2] + hysAlgo.Chotoken[2]
        	divider[3] = hysAlgo.Kimtoken[3] + hysAlgo.Chotoken[3]


	        try:
        	        TVPerPerson = float(TV[self.index])/divider[0]
        	except:
                	TVPerPerson = float(TV[self.index])

        	try:
                	LampPerPerson = float(LampLiving[self.index])/divider[1]
        	except:
                	LampPerPerson = float(LampLiving[self.index])
        	try:
                	CoffeePerPerson = float(Coffee[self.index])/divider[2]
        	except:
                	CoffeePerPerson = float(Coffee[self.index])

        	try:
                	Lamp2PerPerson = float(LampBed[self.index])/divider[3]
        	except:
                	Lamp2PerPerson = float(LampBed[self.index])




	        Kimenergy = hysAlgo.Kimtoken[0]*TVPerPerson + hysAlgo.Kimtoken[1]*LampPerPerson + hysAlgo.Kimtoken[2]*CoffeePerPerson + hysAlgo.Kimtoken[3]*Lamp2PerPerson
	        Choenergy = hysAlgo.Chotoken[0]*TVPerPerson + hysAlgo.Chotoken[1]*LampPerPerson + hysAlgo.Chotoken[2]*CoffeePerPerson + hysAlgo.Chotoken[3]*Lamp2PerPerson

                self.User200Fig.Update(Choenergy)
                self.User201Fig.Update(Kimenergy)


		self.KimTotal = self.KimTotal+Kimenergy
		self.ChoTotal = self.ChoTotal+Choenergy
		#self.User200T.Update(self.ChoTotal/100)
		#self.User201T.Update(self.KimTotal/100)

		#print "User200 RSSI : %s"%(User200RSSI[self.index])
		#print "User201 RSSI : %s"%(User201RSSI[self.index])
		self.index = self.index+1	
		time.sleep(0.5)


if __name__ == '__main__':
    app = wx.PySimpleApp()
    frame = SpotDisplay()
    frame.Show(True)
    app.MainLoop()
