/*
 * bsp-usart.c
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#include <stm32f1xx_ll_usart.h>
#include <stm32f1xx_ll_dma.h>

#include "bsp/bsp.h"
#include "bsp/bsp_gpio.h"
#include "bsp/bsp_tty.h"
#include "generic/generic.h"
#include "generic/fifo.h"

#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

#if BSP_TTY_TX_DMA == BSP_ENABLED

/**
 * TTY Data shared with the interrupt.
 */
struct
{
    char Data[BSP_TTY_TX_BUFSIZ];
    volatile size_t TxBytes;
    uint32_t NumLost;

} ttyTxData;

/**
 * Generic fifo instance operation on ttyData.Data
 */
Fifo *pTxFifo;

/**
 * Starts a DMS transfer at the given address with the given length.
 *
 * @param pData     Address to start.
 * @param siz       Number of bytes to transfer.
 */
void startDmaTx(uint8_t *pData, size_t siz)
{
    LL_DMA_SetMemoryAddress(DMA1, TTY_TXDMACH_LLCH, (uint32_t)pData);
    LL_DMA_SetDataLength(DMA1, TTY_TXDMACH_LLCH, siz);
    LL_USART_EnableDMAReq_TX(TTY_USARTx);
    LL_DMA_EnableChannel(DMA1, TTY_TXDMACH_LLCH);
}

/**
 * TTY Tx DMA Interrupt handler.
 */
extern "C" void TTY_TXDMACH_IRQHandler(void)
{
    uint8_t *ptr = NULL;

    if(TTY_TXDMACH_ISACTIVEFLAG_TC())
    {
        TTY_TXDMACH_CLEARFLAG_TC();

        LL_USART_DisableDMAReq_TX(TTY_USARTx);
        LL_DMA_DisableChannel(DMA1, TTY_TXDMACH_LLCH);

        pTxFifo->free(ttyTxData.TxBytes);
        ttyTxData.TxBytes = pTxFifo->getReadBlock((void**)&ptr);

        if (ttyTxData.TxBytes != 0)
            startDmaTx(ptr, ttyTxData.TxBytes);
    }
    else
    {
        while(1)
        {
            bspGpioToggle(BSP_GPIO_LED);
            LL_mDelay(50);
        }
    }
}

#endif /* BSP_TTY_TX_DMA == BSP_ENABLED */

/**
 * Called by c library for printf calls.
 *
 * @param file      The used Stream number.
 * @param pData     The data to write.
 * @param siz       The number of bytes to write.
 *
 * @return  siz,if BSP_TTY_BLOCKING is disabled.
 *          The number of bytes really written to the tx buffer
 *          if BSP_TTY_BLOCKING is enabled.
 */

extern "C" int _write(int file, char *pData, int siz)
{
#if BSP_TTY_TX_DMA == BSP_ENABLED

    int tmp = 0;
    uint8_t *ptr = NULL;

    NVIC_DisableIRQ(TTY_TXDMACH_IRQn);

    if (siz == 1)
        tmp = pTxFifo->put(pData);
    else
        tmp = pTxFifo->write(pData, siz);

    if (ttyTxData.TxBytes == 0)
    {
        ttyTxData.TxBytes = pTxFifo->getReadBlock((void**)&ptr);
        startDmaTx(ptr, ttyTxData.TxBytes);
    }

    NVIC_EnableIRQ(TTY_TXDMACH_IRQn);

#if BSP_TTY_BLOCKING == BSP_ENABLED

    while (pTxFifo->getFree() == 0 && (tmp < siz));
    return tmp;

#else /* BSP_TTY_BLOCKING == BSP_ENABLED */

    unused(tmp);
    return siz;

#endif /* BSP_TTY_BLOCKING == BSP_ENABLED */

#else /* #if BSP_TTY_TX_DMA == BSP_ENABLED */

    for (int pos = 0; pos < siz; pos++)
    {
        while (!LL_USART_IsActiveFlag_TXE(TTY_USARTx));
        LL_USART_TransmitData8(TTY_USARTx, pData[pos]);
    }

    return siz;

#endif

    errno = EIO;
    return -1;
}

/**
 * Called by scanf calls to read from the given stream.
 *
 * @param file  The stream to read from.
 * @param ptr   The buffer to write to.
 * @param len   The number of bytes to read.
 *
 * @return  TBD
 */
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
    LL_USART_InitTypeDef init;

    init.BaudRate = baud;
    init.DataWidth = LL_USART_DATAWIDTH_8B;
    init.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    init.Parity = LL_USART_PARITY_NONE;
    init.StopBits = LL_USART_STOPBITS_1;
    init.TransferDirection = LL_USART_DIRECTION_TX_RX;
    LL_USART_Init(TTY_USARTx, &init);

    LL_USART_Enable(TTY_USARTx);

#if BSP_TTY_TX_DMA == BSP_ENABLED

    LL_DMA_InitTypeDef dma;

    ttyTxData.TxBytes = 0;
    ttyTxData.NumLost = 0;
    pTxFifo = new Fifo(ttyTxData.Data, sizeof(ttyTxData.Data));

    dma.Mode = LL_DMA_MODE_NORMAL;
    dma.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    dma.Priority = LL_DMA_PRIORITY_HIGH;
    dma.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    dma.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    dma.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
    dma.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
    dma.PeriphOrM2MSrcAddress = LL_USART_DMA_GetRegAddr(TTY_USARTx);
    LL_DMA_Init(DMA1, TTY_TXDMACH_LLCH, &dma);

    NVIC_SetPriority(TTY_TXDMACH_IRQn, 0);
    NVIC_EnableIRQ(TTY_TXDMACH_IRQn);
    LL_DMA_EnableIT_TC(DMA1, TTY_TXDMACH_LLCH);
    LL_DMA_EnableIT_TE(DMA1, TTY_TXDMACH_LLCH);

#endif /* BSP_TTY_TX_DMA == BSP_ENABLED */
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
    return LL_USART_IsActiveFlag_RXNE(TTY_USARTx) ? true : false;
}

char bspTTYGetChar(void)
{
    while (!bspTTYDataAvailable());

    return LL_USART_ReceiveData8(TTY_USARTx);
}
