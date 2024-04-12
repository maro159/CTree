/*
 * pins.h
 *
 * Created: 07.04.2024 18:48:39
 *  Author: mrcm7
 */ 


#ifndef PINS_H_
#define PINS_H_

#include <avr/io.h>
#include <stdbool.h>

// pin definitions
#define BUTTON_PIN 7
#define POWER_HOLD_PIN 6

#define LED1_PIN 2
#define LED2_PIN 3
#define LED3_PIN 1

// macro to replace PINnCTRL
#define PINCTRL_HELPER(n) PIN##n##CTRL
#define PINCTRL(n) PINCTRL_HELPER(n)

// input checks
#define IS_BUTTON ((PORTA.IN & (1U << BUTTON_PIN)) != (1U << BUTTON_PIN))	// inverted logic


#endif /* PINS_H_ */