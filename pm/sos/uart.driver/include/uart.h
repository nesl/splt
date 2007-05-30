#ifndef _UART_H
#define _UART_H

int8_t uart_system_init();

#ifndef _MODULE_
/**
 * @brief UART HPL Related Functions
 */
int8_t ker_uart_reserve_bus(uint8_t calling_id, uint8_t flags);
int8_t ker_uart_release_bus(uint8_t calling_id);

int8_t ker_uart_send_data(
        uint8_t *buff, 
        uint8_t msg_size, 
        uint8_t calling_id);
int8_t ker_uart_read_data(
        uint8_t read_size, 
        uint8_t calling_id);
#endif
#endif // _UART_SYSTEM_H

