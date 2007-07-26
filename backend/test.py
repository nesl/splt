import wx 
import math 
import random 
import sys
import time
import os
import thread
from pysos import pysos


class RadarGraph(wx.Window): 
    def __init__(self, parent, title): 
        wx.Window.__init__(self, parent) 
        self.title = title 
	self.px=0
	self.py=0 
        self.titleFont = wx.Font(14, wx.SWISS, wx.NORMAL, wx.BOLD) 
        self.labelFont = wx.Font(10, wx.SWISS, wx.NORMAL, wx.NORMAL) 
        self.InitBuffer() 
        self.Bind(wx.EVT_SIZE, self.OnSize) 
        self.Bind(wx.EVT_PAINT, self.OnPaint) 

    def OnSize(self, evt): 
        # When the window size changes we need a new buffer. 
        self.InitBuffer() 
    def OnPaint(self, evt):   
        dc = wx.BufferedPaintDC(self, self.buffer)   

    def InitBuffer(self):   
        w, h = self.GetClientSize()         
        self.buffer = wx.EmptyBitmap(w, h) 
        dc = wx.BufferedDC(wx.ClientDC(self), self.buffer) 
        self.DrawGraph(dc) 
        
    def SetData(self, newData): 
        self.px = newData[0]
	self.py = newData[1]
	dc = wx.BufferedDC(wx.ClientDC(self), self.buffer)   
        self.DrawGraph(dc) 

    def PolarToCartesian(self, radius, angle, cx, cy): 
        x = radius * math.cos(math.radians(angle)) 
        y = radius * math.sin(math.radians(angle)) 
        return (cx+x, cy-y) 

    def DrawGraph(self, dc):   
        spacer = 10 
        scaledmax = 150.0 
        dc.SetBackground(wx.Brush(self.GetBackgroundColour())) 
        dc.Clear() 
        dw, dh = dc.GetSize() 

        dc.SetFont(self.titleFont)   
        tw, th = dc.GetTextExtent(self.title) 
        dc.DrawText(self.title, (dw-tw)/2, spacer)   
 
        th = th + 2*spacer   
        cx = dw/2 
        cy = (dh-th)/2 + th 
        dc.SetPen(wx.Pen("red", 20)) 
        dc.SetBrush(wx.TRANSPARENT_BRUSH) 
	dc.DrawPoint(self.px,self.py)

class TestFrame(wx.Frame): 
    def __init__(self): 
        wx.Frame.__init__(self, None, title="Double Buffered Drawing", 
                          size=(800,800)) 
        self.plot = RadarGraph(self, "2D Accel Emulator") 


        # Set some random initial data values 
        self.plot.SetData((100,100)) 
        # Create a timer to update the data values 
        self.Bind(wx.EVT_TIMER, self.OnTimeout) 
        self.timer = wx.Timer(self) 
        self.timer.Start(20) 

        try:
                self.srv = pysos.sossrv()
        except:
                print "Please run sos_server first"

        try:
                thread.start_new_thread(self.getSOSmsg,())
        except thread.error:
                pass

	self.py = 0
	self.px = 0

    def OnTimeout(self, evt): 
	data=(self.px,self.py)
        self.plot.SetData(data) 

    def getSOSmsg(self):
        while True:
                try:
                        msg = self.srv.listen()
                        pkt = msg[0]['data']
                        accMsg = pysos.unpack('<BH',pkt)
                        if accMsg[0]==2:
                                self.py = 2*(accMsg[1]-511)+400
                        if accMsg[0]==1:
                                self.px = 2*(accMsg[1]-511)+400

                except:
                        pass


app = wx.PySimpleApp() 
frm = TestFrame() 
frm.Show() 
app.MainLoop() 

