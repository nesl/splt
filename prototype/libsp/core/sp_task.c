#include <stdlib.h>
#include <inttypes.h>
#include "sp_task.h"

//--------------------------------------------------------------------------------
// Choice of scheduling policy
//

//--------------------------------------------------------------------------------
// Start of SPK
//
task_cb*        _current_task_;
static task_cb *spk_ready_list;
static uint8_t  spk_num_tasks;

static task_cb  spk_tasks[SPK_MAX_TASKS];
uint8_t         spk_in_interrupt;


/**
 * Task ready list
 * This stores the list of tasks that are ready but cannot execute.
 * The tasks are stored according to its priority.
 */


void spk_init( void )
{
	sp_pid_t i;
	CURRENT_TASK() = NULL;
	spk_ready_list = NULL;
	for( i = 0; i < SPK_MAX_TASKS; i++ ) {
		spk_tasks[i].task   = NULL;
		spk_tasks[i].pid    = i;
	}
	
	spk_in_interrupt = 1;
	spk_num_tasks = 0;
}

//
// Assume locking.
//
static void spk_queue_task( sp_pid_t pid )
{
	task_cb *itr = spk_ready_list;
	task_cb *prev = NULL;
	task_cb *task = &spk_tasks[pid];
	
	if( spk_tasks[pid].status != TASK_WAITING ) {
		return;
	}
	spk_tasks[pid].status = TASK_READY;
	// 
	// Cannot dispatch task, put into ready list 
	//
	while( itr != NULL ) {
		if( itr->priority < task->priority ) {
			break;
		}
		prev = itr;
		itr = itr->next;
	}
	if( prev == NULL ) {
		task->next     = itr;
		spk_ready_list = task;
	} else {
		task->next     = itr;
		prev->next     = task;
	}
	spk_num_tasks++;
}

//
// Assume locking
//
static inline task_cb* spk_dequeue_task( void )
{
	register task_cb* task = spk_ready_list;
	spk_ready_list = spk_ready_list->next;
	task->next = NULL;
	spk_num_tasks--;
	return task;
}

//
// Assume interrupt is disabled
// When this function returns, the interrupt is always enabled because 
// spk_schedule_task().
//
static void spk_dispatch( task_cb* task )
{
	task_cb* prev;
	
	prev = CURRENT_TASK();
	CURRENT_TASK() = task;
	task->status   = TASK_ACTIVE;
	task->wait_obj = NULL;
	ENABLE_INT();

	
	(task->task)();
	
	DISABLE_INT();
	CURRENT_TASK() = prev;
	
	//
	// Dispatch task that has higher priority
	//
	spk_schedule_task();
}


//
// Assume interrupt is disabled
// When this function returns, the interrupt is always enabled.
//
void spk_schedule_task( void )
{
	if( (spk_ready_list != NULL) && (spk_ready_list->priority > CURRENT_PRIORITY()) ) {
		task_cb* task = spk_dequeue_task();
		spk_dispatch(task);
	}  
	ENABLE_INT();
}

void spk_sched( void )
{
  spk_in_interrupt = 0;
  ENABLE_INT();
  while( 1 ) {
    DISABLE_INT();
    //if( spk_ready_list != NULL ) {
    if( spk_num_tasks != 0 ) {
      task_cb* task = spk_dequeue_task();
      spk_dispatch(task);
    } else {
      SLEEP();
    } 
  }
}

//--------------------------------------------------------------------------------
// Task Management API
//

sp_pid_t sp_create( task_t task, uint8_t priority )
{
  sp_pid_t i;
  HAS_CRITICAL_SECTION;
  
  ENTER_CRITICAL_SECTION();
  // Search for empty slot
  for( i = 0; i < SPK_MAX_TASKS; i++ ) {
    // Found an empty slot
    if( spk_tasks[i].task == NULL ) {
      spk_tasks[i].lc       = 0;
      // The pid field is not being used
      spk_tasks[i].status   = TASK_WAITING;
      spk_tasks[i].priority = priority;
      spk_tasks[i].errno    = 0;
      spk_tasks[i].task     = task;
      spk_tasks[i].wait_obj = NULL;
      spk_tasks[i].next     = NULL;
      LEAVE_CRITICAL_SECTION();
      sp_signal( i );
      return i;
    }
  }
  // All slots taken
  LEAVE_CRITICAL_SECTION();
  SP_EXCEPTION();
  return 0;
}


void sp_signal( sp_pid_t pid )
{
  HAS_CRITICAL_SECTION;

  // Check for null pid
  if( pid == NULL_PID ) return;
  
  ENTER_CRITICAL_SECTION();
  if((spk_in_interrupt == 0) && 
     (spk_tasks[pid].priority > CURRENT_PRIORITY()) ) {
    spk_dispatch(&(spk_tasks[pid]));
    return;
  } else {
    spk_queue_task( pid );
  } 
  LEAVE_CRITICAL_SECTION();
}

void sp_signal_error( sp_pid_t pid, uint8_t err )
{
	HAS_CRITICAL_SECTION;
	
	ENTER_CRITICAL_SECTION();
	if( spk_tasks[pid].status != TASK_WAITING ) {
		LEAVE_CRITICAL_SECTION();
		return;
	}
	spk_tasks[pid].errno = err;
	LEAVE_CRITICAL_SECTION();
	sp_signal( pid );
}

void sp_wait( void ) 
{
	HAS_CRITICAL_SECTION;
	
	ENTER_CRITICAL_SECTION();
	CURRENT_TASK()->errno = 0;
	CURRENT_TASK()->status = TASK_WAITING;
	LEAVE_CRITICAL_SECTION();
}

void sp_signal_object( void* obj )
{
	register sp_pid_t i;
	HAS_CRITICAL_SECTION;
	
	ENTER_CRITICAL_SECTION();
	for( i = 0; i < SPK_MAX_TASKS; i++ ) {
		if( spk_tasks[i].wait_obj == obj ) {
			spk_queue_task( i );
		}
	}
	if( spk_in_interrupt == 0 ) {
		spk_schedule_task();
		return;
	}
	LEAVE_CRITICAL_SECTION();
}

void sp_wait_object( void* obj )
{
	HAS_CRITICAL_SECTION;
	
	ENTER_CRITICAL_SECTION();
	CURRENT_TASK()->errno = 0;
	CURRENT_TASK()->status = TASK_WAITING;
	CURRENT_TASK()->wait_obj = obj;
	LEAVE_CRITICAL_SECTION();
}



