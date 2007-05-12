from Retrieve import retrieve

# input values are (table_name, fields, condition, data_from, data_to)
# return values are csv data

print retrieve.retrieve('PowerMeasure',"A,UID,W,AID,TimeStamp",1,0,10000)

