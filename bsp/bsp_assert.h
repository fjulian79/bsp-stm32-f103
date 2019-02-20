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