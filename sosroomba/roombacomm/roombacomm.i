# 1 "roombacomm.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "roombacomm.c"
# 1 "../../../kernel/include/module.h" 1
# 58 "../../../kernel/include/module.h"
# 1 "../../../kernel/include/sos_info.h" 1
# 10 "../../../kernel/include/sos_info.h"
# 1 "../../../kernel/include/sos_types.h" 1
# 47 "../../../kernel/include/sos_types.h"
# 1 "../../../platform/micaz/include/hardware_types.h" 1





# 1 "../../../processor/avr/include/hardware_proc.h" 1






# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/io.h" 1 3
# 86 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/io.h" 3
# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/sfr_defs.h" 1 3
# 123 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/sfr_defs.h" 3
# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/inttypes.h" 1 3
# 36 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/inttypes.h" 3
# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdint.h" 1 3
# 65 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdint.h" 3
typedef signed char int8_t;




typedef unsigned char uint8_t;
# 104 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdint.h" 3
typedef int int16_t;




typedef unsigned int uint16_t;
# 120 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdint.h" 3
typedef long int32_t;




typedef unsigned long uint32_t;
# 136 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdint.h" 3
typedef long long int64_t;




typedef unsigned long long uint64_t;
# 155 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdint.h" 3
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 37 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/inttypes.h" 2 3
# 124 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/sfr_defs.h" 2 3
# 87 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/io.h" 2 3
# 168 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/io.h" 3
# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/iom128.h" 1 3
# 169 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/io.h" 2 3
# 256 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/io.h" 3
# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/portpins.h" 1 3
# 257 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/io.h" 2 3
# 8 "../../../processor/avr/include/hardware_proc.h" 2
# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/interrupt.h" 1 3
# 134 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/interrupt.h" 3
static __inline__ void timer_enable_int (unsigned char ints)
{

    (*(volatile uint8_t *)((0x37) + 0x20)) = ints;

}
# 9 "../../../processor/avr/include/hardware_proc.h" 2


# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/signal.h" 1 3
# 12 "../../../processor/avr/include/hardware_proc.h" 2

# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/pgmspace.h" 1 3
# 69 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/pgmspace.h" 3
# 1 "/usr/lib/gcc/avr/3.4.3/include/stddef.h" 1 3 4
# 213 "/usr/lib/gcc/avr/3.4.3/include/stddef.h" 3 4
typedef unsigned int size_t;
# 70 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/pgmspace.h" 2 3
# 90 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/pgmspace.h" 3
typedef void prog_void __attribute__((__progmem__));
typedef char prog_char __attribute__((__progmem__));
typedef unsigned char prog_uchar __attribute__((__progmem__));

typedef int8_t prog_int8_t __attribute__((__progmem__));
typedef uint8_t prog_uint8_t __attribute__((__progmem__));
typedef int16_t prog_int16_t __attribute__((__progmem__));
typedef uint16_t prog_uint16_t __attribute__((__progmem__));

typedef int32_t prog_int32_t __attribute__((__progmem__));
typedef uint32_t prog_uint32_t __attribute__((__progmem__));


typedef int64_t prog_int64_t __attribute__((__progmem__));
typedef uint64_t prog_uint64_t __attribute__((__progmem__));
# 490 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/pgmspace.h" 3
extern void *memcpy_P(void *, const prog_void *, size_t);
extern char *strcat_P(char *, const prog_char *);
extern int strcmp_P(const char *, const prog_char *) __attribute__((__pure__));
extern char *strcpy_P(char *, const prog_char *);
extern int strcasecmp_P(const char *, const prog_char *) __attribute__((__pure__));
extern size_t strlcat_P (char *, const prog_char *, size_t );
extern size_t strlcpy_P (char *, const prog_char *, size_t );
extern size_t strlen_P(const prog_char *) __attribute__((__const__));
extern size_t strnlen_P(const prog_char *, size_t) __attribute__((__const__));
extern int strncmp_P(const char *, const prog_char *, size_t) __attribute__((__pure__));
extern int strncasecmp_P(const char *, const prog_char *, size_t) __attribute__((__pure__));
extern char *strncat_P(char *, const prog_char *, size_t);
extern char *strncpy_P(char *, const prog_char *, size_t);
# 14 "../../../processor/avr/include/hardware_proc.h" 2
# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/avr/wdt.h" 1 3
# 15 "../../../processor/avr/include/hardware_proc.h" 2

# 1 "../../../kernel/include/sos_inttypes.h" 1
# 17 "../../../processor/avr/include/hardware_proc.h" 2
# 110 "../../../processor/avr/include/hardware_proc.h"
typedef uint16_t mod_header_ptr;

typedef uint16_t func_cb_ptr;
# 7 "../../../platform/micaz/include/hardware_types.h" 2
# 48 "../../../kernel/include/sos_types.h" 2

# 1 "../../../kernel/include/sos_endian.h" 1
# 50 "../../../kernel/include/sos_types.h" 2
# 60 "../../../kernel/include/sos_types.h"
enum
{
  FALSE = 0,
  TRUE = 1,
};



typedef enum { false=0, true=1, } bool;
# 83 "../../../kernel/include/sos_types.h"
enum
  {
 SOS_SPLIT = 1,
 SOS_OK = 0,
  };




enum
{
  SOS_BOOT_NORMAL = 0,
  SOS_BOOT_CRASHED = 1,
  SOS_BOOT_WDOGED = 3,
};
# 11 "../../../kernel/include/sos_info.h" 2


# 1 "../../../drivers/uart/include/uart.h" 1
# 14 "../../../kernel/include/sos_info.h" 2







enum {
 UNKNOWN = 0,
 MICA2 = 1,
 MICAZ = 2,
 XYZ = 3,
 CRICKET = 4,
 PROTOSB = 5,
 TMOTE = 6,
 CYCLOPS = 7,
 GW = 200,
 SIM = 201,
 PLATFORM_ANY = 255,
};


enum {
 MCU_UNKNOWN = 0,
 MCU_AVR = 1,
 MCU_ARM7 = 2,
 MCU_MSP430 = 3,
};
# 101 "../../../kernel/include/sos_info.h"
enum { UNIT_CENTIMETERS=0, UNIT_METERS=1, UNIT_KILOMETERS=2, UNIT_INCHES=3, UNIT_FEET=4, UNIT_MILES=5 };

enum { NORTH, SOUTH, WEST, EAST };

typedef struct gps_xy_type{
    int8_t dir;
    int8_t deg;
    int8_t min;
    int8_t sec;
} __attribute__ ((packed))
gps_xy_t;


typedef struct gps_type {
    gps_xy_t x;
    gps_xy_t y;
    int16_t unit;
    int16_t z;
} __attribute__ ((packed))
gps_t;


typedef struct node_loc_type {
 int16_t unit;
 int16_t x;
 int16_t y;
 int16_t z;
} __attribute__ ((packed))
node_loc_t;
# 59 "../../../kernel/include/module.h" 2

# 1 "../../../kernel/include/sos_module_types.h" 1



# 1 "../../../kernel/include/fntable_types.h" 1
# 34 "../../../kernel/include/fntable_types.h"
typedef struct func_cb {
 void *ptr;
 uint8_t proto[4];
 uint8_t pid;
 uint8_t fid;
} func_cb_t;
# 60 "../../../kernel/include/fntable_types.h"
typedef void (*dummy_func)(void);
typedef int8_t (*fn_ptr_t)(void);

dummy_func ker_sys_enter_func( func_cb_ptr p );
void ker_sys_leave_func( void );
# 5 "../../../kernel/include/sos_module_types.h" 2
# 1 "../../../kernel/include/message_types.h" 1
# 45 "../../../kernel/include/message_types.h"
# 1 "../../../kernel/include/pid.h" 1
# 12 "../../../kernel/include/pid.h"
typedef uint8_t sos_pid_t;
# 22 "../../../kernel/include/pid.h"
enum {
          RSVD_0_PID=0,
          RSVD_1_PID,
          KER_SCHED_PID,
          KER_MEM_PID,
          TIMER_PID,
          ADC_PID,
          KER_SENSOR_PID,
          USER_PID,
          RSVD_2_PID,
          RADIO_PID,
          MONITOR_PID,
          MSG_QUEUE_PID,
          FNTABLE_PID,
          SOSBASE_PID,
          KER_TS_PID,
          KER_CODEMEM_PID,
          KER_FETCHER_PID,
          KER_DFT_LOADER_PID,
             KER_SPAWNER_PID,
             KER_CAM_PID,
           NULL_PID = 255,
};




enum {
 KER_MOD_MAX_PID = 63,
 DEV_MOD_MIN_PID = 64,
 APP_MOD_MIN_PID = 128,
 APP_MOD_MAX_PID = 223,
 SOS_MAX_PID = 254,
};






# 1 "../../../modules/include/mod_pid.h" 1
# 17 "../../../modules/include/mod_pid.h"
enum {

  DFLT_APP_ID0 = (APP_MOD_MIN_PID + 0),


  DFLT_APP_ID1 = (APP_MOD_MIN_PID + 1),


  DFLT_APP_ID2 = (APP_MOD_MIN_PID + 2),


  DFLT_APP_ID3 = (APP_MOD_MIN_PID + 3),


  MAG_SENSOR_PID = (APP_MOD_MIN_PID + 4),


  NBHOOD_PID = (APP_MOD_MIN_PID + 5),


  TD_PROTO_PID = (APP_MOD_MIN_PID + 6),


  TD_ENGINE_PID = (APP_MOD_MIN_PID + 7),


  MOD_D_PC_PID = (APP_MOD_MIN_PID + 8),


  MOD_FN_S_PID = (APP_MOD_MIN_PID + 9),


  MOD_FN_C_PID = (APP_MOD_MIN_PID + 10),


  MOD_AGG_TREE_PID = (APP_MOD_MIN_PID + 11),


  MOD_FLOODING_PID = (APP_MOD_MIN_PID + 12),


  TREE_ROUTING_PID = (APP_MOD_MIN_PID + 13),


  SURGE_MOD_PID = (APP_MOD_MIN_PID + 14),


  BEEF_MOD_PID = (APP_MOD_MIN_PID + 15),


  LITEPOT_PID = (APP_MOD_MIN_PID + 16),


  PHOTOTEMP_SENSOR_PID = (APP_MOD_MIN_PID + 17),


  SOUNDER_PID = (APP_MOD_MIN_PID + 18),



  ACK_MOD_PID = (APP_MOD_MIN_PID + 19),


  AODV_PID = (APP_MOD_MIN_PID + 20),


  AODV2_PID = (APP_MOD_MIN_PID + 21),


  GPSR_MOD_PID = (APP_MOD_MIN_PID + 22),


  CLIENT_MOD_PID = (APP_MOD_MIN_PID + 23),


  I2CPACKET_PID = (APP_MOD_MIN_PID + 24),


  VIZ_PID_0 = (APP_MOD_MIN_PID + 25),


  VIZ_PID_1 = (APP_MOD_MIN_PID + 26),


  ACCEL_SENSOR_PID = (APP_MOD_MIN_PID + 27),


  CAMERA_PID = (APP_MOD_MIN_PID + 28),


  BLINK_PID = (APP_MOD_MIN_PID + 29),


  SERIAL_SWITCH_PID = (APP_MOD_MIN_PID + 30),


  TPSN_TIMESYNC_PID = (APP_MOD_MIN_PID + 31),


  RATS_TIMESYNC_PID = (APP_MOD_MIN_PID + 32),


  CRYPTO_SYMMETRIC_PID = (APP_MOD_MIN_PID + 33),


  RFSN_PID = (APP_MOD_MIN_PID + 34),


  OUTLIER_PID = (APP_MOD_MIN_PID + 35),


  PHOTO_SENSOR_PID = (APP_MOD_MIN_PID + 36),

};
# 63 "../../../kernel/include/pid.h" 2



# 1 "../../../processor/avr/include/pid_proc.h" 1
# 19 "../../../processor/avr/include/pid_proc.h"
enum{
            I2C_PID = (DEV_MOD_MIN_PID + 1),
            UART_PID = (DEV_MOD_MIN_PID + 2),
            ADC_PROC_PID = (DEV_MOD_MIN_PID + 3),
};
# 67 "../../../kernel/include/pid.h" 2



# 1 "../../../platform/micaz/include/pid_plat.h" 1
# 15 "../../../platform/micaz/include/pid_plat.h"
enum
  {
    MICASB_PID = ((DEV_MOD_MIN_PID + 3) + 1),
    EXFLASH_PID = ((DEV_MOD_MIN_PID + 3) + 2),
    KER_I2C_PID = ((DEV_MOD_MIN_PID + 3) + 3),
    KER_UART_PID = ((DEV_MOD_MIN_PID + 3) + 4),
    KER_I2C_MGR_PID = ((DEV_MOD_MIN_PID + 3) + 5),
  };
# 71 "../../../kernel/include/pid.h" 2
# 46 "../../../kernel/include/message_types.h" 2
# 1 "/usr/lib/gcc/avr/3.4.3/include/stddef.h" 1 3 4
# 151 "/usr/lib/gcc/avr/3.4.3/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 325 "/usr/lib/gcc/avr/3.4.3/include/stddef.h" 3 4
typedef int wchar_t;
# 47 "../../../kernel/include/message_types.h" 2
# 61 "../../../kernel/include/message_types.h"
typedef struct Message{
 sos_pid_t did;
 sos_pid_t sid;
 uint16_t daddr;
 uint16_t saddr;
 uint8_t type;
 uint8_t len;
 uint8_t *data;
 uint16_t flag;
 uint8_t payload[4];
 struct Message *next;
} __attribute__ ((packed))
Message;


typedef int8_t (*msg_handler_t)(void *state, Message *m);
# 110 "../../../kernel/include/message_types.h"
enum {
 SOS_MSG_NO_STATE,
 SOS_MSG_WAIT,

 SOS_MSG_TX_RAW,
 SOS_MSG_RX_RAW,

 SOS_MSG_TX_CRC_ONLY,
 SOS_MSG_RX_CRC_ONLY,

 SOS_MSG_TX_START,
 SOS_MSG_TX_HDR,
 SOS_MSG_TX_DATA,
 SOS_MSG_TX_CRC_LOW,
 SOS_MSG_TX_CRC_HIGH,
 SOS_MSG_TX_END,

 SOS_MSG_RX_START,
 SOS_MSG_RX_HDR,
 SOS_MSG_RX_DATA,
 SOS_MSG_RX_CRC_LOW,
 SOS_MSG_RX_CRC_HIGH,
 SOS_MSG_RX_END,
};





typedef struct {
 uint8_t byte;
 uint16_t word;
} __attribute__ ((packed))
MsgParam;







enum {



  SOS_MSG_FROM_NETWORK = 0x0100,
  SOS_MSG_RADIO_IO = 0x0200,
  SOS_MSG_I2C_IO = 0x0400,
  SOS_MSG_UART_IO = 0x0800,
  SOS_MSG_SPI_IO = 0x1000,
  SOS_MSG_ALL_LINK_IO = 0x1E00,
  SOS_MSG_LINK_AUTO = 0x2000,

  SOS_MSG_SYSTEM_PRIORITY = 0x0080,
  SOS_MSG_HIGH_PRIORITY = 0x0040,

  SOS_MSG_RELIABLE = 0x0008,
  SOS_MSG_RELEASE = 0x0004,
  SOS_MSG_SEND_FAIL = 0x0002,

  SOS_MSG_USE_UBMAC = 0x0020,
};







enum{
  SOS_RADIO_LINK_ID = 0,
  SOS_I2C_LINK_ID,
  SOS_UART_LINK_ID,
  SOS_SPI_LINK_ID,
};
# 213 "../../../kernel/include/message_types.h"
typedef uint8_t sos_ker_flag_t;
enum {

 SOS_MSG_RULES_PROMISCUOUS = 0x40,

 SOS_KER_STATIC_MODULE = 0x02,

 SOS_KER_MEM_FAILED = 0x01,
};







enum {
 KER_MSG_START = 0,
};







enum {
 MSG_INIT = (KER_MSG_START + 0),
 MSG_DEBUG = (KER_MSG_START + 1),
 MSG_TIMER_TIMEOUT = (KER_MSG_START + 2),
 MSG_PKT_SENDDONE = (KER_MSG_START + 3),
 MSG_DATA_READY = (KER_MSG_START + 4),
 MSG_TIMER3_TIMEOUT = (KER_MSG_START + 5),
 MSG_FINAL = (KER_MSG_START + 6),
 MSG_FROM_USER = (KER_MSG_START + 7),
 MSG_GET_DATA = (KER_MSG_START + 8),
 MSG_SEND_PACKET = (KER_MSG_START + 9),
 MSG_DFUNC_REMOVED = (KER_MSG_START + 10),
 MSG_FUNC_USER_REMOVED = (KER_MSG_START + 11),
 MSG_FETCHER_DONE = (KER_MSG_START + 12),
 MSG_MODULE_OP = (KER_MSG_START + 13),
 MSG_CAL_DATA_READY = (KER_MSG_START + 14),
 MSG_ERROR = (KER_MSG_START + 15),
 MSG_TIMESTAMP = (KER_MSG_START + 16),
 MSG_DISCOVERY = (KER_MSG_START + 17),
 MSG_COMM_TEST = (KER_MSG_START + 21),
 MSG_KER_UNKNOWN = (KER_MSG_START + 31),

 MOD_MSG_START = (KER_MSG_START + 32),
};





enum {
 PROC_MSG_START = 0x40,
 PLAT_MSG_START = 0x80,
 MOD_CMD_START = 0xc0,
};
# 6 "../../../kernel/include/sos_module_types.h" 2


typedef uint16_t sos_code_id_t;
# 23 "../../../kernel/include/sos_module_types.h"
typedef struct mod_header {
  sos_pid_t mod_id;
  uint8_t state_size;
  uint8_t num_timers;
  uint8_t num_sub_func;
  uint8_t num_prov_func;
  uint8_t version;
  uint8_t processor_type;
  uint8_t platform_type;
  sos_code_id_t code_id;
  uint8_t padding;
  uint8_t padding2;
  msg_handler_t module_handler;
  func_cb_t funct[];
} mod_header_t;




typedef struct Module {

  struct Module *next;

  mod_header_ptr header;

  sos_pid_t pid;

  sos_ker_flag_t flag;

  void *handler_state;
} sos_module_t;




enum {
 SOS_CREATE_THREAD = 1,
};







enum {
  MODULE_OP_INSMOD = 1,
  MODULE_OP_RMMOD = 2,
  MODULE_OP_LOAD = 3,
  MODULE_OP_ACTIVATE = 4,
  MODULE_OP_DEACTIVATE = 5,
};






typedef struct {
  sos_pid_t mod_id;
  uint8_t version;

  uint16_t size;
  uint8_t op;
} __attribute__ ((packed))
sos_module_op_t;
# 61 "../../../kernel/include/module.h" 2
# 1 "../../../kernel/include/sos_timer.h" 1
# 21 "../../../kernel/include/sos_timer.h"
# 1 "../../../kernel/include/sos_list.h" 1
# 89 "../../../kernel/include/sos_list.h"
typedef struct list {
 struct list *l_next;
 struct list *l_prev;
} list_t, list_link_t;
# 22 "../../../kernel/include/sos_timer.h" 2
# 1 "../../../processor/avr/include/timer_conf.h" 1
# 23 "../../../kernel/include/sos_timer.h" 2



enum
  {
    TIMER_REPEAT = 0,
    TIMER_ONE_SHOT = 1,
    SLOW_TIMER_REPEAT = 2,
    SLOW_TIMER_ONE_SHOT = 3,
  };
# 41 "../../../kernel/include/sos_timer.h"
enum
  {
    TIMER_PRE_ALLOCATED = 0x02,
  };
# 56 "../../../kernel/include/sos_timer.h"
typedef struct {
  uint8_t tid;
  uint16_t pad;
} __attribute__ ((packed))
sos_timeout_t;





typedef struct {
  list_t list;
  uint8_t type;
  sos_pid_t pid;
  uint8_t tid;
  int32_t ticks;
  int32_t delta;
  uint8_t flag;
} sos_timer_t;





static inline uint8_t timer_get_tid( Message *msg )
{
 MsgParam* params = (MsgParam*)(msg->data);

 return params->byte;
}
# 62 "../../../kernel/include/module.h" 2
# 1 "../../../kernel/include/monitor.h" 1
# 23 "../../../kernel/include/monitor.h"
enum {

 MON_NET_INCOMING = 0x01,

 MON_NET_OUTGOING = 0x02,

 MON_LOCAL = 0x04,

};
# 43 "../../../kernel/include/monitor.h"
typedef struct monitor_str {

 uint8_t type;

 sos_module_t *mod_handle;

 struct monitor_str *next;

} monitor_cb;
# 63 "../../../kernel/include/module.h" 2

# 1 "../../../kernel/include/codemem.h" 1





# 1 "../../../platform/micaz/include/codemem_conf.h" 1
# 9 "../../../platform/micaz/include/codemem_conf.h"
typedef uint16_t code_addr_t;


typedef uint16_t code_page_t;





enum {

 FLASH_PAGE_SIZE = 256L,

 CODEMEM_START_PAGE = 192L,






 CODEMEM_END_PAGE = 495L,


 CODEMEM_START_ADDR = FLASH_PAGE_SIZE * CODEMEM_START_PAGE,


 CODEMEM_SIZE = FLASH_PAGE_SIZE * (CODEMEM_END_PAGE + 1L) - CODEMEM_START_ADDR,


 MAX_RANGE_REL_ADDRESSING_PAGES = 32L,

 MAX_RANGE_REL_ADDRESSING = FLASH_PAGE_SIZE * MAX_RANGE_REL_ADDRESSING_PAGES,



 CODEMEM_ADDRESSING_BYTES = 2L,
};
# 7 "../../../kernel/include/codemem.h" 2
# 32 "../../../kernel/include/codemem.h"
typedef enum {
 CODEMEM_TYPE_NORMAL = 1,
 CODEMEM_TYPE_EXECUTABLE = 2,
} codemem_type_t;

enum {
 CODEMEM_INVALID = 0xffff,
};

typedef uint16_t codemem_t;
# 123 "../../../kernel/include/codemem.h"
extern int8_t ker_codemem_mark_executable(codemem_t h);
# 65 "../../../kernel/include/module.h" 2
# 1 "../../../processor/avr/include/systime.h" 1
# 20 "../../../processor/avr/include/systime.h"
static inline uint32_t ticks_to_msec(uint32_t ticks) {
  uint32_t temp = ticks/1152;
  return (10*temp + ((ticks - temp*1152)*10)/1152);
}




static inline uint32_t msec_to_ticks(uint32_t msec) {
  uint32_t temp = msec/10;
  return (1152*temp + ((msec - temp*10)*1152)/10);
}
# 54 "../../../processor/avr/include/systime.h"
void systime_init();
# 66 "../../../kernel/include/module.h" 2
# 1 "../../../kernel/include/sos_cam.h" 1
# 32 "../../../kernel/include/sos_cam.h"
typedef uint16_t sos_cam_t;
# 71 "../../../kernel/include/sos_cam.h"
static inline sos_cam_t ker_cam_key( sos_pid_t pid, uint8_t id )
{
  return (((sos_cam_t) pid) << 8) | id;
}
# 67 "../../../kernel/include/module.h" 2

# 1 "/usr/lib/gcc/avr/3.4.3/include/stddef.h" 1 3 4
# 69 "../../../kernel/include/module.h" 2
# 1 "../../../processor/avr/include/kertable_conf.h" 1
# 70 "../../../kernel/include/module.h" 2

# 1 "../../../kernel/include/sos_error_types.h" 1
# 12 "../../../kernel/include/sos_error_types.h"
enum {
    READ_ERROR = 0,
 SEND_ERROR,
    SENSOR_ERROR,
    MALLOC_ERROR,
  };
# 72 "../../../kernel/include/module.h" 2
# 82 "../../../kernel/include/module.h"
# 1 "../../../processor/avr/include/module_proc.h" 1




# 1 "../../../processor/avr/include/kertable_proc.h" 1



# 1 "../../../kernel/include/kertable.h" 1
# 5 "../../../processor/avr/include/kertable_proc.h" 2
# 6 "../../../processor/avr/include/module_proc.h" 2






typedef int8_t (*adc_bindPort_func_t)(uint8_t port, uint8_t adcPort, sos_pid_t calling_id, uint8_t cb_fid);
static inline int8_t ker_adc_proc_bindPort(uint8_t port, uint8_t adcPort, sos_pid_t calling_id, uint8_t cb_fid)
{
 adc_bindPort_func_t func = (adc_bindPort_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +1))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(port, adcPort, calling_id, cb_fid);
}





typedef int8_t (*adc_unbindPort_func_t)(uint8_t port, sos_pid_t calling_id);
static inline int8_t ker_adc_proc_unbindPort(uint8_t port, sos_pid_t calling_id)
{
 adc_unbindPort_func_t func = (adc_unbindPort_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +2))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(port, calling_id);
}




typedef int8_t (*adc_getData_func_t)(uint8_t port, uint8_t flags);
static inline int8_t ker_adc_proc_getData(uint8_t port, uint8_t flags)
{
 adc_getData_func_t func = (adc_getData_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +3))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(port, flags);
}




typedef int8_t (*adc_getPeriodicData_func_t)(uint8_t port, uint8_t prescaler, uint16_t count);
static inline int8_t ker_adc_proc_getPeriodicData(uint8_t port, uint8_t prescaler, uint16_t count)
{
 adc_getPeriodicData_func_t func = (adc_getPeriodicData_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +4))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(port, prescaler, count);
}





typedef int8_t (*adc_stopPeriodicData_func_t)(uint8_t port);
static inline int8_t ker_adc_proc_stopPerodicData(uint8_t port)
{
 adc_stopPeriodicData_func_t func = (adc_stopPeriodicData_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +5))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(port);
}




typedef int8_t (*i2c_reserve_bus_func_t)(uint8_t calling_mod_id, uint8_t ownAddress, uint8_t flags);
static inline int8_t ker_i2c_reserve_bus(uint8_t calling_mod_id, uint8_t ownAddress, uint8_t flags) {
 i2c_reserve_bus_func_t func =
  (i2c_reserve_bus_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +6))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(calling_mod_id, ownAddress, flags);
}



typedef int8_t (*i2c_release_bus_func_t)(uint8_t calling_mod_id);
static inline int8_t ker_i2c_release_bus(uint8_t calling_mod_id)
{
 i2c_release_bus_func_t func =
  (i2c_release_bus_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +7))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(calling_mod_id);
}







typedef int8_t (*i2c_send_data_func_t)(
  uint8_t i2c_addr,
   uint8_t *msg,
   uint8_t msg_size,
   uint8_t calling_id);
static inline int8_t ker_i2c_send_data(
  uint8_t i2c_addr,
  uint8_t *msg,
  uint8_t msg_size,
  uint8_t calling_id)
{
 i2c_send_data_func_t func = (i2c_send_data_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +8))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(i2c_addr, msg, msg_size, calling_id);
}





typedef int8_t (*i2c_read_data_func_t)(
  uint8_t i2c_addr,
  uint8_t msg_size,
  uint8_t calling_id);
static inline int8_t ker_i2c_read_data(
  uint8_t i2c_addr,
  uint8_t msg_size,
  uint8_t calling_id)
{
 i2c_read_data_func_t func = (i2c_read_data_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +9))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(i2c_addr, msg_size, calling_id);
}




typedef int8_t (*uart_reserve_bus_func_t)(uint8_t calling_mod_id, uint8_t flags);
static inline int8_t ker_uart_reserve_bus(uint8_t calling_mod_id, uint8_t flags) {
 uart_reserve_bus_func_t func =
  (uart_reserve_bus_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +10))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(calling_mod_id, flags);
}



typedef int8_t (*uart_release_bus_func_t)(uint8_t calling_mod_id);
static inline int8_t ker_uart_release_bus(uint8_t calling_mod_id)
{
 uart_release_bus_func_t func =
  (uart_release_bus_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +11))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(calling_mod_id);
}







typedef int8_t (*uart_send_data_func_t)(
   uint8_t *buff,
   uint8_t msg_size,
   uint8_t calling_id);
static inline int8_t ker_uart_send_data(
  uint8_t *buff,
  uint8_t msg_size,
  uint8_t calling_id)
{
 uart_send_data_func_t func = (uart_send_data_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +12))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(buff, msg_size, calling_id);
}






typedef int8_t (*uart_read_data_func_t)(
  uint8_t msg_size,
  uint8_t calling_id);
static inline int8_t ker_uart_read_data(
  uint8_t msg_size,
  uint8_t calling_id)
{
 uart_read_data_func_t func = (uart_read_data_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +13))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(msg_size, calling_id);
}






typedef void (*ker_memmap_perms_check_func_t)(void* x);
static inline void ker_memmap_perms_check(void* x)
{
  ker_memmap_perms_check_func_t func =
    (ker_memmap_perms_check_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61 +14))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
  func(x);
  return;
}
# 83 "../../../kernel/include/module.h" 2
# 1 "../../../platform/micaz/include/module_plat.h" 1
# 9 "../../../platform/micaz/include/module_plat.h"
# 1 "../../../platform/micaz/include/kertable_plat.h" 1
# 10 "../../../platform/micaz/include/module_plat.h" 2






typedef void (*ker_radio_ack_func_t)(void);

static inline void ker_radio_ack_enable(void)
{
 ker_radio_ack_func_t func = (ker_radio_ack_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*((61 +13)+1))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 func();
 return;
}

static inline void ker_radio_ack_disable(void)
{
 ker_radio_ack_func_t func = (ker_radio_ack_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*((61 +13)+2))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 func();
 return;
}
# 46 "../../../platform/micaz/include/module_plat.h"
typedef int8_t (*ledfunc_t)(uint8_t action);
static inline int8_t ker_led(uint8_t action){
 ledfunc_t func = (ledfunc_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*((61 +13)+3))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(action);
}
# 84 "../../../kernel/include/module.h" 2


typedef uint16_t (* ker_hw_type_ker_func_t)( void);
static inline uint16_t ker_hw_type( void)
{
 ker_hw_type_ker_func_t func =
  (ker_hw_type_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(10))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}



typedef uint16_t (* ker_id_ker_func_t)( void);
static inline uint16_t ker_id( void)
{
 ker_id_ker_func_t func =
  (ker_id_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(11))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}



typedef uint16_t (* ker_rand_ker_func_t)( void);
static inline uint16_t ker_rand( void)
{
 ker_rand_ker_func_t func =
  (ker_rand_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(12))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}



typedef uint32_t (* ker_systime32_ker_func_t)( void);
static inline uint32_t ker_systime32( void)
{
 ker_systime32_ker_func_t func =
  (ker_systime32_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(13))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}




typedef void (* dummy14_ker_func_t)( void);
static inline void dummy14( void)
{
 dummy14_ker_func_t func =
  (dummy14_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(14))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 func( );
}




typedef void (* dummy15_ker_func_t)( void);
static inline void dummy15( void)
{
 dummy15_ker_func_t func =
  (dummy15_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(15))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 func( );
}




typedef void (* dummy16_ker_func_t)( void);
static inline void dummy16( void)
{
 dummy16_ker_func_t func =
  (dummy16_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(16))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 func( );
}




typedef void (* dummy17_ker_func_t)( void);
static inline void dummy17( void)
{
 dummy17_ker_func_t func =
  (dummy17_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(17))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 func( );
}



typedef uint8_t * (* ker_msg_take_data_ker_func_t)( sos_pid_t pid, Message * msg );
static inline uint8_t * ker_msg_take_data( sos_pid_t pid, Message * msg )
{
 ker_msg_take_data_ker_func_t func =
  (ker_msg_take_data_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(18))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid, msg );
}



typedef int8_t (* ker_msg_change_rules_ker_func_t)( sos_pid_t sid, uint8_t rules );
static inline int8_t ker_msg_change_rules( sos_pid_t sid, uint8_t rules )
{
 ker_msg_change_rules_ker_func_t func =
  (ker_msg_change_rules_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(19))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( sid, rules );
}



typedef int8_t (* ker_timer_init_ker_func_t)( sos_pid_t pid, uint8_t tid, uint8_t type );
static inline int8_t ker_timer_init( sos_pid_t pid, uint8_t tid, uint8_t type )
{
 ker_timer_init_ker_func_t func =
  (ker_timer_init_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(20))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid, tid, type );
}



typedef int8_t (* ker_timer_start_ker_func_t)( sos_pid_t pid, uint8_t tid, int32_t interval );
static inline int8_t ker_timer_start( sos_pid_t pid, uint8_t tid, int32_t interval )
{
 ker_timer_start_ker_func_t func =
  (ker_timer_start_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(21))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid, tid, interval );
}



typedef int8_t (* ker_timer_restart_ker_func_t)( sos_pid_t pid, uint8_t tid, int32_t interval );
static inline int8_t ker_timer_restart( sos_pid_t pid, uint8_t tid, int32_t interval )
{
 ker_timer_restart_ker_func_t func =
  (ker_timer_restart_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(22))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid, tid, interval );
}



typedef int8_t (* ker_timer_stop_ker_func_t)( sos_pid_t pid, uint8_t tid );
static inline int8_t ker_timer_stop( sos_pid_t pid, uint8_t tid )
{
 ker_timer_stop_ker_func_t func =
  (ker_timer_stop_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(23))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid, tid );
}



typedef int8_t (* ker_timer_release_ker_func_t)( sos_pid_t pid, uint8_t tid );
static inline int8_t ker_timer_release( sos_pid_t pid, uint8_t tid )
{
 ker_timer_release_ker_func_t func =
  (ker_timer_release_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(24))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid, tid );
}



typedef int8_t (* post_link_ker_func_t)( sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void * larg, uint16_t flag, uint16_t daddr );
static inline int8_t post_link( sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void * larg, uint16_t flag, uint16_t daddr )
{
 post_link_ker_func_t func =
  (post_link_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(25))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( did, sid, type, arg, larg, flag, daddr );
}



typedef int8_t (* post_ker_func_t)( Message * m );
static inline int8_t post( Message * m )
{
 post_ker_func_t func =
  (post_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(26))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( m );
}



typedef int8_t (* post_short_ker_func_t)( sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t byte, uint16_t word, uint16_t flag );
static inline int8_t post_short( sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t byte, uint16_t word, uint16_t flag )
{
 post_short_ker_func_t func =
  (post_short_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(27))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( did, sid, type, byte, word, flag );
}



typedef int8_t (* post_long_ker_func_t)( sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void * larg, uint16_t flag );
static inline int8_t post_long( sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void * larg, uint16_t flag )
{
 post_long_ker_func_t func =
  (post_long_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(28))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( did, sid, type, arg, larg, flag );
}



typedef int8_t (* post_longer_ker_func_t)( sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void * larg, uint16_t flag, uint16_t saddr );
static inline int8_t post_longer( sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void * larg, uint16_t flag, uint16_t saddr )
{
 post_longer_ker_func_t func =
  (post_longer_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(29))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( did, sid, type, arg, larg, flag, saddr );
}



typedef node_loc_t (* ker_loc_ker_func_t)( void);
static inline node_loc_t ker_loc( void)
{
 ker_loc_ker_func_t func =
  (ker_loc_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(30))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}



typedef gps_t (* ker_gps_ker_func_t)( void);
static inline gps_t ker_gps( void)
{
 ker_gps_ker_func_t func =
  (ker_gps_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(31))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}



typedef uint32_t (* ker_loc_r2_ker_func_t)( node_loc_t * loc1, node_loc_t * loc2 );
static inline uint32_t ker_loc_r2( node_loc_t * loc1, node_loc_t * loc2 )
{
 ker_loc_r2_ker_func_t func =
  (ker_loc_r2_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(32))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( loc1, loc2 );
}



typedef uint16_t (* ker_systime16L_ker_func_t)( void);
static inline uint16_t ker_systime16L( void)
{
 ker_systime16L_ker_func_t func =
  (ker_systime16L_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(33))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}



typedef uint16_t (* ker_systime16H_ker_func_t)( void);
static inline uint16_t ker_systime16H( void)
{
 ker_systime16H_ker_func_t func =
  (ker_systime16H_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(34))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}



typedef int8_t (* ker_register_module_ker_func_t)( mod_header_ptr h );
static inline int8_t ker_register_module( mod_header_ptr h )
{
 ker_register_module_ker_func_t func =
  (ker_register_module_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(35))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( h );
}



typedef int8_t (* ker_deregister_module_ker_func_t)( sos_pid_t pid );
static inline int8_t ker_deregister_module( sos_pid_t pid )
{
 ker_deregister_module_ker_func_t func =
  (ker_deregister_module_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(36))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid );
}



typedef sos_module_t * (* ker_get_module_ker_func_t)( sos_pid_t pid );
static inline sos_module_t * ker_get_module( sos_pid_t pid )
{
 ker_get_module_ker_func_t func =
  (ker_get_module_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(37))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid );
}



typedef int8_t (* ker_register_monitor_ker_func_t)( sos_pid_t pid, uint8_t type, monitor_cb * cb );
static inline int8_t ker_register_monitor( sos_pid_t pid, uint8_t type, monitor_cb * cb )
{
 ker_register_monitor_ker_func_t func =
  (ker_register_monitor_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(38))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid, type, cb );
}



typedef int8_t (* ker_deregister_monitor_ker_func_t)( monitor_cb * cb );
static inline int8_t ker_deregister_monitor( monitor_cb * cb )
{
 ker_deregister_monitor_ker_func_t func =
  (ker_deregister_monitor_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(39))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( cb );
}



typedef int8_t (* ker_fntable_subscribe_ker_func_t)( sos_pid_t sub_pid, sos_pid_t pub_pid, uint8_t fid, uint8_t table_index );
static inline int8_t ker_fntable_subscribe( sos_pid_t sub_pid, sos_pid_t pub_pid, uint8_t fid, uint8_t table_index )
{
 ker_fntable_subscribe_ker_func_t func =
  (ker_fntable_subscribe_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(40))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( sub_pid, pub_pid, fid, table_index );
}



typedef int8_t (* ker_sensor_register_ker_func_t)( sos_pid_t sensor_driver_pid, uint8_t sensor_id, uint8_t sensor_fid, void* data );
static inline int8_t ker_sensor_register( sos_pid_t sensor_driver_pid, uint8_t sensor_id, uint8_t sensor_fid, void* data )
{
 ker_sensor_register_ker_func_t func =
  (ker_sensor_register_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(41))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( sensor_driver_pid, sensor_id, sensor_fid, data );
}



typedef int8_t (* ker_sensor_deregister_ker_func_t)( sos_pid_t sensor_driver_pid, uint8_t sensor_id );
static inline int8_t ker_sensor_deregister( sos_pid_t sensor_driver_pid, uint8_t sensor_id )
{
 ker_sensor_deregister_ker_func_t func =
  (ker_sensor_deregister_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(42))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( sensor_driver_pid, sensor_id );
}



typedef int8_t (* ker_sensor_get_data_ker_func_t)( sos_pid_t client_pid, uint8_t sensor_id );
static inline int8_t ker_sensor_get_data( sos_pid_t client_pid, uint8_t sensor_id )
{
 ker_sensor_get_data_ker_func_t func =
  (ker_sensor_get_data_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(43))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( client_pid, sensor_id );
}



typedef int8_t (* ker_sensor_data_ready_ker_func_t)( uint8_t sensor_id, uint16_t sensor_data, uint8_t status );
static inline int8_t ker_sensor_data_ready( uint8_t sensor_id, uint16_t sensor_data, uint8_t status )
{
 ker_sensor_data_ready_ker_func_t func =
  (ker_sensor_data_ready_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(44))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( sensor_id, sensor_data, status );
}


typedef int8_t (* ker_sensor_enable_ker_func_t)( sos_pid_t client_pid, uint8_t sensor_id );
static inline int8_t ker_sensor_enable( sos_pid_t client_pid, uint8_t sensor_id )
{
 ker_sensor_enable_ker_func_t func =
  (ker_sensor_enable_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(45))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( client_pid, sensor_id );
}


static inline int8_t ker_sensor_disable( sos_pid_t client_pid, uint8_t sensor_id )
{
 ker_sensor_enable_ker_func_t func =
  (ker_sensor_enable_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(46))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( client_pid, sensor_id );
}


typedef int8_t (* ker_sensor_control_ker_func_t)( sos_pid_t client_pid, uint8_t sensor_id, void* sensor_new_state );
static inline int8_t ker_sensor_control( sos_pid_t client_pid, uint8_t sensor_id, void* sensor_new_state )
{
 ker_sensor_control_ker_func_t func =
  (ker_sensor_control_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(47))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( client_pid, sensor_id, sensor_new_state );
}




typedef sos_pid_t (* ker_set_current_pid_ker_func_t)( sos_pid_t pid );
static inline sos_pid_t ker_set_current_pid( sos_pid_t pid )
{
 ker_set_current_pid_ker_func_t func =
  (ker_set_current_pid_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(48))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid );
}



typedef sos_pid_t (* ker_get_current_pid_ker_func_t)( void);
static inline sos_pid_t ker_get_current_pid( void)
{
 ker_get_current_pid_ker_func_t func =
  (ker_get_current_pid_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(49))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}
# 497 "../../../kernel/include/module.h"
typedef void * (* ker_get_module_state_ker_func_t)( sos_pid_t pid );
static inline void * ker_get_module_state( sos_pid_t pid )
{
 ker_get_module_state_ker_func_t func =
  (ker_get_module_state_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(50))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid );
}




typedef sos_pid_t (* ker_spawn_module_ker_func_t)( mod_header_ptr h, void * init, uint8_t init_size, uint8_t flag );
static inline sos_pid_t ker_spawn_module( mod_header_ptr h, void * init, uint8_t init_size, uint8_t flag )
{
 ker_spawn_module_ker_func_t func =
  (ker_spawn_module_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(51))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( h, init, init_size, flag );
}



typedef mod_header_ptr (* ker_codemem_get_header_from_code_id_ker_func_t)( sos_code_id_t cid );
static inline mod_header_ptr ker_codemem_get_header_from_code_id( sos_code_id_t cid )
{
 ker_codemem_get_header_from_code_id_ker_func_t func =
  (ker_codemem_get_header_from_code_id_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(52))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( cid );
}



typedef mod_header_ptr (* ker_codemem_get_header_address_ker_func_t)( codemem_t cid );
static inline mod_header_ptr ker_codemem_get_header_address( codemem_t cid )
{
 ker_codemem_get_header_address_ker_func_t func =
  (ker_codemem_get_header_address_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(53))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( cid );
}



typedef codemem_t (* ker_codemem_alloc_ker_func_t)( uint16_t size, codemem_type_t type );
static inline codemem_t ker_codemem_alloc( uint16_t size, codemem_type_t type )
{
 ker_codemem_alloc_ker_func_t func =
  (ker_codemem_alloc_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(54))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( size, type );
}


typedef int8_t (* ker_codemem_write_ker_func_t)( codemem_t h, sos_pid_t pid, void * buf, uint16_t nbytes, uint16_t offset );
static inline int8_t ker_codemem_write( codemem_t h, sos_pid_t pid, void * buf, uint16_t nbytes, uint16_t offset )
{
 ker_codemem_write_ker_func_t func =
  (ker_codemem_write_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(55))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( h, pid, buf, nbytes, offset );
}



typedef int8_t (* ker_codemem_read_ker_func_t)( codemem_t h, sos_pid_t pid, void * buf, uint16_t nbytes, uint16_t offset );
static inline int8_t ker_codemem_read( codemem_t h, sos_pid_t pid, void * buf, uint16_t nbytes, uint16_t offset )
{
 ker_codemem_read_ker_func_t func =
  (ker_codemem_read_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(56))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( h, pid, buf, nbytes, offset );
}



typedef int8_t (* ker_codemem_free_ker_func_t)( codemem_t h );
static inline int8_t ker_codemem_free( codemem_t h )
{
 ker_codemem_free_ker_func_t func =
  (ker_codemem_free_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(57))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( h );
}



typedef int8_t (* ker_codemem_flush_ker_func_t)( codemem_t h, sos_pid_t pid );
static inline int8_t ker_codemem_flush( codemem_t h, sos_pid_t pid )
{
 ker_codemem_flush_ker_func_t func =
  (ker_codemem_flush_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(58))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( h, pid );
}



typedef dummy_func (* ker_get_func_ptr_ker_func_t)( func_cb_ptr p, sos_pid_t * prev );
static inline dummy_func ker_get_func_ptr( func_cb_ptr p, sos_pid_t * prev )
{
 ker_get_func_ptr_ker_func_t func =
  (ker_get_func_ptr_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(59))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( p, prev );
}



typedef int8_t (* ker_panic_ker_func_t)( void);
static inline int8_t ker_panic( void)
{
 ker_panic_ker_func_t func =
  (ker_panic_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(60))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( );
}



typedef int8_t (* ker_mod_panic_ker_func_t)( sos_pid_t pid );
static inline int8_t ker_mod_panic( sos_pid_t pid )
{
 ker_mod_panic_ker_func_t func =
  (ker_mod_panic_ker_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(61))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func( pid );
}



# 1 "../../../kernel/include/module_virtual.h" 1







static inline void error_v(func_cb_ptr p)
{
}

static inline int8_t error_8(func_cb_ptr p)
{
     return -1;
}

static inline int16_t error_16(func_cb_ptr p)
{
     return -1;
}

static inline int32_t error_32(func_cb_ptr p)
{
     return -1;
}

static inline void* error_ptr(func_cb_ptr p)
{
     return ((void *)0);
}


typedef int8_t (* post_link_func_t)( sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void * larg, uint16_t flag, uint16_t daddr );

static inline int8_t post_net(sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void *larg, uint16_t flag, uint16_t daddr)
{
 post_link_func_t func = (post_link_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(25))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(did, sid, type, arg, larg, flag|SOS_MSG_RADIO_IO, daddr);
}

static inline int8_t post_auto(sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void *larg, uint16_t flag, uint16_t daddr)
{
 post_link_func_t func = (post_link_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(25))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(did, sid, type, arg, larg, flag | SOS_MSG_ALL_LINK_IO | SOS_MSG_LINK_AUTO, daddr);
}

static inline int8_t post_uart(sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void *larg, uint16_t flag, uint16_t daddr)
{
 post_link_func_t func = (post_link_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(25))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(did, sid, type, arg, larg, flag|SOS_MSG_UART_IO, daddr);
}

static inline int8_t post_i2c(sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void *larg, uint16_t flag, uint16_t daddr)
{
 post_link_func_t func = (post_link_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(25))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(did, sid, type, arg, larg, flag|SOS_MSG_I2C_IO, daddr);
}

static inline int8_t post_spi(sos_pid_t did, sos_pid_t sid, uint8_t type, uint8_t arg, void *larg, uint16_t flag, uint16_t daddr)
{
 post_link_func_t func = (post_link_func_t)({ uint16_t __addr16 = (uint16_t)((uint16_t)(0x110 +2*(25))); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; });
 return func(did, sid, type, arg, larg, flag|SOS_MSG_SPI_IO, daddr);
}
# 618 "../../../kernel/include/module.h" 2
# 2 "roombacomm.c" 2
# 1 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 1 3
# 47 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
# 1 "/usr/lib/gcc/avr/3.4.3/include/stddef.h" 1 3 4
# 48 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 2 3
# 66 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
typedef struct {
 int quot;
 int rem;
} div_t;


typedef struct {
 long quot;
 long rem;
} ldiv_t;


typedef int (*__compar_fn_t)(const void *, const void *);
# 100 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern __inline__ void abort(void) __attribute__((__noreturn__));




extern __inline__ void
abort(void)
{
 for (;;);
}




extern int abs(int __i) __attribute__((__const__));
# 123 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern long labs(long __i) __attribute__((__const__));
# 146 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern void *bsearch(const void *__key, const void *__base, size_t __nmemb,
       size_t __size, int (*__compar)(const void *, const void *));







extern div_t div(int __num, int __denom) __asm__("__divmodhi4") __attribute__((__const__));





extern ldiv_t ldiv(long __num, long __denom) __asm__("__divmodsi4") __attribute__((__const__));
# 178 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern void qsort(void *__base, size_t __nmemb, size_t __size,
    __compar_fn_t __compar);
# 211 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern long strtol(const char *__nptr, char **__endptr, int __base);
# 245 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern unsigned long strtoul(const char *__nptr, char **__endptr, int __base);
# 255 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern __inline__ long atol(const char *__nptr) __attribute__((__pure__));
extern __inline__ long
atol(const char *__nptr)
{
 return strtol(__nptr, (char **) 0, 10);
}
# 279 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern int atoi(const char *) __attribute__((__pure__));
# 291 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern void exit(int __status) __attribute__((__noreturn__));
# 303 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern void *malloc(size_t __size) __attribute__((__malloc__));






extern void free(void *__ptr);




extern size_t __malloc_margin;




extern char *__malloc_heap_start;




extern char *__malloc_heap_end;






extern void *calloc(size_t __nele, size_t __size) __attribute__((__malloc__));
# 351 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern void *realloc(void *__ptr, size_t __size) __attribute__((__malloc__));
# 383 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern double strtod(const char *__nptr, char **__endptr);
# 394 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern double atof(const char *__nptr);
# 416 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern int rand(void);



extern void srand(unsigned int __seed);






extern int rand_r(unsigned long *ctx);
# 458 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern char *itoa(int __val, char *__s, int __radix);
# 486 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern char *ltoa(long int __val, char *__s, int __radix);
# 512 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern char *utoa(unsigned int __val, char *__s, int __radix);
# 538 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern char *ultoa(unsigned long int __val, char *__s, int __radix);
# 556 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern long random(void);




extern void srandom(unsigned long __seed);







extern long random_r(unsigned long *ctx);
# 611 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern char *dtostre(double __val, char *__s, unsigned char __prec,
       unsigned char __flags);
# 626 "/usr/lib/gcc/avr/3.4.3/../../../../avr/include/stdlib.h" 3
extern char *dtostrf(double __val, char __width, char __prec, char *__s);
# 3 "roombacomm.c" 2
# 1 "../../../kernel/include/sys_module.h" 1
# 13 "../../../kernel/include/sys_module.h"
# 1 "/usr/lib/gcc/avr/3.4.3/include/stddef.h" 1 3 4
# 14 "../../../kernel/include/sys_module.h" 2
# 50 "../../../kernel/include/sys_module.h"
typedef void * (* sys_malloc_ker_func_t)( uint16_t size );
# 70 "../../../kernel/include/sys_module.h"
static inline void * sys_malloc( uint16_t size )
{

 return ((sys_malloc_ker_func_t)(0x8c +4*1))(size);



}


typedef void * (* sys_realloc_ker_func_t)( void * ptr, uint16_t newSize );
# 103 "../../../kernel/include/sys_module.h"
static inline void * sys_realloc( void * ptr, uint16_t newSize )
{

 return ((sys_realloc_ker_func_t)(0x8c +4*2))( ptr, newSize );



}



typedef void (* sys_free_ker_func_t)( void * ptr );
# 123 "../../../kernel/include/sys_module.h"
static inline void sys_free( void * ptr )
{

 ((sys_free_ker_func_t)(0x8c +4*3))( ptr );



}


typedef void * (* sys_msg_take_data_ker_func_t)( Message * msg );
# 169 "../../../kernel/include/sys_module.h"
static inline void * sys_msg_take_data( Message * msg )
{

 return ((sys_msg_take_data_ker_func_t)(0x8c +4*4))( msg );



}
# 190 "../../../kernel/include/sys_module.h"
typedef int8_t (* sys_timer_start_ker_func_t)( uint8_t tid, int32_t interval, uint8_t type );
# 207 "../../../kernel/include/sys_module.h"
static inline int8_t sys_timer_start( uint8_t tid, int32_t interval, uint8_t type )
{

 return ((sys_timer_start_ker_func_t)(0x8c +4*5))( tid, interval, type );



}



typedef int8_t (* sys_timer_restart_ker_func_t)( uint8_t tid, int32_t interval );
# 241 "../../../kernel/include/sys_module.h"
static inline int8_t sys_timer_restart( uint8_t tid, int32_t interval )
{

 return ((sys_timer_restart_ker_func_t)(0x8c +4*6))( tid, interval );



}



typedef int8_t (* sys_timer_stop_ker_func_t)( uint8_t tid );
# 265 "../../../kernel/include/sys_module.h"
static inline int8_t sys_timer_stop( uint8_t tid )
{

 return ((sys_timer_stop_ker_func_t)(0x8c +4*7))( tid );



}
# 282 "../../../kernel/include/sys_module.h"
typedef int8_t (* sys_post_ker_func_t)( sos_pid_t dst_mod_id, uint8_t type, uint8_t size, void * data, uint16_t flag );
# 323 "../../../kernel/include/sys_module.h"
static inline int8_t sys_post( sos_pid_t dst_mod_id, uint8_t type, uint8_t size, void * data, uint16_t flag )
{

 return ((sys_post_ker_func_t)(0x8c +4*8))( dst_mod_id, type, size, data, flag );



}


typedef int8_t (* sys_post_link_ker_func_t)( sos_pid_t dst_mod_id, uint8_t type, uint8_t size, void * data, uint16_t flag, uint16_t dst_node_addr );
# 343 "../../../kernel/include/sys_module.h"
static inline int8_t sys_post_link( sos_pid_t dst_mod_id, uint8_t type, uint8_t size, void * data, uint16_t flag, uint16_t dst_node_addr )
{

 return ((sys_post_link_ker_func_t)(0x8c +4*9))( dst_mod_id, type, size, data, flag, dst_node_addr );



}
# 436 "../../../kernel/include/sys_module.h"
typedef int8_t (* sys_post_value_ker_func_t)( sos_pid_t dst_mod_id, uint8_t type, uint32_t data, uint16_t flag );
# 459 "../../../kernel/include/sys_module.h"
static inline int8_t sys_post_value( sos_pid_t dst_mod_id, uint8_t type, uint32_t data, uint16_t flag )
{

 return ((sys_post_value_ker_func_t)(0x8c +4*10))( dst_mod_id, type, data, flag );



}
# 476 "../../../kernel/include/sys_module.h"
typedef uint16_t (* sys_hw_type_ker_func_t)( void );
# 497 "../../../kernel/include/sys_module.h"
static inline uint16_t sys_hw_type( void )
{

 return ((sys_hw_type_ker_func_t)(0x8c +4*11))( );



}


typedef uint16_t (* sys_id_ker_func_t)( void );
# 526 "../../../kernel/include/sys_module.h"
static inline uint16_t sys_id( void )
{

 return ((sys_id_ker_func_t)(0x8c +4*12))( );



}
# 544 "../../../kernel/include/sys_module.h"
typedef uint16_t (* sys_rand_ker_func_t)( void );
# 556 "../../../kernel/include/sys_module.h"
static inline uint16_t sys_rand( void )
{

 return ((sys_rand_ker_func_t)(0x8c +4*13))();



}
# 573 "../../../kernel/include/sys_module.h"
typedef uint32_t (* sys_time32_ker_func_t)( void );
# 582 "../../../kernel/include/sys_module.h"
static inline uint32_t sys_time32( void )
{

 return ((sys_time32_ker_func_t)(0x8c +4*14))( );



}
# 600 "../../../kernel/include/sys_module.h"
typedef int8_t (* sys_sensor_get_data_ker_func_t)( uint8_t sensor_id );





static inline int8_t sys_sensor_get_data( uint8_t sensor_id )
{

 return ((sys_sensor_get_data_ker_func_t)(0x8c +4*15))( sensor_id );



}
# 626 "../../../kernel/include/sys_module.h"
typedef void (* sys_led_ker_func_t)( uint8_t op );

static inline void sys_led( uint8_t op )
{

 ((sys_led_ker_func_t)(0x8c +4*16))( op );



}
# 645 "../../../kernel/include/sys_module.h"
typedef void* (* sys_get_module_state_func_t)( void );





static inline void* sys_get_state( void )
{

 return ((sys_get_module_state_func_t)(0x8c +4*17))( );



}
# 4 "roombacomm.c" 2

# 1 "../../../kernel/include/led_dbg.h" 1
# 52 "../../../kernel/include/led_dbg.h"
# 1 "../../../platform/micaz/include/led.h" 1
# 53 "../../../kernel/include/led_dbg.h" 2
# 6 "roombacomm.c" 2
# 1 "../../../drivers/uart/include/uart_system.h" 1
# 12 "../../../drivers/uart/include/uart_system.h"
# 1 "../../../processor/avr/include/proc_msg_types.h" 1
# 16 "../../../processor/avr/include/proc_msg_types.h"
enum {

 MSG_I2C_SEND_DONE = PROC_MSG_START,
 MSG_I2C_READ_DONE,

 MSG_UART_SEND_DONE,
 MSG_UART_READ_DONE,
};
# 13 "../../../drivers/uart/include/uart_system.h" 2
# 28 "../../../drivers/uart/include/uart_system.h"
int8_t uart_system_init();

void uart_read_done(uint8_t len, uint8_t status);
void uart_send_done(uint8_t status);
# 7 "roombacomm.c" 2

# 1 "roombacomm.h" 1
# 28 "roombacomm.h"
typedef struct {
    int16_t velocity;
    uint8_t pid;
    uint8_t state;
    uint8_t uart_buf[5];
    uint8_t uart_buf_len;
    uint8_t straightRadius;
    uint8_t straightDistance;
    int8_t (*pauseTimeoutFunc)();
} roombacomm_state_t;

enum {
    ROOMBACOMM_START_FID = 1,
    ROOMBACOMM_CONTROL_FID = 2,
    ROOMBACOMM_STOP_FID = 3,
    ROOMBACOMM_RESET_FID = 4,
    ROOMBACOMM_GOSTRAIGHTAT_FID = 5,
    ROOMBACOMM_GOSTRAIGHTFOR_FID = 6,
    ROOMBACOMM_SPINLEFTAT_FID = 7,
    ROOMBACOMM_SPINLEFTFOR_FID = 8,
    ROOMBACOMM_SPINRIGHTAT_FID = 9,
    ROOMBACOMM_SPINRIGHTFOR_FID = 10
};

typedef int8_t (*roombacomm_Start_proto)(func_cb_ptr);
typedef int8_t (*roombacomm_Control_proto)(func_cb_ptr);
typedef int8_t (*roombacomm_Stop_proto)(func_cb_ptr);
typedef int8_t (*roombacomm_Reset_proto)(func_cb_ptr);
typedef int8_t (*roombacomm_GoStraightAt_proto)(func_cb_ptr, int16_t);
typedef int8_t (*roombacomm_GoStraightFor_proto)(func_cb_ptr, int16_t);
typedef int8_t (*roombacomm_SpinLeftAt_proto)(func_cb_ptr, int16_t);
typedef int8_t (*roombacomm_SpinLeftFor_proto)(func_cb_ptr, int16_t);
typedef int8_t (*roombacomm_SpinRightAt_proto)(func_cb_ptr, int16_t);
typedef int8_t (*roombacomm_SpinRightFor_proto)(func_cb_ptr, int16_t);
# 9 "roombacomm.c" 2


static int8_t roombacomm_MsgHandler(void *state, Message *msg);
static int8_t roombacomm_Init(uint8_t mpid);
static int8_t roombacomm_Drive(int16_t velocity, int16_t radius);
static int8_t roombacomm_Send();
static void roombacomm_ResetBuf();
static int8_t roombacomm_GoStraightSegment(char *p);
static int8_t roombacomm_Pause(int16_t ptime, int8_t (*funcptr)());

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

static mod_header_t mod_header __attribute__ ((__progmem__)) = {
    .mod_id = DFLT_APP_ID1,
    .state_size = sizeof(roombacomm_state_t),
    .num_timers = 1,
    .num_sub_func = 0,
    .num_prov_func = 0,
    .platform_type = MICAZ ,
    .processor_type = MCU_AVR,
    .code_id = (DFLT_APP_ID1),
    .module_handler = roombacomm_MsgHandler,
    .funct = { [0] = {roombacomm_Start, "cvv0", DFLT_APP_ID1, ROOMBACOMM_START_FID}, [1] = {roombacomm_Control, "cvv0", DFLT_APP_ID1, ROOMBACOMM_CONTROL_FID}, [2] = {roombacomm_Stop, "cvv0", DFLT_APP_ID1, ROOMBACOMM_STOP_FID}, [3] = {roombacomm_Reset, "cvv0", DFLT_APP_ID1, ROOMBACOMM_RESET_FID}, [4] = {roombacomm_GoStraightAt, "ccv1", DFLT_APP_ID1, ROOMBACOMM_GOSTRAIGHTAT_FID}, [5] = {roombacomm_GoStraightFor, "ccv1", DFLT_APP_ID1, ROOMBACOMM_GOSTRAIGHTFOR_FID}, [6] = {roombacomm_SpinLeftAt, "ccv1", DFLT_APP_ID1, ROOMBACOMM_SPINLEFTAT_FID}, [7] = {roombacomm_SpinLeftFor, "ccv1", DFLT_APP_ID1, ROOMBACOMM_SPINLEFTFOR_FID}, [8] = {roombacomm_SpinRightAt, "ccv1", DFLT_APP_ID1, ROOMBACOMM_SPINRIGHTAT_FID}, [9] = {roombacomm_SpinRightFor, "ccv1", DFLT_APP_ID1, ROOMBACOMM_SPINRIGHTFOR_FID},
    },
};


static int8_t roombacomm_MsgHandler(void *state, Message *msg)
{







    roombacomm_state_t *s = (roombacomm_state_t*)state;




    switch (msg->type){





        case MSG_INIT:
            {
                ker_led(8);
                roombacomm_Init(msg->did);
                break;
            }







        case MSG_FINAL:
            {
                break;
            }
# 90 "roombacomm.c"
        case MSG_TIMER_TIMEOUT:
            {
                (*s->pauseTimeoutFunc)();
                break;
            }

        case MSG_UART_SEND_DONE:
            {
                roombacomm_ResetBuf();
                break;
            }





        default:
            return -22;
    }




    return SOS_OK;
}

static int8_t roombacomm_Init(uint8_t pid)
{
    int8_t res;
    int i;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    for (i = 0; i<5; i++) {
        rs->uart_buf[i] = 0x0;
    }
    rs->uart_buf_len = 0;
    rs->pid = pid;
    rs->velocity = 256;
    res = ker_uart_reserve_bus(rs->pid, 0x40);

    return res;
}

static int8_t roombacomm_Start(char *p)
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if (rs->uart_buf_len != 0) {
        ker_led(7);
        return -16;
    }
    rs->uart_buf[0] = 0x80;
    rs->uart_buf_len = 1;
    res = roombacomm_Send();

    return res;
}

static int8_t roombacomm_Control(char *p)
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if (rs->uart_buf_len != 0) {
        ker_led(7);
        return -16;
    }
    rs->uart_buf[0] = 0x82;
    rs->uart_buf_len = 1;
    res = roombacomm_Send();

    return res;
}

static int8_t roombacomm_Stop(char *p)
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    res = roombacomm_Drive(0, 0);

    return res;
}

static int8_t roombacomm_Reset(char *p)
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if ((res = roombacomm_Stop(((void *)0))) != SOS_OK) {
        return res;
    }
    if ((res = roombacomm_Start(((void *)0))) != SOS_OK) {
        return res;
    }
    res = roombacomm_Control(((void *)0));

    return res;
}

static int8_t roombacomm_Drive(int16_t velocity, int16_t radius)
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if (rs->uart_buf_len != 0) {
        ker_led(7);
        return -16;
    }

    rs->uart_buf[0] = 0x89;
    rs->uart_buf[1] = velocity >> 8;
    rs->uart_buf[2] = velocity & 0x00ff;
    rs->uart_buf[3] = radius >> 8;
    rs->uart_buf[4] = radius & 0x00ff;

    rs->uart_buf_len = 5;

    res = roombacomm_Send();
    return res;
}

static int8_t roombacomm_Send()
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if ((res = ker_uart_reserve_bus(rs->pid, 0x40)) != SOS_OK) {
        return res;
    }
    res = ker_uart_send_data(rs->uart_buf, rs->uart_buf_len, rs->pid);

    return res;
}




static void roombacomm_ResetBuf()
{
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();


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
    int16_t pausetime;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    rs->straightDistance = distance;
    res = roombacomm_GoStraightSegment(((void *)0));

    return res;
}

static int8_t roombacomm_GoStraightSegment(char *p)
{
    int8_t res;
    int16_t pausetime;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if ((res = roombacomm_Drive(rs->velocity, 0x8000 - rs->straightRadius)) != SOS_OK) {
        return res;
    }
    rs->straightRadius = 1 - rs->straightRadius;

    if (rs->straightDistance > 25) {
        pausetime = __builtin_abs(25 * 1024 / rs->velocity);
        rs->straightDistance -= 25;
        res = roombacomm_Pause(pausetime, roombacomm_GoStraightSegment);
    } else {
        pausetime = __builtin_abs(rs->straightDistance * 1024 / rs->velocity);
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
    int16_t pausetime;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if ((res = roombacomm_SpinLeftAt(((void *)0), rs->velocity)) != SOS_OK) {
        return res;
    }


    if (angle<0) {
        return res;
    }

    pausetime = __builtin_abs(18 * angle * 1024 / rs->velocity);
    res = roombacomm_Pause(pausetime, roombacomm_Stop);

    return res;
}

static int8_t roombacomm_SpinRightFor(char *p, int16_t angle)
{
    int8_t res;
    int16_t pausetime;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    if ((res = roombacomm_SpinRightAt(((void *)0), rs->velocity)) != SOS_OK) {
        return res;
    }


    if (angle<0) {
        return res;
    }

    pausetime = __builtin_abs(18 * angle * 1024 / rs->velocity);
    res = roombacomm_Pause(pausetime, roombacomm_Stop);

    return res;
}

static int8_t roombacomm_Pause(int16_t ptime, int8_t (*funcptr)())
{
    int8_t res;
    roombacomm_state_t *rs = (roombacomm_state_t *)sys_get_state();

    rs->pauseTimeoutFunc = funcptr;
    res = sys_timer_start(0, ptime, TIMER_ONE_SHOT);

    return res;
}
