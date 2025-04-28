/**
 * @file LD14.c
 * @author bignut
 * @brief 
 * @details
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "LD14.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "string.h"

#define TXD_GPIO    (GPIO_NUM_25)
#define RXD_GPIO    (GPIO_NUM_26)

static const int RX_BUF_SIZE = 1024;

static void LD14_uart_lnlt()
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_GPIO, RXD_GPIO, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char *Name, const char *data)
{
    const int len = strlen(data);

    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);

    ESP_LOGI(Name, "uart has trans %d : %s", txBytes, data);

    return txBytes;
}


static void txTask()
{
    static const char *TAG = "TX_TASK";
    while(1)
    {
        sendData(TAG, "Hello-World...");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

static void rxTask(){
    static const char *TAG = "RX_TASK";

    uint8_t *data = (uint8_t *)malloc(RX_BUF_SIZE + 1);
    while(1)
    {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
        if(rxBytes > 0)
        {
            data[rxBytes] = 0;
            ESP_LOGI(TAG, "Read %d data.: %s", rxBytes, data);
        }
    }
    free(data);
}

void LD14_lnlt()
{

    LD14_uart_lnlt();

    xTaskCreate(rxTask, "rx", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    xTaskCreate(txTask, "tx", 1024 * 2, NULL, configMAX_PRIORITIES - 2, NULL);
}
