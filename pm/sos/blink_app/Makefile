# -*-Makefile-*-

PROJ = module_test_app
ROOTDIR = /home/zainul/work/202b/splt/trunk/sos-2x

SRCS += blink.c rfid.c uart_mod.c 
DEFS += -DNO_SOS_UART -DDISABLE_UART

include ../Makerules

vpath blink.c /home/zainul/work/202b/splt/trunk/sos-2x/modules/demos/blink/
vpath uart_mod.c /home/zainul/work/202b/splt/trunk/sos-2x/modules/demos/uart_mod/
vpath rfid.c /home/zainul/work/202b/splt/trunk/sos-2x/modules/demos/rfid/

clean2:
	rm -f *.o *.srec *.elf *.lst $(PROJ).map $(PROJ).exe $(PROJ).exe.out.* $(PROJ).od $(PROJ).srec.out.* .build.tmp
	rm -f $(PROJ).c Makefile

