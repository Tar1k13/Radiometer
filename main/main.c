// #include <stdio.h>
// #include <driver/gpio.h>
// #include "count_rad.h"
// #include "esp_timer.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "esp_event.h"
// #include "hal/gpio_types.h"
// #include "hal/ledc_types.h"
// #include "nvs_flash.h"
// #include "esp_log.h"
// #include "esp_nimble_hci.h"
// #include "nimble/nimble_port.h"
// #include "nimble/nimble_port_freertos.h"
// #include "host/ble_hs.h"
// #include "services/gap/ble_svc_gap.h"
// #include "services/gatt/ble_svc_gatt.h"
// #include "sdkconfig.h"
// #include "string.h"
// #include <driver/ledc.h>
// // #include "indication_module/indication.h"
// #include "driver/uart.h"
// #include "phy_init_data.h"
// #include "ble_module.h"
// #include "pinDefs.h"
// #include "indication.h"




// // char *TAG = "BLE-Server";
// // uint8_t ble_addr_type;

// gptimer_handle_t gptimer = NULL;

// QueueHandle_t uart_queue;

// void ble_app_advertise(void);
// void task_test(void *pvParameters);

// // // Write data to ESP32 defined as server
// // static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
// // {
// //     // printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);

// //     char * data = (char *)ctxt->om->om_data;
// //     printf("%d\n",strcmp(data, (char *)"LIGHT ON")==0);
// //     if (strcmp(data, (char *)"LIGHT ON\0")==0)
// //     {
// //        printf("LIGHT ON\n");
// //     }
// //     else if (strcmp(data, (char *)"LIGHT OFF\0")==0)
// //     {
// //         printf("LIGHT OFF\n");
// //     }
// //     else if (strcmp(data, (char *)"FAN ON\0")==0)
// //     {
// //         printf("FAN ON\n");
// //     }
// //     else if (strcmp(data, (char *)"FAN OFF\0")==0)
// //     {
// //         printf("FAN OFF\n");
// //     }
// //     else{
// //         printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
// //     }
    
    
// //     return 0;
// // }

// // // Read data from ESP32 defined as server
// // static int device_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
// // {
// //     ESP_LOGI("I","READ ISSUED");
// //     char str[50];
// //     snprintf(str,10,"%d", get_radiation());
// //     os_mbuf_append(ctxt->om,str , strlen(str));
// //     return 0;
// // }

// // // Array of pointers to other service definitions
// // // UUID - Universal Unique Identifier
// // static const struct ble_gatt_svc_def gatt_svcs[] = {
// //     {.type = BLE_GATT_SVC_TYPE_PRIMARY,
// //      .uuid = BLE_UUID16_DECLARE(0x180),                 // Define UUID for device type
// //      .characteristics = (struct ble_gatt_chr_def[]){
// //          {.uuid = BLE_UUID16_DECLARE(0xFEF4),           // Define UUID for reading
// //           .flags = BLE_GATT_CHR_F_READ,
// //           .access_cb = device_read},
// //          {.uuid = BLE_UUID16_DECLARE(0xDEAD),           // Define UUID for writing
// //           .flags = BLE_GATT_CHR_F_WRITE,
// //           .access_cb = device_write},
// //          {0}}},
// //     {0}};

// // // BLE event handling
// // static int ble_gap_event(struct ble_gap_event *event, void *arg)
// // {
// //     switch (event->type)
// //     {
// //     // Advertise if connected
// //     case BLE_GAP_EVENT_CONNECT:
// //         ESP_LOGI("GAP", "BLE GAP EVENT CONNECT %s", event->connect.status == 0 ? "OK!" : "FAILED!");
// //         if (event->connect.status != 0)
// //         {
// //             ble_app_advertise();
            
// //         }
// //         break;
// //     // Advertise again after completion of the event
// //     case BLE_GAP_EVENT_DISCONNECT:
// //         ESP_LOGI("GAP", "BLE GAP EVENT DISCONNECTED");
// //         ble_app_advertise();
// //         break;
// //     case BLE_GAP_EVENT_ADV_COMPLETE:
// //         ESP_LOGI("GAP", "BLE GAP EVENT");
// //         ble_app_advertise();
// //         break;
// //     default:
// //         break;
// //     }
// //     return 0;
// // }

// // // Define the BLE connection
// // void ble_app_advertise(void)
// // {
// //     // GAP - device name definition
// //     struct ble_hs_adv_fields fields;
// //     const char *device_name;
// //     memset(&fields, 0, sizeof(fields));
// //     device_name = ble_svc_gap_device_name(); // Read the BLE device name
// //     fields.name = (uint8_t *)device_name;
// //     fields.name_len = strlen(device_name);
// //     fields.name_is_complete = 1;
// //     ble_gap_adv_set_fields(&fields);

// //     // GAP - device connectivity definition
// //     struct ble_gap_adv_params adv_params;
// //     memset(&adv_params, 0, sizeof(adv_params));
// //     adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // connectable or non-connectable
// //     adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // discoverable or non-discoverable
// //     ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
// // }

// // // The application
// // void ble_app_on_sync(void)
// // {
// //     ble_hs_id_infer_auto(0, &ble_addr_type); // Determines the best address type automatically
// //     ble_app_advertise();                     // Define the BLE connection
// // }

// // // The infinite task
// // void host_task(void *param)
// // {
// //     nimble_port_run(); // This function will return only when nimble_port_stop() is executed
// // }


// void app_main()
// {
//     // //PWM TUBE
//     ledc_timer_config_t ledc_timer = {
//         .speed_mode       = LEDC_LOW_SPEED_MODE,
//         .duty_resolution  = LEDC_TIMER_8_BIT,
//         .freq_hz          = 1000,
//         .clk_cfg          = LEDC_AUTO_CLK,
//         .timer_num        = LEDC_TIMER_0
//     };
//     ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
//     ledc_channel_config_t ledc_channel = {
//         .speed_mode     = LEDC_LOW_SPEED_MODE,
//         .channel        = LEDC_CHANNEL_0,
//         .timer_sel      = LEDC_TIMER_0,
//         .intr_type      = LEDC_INTR_DISABLE,
//         .gpio_num       = PIPE_PWM,
//         .duty           = 0,
//         .hpoint         = 0
//     };
//     ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
//     ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0,255/2);
//     ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_0);

//     // //PWM LAMP
//     ledc_timer_config_t ledc_timer_lamp = {
//         .speed_mode       = LEDC_LOW_SPEED_MODE,
//         .duty_resolution  = LEDC_TIMER_10_BIT,
//         .freq_hz          = 33000,
//         .clk_cfg          = LEDC_AUTO_CLK,
//         .timer_num        = LEDC_TIMER_1
//     };
//     ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_lamp));
//     ledc_channel_config_t ledc_channel_lamp = {
//         .speed_mode     = LEDC_LOW_SPEED_MODE,
//         .channel        = LEDC_CHANNEL_1,
//         .timer_sel      = LEDC_TIMER_1,
//         .intr_type      = LEDC_INTR_DISABLE,
//         .gpio_num       = 4,
//         .duty           = (1<<10)*0.3,
//         .hpoint         = 0
//     };
//     ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_lamp));



//     // //Sound PWM
//     // ledc_channel_config_t ledc_channel_sound = {
//     //     .speed_mode     = LEDC_LOW_SPEED_MODE,
//     //     .channel        = LEDC_CHANNEL_1,
//     //     .timer_sel      = LEDC_TIMER_0,
//     //     .intr_type      = LEDC_INTR_DISABLE,
//     //     .gpio_num       = SOUND_PWM,
//     //     .duty           = 0,
//     //     .hpoint         = 0
//     // };
//     // ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_sound));
//     // indication_spi_init();
//     // indication_dynamic_init();
    
    
//     BLE_Init();
//     // // test_send();
//     // count_rad_init();
//     // // vTaskDelay(1000/portTICK_PERIOD_MS);
//     // ESP_ERROR_CHECK(nvs_flash_init());                      // 1 - Initialize NVS flash using
//     // esp_phy_erase_cal_data_in_nvs();
//     // esp_nimble_hci_init();      // 2 - Initialize ESP controller
//     // nimble_port_init();                        // 3 - Initialize the host stack
//     // ble_svc_gap_device_name_set("BLE-Server"); // 4 - Initialize NimBLE configuration - server name
//     // ble_svc_gap_init();                        // 4 - Initialize NimBLE configuration - gap service
//     // ble_svc_gatt_init();                       // 4 - Initialize NimBLE configuration - gatt service
//     // ble_gatts_count_cfg(gatt_svcs);            // 4 - Initialize NimBLE configuration - config gatt services
//     // ble_gatts_add_svcs(gatt_svcs);             // 4 - Initialize NimBLE configuration - queues gatt services.
//     // ble_hs_cfg.sync_cb = ble_app_on_sync;      // 5 - Initialize application
//     // nimble_port_freertos_init(host_task);      // 6 - Run the thread
// }


#include "indication.h"

void app_main()
{
	indication_register_component();
}