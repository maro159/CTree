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

static volatile uint8_t led1_brightness_index = 0;
static volatile uint8_t led2_brightness_index = 85;
static volatile uint8_t led3_brightness_index = 171;
static volatile uint8_t ovf_counter = 0;

static uint8_t update_period = 0;

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
	// connect pins to channels
	TCA0.SINGLE.CTRLB |= TCA_SINGLE_CMP2EN_bm | TCA_SINGLE_CMP1EN_bm | TCA_SINGLE_CMP0EN_bm;
	// enable module
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void led_set_update_period(uint8_t period)
{
	update_period = period;
}

void led_set_brightness(led_t led, uint8_t brightness)
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

ISR(TCA0_OVF_vect)
{
	ovf_counter++;
	// prepare new values for led brightness
	if(ovf_counter >= update_period)
	{
		led_set_brightness(Led1, sine_wave[led1_brightness_index++]);
		led_set_brightness(Led2, sine_wave[led2_brightness_index++]);
		led_set_brightness(Led3, sine_wave[led3_brightness_index++]);
		ovf_counter = 0;
	}
	return;
}
