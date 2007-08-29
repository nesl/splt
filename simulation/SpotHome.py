#!/usr/bin python

import sys
import time
import os
import Queue
import thread
import wx
import wx.lib.plot as wxPlot
import random
import math
from pysos import pysos
from Slog import spltSlog


SpHomeQueue = Queue.Queue(800)



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
	panel = wx.Panel(self, -1)
        #self.SetPosition((0,0))
        wx.StaticText(self, -1, "Pos:", pos=(10, 12)) 
        self.posCtrl = wx.TextCtrl(self, -1, "", pos=(40, 10))
        self.Bind(wx.EVT_CLOSE, self.OnCloseWindow)

        self.Bind(wx.EVT_TIMER, self.OnTimeout)
        self.timer = wx.Timer(self)
        self.timer.Start(1000)
        self.panel = wx.Panel(self, -1, pos=(601,0), size=(200,600))
        self.INIKim = wx.Button(self.panel, -1, "Enter Kim", pos=(0,0))
        self.INIKim.Bind(wx.EVT_LEFT_DOWN, self.EnterKim, self.INIKim)
        self.INICho = wx.Button(self.panel, -1, "Enter Cho", pos=(0,30))
        self.INICho.Bind(wx.EVT_LEFT_DOWN, self.EnterCho, self.INICho)
        self.KimPlot = wx.Button(self.panel, -1, "Kim Profile", pos=(80,0))
        self.ChoPlot = wx.Button(self.panel, -1, "Cho Profile", pos=(80,30))
        self.KimPlot.Bind(wx.EVT_LEFT_DOWN, self.KimFigure, self.KimPlot)
        self.ChoPlot.Bind(wx.EVT_LEFT_DOWN, self.ChoFigure, self.ChoPlot)



        self.Lampbutton = wx.Button(self.panel, -1, "Lamp Energy", pos=(0,90))
        self.Lampbutton.Bind(wx.EVT_LEFT_DOWN, self.Lamppop, self.Lampbutton)
	self.Lamp2button = wx.Button(self.panel, -1, "Lamp2 Energy", pos=(0,150))
	self.Lamp2button.Bind(wx.EVT_LEFT_DOWN, self.Lamp2pop, self.Lamp2button)

	self.TVbutton = wx.Button(self.panel, -1, "TV Energy", pos=(0,180))
	self.TVbutton.Bind(wx.EVT_LEFT_DOWN, self.TVpop, self.TVbutton)


        self.Coffeebutton = wx.Button(self.panel, -1, "Coffee Energy", pos=(0,120))
        self.Coffeebutton.Bind(wx.EVT_LEFT_DOWN, self.Coffeepop, self.Coffeebutton)


        self.Lamp = AppButton(self, "Lamp", (450,50), (60,30), 80, 20)
        self.Coffee = AppButton(self, "Coffee", (50,200), (60,30), 30, 200)
	self.Lamp2 = AppButton(self, "Lamp2", (500, 500), (60,30), 80, 20)
	self.TV = AppButton(self, "TV", (400,300), (60,30), 80,20)

        try:
		self.srv = pysos.sossrv()
	except:
		print "Please run sos_server first"
	try:
		self.srv.register_trigger(self.getSOSmsg)
	except:
		pass

	try: 
		thread.start_new_thread(self.DataSlog,())
	except thread.error:
		pass


    def EnterKim(self, event):
        self.Kim = UserButton(self, "Kim", (20,20), (30,30))
	self.Kim.token = [0,0,0,0]
	self.Kim.rssi = [-200,-200,-200,-200]        
    def EnterCho(self, event):
        self.Cho = UserButton(self, "Cho", (80,80), (60,30))
	self.Cho.token = [0,0,0,0]
	self.Cho.rssi = [-200,-200,-200,-200]
        
    def Lamppop(self, event):
        self.LampFig = AppEnergy("Lamp Energy",(801,20),(400,300),"black")
    def Coffeepop(self, event):
        self.CoffeeFig = AppEnergy("Coffee Energy",(801,20),(400,300),"red")

    def Lamp2pop(self, event):
	self.Lamp2Fig = AppEnergy("Lamp2 Energy",(801,20), (400,300),"magenta")
    def TVpop(self,event):
	self.TVFig = AppEnergy("TV Energy", (801,20), (400,300), "green")

    def KimFigure(self, event):
        self.KimFig = AppEnergy("Kim's Energy Profile",(801,20),(400,300),"blue")
    def ChoFigure(self, event):
        self.ChoFig = AppEnergy("Cho's Energy Profile",(801,20),(400,300),"orange")

    
    def OnCloseWindow(self, event):
        self.Destroy()


    def OnTimeout(self, evt):
	TV = -20
        Lamp = -26
	Coffee = -5
	Lamp2 = -26
#TV
        try:
                if self.Kim.rssi[0] > TV and self.Cho.rssi[0] > TV:
                        self.Kim.token[0] = 1
                        self.Cho.token[0] = 1
                elif self.Kim.rssi[0] > TV and self.Cho.rssi[0] < TV:
                        self.Kim.token[0] = 1
                        self.Cho.token[0] = 0
                elif self.Kim.rssi[0] < TV and self.Cho.rssi[0] > TV:
                        self.Kim.token[0] = 0
                        self.Cho.token[0] = 1
        except:
                pass

#Lamp1
	try:
		if self.Kim.rssi[1] > Lamp and self.Cho.rssi[1] > Lamp:
			self.Kim.token[1] = 1
			self.Cho.token[1] = 1
		elif self.Kim.rssi[1] > Lamp and self.Cho.rssi[1] < Lamp:
			self.Kim.token[1] = 1
			self.Cho.token[1] = 0
		elif self.Kim.rssi[1] < Lamp and self.Cho.rssi[1] > Lamp:
			self.Kim.token[1] = 0
			self.Cho.token[1] = 1
	except:
		pass


#Coffee
        try:
                if self.Kim.rssi[2] > Coffee and self.Cho.rssi[2] > Coffee:
                        self.Kim.token[2] = 1
                        self.Cho.token[2] = 1
                elif self.Kim.rssi[2] > Coffee and self.Cho.rssi[2] < Coffee:
                        self.Kim.token[2] = 1
                        self.Cho.token[2] = 0
                elif self.Kim.rssi[2] < Coffee and self.Cho.rssi[2] > Coffee:
                        self.Kim.token[2] = 0
                        self.Cho.token[2] = 1

        except:
                pass

#Lamp2
        try:
                if self.Kim.rssi[3] > Lamp2 and self.Cho.rssi[3] > Lamp2:
                        self.Kim.token[3] = 1
                        self.Cho.token[3] = 1
                elif self.Kim.rssi[3] > Lamp2 and self.Cho.rssi[3] < Lamp2:
                        self.Kim.token[3] = 1
                        self.Cho.token[3] = 0
                elif self.Kim.rssi[3] < Lamp2 and self.Cho.rssi[3] > Lamp2:
                        self.Kim.token[3] = 0
                        self.Cho.token[3] = 1
        except:
                pass



	print "[TV,LampLiving,Coffee,LampBed]"
	print self.Kim.token
	print self.Cho.token
	#print self.Cho.rssi
	#print self.Kim.rssi

	divider = [0,0,0,0]
	divider[0] = self.Kim.token[0] + self.Cho.token[0]
	divider[1] = self.Kim.token[1] + self.Cho.token[1]
	divider[2] = self.Kim.token[2] + self.Cho.token[2]
	divider[3] = self.Kim.token[3] + self.Cho.token[3]


        try:
                TVPerPerson = self.TV.energy/divider[0]
        except:
                TVPerPerson = self.TV.energy

	try:
		LampPerPerson = self.Lamp.energy/divider[1]
	except:
		LampPerPerson = self.Lamp.energy
	try:
		CoffeePerPerson = self.Coffee.energy/divider[2]
	except:
		CoffeePerPerson = self.Coffee.energy

        try:
                Lamp2PerPerson = self.Lamp2.energy/divider[3]
        except:
                Lamp2PerPerson = self.Lamp2.energy




	self.Kim.energy = self.Kim.token[0]*TVPerPerson + self.Kim.token[1]*LampPerPerson + self.Kim.token[2]*CoffeePerPerson + self.Kim.token[3]*Lamp2PerPerson
	self.Cho.energy = self.Cho.token[0]*TVPerPerson + self.Cho.token[1]*LampPerPerson + self.Cho.token[2]*CoffeePerPerson + self.Cho.token[3]*Lamp2PerPerson


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
	    self.ChoFig.Update(self.Cho.energy)
        except:
            pass


# Put Data into Queue
	Data = (('200_Consumption',self.Cho.energy),('201_Consumption', self.Kim.energy),('200_RSSI',"%f:%f:%f:%f"%(self.Cho.rssi[0],self.Cho.rssi[1],self.Cho.rssi[2],self.Cho.rssi[3])), ('201_RSSI',"%f:%f:%f:%f"%(self.Kim.rssi[0],self.Kim.rssi[1],self.Kim.rssi[2],self.Kim.rssi[3])), ('200_Occupancy',"%i:%i:%i:%i"%(self.Cho.token[0],self.Cho.token[1],self.Cho.token[2],self.Cho.token[3])),('201_Occupancy',"%i:%i:%i:%i"%(self.Kim.token[0],self.Kim.token[1],self.Kim.token[2],self.Kim.token[3])),('Coffee', self.Coffee.energy), ('LampBed', self.Lamp2.energy), ('LampLiving', self.Lamp.energy), ('TV', self.TV.energy), ('TimeStamp',time.time()))
	SpHomeQueue.put(Data,True,0.5)


    def DataSlog(self):
	while True:
		try:
			data = SpHomeQueue.get(False, 1)
			spltSlog.spltSlog('SpotHome2', data)
		except:
			pass

    def getSOSmsg(self,msg):
		try:
			pkt = msg['data']
			try: 
				data = pysos.unpack('<BbB',pkt)
				temp = float(data[1])

# Moving average
				alpha = 0.7
				if data[0] == 2 and data[2] == 200:
					self.Cho.rssi[0] = self.Cho.rssi[0]*(1-alpha)+alpha*temp
				elif data[0] == 3 and data[2] == 200:
					self.Cho.rssi[1] = self.Cho.rssi[1]*(1-alpha)+alpha*temp
				elif data[0] == 7 and data[2] == 200:
					self.Cho.rssi[2] = self.Cho.rssi[2]*(1-alpha)+alpha*temp
				elif data[0] == 8 and data[2] == 200:
					self.Cho.rssi[3] = self.Cho.rssi[3]*(1-alpha)+alpha*temp


				elif data[0] == 2 and data[2] == 201:
					self.Kim.rssi[0] = self.Kim.rssi[0]*(1-alpha)+alpha*temp
				elif data[0] == 3 and data[2] == 201:
					self.Kim.rssi[1] = self.Kim.rssi[1]*(1-alpha)+alpha*temp
				elif data[0] == 7 and data[2] == 201:
					self.Kim.rssi[2] = self.Kim.rssi[2]*(1-alpha)+alpha*temp
				elif data[0] == 8 and data[2] == 201:
					self.Kim.rssi[3] = self.Kim.rssi[3]*(1-alpha)+alpha*temp

# direct value

#                                if data[0] == 2 and data[2] == 200:
#                                        self.Cho.rssi[0] = temp
#                                elif data[0] == 3 and data[2] == 200:
#                                        self.Cho.rssi[1] = temp
#                                elif data[0] == 7 and data[2] == 200:
#                                        self.Cho.rssi[2] = temp
#                                elif data[0] == 8 and data[2] == 200:
#                                        self.Cho.rssi[3] = temp
#
#
#                                elif data[0] == 2 and data[2] == 201:
#                                        self.Kim.rssi[0] = temp
#                                elif data[0] == 3 and data[2] == 201:
#                                        self.Kim.rssi[1] = temp
#                                elif data[0] == 7 and data[2] == 201:
#                                        self.Kim.rssi[2] = temp
#                                elif data[0] == 8 and data[2] == 201:
#                                        self.Kim.rssi[3] = temp



			except:
				addr = pysos.unpack('<B',pkt[0])
				temp = pkt[1:].split(',')
				if addr[0]==3:
					self.Lamp.energy = float(temp[3])/10
				        self.LampFig.Update(self.Lamp.energy)
				elif addr[0]==7:
					self.Coffee.energy = float(temp[3])/10
					self.CoffeeFig.Update(self.Coffee.energy)
				elif addr[0]==2:
					self.TV.energy = float(temp[3])/10
					self.TVFig.Update(self.TV.energy)
				elif addr[0]==8:
					self.Lamp2.energy = float(temp[3])/10
					self.Lamp2Fig.Update(self.Lamp2.energy)
		except:
			pass


        
if __name__ == '__main__': 
    app = wx.PySimpleApp() 
    frame = SpotLight() 
    frame.Show(True) 
    app.MainLoop()
    
        
