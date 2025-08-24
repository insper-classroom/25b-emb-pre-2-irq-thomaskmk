#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int LED_PIN = 4;
const int BTN_PIN = 28;

volatile int flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) {
    flag = 1;
  }
}

int main() {
  int led_state = 0;
  stdio_init_all();

  gpio_init(BTN_PIN);
  gpio_set_dir(BTN_PIN, GPIO_IN);
  gpio_pull_up(BTN_PIN);

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  gpio_set_irq_enabled_with_callback(
    BTN_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (flag) {
      if (led_state) {
        gpio_put(LED_PIN, 0);
        led_state = 0;
      }
      else {
        gpio_put(LED_PIN, 1);
        led_state = 1;
      }
      flag = 0;
    }
  }
}
