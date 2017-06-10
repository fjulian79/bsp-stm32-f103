/*
 * bsp-usart.c
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#include "bsp/bsp.h"
#include "bsp/bsp_usart.h"

#include "fifo.h"

#include "mybuildroot/common.h"

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/usart.h"
#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>

#include <stdio.h>
#include <errno.h>

#if BSP_TTY_DMA_MODE == BSP_ENABLED

typedef struct
{
    char fifoData[160];
    size_t TxBytes;
    uint32_t lostBytes;

}ttyData_t;

ttyData_t ttyData;

fifo_t fifo = FIFO_INIT;

#endif

int _write(int file, char *ptr, int len)
{
    int i = 0;

    if (file == 1)
    {

#if BSP_TTY_DMA_MODE == BSP_ENABLED

        nvic_disable_irq(NVIC_DMA1_CHANNEL7_IRQ);

        i = fifoPut(&fifo, ptr);

        if (ttyData.TxBytes == 0)
        {
            ttyData.TxBytes = fifoGetReadBlock(&fifo, (void**)&ptr);

            dma_set_memory_address(DMA1, DMA_CHANNEL7, (uint32_t)ptr);
            dma_set_number_of_data(DMA1, DMA_CHANNEL7, ttyData.TxBytes);
            dma_enable_channel(DMA1, DMA_CHANNEL7);
            usart_enable_tx_dma(BSP_TTY_USART);
        }

        nvic_enable_irq(NVIC_DMA1_CHANNEL7_IRQ);

#else
        for (i = 0; i < len; i++)
            usart_send_blocking(BSP_TTY_USART, ptr[i]);
#endif

        return i;
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

    fifoFree(&fifo, ttyData.TxBytes);
    ttyData.TxBytes = fifoGetReadBlock(&fifo, (void**)&ptr);

    if (ttyData.TxBytes != 0)
    {
        dma_set_memory_address(DMA1, DMA_CHANNEL7, (uint32_t)ptr);
        dma_set_number_of_data(DMA1, DMA_CHANNEL7, ttyData.TxBytes);
        dma_enable_channel(DMA1, DMA_CHANNEL7);
        usart_enable_tx_dma(BSP_TTY_USART);
    }
}

#endif

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
    setvbuf(stdout,NULL, _IONBF, 0);

#if BSP_TTY_DMA_MODE == BSP_ENABLED

    ttyData.TxBytes = 0;
    ttyData.lostBytes = 0;

    fifoInit(&fifo, ttyData.fifoData, sizeof(ttyData.fifoData));

    rcc_periph_clock_enable(RCC_DMA1);

    nvic_set_priority(NVIC_DMA1_CHANNEL7_IRQ, 0);
    nvic_enable_irq(NVIC_DMA1_CHANNEL7_IRQ);

    dma_channel_reset(DMA1, DMA_CHANNEL7);
    dma_set_peripheral_address(DMA1, DMA_CHANNEL7, (uint32_t)&USART2_DR);
    dma_set_read_from_memory(DMA1, DMA_CHANNEL7);
    dma_enable_memory_increment_mode(DMA1, DMA_CHANNEL7);
    dma_set_peripheral_size(DMA1, DMA_CHANNEL7, DMA_CCR_PSIZE_8BIT);
    dma_set_memory_size(DMA1, DMA_CHANNEL7, DMA_CCR_MSIZE_8BIT);
    dma_set_priority(DMA1, DMA_CHANNEL7, DMA_CCR_PL_VERY_HIGH);

    dma_enable_transfer_complete_interrupt(DMA1, DMA_CHANNEL7);

#endif
}

bspStatus_t bspTTYSendData(uint8_t *pData, uint16_t siz)
{
    unused(pData);
    unused(siz);

    return BSP_OK;
}

bool bspTTYDataAvailable(void)
{
    return usart_get_flag(BSP_TTY_USART, USART_SR_RXNE);
}

char bspTTYGetChar(void)
{
    return usart_recv_blocking(BSP_TTY_USART);
}
