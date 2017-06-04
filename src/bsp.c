/*
 * bsp.c
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#include "bsp/bsp.h"

#include "libopencm3/stm32/rcc.h"

void bspClockSetup(bspHSEBypass_t bypass, bspCpuSpeed_t speed)
{
    if (bypass == BSP_HSE_BYPASS_ON)
    {
        rcc_osc_off(RCC_HSE);
        rcc_osc_bypass_enable(RCC_HSE);
    }

    if (speed == BSP_CPU_SPEED_24MHZ)
    {
        rcc_clock_setup_in_hse_8mhz_out_24mhz();
    }
    else
    {
        rcc_clock_setup_in_hse_8mhz_out_72mhz();
    }
}
