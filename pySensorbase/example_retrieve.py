from Retrieve import retrieve

# input values are (table_name, fields, condition, data_from, number of data(max = 10000))
# return values are csv data

print retrieve.retrieve('PowerMeasure',"AID,TimeStamp",1,10000,10000)

