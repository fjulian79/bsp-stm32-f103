/*
 * bsp_io.h
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#ifndef BSP_GPIO_H_
#define BSP_GPIO_H_

#include "bsp.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Used to get a unique identifier per gpio.
 */
#define BSP_IOMAPVAL(_port, _pin)       (_port + _pin)

/**
 * Used to get the RCC based on the unique gpio id.
 */
#define BSP_IOMAPRCC(_val)              ((uint32_t)((_val & 0x0000FF00U)/0x400))

/**
 * Used to get the port based on the unique gpio id.
 */
#define BSP_IOMAPPORT(_val)             ((uint32_t)(_val & 0xFFFFFF00U))

/**
 * Used to get the pin bitmask based on the unique gpio id.
 */
#define BSP_IOMAPIN(_val)               ((uint32_t)(1 << (_val & 0x000000FFU)))

/**
 * Definition of all gpio's
 */
typedef enum
{
    BSP_GPIO_A0         = BSP_IOMAPVAL(GPIOA, 0),
    BSP_GPIO_A1         = BSP_IOMAPVAL(GPIOA, 1),
    BSP_GPIO_TTYTX      = BSP_IOMAPVAL(GPIOA, 2),
    BSP_GPIO_TTYRX      = BSP_IOMAPVAL(GPIOA, 3),
    BSP_GPIO_A4         = BSP_IOMAPVAL(GPIOA, 4),
    BSP_GPIO_A5         = BSP_IOMAPVAL(GPIOA, 5),
    BSP_GPIO_A6         = BSP_IOMAPVAL(GPIOA, 6),
    BSP_GPIO_A7         = BSP_IOMAPVAL(GPIOA, 7),
    BSP_GPIO_A8         = BSP_IOMAPVAL(GPIOA, 8),
    BSP_GPIO_A9         = BSP_IOMAPVAL(GPIOA, 9),
    BSP_GPIO_A10        = BSP_IOMAPVAL(GPIOA, 10),
    BSP_GPIO_A11        = BSP_IOMAPVAL(GPIOA, 11),
    BSP_GPIO_A12        = BSP_IOMAPVAL(GPIOA, 12),
    BSP_GPIO_A13        = BSP_IOMAPVAL(GPIOA, 13),
    BSP_GPIO_A14        = BSP_IOMAPVAL(GPIOA, 14),
    BSP_GPIO_A15        = BSP_IOMAPVAL(GPIOA, 15),

    BSP_GPIO_B0         = BSP_IOMAPVAL(GPIOB, 0),
    BSP_GPIO_B1         = BSP_IOMAPVAL(GPIOB, 1),
    BSP_GPIO_B2         = BSP_IOMAPVAL(GPIOB, 2),
    BSP_GPIO_B3         = BSP_IOMAPVAL(GPIOB, 3),
    BSP_GPIO_B4         = BSP_IOMAPVAL(GPIOB, 4),
    BSP_GPIO_B5         = BSP_IOMAPVAL(GPIOB, 5),
    BSP_GPIO_B6         = BSP_IOMAPVAL(GPIOB, 6),
    BSP_GPIO_B7         = BSP_IOMAPVAL(GPIOB, 7),
    BSP_GPIO_B8         = BSP_IOMAPVAL(GPIOB, 8),
    BSP_GPIO_B9         = BSP_IOMAPVAL(GPIOB, 9),
    BSP_GPIO_B10        = BSP_IOMAPVAL(GPIOB, 10),
    BSP_GPIO_B11        = BSP_IOMAPVAL(GPIOB, 11),
    BSP_GPIO_B12        = BSP_IOMAPVAL(GPIOB, 12),
    BSP_GPIO_B13        = BSP_IOMAPVAL(GPIOB, 13),
    BSP_GPIO_B14        = BSP_IOMAPVAL(GPIOB, 14),
    BSP_GPIO_B15        = BSP_IOMAPVAL(GPIOB, 15),

    BSP_GPIO_C0         = BSP_IOMAPVAL(GPIOC, 0),
    BSP_GPIO_C1         = BSP_IOMAPVAL(GPIOC, 1),
    BSP_GPIO_C2         = BSP_IOMAPVAL(GPIOC, 2),
    BSP_GPIO_C3         = BSP_IOMAPVAL(GPIOC, 3),
    BSP_GPIO_C4         = BSP_IOMAPVAL(GPIOC, 4),
    BSP_GPIO_C5         = BSP_IOMAPVAL(GPIOC, 5),
    BSP_GPIO_C6         = BSP_IOMAPVAL(GPIOC, 6),
    BSP_GPIO_C7         = BSP_IOMAPVAL(GPIOC, 7),
    BSP_GPIO_C8         = BSP_IOMAPVAL(GPIOC, 8),
    BSP_GPIO_C9         = BSP_IOMAPVAL(GPIOC, 9),
    BSP_GPIO_C10        = BSP_IOMAPVAL(GPIOC, 10),
    BSP_GPIO_C11        = BSP_IOMAPVAL(GPIOC, 11),
    BSP_GPIO_C12        = BSP_IOMAPVAL(GPIOC, 12),
    BSP_GPIO_C13        = BSP_IOMAPVAL(GPIOC, 13),
    BSP_GPIO_C14        = BSP_IOMAPVAL(GPIOC, 14),
    BSP_GPIO_C15        = BSP_IOMAPVAL(GPIOC, 15),

    BSP_GPIO_D2         = BSP_IOMAPVAL(GPIOD, 2),

    BSP_GPIO_F0         = BSP_IOMAPVAL(GPIOF, 0),
    BSP_GPIO_F1         = BSP_IOMAPVAL(GPIOF, 1),

	BSP_GPIO_INVALID	= 0

}bspGpioPin_t;

/**
 *
 */
typedef enum
{
    BSP_GPIO_MODE_IN        = GPIO_MODE_INPUT,
    BSP_GPIO_MODE_OUT_10MHZ = GPIO_MODE_OUTPUT_10_MHZ,
    BSP_GPIO_MODE_OUT_2MHZ  = GPIO_MODE_OUTPUT_2_MHZ,
    BSP_GPIO_MODE_OUT_50MHZ = GPIO_MODE_OUTPUT_50_MHZ

}bspGpioMode_t;

/**
 *
 */
typedef enum
{
    BSP_GPIO_CNF_IN_ANALOG           = GPIO_CNF_INPUT_ANALOG,
    BSP_GPIO_CNF_IN_FLOAT            = GPIO_CNF_INPUT_FLOAT,
    BSP_GPIO_CNF_IN_PULLDOWN         = GPIO_CNF_INPUT_PULL_UPDOWN,
    BSP_GPIO_CNF_OUT_PUSHPULL        = GPIO_CNF_OUTPUT_PUSHPULL,
    BSP_GPIO_CNF_OUT_OPENDRAIN       = GPIO_CNF_OUTPUT_OPENDRAIN,
    BSP_GPIO_CNF_OUT_ALTFN_PUSHPULL  = GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
    BSP_GPIO_CNF_OUT_ALTFN_OPENDRAIN = GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,

}bspGpioCnf_t;

/**
 * Used to initialize any of the defined gpio's.
 *
 * @param pin       id of the gpio
 * @param mode
 * @param cnf
 */
void bspGpioInit(bspGpioPin_t pin, bspGpioMode_t mode, bspGpioCnf_t cnf);

/**
 * Used to initialize the gipo used for the on board led.
 */
inline void bspLedInit(void)
{
    bspGpioInit(BSP_GPIO_A5,
            BSP_GPIO_MODE_OUT_10MHZ, BSP_GPIO_CNF_OUT_PUSHPULL);
}

/**
 * Used to initialize the gipo used for the on board button.
 */
inline void bspButtonInit(void)
{
    bspGpioInit(BSP_GPIO_C13,
            BSP_GPIO_MODE_IN, BSP_GPIO_CNF_IN_FLOAT);
}

/**
 * Used to set any of the defined gpio's to high.
 *
 * @param pin   The gpio.
 */
inline void bspGpioSet(bspGpioPin_t pin)
{
    GPIO_BSRR(BSP_IOMAPPORT(pin)) = BSP_IOMAPIN(pin);
    //gpio_set(BSP_IOMAPPORT(pin), BSP_IOMAPIN(pin));
}

/**
 * Used to set any of the defined gpio's to low.
 *
 * @param pin   The gpio.
 */
inline void bspGpioClear(bspGpioPin_t pin)
{
    GPIO_BSRR(BSP_IOMAPPORT(pin)) = (BSP_IOMAPIN(pin) << 16);
    //gpio_clear(BSP_IOMAPPORT(pin), BSP_IOMAPIN(pin));
}

/**
 * Used to toggle any of the defined gpio's.
 *
 * @param pin   The gpio.
 */
inline void bspGpioToggle(bspGpioPin_t pin)
{
    uint32_t port = GPIO_ODR(BSP_IOMAPPORT(pin));
    GPIO_BSRR(BSP_IOMAPPORT(pin)) =
              ((port & BSP_IOMAPIN(pin)) << 16) | (~port & BSP_IOMAPIN(pin));
    //gpio_toggle(BSP_IOMAPPORT(pin), BSP_IOMAPIN(pin));
}

/**
 * Used to set any of the defined gpio's to the given value (high/low).
 *
 * @param val   0, to turn the led off.
 *              Any other value will turn the led on.
 */
void bspGpioWrite(bspGpioPin_t pin, uint32_t val);

/**
 * Used to read any of the defined gpio's.
 *
 * @return  true   if high.
 *          false  if low.
 */
inline bool bspGpioGet(bspGpioPin_t pin)
{
    return !!(GPIO_IDR(BSP_IOMAPPORT(pin)) & BSP_IOMAPIN(pin));
    //return !!gpio_get(BSP_IOMAPPORT(pin), BSP_IOMAPIN(pin));
}

/**
 * The following definitions can be used to access the debug pins in a
 * comfortable way. Hence you have to define them in bsp_config.h.
 *
 * A example:
 *
 * #define BSP_GPIO_C14                 BSP_DEBUGPIN_0
 * #define BSP_GPIO_C15                 BSP_DEBUGPIN_1
 * #define BSP_GPIO_F0                  BSP_DEBUGPIN_2
 * #define BSP_GPIO_F1                  BSP_DEBUGPIN_3
 *
 */

/**
 * Used to initialize any of the defined debug gpio's.
 *
 * @param pin   Number of the debug pin
 */
#define bspDebugPinInit(_n)                                             \
                                                                        \
        bspGpioInit(BSP_DEBUGPIN_ ## _n,                                \
                BSP_GPIO_MODE_OUT_50MHZ, BSP_GPIO_CNF_OUT_PUSHPULL);

/**
 * Used to set the given debug pin.
 *
 * @param _n    The debug pin to set.
 */
#define bspDebugPinSet(_n)          bspGpioSet(BSP_DEBUGPIN_ ## _n)

/**
 * Used to clear the given debug pin.
 *
 * @param _n    The debug pin to clear.
 */
#define bspDebugPinClear(_n)        bspGpioClear(BSP_DEBUGPIN_ ## _n)

/**
 * Used to toggle the given debug pin.
 *
 * @param _n    The debug pin to toggle.
 */
#define bspDebugPinToggle(_n)       bspGpioToggle(BSP_DEBUGPIN_ ## _n)

#ifdef __cplusplus
}
#endif

#endif /* BSP_GPIO_H_ */
