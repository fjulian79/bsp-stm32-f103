/*
 * bsp_io.c
 *
 *  Created on: Mar 25, 2018
 *      Author: julian
 */

#include <bsp/bsp_gpio.h>

/**
 * Derives the gpio port from the bsp gipo pin ID.
 */
#define BSP_IOMAPPORT(_val)     ((GPIO_TypeDef *) (APB2PERIPH_BASE + (((uint32_t)_val) >> 16)))

/**
 * Derives the bit mask for enabling the gpio clock from the bsp gpio pin ID.
 */
#define BSP_IOMAPPEN(_val)      (0x1 << (((uint32_t)_val) >> 26))

/**
 * Derives the bit mask for accessing the gpio pin from the bsp gpio pin ID.
 */
#define BSP_IOMAPPIN(_val)      (((uint32_t)_val) & 0xFFFF)

void bspGpioPinInit(bspGpioPin_t pin, LL_GPIO_InitTypeDef *init)
{
	__IO uint32_t tmp = BSP_IOMAPPEN(pin);

	SET_BIT(RCC->APB2ENR, tmp);
	tmp = READ_BIT(RCC->APB2ENR, tmp);

	tmp = BSP_IOMAPPIN(pin);
	init->Pin = tmp << 8 ;
	if (init->Pin < 0x10000)
		init->Pin |= tmp;
	else
		init->Pin |= (0x04000000 | (tmp >> 8));

    LL_GPIO_Init(BSP_IOMAPPORT(pin), init);
}

void bspGpioInit(void)
{
    LL_GPIO_InitTypeDef init;

	/* LED */
    init.Mode = LL_GPIO_MODE_OUTPUT;
    init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    init.Pull = LL_GPIO_PULL_DOWN;
    init.Speed = LL_GPIO_SPEED_FREQ_MEDIUM;
    bspGpioPinInit(BSP_GPIO_LED, &init);

	/* Button */
	init.Mode = LL_GPIO_MODE_INPUT;
    init.Pull = LL_GPIO_PULL_DOWN;
    bspGpioPinInit(BSP_GPIO_BUTTON, &init);

	/* TTY */
    init.Mode = LL_GPIO_MODE_ALTERNATE;
    init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    init.Pull = LL_GPIO_PULL_UP;
    init.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    bspGpioPinInit(BSP_GPIO_TTY_TX, &init);

    init.Mode = LL_GPIO_MODE_FLOATING;
    bspGpioPinInit(BSP_GPIO_TTY_RX, &init);
}

void bspGpioSet(bspGpioPin_t pin)
{
	GPIO_TypeDef* GPIOx = BSP_IOMAPPORT(pin);
	uint16_t GPIO_Pin = BSP_IOMAPPIN(pin);

	WRITE_REG(GPIOx->BSRR, GPIO_Pin);
}

void bspGpioClear(bspGpioPin_t pin)
{
	GPIO_TypeDef* GPIOx = BSP_IOMAPPORT(pin);
	uint16_t GPIO_Pin = BSP_IOMAPPIN(pin);

	WRITE_REG(GPIOx->BRR, GPIO_Pin);
}

void bspGpioToggle(bspGpioPin_t pin)
{
	GPIO_TypeDef* GPIOx = BSP_IOMAPPORT(pin);
	uint16_t GPIO_Pin = BSP_IOMAPPIN(pin);

	WRITE_REG(GPIOx->ODR, READ_REG(GPIOx->ODR) ^ GPIO_Pin);
}

void bspGpioWrite(bspGpioPin_t pin, uint32_t val)
{
	if(val != 0)
		bspGpioSet(pin);
	else
		bspGpioClear(pin);
}

bool bspGpioRead(bspGpioPin_t pin)
{
	return false;
}
