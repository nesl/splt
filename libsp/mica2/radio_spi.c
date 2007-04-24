/* -*- Mode: C; tab-width:4 -*- */
/* ex: set ts=4: */

#include <pin_map.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <stdlib.h>
#include "sp_task.h"
#include "radio_spi.h"
#include "cc1k.h"
#include "led.h"

static uint8_t   out_byte;
static sp_pid_t  read_pid;
static sp_pid_t  write_pid;
static uint8_t*  read_buf;
static uint8_t   txmode;

SIGNAL (SIG_SPI) 
{
	sp_pid_t tmp;
	if( read_pid != NULL_PID ) {
		if( txmode == 1 ) {
			//
			// If we are still in the writing process
			// wait until write is done
			//
			asm volatile("spi_prestart_read:");
			if( write_pid == NULL_PID) {
				//volatile uint8_t tmp;
				spi_disableIntr();
				ENABLE_INT();
				cc1k_cnt_RxMode();
				spi_rxmode();
				DISABLE_INT();
				spi_enableIntr();
				//
				// Clear interrupts
				//
				//tmp = SPSR;
				//tmp = SPDR;
				txmode = 0;
			}
			asm volatile("spi_start_read:"); 
		} else {
			// we invert the data here as it is always inverted.
			*read_buf = ~(SPDR);
			tmp = read_pid;
			read_pid = NULL_PID;
			//ENABLE_INT();
			sp_signal(tmp);
			return;
		}
	}
	else if( write_pid != NULL_PID ) {
		asm volatile("spi_prestart_write:");
		if( txmode == 0 ) {
			//volatile uint8_t tmp;
			spi_disableIntr();
			ENABLE_INT();
			cc1k_cnt_TxMode();                              
			spi_txmode();
			DISABLE_INT();
			spi_enableIntr();
			//
			// Clear interrupts
			//
			//tmp = SPSR;
			//tmp = SPDR;
			txmode = 1;
		}
		asm volatile("spi_start_write:");
		SPDR = out_byte;
		tmp = write_pid;
		write_pid = NULL_PID;
		//ENABLE_INT();
		sp_signal(tmp);
		return;
	} else {
		led_red_toggle();
		//SPDR = 0xaa;
	}
	//SP_INT_END();
}

sp_ret_t radio_spi_start_write(uint8_t data)
{
	HAS_CRITICAL_SECTION;
	ENTER_CRITICAL_SECTION();
	if( write_pid != NULL_PID ) {
		LEAVE_CRITICAL_SECTION();
		return -EBUSY;
	}
	
	out_byte = data;
	write_pid = CURRENT_TASK()->pid;
	LEAVE_CRITICAL_SECTION();
	return 0;
}

sp_ret_t radio_spi_start_read( uint8_t *buf )
{
	HAS_CRITICAL_SECTION;
	ENTER_CRITICAL_SECTION();
	if( read_pid != NULL_PID ) {
		LEAVE_CRITICAL_SECTION();
		return -EBUSY;
	}
	
	read_buf = buf;
	read_pid = CURRENT_TASK()->pid;
	LEAVE_CRITICAL_SECTION();
	return 0;
}

void radio_spi_init()
{
	SET_SCK_DD_IN();
	SET_MISO_DD_IN(); // miso
	SET_MOSI_DD_IN(); // mosi
	SPCR &= ~(1<<(CPOL));    // Set proper polarity...
	SPCR &= ~(1<<(CPHA));    // ...and phase
	SPCR |= (1<<(SPIE));  // enable spi port
	SPCR |= (1<<(SPE));
	
	read_pid = NULL_PID;
	write_pid = NULL_PID;
	
	cc1k_cnt_init();
	cc1k_cnt_SelectLock(0x9);
	txmode = 0;
	
	cc1k_cnt_start();
    cc1k_cnt_BIASOn();
    spi_rxmode();
    cc1k_cnt_RxMode();
    //spi_enableIntr();
}


