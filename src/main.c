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
#include "setup.h"
#include "input.h"


// Definindo o Semáforo
K_SEM_DEFINE(data_sem, 1, 1);

// Definindo Fila de Mensagem 
K_MSGQ_DEFINE(my_msgq, sizeof(u32_t), 10, 4);

// --------------------------------------- //
enum { READING, WRITING, NONE} state = NONE;
typedef enum {SENSING, ALERT} event_t;

event_t current_event = SENSING;

es_button_t button0 = {0};

u8_t button0_pressed = 0;
u8_t data = 0;
u32_t val = 0;
struct device *pwm_dev;
input_t sensor1;

void state_machine_action()
{
    switch (state) {
        case READING:
            input_read(&sensor1, &val);
            printk("Realizando a aferição normalmente\n");
            break;
        case WRITING:
            input_read(&sensor1, &val);
            printk("Valor transferido para central: %d.\n", val);
            break;
        case NONE:
            printk("Em espera\n");
    }
}

void state_machine(event_t event)
{
    switch (state) 
    {
        case WRITING: 
        {
            if(event == ALERT) {
	        state = READING;
	} else if (event == SENSING) {
            state = WRITING;}
    	} break;
	case READING: {
            if(event == ALERT) {
	        state = WRITING;
	} else if (event == SENSING) {
            state = READING;}
    	} break;
    case NONE: {
        if(event == ALERT) {
            state = WRITING;
        } else if (event == SENSING) {
            state = NONE; }
        }
	default:
	    state = READING;
	}
	state_machine_action();
}

void button0_callback(struct device *gpiob, struct gpio_callback *cb, u32_t pins)
{
    printk("Button 0 pressed!\n");
    button0_pressed = 1;
    printk("Button 0 Value [%d]\n", button0_pressed);
}

void sensor(input_t *sensor, u32_t pin, char *port){
    int e = input_open(sensor, port);
    if(e){
        printk("Alerta: Sensor para o PIN%d não inicializado.\n", pin);
        return;
    }
    input_configure(sensor, pin, GPIO_DIR_IN | SW_GPIO_FLAGS);
    printk("Sensor em PIN%d inicializado.\n", pin);
}

void main(void)
{
// ---------------------------------------------
    printk("Inicializando o Botão\n");
    button_create(&button0, BUTTON_DEVICE, BUTTON_PIN0, button0_callback);
    printk("Botão OK\n");
// ---------------------------------------------
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
// ---------------------------------------------
    // SENSOR INITIALIZATION
    //input_t sensor1;
    sensor(&sensor1, XL2_PIN, XL2_PORT);
    // END SENSOR
// ---------------------------------------------
    printk("Hello Main Thread\n");

    while(1)
    {
        k_sleep(K_SECONDS(1));
    }
}

// ------------------------------------------------------------
// ------------------------------------------------------------
//  Thread de leitura
void readThread(void)
{
    printk("Hello Read Thread\n");
    
    while(1)
    {	
	    //u32_t val = 0U;
        k_sleep(K_MSEC(SLEEP_TIME));
        k_sem_take(&data_sem, K_FOREVER);

        // ÁREA RESERVADA
	
	current_event = SENSING;
    //printk("STATE - [%d] | CURRENT EVENT - [%d] - READ THREAD\n", state, current_event);
	state_machine(current_event);

	//SAINDO DA ÁREA RESERVADA

        //data += 1; 
        k_sem_give(&data_sem);	
    }
	
}
// ------------------------------------------------------------
// ------------------------------------------------------------

// THREAD DOS BOTÕES - Ao pressionar o botão LED PWM ativado
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
                printk("Value of BUTTON 0 [%d]\n", button0_pressed);
		        current_event = ALERT;
                //printk("STATE - [%d] | CURRENT EVENT - [%d] - BUTTON THREAD\n", state, current_event);
		        state_machine(current_event);

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

// ------------------------------------------------------------
// ------------------------------------------------------------

// THREAD de Sincronização
void printThread(void)
{
    printk("Hello Print Thread\n");
    u8_t test = 0;

    while(1)
    {
        k_sleep(K_MSEC(SLEEP_TIME));
        k_sem_take(&data_sem, K_FOREVER);
	// ÁREA RESERVADA
	
	current_event = SENSING;
    //printk("STATE - [%d] | CURRENT EVENT - [%d] - PRINT THREAD\n", state, current_event);
	state_machine(current_event);

        test = data;
        printk("Value of data [%d]\n", test);

	//SAINDO DA ÁREA RESERVADA
        k_sem_give(&data_sem);	
    }
}

// ------------------------------------------------------------
// ------------------------------------------------------------

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

