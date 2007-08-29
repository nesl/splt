bed = open('spltbed.txt')
liv = open('spltliv.txt')

beddata = bed.read().split('\n')
livdata = liv.read().split('\n')

bed.close()
liv.close()

temp = open('phase1.txt','w')

#phase one
for i in range(325):

	temp1 = beddata[i+1+1].split(',')
	temp2 = livdata[i+1].split(',')

	#BedTime
	BedTime = temp1[2]
	#Bedroom Lamp (Cho,Kim) or (200,201)
	temp1 = temp1[1].split(';')
	temp1 = temp1[0].split(':')
	

	#Living Lamp (Cho,Kim) TV (Cho,Kim)

	temp3 = temp2[1].split(';')
	temp3 = temp3[0].split(':')
	temp4 = temp2[3].split(';')
	temp4 = temp4[0].split(':')
	
	temp2 = temp3+temp4

	Line = BedTime+','+temp2[2]+':'+temp2[0]+':'+'0'+':'+temp1[0]+','+temp2[3]+':'+temp2[1]+':'+'0'+':'+temp1[1]

	temp.write(Line+'\n')

temp.close()

#phase two
temp = open('phase2.txt','w')

for i in range(153):
	temp1 = beddata[i+341].split(',')
	temp2 = livdata[i+327].split(',')

        #BedTime
        BedTime = temp1[2]
        #Bedroom Lamp (Cho,Kim) or (200,201)
        temp1 = temp1[1].split(';')
        temp1 = temp1[0].split(':')

	
        #Living Lamp (Cho,Kim) TV (Cho,Kim)

        temp3 = temp2[1].split(';')
        temp3 = temp3[0].split(':')
        temp4 = temp2[3].split(';')
        temp4 = temp4[0].split(':')

        temp2 = temp3+temp4

        Line = BedTime+','+temp2[2]+':'+temp2[0]+':'+'0'+':'+temp1[0]+','+temp2[3]+':'+temp2[1]+':'+'0'+':'+temp1[1]

        temp.write(Line+'\n')

temp.close()

