
#include <sys_module.h>

//#define LED_DEBUG
#include <led_dbg.h>

#include "rfid.h"

#define MSG_RFID_COMMAND (MOD_MSG_START+2)
#define MSG_RFID_RESPONSE (MOD_MSG_START+3)

typedef struct {
  uint8_t pid;
  uint8_t state;
} app_state_t;

/**
 * Rfid module
 */

static int8_t rfid_msg_handler(void *start, Message *e);

static const mod_header_t mod_header SOS_MODULE_HEADER = {
  .mod_id         = DFLT_APP_ID1,
  .state_size     = sizeof(app_state_t),
  .num_sub_func   = 0,
  .num_prov_func  = 0,
  .platform_type  = HW_TYPE /* or PLATFORM_ANY */,
  .processor_type = MCU_TYPE,
  .code_id        = ehtons(DFLT_APP_ID1),
  .module_handler = rfid_msg_handler,
};


static uint8_t rfid_cmd[8] = { 0x0d, 0x30, 0x31, 0x31,
  0x34, 0x30, 0x31, 0x0d };

static int8_t rfid_msg_handler(void *state, Message *msg)
{
  app_state_t *s = (app_state_t*)state;

  switch (msg->type){
    case MSG_INIT:
      {
        s->pid = msg->did;
        s->state = 0;
        DEBUG("Rfid Start\n");

        sys_post(KER_UART_PID, MSG_RFID_COMMAND, sizeof(rfid_cmd), rfid_cmd, 0);
        break;
      }


    case MSG_FINAL:
      {
        DEBUG("Rfid Stop\n");
        break;
      }

    case MSG_RFID_RESPONSE:
      {
        uint8_t msg_len;
        uint8_t *buff_to_send;
        msg_len = msg->len;
        buff_to_send = (uint8_t*)ker_msg_take_data(s->pid, msg);
        if (buff_to_send != NULL){
          sys_post_net(DFLT_APP_ID0, msg->type, msg_len, (void*)buff_to_send, SOS_MSG_RELEASE, BCAST_ADDRESS);
        }

        break;
      }
    default:
      return -EINVAL;
  }

  return SOS_OK;
}

#ifndef _MODULE_
mod_header_ptr rfid_get_header()
{
  return sos_get_header_address(mod_header);
}
#endif


