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
	LED1,
	LED2,
	LED3,
} led_t;

// how often led are faded
typedef enum led_period_t
{
	LED_PERIOD_LOW = 1,
	LED_PERIOD_MID = 2,
	LED_PERIOD_HIGH = 4,
} led_period_t;

// how much led are dimmed, should be power of 2
typedef enum led_dim_t
{
	LED_DIM_LOW = 1,
	LED_DIM_MID = 2,
	LED_DIM_HIGH = 4,
} led_dim_t;

typedef struct led_mode_t
{
	led_period_t period;
	led_dim_t dim;
} led_mode_t;

void init_led(void);
void led_process(led_mode_t *mode);
void led_enable(uint8_t state);

#endif /* LED_H_ */