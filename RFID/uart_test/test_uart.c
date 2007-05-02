#include <avr/io.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

uint8_t counter;
uint8_t state = 0;
uint8_t baud = 47;

void Foo_trans(uint8_t data)
{
	while(!( UCSR1A & (1<<UDRE)) );
	UDR1 = data;
}


int main () {
  //Initialize
  //This sets it to a baud rate of 9600  
  UBRR1H = baud>>8;
  UBRR1L = baud;

  counter = 0x01;

  sei();
  //UCR = 0x00;
  //UCR = (1 << TXEN) | (1 << TXCIE) | (1 << RXCIE) | (1 << RXEN);
  // enable receiver and transmitter
  UCSR1B = (1<<RXEN) | (1<<TXEN);
  
  //set frame format
  UCSR1C = (1<<UCSZ1) | (1<<UCSZ0);

  while(1)
  {
	Foo_trans(counter);
  }
	  
  return 0;

  
}

/*
SIGNAL(SIG_UART_RECV) {
  counter = UDR;
  if(state != 1)
    state = 1;
}
*/
/*
SIGNAL(SIG_UART_TRANS) {
  PORTB = 0x22;
  UDR = counter++;
}
*/
/*
SIGNAL(SIG_UART_RECV) {
  PORTA = UDR;
}
*/

