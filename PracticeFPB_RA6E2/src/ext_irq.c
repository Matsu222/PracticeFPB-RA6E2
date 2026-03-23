#include "hal_data.h"
#include "common_data.h"
#include "led2_thread.h"

extern TaskHandle_t led2_thread;

void g_irq9_callback(external_irq_callback_args_t *p_args)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR (led2_thread, &xHigherPriorityTaskWoken);

}
