
#ifndef __SP_SEMAPHORE_H__
#define __SP_SEMAPHORE_H__

/**
 * Usage: 
 *   #include <sp_task.h>
 *   #include <sp_semaphore.h>
 */

typedef struct sp_sem_t {
	sp_pid_t       pid;      //!< the task that is waiting for the event
	uint8_t        cnt;      //!< number of events
	//uint8_t        sreg;
} sp_sem_t;

#define TASK_SEM_INIT(s,c)                        \
  do{ (s)->cnt = c; } while(0)
  
  /*
#define TASK_SEM_WAIT(s)                          \
  do {                                            \
	ENTER_CRITICAL_SECTION_WITH_VAL((s)->sreg);                     \
	if( (s)->cnt == 0 ) {                         \
		(s)->pid = CURRENT_TASK()->pid;           \
		LEAVE_CRITICAL_SECTION_WITH_VAL((s)->sreg);                 \
		TASK_WAIT();                              \
	    ENTER_CRITICAL_SECTION_WITH_VAL((s)->sreg);                 \
	}                                             \
	--(s)->cnt;                                   \
	LEAVE_CRITICAL_SECTION_WITH_VAL((s)->sreg);                     \
  } while(0)

#define TASK_SEM_SIGNAL(s)                        \
  do{                                             \
	ENTER_CRITICAL_SECTION_WITH_VAL((s)->sreg);                     \
    ++(s)->cnt;                                   \
	if( (s)->cnt == 1 ) {                         \
	  LEAVE_CRITICAL_SECTION_WITH_VAL((s)->sreg);                     \
	  sp_signal( (s)->pid );                      \
	} else {                                      \
	  LEAVE_CRITICAL_SECTION_WITH_VAL((s)->sreg);                     \
	}                                             \
  } while(0)
  */
#define TASK_SEM_WAIT(s)                          \
  do {                                            \
	HAS_CRITICAL_SECTION;                         \
	ENTER_CRITICAL_SECTION();                     \
	if( (s)->cnt == 0 ) {                         \
		(s)->pid = CURRENT_TASK()->pid;           \
		LEAVE_CRITICAL_SECTION();                 \
		TASK_WAIT();                              \
	    ENTER_CRITICAL_SECTION();                 \
	}                                             \
	--(s)->cnt;                                   \
	LEAVE_CRITICAL_SECTION();                     \
  } while(0)

#define TASK_SEM_SIGNAL(s)                        \
  do{                                             \
	HAS_CRITICAL_SECTION;                         \
	ENTER_CRITICAL_SECTION();                     \
    ++(s)->cnt;                                   \
	if( (s)->cnt == 1 ) {                         \
	  LEAVE_CRITICAL_SECTION();                   \
	  sp_signal( (s)->pid );                      \
	} else {                                      \
	  LEAVE_CRITICAL_SECTION();                   \
	}                                             \
  } while(0)


#endif

