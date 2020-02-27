#include "es_button.h"

int button_create(es_button_t* button, const char* dev_name, u32_t pin,
                     gpio_callback_handler_t callback)
{
    button->dev = device_get_binding(dev_name);
    button->pin = pin;
    gpio_pin_configure(button->dev, pin,
                       GPIO_DIR_IN | GPIO_PUD_PULL_UP | GPIO_INT | GPIO_INT_DEBOUNCE
                           | GPIO_INT_EDGE | GPIO_INT_ACTIVE_HIGH);
    gpio_init_callback(&button->gpio_cb, callback, BIT(pin));
    gpio_add_callback(button->dev, &button->gpio_cb);
    gpio_pin_enable_callback(button->dev, pin);
    return 0;
}
