#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;

void btn_callback(uint gpio, uint32_t events)
{
    if (events == 0x4)
    {
        printf("entrou callback");
        if (gpio == BTN_PIN_R)
        {
            flag_r = 1;
        }
    }
    if (events == 0x8)
    {
        if (gpio == BTN_PIN_G)
        {
            flag_g = 1;
        }
    }
}

int main()
{
    stdio_init_all();

    // btns
    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    // leds
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    int led_state_r = 0;
    
    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    int led_state_g = 0;

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

    while (true)
    {
        if (flag_r)
        {
            if (led_state_r)
            {
                gpio_put(LED_PIN_R, 0);
                led_state_r = 0;
            }
            else
            {
                gpio_put(LED_PIN_R, 1);
                led_state_r = 1;
            }
            printf("entrou main");
            flag_r = 0;
        }

        if (flag_g)
        {
            if (led_state_g)
            {
                gpio_put(LED_PIN_G, 0);
                led_state_g = 0;
            }
            else
            {
                gpio_put(LED_PIN_G, 1);
                led_state_g = 1;
            }
            flag_g = 0;
        }
    }
}
