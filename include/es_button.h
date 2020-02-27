
#ifndef _ES_BUTTON_H_
#define _ES_BUTTON_H_

#include <device.h>
#include <drivers/gpio.h>
#include <zephyr/types.h>

typedef struct button_s es_button_t;

struct button_s {
    struct device* dev;
    struct gpio_callback gpio_cb;
    u32_t pin;
};

int button_create(es_button_t* button, const char* dev_name, u32_t pin,
                     gpio_callback_handler_t callback);

#endif /*_ES_BUTTON_H_*/