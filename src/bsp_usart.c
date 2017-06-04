/*
 * bsp-usart.c
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#include "bsp/bsp.h"
#include "bsp/bsp_usart.h"

#include "mybuildroot/common.h"

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/usart.h"

#include <stdio.h>
#include <errno.h>

int _write(int file, char *ptr, int len)
{
    int i;

    if (file == 1)
    {
        for (i = 0; i < len; i++)
            usart_send_blocking(BSP_TTY_USART, ptr[i]);
        return i;
    }

    errno = EIO;
    return -1;
}

int _read(int file, char *ptr, int len)
{
    unused(file);
    unused(ptr);
    unused(len);

    errno = EIO;
    return -1;
}

void bspTTYInit(uint32_t baud)
{
    rcc_periph_clock_enable(BSP_TTY_USART_RCC);
    rcc_periph_clock_enable(BSP_TTY_GPIO_RCC);

    gpio_set_mode(BSP_TTY_GPIO_PORT,
            GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
            BSP_TTY_GPIO_TXPIN);

    gpio_set_mode(BSP_TTY_GPIO_PORT,
            GPIO_MODE_INPUT,
            GPIO_CNF_INPUT_FLOAT,
            BSP_TTY_GPIO_RXPIN);

    usart_set_baudrate(BSP_TTY_USART, baud);
    usart_set_databits(BSP_TTY_USART, 8);
    usart_set_stopbits(BSP_TTY_USART, USART_STOPBITS_1);
    usart_set_mode(BSP_TTY_USART, USART_MODE_TX_RX);
    usart_set_parity(BSP_TTY_USART, USART_PARITY_NONE);
    usart_set_flow_control(BSP_TTY_USART, USART_FLOWCONTROL_NONE);

    usart_enable(BSP_TTY_USART);

    /**
     * stdout is line buffered by default. This might cause issues on a MCU.
     * ToDo: What impacts does this cause .. is there a better way?
     */
    setbuf(stdout, NULL);
}

bool bspTTYDataAvailable(void)
{
    return usart_get_flag(BSP_TTY_USART, USART_SR_RXNE);
}

char bspTTYGetChar(void)
{
    return usart_recv(BSP_TTY_USART);
}
