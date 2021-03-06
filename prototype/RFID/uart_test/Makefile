PROJ = test_uart
SRCS += $(PROJ).c
OBJS += $(SRCS:.c=.o)
INCDIR += -I./include

TRG = avr-

CC  = $(TRG)gcc

AS     = $(TRG)gcc 

OBJCOPY = $(TRG)objcopy

OBJDUMP = $(TRG)objdump

SIZE = $(TRG)size -A

MCU = atmega128

MCUFLAG += -mmcu=$(MCU)

CFLAGS += $(MCUFLAG) -Werror $(DEFS) -O0 -Wall -Wa,-adhlmsn=$(notdir $(<:.c=.lst))

LDFLAGS += $(MCUFLAG) -Wl,-Map=$(PROJ).map,--cref


################################################
###  install
################################################

ifndef PROG
PROG=mib510
endif
INSTALLER_PORG = -dprog=$(PROG)

# TODO: fix this to handle alternative parallel ports
#PORT=
ifneq ($(PORT),)
INSTALLER_PORT = -dserial=$(PORT)
endif

#IP=
ifneq ($(IP),)
INSTALLER_PORT = -dhost=$(IP)
endif

INSTALLER_PART = -dpart=$(MCU)

UISP_PROG = uisp

ifeq ($(PROG),jtag)
INSTALLER = avarice
else
INSTALLER = $(UISP_PROG) $(INSTALLER_PART) $(INSTALLER_PORG) $(INSTALLER_PORT)
endif



###################################################
# PROTOTYPES
###################################################
%.i : %.c
	$(CC) -E $(CFLAGS) $(INCDIR) $< -o $@

%.s : %.c
	$(CC) -c $(CFLAGS) $(INCDIR) $< -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCDIR) $< -o $@

%.o : %.s
	$(AS) -c $(INCDIR) $(SFLAGS) $<

%.o : %.S
	$(AS) -c $(SFLAGS) $(INCDIR) $< -o $@

%.elf: $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(LDFLAGS) -o $@
	$(SIZE) -A $@

%.elf.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.bin : %.elf
	$(OBJCOPY) -j .text -O binary -R .eeprom $< $@

%.srec : %.elf
	$(OBJCOPY) -j .text -O srec -R .eeprom $< $@


%.srec.out.$(ADDRESS): %.srec
	/home/kimyh/sos-2x/tools/utils/set-mote-id $< $@ 0x$(ADDRESS)




all:    $(PROJ).o $(PROJ).elf $(PROJ).elf.lst $(PROJ).bin $(PROJ).srec $(PROJ).srec.out.$(ADDRESS)
	$(INSTALLER) --wr_fuse_e=0xff
	sleep 2
	$(INSTALLER) --wr_fuse_h=0x9f
	sleep 2
	$(INSTALLER) --wr_fuse_l=0xbf
	sleep 2
	$(INSTALLER) --erase
	sleep 2
	#$(INSTALLER) --upload if=$(PROJ).srec.out.$(ADDRESS)
	$(INSTALLER) --upload if=$(PROJ).srec
clean:
	rm -f *.elf *.elf.lst *.bin *.lst *.o *.map *~ *.ihex *.srec *.srec.out.$(ADDRESS) *.exe.out.$(ADDRESS)
