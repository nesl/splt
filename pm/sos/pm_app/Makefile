# -*-Makefile-*-

PROJ = module_test_app
ROOTDIR = $(SOSROOT)

SRCS += blink.c rfid.c uart_mod_pm.c 
DEFS += -DNO_SOS_UART -DDISABLE_UART
ADDRESS = 0x0002

include $(ROOTDIR)/config/Makerules

vpath blink.c $(ROOTDIR)/modules/demos/blink/
vpath uart_mod_pm.c ../uart_mod/
vpath rfid.c ../rfid/

clean2:
	rm -f *.o *.srec *.elf *.lst $(PROJ).map $(PROJ).exe $(PROJ).exe.out.* $(PROJ).od $(PROJ).srec.out.* .build.tmp
	rm -f $(PROJ).c Makefile

