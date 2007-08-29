import data
import hysAlgorithm
import caliAlgorithm
import baseAlgorithm



if __name__ == '__main__':
        import time
        base = baseAlgorithm.SpotBase()
	cali = caliAlgorithm.SpotCali()
	hys = hysAlgorithm.SpotCali()

        for i in range(3424):
                RSSI1=data.User200RSSI[i].split(':')
                RSSI2=data.User201RSSI[i].split(':')
                RSSI1 = [float(RSSI1[0]),float(RSSI1[1]),float(RSSI1[2]),float(RSSI1[3])]
                RSSI2 = [float(RSSI2[0]),float(RSSI2[1]),float(RSSI2[2]),float(RSSI2[3])]
                base.Token(RSSI1,RSSI2)
		cali.Token(RSSI1,RSSI2)
		hys.Token(RSSI1,RSSI2)
		print "====================================="
                print base.Kimtoken+base.Chotoken
		print cali.Kimtoken+cali.Chotoken
                print hys.Kimtoken+hys.Chotoken
                time.sleep(0.1)

