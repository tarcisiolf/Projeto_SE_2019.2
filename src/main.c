#include <zephyr.h>
#include <device.h>
#include <gpio.h>
#include <misc/printk.h>
#include <shell/shell.h>
#include <misc/util.h>
#include <display/cfb.h>
#include <stdio.h>
#include <drivers/pwm.h>

#include "setup.h"
#include "input.h"
#include "output.h"
#include "sensor_dht.h"

// Structs globais de saida do tipo output_t
output_t led_zero, led_one, led_two, led_three; // Leds

// Sensor Handling
LOG_MODULE_REGISTER(dht, LOG_LEVEL_DBG);
struct device *dev;
int temperature, relative_humidity;

// Configurando message queue
K_MSGQ_DEFINE(my_msq, sizeof(int), 10, 4);

//Configurando Semáforo
K_SEM_DEFINE(var_sem, 1, 1);

u8_t data = 0;

/* Comando Shell press -> parametros: (Struct do tipo shell, argc é o numero de argumentos do comando, argv é um array de tring
   contendo os argumentos em si)*/
static int cmd_run_press(const struct shell *shell, size_t argc, char **argv) {

        // Acessar: Array de Strings -> String na posição argc+1
        char led = *(*(argv+1));

        switch (led) {
        case '0':
            output_pressed(&led_zero);
            break;
        case '1':
            output_pressed(&led_one);
            
            break;
        case '2':
            output_pressed(&led_two);
            break;
        case '3':
            output_pressed(&led_three);
            break;
        default:
            shell_print(shell, "Botao invalido");
            return -1;
        }
        // Printando no terminal do shell que a ação foi executada
        shell_print(shell, "Led %c pressionado.", led);
        return 0;
}

// Colocando os comandos criados como subcomandos do run
SHELL_STATIC_SUBCMD_SET_CREATE(sub_run,
    SHELL_CMD_ARG(press, NULL, "Comando Press.", cmd_run_press, 1, NULL)
    SHELL_SUBCMD_SET_END
);

// Setando o comando run no root
SHELL_CMD_REGISTER(run, &sub_run, "Comandos Run.", NULL);

// Função para botão pressionado
void button_pressed(struct device *btn, struct gpio_callback *cb, u32_t pins){
    if(pins & BIT(SW0_PIN)) {
        output_pressed(&led_zero);
        printk("Botao 0 pressionado em %d ciclos\n", k_cycle_get_32());
    } else if(pins & BIT(SW1_PIN)) {
        output_pressed(&led_one);
        printk("Botao 1 pressionado em %d ciclos\n", k_cycle_get_32());
    } else if(pins & BIT(SW2_PIN)) {
        output_pressed(&led_two);
        printk("Botao 2 pressionado em %d ciclos\n", k_cycle_get_32());
    } else {
        output_pressed(&led_three);
        printk("Botao 3 pressionado em %d ciclos\n", k_cycle_get_32());
    }
}

// Threads

void readThread(void)
{
	int ret_humidity;
    int ret_temperature;
	printk("Hello readThread!\n");
	
	while (1)
	{
		k_sleep(K_MSEC(500));
		//printk(".thread 2: %d\n", data);

		k_sem_take(&var_sem, K_FOREVER);
		data += 10;

		// escrever o valor da valor do sensor em algum local 
		// e depois liberar o uso para o próxima thread 
        ret_value = on_get_humidity(); // Função printa internamente
        ret_temperature = on_get_temperature(); // Função printa internamente

		k_sem_give(&var_sem);		
	}
	
}

void buttonsThread(void)
{
    u32_t val = 0U;

	while (1)
	{
		k_sem_take(&var_sem, K_FOREVER);
		//print("Dado ->%d\n", data);

        input_read(&btn0, &val);
        input_read(&btn1, &val);

        // LED Pisca indicando funcionamento normal
        if (pwm_pin_set_usec(pwm_dev, PWM_CHANNEL, PERIOD, (PERIOD/50) * step))
        {
            
            printk("pwm pin set fails\n");
            return;
        }
        
        step++;

        if (step == 50)
        {
            step = 1;
        }

        k_sleep(K_MSEC(50));

        //input_read(&btn2, &val);
        //input_read(&btn3, &val);

		k_sem_give(&var_sem);
	}
	
}


void main(void) {

    // Structs do tipo input_t representando cada botão
    input_t btn0, btn1, btn2, btn3;

    // Estado inicial dos leds
    u8_t state = 1; // Inicialmente apagados

    // Criando e definindo Leds
    led(&led_zero, led_zero_PIN, led_zero_PORT, state);
    led(&led_one, led_one_PIN, led_one_PORT, state);
    led(&led_two, led_two_PIN, led_two_PORT, state);
    led(&led_three, led_three_PIN, led_three_PORT, state);

    printk("\n");

    // Criando e definindo botoes
    button(&btn0, SW0_PIN, SW0_PORT);
    button(&btn1, SW1_PIN, SW1_PORT);
    //button(&btn2, SW2_PIN, SW2_PORT);
    //button(&btn3, SW3_PIN, SW3_PORT);

    // Criando o Sensor_DHT
    dev = device_get_binding(CONFIG_DHT_NAME);
	if (dev == NULL) {
		LOG_ERR("Could not get DHT device\n");
		}

    printk("Aguardando comandos ...\n");
    while(1) {
        k_sleep(SLEEP_TIME);
    }
}

K_THREAD_DEFINE(readThread_id, STACKSIZE, readThread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);
K_THREAD_DEFINE(buttonsThread_id, STACKSIZE, printThread, NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);