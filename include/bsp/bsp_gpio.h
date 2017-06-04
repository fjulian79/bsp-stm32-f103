/*
 * bsp_io.h
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#ifndef BSP_IO_H_
#define BSP_GPIO_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Used in initialize the led gpio.
 */
void bspLedInit(void);

/**
 * Used to turn the led on.
 */
void bspLedOn(void);

/**
 * Used to turn the led off.
 */
void bspLedOff(void);

/**
 * Used to toggle the led.
 */
void bspLedToggle(void);

/**
 * Used to set the led to the given value.
 *
 * @param val   0, to turn the led off.
 *              Any other value will turn the led on.
 */
void bspLedSet(uint32_t val);

/**
 * Used in initialize the button gpio.
 */
void bspButtonInit(void);

/**
 * Used to get the state of the button gpio.
 *
 * @return  true, if pressed.
 *          false, if not pressed.
 */
bool bspButtonGet(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_GPIO_H_ */
