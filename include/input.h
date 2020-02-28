
#ifndef _INPUT_H_
#define _INPUT_H_

#include <sys/printk.h>
#include <device.h>
#include <gpio.h>

typedef struct input input_t;

struct input {
    struct device *device;
    struct gpio_callback gpio_callback;
    char *dev_label;
    u32_t pin;
};


/**
 * Essa função tem como objetivo iniciar o device do driver
 * de entrada.
 * @param in
 *
 * @return
 */
int input_open(input_t *in, char *dev_label);

/**
 * Essa função tem como objetivo configurar o pino de entrada.
 *
 * @param in
 * @param pin
 * @param flags
 * @param cb
 *
 * @return
 */
int input_configure(input_t *in, u32_t pin, int flags);

/**
 * Essa função tem como objetivo fazer uma leitura do pino
 * de entrada.
 * @param in
 * @param state
 *
 * @return
 */
int input_read(input_t *in, u32_t *state);

#endif
