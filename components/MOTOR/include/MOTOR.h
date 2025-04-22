/**
 * @file MOTROR.h
 * @author bignut
 * @brief 
 * @details
 *      esp32-wroom32 
 *          GPIO21  MOTOR_AIN1
 *          GPIO22  MOTOR_AIN2
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __LTL_MOTOR__
#define __LTL_MOTOR__

#include "stdio.h"

#define ltl_left_motor_pwm_out1 (21)
#define ltl_left_motor_pwm_out2 (22)

#define ltl_right_motor_pwm_out1 (19)
#define ltl_right_motor_pwm_out2 (28)

#define PWM 

void ledc_init();

void ledc_pwm(double duty_percent);

#endif