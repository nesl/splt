from Retrieve import retrieve
import time
import thread



TV = retrieve.retrieve('SpotHome',"TV",1,0,10000)
Coffee = retrieve.retrieve('SpotHome',"Coffee",1,0,10000)
LampLiving = retrieve.retrieve('SpotHome',"LampLiving",1,0,10000)
LampBed = retrieve.retrieve('SpotHome',"LampBed",1,0,10000)
User200 = retrieve.retrieve('SpotHome',"200_Consumption",1,0,10000)
User201 = retrieve.retrieve('SpotHome',"201_Consumption",1,0,10000)

User200Occu = retrieve.retrieve('SpotHome',"200_Occupancy",1,0,10000)
User201Occu = retrieve.retrieve('SpotHome',"201_Occupancy",1,0,10000)

TV = TV.split()
Coffee = Coffee.split()
LampLiving = LampLiving.split()
LampBed = LampBed.split()
User200 = User200.split()
User201 = User201.split()
User200Occu = User200Occu.split()
User201Occu = User201Occu.split()


import wx
from plot import *




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

		try:
			temp = User200Occu[self.index].split(':')
		        self.U200TV.SetValue("%s"%(temp[0]))
	        	self.U200LampLiving.SetValue("%s"%(temp[1]))
		        self.U200Coffee.SetValue("%s"%(temp[2]))
		        self.U200LampBed.SetValue("%s"%(temp[3]))

			temp = User201Occu[self.index].split(':')
                        self.U201TV.SetValue("%s"%(temp[0])) 
                        self.U201LampLiving.SetValue("%s"%(temp[1]))
                        self.U201Coffee.SetValue("%s"%(temp[2]))
                        self.U201LampBed.SetValue("%s"%(temp[3]))
		except:
			print "haha"

		self.index = self.index+1	
		time.sleep(0.5)


if __name__ == '__main__':
    app = wx.PySimpleApp()
    frame = SpotDisplay()
    frame.Show(True)
    app.MainLoop()
