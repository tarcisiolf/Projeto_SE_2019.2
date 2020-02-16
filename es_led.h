#ifndef _ES_LED_H_
#define _ES_LED_H_

#include <device.h>
#include <zephyr/types.h>

typedef struct led_s es_led_t;

struct led_s {
    struct device *dev;
    u32_t pin;
    u8_t value;
};

int led_init(es_led_t *led, char *dev_name, u32_t pin);

void set_led(es_led_t *led, u8_t value);

void set_all_led(es_led_t *led0, es_led_t *led1, es_led_t *led2, es_led_t *led3, u8_t value);

void turn_leds(es_led_t *led0, es_led_t *led1, es_led_t *led2, es_led_t *led3,
                u8_t value0, u8_t value1, u8_t value2, u8_t value3);


#endif  // _ES_LED_H_