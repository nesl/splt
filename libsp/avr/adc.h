
#ifndef __ADC_H__
#define __ADC_H__
/**
 * Usage:
 * #include <sp_task.h>
 * #include <adc.h>
 */

void adc_start(uint8_t channel, uint16_t* buf);

#define adc_read(c,b)    \
  do{ adc_start((c), (b)); TASK_WAIT(); } while(0)
	

#endif

