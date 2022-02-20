#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <avr/interrupt.h>

#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

void uart_init(void){
	UBRRH = (BAUDRATE>>8);
	UBRRL = BAUDRATE;
	UCSRB |= (1<<TXEN) | (1 << RXEN);
	UCSRC |= (1<<URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

//function to send data
void uart_transmit(unsigned char data){
	while (!(UCSRA & (1 << UDRE)));
	UDR = data;
}

//function to receive data
unsigned char uart_receive(void){
	while(!(UCSRA) & (1 << RXC));
	return UDR;
}



int main(void)
{
	DDRD = _BV(0);
	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 64;
	uart_init();

	/* TRANSMITTER */
	while (1)  {

		unsigned char proba = 12;
		uart_transmit(proba);
		_delay_ms(1000);

	}
}
