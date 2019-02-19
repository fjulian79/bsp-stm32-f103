/*
 * bsp_config.h
 *
 *  Created on: Jun 25, 2017
 *      Author: julian
 */

#ifndef BSP_MOTORCTRL_BSP_ASSERT_H_
#define BSP_MOTORCTRL_BSP_ASSERT_H_

#include "bsp/bsp.h"

/**
 * Assertions are enabled/disabled in bsp_config.h.
 */
#if BSP_ASSERT == BSP_ENABLED

/**
 * Implementation of a assertion call which is used to implement all other
 * kinds of assertions.
 *
 * If using locks or things of that kind in here mention that this call might
 * be called from a interrupt service routine.
 */
void bspAssertDoCall(const char *pFunc, int line);

/**
 * Basic conditional assertion. 
 */
#define bspAssert(_x)                                                       \
                                                                            \
   if(!(_x))                                                                \
   {                                                                        \
      bspAssertDoCall(__FUNCTION__, __LINE__);                              \
   }

#else /* BSP_ASSERT == BSP_ENABLED */

/**
 * Empty declaration as assertions are disabled
 */
#define bspAssertDoCall(_func, _line)

/**
 * Empty declaration as assertions are disabled
 */
#define bspAssert(_x)

#endif /* BSP_ASSERT == BSP_ENABLED */

#endif /* BSP_MOTORCTRL_BSP_ASSERT_H_ */