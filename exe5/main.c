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
    uint32_t ms;
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);

    
    while (true) {
        if (flag_fall) {
            ms = to_ms_since_boot(get_absolute_time());

            flag_fall = 0;
        }
        else if (flag_rise == 1) {
            uint32_t ms_release = to_ms_since_boot(get_absolute_time());

            uint32_t duration_ms = (ms_release - ms);

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
