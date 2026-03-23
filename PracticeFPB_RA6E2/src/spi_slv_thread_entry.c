#include "hal_data.h"
#include "spi_slv_thread.h"

#define SPI_BUF_SIZE 255
#define SPI_LENGTH 1

uint8_t tx_buf[SPI_BUF_SIZE];
uint8_t rx_buf[SPI_BUF_SIZE];
uint8_t received_buf[SPI_BUF_SIZE];
uint8_t return_buf[SPI_BUF_SIZE];
uint8_t count;
uint8_t last_count;

extern TaskHandle_t spi_slv_thread;

/* SPI SLV Thread entry function */
/* pvParameters contains TaskHandle_t */
void spi_slv_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);
    spi_slv_thread = xTaskGetCurrentTaskHandle ();

    R_GPT_Open (&g_timer1_ctrl, &g_timer1_cfg);
    R_SPI_Open (&g_spi1_ctrl, &g_spi1_cfg);

    count = 0;
    last_count = 0;
    /* TODO: add your own code here */
    while (1)
    {
        R_SPI_WriteRead (&g_spi1_ctrl, tx_buf, rx_buf, SPI_LENGTH, SPI_BIT_WIDTH_8_BITS);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    }
}

void set_spi_buf()
{
    for (uint8_t i = 0; i < SPI_LENGTH; i++)
    {
        received_buf[i + count] = rx_buf[i];
        count++;
        //tx_buf[i] = rx_buf[i];
        if (last_count > 0 & count < last_count)
        {
            tx_buf[i] = return_buf[i + count];
        }
        else
        {
            tx_buf[i] = 0;
        }
    }
}

void spi1_callback(spi_callback_args_t *p_args)
{
    if (p_args->event == SPI_EVENT_TRANSFER_COMPLETE)
    {
        set_spi_buf ();
    }
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(spi_slv_thread, &xHigherPriorityTaskWoken);
    R_GPT_Stop (&g_timer1_ctrl);
    R_GPT_Start (&g_timer1_ctrl);
}

void g_timer1_callback(timer_callback_args_t *p_args)
{
    if (TIMER_EVENT_CYCLE_END == p_args->event)
    {
        R_GPT_Stop (&g_timer1_ctrl);
        last_count = count;
        count = 0;
        for (uint8_t i = 0; i < SPI_BUF_SIZE; i++)
        {
            return_buf[i] = received_buf[i];
        }
        tx_buf[0] = return_buf[0];
        R_SPI_Close (&g_spi1_ctrl);
        R_SPI_Open (&g_spi1_ctrl, &g_spi1_cfg);
        R_SPI_WriteRead (&g_spi1_ctrl, tx_buf, rx_buf, SPI_LENGTH, SPI_BIT_WIDTH_8_BITS);
    }
}

