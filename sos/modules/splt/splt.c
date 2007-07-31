/* -*- Mode: C; tab-width:4 -*- */
/* ex: set ts=4 shiftwidth=4 softtabstop=4 cindent: */
#include <sys_module.h>
#include <led_dbg.h>
#include "splt.h"
#include <routing/tree_routing/tree_routing.h>
#include <module.h>

typedef struct {
//! Function pointer to get the size of the tree routing header
  func_cb_ptr get_hdr_size; 
  uint8_t hdr_size;
  uint32_t seq_no;
  uint16_t counter;
} app_state_t;

static int8_t splt_msg_handler(void *start, Message *e);
//static void send_msg(app_state_t *s, MsgParam* param, uint8_t type);

static const mod_header_t mod_header SOS_MODULE_HEADER = {
  .mod_id         = SPLT_MOD_PID,
  .state_size     = sizeof(app_state_t),
  .num_sub_func   = 1,
  .num_prov_func  = 0,
  .platform_type  = HW_TYPE /* or PLATFORM_ANY */,
  .processor_type = MCU_TYPE,
  .code_id        = ehtons(DFLT_APP_ID0),
  .module_handler = splt_msg_handler,
  .funct = {
    [0] = {error_8, "Cvv0", TREE_ROUTING_PID, MOD_GET_HDR_SIZE_FID},
  },
};


static int8_t splt_msg_handler(void *state, Message *msg)
{
  app_state_t *s = (app_state_t*)state;
  
  switch (msg->type){
  case MSG_INIT:
    {
      s->counter = 0;
      s->hdr_size = SOS_CALL(s->get_hdr_size, get_hdr_size_proto);      
      sys_timer_start(SPLT_ONE_SHOT_TID, SPLT_TIMER_INTERVAL, TIMER_ONE_SHOT);
      break;
    }

  case MSG_TIMER_TIMEOUT:
    {
      MsgParam *param = (MsgParam*) (msg->data);
      switch (param->byte){
      case SPLT_ONE_SHOT_TID:
		{
		  sys_led(LED_YELLOW_TOGGLE);
		  // ask for data from power meter
		  sys_post_value(SPLT_MOD_PID, MSG_PM_DATA_READY, s->counter++, 0);
		  // ask for data from rfid reader
		  sys_post_value(SPLT_MOD_PID, MSG_RF_DATA_READY, s->counter++, 0);
		  
		  // temporarily start another timer, not needed when drivers are working
		  sys_timer_start(SPLT_REPEATING_TID, SPLT_TIMER_INTERVAL, TIMER_REPEAT);
		  break;
		}
      case SPLT_REPEATING_TID:
		{
		  if (sys_id() != SPLT_BASE_STATION_ADDRESS){
			sys_led(LED_YELLOW_TOGGLE);
			sys_post_value(SPLT_MOD_PID, MSG_PM_DATA_READY, s->counter++, 0);
			//sys_post_value(SPLT_MOD_PID, MSG_RF_DATA_READY, s->counter++, 0);
		  }
		}
      default: 
		{
		  return -EINVAL;
		}
      }
    }

  case MSG_PM_DATA_READY:
    {
	  sys_led(LED_RED_TOGGLE);
      // send the received data
  //    send_msg(s, (MsgParam*)(msg->data), SPLT_TYPE_PMREADING);
      break;
    }

  case MSG_RF_DATA_READY:
    {
	  sys_led(LED_RED_TOGGLE);
      // send the received data
//      send_msg(s, (MsgParam*)(msg->data), SPLT_TYPE_RFREADING);
      // ask for more data from rfid reader
      
      break;
    }

  case MSG_TR_DATA_PKT:
    {
      SpltMsg *smsg;
      smsg = (SpltMsg*)(((uint8_t*)(msg->data)) + s->hdr_size);
      if (sys_id() == SPLT_BASE_STATION_ADDRESS){
		sys_led(LED_GREEN_TOGGLE);
		uint8_t *payload;
		uint8_t msg_len;
		msg_len = msg->len;
		payload = sys_msg_take_data(msg);
		sys_post_uart(SPLT_MOD_PID, msg->type, msg_len,
					  (void*)payload, SOS_MSG_RELEASE, BCAST_ADDRESS);
      }
      break;
    }

  case MSG_FINAL:
    {
      break;
    }

  default:
    return -EINVAL;
  }
  
  return SOS_OK;
}
/*
static void send_msg(app_state_t *s, MsgParam* param, uint8_t type) {
  uint8_t *pkt;
  SpltMsg *smsg;
  
  // malloc the message
  pkt = (uint8_t*)sys_malloc(s->hdr_size + sizeof(SpltMsg));
  if(pkt == NULL) return;
  
  // Setup the message
  smsg = (SpltMsg*)(pkt + s->hdr_size);
  smsg->type = type;
  smsg->reading = ehtons(param->word);
  smsg->seq_no = ehtonl(s->seq_no++);
  smsg->originaddr = ehtons(ker_id());
  
  // send the message
  sys_post(TREE_ROUTING_PID, MSG_SEND_PACKET, 
	   s->hdr_size + sizeof(SpltMsg), (void*)pkt, SOS_MSG_RELEASE);
}
*/
#ifndef _MODULE_
mod_header_ptr splt_get_header()
{
  return sos_get_header_address(mod_header);
}
#endif


