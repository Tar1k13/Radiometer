#include "indication.h"
#include "count_rad.h"
#include "freertos/projdefs.h"
#include <stdint.h>

spi_device_handle_t handle;

// static uint8_t dymIndArr[4]={IN_PWR_CTRL1,IN_PWR_CTRL2,IN_PWR_CTRL3,IN_PWR_CTRL4};

static void display_radiation_task(void *pvParameters);

static int current_radiation=0;

static uint8_t current_indicator=1;

static TaskHandle_t display_rad=NULL;

static gptimer_handle_t sw_timer;
static gptimer_handle_t get_rad_timer;


//called every 5ms to change indicator
static bool switch_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx){
    vTaskNotifyGiveFromISR(display_rad,0);
    return true;
}

//called every 20ms
bool test=false;
static bool get_rad_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx){
    current_radiation=get_radiation();
    current_indicator=1;
    return true;
}


esp_err_t indication_dynamic_init(){
        //Dynamic indication
    ledc_timer_config_t ledc_timer_indication = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_8_BIT,
        .freq_hz          = 50,
        .clk_cfg          = LEDC_AUTO_CLK,
        .timer_num        = LEDC_TIMER_2
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_indication));

    ledc_channel_config_t ledc_channel_ind_one = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = OUT1_CHANNEL,
        .timer_sel      = LEDC_TIMER_2,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = OUT1_GPIO,
        .duty           = 0,
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_one));
    ledc_set_duty(LEDC_LOW_SPEED_MODE,OUT1_CHANNEL,255*0.25);
    ledc_update_duty(LEDC_LOW_SPEED_MODE,OUT1_CHANNEL);

    ledc_channel_config_t ledc_channel_ind_two = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = OUT2_CHANNEL,
        .timer_sel      = LEDC_TIMER_2,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = OUT2_GPIO,
        .duty           = 0,
        .hpoint         = 255*0.25
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_two));
    ledc_set_duty(LEDC_LOW_SPEED_MODE,OUT2_CHANNEL,255*0.25);
    ledc_update_duty(LEDC_LOW_SPEED_MODE,OUT2_CHANNEL);

    ledc_channel_config_t ledc_channel_ind_three = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = OUT3_CHANNEL,
        .timer_sel      = LEDC_TIMER_2,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = OUT3_GPIO,
        .duty           = 0,
        .hpoint         = 255*0.50
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_three));
    ledc_set_duty(LEDC_LOW_SPEED_MODE,OUT3_CHANNEL,255*0.25);
    ledc_update_duty(LEDC_LOW_SPEED_MODE,OUT3_CHANNEL);


    ledc_channel_config_t ledc_channel_ind_four = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = OUT4_CHANNEL,
        .timer_sel      = LEDC_TIMER_2,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = OUT4_GPIO,
        .duty           = 0,
        .hpoint         = 255*0.50
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_ind_four));
    ledc_set_duty(LEDC_LOW_SPEED_MODE,OUT4_CHANNEL,255*0.25);
    ledc_update_duty(LEDC_LOW_SPEED_MODE,OUT4_CHANNEL);


    //timer switch nums
    gptimer_config_t timer_conf={
        .clk_src=GPTIMER_CLK_SRC_DEFAULT,
        .resolution_hz=10000,
        .direction=GPTIMER_COUNT_UP,
    }; 
    gptimer_new_timer(&timer_conf,&sw_timer);
    gptimer_alarm_config_t alarm_conf={
        .alarm_count=50,
        .flags.auto_reload_on_alarm=true
    };
    gptimer_set_alarm_action(sw_timer,&alarm_conf);
    gptimer_event_callbacks_t cbs = {
        .on_alarm = switch_callback // register user callback
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(sw_timer, &cbs, NULL));
    ESP_ERROR_CHECK(gptimer_enable(sw_timer));
    gptimer_start(sw_timer);


    //timer get radiation
    gptimer_config_t timer_conf_rad={
        .clk_src=GPTIMER_CLK_SRC_DEFAULT,
        .resolution_hz=10000,
        .direction=GPTIMER_COUNT_UP,
    }; 
    gptimer_new_timer(&timer_conf_rad,&get_rad_timer);
    gptimer_alarm_config_t alarm_conf_rad={
        .alarm_count=200,
        .flags.auto_reload_on_alarm=true
    };
    gptimer_set_alarm_action(get_rad_timer,&alarm_conf_rad);
    gptimer_event_callbacks_t cbs1 = {
        .on_alarm = get_rad_callback // register user callback
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(get_rad_timer, &cbs1, NULL));
    ESP_ERROR_CHECK(gptimer_enable(get_rad_timer));
    gptimer_start(get_rad_timer);


    return ESP_OK;
}


esp_err_t indication_spi_init(){

    gpio_set_direction(OUT_EN, GPIO_MODE_OUTPUT);
    gpio_set_level(OUT_EN, 0);

    //Dynamic indication pins
    // for(uint8_t i=0;i<4;i++){
    //     gpio_set_direction(dymIndArr[i],GPIO_MODE_OUTPUT);
    //     gpio_set_level(dymIndArr[i],0);
    // }
    
    
    // SPI bus configuration
    spi_bus_config_t buscfg = {
        .mosi_io_num = S_DATA,
        .miso_io_num = -1,          
        .sclk_io_num = SCK,
        .quadwp_io_num = -1,       
        .quadhd_io_num = -1,       
        .max_transfer_sz = 32      
    };
    
   
    esp_err_t ret = spi_bus_initialize(SPI_NUM, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        return ret;
    }

    
    spi_device_interface_config_t devcfg = {
        .mode = 1,                  
        .spics_io_num = LATCH,             
        .clock_speed_hz = 1000 * 1000,  
        .queue_size = 1,                 
        .flags = 0,
        .duty_cycle_pos = 128,           
    };

   
    ret = spi_bus_add_device(SPI_NUM, &devcfg, &handle);
    if (ret != ESP_OK) {
        return ret;
    }

    xTaskCreate(display_radiation_task,"display_radiation_task",2048,NULL,1,&display_rad);
    return ESP_OK;
};

void indication_set_num(uint8_t num){

    uint16_t res=0xFFFF&(1<<num);
    uint8_t tx_buf[2] = { (res>>8),(res&(0xFF)) };

    // SPI transaction
    spi_transaction_t transaction = {
        .flags = 0,
        .length = 16,                   
        .tx_buffer = tx_buf,            
        .rx_buffer = NULL              
    };

    spi_device_polling_transmit(handle, &transaction);

}


void set_num_to_disp(int num,uint8_t currentPos){
    // if(!uZv){
    //     for(uint8_t i=0;i<4;i++){
    //         gpio_set_level(dymIndArr[i],1);
    //         indication_set_num(num%10);
    //         vTaskDelay(100/portTICK_PERIOD_MS);
    //         num/=10;
    //         gpio_set_level(dymIndArr[i],0);
    //     }
    // }

    
}

static void display_radiation_task(void *pvParameters){
    while(1){
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        indication_set_num(current_radiation%10);
        current_radiation/=10;
        if(current_indicator==4)
            current_indicator=1;
        else current_indicator++;
    }
}