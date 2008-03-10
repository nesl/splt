
#include <sys_module.h>

//#define LED_DEBUG
#include <led_dbg.h>

#include "activeRFID.h"
#include "splt.h"
#include <routing/tree_routing/tree_routing.h>
#include <module.h>
#include <random.h>


#define aRFID_TIMER_INTERVAL	1024L
#define aRFID_TID		0
#define RAND_TID		1

#define MSG_RFID_COMMAND (MOD_MSG_START+2)
#define MSG_RFID_RESPONSE (MOD_MSG_START+3)
#define MSG_RFID_TAG (MOD_MSG_START+4)

#define GETBIT(X,I)             ( ( (X) & (1<<I) ) >> I )

//static uint8_t *rfid;

uint8_t id=0;
uint8_t rssi=0;
int8_t counter = 17;
uint16_t code = 0x00000000;

typedef struct {
  func_cb_ptr get_hdr_size;
  uint8_t hdr_size;
  uint32_t seq_no;
  uint16_t counter;
  uint8_t pid;
  uint8_t state;
} app_state_t;

/**
 * Rfid Reader module
 */

static int8_t rfidreader_msg_handler(void *start, Message *e);

static const mod_header_t mod_header SOS_MODULE_HEADER = {
  .mod_id         = DFLT_APP_ID2,
  .state_size     = sizeof(app_state_t),
  .num_sub_func   = 1,
  .num_prov_func  = 0,
  .platform_type  = HW_TYPE /* or PLATFORM_ANY */,
  .processor_type = MCU_TYPE,
  .code_id        = ehtons(DFLT_APP_ID2),
  .module_handler = rfidreader_msg_handler,
  .funct = {
    [0] = {error_8, "Cvv0", TREE_ROUTING_PID, MOD_GET_HDR_SIZE_FID},
  },
};



static int8_t rfidreader_msg_handler(void *state, Message *msg)
{
  app_state_t *s = (app_state_t*)state;

  switch (msg->type){
    case MSG_INIT:
      {
        s->pid = msg->did;
        s->state = 0;
        DEBUG("Rfidreader Start\n");

	s->counter = 0;	
        break;
      }


    case MSG_FINAL:
      {
        DEBUG("Rfidreader Stop\n");
        break;
      }

    case MSG_RFID_TAG:
	{
		sys_led(LED_RED_TOGGLE);
		uint8_t *payload;
		payload = sys_msg_take_data(msg);
		//rfid = sys_malloc(3*sizeof(uint8_t));
		id = (uint8_t)*payload;
		rssi = msg->rssi;
		code = id ;
		code = code << 8;
		code = code | rssi;
		//if (rfid !=NULL)
		//{
		//	rfid[0]=sys_id();
		//	rfid[1]=msg->rssi;
		//	memcpy(rfid+2, payload, 1);
		//	uint8_t time = (uint8_t)ker_rand();
			sys_timer_start(RAND_TID, 1024/20, TIMER_REPEAT);
		//}
		sys_free(payload);
		break;
	}
    case MSG_TIMER_TIMEOUT:
	{
		//if (rfid !=NULL)
		//{
		//sys_post_net(DFLT_APP_ID0, MSG_RFID_RESPONSE, 3, (void*)rfid, SOS_MSG_RELEASE | SOS_MSG_RELIABLE, 0x01);
		//}
		if(counter >= 16)
		{
		sys_led(LED_YELLOW_OFF);
		counter--;
		}
		else
		{
                if(GETBIT(code, counter) ==1)
                {
                sys_led(LED_YELLOW_OFF);
                } else
                {
                sys_led(LED_YELLOW_ON);
                }
                counter --;
                if(counter == -1)
                {
                counter = 17;
		sys_timer_stop(RAND_TID);
		sys_led(LED_YELLOW_ON);
                }
		}

		break;
	}
    default:
      return -EINVAL;
  }

  return SOS_OK;
}






#ifndef _MODULE_
mod_header_ptr rfidreader_get_header()
{
  return sos_get_header_address(mod_header);
}
#endif


