/*
 * bsp-nucleo-f103, the board support package for the hardware used in the 
 * smartsink project.
 *
 * Copyright (C) 2019 Julian Friedrich
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. 
 *
 * You can file issues at https://github.com/fjulian79/bsp-smartsink
 */

#ifndef BSP_NUCLEO_F103_BSP_CONFIG_H_
#define BSP_NUCLEO_F103_BSP_CONFIG_H_

/**
 * BSP global enabled definitions.
 */
#define BSP_ENABLED                       1

/**
 * BSP global disabled definitions.
 */
#define BSP_DISABLED                      0

/**
 * Enable to use the internal oscilator.
 * 
 * Attetion:  If enable the system clock will be set to 64MHz
 *              
 *            If disabled or undefined the system clock we be generated from
 *            HSE bypass and will be set to 72Mhz. It is assumed that a 8MHz
 *            is provided to the external clock input.
 */
#define BSP_CLOCKSRC_HSI                  BSP_DISABLED

/**
 * If enabled the bsp implements the sys tick interrupt and runs a tick counter.
 */
#define BSP_SYSTICK                       BSP_ENABLED

/**
 * Baud rate of the serial interface
 */
#define BSP_TTY_BAUDRATE                  115200

/**
 * If enabled printf will use DMA
 */
#define BSP_TTY_TX_DMA                    BSP_ENABLED

/**
 * Defines the fifo size used for TTY DMA transmissions.
 */
#define BSP_TTY_TX_BUFSIZ                 160

/**
 * If enabled the RX interrupt will be enabled and the incoming data will be
 * written to a internal ring buffer.
 */
#define BSP_TTY_RX_IRQ                    BSP_ENABLED

/**
 * Defines the RX fifo size used in case of enabled RX interrupt.
 */
#define BSP_TTY_RX_BUFSIZ                 16

/**
 * If enabled _write (and therefore printf) will block until everything has
 * been written to the fifo. If not data which does not fit to the fifo will
 * be discarded.
 */
#define BSP_TTY_BLOCKING                  BSP_ENABLED

/**
 * @brief Defines the number of PWM steps
 */
#define BSP_MOTORPWM_MAX                  2048

/**
 * GPIO definitions.
 *
 * The bsp will define all GPIO's within a enumeration type called bspGpioPin_t
 * in the following way BSP_GPIO_<PORT><PIN>. Here those names can be replaced
 * by redefining them to define special function pins.
 *
 * ATTENTION:  Hence that there are such definitions within bsp.h for those pins
 *             the BSP uses internally like the serial port etc.
 */

/**
 * Debug pins for use with a logic analyzer.
 */
#define BSP_GPIO_C14                      BSP_DEBUGPIN_0
#define BSP_GPIO_C15                      BSP_DEBUGPIN_1
#define BSP_GPIO_C2                       BSP_DEBUGPIN_2
#define BSP_GPIO_C3                       BSP_DEBUGPIN_3

/**
 * Interrupt priority configuration.
 *
 * See bsp.h for min max vaules and how they should be interpreted.
 */
#define BSP_SYSTICK_IRQ_PRIO              BSP_IRQPRIO_MAX
#define BSP_TTY_USART_IRQ_PRIO            (BSP_IRQPRIO_MAX + 1)

/**
 * If enabled bsp_assert.h will implement assertions.
 */
#define BSP_ASSERT                        BSP_ENABLED

/**
 * Defines the period of the LED flashes in case of a asseetion
 */
#define BSP_ASSERT_LED_MS                 100

/**
 * If enabled assertions will cause a message on stderr
 */
#define BSP_ASSERT_MESSAGE                BSP_ENABLED

/**
 * Defines the intervall between asseet messages in ms
 */
#define BSP_ASSERT_MESSAGE_MS             5000

#endif /* BSP_NUCLEO_F103_BSP_CONFIG_H_ */
