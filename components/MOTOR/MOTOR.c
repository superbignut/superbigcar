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

// left wheel
#define DRV8833_AIN1_GPIO       GPIO_NUM_22
#define DRV8833_AIN2_GPIO       GPIO_NUM_23

// right wheel
#define DRV8833_BIN1_GPIO       GPIO_NUM_19
#define DRV8833_BIN2_GPIO       GPIO_NUM_21

// universial pwm config
#define PWM_MODE                   LEDC_LOW_SPEED_MODE
#define PWM_DUTY_RES               LEDC_TIMER_13_BIT
#define PWM_FREQUENCY              (4000)


#define LEFT_PWM0_TIMER                 LEDC_TIMER_0    
#define LEFT_PWM0_OUTPUT_IO             DRV8833_AIN1_GPIO
#define LEFT_PWM0_CHANNEL               LEDC_CHANNEL_0


#define LEFT_PWM1_TIMER                 LEDC_TIMER_1 
#define LEFT_PWM1_OUTPUT_IO             DRV8833_AIN2_GPIO
#define LEFT_PWM1_CHANNEL               LEDC_CHANNEL_1


#define RIGHT_PWM2_TIMER                 LEDC_TIMER_2  
#define RIGHT_PWM2_OUTPUT_IO             DRV8833_BIN1_GPIO
#define RIGHT_PWM2_CHANNEL               LEDC_CHANNEL_2


#define RIGHT_PWM3_TIMER                 LEDC_TIMER_3  
#define RIGHT_PWM3_OUTPUT_IO             DRV8833_BIN2_GPIO
#define RIGHT_PWM3_CHANNEL               LEDC_CHANNEL_3





/// @brief Init four pwm ouput for drv8833.
void pwm_init()
{
    // pwm0
    ledc_timer_config_t ledc_timer0 = {
        .speed_mode         = PWM_MODE,                    // speed
        .duty_resolution    = PWM_DUTY_RES,               // count resolution
        .timer_num          = LEFT_PWM0_TIMER,             // timer number
        .freq_hz            = PWM_FREQUENCY,               // timer output freq
        .clk_cfg            = LEDC_AUTO_CLK,               // auto check clock input
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer0));

    ledc_channel_config_t ledc_channel0 = {
        .speed_mode         = PWM_MODE,
        .channel            = LEFT_PWM0_CHANNEL,            // ledc channel
        .timer_sel          = LEFT_PWM0_TIMER,
        .intr_type          = LEDC_INTR_DISABLE,            // no interrupy
        .gpio_num           = LEFT_PWM0_OUTPUT_IO,          // output io
        .duty               = 0,                            // duty = 0
        .hpoint             = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel0));


    // pwm1
    ledc_timer_config_t ledc_timer1 = {
        .speed_mode         = PWM_MODE,                    // speed
        .duty_resolution    = PWM_DUTY_RES,               // count resolution
        .timer_num          = LEFT_PWM1_TIMER,             // timer number
        .freq_hz            = PWM_FREQUENCY,               // timer output freq
        .clk_cfg            = LEDC_AUTO_CLK,               // auto check clock input
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer1));

    ledc_channel_config_t ledc_channel1 = {
        .speed_mode         = PWM_MODE,
        .channel            = LEFT_PWM1_CHANNEL,            // ledc channel
        .timer_sel          = LEFT_PWM1_TIMER,
        .intr_type          = LEDC_INTR_DISABLE,            // no interrupy
        .gpio_num           = LEFT_PWM1_OUTPUT_IO,          // output io
        .duty               = 0,                            // duty = 0
        .hpoint             = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel1));

    // pwm2
    ledc_timer_config_t ledc_timer2 = {
        .speed_mode         = PWM_MODE,                    // speed
        .duty_resolution    = PWM_DUTY_RES,               // count resolution
        .timer_num          = RIGHT_PWM2_TIMER,             // timer number
        .freq_hz            = PWM_FREQUENCY,               // timer output freq
        .clk_cfg            = LEDC_AUTO_CLK,               // auto check clock input
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer2));

    ledc_channel_config_t ledc_channel2 = {
        .speed_mode         = PWM_MODE,
        .channel            = RIGHT_PWM2_CHANNEL,            // ledc channel
        .timer_sel          = RIGHT_PWM2_TIMER,
        .intr_type          = LEDC_INTR_DISABLE,            // no interrupy
        .gpio_num           = RIGHT_PWM2_OUTPUT_IO,          // output io
        .duty               = 0,                            // duty = 0
        .hpoint             = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel2));
    
    
    // pwm3
    ledc_timer_config_t ledc_timer3 = {
        .speed_mode         = PWM_MODE,                    // speed
        .duty_resolution    = PWM_DUTY_RES,               // count resolution
        .timer_num          = RIGHT_PWM3_TIMER,             // timer number
        .freq_hz            = PWM_FREQUENCY,               // timer output freq
        .clk_cfg            = LEDC_AUTO_CLK,               // auto check clock input
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer3));

    ledc_channel_config_t ledc_channel3 = {
        .speed_mode         = PWM_MODE,
        .channel            = RIGHT_PWM3_CHANNEL,            // ledc channel
        .timer_sel          = RIGHT_PWM3_TIMER,
        .intr_type          = LEDC_INTR_DISABLE,            // no interrupy
        .gpio_num           = RIGHT_PWM3_OUTPUT_IO,          // output io
        .duty               = 0,                            // duty = 0
        .hpoint             = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel3));

}
/// @brief Set pwm0 duty.
/// @param percent 
static void set_pwm_0(double percent)
{
    assert(percent <= 1.0 && percent >= 0.0);

    ESP_ERROR_CHECK(ledc_set_duty(PWM_MODE, LEFT_PWM0_CHANNEL, percent * 8192));   // 8192 = 2**13

    ESP_ERROR_CHECK(ledc_update_duty(PWM_MODE, LEFT_PWM0_CHANNEL));
}

/// @brief Set pwm1 duty.
/// @param percent 
static void set_pwm_1(double percent)
{
    assert(percent <= 1.0 && percent >= 0.0);

    ESP_ERROR_CHECK(ledc_set_duty(PWM_MODE, LEFT_PWM1_CHANNEL, percent * 8192));   // 8192 = 2**13

    ESP_ERROR_CHECK(ledc_update_duty(PWM_MODE, LEFT_PWM1_CHANNEL));
}

/// @brief Set pwm2 duty.
/// @param percent 
static void set_pwm_2(double percent)
{
    assert(percent <= 1.0 && percent >= 0.0);

    ESP_ERROR_CHECK(ledc_set_duty(PWM_MODE, RIGHT_PWM2_CHANNEL, percent * 8192));   // 8192 = 2**13

    ESP_ERROR_CHECK(ledc_update_duty(PWM_MODE, RIGHT_PWM2_CHANNEL));
}

/// @brief Set pwm3 duty.
/// @param percent 
static void set_pwm_3(double percent)
{
    assert(percent <= 1.0 && percent >= 0.0);

    ESP_ERROR_CHECK(ledc_set_duty(PWM_MODE, RIGHT_PWM3_CHANNEL, percent * 8192));   // 8192 = 2**13

    ESP_ERROR_CHECK(ledc_update_duty(PWM_MODE, RIGHT_PWM3_CHANNEL));
}

static void move_forward()
{   
    double duty = 0.5;
    set_pwm_0(duty);
    set_pwm_1(0);
    set_pwm_3(duty); // 0132 is because two motors has opposite orientation.
    set_pwm_2(0);
}

static void move_back()
{
    double duty = 0.5;
    set_pwm_0(0);
    set_pwm_1(duty);
    set_pwm_3(0);
    set_pwm_2(duty);
}

static void left_control()
{

}

static void right_control()
{

}

/// @brief Use speed and angle control car's movement.
/// @param speed_mm_s 
/// @param angle_arc 
void motor_control(int speed_mm_s, double angle_arc)
{
    if(speed_mm_s > 0)
    {
        move_forward();
    }
    else
    {
        move_back();
    }
}

