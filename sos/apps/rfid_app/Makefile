
PROJ = rfid_mote

ROOTDIR = $(SOSROOT)

SRCS +=  neighbor.c tree_routing.c rfid.c uart_mod_rfid.c

#DEFS += -D'UART_ADDR=0x8000'
DEFS += -DNO_SOS_UART -DDISABLE_UART
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
vpath blink.c $(ROOTDIR)/modules/test_modules/blink/
vpath surge.c $(ROOTDIR)/modules/test_modules/surge/
vpath test_param.c $(ROOTDIR)/modules/test_modules/test_param/
vpath neighbor.c $(ROOTDIR)/modules/routing/neighbor/
vpath tree_routing.c $(ROOTDIR)/modules/routing/tree_routing/
vpath splt.c ../../modules/splt/
vpath rfid.c ../../modules/rfid/
vpath uart_mod_rfid.c ../../modules/uart_mod/

