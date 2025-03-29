/**
 * @file KEY.c
 * @author bignut
 * @brief 
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "KEY.h"

/**
 * @brief BOOT_BOTTOM_KEY_INIT
 * 
 */
void key_init()
{
    
    gpio_config_t gpio_init_structure = {0};

    gpio_init_structure.intr_type = GPIO_INTR_DISABLE;
    gpio_init_structure.mode = GPIO_MODE_INPUT;             /**< INPUT */
    gpio_init_structure.pull_up_en = GPIO_PULLUP_ENABLE;    /**< PULL_UP */
    gpio_init_structure.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_init_structure.pin_bit_mask = 1ull << BOOT_GPIO_PIN;

    gpio_config(&gpio_init_structure);
}


/**
 * @brief Bottom state scan function.
 * 
 * @param mode 
 */
void key_scan(uint8_t mode)
{
    uint8_t key_val = 0;
    static uint8_t key_state = BOOT_RELEASE;

    if(mode)
    {
        key_state = BOOT_PRESS;
    }
}


