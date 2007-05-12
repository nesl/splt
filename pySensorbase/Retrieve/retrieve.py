from SOAPpy import WSDL
WSDLFILE = 'http://sensorbase.org/soap/sensorbase2.wsdl'


def retrieve(table,fields_value,condi,d_from,d_to):
	"""
		 retrieve's input values are, table name,
		 field value (e.g. fields = 'A,AID,TimeStamp,UID,W')
		 condition, from, to
	"""

	email = 'kimyh@ucla.edu'
	password = 'password'
	project_id = '154'
	table_name = table
	fields = fields_value
	condition = condi
	data_from = d_from
	data_to = d_to
	server = WSDL.Proxy(WSDLFILE)
	tables = "p_%s_%s"%(project_id,table_name)
	re_type = 'csv'
	csv = server.getData(email,password,fields,tables,condition,data_from,data_to,re_type)
	return csv


