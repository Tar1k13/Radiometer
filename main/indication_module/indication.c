// #include "indication.h"
// #include "count_rad.h"
// #include "driver/gpio.h"
// #include "esp_log.h"
// #include "freertos/idf_additions.h"
// #include "freertos/projdefs.h"
// #include <stdint.h>



// static void indication_int(void)
// {
//   // Dynamic indication

//   // General timer init
//   ledc_timer_config_t ledc_timer_indication = {
//       .speed_mode = LEDC_LOW_SPEED_MODE,
//       .duty_resolution = LEDC_TIMER_13_BIT,
//       .freq_hz = 50,
//       .clk_cfg = LEDC_AUTO_CLK,
//       .timer_num = LEDC_TIMER_2};
//   ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_indication));


//   // LINE 1
//   ledc_channel_config_t ledc_channel_ind_one = {
//       .speed_mode = LEDC_LOW_SPEED_MODE,
//       .channel = OUT1_CHANNEL,
//       .timer_sel = LEDC_TIMER_2,
//       .intr_type = LEDC_INTR_DISABLE,
//       .gpio_num = GPIO_OUT1,
//       .duty = (1 << 13) * 0.25,
//       .hpoint = 0};
//   ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_one));
  

//   // LINE 2
//   ledc_channel_config_t ledc_channel_ind_two = {
//       .speed_mode = LEDC_LOW_SPEED_MODE,
//       .channel = OUT2_CHANNEL,
//       .timer_sel = LEDC_TIMER_2,
//       .intr_type = LEDC_INTR_DISABLE,
//       .gpio_num = GPIO_OUT2,
//       .duty = (1 << 13) * 0.25,
//       .hpoint = (1 << 13) * 0.25};
//   ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_two));

  
//   // LINE 3
//   ledc_channel_config_t ledc_channel_ind_three = {
//       .speed_mode = LEDC_LOW_SPEED_MODE,
//       .channel = OUT3_CHANNEL,
//       .timer_sel = LEDC_TIMER_2,
//       .intr_type = LEDC_INTR_DISABLE,
//       .gpio_num = GPIO_OUT3,
//       .duty = (1 << 13) * 0.25,
//       .hpoint = (1 << 13) * 0.50};
//   ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_three));


//   // LINE 4
//   ledc_channel_config_t ledc_channel_ind_four = {
//       .speed_mode = LEDC_LOW_SPEED_MODE,
//       .channel = OUT4_CHANNEL,
//       .timer_sel = LEDC_TIMER_2,
//       .intr_type = LEDC_INTR_DISABLE,
//       .gpio_num = GPIO_OUT4,
//       .duty = (1 << 13) * 0.25,
//       .hpoint = (1 << 13) * 0.75};
//   ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_four));


//   return ESP_OK;
// }
































// static spi_device_handle_t handle;

// // static uint8_t dymIndArr[4]={IN_PWR_CTRL1,IN_PWR_CTRL2,IN_PWR_CTRL3,IN_PWR_CTRL4};

// static void display_radiation_task(void *pvParameters);

// static int current_radiation=0;

// static uint8_t current_indicator=1;

// static TaskHandle_t display_rad=NULL;

// static gptimer_handle_t sw_timer;
// static gptimer_handle_t get_rad_timer;

// static int divider=1000;
// static int current_num=0;
// static bool test=false;

// //called every 5ms to change indicator
// static bool switch_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx){
//     test=true;
//     xTaskNotifyGiveIndexed(display_rad,0);
//     return true;
// }

// //called every 20ms

// int bum=0;
// static bool get_rad_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx){
//     divider=1000;
//     current_radiation=get_radiation();
//     current_indicator=1;
//     return true;
// }


// esp_err_t indication_dynamic_init(){
//         //timer switch nums
//     // xTaskCreate(display_radiation_task,"display_radiation_task",2048,NULL,1,&display_rad);
//     gptimer_config_t timer_conf={
//         .clk_src=GPTIMER_CLK_SRC_DEFAULT,
//         .resolution_hz=10000,
//         .direction=GPTIMER_COUNT_UP,
//     }; 
//     gptimer_new_timer(&timer_conf,&sw_timer);
//     gptimer_alarm_config_t alarm_conf={
//         .alarm_count=50,
//         .flags.auto_reload_on_alarm=true
//     };
//     gptimer_set_alarm_action(sw_timer,&alarm_conf);
//     gptimer_event_callbacks_t cbs = {
//         .on_alarm = switch_callback // register user callback
//     };
//     ESP_ERROR_CHECK(gptimer_register_event_callbacks(sw_timer, &cbs, NULL));
//     ESP_ERROR_CHECK(gptimer_enable(sw_timer));
//     gptimer_start(sw_timer);

    
//     //timer get radiation
//     gptimer_config_t timer_conf_rad={
//         .clk_src=GPTIMER_CLK_SRC_DEFAULT,
//         .resolution_hz=10000,
//         .direction=GPTIMER_COUNT_UP,
//     }; 
//     gptimer_new_timer(&timer_conf_rad,&get_rad_timer);
//     gptimer_alarm_config_t alarm_conf_rad={
//         .alarm_count=200,
//         .flags.auto_reload_on_alarm=true
//     };
//     gptimer_set_alarm_action(get_rad_timer,&alarm_conf_rad);
//     gptimer_event_callbacks_t cbs1 = {
//         .on_alarm = get_rad_callback // register user callback
//     };
//     ESP_ERROR_CHECK(gptimer_register_event_callbacks(get_rad_timer, &cbs1, NULL));
//     ESP_ERROR_CHECK(gptimer_enable(get_rad_timer));
//     gptimer_start(get_rad_timer);

//     while(!test);
//     //     //Dynamic indication
//     ledc_timer_config_t ledc_timer_indication = {
//         .speed_mode       = LEDC_LOW_SPEED_MODE,
//         .duty_resolution  = LEDC_TIMER_13_BIT,
//         .freq_hz          = 50,
//         .clk_cfg          = LEDC_AUTO_CLK,
//         .timer_num        = LEDC_TIMER_2
//     };
//     ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_indication));

//     ledc_channel_config_t ledc_channel_ind_one = {
//         .speed_mode     = LEDC_LOW_SPEED_MODE,
//         .channel        = OUT1_CHANNEL,
//         .timer_sel      = LEDC_TIMER_2,
//         .intr_type      = LEDC_INTR_DISABLE,
//         .gpio_num       = GPIO_OUT1,
//         .duty           = (1<<13)*0.25,
//         .hpoint         = 0
//     };
//     ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_one));
//     // ledc_set_duty(LEDC_LOW_SPEED_MODE,OUT1_CHANNEL,(1<<13)*0.25);
//     // ledc_update_duty(LEDC_LOW_SPEED_MODE,OUT1_CHANNEL);

//     ledc_channel_config_t ledc_channel_ind_two = {
//         .speed_mode     = LEDC_LOW_SPEED_MODE,
//         .channel        = OUT2_CHANNEL,
//         .timer_sel      = LEDC_TIMER_2,
//         .intr_type      = LEDC_INTR_DISABLE,
//         .gpio_num       = GPIO_OUT2,
//         .duty           = (1<<13)*0.25,
//         .hpoint         = (1<<13)*0.25
//     };
//     ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_two));
//     // ledc_set_duty(LEDC_LOW_SPEED_MODE,OUT2_CHANNEL,(1<<13)*0.25);
//     // ledc_update_duty(LEDC_LOW_SPEED_MODE,OUT2_CHANNEL);

//     ledc_channel_config_t ledc_channel_ind_three = {
//         .speed_mode     = LEDC_LOW_SPEED_MODE,
//         .channel        = OUT3_CHANNEL,
//         .timer_sel      = LEDC_TIMER_2,
//         .intr_type      = LEDC_INTR_DISABLE,
//         .gpio_num       = GPIO_OUT3,
//         .duty           = (1<<13)*0.25,
//         .hpoint         = (1<<13)*0.50
//     };
//     ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_three));
//     // ledc_set_duty(LEDC_LOW_SPEED_MODE,OUT3_CHANNEL,(1<<13)*0.25);
//     // ledc_update_duty(LEDC_LOW_SPEED_MODE,OUT3_CHANNEL);


//     ledc_channel_config_t ledc_channel_ind_four = {
//         .speed_mode     = LEDC_LOW_SPEED_MODE,
//         .channel        = OUT4_CHANNEL,
//         .timer_sel      = LEDC_TIMER_2,
//         .intr_type      = LEDC_INTR_DISABLE,
//         .gpio_num       = GPIO_OUT4,
//         .duty           = (1<<13)*0.25,
//         .hpoint         = (1<<13)*0.75
//     };
//     ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_four));
//     // ledc_set_duty(LEDC_LOW_SPEED_MODE,OUT4_CHANNEL,(1<<13)*0.25);
//     // ledc_update_duty(LEDC_LOW_SPEED_MODE,OUT4_CHANNEL);

//     // vTaskDelay(5/portTICK_PERIOD_MS); //bias


//     return ESP_OK;
// }


// esp_err_t indication_spi_init(){

//     gpio_set_direction(OUT_EN, GPIO_MODE_OUTPUT);
//     gpio_set_level(OUT_EN, 0);

//     gpio_set_direction(LATCH,GPIO_MODE_OUTPUT);

//     //Dynamic indication pins
//     // for(uint8_t i=0;i<4;i++){
//     //     gpio_set_direction(dymIndArr[i],GPIO_MODE_OUTPUT);
//     //     gpio_set_level(dymIndArr[i],0);
//     // }
    
    
//     // SPI bus configuration
//     spi_bus_config_t buscfg = {
//         .mosi_io_num = S_DATA,
//         .miso_io_num = -1,          
//         .sclk_io_num = SCK,
//         .quadwp_io_num = -1,       
//         .quadhd_io_num = -1,       
//         .max_transfer_sz = 32      
//     };
    
   
//     esp_err_t ret = spi_bus_initialize(SPI_NUM, &buscfg, SPI_DMA_CH_AUTO);
//     if (ret != ESP_OK) {
//         return ret;
//     }

    
//     spi_device_interface_config_t devcfg = {
//         .mode = 0,                              
//         .clock_speed_hz = 1000 * 1000,  
//         .queue_size = 1,                     
//     };

   
//     ret = spi_bus_add_device(SPI_NUM, &devcfg, &handle);
//     if (ret != ESP_OK) {
//         return ret;
//     }

//     xTaskCreate(display_radiation_task,"display_radiation_task",4048,NULL,1,&display_rad);
//     return ESP_OK;
// };

// void indication_set_num(uint8_t num){

//     uint16_t res=0xFFFF&(1<<num);
//     uint8_t tx_buf[2] = { (res>>8),(res&(0xFF)) };

//     // SPI transaction
//     spi_transaction_t transaction = {
//         .flags = 0,
//         .length = 16,                   
//         .tx_buffer = tx_buf,            
//         .rx_buffer = NULL              
//     };

//     spi_device_polling_transmit(handle, &transaction);
//     gpio_set_level(LATCH, 1);
//     // vTaskDelay(1000/portTICK_PERIOD_MS);
//     gpio_set_level(LATCH,0);

// }


// void set_num_to_disp(int num,uint8_t currentPos){
//     // if(!uZv){
//     //     for(uint8_t i=0;i<4;i++){
//     //         gpio_set_level(dymIndArr[i],1);
//     //         indication_set_num(num%10);
//     //         vTaskDelay(100/portTICK_PERIOD_MS);
//     //         num/=10;
//     //         gpio_set_level(dymIndArr[i],0);
//     //     }
//     // }

    
// }

// static void display_radiation_task(void *pvParameters){
//     while(1){
//         ulTaskNotifyTakeIndexed(0,pdTRUE,portMAX_DELAY);
//         int numdt = current_radiation/divider;
//         indication_set_num(numdt);
//         if(divider!=1)
//             divider/=10;
//     }
// }

// void test_send(){
// }