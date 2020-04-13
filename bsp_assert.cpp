/*
 * bsp-nucleo-f103, a generic bsp for nucleo f103rb based projects.
 *
 * Copyright (C) 2020 Julian Friedrich
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
 * You can file issues at https://github.com/fjulian79/bsp-stm32-f103.git
 */

#include <stdio.h>

#include <stm32f1xx_ll_system.h>

#include "bsp/bsp_gpio.h"
#include "bsp/bsp_tty.h"

#if BSP_ASSERT == BSP_ENABLED

void bspAbort(const char *pFunc, int line)
{
    uint8_t idx = 0;
    char buffer[80];

    /* Disable all interrupts to prevent all further actions */
    __disable_irq();

    snprintf(buffer, 80, "\nAssertion in %s(%d)\n", pFunc, line);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    /* ATTENTION: Do here all what's needed to prevent any kind of hardware 
     * damage or unintended actions 
     **/


    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    while(1)
    {
       
#if BSP_ASSERT_MESSAGE == BSP_ENABLED

        bspTTYAssertMessage(buffer);

#endif /* BSP_ASSERT_MESSAGE == BSP_ENABLED */

        idx=0;
        while(idx < (BSP_ASSERT_MESSAGE_MS/(BSP_ASSERT_LED_MS/2)))
        {
            bspGpioToggle(BSP_GPIO_LED);
            LL_mDelay(BSP_ASSERT_LED_MS/2);
            idx++;
        }
    }
}

#endif /* BSP_ASSERT == BSP_ENABLED */