/*
 * sine.c
 *
 * Created: 13.04.2024 02:59:51
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

#include "sine.h"

static const uint8_t sine_wave[64] = {129,132,135,138,142,145,148,151,154,157,160,163,166,169,172,175,178,181,183,186,189,192,194,197,200,202,205,207,210,212,214,217,219,221,223,225,227,229,231,233,234,236,238,239,241,242,243,245,246,247,248,249,250,251,252,252,253,253,254,254,255,255,255,255};

uint8_t sine_get_value(uint8_t index)
{
	if (index <= 63)
	{
		return (sine_wave[index]);
	}
	else if (index <= 127)
	{
		return (sine_wave[127 - index]);
	}
	else if (index <= 191)
	{
		return (255 - sine_wave[index - 128]);
	}
	else
	{
		return (255 - sine_wave[255 - index]);
	}
}