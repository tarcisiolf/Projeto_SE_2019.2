#include "es_led.h"

#include <gpio.h>

int led_init(es_led_t *led, char *dev_name, u32_t pin)
{
    led->dev   = device_get_binding(dev_name);
    led->pin   = pin;
    led->value = 1;

    gpio_pin_configure(led->dev, pin, GPIO_DIR_OUT);
    return 0;
}

void set_led(es_led_t *led, u8_t value)
{
    led->value = value;
    gpio_write(led->dev, 0, led->pin, led->value);
}

void set_all_led(es_led_t *led0, es_led_t *led1, es_led_t *led2, es_led_t *led3, u8_t value)
{
    led0->value = value;
    gpio_write(led0->dev, 0, led0->pin, led0->value);

    led1->value = value;
    gpio_write(led1->dev, 0, led1->pin, led1->value);

    led2->value = value;
    gpio_write(led2->dev, 0, led2->pin, led2->value);

    led3->value = value;
    gpio_write(led3->dev, 0, led3->pin, led3->value);
}

void turn_leds(es_led_t *led0, es_led_t *led1, es_led_t *led2, es_led_t *led3,
                u8_t value0, u8_t value1, u8_t value2, u8_t value3)
{
    led0->value = value0;
    gpio_write(led0->dev, 0, led0->pin, led0->value);

    led1->value = value1;
    gpio_write(led1->dev, 0, led1->pin, led1->value);

    led2->value = value2;
    gpio_write(led2->dev, 0, led2->pin, led2->value);

    led3->value = value3;
    gpio_write(led3->dev, 0, led3->pin, led3->value);
}
