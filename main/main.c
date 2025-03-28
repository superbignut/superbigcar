/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "LED.h"

static const char *TAG = "Example";


void app_main(void)
{   
    
    while(1)
    {
        ESP_LOGI(TAG, "this is a test num: %d", ltl_add(1, 2));
        vTaskDelay(10);
    }
    
}
