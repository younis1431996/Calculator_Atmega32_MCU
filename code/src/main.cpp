/*
 * main.cpp
 *
 *  Created on: Oct 17, 2021
 *      Author: Mahmoud Mohamed Younis
 */
/* inclusions */
#include "../lcd/lcd16x2.h"
#include "../keypad_4x6/keypad4x6.h"

/* main function */
int main(void){
	/* keypad class object */
	keypad_4x6_ keypad;

	/* keypad init */
	uint8_t R_pin[4] = {PA0, PA1, PA2, PA3};
	uint8_t C_pin[6] = {PB0, PB1, PB2, PB3, PB4, PB5};
	uint8_t key_map[4][6] = {
			{'C', '7', '8', '9', 'X', '/'},
		    {'S', '4', '5', '6', '-', 'M'},
			{'%', '1', '2', '3', '+', 'N'},
			{'R', '0', '.', '=', '+', 'P'},
	};
	keypad.init(&DDRB, &PINB, &PORTB, &DDRA, &PORTA, R_pin, C_pin, key_map);


	/* lcd class object */
	lcd lcd;

	/* lcd init */
	uint8_t data_pins[4] = {PC0, PC1, PC2, PC3};
	uint8_t control_pins[3] = {PC5, PC6, PC7};
	lcd.init(&DDRC, &PORTC, &DDRC, &PORTC, data_pins, control_pins);

	/* init screen */
	lcd.return_home();
	lcd.print_string_to("calculator; OP[ , ]",0,0);
	lcd.print_character_to('*', 0, 15);
	lcd.print_character_to('+', 0, 17);
	lcd.move_cursor_to(1, 0);

	uint8_t key = '\0';

	for(;;)
	{
		keypad.read(&key);
		if(key!='\0'){
			lcd.print_character(key);
			key='\0';
		}
	}
		return 0;
}



