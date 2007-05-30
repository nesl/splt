occu = function(rfidFile, userID) {
  # This gives the occupancy information of the room 

  # RFID file format
  # Timestamp, Appliance ID, Tag ID

  # Max number of people allowed
  MAX_NUM = 100
  
  # Number of people in the room right now
  room = 0
  # pointer for each person in the room
  people = 0
  for(i in 1:MAX_NUM) {
    people[i] = 0
  }
  # total time/room spent in the lab
  total_power = 0
  
  # Read the data from the file
  rfid_data = read.table(rfidFile, head = TRUE, sep = ',')

  # Iterate through all rfid data
  for(i in 1:length(rfid_data$tag_id)) {
    # if the appliance is the door
    if(rfid_data$app_id[i] == 'door') {
      
      # if the person has entered the room
      if(people[rfid_data$tag_id[i]] == 0) {
        room = room + 1
        people[rfid_data$tag_id[i]] = 1
      }
      # if the person has left the room
      else {
        room = room - 1
        people[rfid_data$tag_id[i]] = 0        
      }

      # manage the time/room for userID
      # if the userID is not in the room and he just left
      if((people[userID] == 0) && (rfid_data$tag_id[i] == userID)) {
        total_power = total_power + ((rfid_data$timestamp[i] - rfid_data$timestamp[i - 1]) / (room + 1))
      }
      # if the userID is in the room and he did not just enter the room
      if((people[userID] == 1) && (rfid_data$tag_id[i] != userID)) {
        total_power = total_power + ((rfid_data$timestamp[i] - rfid_data$timestamp[i - 1]) / room)
      }
    }
  }

  # return total power
  total_power
}
