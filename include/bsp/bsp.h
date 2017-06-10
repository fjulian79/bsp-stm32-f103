/*
 * bsp.h
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#ifndef BSP_H_
#define BSP_H_

#include <libopencm3/stm32/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 */
#define BSP_ENABLED                     1
#define BSP_DISABLED                    0

/**
 * If enabled bsp_assert.h will implement assertions.
 */
#define BSP_DOASSERT                    BSP_ENABLED

/*
 * User LED configuration
 * */
#define BSP_LED_RCC                     RCC_GPIOA
#define BSP_LED_PORT                    GPIOA
#define BSP_LED_PIN                     GPIO5

/*
 * Button configuration
 * */
#define BSP_BUTTON_RCC                  RCC_GPIOC
#define BSP_BUTTON_PORT                 GPIOC
#define BSP_BUTTON_PIN                  GPIO13


/*
 * TTY configuration
 * */
#define BSP_TTY_USART_RCC               RCC_USART2
#define BSP_TTY_USART                   USART2
#define BSP_TTY_GPIO_RCC                RCC_GPIOA
#define BSP_TTY_GPIO_PORT               GPIOA
#define BSP_TTY_GPIO_TXPIN              GPIO_USART2_TX
#define BSP_TTY_GPIO_RXPIN              GPIO_USART2_RX

#define BSP_TTY_DMA_MODE                BSP_ENABLED
#define BSP_TTY_DMA                     DMA1
#define BSP_TTY_DMA_CH                  DMA_CHANNEL7

/*
 * DMA configuration
 * */
#define BSP_DMACH_USART0TX              0
#define BSP_DMACH_NUMBEROF              1


#ifndef __NVIC_PRIO_BITS
/*
 * __NVIC_PRIO_BITS should be specified when CMSIS is being used.
 * 7 priority levels as default if the definition is not available.
 * */
#define __NVIC_PRIO_BITS                3
#endif

/**
 * Defines the maximum interrupt priority.
 *
 * Lower values mean higher priorities, zero is the maximum priority.
 * See STM PM0065, Programming manual for STM32F10/20/21/L1 devices.
 */
#define BSP_IRQPRIO_MAX                 0x00

/**
 * The highest interrupt priority that can be used by any interrupt service
 * routine that makes calls to interrupt safe FreeRTOS API functions.
 * DO NOT CALL INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT
 * HAS A HIGHER PRIORITY THAN THIS!
 */
#define BSP_IRQPRIO_FREERTOS_SYSCALL    0x04

#define BSP_IRQPRIO_DMA                 0x05
#define BSP_IRQPRIO_UART0RX             0x07

/**
 * The lowest interrupt priority that can be used in a call to a freeRTOS
 * NVIC_SetPriority() function. Given by the fact that there are only
 * tree priority bits.
 * */
#define BSP_IRQPRIO_MIN                 0x07

/**
 * Generic type used as return value for public BSP functions where applicable.
 */
typedef enum
{
      BSP_OK,               ///<! In case of success
      BSP_IDLE,             ///<! Recourse idle
      BSP_ERR,              ///<! Unspecified error
      BSP_EBUSY,            ///<! Recourse busy
      BSP_ETIMEOUT,         ///<! Timeout
      BSP_EEINVAL,          ///<! Invalid arguments

} bspStatus_t;

/**
 * Used to define if the HSE bypass shall be enabled or not.
 * If enabled a external 8MHz clock has to be provided to the chip.
 * If disabled a 8Mhz crystal or oscillator has to be connected.
 */
typedef enum
{
      BSP_HSE_BYPASS_ON,    ///<! HSE bypass enabled
      BSP_HSE_BYPASS_OFF,   ///<! HSE bypass disabled

} bspHSEBypass_t;

/**
 * Used to define the CPU speed.
 */
typedef enum
{
      BSP_CPU_SPEED_24MHZ,  //!< BSP_CPU_SPEED_24MHZ
      BSP_CPU_SPEED_72MHZ,  //!< BSP_CPU_SPEED_72MHZ

} bspCpuSpeed_t;

/**
 * Used to setup the system clock.
 *
 * @param bypass    To specify the clock input type.
 * @param speed     Tp specify the CPU speed.
 */
void bspClockSetup(bspHSEBypass_t bypass, bspCpuSpeed_t speed);

#ifdef __cplusplus
}
#endif

#endif /* BSP_H_ */
