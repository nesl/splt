/* -*- Mode: C; tab-width:2 -*- */
/* ex: set ts=2 shiftwidth=2 softtabstop=2 cindent: */

#include <avr/io.h>
#include <avr/interrupt.h>

#include <sys_module.h>
#include <string.h>

#define LED_DEBUG
#include <led_dbg.h>

#include <mts310sb.h>

#define ACCEL_TEST_APP_TID 0
#define ACCEL_TEST_APP_INTERVAL 1024

#define ACCEL_TEST_PID DFLT_APP_ID0

#define UART_MSG_LEN 3
#define CHANGE_FREQ 10000
#define CHANGE_FREQ_2 20000
#define MAX_COUNT 100

#define MSG_ACCEL_DATA (MOD_MSG_START + 1)

enum {
	ACCEL_TEST_APP_INIT=0,
	ACCEL_TEST_APP_IDLE,
	ACCEL_TEST_APP_ACCEL_0,
	ACCEL_TEST_APP_ACCEL_0_BUSY,
	ACCEL_TEST_APP_ACCEL_1,
	ACCEL_TEST_APP_ACCEL_1_BUSY,
};

typedef struct {
	uint8_t pid;
	uint8_t state;
	uint16_t counter;
} app_state_t;

typedef struct {
	uint8_t address;
	double variance;
} variance_t;

typedef struct {
	uint8_t address;
	uint16_t count;
} waterflow_t;

typedef struct {
	uint8_t id;
	uint8_t data[UART_MSG_LEN];
} data_msg_t;

static uint16_t number = 0;

static int8_t water_meter_msg_handler(void *state, Message *msg);

static const mod_header_t mod_header SOS_MODULE_HEADER = {
	.mod_id         = ACCEL_TEST_PID,
	.state_size     = sizeof(app_state_t),
	.num_timers     = 1,
	.num_sub_func   = 0,
	.num_prov_func  = 0,
	.platform_type = HW_TYPE,
	.processor_type = MCU_TYPE,
	.code_id = ehtons(ACCEL_TEST_PID),
	.module_handler = water_meter_msg_handler,
};


static int8_t water_meter_msg_handler(void *state, Message *msg)
{
	app_state_t *s = (app_state_t *) state;

	switch ( msg->type ) {

		case MSG_INIT:
//      EICRA = (1<<ISC00)|(1<<ISC01);
//			EIMSK = (1<<INT0);
//			MCUCR |= (1<<ISC01) | (1 << ISC00);
			EICRA = (1<<ISC00)|(1<<ISC01);
	    EIMSK |= (1<<INT0)|(1<<INT1);
		  MCUCR |= (1<<ISC01)|(1<<ISC00);
			sei();
			sys_led(LED_GREEN_OFF);
			sys_led(LED_YELLOW_OFF);
			sys_led(LED_RED_OFF);

			s->state = ACCEL_TEST_APP_INIT;
			s->pid = msg->did;
			s->counter = 0;
			sys_timer_start(ACCEL_TEST_APP_TID, ACCEL_TEST_APP_INTERVAL, SLOW_TIMER_REPEAT);
			break;

		case MSG_FINAL:
			sys_timer_stop( ACCEL_TEST_APP_TID);
			break;

		case MSG_TIMER_TIMEOUT:
			{
        waterflow_t *waterflow = (waterflow_t*) sys_malloc(sizeof(waterflow_t));
				waterflow->count = number;
				waterflow->address = sys_id();
				number = 0;
				if(sys_id() == 0)
				{
				sys_post_uart(s->pid, MSG_ACCEL_DATA, sizeof(waterflow_t),(void*)waterflow, SOS_MSG_RELEASE, BCAST_ADDRESS);
        }
				else
				{
					sys_post_net(s->pid, MSG_ACCEL_DATA, sizeof(waterflow_t),(void*)waterflow, SOS_MSG_RELEASE, BCAST_ADDRESS);
				}
				
				//			  sys_led(LED_RED_TOGGLE);
			}
			break;

		case MSG_ACCEL_DATA:
			{
				uint8_t *payload;
				uint8_t msg_len;

				msg_len = msg->len;
				payload = sys_msg_take_data(msg);
		//		sys_free(payload);

				sys_post_uart (
						s->pid,
						MSG_ACCEL_DATA,
						msg_len,
						payload,
						SOS_MSG_RELEASE,
						BCAST_ADDRESS);
			}
			break;

		default:
			return -EINVAL;
			break;
	}
	return SOS_OK;
}

SIGNAL(SIG_INTERRUPT1)
{
	sys_led(LED_RED_TOGGLE);
}
SIGNAL(SIG_INTERRUPT0)
{
	  sys_led(LED_RED_TOGGLE);
    number++;
}


#ifndef _MODULE_
mod_header_ptr water_meter_get_header() {
	return sos_get_header_address(mod_header);
}
#endif

