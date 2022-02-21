/*
 * key_pad.c
 *
 * Created: 17.02.2022. 20:17:14
 * Author : ilkri
 */

/*
KeyPad pin layout:
	o		o		o		o		o		o		o
	col2	row1	col1	row4	col3	row3	row2
	|		|		|		|		|		|		|		
	|		|		|		|		|		|		|		
	PA1		PA3		PA0		PA6		PA2		PA5		PA4
	
###############################################################################
	
lcd.h on PORTD

###############################################################################

1 --> 0000 0001
2 --> 0000 0010
3 --> 0000 0011

4 --> 0000 0100
5 --> 0000 0101
6 --> 0000 0110

7 --> 0000 0111
8 --> 0000 1000
9 --> 0000 1001

* --> 0000 1111
0 --> 0000 0000
# --> 1111 0000
*/

#define F_CPU 8000000UL

#define DDRx DDRA
#define PORTx PORTA
#define PINx PINA

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



uint8_t digit[4][3] = {
	{1, 2, 3},
	{4, 5, 6},
	{7, 8, 9},
	{0x0F, 0, 0xF0}
};

// iskoristen kod s interneta samo za prikaz na LCD
char *convert(uint8_t *a) {
	char* buffer2;
	int i;

	buffer2 = malloc(9);
	if (!buffer2) return NULL;

	buffer2[8] = 0;
	for (i = 0; i <= 7; i++) buffer2[7 - i] = (((*a) >> i) & (0x01)) + '0';

	puts(buffer2);
	return buffer2;
}

void Col_init() {
	DDRx = 0xF8; // bits 0..2 = columns
	PORTx = 0x07; //pullups on these bits 
	_delay_ms(1);
}

void Row_init() {
	DDRx = 0x87; // bits 3..6 = rows
	PORTx = 0x78; //pullups on these bits 
	_delay_ms(1);
}

void Read_key() {
	uint8_t row = 0xFF;
	uint8_t col = 0xFF;

	Col_init();
	if (bit_is_clear(PINx, 0)) row = 0x00;
	else if (bit_is_clear(PINx, 1)) row = 0x01;
	else if (bit_is_clear(PINx, 2)) row = 0x02;

	Row_init();
	if (bit_is_clear(PINx, 3)) col = 0x00;
	else if (bit_is_clear(PINx, 4)) col = 0x01;
	else if (bit_is_clear(PINx, 5)) col = 0x02;
	else if (bit_is_clear(PINx, 6)) col = 0x03;

	lcd_clrscr();
	lcd_puts(convert(&digit[col][row]));
	
	_delay_ms(100);
}

void init_lcd() {
	DDRD = _BV(4);
	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 128;

	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
}

int main() {
	init_lcd();

	while(1) {
		Col_init();
		if(bit_is_clear(PINx, 0) ^ bit_is_clear(PINx, 1) ^ bit_is_clear(PINx, 2)) Read_key();
	}
}