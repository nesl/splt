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
            self.energy = 0
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
        self.timer.Start(500)

        self.panel = wx.Panel(self, -1, pos=(601,0), size=(200,600))
        self.INIKim = wx.Button(self.panel, -1, "Enter Kim", pos=(0,0))
        self.INIKim.Bind(wx.EVT_LEFT_DOWN, self.EnterKim, self.INIKim)
        self.INICho = wx.Button(self.panel, -1, "Enter Cho", pos=(0,30))
        self.INICho.Bind(wx.EVT_LEFT_DOWN, self.EnterCho, self.INICho)
        self.KimPlot = wx.Button(self.panel, -1, "Kim Profile", pos=(80,0))
        self.ChoPlot = wx.Button(self.panel, -1, "Cho Profile", pos=(80,30))
        self.KimPlot.Bind(wx.EVT_LEFT_DOWN, self.KimFigure, self.KimPlot)
        self.ChoPlot.Bind(wx.EVT_LEFT_DOWN, self.ChoFigure, self.ChoPlot)



        self.TVbutton = wx.Button(self.panel, -1, "TV Energy", pos=(0,60))
        self.TVbutton.Bind(wx.EVT_LEFT_DOWN, self.TVpop, self.TVbutton)
        
        self.Ironbutton = wx.Button(self.panel, -1, "Iron Energy", pos=(0,90))
        self.Ironbutton.Bind(wx.EVT_LEFT_DOWN, self.Ironpop, self.Ironbutton)

        self.Light1button = wx.Button(self.panel, -1, "Light1 Energy", pos=(0,120))
        self.Light1button.Bind(wx.EVT_LEFT_DOWN, self.Light1pop, self.Light1button)

        self.Light2button = wx.Button(self.panel, -1, "Light2 Energy", pos=(0,150))
        self.Light2button.Bind(wx.EVT_LEFT_DOWN, self.Light2pop, self.Light2button)

        self.Light3button = wx.Button(self.panel, -1, "Light3 Energy", pos=(0,180))
        self.Light3button.Bind(wx.EVT_LEFT_DOWN, self.Light3pop, self.Light3button)


        self.TV = AppButton(self, "Television", (400,300), (60,30), 50, 100)
        self.Iron = AppButton(self, "Iron", (450,200), (60,30), 80, 20)
        self.Light1 = AppButton(self, "Light1", (300,350), (60,30), 30, 200)
        self.Light2 = AppButton(self, "Light2", (400,100), (60,30), 30, 200)
        self.Light3 = AppButton(self, "Light3", (200,100), (60,30), 30, 200)

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
        
    def EnterCho(self, event):
        self.Cho = UserButton(self, "Cho", (80,80), (30,30))
        
    def TVpop(self, event):
        self.TVFig = AppEnergy("TV Energy",(801,0),(400,300),"magenta")
    def Ironpop(self, event):
        self.IronFig = AppEnergy("Iron Energy",(801,0),(400,300),"red")
    def Light1pop(self, event):
        self.Light1Fig = AppEnergy("Light1 Energy",(801,0),(400,300),"red")
    def Light2pop(self, event):
        self.Light2Fig = AppEnergy("Light2 Energy",(801,0),(400,300),"black")
    def Light3pop(self, event):
        self.Light3Fig = AppEnergy("Light3 Energy",(801,0),(400,300),"green")
    def KimFigure(self, event):
        self.KimFig = AppEnergy("Kim's Energy Profile",(801,0),(400,300),"blue")
    def ChoFigure(self, event):
        self.ChoFig = AppEnergy("Cho's Energy Profile",(801,0),(400,300),"orange")

    
    def OnCloseWindow(self, event):
        self.Destroy()


    def OnTimeout(self, evt):
        TVcount = 0
        Kim = 0
        Cho = 0
        Ironcount = 0
        Light1count = 0
        Light2count = 0
        Light3count = 0
        try:self.Kim.energy = 0
        except:pass
        try:self.Cho.energy = 0
        except:pass
            
#TV Vicinity check

        try:
            self.posCtrl.SetValue("%s, %s" % (self.Kim.absposition.x,self.Kim.absposition.y ))
            pos  = self.Kim.absposition - self.TV.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.TV.radius:
                #self.posCtrl.SetValue("%s, %s" % (dis, dis))
                TVcount = TVcount + 1
                Kim = 1
        except:
            pass

        try:
            pos  = self.Cho.absposition - self.TV.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.TV.radius:
                #self.posCtrl.SetValue("%s, %s" % (dis, dis))
                TVcount = TVcount + 1
                Cho = 1
        except:
            pass

        try:
            if Kim == 1:
                self.Kim.energy = self.TV.energy/TVcount
                Kim = 0
        except:
            pass
        
        try:
            if Cho == 1:
                self.Cho.energy = self.TV.energy/TVcount
                Cho = 0
        except:
            pass
    

#Iron Vicinity check
        Kim = 0
        Cho = 0
        try:
            pos  = self.Kim.absposition - self.Iron.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.Iron.radius:
                Ironcount = Ironcount + 1
                Kim = 1
        except:
            pass

        try:
            pos  = self.Cho.absposition - self.Iron.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.Iron.radius:
                Ironcount = Ironcount + 1
                Cho = 1
        except:
            pass

        try:
            if Kim == 1:
                self.Kim.energy = self.Kim.energy+self.Iron.energy/Ironcount
                Kim = 0
        except:
            pass
        
        try:
            if Cho == 1:
                self.Cho.energy = self.Cho.energy+self.Iron.energy/Ironcount
                Cho = 0
        except:
            pass

#Light1 Vicinity check
        Kim = 0
        Cho = 0

        try:
            pos  = self.Kim.absposition - self.Light1.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.Light1.radius:
                Light1count = Light1count + 1
                Kim = 1
        except:
            pass

        try:
            pos  = self.Cho.absposition - self.Light1.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.Light1.radius:
                Light1count = Light1count + 1
                Cho = 1
        except:
            pass

        try:
            if Kim == 1:
                self.Kim.energy = self.Kim.energy+self.Light1.energy/Light1count
                Kim = 0
        except:
            pass
        
        try:
            if Cho == 1:
                self.Cho.energy = self.Cho.energy+self.Light1.energy/Light1count
                Cho = 0
        except:
            pass


#Light2 Vicinity check
        Kim = 0
        Cho = 0

        try:
            pos  = self.Kim.absposition - self.Light2.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.Light2.radius:
                Light2count = Light2count + 1
                Kim = 1
        except:
            pass

        try:
            pos  = self.Cho.absposition - self.Light2.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.Light2.radius:
                Light2count = Light2count + 1
                Cho = 1
        except:
            pass

        try:
            if Kim == 1:
                self.Kim.energy = self.Kim.energy+self.Light2.energy/Light2count
                Kim = 0
        except:
            pass
        
        try:
            if Cho == 1:
                self.Cho.energy = self.Cho.energy+self.Light2.energy/Light2count
                Cho = 0
        except:
            pass


#Light3 Vicinity check
        Kim = 0
        Cho = 0

        try:
            pos  = self.Kim.absposition - self.Light3.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.Light3.radius:
                Light3count = Light3count + 1
                Kim = 1
        except:
            pass

        try:
            pos  = self.Cho.absposition - self.Light3.GetPosition()
            dis = math.sqrt(pos.x*pos.x+pos.y*pos.y)
            if dis < self.Light3.radius:
                Light3count = Light3count + 1
                Cho = 1
        except:
            pass

        try:
            if Kim == 1:
                self.Kim.energy = self.Kim.energy+self.Light3.energy/Light3count
                Kim = 0
        except:
            pass
        
        try:
            if Cho == 1:
                self.Cho.energy = self.Cho.energy+self.Light3.energy/Light3count
                Cho = 0
        except:
            pass




# Figure updates
        try:
            self.TVFig.Update(self.TV.energy)
        except:
            pass
        try:
            self.IronFig.Update(self.Iron.energy)
        except:
            pass
        try:
            self.Light1Fig.Update(self.Light1.energy)
        except:
            pass
        try:
            self.Light2Fig.Update(self.Light2.energy)
        except:
            pass
        try:
            self.Light3Fig.Update(self.Light3.energy)
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

    def getSOSmsg(self):
	while True:
		try:
			msg = self.srv.listen()
			pkt = msg[0]['data']
			addr = pysos.unpack('<B',pkt[0])
			#print addr[0]
			temp = pkt[1:].split(',')
			if addr[0]==3:
				self.Iron.energy = float(temp[3])/10
			if addr[0]==8:
				self.Light1.energy = float(temp[3])/10
		except:
			pass


        
if __name__ == '__main__': 
    app = wx.PySimpleApp() 
    frame = SpotLight() 
    frame.Show(True) 
    app.MainLoop()
    
        
