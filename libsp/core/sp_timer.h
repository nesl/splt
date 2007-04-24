
#ifndef __SP_TIMER_H__
#define __SP_TIMER_H__

/**
 * Usage:
 * #include <sp_task.h>
 * #include <sp_semaphore.h>
 * #include <sp_timer.h>
 */

typedef uint16_t sp_timeval_t;
/**
 * Timer Control Block
 */
typedef struct sp_timer_t {
	sp_sem_t             fd;
	sp_timeval_t         ival; 
	sp_timeval_t         rval;
	struct sp_timer_t*   next;
} sp_timer_t;

void sp_timer_init( void );

/**
 * Open a periodic timer with predefined period
 */
#define sp_timer_open(f,p) sp_timer_start((f), (p), (p))

void sp_timer_close( sp_timer_t* f );

#define sp_sleep(f,p)     \
	do{ sp_timer_start((f), (p), 0);  TASK_SEM_WAIT((sp_sem_t*)(f)); } while(0)
	
#define sp_timer_read(f)    \
    do{ TASK_SEM_WAIT((sp_sem_t*)(f)); } while(0)

void sp_timer_start( sp_timer_t *cb, sp_timeval_t ival, sp_timeval_t rval );

#endif

