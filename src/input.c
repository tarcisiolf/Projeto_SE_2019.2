#include <gpio.h>

#include "setup.h"
#include "input.h"

//static struct gpio_callback callback;
int input_open(input_t *in, char *dev_label) {
    in->dev_label = dev_label;
    in->device = device_get_binding(in->dev_label);
    if(!in->device){
        printk("Função: input_open (arquivo: input.c).\nFalha na atribução do in->device.\n");
        return 1;
    }
    return 0;
}

int input_configure(input_t *in, u32_t pin, int flags, gpio_callback_handler_t cb) {
    in->pin = pin;
    gpio_pin_configure(in->device, in->pin, flags);
    gpio_init_callback(&in->gpio_callback, cb, BIT(in->pin));
    gpio_add_callback(in->device, &in->gpio_callback);
    gpio_pin_enable_callback(in->device, in->pin);
    return 0;
}

int input_read(input_t *in, u32_t *state) {
    gpio_pin_read(in->device, in->pin, state);
    return 0;
}

// Inicializador Botão
void button(input_t *button, u32_t pin, char *port){
    int e = input_open(button, port);
    if(e){
        printk("Alerta: Botao para o PIN%d não inicializado.\n", pin);
        return;
    }
    input_configure(button, pin, GPIO_DIR_IN | SW_GPIO_FLAGS, button_pressed);
    printk("Botao em PIN%d inicializado.\n", pin);
}