
# PROJ is the file name of your module.
PROJ = roombacomm
ROOTDIR = ../../..

SUPPORTLIST = cyclops mica2 micaz xyz avrora cricket

include ../../Makerules

test:
	../../../config/sos_tool/sos_tool.exe --rmmod=128
	../../../config/sos_tool/sos_tool.exe --insmod=${PROJ}.sos


