// DEFINIÇÕES DOS COMPONENTES DO PROJETO //

// Definindo o pino do Botão
#define BUTTON_DEVICE DT_ALIAS_SW0_GPIOS_CONTROLLER
#define BUTTON_PIN0 DT_ALIAS_SW0_GPIOS_PIN

#define XL2_PORT DT_ALIAS_SW2_GPIOS_CONTROLLER	//pin P0.01 / XL2
#define XL2_PIN DT_ALIAS_SW2_GPIOS_PIN  		//pin P0.01 / XL2 AJUSTAR CASO NECESSITE COLOCAR O 'S'

// Definindo tempo de espera
#define SLEEP_TIME 500

// Definindo tempo do PWM
#define PWM_TIME 100

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

// Flags de interrupções do GPIO
#define INTERRUPT GPIO_INT
#define FALLING_EDGE (GPIO_INT_EDGE | GPIO_INT_ACTIVE_LOW)
#define DEBOUNCE GPIO_INT_DEBOUNCE
#define PULL_UP GPIO_PUD_PULL_UP
#define SW_GPIO_FLAGS (INTERRUPT | FALLING_EDGE | DEBOUNCE | PULL_UP)
