#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"

// Definições de pinos
#define LED_RED_PIN 11
#define LED_BLUE_PIN 12
#define LED_GREEN_PIN 13
#define JOYSTICK_X_ADC_CHANNEL 0 // GPIO 26
#define JOYSTICK_Y_ADC_CHANNEL 1 // GPIO 27
#define JOYSTICK_BUTTON_PIN 22
#define BUTTON_A_PIN 5
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

// Configurações do display SSD1306
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SQUARE_SIZE 8

// Variáveis globais
volatile bool toggle_green_led = false;
volatile bool pwm_enabled = true;
volatile uint8_t border_style = 0; // 0: sem borda, 1: linha simples, 2: linha dupla
uint16_t joystick_x = 2048, joystick_y = 2048;
int square_x = (SCREEN_WIDTH - SQUARE_SIZE) / 2;
int square_y = (SCREEN_HEIGHT - SQUARE_SIZE) / 2;

// Função para configurar PWM em um pino específico
void setup_pwm(uint gpio_pin) {
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);
    pwm_set_wrap(slice_num, 255); // Resolução de 8 bits (0-255)
    pwm_set_enabled(slice_num, true);
}

// Função para atualizar o brilho do LED via PWM
void update_pwm(uint gpio_pin, uint16_t adc_value) {
    if (!pwm_enabled) return;

    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);
    uint8_t brightness = (adc_value > 2048) ? (adc_value - 2048) / 16 : (2048 - adc_value) / 16;
    pwm_set_gpio_level(gpio_pin, brightness);
}

// Tratamento de interrupção para o botão do joystick
void joystick_button_irq_handler(uint gpio, uint32_t events) {
    static absolute_time_t last_press_time = {0};
    absolute_time_t current_time = get_absolute_time();

    // Debouncing
    if (absolute_time_diff_us(last_press_time, current_time) < 200000) return; // 200ms
    last_press_time = current_time;

    // Alternar estado do LED verde
    toggle_green_led = !toggle_green_led;
    gpio_put(LED_GREEN_PIN, toggle_green_led);

    // Alterar estilo da borda
    border_style = (border_style + 1) % 3;
}

// Tratamento de interrupção para o botão A
void button_a_irq_handler(uint gpio, uint32_t events) {
    static absolute_time_t last_press_time = {0};
    absolute_time_t current_time = get_absolute_time();

    // Debouncing
    if (absolute_time_diff_us(last_press_time, current_time) < 200000) return; // 200ms
    last_press_time = current_time;

    // Ativar/desativar LEDs PWM
    pwm_enabled = !pwm_enabled;
}

// Função para desenhar o quadrado no display
void draw_square(ssd1306_t *display, int x, int y) {
    ssd1306_fill(display, false); // Limpa o display

    // Desenha o quadrado móvel
    ssd1306_rect(display, x, y, SQUARE_SIZE, SQUARE_SIZE, true, true);

    // Desenha a borda conforme o estilo
    if (border_style == 1) {
        ssd1306_rect(display, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, true, false);
    } else if (border_style == 2) {
        ssd1306_rect(display, 2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, true, false);
    }

    ssd1306_send_data(display); // Atualiza o display
}

int main() {
    // Inicialização básica
    stdio_init_all();
    gpio_init(LED_RED_PIN);
    gpio_init(LED_BLUE_PIN);
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);

    // Configuração do ADC
    adc_init();
    adc_gpio_init(26); // Joystick X
    adc_gpio_init(27); // Joystick Y

    // Configuração do PWM
    setup_pwm(LED_RED_PIN);
    setup_pwm(LED_BLUE_PIN);

    // Configuração dos botões com interrupções
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &joystick_button_irq_handler);

    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_a_irq_handler);

    // Configuração do display SSD1306 via I2C
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    ssd1306_t display;
    ssd1306_init(&display, SCREEN_WIDTH, SCREEN_HEIGHT, false, 0x3C, i2c1);
    ssd1306_config(&display);

    while (true) {
        // Leitura do ADC para os eixos X e Y do joystick
        adc_select_input(JOYSTICK_X_ADC_CHANNEL);
        joystick_x = adc_read();
        adc_select_input(JOYSTICK_Y_ADC_CHANNEL);
        joystick_y = adc_read();

        // Atualização do brilho dos LEDs via PWM
        update_pwm(LED_RED_PIN, joystick_x);
        update_pwm(LED_BLUE_PIN, joystick_y);

        // Movimentação do quadrado no display
        square_x = ((joystick_x * (SCREEN_WIDTH - SQUARE_SIZE)) / 4095);
        square_y = ((joystick_y * (SCREEN_HEIGHT - SQUARE_SIZE)) / 4095);
        draw_square(&display, square_x, square_y);

        sleep_ms(50); // Pequeno delay para evitar sobrecarga
    }
}