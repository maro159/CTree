/*
 * rtc.c
 *
 * Created: 12.04.2024 01:20:58
 *  Author: mrcm7
 */ 

#include <avr/interrupt.h>
#include "pins.h"
#include "rtc.h"

#define TIMER_COUNT_500MS 51 // 50 * 10 ms + 1 to accommodate for pre-decrement in ISR

volatile uint8_t time_flags = 0x00;

void init_rtc(void)
{
	/*
	 * NOTE: The peripheral clock (CLK_PER) is required to be at least four times faster than the RTC clock (CLK_RTC)
	 * for reading the counter value, regardless of the prescaler setting
	 */
	// 1kHz clock	
	RTC.CLKSEL |= RTC_CLKSEL_INT1K_gc;
	// set period to 10ms
	while(RTC.STATUS & RTC_PERBUSY_bm);
	RTC.PER = 0x000A; // 10000
	// enable interrupt on overflow
	RTC.INTCTRL |= RTC_OVF_bm;
	// enable module
	while(RTC.STATUS & RTC_CTRLABUSY_bm);
	RTC.CTRLA |= RTC_RTCEN_bm;
}

ISR(RTC_CNT_vect)
{
	static uint8_t timer_count_500ms = TIMER_COUNT_500MS; // static to preserve value

	time_flags |= TIME_FLAG_10MS_bm;		// set 10ms flag
	
	if(--timer_count_500ms == 0)		// pre-decrement is faster
	{
		time_flags |= TIME_FLAG_500MS_bm;	// set 500ms flag
		timer_count_500ms = TIMER_COUNT_500MS;	// reset counter
	}
	RTC.INTFLAGS |= RTC_OVF_bm;	// clear interrupt flag
	return;
}