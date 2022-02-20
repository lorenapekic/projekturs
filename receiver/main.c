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
#include "lcd.h"

#define BAUD 2400//9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

void writeLCDnovi(uint8_t ispis){
	lcd_clrscr();
	char ispisS[16];
	itoa(ispis, ispisS, 10);
	lcd_puts(ispisS);
}

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
	DDRD = _BV(4) | _BV(0);
	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 64;

	unsigned char a;
	char buffer[10];
	uart_init();
	lcd_init(LCD_DISP_ON);
	lcd_home();
	
    /* RECEIVER */
    while (1)  {

		a = uart_receive();
		itoa(a, buffer, 10);
		lcd_clrscr();
		lcd_home();
		lcd_puts(buffer);
		_delay_ms(1000);
	
    }
}

