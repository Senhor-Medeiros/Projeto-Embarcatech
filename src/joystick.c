#include "joystick.h"

// Inicializa e configura o ADC
void setup_joystick(uint x_axis_pin, uint y_axis_pin){
    adc_init(); // Inicializa o conversor anlógico-digital
    adc_gpio_init(x_axis_pin); // Inicializa uma GPIO como entrada do ADC
    adc_gpio_init(y_axis_pin); // Inicializa uma GPIO como entrada do ADC
}

// Lê a posição do joystick no eixo horizontal
uint16_t get_joystick_x_value(){
    adc_select_input(JOYSTICK_Y_AXIS_MULTIPLEXER_INPUT);

    return adc_read();
}

// Lê a posição do joystick no eixo vertical
uint16_t get_joystick_y_value(){
    adc_select_input(JOYSTICK_X_AXIS_MULTIPLEXER_INPUT);

    return adc_read();
}