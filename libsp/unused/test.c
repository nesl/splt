
#include <unistd.h>
#include <stdio.h>
#include <inttypes.h>
#include "sp_task.h"

uint32_t m;

void test_task( void ) 
{
	uint32_t *s = TASK_GET_STATE(); 
	printf("lc = %d\n", CURRENT_TASK()->lc);
	TASK_BEGIN();
	TASK_SET_STATE(&m);

	while(1) {
		TASK_WAIT();
		//ev->type = 0;
		printf("Get one event\n");
	}
	TASK_END();
}


int main()
{
	spk_init();

	sp_create( test_task, 1 );
	spk_sched();
}
