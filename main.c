/*
* Arquitetura e Aplicação de sistemas - exercicio 02 - Leitor de temperatura interna do MCU RP2040
* Aluno: Victor Hugo de Toledo Nunes
* Prof.: Gustavo Ferreira Palma 
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

const uint LED_PIN = 17; // Pino padrão do LED na placa RPI PICO Chinesa

int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    return cyw43_arch_init();
#endif
}

void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    gpio_put(LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

float read_onboard_temperature() {
    
    const float conversionFactor = 3.3f / (1 << 12); //conversão para AD de 12bit, assuminfo ADC_VREF = 3V3

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;
    
    return tempC;
//    return -1.0f;
}

int main() {
    stdio_init_all();
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    while (true) {
        float temperature = read_onboard_temperature();

        printf("Onboard temperature = %.02f %c\n", temperature);

        pico_set_led(true);
        sleep_ms(100);
        pico_set_led(false);

        sleep_ms(900);
    }
}
