#include "led2_thread.h"

extern TaskHandle_t led2_thread;

uint32_t const LONG_TIME = 500000;
uint32_t const SHORT_TIME = 5000;

/* LED2 Thread entry function */
/* pvParameters contains TaskHandle_t */
void led2_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);
    led2_thread = xTaskGetCurrentTaskHandle ();

    R_ICU_ExternalIrqOpen (&g_external_irq9_ctrl, &g_external_irq9_cfg);
    R_ICU_ExternalIrqEnable (&g_external_irq9_ctrl);

    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;

    /* TODO: add your own code here */
    while (1)
    {
        R_BSP_PinAccessEnable ();
        R_BSP_PinWrite (LED2, pin_level);
        R_BSP_PinAccessDisable ();

        R_GPT_Stop (&g_timer0_ctrl);
        R_GPT_PeriodSet (&g_timer0_ctrl, pin_level == BSP_IO_LEVEL_LOW ? LONG_TIME : SHORT_TIME);
        R_GPT_Start (&g_timer0_ctrl);
        pin_level = !pin_level;
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}
