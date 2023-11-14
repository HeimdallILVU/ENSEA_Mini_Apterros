
/* Typedef */

/*!
Struct used to itemize each I2C

    \param addr Is the Addres of the I2C Slave
    \param baudrate Is the Baudrate that will be used with this Slave
*/
typedef struct  {
    uint8_t addr;
    uint32_t baudrate;
} i2c_entity;

/*!
Struct used to itemize each PWM

    \param channel 0 or 1 to select which Hardware PWM you want
    \param divisor A PWMClockDivider value (eg. PWM_CLOCK_DIVIDER_128)
    \param range The definition of the Duty Cycle
    \param default_alpha The default value for alpha (Duty Cycle = Alpha/Range)
    \param default_enable The default value stating wether the pwm is enabled during init.
*/
typedef struct  {
    uint8_t channel;
    uint32_t divisor;
    uint32_t range;
    uint32_t default_alpha;
    uint8_t default_enable;
} pwm_entity;

//struct Software_PWM
typedef struct {
	uint8_t channel; //Handling of multiple Sw_PWM
	uint8_t Pin; //Physical Pin
	uint8_t state; //State of the output
	uint8_t enable; //Enable of the output
	uint32_t range; //Range of the Duty Cycle
	uint32_t alpha; //Duty Cycle = alpha/range
    uint32_t frequency; //Base frequency of the PWM.
	uint32_t nano_delay; //(Automaticaly calculated) Period in ns of the PWM (P = 1/T) e.g for 50Hz, nano_delay = 20,000,000

	uint64_t up_nano_delay; //Delay to wait at 1 (Automaticaly calculated)
	uint64_t down_nano_delay; //DElay to wait at 0 (Automaticaly calculated)

	//We need to separate both of them, as toggling the Pin is not instantaneous at very high frequencies.
	pthread_t thread_waiter; //The waiting thread
	pthread_t thread_toggler; //The toggler thread 

	pthread_mutex_t mutex_state; //The mutex to protect the state value.
	pthread_mutex_t mutex_data; //The mutex to protect the data values.
	pthread_cond_t cond_wait; //The condition that the toggler will wake up from.
} Sw_PWM;

/*
Initialize this very "useful" library

\return 1 if successful else 0
*/
int Heimdall_init(void);

/*! Close the library, deallocating any allocated memory and closing /dev/mem
    \return 1 if successful else 0
*/
int Heimdall_close(void);


/*! Sets the output state of the specified Pin.
    \param[in] pin GPIO number, or one of RPI_GPIO_P1_* from \ref RPiGPIOPin.
    \param[in] on HIGH sets the output to HIGH and LOW to LOW.
*/
void GPIO_write(uint8_t pin, uint8_t on);


/*! set the pin as Input, Output or one of the 6 alternate functions.
    \param[in] pin GPIO number, eg (RPI_GPIO_P1_1) <- Name for the GPIO1.
    \param[in] mode Mode to set the pin to (BCM2835_GPIO_FSEL_INPT or BCM2835_GPIO_FSEL_OUTP)
*/
void GPIO_set_pin_mode(uint8_t pin, uint8_t mode);

/*! Start I2C operations.
    \return 1 if successful, 0 otherwise (perhaps because you are not running as root)
*/
int I2C_begin(void);

/*! End I2C operations.
*/
void I2C_end(void);

/*! Transfers any number of bytes to the I2C Slave designated trough i2c_entity.

    \param[in] i2c_entity i2c_entity designating the target slave.
    \param[in] buf Buffer of bytes to send.
    \param[in] len Number of bytes in the buf buffer, and the number of bytes to send.
*/
void I2C_write(i2c_entity i2c_entity, const char * buf, uint32_t len);

/*! Read any number of bytes of the I2C Slave designated trough i2c_entity.

    \param[in] i2c_entity i2c_entity designating the target slave.
    \param[in] buf Buffer of bytes to read.
    \param[in] len Number of bytes to read.
*/
void I2C_read(i2c_entity i2c_entity, char * buf, uint32_t len);

/*! Write a byte to a register

    \param[in] i2c_entity i2c_entity designating the target slave.
    \param[in] REGISTER One Byte that is the address of the register to write to.
    \param[in] DATA One Byte that is the data to write inside the register.
*/
void I2C_write_to_register(i2c_entity i2c_entity, uint8_t REGISTER, uint8_t DATA);

/*! Read a byte from a register

    \param[in] i2c_entity i2c_entity designating the target slave.
    \param[in] REGISTER One Byte that is the address of the register to read from.
    \return A bytes of data that was in the register.
*/
uint8_t I2C_read_from_register(i2c_entity i2c_entity, uint8_t REGISTER);


/*! Updates values (Used only inside the library, not made to be used from outside)

    \param[in] psw_PWM pointer for the Sw_PWM.
*/
void Sw_PWM_Update(void * psw_PWM);


/*! Will return once the Sw_PWM thread is finished (Will never return as they are while(1) loop)

    \param[in] psw_PWM pointer for the Sw_PWM.
*/
void Sw_PWM_join(void * psw_PWM);

/*! Cancel the thread located for the PWM. (Don't use it)

    \param[in] psw_PWM pointer for the Sw_PWM.
*/
void Sw_PWM_cancel(void * psw_PWM);


/*! Initialize the pwm with the default values of the pwm_entity

    \param[in] psw_PWM pointer for the Sw_PWM struct containing all the pwm default values.
*/
int Sw_PWM_Init(void * psw_PWM);

/*! Set the frequency of the PWM

    \param[in] psw_PWM pointer for the Sw_PWM struct
    \param[in] frequency The frequency of the PWM signal.
*/
void Sw_PWM_set_frequency(void * psw_PWM, uint32_t frequency);

/*! Set the frequency of the PWM

    \param[in] psw_PWM pointer for the Sw_PWM struct
    \param[in] nano_delay The Period in nano seconds of the PWM signal P = 1/F
*/
void Sw_PWM_set_nano_delay(void * psw_PWM, uint32_t nano_delay);

/*! Set the range for the PWM

    \param[in] psw_PWM pointer for the Sw_PWM struct
    \param[in] range The Duty Cycle is calculated as alpha/range
*/
void Sw_PWM_set_range(void * psw_PWM, uint32_t range);

/*! Enable or Disable the PWM

    \param[in] psw_PWM pointer for the Sw_PWM struct
    \param[in] enable 1 for True (Activated), 0 for False (Deactivated)
*/
void Sw_PWM_enable(void * psw_PWM, uint8_t enable);

/*! Set the Duty Cycle for the PWM

    \param[in] psw_PWM pointer for the Sw_PWM struct
    \param[in] range The Duty Cycle is calculated as alpha/range
*/
void Sw_PWM_set_alpha(void * psw_PWM, uint32_t alpha);




/*! Initialize the pwm with the default values of the pwm_entity

    \param[in] pwm_entity pwm_entity containing all the pwm default values.
*/
void PWM_init(pwm_entity pwm_entity);

/*! \brief PWMClockDivider
  Specifies the divider used to generate the PWM clock from the system clock.
  Figures below give the divider, clock period and clock frequency.
  Clock divided is based on nominal PWM base clock rate of 19.2MHz
  The frequencies shown for each divider have been confirmed by measurement
*/
typedef enum
{
    PWM_CLOCK_DIVIDER_2048  = 2048,    /*!< 2048 = 9.375kHz */
    PWM_CLOCK_DIVIDER_1024  = 1024,    /*!< 1024 = 18.75kHz */
    PWM_CLOCK_DIVIDER_512   = 512,     /*!< 512 = 37.5kHz */
    PWM_CLOCK_DIVIDER_256   = 256,     /*!< 256 = 75kHz */
    PWM_CLOCK_DIVIDER_128   = 128,     /*!< 128 = 150kHz */
    PWM_CLOCK_DIVIDER_64    = 64,      /*!< 64 = 300kHz */
    PWM_CLOCK_DIVIDER_32    = 32,      /*!< 32 = 600.0kHz */
    PWM_CLOCK_DIVIDER_16    = 16,      /*!< 16 = 1.2MHz */
    PWM_CLOCK_DIVIDER_8     = 8,       /*!< 8 = 2.4MHz */
    PWM_CLOCK_DIVIDER_4     = 4,       /*!< 4 = 4.8MHz */
    PWM_CLOCK_DIVIDER_2     = 2,       /*!< 2 = 9.6MHz, fastest you can get */
    PWM_CLOCK_DIVIDER_1     = 1        /*!< 1 = 4.6875kHz, same as divider 4096 */
} PWMClockDivider;

/*! Set the clock speed by setting the clock divisor. 
    Be carefull ! there is two hardware PWM but only one clock speed for both

    \param[in] divisor divisor of the clock. Can be a PWMClockDivider (eg. PWM_CLOCK_DIVIDER_512 or 69)
*/
void PWM_set_clock(uint32_t divisor);

/*! Set the range for the PWM

    \param[in] pwm_entity pwm_entity. DUH !
    \param[in] range The Duty Cycle is calculated as alpha/range
*/
void PWM_set_range(pwm_entity pwm_entity, uint32_t range);

/*! Enable or Disable the PWM

    \param[in] pwm_entity pwm_entity. DUH !
    \param[in] enable 1 for True (Activated), 0 for False (Deactivated)
*/
void PWM_enable(pwm_entity pwm_entity, uint8_t enable);

/*! Set the Duty Cycle for the PWM

    \param[in] pwm_entity pwm_entity. DUH !
    \param[in] range The Duty Cycle is calculated as alpha/range
*/
void PWM_set_alpha(pwm_entity pwm_entity, uint32_t alpha);


/*! Delays for the specified number of milliseconds.
    Thread safe (Doesn't use the CPU during the wait).
    \param[in] millis Delay in milliseconds
*/
void Delay(unsigned int ms);

/*! Delays for the specified number of microseconds.
    Thread safe (Doesn't use the CPU during the wait).
    \param[in] delay Delay in microseconds
*/
void Delay_Micro_accurate(uint64_t delay);

/*! Delays for the specified number of nanoseconds.
    Thread safe (Doesn't use the CPU during the wait).
    \param[in] nano_delay Delay in nano seconds
*/
void Delay_nano(uint64_t nano_delay) ;

/*! Premade function to blink a pin.
    \param[in] pin GPIO number, eg (RPI_GPIO_P1_1) <- Name for the GPIO1.
    \param[in] millis Delay in milliseconds
    \param[in] iteration Number of blink. (negative or null values for infinite)
*/
void Blink(uint8_t pin, unsigned int ms, uint8_t iteration);