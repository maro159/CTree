/*
 * sine.h
 *
 * Created: 12.04.2024 00:42:58
 *  Author: mrcm7
 */ 

/*
 * This file provides efficient implementation of sine signal generator
 * sine_wave array contains 64 values, representing the first quadrant of the function (sin(x)+1)/2
 * Using the fact that the sin function is odd and periodic, whole period can be reconstructed
 * To reduce the error a phase shift was introduced
 * To be strict, equation of realized function is f(x) = 255 * (sin(PI / 128 * (x + 0.5)) + 1) / 2
 * Note that x is an integer and result is rounded to nearest integer
 *
 * function get_sine allows you to get f(x) for argument in range (0, 255)
 *
 * TL;DR: use function get_sine() with arguments in range (0, 255) to get sine wave oscillating in range (0, 255)
 */

#ifndef SINE_H_
#define SINE_H_

#include <avr/io.h>

uint8_t sine_get_value(uint8_t index);


#endif /* SINE_H_ */