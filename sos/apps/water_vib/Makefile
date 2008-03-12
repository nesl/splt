
PROJ = Water

ROOTDIR = $(SOSROOT)

DEFS += -D'UART_ADDR=0x8000'
#DEFS += -DSOS_TREE_ROUTING_DEBUG
#DEFS += -DSOS_DEBUG_CODEMEM
#DEFS += -DSOS_DEBUG_SCHED
#DEFS += -DSOS_DEBUG_MALLOC
#DEFS += -DSOS_DEBUG_DFT_LOADER 
#DEFS += -DSOS_DEBUG_FETCHER
#DEFS += -DSOS_DEBUG_SIM_INTERFACE
#Choose the appropriate sensor board
#DEFS += -DMICASB
#DEFS += -DMTS400SB
#DEFS += -DUSE_UART1
#DEFS += -DSOS_USE_LOGGING

SRCS += loader.c WaterVib.c accel_sensor.c accel_test.c

#################################################
# MEMORY PROTECTION OPTIONS
#################################################
ifeq ($(MODE),sfi)
SRCS += sos_mod_verify.c
VPATH += $(ROOTDIR)/tools/harbor/soslib
INCDIR += -I$(ROOTDIR)/tools/harbor/soslib
INCDIR += -I$(ROOTDIR)/tools/harbor/lib
endif

# Include simulated module 
#SIM_SRCS += blink.c surge.c tree_routing.c test_param.c 
INCDIR += -I$(ROOTDIR)/modules/sensordrivers/mts310/include/
include $(ROOTDIR)/config/Makerules

vpath loader.c $(ROOTDIR)/extensions/loader/
vpath surge.c $(ROOTDIR)/modules/test_modules/surge/
vpath test_param.c $(ROOTDIR)/modules/test_modules/test_param/
vpath WaterVib.c ../../modules/water_vib/
vpath accel_sensor.c $(ROOTDIR)/modules/sensordrivers/mts310/accel/
vpath accel_test.c ../../modules/accel_test/
