/*
 * rtc.h
 *
 * Created: 12.04.2024 01:23:31
 *  Author: mrcm7
 */ 


#ifndef RTC_H_
#define RTC_H_

#include "pins.h"

#define TIME_FLAG_10MS_bm 0x01
#define TIME_FLAG_500MS_bm 0x02

volatile uint8_t time_flags;

void init_rtc(void);

#endif /* RTC_H_ */