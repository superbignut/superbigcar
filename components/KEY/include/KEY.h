/**
 * @file KEY.h
 * @author bignut
 * @brief 
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#ifndef __LTL_KEY__
#define __LTL_KEY__


#include "driver/gpio.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"



#define BOOT_GPIO_PIN GPIO_NUM_0    /**> BOOT BOTTOM in esp32-wroom32 */

#define GET_BOOT_STATE  gpio_get_level(BOOT_GPIO_PIN)   /** Get boot-bottom state */

enum BOOT_BOTTOM_STATE
{
    BOOT_RELEASE = 0,
    BOOT_PRESS = 1,
};

void key_init();

void key_scan(uint8_t mode);

#endif