#include <avr/io.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <led.h>


#define BAUD_9600 47

uint8_t foo0 = 0;
uint8_t foo1 = 0;

//BAUD_9600
uint8_t BAUD = 47; // 95 2x

void Foo1_trans(uint8_t data)
{
	while(!( UCSR1A & (1<<UDRE1)) );
	UDR1 = data;
}

void Foo0_trans(uint8_t data)
{
	while(!( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

void USART1_init(void)
{
	UBRR1H = (uint8_t) (BAUD_9600>>8);
	UBRR1L = (uint8_t) (BAUD_9600);
	UCSR1C = (1<<UCSZ1) | (1<<UCSZ0);
//	UCSR1A = (1 << U2X);
/**
 ** Enable reciever and transmitter and their interrupts
 ** transmit interrupt will be disabled until there is
 ** packet to send.
 **/

        UCSR1B = ((1 << RXCIE) | (1 << RXEN) | (1 << TXEN));
	
	//UCSR0B = (1<<RXEN) | (1<<TXEN);
}	

void USART0_init(void)
{
	UBRR0H = (uint8_t) (BAUD_9600>>8);
	UBRR0L = (uint8_t) (BAUD_9600);
	UCSR0C = (1<<UCSZ1) | (1<<UCSZ0);
//	UCSR0A = (1 << U2X);
/**
 ** Enable reciever and transmitter and their interrupts
 ** transmit interrupt will be disabled until there is
 ** packet to send.
 **/

        UCSR0B = ((1 << RXCIE) | (1 << RXEN) | (1 << TXEN));
	
	// UCSR0B = (1<<RXEN) | (1<<TXEN);
}

int main () {
  //Initialize
  //This sets it to a baud rate of 9600  
  
  char x = 0;
  USART0_init();
  USART1_init();
  sei();
  led_init();
  led_red_on();
  led_yellow_on();
  x = 0x0d;
  Foo1_trans(x);
  x = 0x30;
  Foo1_trans(x);
  x = 0x31;
  Foo1_trans(x);
  Foo1_trans(x);
  x = 0x34;
  Foo1_trans(x);
  x = 0x30;
  Foo1_trans(x);
  x = 0x31;
  Foo1_trans(x);
  x = 0x0d;
  Foo1_trans(x);
  
  while(1);
 // {
//	Foo0_trans(foo0);
  //}
	  
  return 0;

  
}


SIGNAL(SIG_USART0_RECV) {
  foo0 = UDR0;
  Foo1_trans(foo0);
}

SIGNAL(SIG_USART1_RECV) {
	foo1 = UDR1;
	Foo0_trans(foo1);
	led_yellow_toggle();
}

/*
SIGNAL(SIG_UART0_TRANS) {
  while(!( UCSR0A & (1<<UDRE0)) );
  UDR0='a';
}

*/
