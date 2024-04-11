/*
 * switch.c
 *
 * Created: 10.04.2024 23:31:31
 *  Author: mrcm7
 */ 

#include <avr/interrupt.h>

#include "pins.h"
#include "switch.h"

static uint8_t press_count = 0;

void init_switch(void)
{
	// set direction to input (redundant)
	PORTA.DIRCLR = SWITCH_PIN;
	// enable internal pullup
	PORTA.PINCTRL(SWITCH_PIN) |= PORT_INVEN_bm;
	// interrupt on falling edge
	PORTA.PINCTRL(SWITCH_PIN) |= PORT_ISC_FALLING_gc;
}

ISR(PORTA_PORT_vect)
{

}
