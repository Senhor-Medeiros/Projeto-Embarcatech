#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#include "ssd1306.h"
#include "joystick.h"
#include "interruption.h"

// Definições de pinos
#define LED_R         13
#define LED_G         11
#define LED_B         12
#define BUZZER        10      // Buzzer conectado a GP10
#define BOTAO_A       5       // Botão A: desabilita o sistema
#define BOTAO_B       6       // Botão B: reabilita o sistema
#define JOYSTICK_SEL  22

#define ADC_UMIDADE_PIN       26  // Canal usado para umidade (originalmente canal 0)
#define ADC_TEMPERATURA_PIN   27  // Canal usado para temperatura (originalmente canal 1)

#define I2C_SDA_PIN   14
#define I2C_SCL_PIN   15
#define SSD1306_ADDR  0x3C

// Configuração do buzzer via PWM
#define BUZZER_FREQUENCY 100  // Hz

// Estados do sistema
typedef enum {
    NORMAL,
    ALERTA,
    CRITICO
} EstadoSistema;

int umidade = 50;      // em %
int temperatura = 25;  // em °C
EstadoSistema estado_atual = NORMAL;

// Flag de segurança: quando false, o sistema está desabilitado
bool sistema_habilitado = true;

ssd1306_t display;
// Para o Pico W, recomenda-se usar o i2c1 para periféricos externos
i2c_inst_t *i2c_port = i2c1;

// --- Funções para o buzzer via PWM ---
void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

void beep(uint pin, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_gpio_level(pin, 2048); // 50% duty cycle
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0);
    sleep_ms(100);
}
// --- Fim das funções para o buzzer ---

// Atualiza o display para mostrar "Sistema desabilitado"
void display_desabilitado() {
    ssd1306_fill(&display, false);
    ssd1306_draw_string(&display, "Sistema", 0, 0);
    ssd1306_draw_string(&display, "desabilitado", 0, 10);
    ssd1306_send_data(&display);
}

// Define o estado com base nos valores de temperatura e umidade
void definir_estado() {
    // Crítico: temperatura > 34°C ou umidade < 30%
    if (temperatura > 34 || umidade < 30)
        estado_atual = CRITICO;
    // Alerta: temperatura entre 30°C e 33°C ou umidade entre 30% e 50%
    else if ((temperatura >= 30 && temperatura <= 33) || (umidade >= 30 && umidade <= 50))
        estado_atual = ALERTA;
    // Normal: temperatura até 29°C e umidade > 50%
    else if (temperatura <= 29 && umidade > 50)
        estado_atual = NORMAL;
    else
        estado_atual = NORMAL;
}

// Atualiza os LEDs RGB conforme o estado
void atualizar_led() {
    gpio_put(LED_R, 0);
    gpio_put(LED_G, 0);
    gpio_put(LED_B, 0);

    switch (estado_atual) {
        case NORMAL:
            gpio_put(LED_G, 1); // Verde
            break;
        case ALERTA:
            gpio_put(LED_R, 1);
            gpio_put(LED_G, 1); // Amarelo (vermelho + verde)
            break;
        case CRITICO:
            gpio_put(LED_R, 1); // Vermelho
            break;
    }
}

// Atualiza o display OLED com as informações
void atualizar_display() {
    ssd1306_fill(&display, false);
    char buffer[32];
    const char *estado_str;

    if (estado_atual == NORMAL)
        estado_str = "Normal";
    else if (estado_atual == ALERTA)
        estado_str = "Alerta";
    else
        estado_str = "Critico";

    snprintf(buffer, sizeof(buffer), "Estado: %s", estado_str);
    ssd1306_draw_string(&display, buffer, 0, 0);
    snprintf(buffer, sizeof(buffer), "Umid: %d%%", umidade);
    ssd1306_draw_string(&display, buffer, 0, 10);
    snprintf(buffer, sizeof(buffer), "Temp: %dC", temperatura);
    ssd1306_draw_string(&display, buffer, 0, 20);
    const char *buzzer_str = (estado_atual == CRITICO) ? "Buzzer: ON" : "Buzzer: OFF";
    ssd1306_draw_string(&display, buzzer_str, 0, 30);
    ssd1306_send_data(&display);
}

// Configuração dos periféricos
void setup_peripherals() {
    stdio_init_all();

    // Inicializa os LEDs RGB
    gpio_init(LED_R); gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G); gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B); gpio_set_dir(LED_B, GPIO_OUT);

    // Inicializa o buzzer via PWM
    pwm_init_buzzer(BUZZER);

    // Inicializa os botões (configurados como entrada com pull-up)
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);

    // Inicializa o joystick (mantendo a inversão da BitDogLab)
    setup_joystick(ADC_UMIDADE_PIN, ADC_TEMPERATURA_PIN);

    // Inicializa o I2C para o display OLED (usando i2c1 no Pico W)
    i2c_init(i2c_port, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Inicializa e configura o display OLED
    ssd1306_init(&display, WIDTH, HEIGHT, false, SSD1306_ADDR, i2c_port);
    ssd1306_config(&display);
}

int main() {
    setup_peripherals();

    while (true) {
        // Verifica se o botão A foi pressionado para desabilitar o sistema
        if (gpio_get(BOTAO_A) == 0) {
            sleep_ms(50);
            if (gpio_get(BOTAO_A) == 0) {
                sistema_habilitado = false;
                display_desabilitado();
                gpio_put(LED_R, 0);
                gpio_put(LED_G, 0);
                gpio_put(LED_B, 0);
                pwm_set_gpio_level(BUZZER, 0);
                sleep_ms(500);
                continue;
            }
        }

        // Se o sistema estiver desabilitado, verifica se o botão B foi pressionado para reabilitar
        if (!sistema_habilitado) {
            if (gpio_get(BOTAO_B) == 0) {
                sleep_ms(50);
                if (gpio_get(BOTAO_B) == 0) {
                    sistema_habilitado = true;
                    sleep_ms(300);
                }
            }
            display_desabilitado();
            gpio_put(LED_R, 0);
            gpio_put(LED_G, 0);
            gpio_put(LED_B, 0);
            pwm_set_gpio_level(BUZZER, 0);
            sleep_ms(200);
            continue;
        }

        // Se o sistema estiver habilitado, procede com a rotina normal:
        // Leitura dos sensores:
        // get_joystick_x_value() retorna o valor do canal 1 (temperatura)
        // get_joystick_y_value() retorna o valor do canal 0 (umidade)
        uint16_t rawX = get_joystick_x_value(); // Temperatura
        uint16_t rawY = get_joystick_y_value(); // Umidade

        umidade = (rawY * 80) / 4095;
        temperatura = (rawX * 35) / 4095;

        // Define o estado com base nos valores dos sensores
        definir_estado();
        // Atualiza os LEDs conforme o estado
        atualizar_led();

        // Se o estado for Crítico, mantém o buzzer ligado (50% duty cycle); caso contrário, desliga-o
        if (estado_atual == CRITICO)
            pwm_set_gpio_level(BUZZER, 2048);
        else
            pwm_set_gpio_level(BUZZER, 0);

        // Atualiza o display com as informações atuais
        atualizar_display();

        sleep_ms(200);
    }

    return 0;
}
