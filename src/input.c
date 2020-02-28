#include <gpio.h>

//#include "setup.h"
#include "input.h"

int input_open(input_t *in, char *dev_label) {
    in->dev_label = dev_label;
    in->device = device_get_binding(in->dev_label);

    if(in->device == NULL) {
      printk("[INPUT] Error: Device not found or cannot be used.\n");
      return 1;
    }
    return 0;
}

int input_configure(input_t *in, u32_t pin, int flags) {
    in->pin = pin;
    gpio_pin_configure(in->device, in->pin, flags);
    return 0;
}

int input_read(input_t *in, u32_t *state) {
    gpio_pin_read(in->device, in->pin, state);
    return 0;
}
