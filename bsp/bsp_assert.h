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

#ifndef BSP_MOTORCTRL_BSP_ASSERT_H_
#define BSP_MOTORCTRL_BSP_ASSERT_H_

#include "bsp/bsp.h"

/**
 * Assertions are enabled/disabled in bsp_config.h.
 */
#if BSP_ASSERT == BSP_ENABLED

/**
 * @brief Implementation of a assertion call which is used to implement all 
 * other kinds of assertions.
 *
 * If using locks or things of that kind in here mention that this call might
 * be called from a interrupt service routine.
 * 
 * Actually there is no need to use this function directly, consider to use 
 * bspDoAssert()
 * 
 * @param pFunc The 
 * @param line 
 */
void bspAbort(const char *pFunc, int line);

/**
 * @brief Used to perfrom a assertion without any further condtions.
 */
#define bspDoAssert()      bspAbort(__FUNCTION__, __LINE__)

/**
 * @brief Used to perform a basic conditional assertion on the term _x. If _x 
 * will evaluate as false the assertion will take place.
 */
#define bspAssert(_x)                                                       \
                                                                            \
   if(!(_x))                                                                \
   {                                                                        \
      bspDoAssert();                                                        \
   }

#else /* BSP_ASSERT == BSP_ENABLED */

/**
 * @brief Empty declaration as assertions are disabled
 */
#define bspAbort(_func, _line)

/**
 * @brief Empty declaration as assertions are disabled
 */
#define bspDoAssert()

/**
 * @brief Empty declaration as assertions are disabled
 */
#define bspAssert(_x)

#endif /* BSP_ASSERT == BSP_ENABLED */

#endif /* BSP_MOTORCTRL_BSP_ASSERT_H_ */