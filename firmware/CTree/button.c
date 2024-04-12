/*
 * BUTTON.c
 *
 * Created: 10.04.2024 23:31:31
 *  Author: mrcm7
 */ 

#include <avr/interrupt.h>

#include "pins.h"
#include "button.h"

void init_button(void)
{
	// set direction to input (redundant)
	PORTA.DIRCLR = BUTTON_PIN;
	// enable internal pullup
	PORTA.PINCTRL(BUTTON_PIN) |= PORT_INVEN_bm;
	// interrupt on falling edge
	PORTA.PINCTRL(BUTTON_PIN) |= PORT_ISC_FALLING_gc;
}