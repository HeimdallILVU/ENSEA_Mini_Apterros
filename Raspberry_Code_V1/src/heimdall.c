#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <bcm2835.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "heimdall.h"

struct timespec delay;



/* Init and Close function */

int Heimdall_init() {
    delay.tv_sec = 0;
    if (!bcm2835_init()) {
        printf("Heimdall_init (bcm2835_init) failed. Are you running as root??\n");
        return 0;
	}
    printf("init success\n");
    return 1;
}

int Heimdall_close() {
    return bcm2835_close();
}

/* GPIO commands */

void GPIO_set_pin_mode(uint8_t pin, uint8_t mode) {
    bcm2835_gpio_fsel(pin, mode);
}

void GPIO_write(uint8_t pin, uint8_t on) {
    bcm2835_gpio_write(pin, on);
}

/* I2C commands */

int I2C_begin(void){
    return bcm2835_i2c_begin();
}

void I2C_end(void) {
    bcm2835_i2c_end();
}

void I2C_write(i2c_entity i2c_entity, const char * buf, uint32_t len) {
    bcm2835_i2c_setSlaveAddress(i2c_entity.addr);
    bcm2835_i2c_set_baudrate(i2c_entity.baudrate);
    bcm2835_i2c_write(buf, len);
}

void I2C_read(i2c_entity i2c_entity, char * buf, uint32_t len) {
    bcm2835_i2c_setSlaveAddress(i2c_entity.addr);
    bcm2835_i2c_set_baudrate(i2c_entity.baudrate);
    bcm2835_i2c_read(buf, len);
}

void I2C_write_to_register(i2c_entity i2c_entity, uint8_t REGISTER, uint8_t DATA) {
    // Create an array to hold the register and data
    uint8_t buffer[2] = { REGISTER, DATA };

    bcm2835_i2c_setSlaveAddress(i2c_entity.addr);
    bcm2835_i2c_set_baudrate(i2c_entity.baudrate);

    // Write the register and data
    bcm2835_i2c_write((const char *)buffer, 2);
}

uint8_t I2C_read_from_register(i2c_entity i2c_entity, uint8_t REGISTER) {
    // Create an array to hold the register and data
    uint8_t buffer[1] = { REGISTER };

    bcm2835_i2c_setSlaveAddress(i2c_entity.addr);
    bcm2835_i2c_set_baudrate(i2c_entity.baudrate);

    // Write the register
    bcm2835_i2c_write((const char *)buffer, 1);

    // Read the data
    char DATA[1];
    bcm2835_i2c_read(DATA, 1);

    return DATA[0];
}

/* UART commands */

/* Uart Write in format bit start -> 8 data bits -> bit stop */
void UART_Write(uint8_t pin, uint64_t freq, char * buf) {

    uint64_t period = 1e9 / freq;
    for(int i = 0; i < strlen(buf); i++) {
        GPIO_write(pin, 0);
        Delay_Micro_accurate(period/1000);
        char char_i = buf[i];
        for(int j = 0; j < 8; j++) {
            GPIO_write(pin, (char_i & (1<<(7-j)))?1:0);
            Delay_Micro_accurate(period/1000);
        }
        GPIO_write(pin, 1);
        Delay_Micro_accurate(10 * period/1000);
    }
    GPIO_write(pin, 1);
}

void UART_Read(void);

void UART_Init(void);

/* Software PWM commands*/

void Sw_PWM_Update(void * psw_PWM) {
	Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
    //pthread_mutex_lock (& sw_PWM->mutex_data);

    sw_PWM->nano_delay = 1e9 / sw_PWM->frequency;

	sw_PWM->up_nano_delay = ((uint64_t) sw_PWM->nano_delay) * ((uint64_t) sw_PWM->alpha) / sw_PWM->range ;
	sw_PWM->down_nano_delay = ((uint64_t) sw_PWM->nano_delay * ((uint64_t) (sw_PWM->range - sw_PWM->alpha)) / sw_PWM->range) ;

	GPIO_set_pin_mode(sw_PWM->Pin, BCM2835_GPIO_FSEL_OUTP);
    //pthread_mutex_unlock (& sw_PWM->mutex_data);
}

static void * fn_pwm_waiter (void * psw_PWM) {
	Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
	while(1) {
        sw_PWM->state = 0;

		//pthread_cond_signal (& sw_PWM->cond_wait);
        GPIO_write(sw_PWM->Pin, 0);
		if (sw_PWM->down_nano_delay != 0) Delay_Micro_accurate(sw_PWM->down_nano_delay/1000);

        sw_PWM->state = 1;

		//pthread_cond_signal (& sw_PWM->cond_wait);
        GPIO_write(sw_PWM->Pin, sw_PWM->enable);
		if (sw_PWM->up_nano_delay != 0) Delay_Micro_accurate(sw_PWM->up_nano_delay/1000);
	}
	return NULL;
}

static void * fn_pwm_toggler (void * psw_PWM) {
	Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;

	while(1) {
        //pthread_mutex_lock (& sw_PWM->mutex_state);

		pthread_cond_wait (& sw_PWM->cond_wait, & sw_PWM->mutex_state);
		GPIO_write(sw_PWM->Pin, sw_PWM->state * sw_PWM->enable);

        //pthread_mutex_unlock (& sw_PWM->mutex_state);
	}
	return NULL;
}

int Sw_PWM_Init(void * psw_PWM) {

    Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
    Sw_PWM_Update(psw_PWM);

    int ret = 0;
	ret = pthread_create(& sw_PWM->thread_waiter, NULL, fn_pwm_waiter, sw_PWM);
	if(ret) {
		return 1;
	}

	ret = pthread_create(& sw_PWM->thread_toggler, NULL, fn_pwm_toggler, sw_PWM);		
	if(ret) {
		return 1;
	}
    return 0;
}

void Sw_PWM_set_frequency(void * psw_PWM, uint32_t frequency) {
    Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
    sw_PWM->frequency = frequency;
    Sw_PWM_Update(psw_PWM);
}

void Sw_PWM_set_nano_delay(void * psw_PWM, uint32_t nano_delay) {
    Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
    sw_PWM->nano_delay = nano_delay;
    Sw_PWM_Update(psw_PWM);
}

void Sw_PWM_set_range(void * psw_PWM, uint32_t range) {
    Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
    sw_PWM->range = range;
    Sw_PWM_Update(psw_PWM);
}

void Sw_PWM_enable(void * psw_PWM, uint8_t enable) {
    Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
    sw_PWM->enable = enable;
    Sw_PWM_Update(psw_PWM);
}

void Sw_PWM_set_alpha(void * psw_PWM, uint32_t alpha) {
    Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
    
    sw_PWM->alpha = alpha;
    Sw_PWM_Update(psw_PWM);
}

void Sw_PWM_join(void * psw_PWM) {
    Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
    pthread_join(sw_PWM->thread_toggler, NULL);
    pthread_join(sw_PWM->thread_waiter, NULL);
}

void Sw_PWM_cancel(void * psw_PWM) {
    Sw_PWM* sw_PWM = (Sw_PWM*) psw_PWM;
    pthread_cancel(sw_PWM->thread_toggler);
    pthread_cancel(sw_PWM->thread_waiter);
}

/* HardWare PWM commands */

void PWM_init(pwm_entity pwm) {
    PWM_set_clock(pwm.divisor);
    PWM_set_range(pwm, pwm.range);
    PWM_set_alpha(pwm, pwm.default_alpha);
    PWM_enable(pwm, pwm.default_enable);
}

void PWM_set_clock(uint32_t divisor) {
    bcm2835_pwm_set_clock(divisor);
}

void PWM_set_range(pwm_entity pwm_entity, uint32_t range) {
    bcm2835_pwm_set_range(pwm_entity.channel, range);
}

void PWM_enable(pwm_entity pwm_entity, uint8_t enable) {
    bcm2835_pwm_set_mode(pwm_entity.channel, 1, enable);
}

void PWM_set_alpha(pwm_entity pwm_entity, uint32_t alpha) {
    bcm2835_pwm_set_data(pwm_entity.channel, alpha);
}

/* Thread Handling */

/* Miscellaneous */

void Delay(unsigned int ms) {
    bcm2835_delay(ms);
}

void Delay_Micro_accurate(uint64_t delay) {
    delayMicroseconds(delay);
}

void Delay_nano(uint64_t nano_delay) {
    delay.tv_nsec = nano_delay;
    clock_nanosleep(CLOCK_REALTIME, 0, &delay, NULL);
}

/* Pre-Made funcion */

void Blink(uint8_t pin, unsigned int ns, uint8_t iteration) {

    if (iteration <= 0) {
        while(1) {
            bcm2835_gpio_write(pin, HIGH);
            Delay_nano(ns);
            bcm2835_gpio_write(pin, LOW);
            Delay_nano(ns);
        }
    } else {
        for (int i = iteration; i > 0; i--) {
            bcm2835_gpio_write(pin, HIGH);
            Delay_nano(ns);
            bcm2835_gpio_write(pin, LOW);
            Delay_nano(ns);
        }
    }
}