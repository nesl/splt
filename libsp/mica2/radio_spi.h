/* -*- Mode: C; tab-width:4 -*- */
/* ex: set ts=4 shiftwidth=4 softtabstop=4 cindent: */

#ifndef __RADIO_SPI_H__
#define __RADIO_SPI_H__


/**
 * @brief SPI related functions
 */
#define spi_isBufBusy()  bit_is_clear(SPSR,SPIF)
sp_ret_t radio_spi_start_write(uint8_t data);
sp_ret_t radio_spi_start_read(uint8_t *buf);
void radio_spi_init();

#define radio_spi_write(d)   do{ radio_spi_start_write(d); TASK_WAIT(); } while(0)
#define radio_spi_read(b)    do{ radio_spi_start_read(b); TASK_WAIT();  } while(0)


// Added ! -Ram
#define spi_enableIntr() do{ SPCR = 0xC0; DDRB &= ~(1);} while(0)
#define spi_disableIntr() do{ SPCR &= ~(0xC0); DDRB |= (1); PORTB &= ~(1);}while(0)
#define spi_txmode() do{ SET_MISO_DD_OUT(); SET_MOSI_DD_OUT(); }while(0)
#define spi_rxmode() do{ SET_MISO_DD_IN(); SET_MOSI_DD_IN(); }while(0)

#endif // _SOS_SPI_H

