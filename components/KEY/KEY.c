#include "KEY.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void key_init_ltl(void){
    gpio_config_t gpio_init_structure = {0};

    gpio_init_structure.intr_type = GPIO_INTR_DISABLE;
    gpio_init_structure.mode = GPIO_MODE_INPUT;             /**< INPUT */
    gpio_init_structure.pull_up_en = GPIO_PULLUP_ENABLE;    /**< PULL_UP */
    gpio_init_structure.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_init_structure.pin_bit_mask = 1ull << BOOT_GPIO_PIN;

    gpio_config(&gpio_init_structure);
}

uint8_t key_scan_ltl(void){
    uint8_t key_state = BUTTON_RELEASE;   

    static bool button_is_release = true;
    
    if(button_is_release && (GET_BOOT_STATE == BUTTON_PRESS)) /**> first press */
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);

        if(GET_BOOT_STATE == BUTTON_PRESS)
        {
            button_is_release = false;
            key_state = BUTTON_PRESS;
        }
    }
    else if(GET_BOOT_STATE == BUTTON_RELEASE)   /**> first release */
    {
        button_is_release = true;
    }
    else    /**> still pressing. */
    {
        ;
    }

    return key_state;
}