#include "ble_module.h"

char *TAG = "BLE-Server";
uint8_t ble_addr_type;

static void ble_app_advertise(void);

// Write data to ESP32 defined as server
static int device_write(uint16_t conn_handle, uint16_t attr_handle,
                        struct ble_gatt_access_ctxt *ctxt, void *arg)
{
  char *data = (char *)ctxt->om->om_data;
  printf("%d\n", strcmp(data, (char *)"LIGHT ON") == 0);
  if (strcmp(data, (char *)"LIGHT ON\0") == 0)
  {
    printf("LIGHT ON\n");
  }
  else if (strcmp(data, (char *)"LIGHT OFF\0") == 0)
  {
    printf("LIGHT OFF\n");
  }
  else if (strcmp(data, (char *)"FAN ON\0") == 0)
  {
    printf("FAN ON\n");
  }
  else if (strcmp(data, (char *)"FAN OFF\0") == 0)
  {
    printf("FAN OFF\n");
  }
  else
  {
    printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
  }

  return 0;
}

// Read data from ESP32 defined as server
static int device_read(uint16_t con_handle, uint16_t attr_handle,
                       struct ble_gatt_access_ctxt *ctxt, void *arg)
{
  ESP_LOGI("I", "READ ISSUED");
  char str[50];
  snprintf(str, 10, "%d", get_radiation());
  os_mbuf_append(ctxt->om, str, strlen(str));
  return 0;
}

// Array of pointers to other service definitions
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(0x180), // Define UUID for device type
     .characteristics =
         (struct ble_gatt_chr_def[]){
             {.uuid = BLE_UUID16_DECLARE(0xFEF4), // Define UUID for reading
              .flags = BLE_GATT_CHR_F_READ,
              .access_cb = device_read},
             {.uuid = BLE_UUID16_DECLARE(0xDEAD), // Define UUID for writing
              .flags = BLE_GATT_CHR_F_WRITE,
              .access_cb = device_write},
             {0}}},
    {0}};

// BLE event handling
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
  switch (event->type)
  {
  // Advertise if connected
  case BLE_GAP_EVENT_CONNECT:
    ESP_LOGI("GAP", "BLE GAP EVENT CONNECT %s",
             event->connect.status == 0 ? "OK!" : "FAILED!");
    if (event->connect.status != 0)
    {
      ble_app_advertise();
    }
    break;
  // Advertise again after completion of the event
  case BLE_GAP_EVENT_DISCONNECT:
    ESP_LOGI("GAP", "BLE GAP EVENT DISCONNECTED");
    ble_app_advertise();
    break;
  case BLE_GAP_EVENT_ADV_COMPLETE:
    ESP_LOGI("GAP", "BLE GAP EVENT");
    ble_app_advertise();
    break;
  default:
    break;
  }
  return 0;
}

// Define the BLE connection
static void ble_app_advertise(void)
{
  // GAP - device name definition
  struct ble_hs_adv_fields fields;
  const char *device_name;
  memset(&fields, 0, sizeof(fields));
  device_name = ble_svc_gap_device_name(); // Read the BLE device name
  fields.name = (uint8_t *)device_name;
  fields.name_len = strlen(device_name);
  fields.name_is_complete = 1;
  ble_gap_adv_set_fields(&fields);

  // GAP - device connectivity definition
  struct ble_gap_adv_params adv_params;
  memset(&adv_params, 0, sizeof(adv_params));
  adv_params.conn_mode =
      BLE_GAP_CONN_MODE_UND; // connectable or non-connectable
  adv_params.disc_mode =
      BLE_GAP_DISC_MODE_GEN; // discoverable or non-discoverable
  ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params,
                    ble_gap_event, NULL);
}

// The application
static void ble_app_on_sync(void)
{
  ble_hs_id_infer_auto(
      0, &ble_addr_type); // Determines the best address type automatically
  ble_app_advertise();    // Define the BLE connection
}

// The infinite task
static void host_task(void *param)
{
  nimble_port_run(); // This function will return only when nimble_port_stop()
                     // is executed
}

void BLE_Init()
{
  ESP_ERROR_CHECK(nvs_flash_init()); // Initialize NVS flash using
  esp_phy_erase_cal_data_in_nvs();
  esp_nimble_hci_init();                     // Initialize ESP controller
  nimble_port_init();                        // Initialize the host stack
  ble_svc_gap_device_name_set("BLE-Server"); // Initialize NimBLE configuration - server name
  ble_svc_gap_init();                        // Initialize NimBLE configuration - gap service
  ble_svc_gatt_init();                       // 4 - Initialize NimBLE configuration - gatt service
  ble_gatts_count_cfg(gatt_svcs);            // 4 - Initialize NimBLE configuration - config gatt services
  ble_gatts_add_svcs(gatt_svcs);             // Initialize NimBLE configuration - queues gatt services.
  ble_hs_cfg.sync_cb = ble_app_on_sync;      // Initialize application
  nimble_port_freertos_init(host_task);      // Run the thread
}