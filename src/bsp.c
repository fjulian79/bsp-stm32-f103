/*
 * bsp.c
 *
 *  Created on: Jun 4, 2017
 *      Author: julian
 */

#include "bsp/bsp.h"

#include "libopencm3/stm32/rcc.h"

void bspClockSetup(void)
{
    /*
     * We want to use full speed .. check that the clock from the st-link
     * is provided to the mcu.
     * */
    rcc_osc_off(RCC_HSE);
    rcc_osc_bypass_enable(RCC_HSE);
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}
