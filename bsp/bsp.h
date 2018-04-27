/*
 * bsp.h
 *
 *  Created on: Mar 25, 2018
 *      Author: julian
 */

#ifndef LIBBSP_NUCLEO_F103_BSP_H_
#define LIBBSP_NUCLEO_F103_BSP_H_

#include "bsp_config.h"

#include <stm32f1xx.h>
#include <stm32f1xx_ll_system.h>
#include <stm32f1xx_ll_utils.h>

/**
 * Special GPIO Pin definitions
 */
#ifndef BSP_GPIO_A5
#define BSP_GPIO_A5                         BSP_GPIO_LED
#endif

#ifndef BSP_GPIO_C13
#define BSP_GPIO_C13                        BSP_GPIO_BUTTON
#endif

/**
 * TTY configuration
 */
#define TTY_USARTx                          USART2
#define TTY_USARTx_CLK_ENABLE()             LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)

#define BSP_GPIO_A2                         BSP_GPIO_TTY_TX
#define BSP_GPIO_A3                         BSP_GPIO_TTY_RX

/* Hence that the DMA channel to use is hard coded by ST.
 * See DMA channel assignment table below. */
#define TTY_TXDMACH                         DMA1_Channel7
#define TTY_TXDMACH_IRQn                    DMA1_Channel7_IRQn
#define TTY_TXDMACH_IRQHandler              DMA1_Channel7_IRQHandler
#define TTY_TXDMACH_LLCH                    LL_DMA_CHANNEL_7
#define TTY_TXDMACH_ISACTIVEFLAG_TC()       LL_DMA_IsActiveFlag_TC7(DMA1)
#define TTY_TXDMACH_CLEARFLAG_TC()          LL_DMA_ClearFlag_TC7(DMA1)

/**
 * I2C configuration
 */
//#define BSP_I2CPORT                         gpioPortA
//#define BSP_I2CSDAPIN                       0
//#define BSP_I2CSCLPIN                       1

/**
 * DMA channel assignment from ST.
 * Hence that this hard coded and can not be changed!
 */
#define BSP_DMACH_ADC1                      DMA1_Channel1

#define BSP_DMACH_SPI1_TX                   DMA1_Channel3
#define BSP_DMACH_SPI1_RX                   DMA1_Channel2
#define BSP_DMACH_SPI2_TX                   DMA1_Channel5
#define BSP_DMACH_SPI2_RX                   DMA1_Channel4

#define BSP_DMACH_USART1_TX                 DMA1_Channel4
#define BSP_DMACH_USART1_RX                 DMA1_Channel5
#define BSP_DMACH_USART2_TX                 DMA1_Channel7
#define BSP_DMACH_USART2_RX                 DMA1_Channel6
#define BSP_DMACH_USART3_TX                 DMA1_Channel2
#define BSP_DMACH_USART3_RX                 DMA1_Channel3

#define BSP_DMACH_I2C1_TX                   DMA1_Channel6
#define BSP_DMACH_I2C1_RX                   DMA1_Channel7
#define BSP_DMACH_I2C2_TX                   DMA1_Channel4
#define BSP_DMACH_I2C2_RX                   DMA1_Channel5

#define BSP_DMACH_TIM1_CH1                  DMA1_Channel2
#define BSP_DMACH_TIM1_CH3                  DMA1_Channel6
#define BSP_DMACH_TIM1_CH4                  DMA1_Channel4
#define BSP_DMACH_TIM1_UP                   DMA1_Channel5
#define BSP_DMACH_TIM1_TRIG                 DMA1_Channel4
#define BSP_DMACH_TIM1_COM                  DMA1_Channel4

#define BSP_DMACH_TIM2_CH1                  DMA1_Channel5
#define BSP_DMACH_TIM2_CH2                  DMA1_Channel7
#define BSP_DMACH_TIM2_CH3                  DMA1_Channel1
#define BSP_DMACH_TIM2_CH4                  DMA1_Channel7
#define BSP_DMACH_TIM2_UP                   DMA1_Channel2

#define BSP_DMACH_TIM3_CH1                  DMA1_Channel6
#define BSP_DMACH_TIM3_CH3                  DMA1_Channel2
#define BSP_DMACH_TIM3_CH4                  DMA1_Channel3
#define BSP_DMACH_TIM3_UP                   DMA1_Channel3
#define BSP_DMACH_TIM3_TRIG                 DMA1_Channel6

#define BSP_DMACH_TIM4_CH1                  DMA1_Channel1
#define BSP_DMACH_TIM4_CH2                  DMA1_Channel4
#define BSP_DMACH_TIM4_CH3                  DMA1_Channel5
#define BSP_DMACH_TIM4_UP                   DMA1_Channel7

#define BSP_DMAIRQn(_ch)                    _ch##_IRQn
#define BSP_DMAIRQHandler(_ch)              _ch##_IRQHandler

/*
 * Interrupt configuration
 * */
#ifndef __NVIC_PRIO_BITS
/*
 * __NVIC_PRIO_BITS should be specified when CMSIS is being used.
 * 7 priority levels as default
 * */
#define __NVIC_PRIO_BITS                    3
#endif

/*
 * Lower values mean higher priorities, see EFM32 app note AN0039.
 */
#define BSP_IRQPRIO_MAX                     0x00

//#define BSP_IRQPRIO_TIMER0                  0x02
//#define BSP_IRQPRIO_I2C0                    0x03

#ifndef BSP_IRQPRIO_FREERTOS_SYSCALL

/**
 * The highest interrupt priority that can be used by any interrupt service
 * routine that makes calls to interrupt safe FreeRTOS API functions.
 * DO NOT CALL INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT
 * HAS A HIGHER PRIORITY THAN THIS!
 */
#define BSP_IRQPRIO_FREERTOS_SYSCALL        0x04

#endif

//#define BSP_IRQPRIO_USART2TX                0x04
//#define BSP_IRQPRIO_DMA                     0x05
//#define BSP_IRQPRIO_TIMER2                  0x06
//#define BSP_IRQPRIO_UART0TX                 0x07
//#define BSP_IRQPRIO_UART0RX                 0x07

/**
 * The lowest interrupt priority that can be used in a call to a
 * NVIC_SetPriority() function. Given by the fact that there are only
 * tree priority bits.
 * */
#define BSP_IRQPRIO_MIN                     0x07

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
 * Used to implement generic chip initialization
 */
void bspChipInit(
      void);

#endif /* LIBBSP_NUCLEO_F103_BSP_H_ */
