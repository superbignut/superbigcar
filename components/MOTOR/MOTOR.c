/**
 * @file MOTOR.c
 * @author bignut
 * @brief 
 * @details
 *      -> clock -> timer -> channel -> ledc_pwm
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "MOTOR.h"
#include "driver/ledc.h"

#define LEDC_TIMER              LEDC_TIMER_0    
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (5)
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT
#define LEDC_DUTY               (4096)
#define LEDC_FREQUENCY          (4000)




void ledc_init()
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode         = LEDC_MODE,                    // speed
        .duty_resolution    = LEDC_DUTY_RES,                // count resolution
        .timer_num          = LEDC_TIMER,                   // timer number
        .freq_hz            = LEDC_FREQUENCY,               // timer output freq
        .clk_cfg            = LEDC_AUTO_CLK,                // auto check clock input
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel = {
        .speed_mode         = LEDC_MODE,
        .channel            = LEDC_CHANNEL,                 // ledc channel
        .timer_sel          = LEDC_TIMER,
        .intr_type          = LEDC_INTR_DISABLE,            // no interrupy
        .gpio_num           = LEDC_OUTPUT_IO,               // output io
        .duty               = 0,                            // duty = 0
        .hpoint             = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

}

void ledc_pwm(double duty_percent)
{   
    assert(duty_percent <= 1.0 && duty_percent >= 0.0);

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty_percent * 8192));   // 8192 = 2**13

    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}



