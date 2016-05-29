#include <avr/io.h>          // register definitions
#include <avr/pgmspace.h>    // storing data in program memory
#include <avr/interrupt.h>   // interrupt vectors
#include <util/delay.h>      // delay functions

#include <stdint.h>          // C header for int types like uint8_t
#include <stdbool.h>         // C header for the bool type

// Include stuff from the library
#include "lib/iopins.h"
#include "lib/usart.h"
#include "lib/debounce.h"

static const char names[] = {
	'L', // 2
	'U', // 3
	'R', // 4
	'D', // 5
	'I', // 6
	'J', // 7
	'K', // 8
	'X', // 9
	'B', // 10
	'A', // 11
	'Y', // 12
};

uint8_t down = 0;

static void pin_change_handler(uint8_t n, bool state)
{
	char c = names[n];
	if (!state) {
		c += 32; // lowecase
	}

	if (state) {
		down++;
	} else {
		down--;
	}

	pin_set(9, (bool)(down > 0));

	usart_tx((uint8_t) c);
}

static void hw_init(void)
{
	usart_init(BAUD_115200);

	as_input_pu(2);
	as_input_pu(3);
	as_input_pu(4);
	as_input_pu(5);

	as_input_pu(6);
	as_input_pu(7);
	as_input_pu(8);

	// 9 - LED
	as_output(9); // front LED
	as_output(13); // LED on pro mini

	as_input_pu(10);
	as_input_pu(11);
	as_input_pu(12);
	as_input_pu(14);

	// cant use loop because macros
	debo_add(2, true, pin_change_handler);
	debo_add(3, true, pin_change_handler);
	debo_add(4, true, pin_change_handler);
	debo_add(5, true, pin_change_handler);
	debo_add(6, true, pin_change_handler);
	debo_add(7, true, pin_change_handler);
	debo_add(8, true, pin_change_handler);
	debo_add(14, true, pin_change_handler);
	debo_add(10, true, pin_change_handler);
	debo_add(11, true, pin_change_handler);
	debo_add(12, true, pin_change_handler);
}


void main(void)
{
	hw_init();


	// globally enable interrupts (for the USART_RX handler)
	//sei();
	//pin_up(9);

	int cnt = 0;
	while (1) {
		_delay_ms(1);
		debo_tick();

		if (cnt++ > 500) {
			cnt = 0;
		}
	}
}
