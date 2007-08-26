from Retrieve import retrieve
import time
import thread



TV = retrieve.retrieve('SpotHome',"TV",1,0,10000)
Coffee = retrieve.retrieve('SpotHome',"Coffee",1,0,10000)
LampLiving = retrieve.retrieve('SpotHome',"LampLiving",1,0,10000)
LampBed = retrieve.retrieve('SpotHome',"LampBed",1,0,10000)
User200 = retrieve.retrieve('SpotHome',"200_Consumption",1,0,10000)
User201 = retrieve.retrieve('SpotHome',"201_Consumption",1,0,10000)

TV = TV.split()
Coffee = Coffee.split()
LampLiving = LampLiving.split()
LampBed = LampBed.split()
User200 = User200.split()
User201 = User201.split()


import wx
from plot import *




class SpotDisplay(wx.Frame):
    def __init__(self):

        self.TVFig = AppEnergy("TV Energy",(0,20),(400,300),"red")  
        self.CoffeeFig = AppEnergy("Coffee Energy",(401,20),(400,300),"blue")  
        self.LampLivingFig = AppEnergy("LampLiving Energy",(802,20),(400,300),"black")  
        self.LampBedFig = AppEnergy("LampBed Energy",(0,321),(400,300),"magenta")  
        self.User200Fig = AppEnergy("User200 Energy",(401,321),(400,300),"green")  
        self.User201Fig = AppEnergy("User201 Energy",(802,321),(400,300),"orange")  

	try:
		thread.start_new_thread(self.Update,())
	except thread.error:
		pass
	self.index = 0

    def Update(self):
	while True:
		try:
			self.TVFig.Update(float(TV[self.index]))
			self.CoffeeFig.Update(float(Coffee[self.index]))
			self.LampLivingFig.Update(float(LampLiving[self.index]))
			self.LampBedFig.Update(float(LampBed[self.index]))
			self.User200Fig.Update(float(User200[self.index]))
			self.User201Fig.Update(float(User201[self.index]))
		except:
			pass
		self.index = self.index+1	
		time.sleep(0.5)


if __name__ == '__main__':
    app = wx.PySimpleApp()
    frame = SpotDisplay()
    app.MainLoop()
