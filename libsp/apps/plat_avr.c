
#define SENDER
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include "led.h"
#include "sp_task.h"
#include "sp_semaphore.h"
#include "adc.h"
#include "sp_timer.h"
#include "fixed_fft.h"
#include "cc1k_radio.h"
#include "systime.h"

#define M        7
#define NFFT    (1<<M)

sp_sem_t data_ready;
fixed    real_buf1[NFFT];
fixed    real_buf2[NFFT];
fixed*   sampling_buf;
fixed*   processing_buf;
sp_timer_t  timer1;

void sampling_task( void ) 
{
	static uint16_t i = 0;
	TASK_BEGIN();
	sampling_buf = real_buf1;
	processing_buf = NULL;
	sp_timer_open( &timer1, 32 ); // open periodic timer for reading
	
	while(1) {
		led_yellow_toggle();
		for(i = 0; i < NFFT; i++) {
			sp_timer_read(&timer1);
			adc_read(0x01, &(sampling_buf[i]));
			//cc1k_radio_send(&(sampling_buf[i]), 2);
		}
		//
		// Move to processing buf
		//
		processing_buf = sampling_buf;
		if( sampling_buf == real_buf1 ) {
			sampling_buf = real_buf2;
		} else {
			sampling_buf = real_buf1;
		}
		led_yellow_toggle();
		
		TASK_SEM_SIGNAL(&data_ready);
	}
	TASK_END();
}

void fft_task( void )
{
	static fixed    imag[NFFT];
	static uint16_t i = 0;
	TASK_BEGIN();
	
	while(1) {
		TASK_SEM_WAIT(&data_ready);
		led_red_toggle();
		for(i = 0; i < NFFT; i++) {
			imag[i] = 0;
		}
		// compute FFT from 512 samples, the routines takes 200 ms
		fix_fft(processing_buf, imag, M, 0);
		processing_buf = NULL;
		led_green_toggle();
	}
	TASK_END();
}

void test_radio( void )
{
	static uint32_t cnt;
	TASK_BEGIN();
	cnt = 0;
	while( 1 ) {
		cc1k_radio_send(&cnt, sizeof(cnt));
		cnt++;
		led_green_toggle();
	}
	TASK_END();
}

void test_receiver( void )
{
	static uint32_t buf;
	TASK_BEGIN();
	while( 1 ) {
		cc1k_radio_recv(&buf, sizeof(buf));
		led_green_toggle();
	}
	TASK_END();
}
//
// the interrupt handler
//
#ifdef SENDER
static uint16_t sp_node_address = 1;
#else
static uint16_t sp_node_address = 2;
#endif

void adc_init();
void timer_init();

int main()
{
	spk_init();
	sp_timer_init();
	led_init();
	adc_init();
	systime_init();

	TASK_SEM_INIT(&data_ready, 0);
	sp_create( fft_task, 1 );
	cc1k_radio_init();
	sp_create( sampling_task, 2 );
	/*
	if( sp_node_address == 1 ) {
		sp_create( test_radio, 1 );
		led_yellow_toggle();
	} else {
		sp_create( test_receiver, 1 );
		led_yellow_toggle();
	}	
	*/
	//ENABLE_INT();

	spk_sched();
	return 0;
}





