#include "led1_thread.h"
#include "hal_data.h"

extern TaskHandle_t led1_thread;

void g_timer0_callback(timer_callback_args_t *p_args)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (TIMER_EVENT_CYCLE_END == p_args->event)
    {
        vTaskNotifyGiveFromISR(led1_thread, &xHigherPriorityTaskWoken);
    }
}
