#ifndef _ROOMBACOMM_H_
#define _ROOMBACOMM_H_

#define ROOMBACOMM_PID DFLT_APP_ID1
#define UART_BUF_SIZE 7
#define START 0x80
#define CONTROL 0x82
#define FULL_CONTROL 0x84
#define DRIVE 0x89
#define SONG 0x8C
#define PLAY 0x8D
#define SENSORS 0x8E
#define SENSORS_ALL 0
#define SENSORS_PKT_SIZE 26

// offsets into sensor_bytes data
#define BUMPSWHEELDROPS     0
#define WALL                1
#define CLIFFLEFT           2
#define CLIFFFRONTLEFT      3
#define CLIFFFRONTRIGHT     4
#define CLIFFRIGHT          5
#define VIRTUALWALL         6
#define MOTOROVERCURRENTS   7
#define DIRTLEFT            8
#define DIRTRIGHT           9
#define REMOTEOPCODE        10
#define BUTTONS             11
#define DISTANCE_HI         12
#define DISTANCE_LO         13
#define ANGLE_HI            14
#define ANGLE_LO            15
#define CHARGINGSTATE       16
#define VOLTAGE_HI          17
#define VOLTAGE_LO          18
#define CURRENT_HI          19
#define CURRENT_LO          20
#define TEMPERATURE         21
#define CHARGE_HI           22
#define CHARGE_LO           23
#define CAPACITY_HI         24
#define CAPACITY_LO         25

// bitmasks for various thingems
#define WHEELDROP_MASK      0x1C
#define BUMP_MASK           0x03
#define BUMPRIGHT_MASK      0x01
#define BUMPLEFT_MASK       0x02
#define WHEELDROPRIGHT_MASK 0x04
#define WHEELDROPLEFT_MASK  0x08
#define WHEELDROPCENT_MASK  0x10

#define MOVERDRIVELEFT_MASK 0x10
#define MOVERDRIVERIGHT_MASK 0x08
#define MOVERMAINBRUSH_MASK 0x04
#define MOVERVACUUM_MASK    0x02
#define MOVERSIDEBRUSH_MASK 0x01

#define POWERBUTTON_MASK    0x08
#define SPOTBUTTON_MASK     0x04
#define CLEANBUTTON_MASK    0x02
#define MAXBUTTON_MASK      0x01


/** distance between wheels on the roomba, in millimeters */
//To make things easier to compute, values have been hardcoded
//To make things non float, values have been pre-multiplied by 8
//#define WHEELBASE = (258<<3);
#define WHEELBASE 2064
/** mm/deg is circumference distance divided by 360 degrees */
//#define MILLIMETERSPERDEGREE = (WHEELBASE * PI / 360);
#define MILLIMETERSPERDEGREE 18
/** mm/rad is a circumference distance divied by two pi */
//#define MILLIMETERSPERRADIAN = (float)(WHEELBASE/2);
#define MILLIMETERSPERRADIAN 1032
/** default speed for movement operations if
 * speed isn't specified */
#define DEFAULTVELOCITY  256
#define MILLIMETERSPERGRID 200

#define ROOMBACOMM_PAUSE_TID               0
#define ROOMBACOMM_UART_BACKOFF_TID        1
#define ROOMBACOMM_UART_BACKOFF_INTERVAL   5
#define ROOMBACOMM_SENSORS_TID             2
#define ROOMBACOMM_SENSORS_INTERVAL        100
#define STRAIGHTSEGMENT                    25

typedef struct {
    int16_t velocity;
    uint8_t pid;
    uint8_t state;
    uint8_t uart_buf[UART_BUF_SIZE];
    uint8_t uart_buf_len;
    uint8_t uart_bkp_buf[UART_BUF_SIZE];
    uint8_t uart_bkp_buf_len;
    uint8_t straightRadius;
    uint16_t straightDistance;
    uint8_t moving;
    uint8_t sensor_bytes[SENSORS_PKT_SIZE];
    int8_t (*pauseTimeoutFunc)(char *);
} roombacomm_state_t;

enum {
    ROOMBACOMM_START_FID = 0,
    ROOMBACOMM_CONTROL_FID = 1,
    ROOMBACOMM_STOP_FID = 2,
    ROOMBACOMM_RESET_FID = 3,
    ROOMBACOMM_GOSTRAIGHTAT_FID = 4,
    ROOMBACOMM_GOSTRAIGHTFOR_FID = 5,
    ROOMBACOMM_SPINLEFTAT_FID = 6,
    ROOMBACOMM_SPINLEFTFOR_FID = 7,
    ROOMBACOMM_SPINRIGHTAT_FID = 8,
    ROOMBACOMM_SPINRIGHTFOR_FID = 9,
    ROOMBACOMM_MOVING_FID = 10,
    ROOMBACOMM_PLAYNOTE_FID = 11,
    ROOMBACOMM_BUMPSENSOR_FID = 12,
    ROOMBACOMM_CHANGEVELOCITY_FID = 13
};

enum {
    ROOMBA_STOPPED = 0,
    ROOMBA_SPINNING = 1,
    ROOMBA_DRIVING = 2
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
typedef int8_t (*roombacomm_Moving_proto)(func_cb_ptr, uint8_t *);
typedef int8_t (*roombacomm_PlayNote_proto)(func_cb_ptr, uint8_t, uint8_t);
typedef int8_t (*roombacomm_BumpSensor_proto)(func_cb_ptr, uint8_t *);
typedef int8_t (*roombacomm_ChangeVelocity_proto)(func_cb_ptr, int16_t);

#define NUM_FUNC    14
#define FUNC_HDR_BLOCK \
    func_cb_ptr roombacomm_Start; \
func_cb_ptr roombacomm_Control; \
func_cb_ptr roombacomm_Stop; \
func_cb_ptr roombacomm_Reset; \
func_cb_ptr roombacomm_GoStraightAt; \
func_cb_ptr roombacomm_GoStraightFor; \
func_cb_ptr roombacomm_SpinLeftAt; \
func_cb_ptr roombacomm_SpinLeftFor; \
func_cb_ptr roombacomm_SpinRightAt; \
func_cb_ptr roombacomm_SpinRightFor; \
func_cb_ptr roombacomm_Moving; \
func_cb_ptr roombacomm_PlayNote; \
func_cb_ptr roombacomm_BumpSensor; \
func_cb_ptr roombacomm_ChangeVelocity;  


#define FUNC_PRV_BLOCK \
    [0] = {roombacomm_Start, "cvv0", ROOMBACOMM_PID, ROOMBACOMM_START_FID}, \
[1] = {roombacomm_Control, "cvv0", ROOMBACOMM_PID, ROOMBACOMM_CONTROL_FID}, \
[2] = {roombacomm_Stop, "cvv0", ROOMBACOMM_PID, ROOMBACOMM_STOP_FID}, \
[3] = {roombacomm_Reset, "cvv0", ROOMBACOMM_PID, ROOMBACOMM_RESET_FID}, \
[4] = {roombacomm_GoStraightAt, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_GOSTRAIGHTAT_FID}, \
[5] = {roombacomm_GoStraightFor, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_GOSTRAIGHTFOR_FID},\
[6] = {roombacomm_SpinLeftAt, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_SPINLEFTAT_FID}, \
[7] = {roombacomm_SpinLeftFor, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_SPINLEFTFOR_FID}, \
[8] = {roombacomm_SpinRightAt, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_SPINRIGHTAT_FID}, \
[9] = {roombacomm_SpinRightFor, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_SPINRIGHTFOR_FID}, \
[10] = {roombacomm_Moving, "cDv1", ROOMBACOMM_PID, ROOMBACOMM_MOVING_FID}, \
[11] = {roombacomm_PlayNote, "cCC2", ROOMBACOMM_PID, ROOMBACOMM_PLAYNOTE_FID}, \
[12] = {roombacomm_BumpSensor, "cDv1", ROOMBACOMM_PID, ROOMBACOMM_BUMPSENSOR_FID}, \
[13] = {roombacomm_ChangeVelocity, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_CHANGEVELOCITY_FID}, 

#define FUNC_USE_BLOCK \
    [0] = {error_8, "cvv0", ROOMBACOMM_PID, ROOMBACOMM_START_FID}, \
[1] = {error_8, "cvv0", ROOMBACOMM_PID, ROOMBACOMM_CONTROL_FID}, \
[2] = {error_8, "cvv0", ROOMBACOMM_PID, ROOMBACOMM_STOP_FID}, \
[3] = {error_8, "cvv0", ROOMBACOMM_PID, ROOMBACOMM_RESET_FID}, \
[4] = {error_8, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_GOSTRAIGHTAT_FID}, \
[5] = {error_8, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_GOSTRAIGHTFOR_FID},\
[6] = {error_8, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_SPINLEFTAT_FID}, \
[7] = {error_8, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_SPINLEFTFOR_FID}, \
[8] = {error_8, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_SPINRIGHTAT_FID}, \
[9] = {error_8, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_SPINRIGHTFOR_FID}, \
[10] = {error_8, "cDv1", ROOMBACOMM_PID, ROOMBACOMM_MOVING_FID}, \
[11] = {error_8, "cCC2", ROOMBACOMM_PID, ROOMBACOMM_PLAYNOTE_FID}, \
[12] = {error_8, "cDv1", ROOMBACOMM_PID, ROOMBACOMM_BUMPSENSOR_FID}, \
[13] = {error_8, "cSv1", ROOMBACOMM_PID, ROOMBACOMM_CHANGEVELOCITY_FID}, 

#define ROOMBACOMM_START(s) SOS_CALL((s)->roombacomm_Start, roombacomm_Start_proto)
#define ROOMBACOMM_CONTROL(s) SOS_CALL((s)->roombacomm_Control, roombacomm_Control_proto)
#define ROOMBACOMM_STOP(s) SOS_CALL((s)->roombacomm_Stop, roombacomm_Stop_proto)
#define ROOMBACOMM_RESET(s) SOS_CALL((s)->roombacomm_Reset, roombacomm_Reset_proto)
#define ROOMBACOMM_GOSTRAIGHTAT(s, i) SOS_CALL((s)->roombacomm_GoStraightAt, roombacomm_GoStraightAt_proto, (i))
#define ROOMBACOMM_GOSTRAIGHTFOR(s, i) SOS_CALL((s)->roombacomm_GoStraightFor, roombacomm_GoStraightFor_proto, (i))
#define ROOMBACOMM_SPINLEFTAT(s, i) SOS_CALL((s)->roombacomm_SpinLeftAt, roombacomm_SpinLeftAt_proto, (i))
#define ROOMBACOMM_SPINLEFTFOR(s, i) SOS_CALL((s)->roombacomm_SpinLeftFor, roombacomm_SpinLeftFor_proto, (i))
#define ROOMBACOMM_SPINRIGHTAT(s, i) SOS_CALL((s)->roombacomm_SpinRightAt, roombacomm_SpinRightAt_proto, (i))
#define ROOMBACOMM_SPINRIGHTFOR(s, i) SOS_CALL((s)->roombacomm_SpinRightFor, roombacomm_SpinRightFor_proto, (i))
#define ROOMBACOMM_MOVING(s, i) SOS_CALL((s)->roombacomm_Moving, roombacomm_Moving_proto, (i))
#define ROOMBACOMM_PLAYNOTE(s, i, j) SOS_CALL((s)->roombacomm_PlayNote, roombacomm_PlayNote_proto, (i), (j))
#define ROOMBACOMM_BUMPSENSOR(s, i) SOS_CALL((s)->roombacomm_BumpSensor, roombacomm_BumpSensor_proto, (i))
#define ROOMBACOMM_CHANGEVELOCITY(s, i) SOS_CALL((s)->roombacomm_ChangeVelocity, roombacomm_ChangeVelocity_proto, (i))

#endif

