/*
 * bsp-motorctrl, a board support package library for a STM32 based motor 
 * control PCB. It is designed to abstract access to HW features in a generic 
 * and simple way. Please note thet it should not conain any buissness logic.
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
 * You can file issues at https://github.com/fjulian79/bsp-motorctrl
 */

#include "bsp/bsp.h"
#include "bsp/bsp_exti.h"
#include "bsp/bsp_assert.h"

#include <stm32f1xx_ll_exti.h>

/**
 * @brief Waek declaration of the users button irq callback.
 */
void bspButtonIrqCb(void) __attribute__((weak, alias("bspDefaultExtiCb")));

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defnition of the default empty handler if the user has not 
 * implemented his own.
 * 
 * ATTENTION: If you end up here a EXTI interrupt has occured and the user has 
 * not implemented the corresponding callback. 
 */
void bspDefaultExtiCb(void)
{
   bspDoAssert();
}

/**
 * @brief Common interrupt handler for all external interrupts.
 * 
 */
void EXTI15_10_IRQHandler(void)
{
   if(LL_EXTI_IsActiveFlag_0_31(BSP_BUTTON_EXTI_LINE) != RESET)
   {
      LL_EXTI_ClearFlag_0_31(BSP_BUTTON_EXTI_LINE);
      bspButtonIrqCb();
   }
   else
   {
      bspDoAssert();
   }
}

#ifdef __cplusplus
}
#endif

void bspExtiInit(void)
{           
   LL_GPIO_AF_SetEXTISource(
         BSP_BUTTON_GPIO_EXTI_PORT, BSP_BUTTON_GPIO_EXTI_LINE); 
   LL_EXTI_EnableIT_0_31(BSP_BUTTON_EXTI_LINE);
   LL_EXTI_EnableFallingTrig_0_31(BSP_BUTTON_EXTI_LINE);

   NVIC_EnableIRQ(EXTI15_10_IRQn); 
   NVIC_SetPriority(EXTI15_10_IRQn, BSP_IRQPRIO_EXTI);
}
