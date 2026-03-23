/*
 * Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blinky_thread.h"
#include "common_data.h"

extern TaskHandle_t blinky_thread;

/* Blinky Thread entry function */
void blinky_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);
    blinky_thread = xTaskGetCurrentTaskHandle ();

    R_GPT_Open (&g_timer0_ctrl, &g_timer0_cfg);
    R_GPT_Start(&g_timer0_ctrl);

    R_ICU_ExternalIrqOpen(&g_external_irq9_ctrl, &g_external_irq9_cfg);
    R_ICU_ExternalIrqEnable(&g_external_irq9_ctrl);

    /* Holds level to set for pins */
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;

    while (1)
    {
        /* Enable access to the PFS registers. If using r_ioport module then register protection is automatically
         * handled. This code uses BSP IO functions to show how it is used.
         */
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        R_BSP_PinAccessEnable ();
        R_BSP_PinWrite (LED1, pin_level);

        /* Protect PFS registers */
        R_BSP_PinAccessDisable ();
        /* Toggle level for next write */
        pin_level = !pin_level;
    }
}
