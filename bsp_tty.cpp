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

#include <stm32f1xx_ll_usart.h>
#include <stm32f1xx_ll_dma.h>

#include "bsp/bsp.h"
#include "bsp/bsp_gpio.h"
#include "bsp/bsp_tty.h"
#include "bsp/bsp_assert.h"

#include "generic/generic.hpp"
#include "generic/fifo.hpp"

#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

#if BSP_TTY_TX_DMA == BSP_ENABLED

/**
 * @brief TTY Data shared with the interrupt.
 */
struct
{
    char Data[BSP_TTY_TX_BUFSIZ];
    volatile size_t TxBytes;
    uint32_t NumLost;

} ttyTxData;

/**
 * @brief Generic fifo instance operation on ttyData.Data
 */
Fifo *pTxFifo;

/**
 * @brief Starts a DMS transfer at the given address with the given length.
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
 * @brief TTY Tx DMA Interrupt handler.
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
        bspDoAssert();
    }
}

#endif /* BSP_TTY_TX_DMA == BSP_ENABLED */

#if BSP_TTY_RX_IRQ == BSP_ENABLED

/**
 * @brief TTY Rx data shared with the interrupt.
 */
struct
{
    char Data[BSP_TTY_RX_BUFSIZ];
    uint32_t NumLost;

} ttyRxData;

/**
 * @brief Generic fifo instance operation on ttyData.Data
 */
Fifo *pRxFifo;

extern "C" void TTY_USARTx_IRQHandler(void)
{
    if(   LL_USART_IsActiveFlag_RXNE(TTY_USARTx)
       && LL_USART_IsEnabledIT_RXNE(TTY_USARTx))
    {
        uint8_t data = LL_USART_ReceiveData8(TTY_USARTx);

        if (pRxFifo->put(&data))
            ttyRxData.NumLost++;
    }
}

#endif /* BSP_TTY_RX_IRQ == BSP_ENABLED */

/**
 * @brief Called by c library for printf calls.
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

    do
    {
        NVIC_DisableIRQ(TTY_TXDMACH_IRQn);

        if ((siz - tmp) == 1)
            tmp += pTxFifo->put(pData + tmp);
        else
            tmp += pTxFifo->write(pData + tmp, (siz - tmp));

        if (ttyTxData.TxBytes == 0)
        {
            ttyTxData.TxBytes = pTxFifo->getReadBlock((void**)&ptr);
            startDmaTx(ptr, ttyTxData.TxBytes);
        }

        NVIC_EnableIRQ(TTY_TXDMACH_IRQn);

#if BSP_TTY_BLOCKING == BSP_ENABLED

        while (pTxFifo->getFree() == 0 && (tmp < siz));

    } while (tmp < siz);
    
    return tmp;

#else /* BSP_TTY_BLOCKING == BSP_ENABLED */

    } while (0);

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
 * @brief Called by scanf calls to read from the given stream.
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

#if BSP_TTY_RX_IRQ == BSP_ENABLED

    ttyRxData.NumLost = 0;
    pRxFifo = new Fifo(ttyRxData.Data, sizeof(ttyRxData.Data));

    NVIC_SetPriority(USART2_IRQn, BSP_IRQPRIO_TTY);
    NVIC_EnableIRQ(USART2_IRQn);
    LL_USART_EnableIT_RXNE(TTY_USARTx);

#endif /* BSP_TTY_RX_IRQ == BSP_ENABLED */

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

    NVIC_SetPriority(TTY_TXDMACH_IRQn, BSP_IRQPRIO_TTY);
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
#if BSP_TTY_RX_IRQ == BSP_ENABLED

    return pRxFifo->getUsed() != 0 ? true : false;

#else /* BSP_TTY_RX_IRQ == BSP_ENABLED */

    return LL_USART_IsActiveFlag_RXNE(TTY_USARTx) ? true : false;

#endif /* BSP_TTY_RX_IRQ == BSP_ENABLED */
}

char bspTTYGetChar(void)
{
    while (!bspTTYDataAvailable());

#if BSP_TTY_RX_IRQ == BSP_ENABLED

    char data;
    pRxFifo->get(&data);

    return data;

#else /* BSP_TTY_RX_IRQ == BSP_ENABLED */

    return LL_USART_ReceiveData8(TTY_USARTx);

#endif /* BSP_TTY_RX_IRQ == BSP_ENABLED */
}

#if BSP_ASSERT_MESSAGE == BSP_ENABLED

void bspTTYAssertMessage(char *pChar)
{
#if BSP_TTY_TX_DMA == BSP_ENABLED

    /* If a transfer is ongoing let it complete and disable the DMA once it 
     * is done */
    if(LL_DMA_IsEnabledChannel(DMA1, TTY_TXDMACH_LLCH))
    {
        while(!TTY_TXDMACH_ISACTIVEFLAG_TC());
        TTY_TXDMACH_CLEARFLAG_TC();
        LL_USART_DisableDMAReq_TX(TTY_USARTx);
        LL_DMA_DisableChannel(DMA1, TTY_TXDMACH_LLCH);
    }

#endif /* BSP_TTY_TX_DMA == BSP_ENABLED */    

    /* Do use DMA here as interrupts will most likely not work anymore */
    while (*pChar != 0)
    {
        while (!LL_USART_IsActiveFlag_TXE(TTY_USARTx));
        LL_USART_TransmitData8(TTY_USARTx, *pChar);
        pChar++;
    }
}

#endif /* BSP_ASSERT_MESSAGE == BSP_ENABLED */
