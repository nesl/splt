
/**
 * Preemptive proto thread
 * #include <inttypes.h>
 * #include <sp_task.h>
 */

#ifndef __SP_TASK_H__
#define __SP_TASK_H__


//-----------------------------------------------------------------------------------------------
// Configurable parameters
//
#define SPK_MAX_TASKS     8

//-----------------------------------------------------------------------------------------------
// Typedefs
//
typedef enum {
	TASK_WAITING = 0,
	TASK_ACTIVE,
	TASK_KILLED,    
	TASK_READY,
	TASK_TIMEOUT,
} task_status_t;

typedef uint8_t pri_t;       //!< task priority type
typedef void (*task_t)(void);
typedef uint8_t sp_pid_t;
typedef int8_t  sp_ret_t;     //!< standard return value

#define NULL_PID            255

typedef struct task_cb {
	uint16_t lc;           //!< local continuation, must initialize to zero before starting task
	void*    state;        //!< task state.

//-----------------------------------------------------------------------------------------------
// Below are OS managed states
//
	sp_pid_t        pid;       //!< task ID
	task_status_t   status;    //!< WAITING (blocked), and ACTIVE (on stack), READY (queued), KILLED
	pri_t           priority;  //!< task priority
	uint8_t         errno;
	task_t          task; 
	void*           wait_obj;  //!< the object task is waiting on
	struct task_cb* next;      //!< link list for ready tasks
} task_cb;



extern  task_cb*           _current_task_;
#define CURRENT_TASK()     _current_task_
#define CURRENT_PRIORITY() (CURRENT_TASK() == NULL? 0 : CURRENT_TASK()->priority)

extern  uint8_t            spk_in_interrupt;

//--------------------------------------------------------------------------
// Task API
//
#define TASK_BEGIN()       switch(CURRENT_TASK()->lc) { case 0:
#define TASK_END()         CURRENT_TASK()->status = TASK_KILLED; return;}

#define TASK_WAIT()        CURRENT_TASK()->lc = __LINE__; sp_wait(); return; case __LINE__: 
#define TASK_WAIT_OBJECT(obj)  CURRENT_TASK()->lc = __LINE__; sp_wait_object((obj)); return; case __LINE__: 

#define TASK_SET_STATE(s)  CURRENT_TASK()->state = (s)
#define TASK_GET_STATE()   CURRENT_TASK()->state

//--------------------------------------------------------------------------
// Task Management API
//

/**
 * Create a task
 * @param  priority priority of the task, starts with 1.  Higher number means
                    higher priority.
 * @return sp_pid_t the task ID
 */
sp_pid_t sp_create( task_t task, pri_t priority );

/**
 * Signal OS to indicate event completion
 * \note sp_signal will preempt current task if the priority of pid is higher.
 *       The exception is the code in SP_INT_BEGIN, SP_INT_END. 
 *       Therefore, do necessary before calling sp_signal
 */
void sp_signal( sp_pid_t pid ); 

void sp_signal_error( sp_pid_t pid, uint8_t err );

/**
 * Put itself into TASK_WAITING state
 */
void sp_wait( void );

void sp_signal_object( void *obj );

void sp_wait_object( void *obj );

#define SP_INT_BEGIN()      do {spk_in_interrupt = 1;} while(0)
#define SP_INT_END()        \
	do {spk_in_interrupt = 0; spk_schedule_task(); } while(0)

//--------------------------------------------------------------------------
// Kernel API (spk prefix)
//

void spk_init( void );

/**
 * OS scheduler loop.  
 * This will never return.
 */ 
void spk_sched( void );

void spk_schedule_task( void );

//--------------------------------------------------------------------------
// Errno
//
enum {
	EBUSY = 1,
	ENOMEM = 2,
};

//--------------------------------------------------------------------------
// Platform specific routines
//
#ifdef PLATFORM_AVR
#include <avr/io.h>
#define DISABLE_INT()   asm volatile("cli")
#define ENABLE_INT()    asm volatile("sei")
#define HAS_CRITICAL_SECTION       register uint8_t _prev_
#define ENTER_CRITICAL_SECTION()  \
asm volatile ( \
    "in %0, __SREG__"   "\n\t" \
    "cli"               "\n\t" \
    : "=r" (_prev_) \
    : )

#define LEAVE_CRITICAL_SECTION() \
asm volatile ( \
    "out __SREG__, %0"   "\n\t" \
    : \
    : "r" (_prev_) )

/*
#define ENTER_CRITICAL_SECTION_WITH_VAL(v)  \
asm volatile ( \
    "in %0, __SREG__"   "\n\t" \
    "cli"               "\n\t" \
    : "=r" ((v)) \
    : )

#define LEAVE_CRITICAL_SECTION_WITH_VAL(v) \
asm volatile ( \
    "out __SREG__, %0"   "\n\t" \
    : \
    : "r" ((v)) )
#define LEAVE_CRITICAL_SECTION_WITH_VAL(v) \
	do{ if(((v) & 0x80) == 0x80) asm volatile("sei"); } while(0)

#define LEAVE_CRITICAL_SECTION() \
	do { if((_prev_ & 0x80) == 0x80) asm volatile("sei"); } while(0)
	*/

#define SLEEP()                     \
do {                                \
    MCUCR |= (1 <<(SE));            \
    asm volatile ("sei");           \
    asm volatile ("sleep");         \
    asm volatile ("nop");           \
    asm volatile ("nop");           \
} while(0)

#define SP_EXCEPTION()  
#endif

#ifdef PLATFORM_UNIX

void plat_unix_disable_int();
void plat_unix_enable_int();
void plat_unix_sleep();
void plat_unix_exception();

#define DISABLE_INT()   plat_unix_disable_int()
#define ENABLE_INT()    plat_unix_enable_int()
#define SLEEP()         plat_unix_sleep()
#define SP_EXCEPTION()  plat_unix_exception()
#endif    

#endif


