#ifndef _PWM_H
#define _PWM_H

#include "pico/stdlib.h"

void setup_pwm(uint pin,float pwm_clock_divisor, uint wrap, uint duty_cycle);

void update_duty_cycle(uint pin, uint duty_cycle);

float get_duty_cycle(uint wrap, float percentage);

void toggle_pwm(uint pin, bool state);
#endif