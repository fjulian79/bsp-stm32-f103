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

#ifndef BSP_NUCLEO_F103_GPIO_H_
#define BSP_NUCLEO_F103_GPIO_H_

#include <stm32f1xx_ll_gpio.h>

#include <stdint.h>
#include <stdbool.h>

#include "bsp/bsp.h"

#define BSP_GPIO_PIN_0                      (0x0001)
#define BSP_GPIO_PIN_1                      (0x0002)
#define BSP_GPIO_PIN_2                      (0x0004)
#define BSP_GPIO_PIN_3                      (0x0008)
#define BSP_GPIO_PIN_4                      (0x0010)
#define BSP_GPIO_PIN_5                      (0x0020)
#define BSP_GPIO_PIN_6                      (0x0040)
#define BSP_GPIO_PIN_7                      (0x0080)
#define BSP_GPIO_PIN_8                      (0x0100)
#define BSP_GPIO_PIN_9                      (0x0200)
#define BSP_GPIO_PIN_10                     (0x0400)
#define BSP_GPIO_PIN_11                     (0x0800)
#define BSP_GPIO_PIN_12                     (0x1000)
#define BSP_GPIO_PIN_13                     (0x2000)
#define BSP_GPIO_PIN_14                     (0x4000)
#define BSP_GPIO_PIN_15                     (0x8000)
#define BSP_GPIO_PIN_All                    (0xffff)

/**
 * Derives a bsp internal gpio ID from the given port and pin.
 */
#define BSP_IOMAPVAL(_port, _pin)           ((_port << 16) + _pin)

/**
 * Descriptor of all gpio pins. Hence that some of those names might be
 * overwritten in bsp.h as there special function pins names will be mapped
 * the the generic gpio names.
 */
typedef enum
{
    BSP_GPIO_A0        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_0),
    BSP_GPIO_A1        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_1),
    BSP_GPIO_A2        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_2),
    BSP_GPIO_A3        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_3),
    BSP_GPIO_A4        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_4),
    BSP_GPIO_A5        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_5),
    BSP_GPIO_A6        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_6),
    BSP_GPIO_A7        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_7),
    BSP_GPIO_A8        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_8),
    BSP_GPIO_A9        = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_9),
    BSP_GPIO_A10       = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_10),
    BSP_GPIO_A11       = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_11),
    BSP_GPIO_A12       = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_12),
    BSP_GPIO_A13       = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_13),
    BSP_GPIO_A14       = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_14),
    BSP_GPIO_A15       = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_15),
    BSP_GPIO_A_ALL     = BSP_IOMAPVAL(GPIOA_BASE, BSP_GPIO_PIN_All),

    BSP_GPIO_B0        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_0),
    BSP_GPIO_B1        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_1),
    BSP_GPIO_B2        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_2),
    BSP_GPIO_B3        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_3),
    BSP_GPIO_B4        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_4),
    BSP_GPIO_B5        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_5),
    BSP_GPIO_B6        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_6),
    BSP_GPIO_B7        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_7),
    BSP_GPIO_B8        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_8),
    BSP_GPIO_B9        = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_9),
    BSP_GPIO_B10       = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_10),
    BSP_GPIO_B11       = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_11),
    BSP_GPIO_B12       = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_12),
    BSP_GPIO_B13       = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_13),
    BSP_GPIO_B14       = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_14),
    BSP_GPIO_B15       = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_15),
    BSP_GPIO_B_ALL     = BSP_IOMAPVAL(GPIOB_BASE, BSP_GPIO_PIN_All),

    BSP_GPIO_C0        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_0),
    BSP_GPIO_C1        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_1),
    BSP_GPIO_C2        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_2),
    BSP_GPIO_C3        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_3),
    BSP_GPIO_C4        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_4),
    BSP_GPIO_C5        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_5),
    BSP_GPIO_C6        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_6),
    BSP_GPIO_C7        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_7),
    BSP_GPIO_C8        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_8),
    BSP_GPIO_C9        = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_9),
    BSP_GPIO_C10       = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_10),
    BSP_GPIO_C11       = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_11),
    BSP_GPIO_C12       = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_12),
    BSP_GPIO_C13       = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_13),
    BSP_GPIO_C14       = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_14),
    BSP_GPIO_C15       = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_15),
    BSP_GPIO_C_ALL     = BSP_IOMAPVAL(GPIOC_BASE, BSP_GPIO_PIN_All),

}bspGpioPin_t;

/**
 * Used to initialize a single gpio pin.
 *
 * @param pin   The bsp gpio pin ID.
 * @param init  Parameters to use. Hence the pin number given by this struct
 *              will be overwritten by the number derived from the pin ID.
 */
void bspGpioPinInit(
        bspGpioPin_t pin,
        LL_GPIO_InitTypeDef *init);

/**
 * Used to initialize gpio pins used by the bsp internally.
 * E.g. tty, led, buttons, etc.
 */
void bspGpioInit(
        void);

/**
 * Sets the given gpio pin to high.
 *
 * @param pin   The bsp gpio pin ID.
 */
void bspGpioSet(
        bspGpioPin_t pin);

/**
 *
 * @param pin
 */
void bspGpioClear(
        bspGpioPin_t pin);

/**
 * Sets the given gpio pin to low.
 *
 * @param pin   The bsp gpio pin ID.
 */
void bspGpioToggle(
        bspGpioPin_t pin);

/**
 * Sets the given gpio pin based on the given value.
 *
 * @param pin   The bsp gpio pin ID.
 * @param val   If not zero the pin will be set to high.
 *              If zero the pin will become low.
 */
void bspGpioWrite(
        bspGpioPin_t pin,
        uint32_t val);

/**
 * Used to read the given gpio pin.
 *
 * @param pin   The bsp gpio pin ID.
 *
 * @return  true    if the pin is high.
 *          false   if the pin is low.
 */
bool bspGpioRead(
        bspGpioPin_t pin);

#endif /* BSP_NUCLEO_F103_GPIO_H_ */
