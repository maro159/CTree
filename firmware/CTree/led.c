/*
 * led.c
 *
 * Created: 07.04.2024 18:46:55
 *  Author: mrcm7
 */ 

#include "pins.h"
#include "led.h"
#include "sine.h"
#include "avr/interrupt.h"

static volatile uint8_t ovf_counter = 0;

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
	// set period to 256 ticks (128 Hz frequency for 32.768 kHz clock)
	TCA0.SINGLE.PER = 255;
	// interrupt on overflow - each cycle
	TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm;
}

void led_enable(uint8_t state)
{
	if(true == state)
	{
		// connect pins to channels
		TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_CMP1EN_bm | TCA_SINGLE_CMP0EN_bm;
		// enable module
		TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
	}
	else
	{
		// disconnect pins from channels
		TCA0.SINGLE.CTRLB &= ~(TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_CMP1EN_bm | TCA_SINGLE_CMP0EN_bm);
		// disable module
		TCA0.SINGLE.CTRLA &= ~TCA_SINGLE_ENABLE_bm;
	}
}

static void led_set_brightness(led_t led, uint8_t brightness)
{
	switch(led)
	{
		case LED1:
			TCA0.SINGLE.CMP0BUFL = brightness;
			break;
		case LED2:
			TCA0.SINGLE.CMP1BUFL = brightness;
			break;
		case LED3:
			TCA0.SINGLE.CMP2BUFL = brightness;
			break;
	}
}

void led_process(led_mode_t *mode)
{
	// definition of start index for led channels, provide phase offset of approx. 120 degrees
	static uint8_t led1_brightness_index = 0;
	static uint8_t led2_brightness_index = 85;
	static uint8_t led3_brightness_index = 171;
	
	// prepare new values for led brightness
	if(ovf_counter >= mode->period)
	{
		led_set_brightness(LED1, sine_get_value(led1_brightness_index++) / mode->dim);
		led_set_brightness(LED2, sine_get_value(led2_brightness_index++) / mode->dim);
		led_set_brightness(LED3, sine_get_value(led3_brightness_index++) / mode->dim);
		ovf_counter = 0;
	}
}

ISR(TCA0_OVF_vect)
{
	ovf_counter++;
	return;
}
