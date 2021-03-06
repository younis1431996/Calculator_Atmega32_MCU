/*
 * lcd16x2.h
 *
 *  Created on: Sep 27, 2021
 *      Author: Mahmoud Mohamed Younis
 */

#ifndef LCD_LCD16X2_H_
#define LCD_LCD16X2_H_
/* inclusions */
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd16x2_commands.h"

/* lcd class. */
/* lcd will be initialized to a 4-bit mode, 2-lines and 5x8 dots. */
/* some functions will able to change lcd mode and type custom characters. */
class lcd{
public:
	// initialization function.
	// control pins array as following ==> RS, RW and EN.
	void init(volatile uint8_t *Data_DDRx, volatile uint8_t *Data_PORTx,
			  volatile uint8_t *Control_DDRx, volatile uint8_t *Control_PORTx,
			           uint8_t data_pins[4], uint8_t control_pins[3]);
	/* send command. */
	void send_command(uint8_t command);

	/* print character */
	void print_character(uint8_t character);

	/* print character to x, y position*/
	void print_character_to(uint8_t characetr, uint8_t x, uint8_t y);

	/* print string */
	void print_string(const char *str);

	/* print string to x, y position*/
	void print_string_to(const char *str, uint8_t x, uint8_t y);

	/* print number */
	void print_number(double number);

	/* print number to x, y position*/
	void print_number_to(float number, uint8_t x, uint8_t y);

	/* move cursor to x, y position */
	void move_cursor_to(uint8_t x, uint8_t y);

	/* clear lcd*/
	void clear(void);

	/* return to x=0, y=0 position */
	void return_home(void);

	/* new line */
	void new_line(void);

private:
	// Hardware configuration.
	volatile uint8_t *Data_DDR;
	volatile uint8_t *Data_PORT;
	volatile uint8_t *Control_DDR;
	volatile uint8_t *Control_PORT;
			 uint8_t  Data_pins[4];
			 uint8_t  RS;
			 uint8_t  RW;
			 uint8_t  EN;
	/* flag to set the function of the lcd. */
	/* f=0 4-bits, 1-line, 5x8 dots.        */
	/* f=1 4-bits, 1-line, 5x10 dots.       */
    /* f=2 4-bits, 2-lines, 5x8 dots.       */
	/* f=3 8-bits, 1-line, 5x8 dots. 	    */
	/* f=4 8-bits, 1-line, 5x10 dots.	    */
	/* f=5 8-bits, 2-lines, 5x8 dots.       */
			 uint8_t function;
	void set_data_pin(uint8_t pin){
		*Data_PORT |= (1<<pin);
	}

	void reset_data_pin(uint8_t pin){
		*Data_PORT &= ~(1<<pin);
	}

	void double_to_str(double number, char *buffer, bool *after_point){
		/* used in float conversion. */
		double _10_pow[5] = {10.0, 100.0, 1000.0, 10000.0, 100000.0};
		double float_part = 0;
		double result = 0;

		/* check if float number is 0*/
		if(number == 0){
			*after_point = 0;
			return;
		}
		if(number < 0){
			number *= -1;
		}

		/* loop to extract float part and convert it to integer. */
		int i = 0;
		char temp_buffer[5] = "\0";
		while(i<5){
			float_part = number * _10_pow[i];
			result = float_part - (long)float_part;
			if( ((int)(float_part)) == 0){
				buffer[i] = '0';
			}
			if(result == 0){
				break;
			}
			i++;
		}

		/* convert float part into string. */
		ltoa((long)float_part, temp_buffer, 10);
		strcat(buffer, temp_buffer);

		/* after point !=0 */
		*after_point = 1;

		return;
	}

	 void int_to_str(long number, char *buffer){
		ltoa(number, buffer, 10);
	}
};



#endif /* LCD_LCD16X2_H_ */
