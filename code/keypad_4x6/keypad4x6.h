/*
 * keypad4x6.h
 *
 *  Created on: Oct 8, 2021
 *      Author: Mahmoud Mohamed Younis
 */

#ifndef KEYPAD_4X6_KEYPAD4X6_H_
#define KEYPAD_4X6_KEYPAD4X6_H_
/* inclusions */
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/* keypad class */
class keypad_4x6_{
public:
	/* initialization function */
	void init(volatile uint8_t *column_DDRx, volatile uint8_t *column_PINx,  volatile uint8_t *column_PUDx,
			  volatile uint8_t *row_DDRx, volatile uint8_t *row_PRTx,
			  uint8_t  R_pin[4], uint8_t  C_pin[6], uint8_t key_map[4][6]){

		/* 1.Save hardware configuration */
		// 1.1.Columns
		column_DDR = column_DDRx;
		column_PIN = column_PINx;
		column_PUD = column_PUDx;
		for(uint8_t c=0; c<6; c++){
			column_pin[c] = C_pin[c];
		}
	/*-------------------------------------------*/
		// 1.2.Rows.
		row_DDR = row_DDRx;
		row_PRT = row_PRTx;
		for(uint8_t r=0; r<4; r++){
			row_pin[r] = R_pin[r];
		}
	/*-------------------------------------------*/
		/* 2.Save key map. */
		for(uint8_t r=0; r<4; r++){
			for(uint8_t c=0; c<6; c++){
				map[r][c] = key_map[r][c];
			}
		}
	/*-------------------------------------------*/
		/* Configure columns as input, plus activate pull-up resistors. */
		for(uint8_t c=0; c<6; c++){
			// 1.Configure pins as input.
			*column_DDR &= ~(1<<column_pin[c]);
			// Insert nop for synchronization
			__asm__("nop");

			// 2.Activate PUD.
			*column_PUD |= (1<<column_pin[c]);
			// Insert nop for synchronization
			__asm__("nop");
		}
	/*-------------------------------------------*/
		/* Configure rows as output. */
		for(uint8_t r=0; r<4; r++){
			// 1.Configure pins as output.
			*row_DDR |= (1<<row_pin[r]);
			// Insert nop for synchronization
			__asm__("nop");

			// 2. All pins initially low.
			*row_PRT &= ~(1<<row_pin[r]);
			// Insert nop for synchronization
			__asm__("nop");
		}
	/*-------------------------------------------*/
	} /* End of init function */

/*  <$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$> */

	void read(uint8_t *key){
		/* Idea: activate only one row then read all columns.
		 * when a key is pressed i already know it's row and
		 * it's column.
		 * Note that: row is active low.
		 */
		for(uint8_t r=0; r<4; r++){
			// 1.Deactivate all rows.
			*row_PRT |= (1<<row_pin[0]);
			*row_PRT |= (1<<row_pin[1]);
			*row_PRT |= (1<<row_pin[2]);
			*row_PRT |= (1<<row_pin[3]);

			// 2.Activate specific row.
			*row_PRT &= ~(1<<row_pin[r]);

			// 3.Read all columns then return key value.
			for(uint8_t c=0; c<6; c++){
				if(is_pressed(column_pin[c]) == true){
					*key = map[r][c];
					return;
				}
				// Insert nop for synchronization
				__asm__("nop");
			}
		}
	} /* End of read function */

/*  <$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$> */

private:
	volatile uint8_t *column_DDR;
	volatile uint8_t *column_PIN;
	volatile uint8_t *column_PUD;
	volatile uint8_t *row_DDR;
	volatile uint8_t *row_PRT;
	uint8_t  row_pin[4];
	uint8_t  column_pin[6];
	uint8_t  map[4][6];

/*  <$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$> */

	bool read_pin(uint8_t pin){
		// PIN register value.
		uint8_t PINR_value = 0x00;

		// connected pin value.
		bool pin_value = false;

		// Read PIN register.
		PINR_value = *column_PIN;

		// Mask to read the certain pin.
		pin_value = (bool) (PINR_value & (1<<pin));

		// Insert nop for synchronization
		__asm__("nop");
		__asm__("nop");

		return pin_value;
	} /* End of read_pin function */

/*  <$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$>*<$> */

	bool is_pressed(uint8_t pin){
		// pressed key state value.
		bool ispressed = false;

		/* stability loop. */
		// key is pressed when red_pin function return zero.
		while(read_pin(pin) == false){
			ispressed = true;
		}

		return ispressed;
	} /* End of is_pressed function */
}; /* End of keypad_4x6_ class */




#endif /* KEYPAD_4X6_KEYPAD4X6_H_ */
