#include "output.h"
#include <gpio.h>
#include <drivers/pwm.h>

#include "setup.h"

int output_open(output_t *out, char *dev_label) {
    out->dev_label = dev_label;
    out->device = device_get_binding(out->dev_label);
    //printk("output_open done\n");
     if(!out->device){
        printk("Função: output_open (arquivo: ouput.c).\nFalha na atribução do out->device.\n");
        return 1;
    }
    return 0;
}

int output_configure(output_t *out, u32_t pin, int flags) {
    out->pin = pin;
    gpio_pin_configure(out->device, out->pin, flags);
    //printk("output_configure done\n");
    return 0;
}

int output_set(output_t *out, u8_t value) {
    out->state = value;
    gpio_pin_write(out->device, out->pin, out->state);
    //printk("output_set done\n");
    return 0;
}

int output_pressed(output_t *out) {
    if(out->state == 1) {
        out->state = 0;
        gpio_pin_write(out->device, out->pin, out->state);
    } else {
        out->state = 1;
        gpio_pin_write(out->device, out->pin, out->state);
    }
    //printk("output_pressed done\n");
    return 0;
}


// Inicializador Led
void led(output_t *led, u32_t pin, char *port, u8_t state){
    int e = output_open(led, port);
    if(e){
        printk("Alerta: Led para o PIN%d não inicializado.\n", pin);
        return;
    }
    output_configure(led, pin, GPIO_DIR_OUT);
    output_set(led, state);
    printk("Led em PIN%d inicializado.\n", pin);
}
