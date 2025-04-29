/**
 * @file LD14.c
 * @author bignut
 * @brief 
 * @details
 * @version 0.1
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "LD14.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "string.h"

#define TXD_GPIO    (GPIO_NUM_25)       // Not-use.
#define RXD_GPIO    (GPIO_NUM_26)       // Receive-GPIO.
#define POINT_PER_PACK      12          // Point num specified in mannual.
#define PACK_SIZE           47          // 3 * 12 + 11 : Sizeof(LidarFrameDef).
#define HEADER              0x54        // Fixed according to ld14 mannual.
#define VERLEN              0x2c        // same as before.


static const int RX_BUF_SIZE = 1024;    // Uart reveive buffer.


/// @brief Crc table.
static const uint8_t CrcTable[256] = {
    0x00, 0x4d, 0x9a, 0xd7, 0x79, 0x34, 0xe3, 0xae, 0xf2, 0xbf, 0x68, 0x25,
    0x8b, 0xc6, 0x11, 0x5c, 0xa9, 0xe4, 0x33, 0x7e, 0xd0, 0x9d, 0x4a, 0x07,
    0x5b, 0x16, 0xc1, 0x8c, 0x22, 0x6f, 0xb8, 0xf5, 0x1f, 0x52, 0x85, 0xc8,
    0x66, 0x2b, 0xfc, 0xb1, 0xed, 0xa0, 0x77, 0x3a, 0x94, 0xd9, 0x0e, 0x43,
    0xb6, 0xfb, 0x2c, 0x61, 0xcf, 0x82, 0x55, 0x18, 0x44, 0x09, 0xde, 0x93,
    0x3d, 0x70, 0xa7, 0xea, 0x3e, 0x73, 0xa4, 0xe9, 0x47, 0x0a, 0xdd, 0x90,
    0xcc, 0x81, 0x56, 0x1b, 0xb5, 0xf8, 0x2f, 0x62, 0x97, 0xda, 0x0d, 0x40,
    0xee, 0xa3, 0x74, 0x39, 0x65, 0x28, 0xff, 0xb2, 0x1c, 0x51, 0x86, 0xcb,
    0x21, 0x6c, 0xbb, 0xf6, 0x58, 0x15, 0xc2, 0x8f, 0xd3, 0x9e, 0x49, 0x04,
    0xaa, 0xe7, 0x30, 0x7d, 0x88, 0xc5, 0x12, 0x5f, 0xf1, 0xbc, 0x6b, 0x26,
    0x7a, 0x37, 0xe0, 0xad, 0x03, 0x4e, 0x99, 0xd4, 0x7c, 0x31, 0xe6, 0xab,
    0x05, 0x48, 0x9f, 0xd2, 0x8e, 0xc3, 0x14, 0x59, 0xf7, 0xba, 0x6d, 0x20,
    0xd5, 0x98, 0x4f, 0x02, 0xac, 0xe1, 0x36, 0x7b, 0x27, 0x6a, 0xbd, 0xf0,
    0x5e, 0x13, 0xc4, 0x89, 0x63, 0x2e, 0xf9, 0xb4, 0x1a, 0x57, 0x80, 0xcd,
    0x91, 0xdc, 0x0b, 0x46, 0xe8, 0xa5, 0x72, 0x3f, 0xca, 0x87, 0x50, 0x1d,
    0xb3, 0xfe, 0x29, 0x64, 0x38, 0x75, 0xa2, 0xef, 0x41, 0x0c, 0xdb, 0x96,
    0x42, 0x0f, 0xd8, 0x95, 0x3b, 0x76, 0xa1, 0xec, 0xb0, 0xfd, 0x2a, 0x67,
    0xc9, 0x84, 0x53, 0x1e, 0xeb, 0xa6, 0x71, 0x3c, 0x92, 0xdf, 0x08, 0x45,
    0x19, 0x54, 0x83, 0xce, 0x60, 0x2d, 0xfa, 0xb7, 0x5d, 0x10, 0xc7, 0x8a,
    0x24, 0x69, 0xbe, 0xf3, 0xaf, 0xe2, 0x35, 0x78, 0xd6, 0x9b, 0x4c, 0x01,
    0xf4, 0xb9, 0x6e, 0x23, 0x8d, 0xc0, 0x17, 0x5a, 0x06, 0x4b, 0x9c, 0xd1,
    0x7f, 0x32, 0xe5, 0xa8
  };


/// @brief Sample Point info include:1 distance and 2 light-intensity , light-intensity represent signal's intensity.
typedef struct 
{
    uint16_t distance;
    uint8_t intensity;
} __attribute__((packed)) LidarPointDef;



/// @brief Data Frame from uarf-ld14 has follow struct, inside, point means some sample point's info.
typedef struct 
{
    uint8_t header;                             // 0x54  fixed
    uint8_t verlen;                             // 0x2c  fixed
    uint16_t speed;                             // degree / second
    uint16_t start_angle;                       // 0.01 / per
    LidarPointDef point[POINT_PER_PACK];        // 
    uint16_t end_angle;                         
    uint16_t timestamp;                         // ms max 30000
    uint8_t crc8;
}__attribute__((packed)) LidarFrameDef;         // 11 + 3 * 12 = 47 Bytes



/// @brief Compute crc.
/// @param p 
/// @param len 
/// @return 
static uint8_t CalCRC8(uint8_t *p, uint8_t len)
{
    uint8_t crc = 0;
    uint16_t i;
    for(i=0; i< len; ++i)
    {
        crc = CrcTable[(crc^*p++) & 0xff];  // operator precedence is p++, *, ^
    }
    return crc;
}

/// @brief Ld14 Uart init function.
static void LD14_uart_lnlt()
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_GPIO, RXD_GPIO, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

/// @brief Test LD14 data.
static void LD14_test_crc()
{
    uint8_t data[PACK_SIZE] = {0};

    while(1)
    {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, PACK_SIZE, 0);
        
        
        if(rxBytes == PACK_SIZE)
        {
            if(CalCRC8(data, PACK_SIZE-1) == data[PACK_SIZE-1])
            {
                for(int j = 0; j< PACK_SIZE; ++j)
                {
                    printf("%02x ", data[j]);
                }
                ESP_LOGI("LD14", "True: %d", 1);
            }
            else
            {
                uart_flush(UART_NUM_1);     // crucial which is used to data alignment.
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);   
    }
}


void LD14_lnlt()
{

    LD14_uart_lnlt();

    // xTaskCreate(rxTask, "rx", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);

    // vTaskDelay(1000 / portTICK_PERIOD_MS);

    // xTaskCreate(txTask, "tx", 1024 * 2, NULL, configMAX_PRIORITIES - 2, NULL);
    
    xTaskCreate(LD14_test_crc, "LD14-test", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
    
    
    // ESP_LOGI("111: ","%d", sizeof(LidarPointDef));
}


/*
    Below is Uart's Hello-World Test code.
*/
/* int sendData(const char *Name, const char *data)
{
    const int len = strlen(data);

    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);

    ESP_LOGI(Name, "uart has trans %d : %s", txBytes, data);

    return txBytes;
}


static void txTask()
{
    static const char *TAG = "TX_TASK";
    while(1)
    {
        sendData(TAG, "Hello-World...");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

static void rxTask(){
    static const char *TAG = "RX_TASK";

    uint8_t *data = (uint8_t *)malloc(RX_BUF_SIZE + 1);
    while(1)
    {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
        if(rxBytes > 0)
        {
            data[rxBytes] = 0;
            ESP_LOGI(TAG, "Read %d data.: %s", rxBytes, data);
        }
    }
    free(data);
} */
