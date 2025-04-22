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
#include "nvs_flash.h"
#include "LED.h"
#include "KEY.h"
#include "MOTOR.h"
//#include "driver/ledc.h"

// static const char *TAG = "Example";


void app_main(void)
{   

    esp_err_t ret;

    ret = nvs_flash_init(); 
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    led_init_ltl();
    // key_init_ltl();
    ledc_init();
    int i = 0;
    
    while(1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        
        ledc_pwm(i * 1.0 / 100);
        
        i = (i+1) % 100;
    }
    
}
