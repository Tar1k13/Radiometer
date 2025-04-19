#pragma once

#include <stdio.h>
#include <driver/gpio.h>
#include "count_rad.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "hal/gpio_types.h"
#include "hal/ledc_types.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "sdkconfig.h"
#include "string.h"
#include <driver/ledc.h>
// #include "indication.h"
#include "driver/uart.h"
#include "phy_init_data.h"


void BLE_Init();