from Retrieve import retrieve

# input values are (table_name, fields, condition, data_from, number of data(max = 10000))
# return values are csv data


# the following means, from the "PowerMeasure" table, get AID, TimeStamp, Watt, Voltage and A data from 0 to 10000
print retrieve.retrieve('PowerMeasure',"AID,TimeStamp,W,V,A",1,0,10000)

