PROJ  = app
SRCS  = fixed_fft.c sp_task.c sp_timer.c plat_avr.c hw_timer.c sp_rand.c
SRCS += cc1k_cnt.c radio_spi.c adc.c cc1k.c cc1k_radio.c crc.c systime.c
OBJS += $(SRCS:.c=.o)
INSTALLER = uisp -dpart=atmega128 -dprog=mib510 -dserial=$(PORT)

VPATH  += ../core/ ../lib/ ../avr/ ../mica2/
INCDIR += -I../core/ -I../lib/ -I../avr/ -I../mica2/
	
%.o : %.c
	avr-gcc -c -g -Os -fshort-enums -Wa,-mmcu=atmega128,-Wa,-gstabs,-adhlmsn=$(notdir $(<:.c=.lst)) -mmcu=atmega128 $(INCDIR) -Wall -DPLATFORM_AVR $< -o $@

%.elf: $(OBJS)
	avr-gcc -g -Wl,-Map=$(PROJ).map,--cref $(OBJS) -lm -mmcu=atmega128 -o $@

%.elf.lst: %.elf
	avr-objdump -h -S $< > $@

%.od: %.elf
	avr-objdump -zhD $< > $@

%.srec : %.elf
	avr-objcopy -O srec -R .eeprom $< $@
	
all: $(PROJ).od $(PROJ).elf $(PROJ).elf.lst $(OBJS) 
	avr-size -A $(PROJ).elf

clean:
	rm -f $(PROJ).elf $(PROJ).od $(OBJS) *.lst *.map

install: $(PROJ).srec
	$(INSTALLER) --erase						    
	sleep 2
	$(INSTALLER) --upload if=$(PROJ).srec
	sleep 2
	$(INSTALLER) --verify if=$(PROJ).srec

