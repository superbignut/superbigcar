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
#include "MPU6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "sdkconfig.h"
#include "I2Cdev.h"
//#include "driver/ledc.h"

// static const char *TAG = "Example";

#define PIN_SDA GPIO_NUM_7
#define PIN_SCL GPIO_NUM_8


void mpu_tmp(){


    Quaternion q;   // w x y z

    VectorFloat gravity; // g vector

    float ypr[3];   // yaw, pitch, roll

    uint16_t DMP_package_size = 42;

    uint16_t fifo_count;
    
    uint8_t fifo_buffer[64];

    uint8_t mpuIntStatus;


    MPU6050 mpu = MPU6050();
    mpu.initialize();
    mpu.dmpInitialize();
}