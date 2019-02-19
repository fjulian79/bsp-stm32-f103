#include <stdio.h>

#include <stm32f1xx_ll_system.h>

#include "bsp/bsp_gpio.h"
#include "bsp/bsp_tty.h"

#if BSP_ASSERT == BSP_ENABLED

void bspAssertDoCall(const char *pFunc, int line)
{
    uint8_t idx = 0;
    char buffer[80];

    /* Disable all interrupts to prevent all further actions */
    __disable_irq();

    snprintf (buffer, 80, "\nAssertion in %s(%d)\n", pFunc, line);

    /* Do here all what's needed to prevent any kind of hardware damage or 
     * unintended actions */

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