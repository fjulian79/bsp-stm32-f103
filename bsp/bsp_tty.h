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

#ifndef BSP_NUCLEO_F103_TTY_H_
#define BSP_NUCLEO_F103_TTY_H_

#include "bsp/bsp.h"

/**
 * @brief Used to setup the usart.
 *
 * @param baud the desired baud rate
 */
void bspTTYInit(uint32_t baud);

/**
 * @brief Used to transmit any data.
 *
 * @param pData     Pointer to the data to transmit.
 * @param siz       Number of bytes.
 *
 * @return          Number of transmitted bytes.
 */
bspStatus_t bspTTYSendData(uint8_t *pData, uint16_t siz);

/**
 * @brief Used to check if there is data available on the TTY.
 *
 * @return  true if there date.
 *          false it there is no data
 */
bool bspTTYDataAvailable(void);

/**
 * @brief Used to read a single character from the TTY.
 * 
 * Hence that this function will block.
 *
 * @return  The read character.
 */
char bspTTYGetChar(void);

#if BSP_ASSERT_MESSAGE == BSP_ENABLED

/**
 * @brief Used to print a assert message from any possible context.
 * 
 * This function can be assumed to prevent the tty from working as usual. The 
 * root cause for that behaviour is that this function as to work in any 
 * possible context, also in every interrupt.
 */
void bspTTYAssertMessage(char *pChar);

#endif /* BSP_ASSERT_MESSAGE == BSP_ENABLED */

#endif /* BSP_NUCLEO_F103_TTY_H_ */
