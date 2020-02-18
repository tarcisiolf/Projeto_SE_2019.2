
#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/pwm.h>

#if defined(DT_ALIAS_PWM_LED0_PWMS_CONTROLLER) && defined(DT_ALIAS_PWM_LED0_PWMS_CHANNEL)
/* get the defines from dt (based on alias 'pwm-led0') */
#define PWM_DRIVER	DT_ALIAS_PWM_LED0_PWMS_CONTROLLER
#define PWM_CHANNEL	DT_ALIAS_PWM_LED0_PWMS_CHANNEL
#else
#error "Choose supported PWM driver"
#endif

#define PERIOD (USEC_PER_SEC / 50U)

void main(void)
{
    struct device *pwm_dev;
    printk("PWM demo app-fade LED\n");

    pwm_dev = device_get_binding(PWM_DRIVER);

    if(!pwm_dev)
    {
        printk("Cannot find %s!\n", PWM_DRIVER);
        return;
    }
    
    u8_t step = 1;

    while(1)
    {
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
        
    }

}
