/*
 * rtc.h
 *
 * Created: 12.04.2024 01:23:31
 *  Author: mrcm7
 */ 


#ifndef RTC_H_
#define RTC_H_

#include "pins.h"

volatile bool flag10ms;
volatile bool flag100ms;

void init_rtc(void);

#endif /* RTC_H_ */