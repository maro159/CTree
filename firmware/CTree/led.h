/*
 * led.h
 *
 * Created: 11.04.2024 00:52:01
 *  Author: mrcm7
 */ 


#ifndef LED_H_
#define LED_H_

typedef enum led_t
{
	Led1,
	Led2,
	Led3,
} led_t;

void init_led(void);
void led_set_brightness(led_t led, uint8_t brightness);
void led_set_update_period(uint8_t period);

#endif /* LED_H_ */