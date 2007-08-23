
#include <sys_module.h>

//#define LED_DEBUG
#include <led_dbg.h>

#include "rfid.h"
#include "splt.h"
#include <routing/tree_routing/tree_routing.h>
#include <module.h>

#define MSG_RFID_COMMAND (MOD_MSG_START+2)
#define MSG_RFID_RESPONSE (MOD_MSG_START+3)

typedef struct {
  func_cb_ptr get_hdr_size;
  uint8_t hdr_size;
  uint32_t seq_no;
  uint16_t counter;
  uint8_t pid;
  uint8_t state;
} app_state_t;

/**
 * Rfid module
 */

static int8_t rfid_msg_handler(void *start, Message *e);
static void send_msg(app_state_t *s, uint8_t *buf, uint8_t msg_len, uint8_t msg_type);

static const mod_header_t mod_header SOS_MODULE_HEADER = {
  .mod_id         = DFLT_APP_ID0,
  .state_size     = sizeof(app_state_t),
  .num_sub_func   = 1,
  .num_prov_func  = 0,
  .platform_type  = HW_TYPE /* or PLATFORM_ANY */,
  .processor_type = MCU_TYPE,
  .code_id        = ehtons(DFLT_APP_ID0),
  .module_handler = rfid_msg_handler,
  .funct = {
    [0] = {error_8, "Cvv0", TREE_ROUTING_PID, MOD_GET_HDR_SIZE_FID},
  },
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
        s->counter = 0;
        s->hdr_size = SOS_CALL(s->get_hdr_size, get_hdr_size_proto);
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
        uint8_t *msg_rssi;
        msg_len = msg->len;
        buff_to_send = (uint8_t*)ker_msg_take_data(s->pid, msg);
        if (buff_to_send != NULL){
          send_msg(s, buff_to_send, msg_len, SPLT_TYPE_RFREADING);
          //	  sys_free(buff_to_send);

          //	  sys_post(TREE_ROUTING_PID, MSG_SEND_PACKET, msg_len, (void*)buff_to_send,SOS_MSG_RELEASE);
          if (sys_id() ==1){
            sys_post_uart(msg->did, MSG_RFID_RESPONSE, msg_len,(void*)buff_to_send, SOS_MSG_RELEASE, BCAST_ADDRESS);
            msg_rssi = sys_malloc(sizeof(int8_t));
            if (msg_rssi != NULL){
              *msg_rssi = msg->rssi;
              sys_post_uart(msg->did, MSG_RFID_RESPONSE+1, sizeof((*msg_rssi)), (void*)msg_rssi, SOS_MSG_RELEASE, BCAST_ADDRESS);
            }
            sys_led(LED_RED_TOGGLE);
          } else{
            sys_post_net(DFLT_APP_ID0, msg->type, msg_len, (void*)buff_to_send, SOS_MSG_RELEASE, BCAST_ADDRESS);
          }
        }

        break;
      }
    default:
      return -EINVAL;
  }

  return SOS_OK;
}


static void send_msg(app_state_t *s, uint8_t *buf, uint8_t msg_len,uint8_t msg_type) {
  uint8_t *pkt;
  SpltMsg *smsg;

  // malloc the message
  pkt = (uint8_t*)sys_malloc(s->hdr_size + msg_len + sizeof(SpltMsg));
  if(pkt == NULL) return;

  // Setup the message
  smsg = (SpltMsg*)(pkt + s->hdr_size);
  smsg->type = msg_type;
  smsg->seq_no = ehtonl(s->seq_no++);
  smsg->originaddr = ehtons(ker_id());
  smsg->data_len = msg_len;
  memcpy((uint8_t*)(pkt+s->hdr_size+sizeof(SpltMsg)), buf, msg_len);

  // send the message

  sys_post(TREE_ROUTING_PID, MSG_SEND_PACKET,
      s->hdr_size + sizeof(SpltMsg)+msg_len, (uint8_t*)pkt, SOS_MSG_RELEASE);
  //  sys_free(pkt);

}




#ifndef _MODULE_
mod_header_ptr rfid_get_header()
{
  return sos_get_header_address(mod_header);
}
#endif


