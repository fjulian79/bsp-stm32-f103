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

#include "bsp/bsp_flash.h"

#include <stm32f1xx.h>
#include <stm32f1xx_hal_flash.h>

#include <stdint.h>

/**
 * Defines the bsp internal timeout in ms for flash operations.
 * 
 * I have cosidered this to become a part of the bsp configuration, but it has
 * turned out that on a stm32f103 a write or erase opteration block the flash
 * controller from fetching further instructions. Therefore no code can be 
 * executed while programming or erasing flash. The wait function does not
 * really makes that much sense!
 * 
 * Therefore this setting is hidden and hardcoded to 1ms!
 */
#define BSP_FLASH_TIMEOUT               1

/**
 * @brief Bitmask of all the flash operation mode bits.
 */
#define BSP_FLASH_MODEBITS                                      \
                                                                \
        (FLASH_CR_PG | FLASH_CR_PER | FLASH_CR_MER | FLASH_CR_OPTPG)

/**
 * @brief Needed to make shure to hae only the desired mode bit set.
 */
#define BSP_FLASH_SETMODE(_mode)                                \
                                                                \
        do                                                      \
        {                                                       \
            CLEAR_BIT(FLASH->CR, BSP_FLASH_MODEBITS);           \
            SET_BIT(FLASH->CR, _mode);                          \
        }while(0);

/**
 * @brief Like errno.
 */
uint32_t bspFlashErr = 0;

/**
 * @brief Used to check the error flags and clean them if needed. Actually it 
 * seams as further operations will fail until a set error flag gets cleared. 
 * 
 * In case of a error bspFlashGetErr() will provide the error code.
 * 
 * @return BSP_OK   In case of ne error
 *                  In case of any error.
 */
bspStatus_t bspFlashSetErr(void)
{
    bspFlashErr = 0;

    if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_WRPERR))
    {
        bspFlashErr |= HAL_FLASH_ERROR_WRP;
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_WRPERR);
    }

    if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_PGERR))
    {
        bspFlashErr |= HAL_FLASH_ERROR_PROG;
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGERR);
    }

    if(__HAL_FLASH_GET_FLAG(FLASH_FLAG_OPTVERR))
    {
        bspFlashErr |= HAL_FLASH_ERROR_OPTV;
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    }

    return bspFlashErr == 0 ? BSP_OK : BSP_ERR;
}  

uint32_t bspFlashGetErr(void)
{
    return bspFlashErr;
}

bspStatus_t bspFlashUnlock(void)
{
    if (HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK))
    {
        WRITE_REG(FLASH->KEYR, FLASH_KEY1);
        WRITE_REG(FLASH->KEYR, FLASH_KEY2);

        if (HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK))
        {
            return BSP_ERR;
        }
    }

    return BSP_OK; 
}

bspStatus_t bspFlashLock(void)
{
    /* Set the LOCK Bit if it is not set to lock the 
     * FLASH Registers access */
    if (!HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK))
    {
        SET_BIT(FLASH->CR, FLASH_CR_LOCK);

        if (!HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK))
        {
            return BSP_ERR;
        }   
    }

    return BSP_OK;
}

bspStatus_t bspFlashProgHalfWord(uint16_t *addr, uint16_t data)
{
    if (HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK))
        return BSP_ELOCK;

    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))
        return BSP_EBUSY;

    if (addr < (uint16_t*)FLASH_BASE || (addr+1) > (uint16_t*)FLASH_BANK1_END)
        return BSP_ERANGE;

    /* Proceed to program the new data */
    BSP_FLASH_SETMODE(FLASH_CR_PG);
    *(__IO uint16_t*)addr = data;

    return bspFlashWaitForLastOperation(BSP_FLASH_TIMEOUT);
}

bspStatus_t bspFlashProg(uint16_t *addr, void *pData, size_t siz)
{
    bspStatus_t ret = BSP_OK;

    if (HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK))
        return BSP_ELOCK;

    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))
        return BSP_EBUSY;

    if (addr < (uint16_t*)FLASH_BASE || (addr+siz) > (uint16_t*)FLASH_BANK1_END)
        return BSP_ERANGE;

    if (siz%2 != 0)
        return BSP_ESIZE;

    /* The size argument is expected to be byte based. But can only write 16 Bit
     * at once. Previously there is a check if the given size is a even number, 
     * so we can dive the given size now savely by two as preparation for 
     * programming halve words. 
     * */
    siz = siz/2;

    BSP_FLASH_SETMODE(FLASH_CR_PG);

    for(size_t i = 0; i < siz; i++)
    {
        ((__IO uint16_t*)addr)[i] = ((uint16_t*)pData)[i];
        ret = bspFlashWaitForLastOperation(BSP_FLASH_TIMEOUT);
        if (ret  != BSP_OK)
            break;
    }

    return ret;
}

bspStatus_t bspFlashErasePage(uint16_t *addr)
{
    if (HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK))
        return BSP_ELOCK;

    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY))
        return BSP_EBUSY;

    BSP_FLASH_SETMODE(FLASH_CR_PER);

    WRITE_REG(FLASH->AR, ((uint32_t)addr));
    SET_BIT(FLASH->CR, FLASH_CR_STRT);

    return bspFlashWaitForLastOperation(BSP_FLASH_TIMEOUT);
}

bspStatus_t bspFlashWaitForLastOperation(uint32_t timeout)
{
    uint32_t tickStart = bspGetSysTick();

    /* guarantee minimum wait */
    timeout++;

    /* Wait for the FLASH operation to complete by polling on BUSY flag to be 
     * reset. Even if the FLASH operation fails, the BUSY flag will be reset 
     * and an error flag will be set. The error check is performed by
     * bspFlashSetErr();
     * */
    while(__HAL_FLASH_GET_FLAG(FLASH_FLAG_BSY)) 
    { 
        if (timeout != HAL_MAX_DELAY)
        {
            if((timeout == 0) || ((bspGetSysTick()-tickStart) > timeout))
                return BSP_ETIMEOUT;
        }
    }
  
    if (__HAL_FLASH_GET_FLAG(FLASH_FLAG_EOP))
        __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP);
  
    return bspFlashSetErr();
}
