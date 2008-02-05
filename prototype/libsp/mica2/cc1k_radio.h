
#ifndef __CC1K_RADIO_H__
#define __CC1K_RADIO_H__

#define cc1k_radio_send(d,s) \
	do { cc1k_radio_start_send(d, s); TASK_WAIT(); } while(0)

#define cc1k_radio_recv(d,s) \
	do { cc1k_radio_start_recv(d, s); TASK_WAIT(); } while(0)

void cc1k_radio_start_send( void* data, uint8_t size );

void cc1k_radio_start_recv( void* data, uint8_t size );

void cc1k_radio_init( void );

#endif


