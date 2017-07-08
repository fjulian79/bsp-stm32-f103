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

#include <bsp_config.h>

#ifndef BSP_ENABLED

/**
 * Default enabled definition in the bsp.
 */
#define BSP_ENABLED                     1

#endif /* BSP_ENABLED */

#ifndef BSP_DISABLED

/**
 * Default disabled definition in the bsp.
 */
#define BSP_DISABLED                    0

#endif /* BSP_DISABLE*/

#ifndef BSP_TTY_DMA_MODE

/**
 * Default TTY DMA mode.
 */
#define BSP_TTY_DMA_MODE                BSP_ENABLED

#endif /* BSP_TTY_DMA_MODE*/

#ifndef BSP_TTY_BUFFERSIZE

/**
 * Default TTY DMA FIFO size.
 */
#define BSP_TTY_BUFFERSIZE              160

#endif /* BSP_TTY_BUFFERSIZE*/

#ifndef BSP_TTY_BLOCKING

/**
 * Default TTY blocking mode.
 */
#define BSP_TTY_BLOCKING                BSP_ENABLED

#endif /* BSP_TTY_BLOCKING*/

/**
 * GPIO definitions used in the BSP.
 *
 * The BSP will define all GPIO's within a enumeration type called bspGpioPin_t
 * in the following way BSP_GPIO_<PORT><PIN>. Here those names can be replaced
 * by redefining them to define special function pins for led's, button's and
 * special io busses, etc.
 *
 * User definitions shall be defined within bsp_config.h.
 */

/**
 * The TTY is seen as mandatory for all projects and therefore the needed gpio
 * pins are defined here.
 */
#define BSP_GPIO_A2                     BSP_GPIO_TTYTX
#define BSP_GPIO_A3                     BSP_GPIO_TTYRX

/**
 * TTY definitions
 */
#define BSP_TTY_USART_RCC               RCC_USART2
#define BSP_TTY_USART                   USART2

/**
 * DMA channel definitions
 */
#define BSP_DMACH_USART0TX              0
#define BSP_DMACH_NUMBEROF              1

/**
 * Interrupt definitions. Check the Cortex-M3 programming guide for details if
 * needed.
 */

#ifndef __NVIC_PRIO_BITS
/**
 * __NVIC_PRIO_BITS should be specified when CMSIS is being used.
 * 7 priority levels as default if the definition is not available.
 */
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
