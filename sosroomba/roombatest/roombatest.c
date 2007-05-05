/**
 * Module needs to include <module.h>
 */
#include <sys_module.h>
#include <module.h>
#define LED_DEBUG
#include <led_dbg.h>
#include <uart_system.h>

#include "roombatest.h"
#include "../roombacomm/roombacomm.h"

#define PAUSE_TIMER_INTERVAL	128L
#define PAUSE_TID               1
#define DIST                    5
#define ANGLE                   90

/**
 * Module can define its own state
 */
typedef struct {
    FUNC_HDR_BLOCK;

    uint8_t pid;
    uint8_t state;
    uint8_t stcnt;
} app_state_t;

static int8_t roombatest_msg_handler(void *start, Message *e);

/**
 * This is the only global variable one can have.
 */
static mod_header_t mod_header SOS_MODULE_HEADER = {
    .mod_id         = DFLT_APP_ID0,
    .state_size     = sizeof(app_state_t),
    .num_timers     = 1,
    .num_sub_func   = NUM_FUNC,
    .num_prov_func  = 0,
    .platform_type  = HW_TYPE /* or PLATFORM_ANY */,
    .processor_type = MCU_TYPE,
    .code_id        = ehtons(DFLT_APP_ID0),
    .module_handler = roombatest_msg_handler,
    .funct = { FUNC_USE_BLOCK
    }
};


static int8_t roombatest_msg_handler(void *state, Message *msg)
{
    app_state_t *s = (app_state_t*)state;

    /**
     * Switch to the correct message handler
     */
    switch (msg->type){

        case MSG_INIT:
            {
                LED_DBG(LED_GREEN_TOGGLE);
                s->pid = msg->did;
                s->state = 0;
                s->stcnt = 0;
                sys_timer_start(PAUSE_TID, PAUSE_TIMER_INTERVAL, TIMER_REPEAT);
                break;
            }

        case MSG_FINAL:
            {
                sys_timer_stop(PAUSE_TID);
                break;
            }


        case MSG_TIMER_TIMEOUT:
            {
                MsgParam* params = (MsgParam*)(msg->data);

                if (params->byte == PAUSE_TID)
                {
                    switch (s->state) {
                        case 0:
                            {
                                if (s->stcnt == 0) {
                                    ROOMBACOMM_START(s);
                                }
                                if (s->stcnt++ == 16) {
                                    s->state++;
                                    s->stcnt = 0;
                                }
                                break;
                            }
                        case 1:
                            {
                                if (s->stcnt == 0) {
                                    ROOMBACOMM_CONTROL(s);
                                }
                                if (s->stcnt++ == 16) {
                                    s->state++;
                                    s->stcnt = 0;
                                }
                                break;
                            }
                        case 2:
                            {
                                if (s->stcnt == 0) {
                                    ROOMBACOMM_PLAYNOTE(s, 72, 10);
                                }
                                if (s->stcnt == 6) {
                                    ROOMBACOMM_PLAYNOTE(s, 76, 10);
                                }
                                if (s->stcnt == 12) {
                                    ROOMBACOMM_PLAYNOTE(s, 79, 10);
                                }
                                // Stay in the state for 18 times that
                                // the timer fires (2.25 sec)
                                if (s->stcnt++ == 18) {
                                    s->state++;
                                    s->stcnt = 0;
                                }
                                break;
                            }
                        case 3:
                            {
                                uint8_t st;
                                if (s->stcnt == 0) {
                                    //ROOMBACOMM_SPINRIGHTFOR(s, ANGLE);
                                    //ROOMBACOMM_GOSTRAIGHTFOR(s, DIST*MILLIMETERSPERGRID);
                                }
                                if (s->stcnt++ == 40) {
                                    s->stcnt = 0;
                                }
                                ROOMBACOMM_BUMPSENSOR(s, &st);
                                if (st != 0) {
                                    LED_DBG(LED_YELLOW_ON);
                                } else {
                                    LED_DBG(LED_YELLOW_OFF);
                                }
                                break;
                            }
                        default:
                            s->state = 0;
                    }
                }
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
mod_header_ptr roombatest_get_header()
{
    return sos_get_header_address(mod_header);
}
#endif

