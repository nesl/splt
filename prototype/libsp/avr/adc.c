#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <stdlib.h>
#include "sp_task.h"
#include "sp_semaphore.h"
#include "adc.h"
#include "led.h"

enum {
	// must be power of 2
	ADC_WQ_TOTAL_SIZE   = 8,
	ADC_WQ_MASK         = ADC_WQ_TOTAL_SIZE - 1,
};

typedef struct sp_adc_t {
	sp_pid_t      pid;
	uint8_t       mux;
	uint16_t*     buf;
} sp_adc_t;


static sp_adc_t request[ADC_WQ_TOTAL_SIZE];
static uint8_t  adc_wq_head;
static volatile uint8_t  adc_wq_tail;

#include "led.h"

//
// We don't need to use SP_INT_BEGIN block as we only signal 
// just one task.  there is no need to worry about wrong priority order.
//
SIGNAL(SIG_ADC)
{
	register sp_pid_t dest;
	//SP_INT_BEGIN();
	
	*(request[adc_wq_head].buf) = ADC;
	dest = request[adc_wq_head].pid;
	request[adc_wq_head].pid = NULL_PID;
	adc_wq_head = (adc_wq_head + 1) & ADC_WQ_MASK;
	
	ADCSRA &= ~_BV(ADEN);

	if( adc_wq_head != adc_wq_tail ) {
		ADMUX   = request[adc_wq_head].mux;
		ADCSRA |= _BV(ADEN);
		ADCSRA |= _BV(ADSC);  // start conversion 
	} 
	
	//ENABLE_INT();
	sp_signal(dest);
	//SP_INT_END();
}

void adc_start( uint8_t channel, uint16_t* buf )
{
	uint8_t tmp;
	HAS_CRITICAL_SECTION;
	
	ENTER_CRITICAL_SECTION();
	tmp = adc_wq_tail;
	
	if( request[tmp].pid != NULL_PID ) {
		LEAVE_CRITICAL_SECTION();
		CURRENT_TASK()->errno = ENOMEM;
		return;
	}
	
	CURRENT_TASK()->errno = 0;
	adc_wq_tail = (tmp + 1) & ADC_WQ_MASK;
	request[tmp].pid = CURRENT_TASK()->pid;
	request[tmp].buf  = buf;
	request[tmp].mux  = channel;
	
	if( tmp == adc_wq_head ) {
		//
		// First conversion
		//
		ADMUX   = channel;
		ADCSRA |= _BV(ADEN);
		ADCSRA |= _BV(ADSC);  // start conversion 
	} 
	LEAVE_CRITICAL_SECTION();
}

void adc_init()
{
	uint8_t i;
	ADMUX = (0 | 0x1F);
	// disable ADC, clear any pending interrupts and enable ADC
	ADCSRA &= ~_BV(ADEN);
	ADCSRA |= (_BV(ADPS2)|_BV(ADPS1)); // 6
	ADCSRA |= _BV(ADIE);
		
	
	adc_wq_head = 0;
	adc_wq_tail = 0;
	
	for( i = 0; i < ADC_WQ_TOTAL_SIZE; i++ ) {
		request[i].pid = NULL_PID;
	}
}



