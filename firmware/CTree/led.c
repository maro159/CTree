/*
 * led.c
 *
 * Created: 07.04.2024 18:46:55
 *  Author: mrcm7
 */ 

#include "pins.h"
#include "led.h"

void init_led(void)
{
	// set direction to output
	PORTA.DIRSET = (1U << LED1_PIN) | (1U << LED2_PIN) | (1U << LED3_PIN);
	// set pin inversion
	PORTA.PINCTRL(LED1_PIN) |= PORT_INVEN_bm;
	PORTA.PINCTRL(LED2_PIN) |= PORT_INVEN_bm;
	PORTA.PINCTRL(LED3_PIN) |= PORT_INVEN_bm;
	
	// single slope PWM mode
	TCA0.SINGLE.CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
	// set period to 128 ticks (256 Hz frequency for 32.768 kHz clock)
	TCA0.SINGLE.PER = 0x0010;
	// interrupt on overflow - each cycle
	TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm;
	// connect pins to channels
	TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_CMP1EN_bm | TCA_SINGLE_CMP0EN_bm;
	// enable module
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void set_led_brightness(led_t led, uint8_t brightness)
{
	switch(led)
	{
		case Led1:
			TCA0.SINGLE.CMP0BUFL = brightness;
			break;
		case Led2:
			TCA0.SINGLE.CMP1BUFL = brightness;
			break;
		case Led3:
			TCA0.SINGLE.CMP2BUFL = brightness;
			break;
	}
}