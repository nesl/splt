import wx
import sys
import time
import os
import Queue
import thread

from pysos import pysos
from Slog import spltSlog

from display import hysAlgorithm
from display import caliAlgorithm
from display import baseAlgorithm


SqQueue = Queue.Queue(400)

class TextFrame(wx.Frame): 
    def __init__(self): 
        wx.Frame.__init__(self, None, -1, 'Text Entry Example', 
                size=(400, 150)) 
        panel = wx.Panel(self, -1) 
        basicLabel = wx.StaticText(panel, -1, "Position index") 
        self.basicText = wx.TextCtrl(panel, -1, "1", 
                size=(175, -1)) 
        self.basicText.SetInsertionPoint(0) 
        pwdLabel = wx.StaticText(panel, -1, "Position Description") 
        self.pwdText = wx.TextCtrl(panel, -1, "password", size=(175, -1)) 
        sizer = wx.FlexGridSizer(cols=2, hgap=6, vgap=6) 
        sizer.AddMany([basicLabel, self.basicText, pwdLabel, self.pwdText]) 
        panel.SetSizer(sizer) 

	self.button = wx.Button(panel, -1, "Start", pos=(300,50))
	self.Bind(wx.EVT_BUTTON, self.OnClick, self.button)
	self.button.SetDefault()

	self.Bind(wx.EVT_TIMER, self.OnTimeout)
	self.timer = wx.Timer(self)


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


	self.Chorssi = [-200,-200,-200,-200]
	self.Kimrssi = [-200,-200,-200,-200]
	self.counter = 0


	self.hys = hysAlgorithm.SpotCali()
	self.cali = caliAlgorithm.SpotCali()
	self.base = baseAlgorithm.SpotBase()


    def OnTimeout(self, evt):
	#self.basicText.GetValue()

	self.base.Token(self.Chorssi, self.Kimrssi)
	self.cali.Token(self.Chorssi, self.Kimrssi)
	self.hys.Token(self.Chorssi, self.Kimrssi)



	Data1 = (('PositionIndex',int(self.basicText.GetValue())),('PosDescription', self.pwdText.GetValue()),('RSSI',"%f:%f:%f:%f"%(self.Kimrssi[0],self.Kimrssi[1],self.Kimrssi[2],self.Kimrssi[3])),('BaseToken',"%i:%i:%i:%i"%(self.base.Kimtoken[0],self.base.Kimtoken[1],self.base.Kimtoken[2],self.base.Kimtoken[3])),('CaliToken',"%i:%i:%i:%i"%(self.cali.Kimtoken[0],self.cali.Kimtoken[1],self.cali.Kimtoken[2],self.cali.Kimtoken[3])),('HysToken',"%i:%i:%i:%i"%(self.hys.Kimtoken[0],self.hys.Kimtoken[1],self.hys.Kimtoken[2],self.hys.Kimtoken[3])),('TimeStamp',time.time()),('ID',201))
	Data2 = (('PositionIndex',int(self.basicText.GetValue())),('PosDescription', self.pwdText.GetValue()),('RSSI',"%f:%f:%f:%f"%(self.Chorssi[0],self.Chorssi[1],self.Chorssi[2],self.Chorssi[3])),('BaseToken',"%i:%i:%i:%i"%(self.base.Chotoken[0],self.base.Chotoken[1],self.base.Chotoken[2],self.base.Chotoken[3])),('CaliToken',"%i:%i:%i:%i"%(self.cali.Chotoken[0],self.cali.Chotoken[1],self.cali.Chotoken[2],self.cali.Chotoken[3])),('HysToken',"%i:%i:%i:%i"%(self.hys.Chotoken[0],self.hys.Chotoken[1],self.hys.Chotoken[2],self.hys.Chotoken[3])),('TimeStamp',time.time()),('ID',200))

	SqQueue.put(Data1,True,0.5)
	SqQueue.put(Data2,True,0.5)
	



	self.counter = self.counter-1
	if self.counter <= 0:
	        self.counter = 5
        	self.base.Kimtoken = [0,0,0,0]
        	self.base.Chotoken = [0,0,0,0]

        	self.cali.Kimtoken = [0,0,0,0]
        	self.cali.Chotoken = [0,0,0,0]

        	self.hys.Kimtoken = [0,0,0,0]
        	self.hys.Chotoken = [0,0,0,0]

        	temp = int(self.basicText.GetValue())
        	temp = temp + 1
        	self.basicText.SetValue("%i"%(temp))


		#self.timer.Stop()

    def OnClick(self, event):
#	self.counter = 5
#	self.base.Kimtoken = [0,0,0,0]
#	self.base.Chotoken = [0,0,0,0]

#	self.cali.Kimtoken = [0,0,0,0]
#	self.cali.Chotoken = [0,0,0,0]
	
#	self.hys.Kimtoken = [0,0,0,0]
#	self.hys.Chotoken = [0,0,0,0]

#	temp = int(self.basicText.GetValue())
#	temp = temp + 1
#	self.basicText.SetValue("%i"%(temp))

	self.counter = 5

	self.timer.Start(1000)


    def DataSlog(self):
	while True:
		try:
			data = SqQueue.get(False,1)
			spltSlog.spltSlog('HomeTokens',data)
		except:
			pass


    def getSOSmsg(self, msg):
                try:
                        pkt = msg['data']
                        try:
                                data = pysos.unpack('<BbB',pkt)
                                temp = float(data[1])
# Moving average
                                alpha = 0.7
                                if data[0] == 2 and data[2] == 200:
                                        self.Chorssi[0] = self.Chorssi[0]*(1-alpha)+alpha*temp
                                elif data[0] == 3 and data[2] == 200:
                                        self.Chorssi[1] = self.Chorssi[1]*(1-alpha)+alpha*temp
                                elif data[0] == 7 and data[2] == 200:
                                        self.Chorssi[2] = self.Chorssi[2]*(1-alpha)+alpha*temp
                                elif data[0] == 8 and data[2] == 200:
                                        self.Chorssi[3] = self.Chorssi[3]*(1-alpha)+alpha*temp

                                if data[0] == 2 and data[2] == 201:
                                        self.Kimrssi[0] = self.Kimrssi[0]*(1-alpha)+alpha*temp
                                elif data[0] == 3 and data[2] == 201:
                                        self.Kimrssi[1] = self.Kimrssi[1]*(1-alpha)+alpha*temp
                                elif data[0] == 7 and data[2] == 201:
                                        self.Kimrssi[2] = self.Kimrssi[2]*(1-alpha)+alpha*temp
                                elif data[0] == 8 and data[2] == 201:
                                        self.Kimrssi[3] = self.Kimrssi[3]*(1-alpha)+alpha*temp
			except:
				pass
		except:
			pass


if __name__ == '__main__': 
    app = wx.PySimpleApp() 
    frame = TextFrame() 
    frame.Show() 
    app.MainLoop() 
	
