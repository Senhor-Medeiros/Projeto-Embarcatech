#ifndef _JOYSTICK_H
    #define _JOYSTICK_H

    #include "pico/stdlib.h"
    #include "hardware/adc.h"

    #define JOYSTICK_X_AXIS_MULTIPLEXER_INPUT 0 // Indica no multiplexador do ADC a entrada referente a GPIO 26
    #define JOYSTICK_Y_AXIS_MULTIPLEXER_INPUT 1 // Indica no multiplexador do ADC a entrada referente a GPIO 27

    void setup_joystick(uint x_axis_pin, uint y_axis_pin);

    uint16_t get_joystick_x_value();

    uint16_t get_joystick_y_value();
#endif