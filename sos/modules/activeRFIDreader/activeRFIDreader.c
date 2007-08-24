
#include <sys_module.h>

//#define LED_DEBUG
#include <led_dbg.h>

#include "activeRFID.h"
#include "splt.h"
#include <routing/tree_routing/tree_routing.h>
#include <module.h>

#define aRFID_TIMER_INTERVAL	1024L
#define aRFID_TID		0

#define MSG_RFID_COMMAND (MOD_MSG_START+2)
#define MSG_RFID_RESPONSE (MOD_MSG_START+3)
#define MSG_RFID_TAG (MOD_MSG_START+4)


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

	sys_timer_start(aRFID_TID,aRFID_TIMER_INTERVAL,TIMER_REPEAT);
	s->counter = 0;	
        break;
      }


    case MSG_FINAL:
      {
        DEBUG("Rfidreader Stop\n");
        break;
      }

    case MSG_TIMER_TIMEOUT:
	{
		sys_led(LED_YELLOW_TOGGLE);
		//sys_post_net(DFLT_APP_ID0, MSG_RFID_RESPONSE, 1, (void*)address, SOS_MSG_RELEASE, BCASE_ADDRESS);
		s->counter=0;
		break;

	}
    case MSG_RFID_TAG:
	{
		sys_led(LED_RED_TOGGLE);
		uint8_t *rfidfield;
		uint8_t *payload;
		payload = sys_msg_take_data(msg);
		rfidfield = sys_malloc(3*sizeof(uint8_t));
		if (rfidfield !=NULL)
		{
			rfidfield[0]=sys_id();
			rfidfield[1]=msg->rssi;
			memcpy(rfidfield+2, payload, 1);
			sys_post_net(DFLT_APP_ID0, MSG_RFID_RESPONSE, 3, (void*)rfidfield, SOS_MSG_RELEASE, BCAST_ADDRESS);
		}
		sys_free(payload);
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


