#!/usr/bin/python

import thread
import sys,os
import urllib
import urllib2
import time



## Assumption : There is only one type

class DataSlog:
    def __init__(self):
        self.sb_email = ''
        self.sb_password = ''
        self.sb_project_id = ''
        self.sb_table = ''
	self.xml = ''
        try:
            pass
        except:
	    pass

    def Slog(self):
        
        sb_api = 'http://sensorbase.org/alpha/upload.php' # the interface of sensorbase used for uploading data
        param = {'email' : self.sb_email,
                      'pw' : self.sb_password,
                      'project_id' : self.sb_project_id,
                      'data_string': self.xml,
                      'type':'xml',
                      'tableName': self.sb_table}
	print param
        try: 
		data = urllib.urlencode(param)
	        req = urllib2.Request(sb_api, data)
	        response = urllib2.urlopen(req)
	        SlogResult = "DATA POST result: " + response.read()
	        response.close()
        except:
		pass
	print SlogResult


    def ChangeDB(self,sb_email,sb_password,sb_project_id,sb_table):
        self.sb_email = sb_email
        self.sb_password = sb_password
        self.sb_project_id = sb_project_id
        self.sb_table = sb_table
        #print self.sb_email
        #print self.sb_password
        #print self.sb_project_id
        #print self.sb_table

    def ChangeXML(self,XML):
	self.xml = XML


    def WrapSlog(self,DB):
	DBentry = DB[0]
	DBdata = DB[1]
	self.ChangeDB(DBentry[0],DBentry[1],DBentry[2],DBentry[3])
	self.ChangeXML(DBdata)
	self.Slog()
