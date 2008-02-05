import SlogModule
import XMLwrap

#Account information
email='kimyh@ucla.edu'
password = 'password'
project_id = '154'
table = 'PowerMeasure'

#Date and Time / follows the format YYYY-MM-DD HH:MM:SS



def spltSlog(tablename,string):
	spltSlog = SlogModule.DataSlog()
	data = XMLwrap.Wrap(string)
	spltSlog.ChangeDB(email,password,project_id,tablename)
	spltSlog.ChangeXML(data)
	spltSlog.Slog()

	
