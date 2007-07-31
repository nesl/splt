
#include <sos.h>
/**
 * Must also include the header file that defines the 
 * blink_get_header()
 */

//mod_header_ptr blink_get_header();
//mod_header_ptr neighbor_get_header();
//mod_header_ptr tree_routing_get_header();
mod_header_ptr rfid_get_header();
mod_header_ptr uart_mod_get_header();

/**
 * application start
 * This function is called once at the end od SOS initialization
 */
void sos_start(void)
{
//ker_register_module(blink_get_header());
//ker_register_module(neighbor_get_header());
//ker_register_module(tree_routing_get_header());
ker_register_module(rfid_get_header());
ker_register_module(uart_mod_get_header());
}
