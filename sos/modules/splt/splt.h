#ifndef _SPLT_H_
#define _SPLT_H_

//! This can be made a compile time configuration
#define SPLT_BASE_STATION_ADDRESS 1

#define SPLT_MOD_PID            DFLT_APP_ID1
#define SPLT_TIMER_INTERVAL	1024L
#define SPLT_ONE_SHOT_TID       0
#define SPLT_REPEATING_TID      1

#define MSG_PM_DATA_READY (MOD_MSG_START + 0)
#define MSG_RF_DATA_READY (MOD_MSG_START + 1)

#define SPLT_TYPE_PMREADING 1
#define SPLT_TYPE_RFREADING 2

//! Splt Message Structure - Modified from the TOS Implementation
typedef struct SpltMsg {
  uint8_t type;
  uint16_t reading;
  uint16_t originaddr;
  uint32_t seq_no;
} __attribute__ ((packed)) SpltMsg;



#ifndef _MODULE_
extern mod_header_ptr splt_get_header();
#endif //_MODULE_

#endif
