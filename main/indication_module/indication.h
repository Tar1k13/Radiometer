// #pragma once

// #include <stdio.h>
// #include "esp_err.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/spi_common.h"
// #include "driver/spi_master.h"
// #include "driver/gpio.h"
// #include <driver/ledc.h>
// #include <assert.h>

// //Shift reg pins
// #define LATCH GPIO_NUM_23
// #define SCK GPIO_NUM_14
// #define S_DATA GPIO_NUM_13
// #define OUT_EN GPIO_NUM_22

// //Dynamic ind pins
// // #define IN_PWR_CTRL1 40
// // #define IN_PWR_CTRL2 41
// // #define IN_PWR_CTRL3 GPIO_NUM_39
// // #define IN_PWR_CTRL4 GPIO_NUM_16
// #define OUT1_CHANNEL LEDC_CHANNEL_2
// #define OUT2_CHANNEL LEDC_CHANNEL_3
// #define OUT3_CHANNEL LEDC_CHANNEL_4
// #define OUT4_CHANNEL LEDC_CHANNEL_5

// #define GPIO_OUT1 GPIO_NUM_12
// #define GPIO_OUT2 GPIO_NUM_14
// #define GPIO_OUT3 GPIO_NUM_13
// #define GPIO_OUT4 GPIO_NUM_15

// #define SPI_NUM HSPI_HOST

// esp_err_t indication_spi_init();
// esp_err_t indication_dynamic_init();
// void indication_set_num(uint8_t num);
// void set_num_to_disp(int num,uint8_t numPos);
// void test_send();