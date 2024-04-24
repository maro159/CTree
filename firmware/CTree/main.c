/*
 * main.c
 *
 * Created: 4/7/2024 5:31:34 PM
 *  Author: mrcm7
 */ 
#define F_CPU 32768

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "pins.h"
#include "led.h"
#include "power_hold.h"
#include "BUTTON.h"
#include "rtc.h"


static void init_clock(void)
{
	// Unlock protected I/O registers
	CPU_CCP = CCP_IOREG_gc;
	// 32kHz main clock
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
	// disable prescaler
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB &= ~CLKCTRL_PEN_bm;
	// lock clock settings till next hardware reset
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKLOCK = CLKCTRL_LOCKEN_bm;
}

static void init(void)
{
	init_clock();
	init_rtc();
	init_led();
	init_power_hold();
	init_button();
}

static void led_mode_decode(led_mode_t *mode, uint8_t click_count)
{
	switch(click_count)
	{
		case 0:
			mode->period = LED_PERIOD_LOW;
			mode->dim = LED_DIM_HIGH;
			break;
		case 1:
			mode->period = LED_PERIOD_LOW;
			mode->dim = LED_DIM_MID;
			break;
		case 2:
			mode->period = LED_PERIOD_LOW;
			mode->dim = LED_DIM_LOW;
			break;
		case 3:
			mode->period = LED_PERIOD_HIGH;
			mode->dim = LED_DIM_HIGH;
			break;
		case 4:
			mode->period = LED_PERIOD_HIGH;
			mode->dim = LED_DIM_MID;
			break;
		case 5:
			mode->period = LED_PERIOD_HIGH;
			mode->dim = LED_DIM_LOW;
			break;
		default:
			mode->period = LED_PERIOD_LOW;
			mode->dim = LED_DIM_HIGH;
			break;
	}
}

int main(void)
{
	uint8_t button_states_10ms = 0;
	uint8_t button_states_500ms = 0;
	uint8_t click_count = 0;
	led_mode_t led_mode = {.dim = LED_DIM_HIGH, .period = LED_PERIOD_HIGH};
	led_mode_decode(&led_mode, click_count);
	
	init();
	
	if (IS_BUTTON) power_hold_enable(true);	// enable power hold after power-on initiated by button
	led_enable(true);
	sei();									// enable interrupts
	while (IS_BUTTON)						// wait for button release
	{
		led_process(&led_mode);
	}
	//_delay_ms(1000);
	
	while(true)
	{
		led_process(&led_mode);
		if(time_flags & TIME_FLAG_10MS_bm)
		{
			// left shift button states and add current button state
			button_states_10ms = (button_states_10ms << 1) | (IS_BUTTON & 0x01);
			// detect hold for 40ms to debounce
			if((button_states_10ms & 0b00001111) == 0b00000111)
			{
				click_count++;
				if(click_count > 5) click_count = 0;
				led_mode_decode(&led_mode, click_count);
			}
			time_flags &= ~TIME_FLAG_10MS_bm;
		}
		
		if(time_flags & TIME_FLAG_500MS_bm)
		{
			button_states_500ms = (button_states_500ms << 1) | (IS_BUTTON & 0x01);
			// power off when button hold for at least 2000ms
			if((button_states_500ms & 0b00001111) == 0b00000111)
			{	
				led_enable(false);
				while (IS_BUTTON);			// wait for button release
				power_hold_enable(false);
			}
			time_flags &= ~TIME_FLAG_500MS_bm;
		}
	}
	return 0;
}


