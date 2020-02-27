/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/pwm.h>
#include <drivers/gpio.h>


#include "es_button.h"
//#include "sensor_dht.h"


// Definindo o pino do Botão
#define BUTTON_DEVICE DT_ALIAS_SW0_GPIOS_CONTROLLER
#define BUTTON_PIN0 DT_ALIAS_SW0_GPIOS_PIN

/*
// Definindo os pinos do Sensor
#define SENSOR_DEVICE "I2C_0" // PINS 26 SDA E 27 SCL

// Sensor 
LOG_MODULE_REGISTER(dht, LOG_LEVEL_DGB)
struct device *dev;
u8_t temperature, relative_humidity;
*/

// Definindo o Semáforo
K_SEM_DEFINE(data_sem, 1, 1);

// Definindo Fila de Mensagem 
K_MSGQ_DEFINE(my_msgq, sizeof(u32_t), 10, 4);

// Definindo tempo de espera
#define SLEEP_TIME 500

// Definindo tempo do PWM
#define PWM_TIME 50

// Tamanho da STACK
#define STACKSIZE 1024

// Prioridade das Threads
#define PRIORITY 0

// PWM

//#if defined(DT_ALIAS_PWM_LED0_PWMS_CONTROLLER) && defined(DT_ALIAS_PWM_LED0_PWMS_CHANNEL)
/* get the defines from dt (based on alias 'pwm-led0') */
#define PWM_DRIVER	DT_ALIAS_PWM_LED0_PWMS_CONTROLLER
#define PWM_CHANNEL	DT_ALIAS_PWM_LED0_PWMS_CHANNEL
//#else
//#error "Choose supported PWM driver"
//#endif


#define PERIOD (USEC_PER_SEC / 50U)
// --------------------------------------- //


es_button_t button0 = {0};

u8_t button0_pressed = 0;
u8_t state = 0;
u8_t data = 0;

struct device *pwm_dev;

void button0_callback(struct device *gpiob, struct gpio_callback *cb, u32_t pins)
{
    printk("Button 0 pressed!\n");
    button0_pressed = 1;
    printk("Button 0 Value [%d]\n", button0_pressed);
}


void main(void)
{
    printk("Inicializando o Botão\n");
    button_create(&button0, BUTTON_DEVICE, BUTTON_PIN0, button0_callback);
    printk("Botão OK\n");

   // BEGIN PWM
    printk("Inicializando o device PWM\n");

    pwm_dev = device_get_binding(PWM_DRIVER);

    if (!pwm_dev) 
    {
        printk("Cannot find %s!\n", PWM_DRIVER);
    }

    else
    {
        printk("PWM device inicializado\n");
    }
    // END PWM

    // TODO - INICIALIZAR O SENSOR DHT
    /*
    // Criando o Sensor_DHT
    dev = device_get_binding(CONFIG_DHT_NAME);
	if (dev == NULL) 
    {
		LOG_ERR("Could not get DHT device\n");
	}
    */

    printk("Hello Main Thread\n");

    while(1)
    {
        k_sleep(K_SECONDS(1));
        printk(". main %d\n", data);
    }
}


void readThread(void)
{
    printk("Hello Read Thread\n");
    
    while(1)
    {
        k_sleep(K_MSEC(SLEEP_TIME));
        k_sem_take(&data_sem, K_FOREVER);

        //ret_value = on_get_humidity(); // Função printa internamente
        //ret_temperature = on_get_temperature(); // Função printa internamente

        // Teste Sincronização das Threads 

        data += 1; 
        k_sem_give(&data_sem);	
    }
	
}


// THREAD DOS BOTÕES
// Ao pressionar o botão LED PWM ativado
void buttonPWMThread(void)
{
        printk("Hello button Thread\n");
        u8_t step = 1;
        u8_t counter = 0;

        //PWM


        while(1)
        {   
            k_sleep(K_MSEC(PWM_TIME));
            if(button0_pressed == 1)    
            {
                if (pwm_pin_set_usec(pwm_dev, PWM_CHANNEL, PERIOD, (PERIOD / 50) * step)) 
                {
                    printk("pwm pin set fails\n");
                }

                step++;
                if (step == 50) 
                {
                    step = 1;
                    counter += 1;
                }

                if (counter == 2)
                {
                    button0_pressed = 0;
                    counter = 0;
                    pwm_pin_set_usec(pwm_dev, PWM_CHANNEL, PERIOD, (PERIOD / 50) * 1);
                }
            }
        }
}

// TODO - MÁQUINA DE ESTADOS


// THREAD de Sincronização
void printThread(void)
{
    printk("Hello Print Thread\n");
    u8_t test = 0;

    while(1)
    {
        k_sleep(K_MSEC(SLEEP_TIME));
        k_sem_take(&data_sem, K_FOREVER);
        test = data;
        printk("Value of data [%d]\n", test);
        k_sem_give(&data_sem);	
    }
}

K_THREAD_DEFINE(readid, STACKSIZE, readThread, NULL, NULL, NULL,
		PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(printid, STACKSIZE, printThread, NULL, NULL, NULL,
		PRIORITY, 0, K_NO_WAIT);

K_THREAD_DEFINE(buttonid, STACKSIZE, buttonPWMThread, NULL, NULL, NULL,
		PRIORITY, 0, K_NO_WAIT);

/*
K_THREAD_DEFINE(machineid, STACKSIZE, machineThread, NULL, NULL, NULL,
		PRIORITY, 0, K_NO_WAIT);
*/

