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

#ifndef BSP_SMARTSINK_ADC_H_
#define BSP_SMARTSINK_ADC_H_

#include "bsp/bsp.h"

/**
 * The minimal number of Systicks which can be used as oversampling duration.
 */
#define BSP_ADC_SAMPLETICKS_MIN           1

/**
 * The maximum number of Systicks which can be used as oversampling duration.
 */
#define BSP_ADC_SAMPLETICKS_MAX           1000

/**
 * Initializes the adc and all other needed peripheral units as given by the 
 * configuration.
 */
void bspAdcInit(void);

/**
 * @brief   Used to set the duration in Systicks the ADC will accumulate 
 *          measurement results to eliminate jitter.
 *
 * Note: This will not clear existing results.
 * 
 * @param sampleTicks   Numnber of SysTicks to sample the ADC chanel before a
 *                      result will be calculated.
 */
bspStatus_t bspAdcSetSampleTicks(uint32_t ticks);

/**
 * Used to Reset the internal data structures. All existing measurments will be
 * discarded and the oversampling period starts again.
 * 
 */
void bspAdcResetResults(void);

/**
 * @brief Used to get the result of the last measurements
 * 
 * @param pData      Point to a arry large enought to take the results.
 * 
 * @return uint32_t  The number of ADC samples contributed to the results. 
 */
uint32_t bspAdcGetResult(uint32_t *pData);

#endif /* BSP_SMARTSINK_ADC_H_ */
