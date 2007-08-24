#!/usr/bin python

import wx
import wx.lib.plot as wxPlot
import random
import math
import time
from pysos import pysos
import thread


class UserButton(wx.Button):
    def __init__(self, parent, name, position, size):
        wx.Button.__init__(self, parent, -1, name, position, size) 
        self.Bind(wx.EVT_LEFT_DOWN, self.OnMe)
        self.Bind(wx.EVT_LEFT_DCLICK, self.OnCloseMe)
        self.Bind(wx.EVT_MOTION, self.OnMotion)
        self.absposition = self.GetPosition()
        self.energy = 0
        self.total = 0
                
    def OnMe(self, event):
        self.pos = event.GetPosition()

    def OnCloseMe(self, event):
        self.Destroy()

    def OnMotion(self, event):
        if event.Dragging() and event.LeftIsDown():
            iniposition = self.GetPosition()
            position = iniposition+event.GetPosition()-self.pos
            self.SetPosition((position.x, position.y))
            self.absposition = position             
        event.Skip()

class AppButton(wx.Button):
    def __init__(self, parent, name, position, size, active, radius):
        wx.Button.__init__(self, parent, -1, name, position, size) 
        self.status = "Off"
        self.energy = 0
        self.active = active
        self.radius = radius
        self.Bind(wx.EVT_LEFT_DOWN, self.OnMe)
        self.Bind(wx.EVT_TIMER, self.OnTimeout)
        self.timer = wx.Timer(self)
        self.timer.Start(1000)
                
    def OnMe(self, event):
        if self.status == "Off":
            self.status = "On"
            self.SetBackgroundColour("red")
            self.SetForegroundColour("black")
        elif self.status == "On":
            self.status ="Off"
            self.SetBackgroundColour("black")
            self.SetForegroundColour("white")

    def OnTimeout(self, evt):
        if self.status =="Off":
	    pass
            #self.energy = 0
        elif self.status == "On":
            #self.energy = self.active+random.uniform(-3,10)
            pass
        

class AppEnergy(wx.Frame):
    def __init__(self, name,pos, size, color):
        wx.Frame.__init__(self, None, -1, name, pos, size) 
        self.name = name
        self.color = color
        self.data = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
        for index in range(31):
            self.data[index]=(index,0)

        self.client = wxPlot.PlotCanvas(self)
        self.markers = wxPlot.PolyMarker(self.data, legend='', colour=self.color, marker='circle', size=1)
        self.gc = wxPlot.PlotGraphics([self.markers], name, 'Time', 'Watt')
        self.client.Draw(self.gc, xAxis=(0,30), yAxis=(-5,300))
        self.Show(True)

        self.Bind(wx.EVT_CLOSE, self.OnCloseWindow)

    def Update(self, data):
        for index in range(30):
            self.data[index]=(self.data[index][0],self.data[index+1][1])

        self.data[30] = (30,data)
        
        self.markers = wxPlot.PolyMarker(self.data, legend='', colour=self.color, marker='circle', size=1)
        self.gc = wxPlot.PlotGraphics([self.markers], self.name, 'Time', 'Watt')
        self.client.Draw(self.gc, xAxis=(0,30), yAxis=(-5,300))
        self.Show(True)
        
    def OnCloseWindow(self, event):
        self.Destroy()
        

class SpotLight(wx.Frame): 
    def __init__(self): 
        wx.Frame.__init__(self, None, -1, "SpotLight", size=(800, 600)) 
        self.SetPosition((0,0))
        wx.StaticText(self, -1, "Pos:", pos=(10, 12)) 
        self.posCtrl = wx.TextCtrl(self, -1, "", pos=(40, 10))
        self.Bind(wx.EVT_CLOSE, self.OnCloseWindow)

        self.Bind(wx.EVT_TIMER, self.OnTimeout)
        self.timer = wx.Timer(self)
        self.timer.Start(1000)
        self.panel = wx.Panel(self, -1, pos=(601,0), size=(200,600))
        self.INIKim = wx.Button(self.panel, -1, "Enter Kim", pos=(0,0))
        self.INIKim.Bind(wx.EVT_LEFT_DOWN, self.EnterKim, self.INIKim)
        self.INIZainul = wx.Button(self.panel, -1, "Enter Zainul", pos=(0,30))
        self.INIZainul.Bind(wx.EVT_LEFT_DOWN, self.EnterZainul, self.INIZainul)
        self.KimPlot = wx.Button(self.panel, -1, "Kim Profile", pos=(80,0))
        self.ZainulPlot = wx.Button(self.panel, -1, "Zainul Profile", pos=(80,30))
        self.KimPlot.Bind(wx.EVT_LEFT_DOWN, self.KimFigure, self.KimPlot)
        self.ZainulPlot.Bind(wx.EVT_LEFT_DOWN, self.ZainulFigure, self.ZainulPlot)



        self.Lampbutton = wx.Button(self.panel, -1, "Lamp Energy", pos=(0,90))
        self.Lampbutton.Bind(wx.EVT_LEFT_DOWN, self.Lamppop, self.Lampbutton)

        self.Coffeebutton = wx.Button(self.panel, -1, "Coffee Energy", pos=(0,120))
        self.Coffeebutton.Bind(wx.EVT_LEFT_DOWN, self.Coffeepop, self.Coffeebutton)


        self.Lamp = AppButton(self, "Lamp", (450,200), (60,30), 80, 20)
        self.Coffee = AppButton(self, "Coffee", (300,350), (60,30), 30, 200)

        try:
		self.srv = pysos.sossrv()
	except:
		print "Please run sos_server first"
	try:
		thread.start_new_thread(self.getSOSmsg,())
	except thread.error:
		pass


    def EnterKim(self, event):
        self.Kim = UserButton(self, "Kim", (20,20), (30,30))
	self.Kim.token = [0,0]
	self.Kim.rssi = [-200,-200]        
    def EnterZainul(self, event):
        self.Zainul = UserButton(self, "Zainul", (80,80), (60,30))
	self.Zainul.token = [0,0]
	self.Zainul.rssi = [-200,-200]
        
    def Lamppop(self, event):
        self.LampFig = AppEnergy("Lamp Energy",(801,0),(400,300),"black")
    def Coffeepop(self, event):
        self.CoffeeFig = AppEnergy("Coffee Energy",(801,0),(400,300),"red")
    def KimFigure(self, event):
        self.KimFig = AppEnergy("Kim's Energy Profile",(801,0),(400,300),"blue")
    def ZainulFigure(self, event):
        self.ZainulFig = AppEnergy("Zainul's Energy Profile",(801,0),(400,300),"orange")

    
    def OnCloseWindow(self, event):
        self.Destroy()


    def OnTimeout(self, evt):
	try:
		if self.Kim.rssi[0] > -13 and self.Zainul.rssi[0] > -13:
			self.Kim.token[0] = 1
			self.Zainul.token[0] = 1
		elif self.Kim.rssi[0] > -13 and self.Zainul.rssi[0] < -13:
			self.Kim.token[0] = 1
			self.Zainul.token[0] = 0
		elif self.Kim.rssi[0] < -13 and self.Zainul.rssi[0] > -13:
			self.Kim.token[0] = 0
			self.Zainul.token[0] = 1
	except:
		pass

        try:
                if self.Kim.rssi[1] > 0 and self.Zainul.rssi[1] > 0:
                        self.Kim.token[1] = 1
                        self.Zainul.token[1] = 1
                elif self.Kim.rssi[1] > 0 and self.Zainul.rssi[1] < 0:
                        self.Kim.token[1] = 1
                        self.Zainul.token[1] = 0
                elif self.Kim.rssi[1] < 0 and self.Zainul.rssi[1] > 0:
                        self.Kim.token[1] = 0
                        self.Zainul.token[1] = 1

        except:
                pass

	#print self.Kim.token
	#print self.Zainul.token
	#print self.Zainul.rssi

	divider = [0,0]
	divider[0] = self.Kim.token[0] + self.Zainul.token[0]
	divider[1] = self.Kim.token[1] + self.Zainul.token[1]
	
	try:
		LampPerPerson = self.Lamp.energy/divider[0]
	except:
		LampPerPerson = self.Lamp.energy
	try:
		CoffeePerPerson = self.Coffee.energy/divider[1]
	except:
		CoffeePerPerson = self.Coffee.energy

	self.Kim.energy = self.Kim.token[0]*LampPerPerson + self.Kim.token[1]*CoffeePerPerson
	self.Zainul.energy = self.Zainul.token[0]*LampPerPerson + self.Zainul.token[1]*CoffeePerPerson
	print self.Lamp.energy
	print self.Kim.energy
	print self.Zainul.energy

# Figure updates
        try:
            pass 
        except:
            pass
        try:
            pass
        except:
            pass
        try:
	    self.KimFig.Update(self.Kim.energy)
        except:
            pass
        try:
	    self.ZainulFig.Update(self.Zainul.energy)
        except:
            pass

    def getSOSmsg(self):
	while True:
		try:
			msg = self.srv.listen()
			pkt = msg[0]['data']
			try: 
				data = pysos.unpack('<BbB',pkt)
				temp = float(data[1])
				if data[0] == 3 and data[2] == 200:
					self.Kim.rssi[0] = temp
				if data[0] == 3 and data[2] == 201:
					self.Zainul.rssi[0] = temp

				if data[0] == 8 and data[2] == 200:
					self.Kim.rssi[1] = temp
				if data[0] == 8 and data[2] == 201:
					self.Zainul.rssi[1] = temp



			except:
				addr = pysos.unpack('<B',pkt[0])
				temp = pkt[1:].split(',')
				if addr[0]==3:
					self.Lamp.energy = float(temp[3])/10
				        self.LampFig.Update(self.Lamp.energy)

				if addr[0]==8:
					self.Coffee.energy = float(temp[3])/10
					self.CoffeeFig.Update(self.Coffee.energy)
		except:
			pass


        
if __name__ == '__main__': 
    app = wx.PySimpleApp() 
    frame = SpotLight() 
    frame.Show(True) 
    app.MainLoop()
    
        
