#include <avr/io.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <uart_hal.h>

uint8_t counter;
uint8_t state = 0;
uint8_t foo;

//BAUD_9600
uint8_t baud = 47;

void Foo_trans(uint8_t data)
{
	while(!( UCSR1A & (1<<UDRE)) );
	UDR1 = data;
}

void Foo0_trans(uint8_t data)
{
	while(!( UCSR0A & (1<<UDRE)) );
	UDR0 = data;
}

void USART1_init(uint8_t baud)
{
	/* Set Baud rate */
	UBRR1H = (uint8_t) (baud>>8);
	UBRR1L = (uint8_t) baud;

	/* Enable Receiver and Transmitter */
	UCSR1B = (1<<RXEN) | (1<<TXEN);
	/*Set Frame Format 8N1*/
	UCSR1C = (1<<UCSZ1) | (1<<UCSZ0);
}	

void USART0_init(uint8_t baud)
{
	UBRR0H = (uint8_t) (baud>>8);
	UBRR0L = (uint8_t) baud;

	UCSR0B = (1<<RXEN) | (1<<TXEN);
	UCSR0C = (1<<UCSZ1) | (1<<UCSZ0);
}

int main () {
  //Initialize
  //This sets it to a baud rate of 9600  
  
  counter = 0x40;
  sei();
  USART0_init(baud);
  while(1)
  {
	Foo0_trans(foo);
  }
	  
  return 0;

  
}


SIGNAL(SIG_UART_RECV) {
  //counter = UDR1;
  foo = UDR0;
  if(state != 1)
    state = 1;
}

/*
SIGNAL(SIG_UART_TRANS) {
  PORTA = 0x22;
  UDR0 = counter++;
}
*/

/*
SIGNAL(SIG_UART_RECV) {
  PORTA = UDR0;
}
*/

