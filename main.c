#include <zephyr.h>
#include <gpio.h>
#include <misc/util.h>
#include <display/cfb.h>
#include <misc/printk.h>
#include <shell/shell.h>
#include <stdio.h>

#define STACKSIZE 1024

// Definindo o Botão
#define BUTTON_DEVICE DT_ALIAS_SW0_GPIOS_CONTROLLER
#define BUTTON_PIN0 DT_ALIAS_SW0_GPIOS_PIN

// Configurando message queue
K_MSGQ_DEFINE(my_msq, sizeof(int), 10, 4);

//Configurando Semáforo
K_SEM_DEFINE(my_sem, 0, 1);

void button_pressed(struct device *gpiob, struct gpio_callback *cb, u32_t pins)
{
    printk("Button 0 pressed!\n");
    // mudar valor da variável que fara o estado da máquina mudar
}

// Jogar isso na main
//button_create(&button0, BUTTON_DEVICE, BUTTON_PIN0, button0_callback);

void readThread(void)
{

}

void comunicationThread(void)
{

}

void main()
{

}