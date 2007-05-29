occu = function(rfidFile, userID) {
  # This gives the occupancy information of the room 

  # RFID file format
  # Timestamp, Appliance ID, Tag ID
  # PM file format
  # Timestamp, Appliance ID, Watt
  
  # Usage time
  USAGE_TIME = 60
  
  total_power = 0
  
  # Read the data from the file
  rfid_data = read.table(rfidFile, head = TRUE, sep = ',')
  pm_data = read.table(pmFile, head = TRUE, sep = ',')

  # Find all tag readings of this user
  for(i in 1:length(rfid_data$tag_id)) {
    if(userID == rfid_data$tag_id[i]) {
      print('user used appliance')
      print(rfid_data$app_id[i])
      print('at time')
      print(rfid_data$timestamp[i])
      print('the amount of power used is')

      # Find the power usage of the appliance
      for(j in 1:length(pm_data$timestamp)) {
        if((rfid_data$timestamp[i] == pm_data$timestamp[j])
           && (rfid_data$app_id[i] == pm_data$app_id[j])){

          # idle_power is the minimum power seen in the 10 seconds around usage
          idle_power = min(pm_data$watt[(j - 5) : (j + 5)])

          integ = 0
          # the appliance will be in use for usage_time or end of data stream
          for(k in j:min(j + USAGE_TIME, length(pm_data$timestamp) - j)) {
            # add to integral if greater than idle power
            if(pm_data$watt[k] > idle_power)
              integ = integ + pm_data$watt[k]
          }
          
          # Add to the total usage of the user
          total_power = total_power + (integ / min(j + USAGE_TIME, length(pm_data$timestamp) - j))
        }
      }
    }
  }

  # return the total power usage
  print('total power = ')
  total_power
}
