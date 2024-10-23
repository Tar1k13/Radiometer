#pragma once

#include <stdio.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

//Shift reg pins
#define LATCH GPIO_NUM_10
#define SCK GPIO_NUM_12
#define S_DATA GPIO_NUM_11
#define OUT_EN GPIO_NUM_3

//Dynamic ind pins
#define IN_PWR_CTRL1 40
#define IN_PWR_CTRL2 41
#define IN_PWR_CTRL3 GPIO_NUM_39
#define IN_PWR_CTRL4 GPIO_NUM_16

#define SPI_NUM SPI3_HOST

esp_err_t indication_spi_init();
void indication_set_num(uint8_t num);
void set_num_to_disp(int num,bool uZv);