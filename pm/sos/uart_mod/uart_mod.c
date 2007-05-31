#include <sys_module.h>

//#define LED_DEBUG
#include <led_dbg.h>

#include "uart_mod.h"

#define UART_TIMER_INTERVAL	32L
#define UART_TID               0
#define BUFSIZE             25

#define MSG_RFID_COMMAND (MOD_MSG_START+2)
#define MSG_RFID_RESPONSE (MOD_MSG_START+3)

static int8_t uart_mod_init();
static void USART1_send(uint8_t data);

/* GLOBAL VARIABLES - ooooo not goood*/
uint8_t *uart_mod_buf;
uint8_t uart_mod_bufcnt;

/**
 * Module can define its own state
 */
typedef struct {
    uint8_t pid;
} app_state_t;


/**
 * Uart module
 */

static int8_t uart_mod_msg_handler(void *start, Message *e);

static const mod_header_t mod_header SOS_MODULE_HEADER = {
    .mod_id         = KER_UART_PID,
    .state_size     = sizeof(app_state_t),
    .num_sub_func   = 0,
    .num_prov_func  = 0,
    .platform_type  = HW_TYPE /* or PLATFORM_ANY */,
    .processor_type = MCU_TYPE,
    .code_id        = ehtons(DFLT_APP_ID0),
    .module_handler = uart_mod_msg_handler,
};


static int8_t uart_mod_msg_handler(void *state, Message *msg)
{
    app_state_t *s = (app_state_t*)state;

    switch (msg->type){
        case MSG_INIT:
            {
                s->pid = msg->did;
                uart_mod_bufcnt = 0;
                uart_mod_buf = (uint8_t *)sys_malloc(BUFSIZE);
                DEBUG("Uart Start\n");
                uart_mod_init();
                ker_timer_init(KER_UART_PID, UART_TID, SLOW_TIMER_ONE_SHOT);
                sys_led(LED_YELLOW_ON);
                break;
            }

        case MSG_FINAL:
            {
                sys_timer_stop(UART_TID);
                DEBUG("Uart Stop\n");
                break;
            }


        case MSG_TIMER_TIMEOUT:
            {
              /* Timer fired, should sent out the message to the uart0 now
               * */
              uint8_t *tmpbuf;
              uint8_t tmpbufcnt;

              tmpbufcnt = uart_mod_bufcnt;
              tmpbuf = (uint8_t *)sys_malloc(tmpbufcnt);
              memcpy(tmpbuf, uart_mod_buf, tmpbufcnt);
              sys_post(DFLT_APP_ID1, MSG_RFID_RESPONSE, tmpbufcnt, tmpbuf, SOS_MSG_RELEASE);
              uart_mod_bufcnt = 0;
              sys_led(LED_YELLOW_TOGGLE);
              break;
            }
        case MSG_RFID_COMMAND:
            {
              uint8_t i, msg_len;
              msg_len = msg->len;

              for (i = 0; i < msg_len; i++) {
                USART1_send(*((msg->data)+i));
              }
              break;
            }

            default:
            return -EINVAL;
    }

    return SOS_OK;
}


#include <hardware.h>
#include <net_stack.h>
#include <message.h>
#include <malloc.h>
#include <sos_error_types.h>

#define LED_DEBUG
#include <led_dbg.h>

#include <avr/io.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <led.h>

#define BAUD_9600 47
#define BAUD_115_2K 3

#define USART1_BAUDRATE BAUD_9600


/**
 ****************************************
 Initialize the UART hardware on an AVR
 ****************************************
 */
static void USART1_init(void)
{
  UBRR1H = (uint8_t) (USART1_BAUDRATE>>8);
  UBRR1L = (uint8_t) (USART1_BAUDRATE);
  UCSR1C = (1<<UCSZ1) | (1<<UCSZ0);

  UCSR1B = ((1 << RXCIE) | (1 << RXEN) | (1 << TXEN));

}	

static bool uart_initialized = false;

static int8_t uart_mod_init() {
  HAS_CRITICAL_SECTION;
  if(uart_initialized == false) {
    ENTER_CRITICAL_SECTION();
    USART1_init();
    LEAVE_CRITICAL_SECTION();
    uart_initialized = true;
  }
  return SOS_OK;
} 

static void USART1_send(uint8_t data)
{
  while(!( UCSR1A & (1<<UDRE1)) );
  UDR1 = data;
}


SIGNAL(SIG_USART1_RECV) {
  if (uart_mod_bufcnt >= BUFSIZE) {
    /* The buffer is full, we should send the message immediately */
    ker_timer_start(KER_UART_PID, UART_TID, 0);
    //uart_mod_bufcnt = 0;
    sys_led(LED_RED_TOGGLE);
  } else {
    /* Add the received byte to the buffer and reset the timer */
    *(uart_mod_buf+uart_mod_bufcnt) = UDR1;
    uart_mod_bufcnt++;
    ker_timer_start(KER_UART_PID, UART_TID, UART_TIMER_INTERVAL);
  }
}


#ifndef _MODULE_
mod_header_ptr uart_mod_get_header()
{
    return sos_get_header_address(mod_header);
}
#endif
