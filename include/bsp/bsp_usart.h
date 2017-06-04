/*
 * bsp_usart.h
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#ifndef BSP_USART_H_
#define BSP_USART_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Used to setup the usart.
 *
 * @param baud the desired baud rate
 */
void bspTTYInit(uint32_t baud);

/**
 * Used to check if there is data available on the TTY.
 *
 * @return  true if there date.
 *          false it there is no data
 */
bool bspTTYDataAvailable(void);

/**
 * Used to read a single character frm the TTY.
 * @return
 */
char bspTTYGetChar(void);

#ifdef __cplusplus
}
#endif


#endif /* BSP_USART_H_ */
