#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

const int BTN_PIN = 28;
const int LED_PIN = 4;

volatile int flag_fall = 0;
volatile int flag_rise = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        flag_fall = 1;
    }
    else if (events == 0x8) {
        flag_rise = 1;
    }
}

int main() {
    int64_t micros;
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);

    
    while (true) {
        if (flag_fall) {
            absolute_time_t press = get_absolute_time();
            micros = to_us_since_boot(press);

            flag_fall = 0;
        }
        else if (flag_rise == 1) {
            absolute_time_t release = get_absolute_time();
            int64_t micros_release = to_us_since_boot(release);

            int64_t duration_ms = (micros_release - micros) * 0.001;

            if (duration_ms > 800) {
                printf("Aperto longo!\n");
            }
            else {
                printf("Aperto curto!\n");
            }

            flag_rise = 0;
        }
    }
}
