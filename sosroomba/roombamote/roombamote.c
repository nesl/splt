/**
 * Module needs to include <module.h>
 */
#include <sys_module.h>
#include <module.h>
#define LED_DEBUG
#include <led_dbg.h>
#include <uart_system.h>

#include "roombamote.h"
#include "../roombacomm/roombacomm.h"

#define ROOMBAMOTE_TIMER_INTERVAL	128L
#define ROOMBAMOTE_TID               0
#define DIST                    500
#define ANGLE                   90
#define GRIDX                   12
#define GRIDY                   10
#define MSG_LOC_DATA        (MOD_MSG_START + 10)

//Sin and Cosine tables are for 45 degree angles
//Values have to been normalized to 128
static int8_t sin_t[8] = {   0,   90,  128,   90,    0,  -90, -128,  -90}; 
static int8_t cos_t[8] = { 128,   90,    0,  -90, -128,  -90,    0,   90}; 
static uint8_t left_bounce_map  = 0xC7; 
static uint8_t right_bounce_map = 0x7C; 
static uint8_t top_bounce_map   = 0xF1; 
static uint8_t bottom_bounce_map= 0x1F; 

enum {
    INIT = 0,
    SENT_START = 1,
    SENT_CONTROL = 2,
    WAITING_FOR_FIRSTLOC = 3,
    MOVING = 4,
    TURNING = 5,
    BACKING = 6
};

/**
 * Module can define its own state
 */
typedef struct {
    FUNC_HDR_BLOCK;

    uint8_t pid;
    uint8_t state;
    uint8_t stcnt;
    // Coordinates x and y are in grid coordinates, so we approximate the
    // Roomba's position as anywhere inside the grid to be the center of that
    // grid unit
    // The direction is quantized to be in multiples of 45 degrees, so that
    // the trig functions are simplified.
    uint16_t x, y, dir;
    uint16_t prevx, prevy;
    uint8_t cov[GRIDX][GRIDY];
    uint16_t targetX, targetY, targetDir;
    uint16_t turnDir;
    uint8_t hasTarget;
    uint16_t estdir;
} roombamote_state_t;

static int8_t roombamote_msg_handler(void *start, Message *e);
static int8_t roombamote_UpdateLocation(uint8_t *buf, uint8_t buf_len);
static int8_t roombamote_BoundsCheck(uint16_t *rdir);
static int8_t roombamote_BumpCheck(uint16_t *rdir);
static int8_t roombamote_RunAlgo(uint16_t *rdir);
static int8_t roombamote_Turn(uint16_t rdir);
static int8_t roombamote_Backup();

/**
 * This is the only global variable one can have.
 */
static mod_header_t mod_header SOS_MODULE_HEADER = {
    .mod_id         = DFLT_APP_ID0,
    .state_size     = sizeof(roombamote_state_t),
    .num_timers     = 1,
    .num_sub_func   = NUM_FUNC,
    .num_prov_func  = 0,
    .platform_type  = HW_TYPE /* or PLATFORM_ANY */,
    .processor_type = MCU_TYPE,
    .code_id        = ehtons(DFLT_APP_ID0),
    .module_handler = roombamote_msg_handler,
    .funct = { FUNC_USE_BLOCK
    }
};


static int8_t roombamote_msg_handler(void *state, Message *msg)
{
    roombamote_state_t *s = (roombamote_state_t*)state;

    /**
     * Switch to the correct message handler
     */
    switch (msg->type){

        case MSG_INIT:
            {
                int i, j;
                LED_DBG(LED_GREEN_TOGGLE);
                s->pid = msg->did;
                s->state = INIT;
                s->stcnt = 0;
#ifndef TESTING
                s->x = 6;
                s->y = 5;
#else
                s->x = 0;
                s->y = 0;
#endif
                s->prevx = 0;
                s->prevy = 0;
                s->dir = 0;
                s->estdir = 0;
                s->targetX = 0;
                s->targetY = 0;
                s->targetDir = 0;
                s->turnDir = 0;
                s->hasTarget = 0;
                for (i=0; i<GRIDX; i++) {
                    for (j=0; j<GRIDY; j++) {
                        s->cov[i][j] = 0;
                    }
                }

                sys_timer_start(ROOMBAMOTE_TID, ROOMBAMOTE_TIMER_INTERVAL, TIMER_REPEAT);
                break;
            }

        case MSG_FINAL:
            {
                sys_timer_stop(ROOMBAMOTE_TID);
                break;
            }

        case MSG_LOC_DATA:
            {
                uint8_t buf_len; 
                uint8_t *buf;

                buf_len = msg->len;
                buf = (uint8_t *)sys_msg_take_data(msg);
                /*
                   if (buf_len >= 8) {
                   LED_DBG(LED_YELLOW_ON);
                   }
                   if (msg->len >= 8) {
                   LED_DBG(LED_YELLOW_ON);
                   }
                   */
                if (buf != NULL) {
                    roombamote_UpdateLocation(buf, buf_len);
                    sys_free(buf);
                } else {
                    LED_DBG(LED_RED_TOGGLE);
                    return -EINVAL;
                }
                switch (s->state) {
                    case WAITING_FOR_FIRSTLOC:
                        {
                            // Start moving the Roomba in the direction it is
                            // in for a large distance. Hopefully, it will
                            // bump into something, or we'll get a new
                            // location update to process further
                            ROOMBACOMM_GOSTRAIGHTFOR(s, (GRIDX + GRIDY) * MILLIMETERSPERGRID);
                            s->state = MOVING;
                            break;
                        }
                }
                break;
            }

        case MSG_TIMER_TIMEOUT:
            {
                MsgParam* params = (MsgParam*)(msg->data);

                if (params->byte == ROOMBAMOTE_TID)
                {
                    switch (s->state) {
                        case INIT:
                            {
                                if (s->stcnt == 0) {
                                    ROOMBACOMM_START(s);
                                }
                                // Stay in the state for 16 times that
                                // the timer fires (2 sec)
                                if (s->stcnt++ == 16) {
                                    s->state = SENT_START;
                                    s->stcnt = 0;
                                }
                                break;
                            }
                        case SENT_START:
                            {
                                if (s->stcnt == 0) {
                                    ROOMBACOMM_CONTROL(s);
                                }
                                // Stay in the state for 16 times that
                                // the timer fires (2 sec)
                                if (s->stcnt++ == 16) {
                                    s->state = SENT_CONTROL;
                                    s->stcnt = 0;
                                }
                                break;
                            }
                        case SENT_CONTROL:
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
                                    s->state = WAITING_FOR_FIRSTLOC;
                                    s->stcnt = 0;
                                }
                                break;
                            }
                        case WAITING_FOR_FIRSTLOC:
                            {
#ifndef TESTING_WITHOUT_HOST
                                // Stay in the state for 16 times that
                                // the timer fires (2 sec)
                                if (s->stcnt++ == 16) {
                                    ROOMBACOMM_GOSTRAIGHTFOR(s, (GRIDX + GRIDY) * MILLIMETERSPERGRID);
                                    s->state = MOVING;
                                    s->stcnt = 0;
                                }
#else
                                s->state = WAITING_FOR_FIRSTLOC;
#endif
                                break;
                            }
                        case MOVING:
                            {
                                uint16_t rdir;
                                rdir = s->dir;
                                    
            LED_DBG(LED_RED_TOGGLE);
                                roombamote_RunAlgo(&rdir);
            LED_DBG(LED_RED_TOGGLE);
                                s->stcnt++;
                                s->x = s->stcnt % 14;
                                s->y = s->stcnt / 12;
                                if (s->stcnt == 168) {
                                    LED_DBG(LED_GREEN_TOGGLE);
                                    s->stcnt = 0;
                                }
                                /*

                                //Check for bumps and turn 
                                roombamote_BumpCheck(&rdir);
                                if (rdir != s->dir ) {
                                    s->turnDir = rdir;
                                    roombamote_Turn(s->turnDir);
                                    s->state = TURNING;
                                    s->stcnt = 0;
                                    break;
                                }
                                if (s->x != s->prevx || s->y != s->prevy) {
                                    roombamote_RunAlgo(&rdir);
                                    //Update the prev values so that we don't
                                    //process it again
                                    s->prevx = s->x;
                                    s->prevy = s->y;
                                }*/
                                break;
                            }
                        case BACKING:
                            {
                                uint8_t rst;
                                ROOMBACOMM_MOVING(s, &rst);
                                if (rst == ROOMBA_STOPPED) {
                                    ROOMBACOMM_CHANGEVELOCITY(s, DEFAULTVELOCITY);
                                    roombamote_Turn(s->turnDir);
                                    s->stcnt = 0;
                                    s->state = TURNING;
                                } else if (s->stcnt++ == 32) {
                                    s->stcnt = 0;
                                    ROOMBACOMM_STOP(s);
                                }
                                break;
                            }
                        case TURNING:
                            {
                                uint8_t rst;
                                ROOMBACOMM_MOVING(s, &rst);
                                // If the Roomba has finished turning 
                                // set it going 
                                if (rst == ROOMBA_STOPPED) {
                                    ROOMBACOMM_GOSTRAIGHTFOR(s, (GRIDX + GRIDY) * MILLIMETERSPERGRID);
                                    s->state = MOVING;
                                    s->stcnt = 0;
                                } else if (s->stcnt++ == 32) {
                                    s->stcnt = 0;
                                    // If it has been turning too long (4 sec), then
                                    // stop it. But don't change state so that it
                                    // stays in the turning state and the next
                                    // time it comes in, it triggers the above
                                    // block
                                    ROOMBACOMM_STOP(s);
                                }
                                break;
                            }

                        default:
                            {
                                s->state = INIT;
                                LED_DBG(LED_RED_TOGGLE);
                            }
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

static int8_t roombamote_UpdateLocation(uint8_t *buf, uint8_t buf_len)
{
    int i;
    int16_t xl, yl, dirl;
    int foundmyloc;
    uint16_t *mbuf;
    uint8_t mbuf_len;
    roombamote_state_t *rs = (roombamote_state_t *)sys_get_state();

    foundmyloc = 0;
    mbuf_len = buf_len /2;
    mbuf = (uint16_t *)buf;

    for(i=0; i<mbuf_len/4; i++) {
        //addr = mbuf[i*4];
        xl = mbuf[i*4+1]; 
        yl = mbuf[i*4+2]; 
        dirl = mbuf[i*4+3]; 
        //Increment the clean value 
        rs->cov[xl][yl]++;

        //Node addresses are 56 and 57 for the two Roombas
        //if (addr != node_address) {
        //    continue;
        //}
        if (xl != rs->x || yl != rs->y) {
            rs->prevx = rs->x;
            rs->prevy = rs->y;
            rs->x = xl;
            rs->y = yl;

            // If the direction was invalid just set to some random direction
            //if (dirl > 7) { dirl = ker_rand() >> 13; }
            //if (dirl > 7) { dirl = rs->estdir; }
            rs->dir = dirl;
        } else {
            //If the position hasn't changed since the last update, assume
            //that the host cannot tell in which direction we are in, and thus
            //we used our estimated direction
            rs->dir = rs->estdir;
        }
        foundmyloc = 1;
    }
    if (!foundmyloc) {
        //HOST sent a location but didn't tell me where I am :-(
#ifdef TESTING
        LED_DBG(LED_RED_TOGGLE);
#endif
        return -EINVAL;
    }
    return SOS_OK;
}

static int8_t roombamote_BoundsCheck(uint16_t *rdir)
{
    uint16_t r;
    uint8_t map;
    uint8_t need_to_turn;
    uint8_t whcnt;
    roombamote_state_t *rs = (roombamote_state_t *)sys_get_state();

    map = 0xFF;
    need_to_turn = 0;
    whcnt = 0;

    //Bounds check: If the x value is 0 or GRIDX or y value is 0 or GRIDY,
    //we have hit an edge
    if (rs->x == 0) {
        //We hit the left edge
        map &= left_bounce_map;
        need_to_turn = 1;
    } else if (rs->x >= GRIDX+1) {
        //We hit the right edge
        map &= right_bounce_map;
        need_to_turn = 1;
    }
    if (rs->y == 0) {
        //We hit the top edge
        map &= top_bounce_map;
        need_to_turn = 1;
    } else if (rs->y >= GRIDY+1) {
        //We hit the bottom edge
        map &= bottom_bounce_map;
        need_to_turn = 1;
    }
    //Turn around randomly once every 64 moves or so
    if ((ker_rand() >> 10) == 0) {
        need_to_turn = 1;
    }
    if (need_to_turn) {
        //Now pick a turn value which happens to be valid in the turn map just
        //computed
        r = ker_rand() >> 13; //r is between 0 and 7
        while (!(map & (0x01 << r)) && whcnt++ < 16) { //Try to find a good angle 16 times, and then exit since we don't want to be stuck in this loop forever
            r = ker_rand() >> 13; //r is between 0 and 7
        }
        if (whcnt >= 16) {
#ifdef TESTING
            LED_DBG(LED_RED_TOGGLE);
#endif
            *rdir = rs->dir;
            return -EINVAL;
        }
        *rdir = r;
    }
    return SOS_OK;
}

static int8_t roombamote_RunAlgo(uint16_t *rdir)
{
    uint16_t leastClean;
    uint16_t longestDist;
    uint16_t bestDir;
    uint16_t tgtX, tgtY, tgtDir;
    int16_t xp, yp;
    uint16_t rad;
    uint16_t cleanliness = 0;
    roombamote_state_t *rs = (roombamote_state_t *)sys_get_state();
                                
    leastClean = 65535;
    longestDist = 1;
    bestDir = 0;

    if (rs->hasTarget == 1) {
        //Check to see whether it reached its target block
        //Or whether it has turned due to an osbtacle
        //or just some randomness
        if ((rs->x == rs->targetX && rs->y == rs->targetY) || *rdir != rs->targetDir) {
            rs->hasTarget = 0;
        } else {
            //The Roomba is still on its way to the least clean block so let
            //it continue on its path without disturbing it
            *rdir = rs->dir;
            return SOS_OK;
        }
    }
    //if (rs->cov[rs->x][rs->y] > 1) {
    if (1) {
        uint16_t deg;
        tgtX = rs->x;
        tgtY = rs->y;
        tgtDir = rs->dir;
        // A good idea would be to get the bounce map from the bounds check so
        // that we don't have iterate over invalid directions
        for (deg = 0; deg < 8; deg++) {
            cleanliness = 0;
            rad = 1;
            while (1) {
                // calculate everything with a higher precision in the hope
                // that the trig functions still work reasonably well
                xp = ((int16_t)(rs->x << 7) + rad*cos_t[deg])>>7;
                yp = ((int16_t)(rs->y << 7) - rad*sin_t[deg])>>7;
                //Bounds check
                if (xp <= 0 || xp > GRIDX || yp <= 0 || yp > GRIDY) {
                    break;
                }
                cleanliness += rs->cov[xp][yp];
                rad++;
            }
            if (((uint32_t)cleanliness*longestDist < (uint32_t)leastClean*rad) && rad > 1) {
                bestDir = deg;
                longestDist = rad;
                leastClean = cleanliness;
                //Update these values so we can use them later
                tgtX = (xp <= 0 ? 1 : xp > GRIDX ? GRIDX : xp);
                tgtY = (yp <= 0 ? 1 : yp > GRIDY ? GRIDY : yp);
                tgtDir = deg;
            }
        }
        if (leastClean != 65535) {
            rs->targetX = tgtX;
            rs->targetY = tgtY;
            rs->targetDir = tgtDir;
            *rdir = tgtDir;
            //rs->hasTarget = 1;
        } else {
#ifdef TESTING
            LED_DBG(LED_RED_TOGGLE);
#endif
            *rdir = rs->dir;
            return -EINVAL;
        }
    }
                                
    return SOS_OK;
}

static int8_t roombamote_Turn(uint16_t rdir)
{
    roombamote_state_t *rs = (roombamote_state_t *)sys_get_state();

    if (rdir != rs->dir) {
        //Compute the right amount of turn and direction
        int16_t turn = (8 + rdir - rs->dir)%8;
        if (turn > 4) {
            turn = 8 - turn;
            ROOMBACOMM_SPINRIGHTFOR(rs, 45*turn);
        } else {
            ROOMBACOMM_SPINLEFTFOR(rs, 45*turn);
        }
        rs->state = TURNING;
        rs->estdir = rdir;
    }
    return SOS_OK;
}

static int8_t roombamote_Backup()
{
    roombamote_state_t *rs = (roombamote_state_t *)sys_get_state();

    ROOMBACOMM_CHANGEVELOCITY(rs, -DEFAULTVELOCITY);
    ROOMBACOMM_GOSTRAIGHTFOR(rs, 3*STRAIGHTSEGMENT);
}

static int8_t roombamote_BumpCheck(uint16_t *rdir) 
{
    uint8_t st;
    roombamote_state_t *rs = (roombamote_state_t *)sys_get_state();

    ROOMBACOMM_BUMPSENSOR(rs, &st);
    if (st > 0) {
        //LED_DBG(LED_YELLOW_ON);
        // If we bump into something, turn around 135 degrees
        // If we bump right, we turn left and vice versa
        if (st & BUMPLEFT_MASK) {
            *rdir = (rs->dir + 5)%8; 
        } else {
            *rdir = (rs->dir + 3)%8; 
        }
    } else {
        //LED_DBG(LED_YELLOW_OFF);
    }
    return SOS_OK;
}

#ifndef _MODULE_
mod_header_ptr roombamote_get_header()
{
    return sos_get_header_address(mod_header);
}
#endif


