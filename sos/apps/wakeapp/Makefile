PROJ = accel_app
ROOTDIR = $(SOSROOT)

#DEFS += -DMICASB
SRCS += accel_test.c accel_sensor.c loader.c 
INCDIR += -I$(ROOTDIR)/modules/sensordrivers/h34c/include/
SB = h34c

include $(ROOTDIR)/config/Makerules

vpath loader.c $(ROOTDIR)/extensions/loader/
vpath accel_test.c ../../modules/wakeup/
vpath accel_sensor.c $(ROOTDIR)/modules/sensordrivers/h34c/accel/
