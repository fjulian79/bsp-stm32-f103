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

void bspGpioInit(bspGpioPin_t pin, bspGpioMode_t mode, bspGpioCnf_t cnf)
{
    switch (mode)
    {
        case BSP_GPIO_CNF_OUT_ALTFN_PUSHPULL:
        case BSP_GPIO_CNF_OUT_ALTFN_OPENDRAIN:

            rcc_periph_clock_enable(RCC_AFIO);
            break;

        default:
            break;
    }

    rcc_periph_clock_enable(
            (enum rcc_periph_clken) _REG_BIT(0x18, BSP_IOMAPRCC(pin)));
    gpio_set_mode(BSP_IOMAPPORT(pin), mode, cnf, BSP_IOMAPIN(pin));
}

void bspGpioWrite(bspGpioPin_t pin, uint32_t val)
{
    if (val == 0)
        bspGpioClear(pin);
    else
        bspGpioSet(pin);
}

