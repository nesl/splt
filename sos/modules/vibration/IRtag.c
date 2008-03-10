
#include <sys_module.h>

//#define LED_DEBUG
#include <led_dbg.h>

#include "activeRFID.h"
#include "splt.h"
#include <routing/tree_routing/tree_routing.h>
#include <module.h>

#define aRFID_TIMER_INTERVAL	1024L
#define aRFID_TID		10

#define MSG_RFID_COMMAND (MOD_MSG_START+2)
#define MSG_RFID_RESPONSE (MOD_MSG_START+3)
#define MSG_RFID_TAG (MOD_MSG_START+4)
#define GETBIT(X,I)             ( ( (X) & (1<<I) ) >> I )

uint16_t code = 0x0FB0;
int8_t counter = 11;


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

static int8_t irtag_msg_handler(void *start, Message *e);

static const mod_header_t mod_header SOS_MODULE_HEADER = {
  .mod_id         = DFLT_APP_ID1,
  .state_size     = sizeof(app_state_t),
  .num_sub_func   = 1,
  .num_prov_func  = 0,
  .platform_type  = HW_TYPE /* or PLATFORM_ANY */,
  .processor_type = MCU_TYPE,
  .code_id        = ehtons(DFLT_APP_ID1),
  .module_handler = irtag_msg_handler,
  .funct = {
    [0] = {error_8, "Cvv0", TREE_ROUTING_PID, MOD_GET_HDR_SIZE_FID},
  },
};



static int8_t irtag_msg_handler(void *state, Message *msg)
{
  app_state_t *s = (app_state_t*)state;

  switch (msg->type){
    case MSG_INIT:
      {
        s->pid = msg->did;
        s->state = 0;
        DEBUG("Rfidtag Start\n");

	sys_timer_start(aRFID_TID,2,TIMER_REPEAT);	
        break;
      }


    case MSG_FINAL:
      {
        DEBUG("Rfid Stop\n");
        break;
      }

    case MSG_TIMER_TIMEOUT:
	{
		sys_led(LED_GREEN_TOGGLE);
		break;
	}
    default:
      return -EINVAL;
  }

  return SOS_OK;
}






#ifndef _MODULE_
mod_header_ptr irtag_get_header()
{
  return sos_get_header_address(mod_header);
}
#endif


