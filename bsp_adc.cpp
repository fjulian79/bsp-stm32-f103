/*
 * bsp-samrtsink, the board support package for the hardware used in the 
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

#include "bsp/bsp.h"
#include "bsp/bsp_gpio.h"
#include "bsp/bsp_adc.h"

#include <stm32f1xx_ll_adc.h>
#include <stm32f1xx_ll_dma.h>

#include "generic/generic.h"

#include <string.h>

/**
 * Number of CPU Cycles to wait prior ADC Calibration
 */
#define BSP_ADC_CALIBDELAY          (LL_ADC_DELAY_ENABLE_CALIB_ADC_CYCLES * 32)

/**
 * Defines the Multiplactor for the conversion to mV per Cel
 */
const uint32_t bspAdcNum[BSP_ADC_LIPOCELLS] = 
        {40035, 2255, 14025, 4565, 1375, 434133};

/**
 * Defines the Divisor (as digits to shift) for the conversion to mV per Cel
 */
const uint8_t bspAdcDen[BSP_ADC_LIPOCELLS] = 
        {15, 10, 12, 10, 8, 16};

/**
 * The buffer used for the DMA transfers;
 */
volatile uint16_t bspAdcDmaBuffer[BSP_ADC_LIPOCELLS];

/**
 * Data structure used for storing raw accumulated adc values and the number 
 * of contributing samples 
 */
typedef struct
{
    uint32_t Samples;
    uint32_t Data[BSP_ADC_LIPOCELLS];

} bspAdcRawData_t;

/**
 * The Global ADC data structure
 */
struct 
{
    bspAdcRawData_t Accu;
    bspAdcRawData_t Result;

    uint32_t LastTick;
    uint32_t SampleTicks;

} bspAdcData;

/**
 * ADC DMA Interrupt handler.
 */
extern "C" void BSP_ADC_DMACH_IRQHandler(void)
{
    uint32_t sysTick = 0;

    if(BSP_ADC_DMACH_ISACTIVEFLAG_TC())
    {
        BSP_ADC_DMACH_CLEARFLAG_TC();

        sysTick = bspGetSysTick();
        bspAdcData.Accu.Samples++;

        for(uint8_t pos = 0; pos < BSP_ADC_LIPOCELLS; pos++)
        {
            bspAdcData.Accu.Data[pos] += bspAdcDmaBuffer[pos];
        }

        if(sysTick - bspAdcData.LastTick > bspAdcData.SampleTicks)
        {   
            bspAdcData.LastTick = sysTick;

            bspAdcData.Result.Samples = bspAdcData.Accu.Samples;

            memcpy((void*)&bspAdcData.Result, (void*)&bspAdcData.Accu, sizeof(bspAdcRawData_t));
            memset((void*)&bspAdcData.Accu, 0, sizeof(bspAdcRawData_t));
        }

        LL_ADC_REG_StartConversionSWStart(ADC1);
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

/**
 * Resets the ADC data structure.
 *
 * @param sampleTicks   Numnber of SysTicks to sample the ADC chanel before a
 *                  result will be calculated.
 */
void bspAdcDataInit(uint16_t sampleTicks)
{
    memset((void*)&bspAdcData, 0, sizeof(bspAdcData));

    bspAdcData.SampleTicks = sampleTicks;
    bspAdcData.LastTick = bspGetSysTick();
}

void bspAdcInit(void)
{
    LL_DMA_InitTypeDef dma;
    LL_ADC_InitTypeDef adcInit;
    LL_ADC_REG_InitTypeDef adcRegInit;
    uint32_t temp = 0;

    dma.Mode = LL_DMA_MODE_CIRCULAR;
    dma.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
    dma.Priority = LL_DMA_PRIORITY_HIGH;
    dma.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
    dma.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
    dma.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
    dma.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
    dma.PeriphOrM2MSrcAddress = LL_ADC_DMA_GetRegAddr(ADC1, 
        LL_ADC_DMA_REG_REGULAR_DATA);
    LL_DMA_Init(DMA1, BSP_ADC_DMACH_LLCH, &dma);

    LL_DMA_SetMemoryAddress(DMA1, BSP_ADC_DMACH_LLCH, 
        (uint32_t)&bspAdcDmaBuffer);
    LL_DMA_SetDataLength(DMA1, BSP_ADC_DMACH_LLCH, 
        arraysize(bspAdcDmaBuffer));

    NVIC_SetPriority(BSP_ADC_DMACH_IRQn, 0);
    NVIC_EnableIRQ(BSP_ADC_DMACH_IRQn);
    LL_DMA_EnableIT_TC(DMA1, BSP_ADC_DMACH_LLCH);
    LL_DMA_EnableIT_TE(DMA1, BSP_ADC_DMACH_LLCH);

    adcInit.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    adcInit.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;
    LL_ADC_Init(ADC1, &adcInit);

    adcRegInit.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    adcRegInit.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_6RANKS;
    adcRegInit.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    adcRegInit.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
    adcRegInit.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
    LL_ADC_REG_Init(ADC1, &adcRegInit);

    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, BSP_ADC_CH_LIPO1);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, BSP_ADC_CH_LIPO2);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3, BSP_ADC_CH_LIPO3);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_4, BSP_ADC_CH_LIPO4);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_5, BSP_ADC_CH_LIPO5);
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_6, BSP_ADC_CH_LIPO6);

    LL_ADC_SetChannelSamplingTime(ADC1, BSP_ADC_CH_LIPO1, BSP_ADC_SAMPLETIME);
    LL_ADC_SetChannelSamplingTime(ADC1, BSP_ADC_CH_LIPO2, BSP_ADC_SAMPLETIME);
    LL_ADC_SetChannelSamplingTime(ADC1, BSP_ADC_CH_LIPO3, BSP_ADC_SAMPLETIME);
    LL_ADC_SetChannelSamplingTime(ADC1, BSP_ADC_CH_LIPO4, BSP_ADC_SAMPLETIME);
    LL_ADC_SetChannelSamplingTime(ADC1, BSP_ADC_CH_LIPO5, BSP_ADC_SAMPLETIME);
    LL_ADC_SetChannelSamplingTime(ADC1, BSP_ADC_CH_LIPO6, BSP_ADC_SAMPLETIME);

    /* Note: Variable divided by 2 to compensate partially CPU processing 
     * cycles. (depends on compilation optimization). 
     * */
    temp = BSP_ADC_CALIBDELAY >> 1;

    LL_ADC_Enable(ADC1);
    while(temp != 0) temp--;
    
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0);

    bspAdcDataInit(BSP_ADC_SAMPLETICKS_DEFAULT);
    LL_DMA_EnableChannel(DMA1, BSP_ADC_DMACH_LLCH);
    LL_ADC_REG_StartConversionSWStart(ADC1);
}

bspStatus_t bspAdcSetSampleTicks(uint32_t ticks)
{
    if ((ticks > BSP_ADC_SAMPLETICKS_MAX) || (ticks < BSP_ADC_SAMPLETICKS_MIN))
        return BSP_EEINVAL;

    NVIC_DisableIRQ(BSP_ADC_DMACH_IRQn);
    bspAdcData.SampleTicks = ticks;
    NVIC_EnableIRQ(BSP_ADC_DMACH_IRQn);

    return BSP_OK;
}

void bspAdcResetResults(void)
{
    uint32_t sampleTicks = bspAdcData.SampleTicks;

    NVIC_DisableIRQ(BSP_ADC_DMACH_IRQn);
    bspAdcDataInit(sampleTicks);
    NVIC_EnableIRQ(BSP_ADC_DMACH_IRQn);
}

uint32_t  bspAdcGetResult(uint32_t *pDat)
{
    uint32_t samples = 0;

    /* Copy the data in a critical section, process it later */
    NVIC_DisableIRQ(BSP_ADC_DMACH_IRQn);
    memcpy(pDat, bspAdcData.Result.Data, sizeof(bspAdcData.Result.Data));
    samples = bspAdcData.Result.Samples;
    NVIC_EnableIRQ(BSP_ADC_DMACH_IRQn);

    for(uint8_t pos = 0; pos < BSP_ADC_LIPOCELLS; pos++)
    {
        /* the data my contain of several samples, normalize it */
        pDat[pos] /= samples;
        /* convert to mv */
        pDat[pos] = (pDat[pos] * bspAdcNum[pos]) >> bspAdcDen[pos];
    }

    return samples;
}
