/*
 * power_hold.c
 *
 * Created: 10.04.2024 23:01:03
 *  Author: mrcm7
 */ 

#include "pins.h"
#include "power_hold.h"

void init_power_hold(void)
{
	// set pin direction to output
	PORTA.DIRSET = (1U << POWER_HOLD_PIN);
}

void power_hold_enable(bool state)
{
	if (true == state)
	{
		PORTA.OUTSET = (1U << POWER_HOLD_PIN);
	}
	else
	{
		PORTA.OUTCLR = (1U << POWER_HOLD_PIN);
	}
}