#include "hal_data.h"
#include "blinky_thread.h"

extern TaskHandle_t blinky_thread;

void g_timer0_callback(timer_callback_args_t *p_args)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (TIMER_EVENT_CYCLE_END == p_args->event)
    {
        vTaskNotifyGiveFromISR(blinky_thread, &xHigherPriorityTaskWoken);
    }
}
