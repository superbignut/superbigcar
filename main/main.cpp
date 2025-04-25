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
#include "driver/i2c.h"
#include "esp_err.h"
// #include "MPU6050.h"
// p#include "MPU6050_6Axis_MotionApps20.h"
#include "sdkconfig.h"
#include "I2Cdev.h"
//#include "driver/ledc.h"

// static const char *TAG = "Example";

extern void mpu_tmp();


extern "C" void app_main(void)
{   

    esp_err_t ret;

    ret = nvs_flash_init(); 
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    // I2Cdev tmp;

    mpu_tmp();
    
    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    } 
    
}
