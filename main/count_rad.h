#pragma once

#include "driver/gptimer.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define TUBE_PIN GPIO_NUM_27
#define DEFAULT_MONITOR_PERIOD 5

void count_rad_init();
int get_radiation();