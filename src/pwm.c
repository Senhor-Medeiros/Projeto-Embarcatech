#include "pwm.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Inicializa e configura o módulo de PWM para uma GPIO
void setup_pwm(uint pin,float pwm_clock_divisor, uint wrap, uint duty_cycle){
    uint slice; // Armazena o slice PWM da GPIO

    gpio_set_function(pin,GPIO_FUNC_PWM); // Informa ao microcontrolador que o pino em questão deve ser habilitado como PWM

    slice = pwm_gpio_to_slice_num(pin); // Obtém o número do slide da GPIO informada

    pwm_set_clkdiv(slice,pwm_clock_divisor); // Define o divisor de clock do PWM

    pwm_set_wrap(slice,wrap); // Define o wrap do PWM, o valor máximo que o contador pode alcançar

    pwm_set_gpio_level(pin,duty_cycle); // Define o duty cycle do PWM

    pwm_set_enabled(slice,true); // Habilita o PWM para o slice informado
}

// Obtém o valor numérico do duty cycle com base em uma porcentagem e valor de wrap informados. A porcentagem varia de 0.0 a 1.0
float get_duty_cycle(uint wrap, float percentage){
    return percentage * wrap;
}

// Atualiza o valor do duty cycle para o pino informado
void update_duty_cycle(uint pin, uint duty_cycle){
    pwm_set_gpio_level(pin,duty_cycle); // Define o duty cycle do PWM
}

// Habilita/desabilita o PWM
void toggle_pwm(uint pin, bool state){
    uint slice = pwm_gpio_to_slice_num(pin); // obtém o slice PWM conectado á GPIO informada

    pwm_set_enabled(slice,state); // Habilita ou desabilita o PWM para o respectivo slice
}