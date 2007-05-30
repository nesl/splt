#include <hardware.h>
#include <net_stack.h>
#include <message.h>
#include <malloc.h>
#include <sos_error_types.h>

#include "uart.h"

int8_t uart_system_init() {
  return SOS_OK;
}    


/**
 ****************************************
 SOS Specific Interface to the uart
 ****************************************
 */
int8_t ker_uart_reserve_bus(uint8_t calling_id, uint8_t flags) {
  return SOS_OK;
}


int8_t ker_uart_release_bus(uint8_t calling_id) {
  return SOS_OK;
}

/**
 * Send data over the uart
 *
 * if called with a sos msg (after reserving with the UART_SYS_SOS_MSG flag)
 * type cast the msg pointer to a (uint8_t*) the lower layers will handle it
 * correctly based on the flags that are set.
 */

int8_t ker_uart_send_data(
    uint8_t *buff, 
    uint8_t msg_size, 
    uint8_t calling_id) {
  return SOS_OK;
}


/**
 * Read data from the uart.
 */
int8_t ker_uart_read_data(
    uint8_t read_size,
    uint8_t calling_id) {
  return SOS_OK;
}
