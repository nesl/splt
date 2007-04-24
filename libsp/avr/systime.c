
#include <inttypes.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "sp_task.h"


union time_u
{
	struct
	{
		uint16_t low;
		uint16_t high;
	};
	uint32_t full;
};

static volatile uint16_t currentTime;


SIGNAL(SIG_OVERFLOW3)
{
	++currentTime;
}



void systime_init()
{
	uint8_t etimsk;
	HAS_CRITICAL_SECTION;

	TCCR3A = 0x00;
	TCCR3B = 0x00;

	ENTER_CRITICAL_SECTION();
	{
		etimsk = ETIMSK;
		etimsk &= (1<<OCIE1C);
		etimsk |= (1<<TOIE3);
		ETIMSK = etimsk;
	}
	LEAVE_CRITICAL_SECTION();
	//! start the timer
	//! start the timer with 1/64 prescaler, 115.2 KHz on MICA2
	TCCR3B = 0x03; 
}   

