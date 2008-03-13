#include <avr/io.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <led.h>


#define BAUD_9600 47
#define BAUD_115k 3

uint8_t foo0 = 0;
uint8_t foo1 = 0;

//BAUD_9600
//uint8_t BAUD = 47; // 95 2x
//uint8_t BAUD = 3;


int main () {
  //Initialize
  //This sets it to a baud rate of 9600  
  
//  char x = 0;
  EICRA = (1<<ISC00)|(1<<ISC01);
  EIMSK |= (1<<INT0)|(1<<INT1);
  MCUCR |= (1<<ISC01)|(1<<ISC00);
  sei();
  led_init();
  led_red_off();
  led_yellow_off();
  while(1);
	  
  return 0;

  
}


SIGNAL(SIG_INTERRUPT0) {
  led_red_toggle();
}
SIGNAL(SIG_INTERRUPT1)
{
led_green_toggle();
}
