/*
 * rtc.c
 *
 * Created: 12.04.2024 01:20:58
 *  Author: mrcm7
 */ 

#include <avr/interrupt.h>
#include "pins.h"
#include "rtc.h"

static volatile uint8_t count_10ms = 0;
volatile bool flag10ms = 0;
volatile bool flag100ms = 0;

void init_rtc(void)
{
	/*
	 * NOTE: The peripheral clock (CLK_PER) is required to be at least four times faster than the RTC clock (CLK_RTC)
	 * for reading the counter value, regardless of the prescaler setting
	 */
	// 1kHz clock	
	RTC.CLKSEL |= RTC_CLKSEL_INT1K_gc;
	// enable interrupt on overflow
	RTC.INTCTRL |= RTC_OVF_bm;
	// set period to 10ms
	while(RTC.STATUS & RTC_PERBUSY_bm)
	RTC.PER = 0x2710; // 10000
	// enable module
	while(RTC.STATUS & RTC_CTRLABUSY_bm);
	RTC.CTRLA |= RTC_RTCEN_bm;
}

ISR(RTC_CNT_vect)
{
	flag10ms = true;
	count_10ms++;
	if(count_10ms >= 10)
	{
		flag100ms = true;
		count_10ms = 0;
	}
	return;
}