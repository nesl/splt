#include <module.h>
#include <stdlib.h>
#include <sys_module.h>
#define LED_DEBUG
#include <led_dbg.h>
#include <uart_system.h>

#include "roombacomm.h"

// Internal functions
static int8_t roombacomm_MsgHandler(void *state, Message *msg);
static int8_t roombacomm_Init(uint8_t mpid);
static int8_t roombacomm_Drive(int16_t velocity, int16_t radius);
static int8_t roombacomm_Send(uint8_t *buf, uint8_t buf_len);
static void   roombacomm_ResetBuf();
static int8_t roombacomm_GoStraightSegment(char *p);
static int8_t roombacomm_Pause(uint16_t ptime, int8_t (*funcptr)(char *));
static uint16_t roombacomm_GetPauseTime(int16_t angle);
static int8_t roombacomm_ReadSensors();
static int8_t roombacomm_ProcessSensorsPkt(uint8_t *buf, uint8_t buf_len);
static int8_t roombacomm_Read(uint8_t buf_len);
// Exported functions
static int8_t roombacomm_Start(char *p);
static int8_t roombacomm_Control(char *p);
static int8_t roombacomm_Stop(char *p);
static int8_t roombacomm_Reset(char *p);
static int8_t roombacomm_GoStraightAt(char *p, int16_t velocity);
static int8_t roombacomm_GoStraightFor(char *p, int16_t distance);
static int8_t roombacomm_SpinLeftAt(char *p, int16_t velocity);
static int8_t roombacomm_SpinLeftFor(char *p, int16_t angle);
static int8_t roombacomm_SpinRightAt(char *p, int16_t velocity);
static int8_t roombacomm_SpinRightFor(char *p, int16_t angle);
static int8_t roombacomm_Moving(char *p, uint8_t *state);
static int8_t roombacomm_PlayNote(char *p, uint8_t note, uint8_t dur);
static int8_t roombacomm_BumpSensor(char *p, uint8_t *state);
static int8_t roombacomm_ChangeVelocity(char *p, int16_t vel);

static mod_header_t mod_header SOS_MODULE_HEADER = {
    .mod_id         = ROOMBACOMM_PID,
    .state_size     = sizeof(roombacomm_state_t),
    .num_timers     = 3,
    .num_sub_func   = 0,
    .num_prov_func  = NUM_FUNC,
    .platform_type  = HW_TYPE /* or PLATFORM_ANY */,
    .processor_type = MCU_TYPE,
    .code_id        = ehtons(ROOMBACOMM_PID),
    .module_handler = roombacomm_MsgHandler,
    .funct = { FUNC_PRV_BLOCK
    },
};


static int8_t roombacomm_MsgHandler(void *state, Message *msg)
{
    /**
     * The module is passed in a void* that contains its state.  For easy
     * reference it is handy to typecast this variable to be of the
     * applications state type.  Note that since we are running as a module,
     * this state is not accessible in the form of a global or static
     * variable.
     */
    roombacomm_state_t *s = (roombacomm_state_t*)state;

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
                if (roombacomm_Init(msg->did) != SOS_OK) {
                    LED_DBG(LED_RED_TOGGLE);
                }
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
                MsgParam* params = (MsgParam*)(msg->data);

                switch (params->byte) {
                    case ROOMBACOMM_PAUSE_TID:
                        {
                            (*s->pauseTimeoutFunc)(NULL);
                            break;
                        }
                    case ROOMBACOMM_UART_BACKOFF_TID:
                        {
                            roombacomm_Send(NULL, 0);
                            break;
                        }
                    case ROOMBACOMM_SENSORS_TID:
                        {
                            roombacomm_ReadSensors();
                            break;
                        }
                }
                break;
            }

        case MSG_UART_SEND_DONE:
            {
                roombacomm_ResetBuf();
                break;
            }

        case MSG_UART_READ_DONE:
            {
                uint8_t buf_len;
                uint8_t *buf;
                buf_len = msg->len;
                buf = (uint8_t *)sys_msg_take_data(msg);

		//LED_DBG(LED_GREEN_TOGGLE);
  
                roombacomm_ProcessSensorsPkt(buf, buf_len);
                sys_free(buf);
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

static int8_t roombacomm_Init(uint8_t mpid)
{
    int8_t res;
    int i;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    for (i = 0; i<UART_BUF_SIZE; i++) {
        rs->uart_buf[i] = 0x0;
    }
    rs->uart_buf_len = 0;
    for (i = 0; i<UART_BUF_SIZE; i++) {
        rs->uart_bkp_buf[i] = 0x0;
    }
    rs->uart_bkp_buf_len = 0;
    rs->pid = mpid;
    rs->velocity = DEFAULTVELOCITY;
    rs->moving = ROOMBA_STOPPED;

    if ((res = ker_uart_reserve_bus(ROOMBACOMM_PID, UART_SYS_TX_FLAG)) != SOS_OK) {
        LED_DBG(LED_RED_TOGGLE);
        return res;
    }
    if ((res = ker_uart_reserve_bus(ROOMBACOMM_PID, 0)) != SOS_OK) {
        LED_DBG(LED_RED_TOGGLE);
        return res;
    }
    //res = sys_timer_start(ROOMBACOMM_SENSORS_TID, ROOMBACOMM_SENSORS_INTERVAL, TIMER_REPEAT);

    return res;
}

static int8_t roombacomm_Start(char *p)
{
    int8_t res;
    uint8_t uart_buf[1];

    uart_buf[0] = START;
    res = roombacomm_Send(uart_buf, 1);

    return res;
}

static int8_t roombacomm_Control(char *p)
{
    int8_t res;
    uint8_t uart_buf[2];

    uart_buf[0] = CONTROL;
    //uart_buf[1] = FULL_CONTROL;
    //res = roombacomm_Send(uart_buf, 2);
    res = roombacomm_Send(uart_buf, 1);

    res = sys_timer_start(ROOMBACOMM_SENSORS_TID, ROOMBACOMM_SENSORS_INTERVAL, TIMER_REPEAT);
    return res;
}

static int8_t roombacomm_Stop(char *p)
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    rs->straightDistance = 0;
    res = roombacomm_Drive(0, 0);

    return res;
}

static int8_t roombacomm_Reset(char *p)
{
    int8_t res;

    if ((res = roombacomm_Stop(NULL)) != SOS_OK) {
        return res;
    }
    if ((res = roombacomm_Start(NULL)) != SOS_OK) {
        return res;
    }
    res = roombacomm_Control(NULL);

    return res;
}

static int8_t roombacomm_Drive(int16_t velocity, int16_t radius)
{
    int8_t res;
    uint8_t uart_buf[5];
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    uart_buf[0] = DRIVE;
    uart_buf[1] = velocity >> 8;
    uart_buf[2] = velocity & 0x00ff;
    uart_buf[3] = radius >> 8;
    uart_buf[4] = radius & 0x00ff;

    if (velocity == 0) {
        rs->moving = ROOMBA_STOPPED;
    } else if (radius == 1 || radius == -1) {
        rs->moving = ROOMBA_SPINNING;
    } else {
        rs->moving = ROOMBA_DRIVING;
    } 

    res = roombacomm_Send(uart_buf, 5);
    return res;
}

static int8_t roombacomm_Send(uint8_t *buf, uint8_t buf_len)
{
    int8_t res;
    uint8_t i;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();
#ifndef BKP
    if (rs->uart_buf_len != 0) {
        //If the main UART buf is occupied then try the backup buffer
        if (rs->uart_bkp_buf_len != 0) {
            LED_DBG(LED_RED_TOGGLE);
            return -EBUSY;
        }
        for (i=0; i<buf_len; i++) {
            rs->uart_bkp_buf[i] = buf[i];
        }
        rs->uart_bkp_buf_len = buf_len;
        res = sys_timer_start(ROOMBACOMM_UART_BACKOFF_TID, ROOMBACOMM_UART_BACKOFF_INTERVAL, TIMER_ONE_SHOT);
        return res;
    }
    if (rs->uart_bkp_buf_len != 0) {
        //There's something to send in the backup buffer
        for (i=0; i<rs->uart_bkp_buf_len; i++) {
            rs->uart_buf[i] = rs->uart_bkp_buf[i];
        }
        rs->uart_buf_len = rs->uart_bkp_buf_len;
        rs->uart_bkp_buf_len = 0;
    } else {
        for (i=0; i<buf_len; i++) {
            rs->uart_buf[i] = buf[i];
        }
        rs->uart_buf_len = buf_len;
    }
#else
    if (rs->uart_buf_len != 0) {
        LED_DBG(LED_RED_TOGGLE);
        return -EBUSY;
    }
    for (i=0; i<buf_len; i++) {
        rs->uart_buf[i] = buf[i];
    }
    rs->uart_buf_len = buf_len;
#endif
    /*
    if ((res = ker_uart_reserve_bus(ROOMBACOMM_PID, UART_SYS_TX_FLAG)) != SOS_OK) {
        LED_DBG(LED_RED_TOGGLE);
        return res;
    }*/

    res = ker_uart_send_data(rs->uart_buf, rs->uart_buf_len, ROOMBACOMM_PID);

    return res;
}

/* This function should be called when the MSG_UART_SEND_DONE message is received
 * by the module from the UART driver
 */
static void roombacomm_ResetBuf()
{
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    //All we do is set the length as zero, since we check that for a busy UART
    rs->uart_buf_len = 0;
}

static int8_t roombacomm_GoStraightAt(char *p, int16_t velocity) 
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    rs->velocity = velocity > 500 ? 500: velocity;
    rs->velocity = velocity < -500 ? -500: velocity;
    res = roombacomm_Drive(rs->velocity, 0x8000);

    return res;
}

static int8_t roombacomm_GoStraightFor(char *p, int16_t distance)
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    rs->straightDistance = distance;
    res = roombacomm_GoStraightSegment(NULL);

    return res;
} 

static int8_t roombacomm_GoStraightSegment(char *p) 
{
    int8_t res;
    uint16_t pausetime;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if ((res = roombacomm_Drive(rs->velocity, 0x8000 - rs->straightRadius)) != SOS_OK) {
        return res;
    }
    rs->straightRadius = 1 - rs->straightRadius;

    if (rs->straightDistance > STRAIGHTSEGMENT) {
        pausetime = abs(STRAIGHTSEGMENT * 1024 / rs->velocity);
        rs->straightDistance -= STRAIGHTSEGMENT;
        res = roombacomm_Pause(pausetime, roombacomm_GoStraightSegment);
    } else {
        pausetime = abs(rs->straightDistance * 1024 / rs->velocity);
        rs->straightDistance = 0;
        res = roombacomm_Pause(pausetime, roombacomm_Stop);
    }

    return res;
}

static int8_t roombacomm_SpinRightAt(char *p, int16_t velocity) 
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    rs->velocity = velocity > 500 ? 500: velocity;
    rs->velocity = velocity < -500 ? -500: velocity;
    res = roombacomm_Drive(rs->velocity, -1);

    return res;
}

static int8_t roombacomm_SpinLeftAt(char *p, int16_t velocity) 
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    rs->velocity = velocity > 500 ? 500: velocity;
    rs->velocity = velocity < -500 ? -500: velocity;
    res = roombacomm_Drive(rs->velocity, 1);

    return res;
}

static int8_t roombacomm_SpinLeftFor(char *p, int16_t angle) 
{
    int8_t res;
    uint16_t pausetime;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if ((res = roombacomm_SpinLeftAt(NULL, rs->velocity)) != SOS_OK) {
        return res;
    }
    // If the angle is negative, it would mean the spinning should last
    // forever
    if (angle<0) {
        return res;
    }

    //pausetime = ((int32_t)angle * (MILLIMETERSPERDEGREE * 1024 / rs->velocity))>> 3;
    pausetime = roombacomm_GetPauseTime(angle);
    res = roombacomm_Pause(pausetime, roombacomm_Stop);

    return res;
}

static int8_t roombacomm_SpinRightFor(char *p, int16_t angle) 
{
    int8_t res;
    uint16_t pausetime;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if ((res = roombacomm_SpinRightAt(NULL, rs->velocity)) != SOS_OK) {
        return res;
    }
    // If the angle is negative, it would mean the spinning should last
    // forever
    if (angle<0) {
        return res;
    }

    //pausetime =  * (MILLIMETERSPERDEGREE * 1024 / rs->velocity))>> 3;
    pausetime = roombacomm_GetPauseTime(angle);
    //pausetime = 820;
    res = roombacomm_Pause(pausetime, roombacomm_Stop);

    return res;
}

static int8_t roombacomm_Pause(uint16_t ptime, int8_t (*funcptr)(char *))
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    rs->pauseTimeoutFunc = funcptr; 
    res = sys_timer_start(ROOMBACOMM_PAUSE_TID, ptime, TIMER_ONE_SHOT);

    return res;
}

static uint16_t roombacomm_GetPauseTime(int16_t angle)
{
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if (angle%45 == 0) {
        return (uint16_t)(450*(int32_t)angle/45);
    } else {
        return (uint16_t)(abs((int32_t)angle * (MILLIMETERSPERDEGREE * 1024 / rs->velocity))>> 3);
    }

}

static int8_t roombacomm_Moving(char *p, uint8_t *state)
{
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    *(state) = rs->moving;

    return SOS_OK;
}

static int8_t roombacomm_PlayNote(char *p, uint8_t note, uint8_t dur)
{
    int8_t res;
    uint8_t uart_buf[7];

    uart_buf[0] = SONG;
    uart_buf[1] = 15;
    uart_buf[2] = 1;
    uart_buf[3] = note;
    uart_buf[4] = dur;
    uart_buf[5] = PLAY;
    uart_buf[6] = 15;

    res = roombacomm_Send(uart_buf, 7);
    return res;
}

static int8_t roombacomm_ReadSensors()
{
    int8_t res;
    uint8_t uart_buf[2];
    //roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if ((res = roombacomm_Read(SENSORS_PKT_SIZE)) != SOS_OK) {
        return res;
    }

    uart_buf[0] = SENSORS;
    uart_buf[1] = SENSORS_ALL;

    res = roombacomm_Send(uart_buf, 2);

    return res;
}

static int8_t roombacomm_Read(uint8_t buf_len)
{
    int8_t res;
    //roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    /*
    if ((res = ker_uart_reserve_bus(ROOMBACOMM_PID, 0)) != SOS_OK) {
        LED_DBG(LED_RED_TOGGLE);
        return res;
    }
*/

    res = ker_uart_read_data(buf_len, ROOMBACOMM_PID);

    return res;


}

static int8_t roombacomm_ProcessSensorsPkt(uint8_t *buf, uint8_t buf_len)
{
    uint8_t i;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if (buf_len != SENSORS_PKT_SIZE) {
        LED_DBG(LED_RED_TOGGLE);
        return -EINVAL;
    }

    for (i=0; i<buf_len; i++) {
        rs->sensor_bytes[i] = buf[i];
    }

    return SOS_OK;
}

static int8_t roombacomm_BumpSensor(char *p, uint8_t *state)
{
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    *(state) = (rs->sensor_bytes[BUMPSWHEELDROPS] & BUMP_MASK);

    return SOS_OK;
};

static int8_t roombacomm_ChangeVelocity(char *p, int16_t vel)
{
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    rs->velocity = vel;
    
    return SOS_OK;
}

#ifndef _MODULE_
mod_header_ptr roombacomm_get_header()
{
    return sos_get_header_address(mod_header);
}
#endif


