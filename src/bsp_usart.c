/*
 * bsp-usart.c
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#include "bsp/bsp.h"
#include "bsp/bsp_gpio.h"
#include "bsp/bsp_usart.h"
#include "mybuildroot/common.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

#if BSP_TTY_DMA_MODE == BSP_ENABLED

#include "fifo.h"

typedef struct
{
    char Data[BSP_TTY_BUFFERSIZE];
    volatile size_t TxBytes;
    uint32_t lostBytes;

}ttyData_t;

ttyData_t ttyData;

fifo_t ttyFifo = FIFO_INIT;

#endif /* BSP_TTY_DMA_MODE == BSP_ENABLED */

int _write(int file, char *pData, int siz)
{
    volatile int tmp = 0;
    char *ptr = NULL;

    if (file == 1)
    {

#if BSP_TTY_DMA_MODE == BSP_ENABLED

        nvic_disable_irq(NVIC_DMA1_CHANNEL7_IRQ);

        if (siz == 1)
            tmp = fifoPut(&ttyFifo, pData);
        else
            tmp = fifoWrite(&ttyFifo, pData, siz);

        if (ttyData.TxBytes == 0)
        {
            ttyData.TxBytes = fifoGetReadBlock(&ttyFifo, (void**)&ptr);
            dma_set_memory_address(DMA1, DMA_CHANNEL7, (uint32_t)ptr);
            dma_set_number_of_data(DMA1, DMA_CHANNEL7, ttyData.TxBytes);
            dma_enable_channel(DMA1, DMA_CHANNEL7);
            usart_enable_tx_dma(BSP_TTY_USART);
        }

        nvic_enable_irq(NVIC_DMA1_CHANNEL7_IRQ);

#if BSP_TTY_BLOCKING == BSP_ENABLED

        while (fifoGetFree(&ttyFifo) == 0 && (tmp < siz));
        return tmp;

#else /* BSP_TTY_BLOCKING == BSP_ENABLED */

        return siz;

#endif /* BSP_TTY_BLOCKING == BSP_ENABLED */

#else /* BSP_TTY_DMA_MODE == BSP_ENABLED */

        for (tmp = 0; tmp < siz; tmp++)
            usart_send_blocking(BSP_TTY_USART, pDate[tmp]);

        return tmp;

#endif /* BSP_TTY_DMA_MODE == BSP_ENABLED */

    }

    errno = EIO;
    return -1;
}

#if BSP_TTY_DMA_MODE == BSP_ENABLED

void dma1_channel7_isr(void)
{
    char *ptr = NULL;

    if ((DMA1_ISR & DMA_ISR_TCIF7) != 0)
    {
        DMA1_IFCR |= DMA_IFCR_CTCIF7;
    }

    usart_disable_tx_dma(BSP_TTY_USART);
    dma_disable_channel(DMA1, DMA_CHANNEL7);

    fifoFree(&ttyFifo, ttyData.TxBytes);
    ttyData.TxBytes = fifoGetReadBlock(&ttyFifo, (void**)&ptr);

    if (ttyData.TxBytes != 0)
    {
        dma_set_memory_address(DMA1, DMA_CHANNEL7, (uint32_t)ptr);
        dma_set_number_of_data(DMA1, DMA_CHANNEL7, ttyData.TxBytes);
        dma_enable_channel(DMA1, DMA_CHANNEL7);
        usart_enable_tx_dma(BSP_TTY_USART);
    }
}

#endif /* BSP_TTY_DMA_MODE == BSP_ENABLED */

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

    bspGpioInit(BSP_GPIO_TTYTX,
            BSP_GPIO_MODE_OUT_50MHZ, BSP_GPIO_CNF_OUT_ALTFN_PUSHPULL);

    bspGpioInit(BSP_GPIO_TTYRX,
            BSP_GPIO_MODE_IN, BSP_GPIO_CNF_IN_FLOAT);

    usart_set_baudrate(BSP_TTY_USART, baud);
    usart_set_databits(BSP_TTY_USART, 8);
    usart_set_stopbits(BSP_TTY_USART, USART_STOPBITS_1);
    usart_set_mode(BSP_TTY_USART, USART_MODE_TX_RX);
    usart_set_parity(BSP_TTY_USART, USART_PARITY_NONE);
    usart_set_flow_control(BSP_TTY_USART, USART_FLOWCONTROL_NONE);

    usart_enable(BSP_TTY_USART);

#if BSP_TTY_DMA_MODE == BSP_ENABLED

    ttyData.TxBytes = 0;
    ttyData.lostBytes = 0;

    fifoInit(&ttyFifo, ttyData.Data, sizeof(ttyData.Data));

    rcc_periph_clock_enable(RCC_DMA1);

    nvic_set_priority(NVIC_DMA1_CHANNEL7_IRQ, 0);

    dma_channel_reset(DMA1, DMA_CHANNEL7);
    dma_set_peripheral_address(DMA1, DMA_CHANNEL7, (uint32_t)&USART2_DR);
    dma_set_read_from_memory(DMA1, DMA_CHANNEL7);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL7);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL7, DMA_CCR_PSIZE_8BIT);
    dma_set_memory_size(DMA1, DMA_CHANNEL7, DMA_CCR_MSIZE_8BIT);
    dma_set_priority(DMA1, DMA_CHANNEL7, DMA_CCR_PL_VERY_HIGH);

    dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL7);

#endif /* BSP_TTY_DMA_MODE == BSP_ENABLED */
}

bspStatus_t bspTTYSendData(uint8_t *pData, uint16_t siz)
{
    bspStatus_t ret = BSP_OK;

    if (_write(1, (char *) pData, siz) != (int) siz)
        ret = BSP_ERR;

    return ret;
}

bool bspTTYDataAvailable(void)
{
    return usart_get_flag(BSP_TTY_USART, USART_SR_RXNE);
}

char bspTTYGetChar(void)
{
    return usart_recv_blocking(BSP_TTY_USART);
}
