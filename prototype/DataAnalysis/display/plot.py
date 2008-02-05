import wx
import wx.lib.plot as wxPlot



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

