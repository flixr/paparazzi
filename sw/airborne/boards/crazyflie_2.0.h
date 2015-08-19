/* CrazyFlie 2.0
 * https://www.bitcraze.io/crazyflie-2/
 * STM32F405 168MHz, 192kb SRAM, 1Mb flash
 * IMU MPU9250
 * High precision pressure sensor (LPS25H)
 */

#ifndef CONFIG_CRAZYFLIE_2_0_H
#define CONFIG_CRAZYFLIE_2_0_H

#define BOARD_CRAZYFLIE_2

#define EXT_CLK 8000000
#define AHB_CLK 168000000

/*
 * Onboard LEDs
 */

/* red: right on M1 */
#ifndef USE_LED_1
#define USE_LED_1 1
#endif
#define LED_1_GPIO GPIOC
#define LED_1_GPIO_PIN GPIO3
#define LED_1_GPIO_ON gpio_clear
#define LED_1_GPIO_OFF gpio_set
#define LED_1_AFIO_REMAP ((void)0)

/* green: right on M1 */
#ifndef USE_LED_2
#define USE_LED_2 1
#endif
#define LED_2_GPIO GPIOC
#define LED_2_GPIO_PIN GPIO2
#define LED_2_GPIO_ON gpio_clear
#define LED_2_GPIO_OFF gpio_set
#define LED_2_AFIO_REMAP ((void)0)

/* blue: right on M2 is controlled by the NRF51822 */

/* blue: left on M3 */
#ifndef USE_LED_3
#define USE_LED_3 1
#endif
#define LED_3_GPIO GPIOD
#define LED_3_GPIO_PIN GPIO2
#define LED_3_GPIO_ON gpio_clear
#define LED_3_GPIO_OFF gpio_set
#define LED_3_AFIO_REMAP ((void)0)

/* red: left on M4 */
#ifndef USE_LED_4
#define USE_LED_4 1
#endif
#define LED_4_GPIO GPIOC
#define LED_4_GPIO_PIN GPIO0
#define LED_4_GPIO_ON gpio_clear
#define LED_4_GPIO_OFF gpio_set
#define LED_4_AFIO_REMAP ((void)0)

/* green: left on M4 */
#ifndef USE_LED_5
#define USE_LED_5 1
#endif
#define LED_5_GPIO GPIOC
#define LED_5_GPIO_PIN GPIO1
#define LED_5_GPIO_ON gpio_clear
#define LED_5_GPIO_OFF gpio_set
#define LED_5_AFIO_REMAP ((void)0)


/* Default actuators driver */
#define DEFAULT_ACTUATORS "subsystems/actuators/actuators_pwm.h"
#define ActuatorDefaultSet(_x,_y) ActuatorPwmSet(_x,_y)
#define ActuatorsDefaultInit() ActuatorsPwmInit()
#define ActuatorsDefaultCommit() ActuatorsPwmCommit()


/*
 * ADC
 * Bat voltage is measured by NRF51822
 * ADCs on right expansion header
 *  ADC0 PA2/ADC02
 *  ADC1 PA3/ADC03
 *  ADC2 PA5/ADC05
 *  ADC3 PA6/ADC06
 *  ADC4 PA7/ADC07
 */

#if USE_ADC_1
#define AD1_1_CHANNEL 2
#define ADC_1 AD1_1
#define ADC_1_GPIO_PORT GPIOA
#define ADC_1_GPIO_PIN GPIO2
#endif

#if USE_ADC_1
#define AD1_1_CHANNEL 3
#define ADC_1 AD1_2
#define ADC_1_GPIO_PORT GPIOA
#define ADC_1_GPIO_PIN GPIO3
#endif

#if USE_ADC_2
#define AD1_2_CHANNEL 5
#define ADC_2 AD1_3
#define ADC_2_GPIO_PORT GPIOA
#define ADC_2_GPIO_PIN GPIO5
#endif

#if USE_ADC_3
#define AD1_3_CHANNEL 6
#define ADC_3 AD1_4
#define ADC_3_GPIO_PORT GPIOA
#define ADC_3_GPIO_PIN GPIO6
#endif

#if USE_ADC_4
#define AD2_1_CHANNEL 7
#define ADC_4 AD2_1
#define ADC_4_GPIO_PORT GPIOA
#define ADC_4_GPIO_PIN GPIO7
#endif


/* TODO: by default activate onboard baro */
//#ifndef USE_BARO_BOARD
//#define USE_BARO_BOARD 1
//#endif


/* SPI1 on right expansion header */
#define SPI1_GPIO_AF GPIO_AF5
#define SPI1_GPIO_PORT_MISO GPIOA
#define SPI1_GPIO_MISO GPIO6
#define SPI1_GPIO_PORT_MOSI GPIOA
#define SPI1_GPIO_MOSI GPIO7
#define SPI1_GPIO_PORT_SCK GPIOA
#define SPI1_GPIO_SCK GPIO5

/* slave select IO 4 on left expansion header */
#define SPI_SELECT_SLAVE0_PORT GPIOC
#define SPI_SELECT_SLAVE0_PIN GPIO12


/*
 * UART pin configuration
 *
 * sets on which pins the UARTs are connected
 */
/* UART1 on left expansion header */
#define UART1_GPIO_AF GPIO_AF7
#define UART1_GPIO_PORT_RX GPIOC
#define UART1_GPIO_RX GPIO11
#define UART1_GPIO_PORT_TX GPIOC
#define UART1_GPIO_TX GPIO10

/* UART2 on right expansion header */
#define UART2_GPIO_AF GPIO_AF7
#define UART2_GPIO_PORT_RX GPIOA
#define UART2_GPIO_RX GPIO3
#define UART2_GPIO_PORT_TX GPIOA
#define UART2_GPIO_TX GPIO2


/*
 * I2C
 *
 */
/* externalS TX & RX */
#define I2C1_GPIO_PORT GPIOB
#define I2C1_GPIO_SCL GPIO6
#define I2C1_GPIO_SDA GPIO7

/* IMU MPU9250 on PA8 and PC9*/
#define I2C3_GPIO_PORT_SCL GPIOA
#define I2C3_GPIO_SCL GPIO8
#define I2C3_GPIO_PORT_SDA GPIOC
#define I2C3_GPIO_SDA GPIO9



/*
 * PWM
 *
 */
#define PWM_USE_TIM2 1
#define PWM_USE_TIM4 1
#define PWM_USE_TIM5 1

// motor numbering on crazyflie silkscreen/docs starts with 1

/* PWM_SERVO_x is the index of the servo in the actuators_pwm_values array */
/* motor 1 (front right, CCW) on PA0 */
#define PWM_SERVO_1 0
#define PWM_SERVO_1_TIMER TIM5
#define PWM_SERVO_1_GPIO GPIOA
#define PWM_SERVO_1_PIN GPIO1
#define PWM_SERVO_1_AF GPIO_AF2
#define PWM_SERVO_1_OC TIM_OC2
#define PWM_SERVO_1_OC_BIT (1<<1)

/* motor 2 (back right, CW) on PB11 */
#define PWM_SERVO_2 1
#define PWM_SERVO_2_TIMER TIM2
#define PWM_SERVO_2_GPIO GPIOB
#define PWM_SERVO_2_PIN GPIO11
#define PWM_SERVO_2_AF GPIO_AF1
#define PWM_SERVO_2_OC TIM_OC4
#define PWM_SERVO_2_OC_BIT (1<<1)

/* motor 2 (back left, CCW) on PA15 */
#define PWM_SERVO_3 0
#define PWM_SERVO_3_TIMER TIM2
#define PWM_SERVO_3_GPIO GPIOA
#define PWM_SERVO_3_PIN GPIO15
#define PWM_SERVO_3_AF GPIO_AF1
#define PWM_SERVO_3_OC TIM_OC1
#define PWM_SERVO_3_OC_BIT (1<<0)

/* motor 4 (front left, CW) on PB9 */
#define PWM_SERVO_4 1
#define PWM_SERVO_4_TIMER TIM4
#define PWM_SERVO_4_GPIO GPIOB
#define PWM_SERVO_4_PIN GPIO9
#define PWM_SERVO_4_AF GPIO_AF2
#define PWM_SERVO_4_OC TIM_OC4
#define PWM_SERVO_4_OC_BIT (1<<3)

#define PWM_TIM2_CHAN_MASK (PWM_SERVO_2_OC_BIT|PWM_SERVO_3_OC_BIT)
#define PWM_TIM4_CHAN_MASK PWM_SERVO_4_OC_BIT
#define PWM_TIM5_CHAN_MASK PWM_SERVO_1_OC_BIT


#endif /* CONFIG_CRAZYLFIE_2_0_H */
