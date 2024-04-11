/*
 * main.c
 *
 * Created: 4/7/2024 5:31:34 PM
 *  Author: mrcm7
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#include "pins.h"
#include "led.h"
#include "power_hold.h"
#include "switch.h"

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

void init_rtc(void)
{
	/*
	 * NOTE: The peripheral clock (CLK_PER) is required to be at least four times faster than the RTC clock (CLK_RTC)
	 * for reading the counter value, regardless of the prescaler setting
	 */
	// 1kHz clock	
	RTC.CLKSEL |= RTC_CLKSEL_INT1K_gc;
	// set period to 
	// enable interrupt on overflow
	RTC.INTCTRL |= RTC_OVF_bm;	
	// enable module
	while(RTC.STATUS & RTC_CTRLABUSY_bm);
	RTC.CTRLA |= RTC_RTCEN_bm;
}

static void init(void)
{
	init_clock();
	init_rtc();
	init_led();
	init_power_hold();
	init_switch();
}


int main(void)
{
	init();
	if (IS_SWITCH) power_hold_set(true);	// enable power hold after power-on initiated by switch
	sei();									// enable interrupts
	
	while(true)
	{
		
	}
	return 0;
}


