from Retrieve import retrieve
import time
import thread


TimeStamp = retrieve.retrieve('SpotHome2',"TimeStamp",1,0,10000)
TV = retrieve.retrieve('SpotHome2',"TV",1,0,10000)
Coffee = retrieve.retrieve('SpotHome2',"Coffee",1,0,10000)
LampLiving = retrieve.retrieve('SpotHome2',"LampLiving",1,0,10000)
LampBed = retrieve.retrieve('SpotHome2',"LampBed",1,0,10000)
User200 = retrieve.retrieve('SpotHome2',"200_Consumption",1,0,10000)
User201 = retrieve.retrieve('SpotHome2',"201_Consumption",1,0,10000)

User200Occu = retrieve.retrieve('SpotHome2',"200_Occupancy",1,0,10000)
User201Occu = retrieve.retrieve('SpotHome2',"201_Occupancy",1,0,10000)

User200RSSI = retrieve.retrieve('SpotHome2',"200_RSSI",1,0,10000)
User201RSSI = retrieve.retrieve('SpotHome2',"201_RSSI",1,0,10000)


TimeStamp = TimeStamp.split()
TV = TV.split()
Coffee = Coffee.split()
LampLiving = LampLiving.split()
LampBed = LampBed.split()
User200 = User200.split()
User201 = User201.split()
User200Occu = User200Occu.split()
User201Occu = User201Occu.split()
User200RSSI = User200RSSI.split()
User201RSSI = User201RSSI.split()

