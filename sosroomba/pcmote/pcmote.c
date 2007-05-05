/* -*- Mode: C; tab-width:4 -*- */
/* ex: set ts=4 shiftwidth=4 softtabstop=4 cindent: */
/**
 * @brief Example module for SOS
 * This module shows some of concepts in SOS
 */

/**
 * Module needs to include <module.h>
 */
#include <module.h>
#define LED_DEBUG
#include <led_dbg.h>
#include <uart_system.h>

#include "pcmote.h"

#define MSG_LOC_UPDATE (MOD_MSG_START + 10)

/**
 * Module can define its own state
 */
typedef struct {
  uint8_t pid;
  uint8_t state;
  uint8_t* buff_to_send;
  uint8_t ackbyte;
} app_state_t;

/**
 * Blink module
 *
 * @param msg Message being delivered to the module
 * @return int8_t SOS status message
 *
 * Modules implement a module function that acts as a message handler.  The
 * module function is typically implemented as a switch acting on the message
 * type.
 *
 * All modules should included a handler for MSG_INIT to initialize module
 * state, and a handler for MSG_FINAL to release module resources.
 */

static int8_t pcmote_msg_handler(void *start, Message *e);

/**
 * This is the only global variable one can have.
 */
static mod_header_t mod_header SOS_MODULE_HEADER = {
	.mod_id         = DFLT_APP_ID0,
	.state_size     = sizeof(app_state_t),
    .num_timers     = 1,
	.num_sub_func   = 0,
	.num_prov_func  = 0,
	.platform_type  = HW_TYPE /* or PLATFORM_ANY */,
	.processor_type = MCU_TYPE,
	.code_id        = ehtons(DFLT_APP_ID0),
	.module_handler = pcmote_msg_handler,
};


static int8_t pcmote_msg_handler(void *state, Message *msg)
{
  /**
   * The module is passed in a void* that contains its state.  For easy
   * reference it is handy to typecast this variable to be of the
   * applications state type.  Note that since we are running as a module,
   * this state is not accessible in the form of a global or static
   * variable.
   */
  app_state_t *s = (app_state_t*)state;

  /**
   * Switch to the correct message handler
   */
  switch (msg->type){
	/**
	 * MSG_INIT is used to initialize module state the first time the
	 * module is used.  Many modules set timers at this point, so that
	 * they will continue to receive periodic (or one shot) timer events.
	 */
  case MSG_INIT:
  {
	  LED_DBG(LED_GREEN_TOGGLE);
	  s->pid = msg->did;
	  s->state = 0;

      s->buff_to_send=NULL;
      s->ackbyte = 0x7f;
	  break;
	}


	/**
	 * MSG_FINAL is used to shut modules down.  Modules should release all
	 * resources at this time and take care of any final protocol
	 * shutdown.
	 */
  case MSG_FINAL:
	{
	  break;
	}


	/**
	 * All timers addressed to this PID, regardless of the timer ID, are of
	 * type MSG_TIMER_TIMEOUT and handled by this handler.  Timers with
	 * different timer IDs can be further distinguished by testing for the
	 * type, as demonstrated in the relay module.
	 */
  case MSG_TIMER_TIMEOUT:
	{
	  break;
	}

  case MSG_LOC_UPDATE:
    {
        uint8_t msg_len;
		LED_DBG(LED_YELLOW_TOGGLE);
		DEBUG_PID(s->pid, "Blink Yellow ON\n");
        /*
		post_net(s->pid, s->pid, MSG_ROOMBA_DATA_FROM_MOTE, 	
				msg->len, msg->data,
				SOS_MSG_RELIABLE, BCAST_ADDRESS);
*/
        msg_len = msg->len;
        s->buff_to_send = (uint8_t*)ker_msg_take_data(s->pid, msg);
        if (s->buff_to_send != NULL){
            post_net(s->pid, s->pid, MSG_LOC_UPDATE, msg_len, (void*)s->buff_to_send, SOS_MSG_RELEASE | SOS_MSG_RELIABLE, BCAST_ADDRESS);
        }
		/*post_uart(s->pid, s->pid, MSG_ROOMBA_DATA_FROM_MOTE, 	
				msg->len, msg->data,
				SOS_MSG_RELIABLE, BCAST_ADDRESS);
                */
        break;
    }

	/**
	 * The default handler is used to catch any messages that the module
	 * does no know how to handle.
	 */
  default:
	return -EINVAL;
  }

  /**
   * Return SOS_OK for those handlers that have successfully been handled.
   */
  return SOS_OK;
}

#ifndef _MODULE_
mod_header_ptr pcmote_get_header()
{
  return sos_get_header_address(mod_header);
}
#endif

