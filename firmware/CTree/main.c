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


static uint8_t button_states_10ms = 0;
static uint8_t button_states_100ms = 0;

static void init_clock(void)
{
	// Unlock protected I/O registers
	CPU_CCP = CCP_IOREG_gc;
	// 32kHz main clock
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
	// disable prescaler
	CPU_CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLB &= !CLKCTRL_PEN_bm;
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

int main(void)
{
	init();
	if (IS_BUTTON) power_hold_set(true);	// enable power hold after power-on initiated by button
	sei();									// enable interrupts
	while (IS_BUTTON);						// wait for button release
	_delay_ms(1000);
	
	while(true)
	{
		
		
		if(flag10ms)
		{
			// left shift button states and add current button state
			button_states_10ms = (button_states_10ms << 1) | (IS_BUTTON & 0x01);
			// when click for at least 30ms
			if((button_states_100ms & 0b00001111) == 0b00000111)
			{
				// TODO: button clicked
			}
			flag10ms = false;
		}
		
		if(flag100ms)
		{
			button_states_100ms = (button_states_100ms << 1) | (IS_BUTTON & 0x01);
			// power off when button hold for at least 800ms
			if(button_states_100ms == 0b01111111)
			{	
				power_hold_set(false);
			}
			flag100ms = false;
		}
	}
	return 0;
}


