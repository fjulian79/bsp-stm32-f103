/*
 * bsp-io.c
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#include "bsp/bsp.h"
#include "bsp/bsp_gpio.h"

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"

void bspLedInit(void)
{
    rcc_periph_clock_enable(BSP_LED_RCC);

    gpio_set_mode(BSP_LED_PORT,
            GPIO_MODE_OUTPUT_2_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL,
            BSP_LED_PIN);
}

void bspLedOn(void)
{
    gpio_set(BSP_LED_PORT, BSP_LED_PIN);
}

void bspLedOff(void)
{
    gpio_clear(BSP_LED_PORT, BSP_LED_PIN);
}

void bspLedToggle(void)
{
    gpio_toggle(BSP_LED_PORT, BSP_LED_PIN);
}

void bspLedSet(uint32_t val)
{
    if (val == 0)
        gpio_clear(BSP_LED_PORT, BSP_LED_PIN);
    else
        gpio_set(BSP_LED_PORT, BSP_LED_PIN);

}

void bspButtonInit(void)
{
    rcc_periph_clock_enable(BSP_BUTTON_RCC);

    gpio_set_mode(BSP_BUTTON_PORT,
            GPIO_MODE_INPUT,
            GPIO_CNF_INPUT_FLOAT,
            BSP_BUTTON_PIN);
}

bool bspButtonGet(void)
{
    return !!gpio_get(BSP_BUTTON_PORT, BSP_BUTTON_PIN);
}


