/* -*- Mode: C; tab-width:2 -*- */
/* ex: set ts=2 shiftwidth=2 softtabstop=2 cindent: */

#include <sys_module.h>
#include <string.h>

#define LED_DEBUG
#include <led_dbg.h>

#include <h34c.h>


#define MSG_RFID_TAG (MOD_MSG_START+4)
#define TIMER_TRIGGER_POST 1
#define TIMER_PERIODIC_POST 2

#define TIMER_TRIGGER_INTERVAL 1024L
#define TIMER_PERIODIC_INTERVAL 20000L

#define ACCEL_TEST_APP_TID 0
#define ACCEL_TEST_APP_INTERVAL 128L

#define ACCEL_TEST_PID DFLT_APP_ID0

#define UART_MSG_LEN 3

enum {
	ACCEL_TEST_APP_INIT=0,
	ACCEL_TEST_APP_IDLE,
	ACCEL_TEST_APP_ACCEL_0,
	ACCEL_TEST_APP_ACCEL_0_BUSY,
	ACCEL_TEST_APP_ACCEL_1,
	ACCEL_TEST_APP_ACCEL_1_BUSY,
	ACCEL_TEST_APP_ACCEL_2,
	ACCEL_TEST_APP_ACCEL_2_BUSY,
};

typedef struct {
	uint8_t pid;
	uint8_t state;
	uint8_t fire;
	uint16_t X;
	uint16_t Y;
	uint16_t Z;
	uint16_t Xpre;
	uint16_t Ypre;
	uint16_t Zpre;
} app_state_t;


static int8_t accel_test_msg_handler(void *state, Message *msg);

static const mod_header_t mod_header SOS_MODULE_HEADER = {
	.mod_id         = ACCEL_TEST_PID,
	.state_size     = sizeof(app_state_t),
	.num_timers     = 1,
	.num_sub_func   = 0,
	.num_prov_func  = 0,
	.platform_type = HW_TYPE,
	.processor_type = MCU_TYPE,
	.code_id = ehtons(ACCEL_TEST_PID),
	.module_handler = accel_test_msg_handler,
};


static int8_t accel_test_msg_handler(void *state, Message *msg)
{
	app_state_t *s = (app_state_t *) state;

	switch ( msg->type ) {

		case MSG_INIT:
			sys_led(LED_RED_OFF);
			sys_led(LED_GREEN_OFF);
			sys_led(LED_YELLOW_OFF);

			s->state = ACCEL_TEST_APP_INIT;
			s->pid = msg->did;
			s->fire = 0;
			sys_timer_start(ACCEL_TEST_APP_TID, ACCEL_TEST_APP_INTERVAL, SLOW_TIMER_REPEAT);
			sys_timer_start(TIMER_TRIGGER_POST, TIMER_TRIGGER_INTERVAL, TIMER_REPEAT);
			sys_timer_start(TIMER_PERIODIC_POST, TIMER_PERIODIC_INTERVAL, TIMER_REPEAT);
			if(sys_sensor_enable(H34C_ACCEL_0_SID) != SOS_OK) {
				sys_led(LED_RED_ON);
				sys_timer_stop( ACCEL_TEST_APP_TID);
			}
			break;

		case MSG_FINAL:
			sys_sensor_disable(H34C_ACCEL_0_SID);
			sys_timer_stop( ACCEL_TEST_APP_TID);
			break;

		case MSG_TIMER_TIMEOUT:
			{

				MsgParam *param = (MsgParam*) (msg->data);
				switch (param->byte){
					case ACCEL_TEST_APP_TID:
						{
							uint16_t temp = (s->Z-s->Zpre)*(s->Z-s->Zpre)+(s->Y-s->Ypre)*(s->Y-s->Ypre)+(s->X-s->Xpre)*(s->X-s->Xpre);
							if (temp > 10000)
							{ 
								s->fire = 5;
							}
						
				
						switch (s->state) {
							case ACCEL_TEST_APP_INIT:
								// do any necessary init here
								s->state = ACCEL_TEST_APP_IDLE;
								break;
					
							case ACCEL_TEST_APP_IDLE:
								s->state = ACCEL_TEST_APP_ACCEL_0;
								break;
					
							case ACCEL_TEST_APP_ACCEL_0:
								s->state = ACCEL_TEST_APP_ACCEL_0_BUSY;
								sys_sensor_get_data(H34C_ACCEL_0_SID);
								break;
					
							case ACCEL_TEST_APP_ACCEL_0_BUSY:
								//s->state = ACCEL_TEST_APP_ACCEL_1;
								break;
					
							case ACCEL_TEST_APP_ACCEL_1:
								s->state = ACCEL_TEST_APP_ACCEL_1_BUSY;
								sys_sensor_get_data(H34C_ACCEL_1_SID);
								break;

							case ACCEL_TEST_APP_ACCEL_1_BUSY:
								//s->state = ACCEL_TEST_APP_ACCEL_0;
								break;
					
							case ACCEL_TEST_APP_ACCEL_2:
								s->state = ACCEL_TEST_APP_ACCEL_2_BUSY;
								sys_sensor_get_data(H34C_ACCEL_2_SID);
								break;
					
							case ACCEL_TEST_APP_ACCEL_2_BUSY:
								//s->state = ACCEL_TEST_APP_ACCEL_0;
								break;
							default:
								//sys_led(LED_RED_TOGGLE);
								s->state = ACCEL_TEST_APP_INIT;
								break;
						}
						}
						break;
					case TIMER_TRIGGER_POST:
						{
							if(s->fire>0)
							{
								sys_led(LED_RED_TOGGLE);
								s->fire=s->fire-1;
								uint8_t data;
								uint8_t* address;
								data = sys_id();
								address = sys_malloc(sizeof(uint8_t));
								if (address !=NULL)
								{
									*address = data;
									sys_post_net(DFLT_APP_ID2, MSG_RFID_TAG, 1, (void*)address, SOS_MSG_RELEASE | SOS_MSG_RELIABLE, BCAST_ADDRESS);
								}
							}
						}
						break;
					case TIMER_PERIODIC_POST:
						{
							if(s->fire==0)
							{
								//sys_led(LED_RED_TOGGLE);
							}
						}
						break;
				}
					
			}	

			
				break;

		case MSG_DATA_READY:
			{
        uint8_t *payload;
				payload = sys_msg_take_data(msg);

				if(payload[0]==0x01)
				{
					s->Xpre=s->X;
					s->X = *(uint16_t*)((uint8_t*)(payload+1));	
				}
				if(payload[0]==0x02)
				{
					s->Ypre=s->Y;
					s->Y = *(uint16_t*)((uint8_t*)(payload+1));
				}
				if(payload[0]==0x03)
				{
					s->Zpre=s->Z;
					s->Z = *(uint16_t*)((uint8_t*)(payload+1));

          //int16_t temp = (s->Z-s->Zpre)*(s->Z-s->Zpre)+(s->Y-s->Ypre)*(s->Y-s->Ypre)+(s->X-s->Xpre)*(s->X-s->Xpre);
				                
				  //if (temp > 1000)
					//{   
								//sys_led(LED_GREEN_TOGGLE);
					//} 
								
					
				}
				sys_free(payload);

				
				switch(s->state) {
					case ACCEL_TEST_APP_ACCEL_0_BUSY:
						s->state = ACCEL_TEST_APP_ACCEL_1;
						break;
					case ACCEL_TEST_APP_ACCEL_1_BUSY:
						s->state = ACCEL_TEST_APP_ACCEL_2;
						break;
					case ACCEL_TEST_APP_ACCEL_2_BUSY:
						s->state = ACCEL_TEST_APP_ACCEL_0;
						break;
				}
			}
			break;

		default:
			return -EINVAL;
			break;
	}
	return SOS_OK;
}

#ifndef _MODULE_
mod_header_ptr accel_test_get_header() {
	return sos_get_header_address(mod_header);
}
#endif

