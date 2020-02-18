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
K_SEM_DEFINE(var_sem, 1, 1);

/*
void button_pressed(struct device *gpiob, struct gpio_callback *cb, u32_t pins)
{
    printk("Button 0 pressed!\n");
    // mudar valor da variável que fara o estado da máquina mudar
}

// Jogar isso na main
//button_create(&button0, BUTTON_DEVICE, BUTTON_PIN0, button0_callback);
*/

u8_t data = 0;

void main(void)
{
	printk("Hello main thread!\n");
	while (1)
	{
		k_sleep(K_SECONDS(1));
		printk(".main %d\n, var", data);
	}
	
}

void readThread(void)
{
	
	printk("Hello readThread!\n");
	
	while (1)
	{
		k_sleep(K_MSEC(500));
		printk(".thread 2: %d\n", data);

		k_sem_take(&var_sem, K_FOREVER);
		data += 10;
		// escrever o valor da valor do sensor em algum local 
		// e depois liberar o uso para o próxima thread
		k_sem_give(&var_sem);		
	}
	
}

void printThread(void)
{
	printk("Hello printThread!\n");

	while (1)
	{
		k_sem_take(&var_sem, K_FOREVER);
		print("Dado ->%d\n", data);
		// Printar o estado da máquina e o valor do sensor
		k_sem_give(&var_sem);
	}
	
}

K_THREAD_DEFINE(readThread_id, STACKSIZE, readThread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(printThread_id, STACKSIZE, printThread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
