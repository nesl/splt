/* -*- Mode: C; tab-width:4 -*- */
/* ex: set ts=4 shiftwidth=4 softtabstop=4 cindent: */
/**
 * @brief Example module for SOS
 * This module shows some of concepts in SOS
 */

/**
 * Module needs to include <module.h>
 */
//#undef _MODULE_

#include <sys_module.h>

//#define LED_DEBUG
#include <led_dbg.h>

#include "WaterVib.h"

#define BLINK_TIMER_INTERVAL	1024L
#define BLINK_TID               1
/**
 * Module can define its own state
 */
typedef struct {
  uint8_t pid;
  uint8_t state;
} app_state_t;

static int8_t WaterVib_msg_handler(void *start, Message *e);

uint8_t values[8] = {1,2,3,4,5,6,7,8};

static double var(int count, uint8_t data[])
{
	double mean = 0;
	double variance = 0;
	int i=0;
	for (i=0;i<count;i++)
	{ 
		  mean = mean+data[i];
	} 
	  mean = mean/count;

	  for (i=0;i<count;i++)
	  { 
		    variance = variance+(data[i]-mean)*(data[i]-mean);
	  } 
	    variance = variance/(count-1);

		return variance;
}



static const mod_header_t mod_header SOS_MODULE_HEADER = {
	.mod_id         = DFLT_APP_ID1,
	.state_size     = sizeof(app_state_t),
	.num_sub_func   = 0,
	.num_prov_func  = 0,
	.platform_type  = HW_TYPE /* or PLATFORM_ANY */,
	.processor_type = MCU_TYPE,
	.code_id        = ehtons(DFLT_APP_ID0),
	.module_handler = WaterVib_msg_handler,
};


static int8_t WaterVib_msg_handler(void *state, Message *msg)
{
  app_state_t *s = (app_state_t*)state;

  switch (msg->type){
  case MSG_INIT:
  {
	  s->pid = msg->did;
	  s->state = 0;
	  sys_led(LED_RED_TOGGLE);
	  DEBUG("Blink Start\n");
	  sys_timer_start(BLINK_TID, BLINK_TIMER_INTERVAL, TIMER_REPEAT);
	  break;
	}


  case MSG_FINAL:
	{
	  sys_timer_stop(BLINK_TID);
      sys_led(LED_GREEN_OFF);
      sys_led(LED_RED_OFF);
	  DEBUG("Blink Stop\n");
	  break;
	}


  case MSG_TIMER_TIMEOUT:
	{
	  sys_led(LED_GREEN_TOGGLE);
      double *variance = sys_malloc(sizeof(double));
	  variance[0] = var(8,values);
	  sys_post_uart(msg->did,MSG_TIMER_TIMEOUT,sizeof(double),(void*)variance,SOS_MSG_RELEASE,BCAST_ADDRESS);
	  break;
	}

	default:
	return -EINVAL;
  }

  return SOS_OK;
}

#ifndef _MODULE_
mod_header_ptr WaterVib_get_header()
{
  return sos_get_header_address(mod_header);
}
#endif


