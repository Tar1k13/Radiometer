#include "indication.h"
#include "count_rad.h"

spi_device_handle_t handle;

static uint8_t dymIndArr[4]={IN_PWR_CTRL1,IN_PWR_CTRL2,IN_PWR_CTRL3,IN_PWR_CTRL4};

static void display_radiation_task(void *pvParameters);

esp_err_t indication_spi_init(){

    gpio_set_direction(OUT_EN, GPIO_MODE_OUTPUT);
    gpio_set_level(OUT_EN, 0);

    //Dynamic indication pins
    for(uint8_t i=0;i<4;i++){
        gpio_set_direction(dymIndArr[i],GPIO_MODE_OUTPUT);
        gpio_set_level(dymIndArr[i],0);
    }
    
    
    // SPI bus configuration
    spi_bus_config_t buscfg = {
        .mosi_io_num = S_DATA,
        .miso_io_num = -1,          
        .sclk_io_num = SCK,
        .quadwp_io_num = -1,       
        .quadhd_io_num = -1,       
        .max_transfer_sz = 32      
    };
    
   
    esp_err_t ret = spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        return ret;
    }

   
    spi_device_interface_config_t devcfg = {
        .mode = 1,                  
        .spics_io_num = 10,             
        .clock_speed_hz = 1000 * 1000,  
        .queue_size = 1,                 
        .flags = 0,
        .duty_cycle_pos = 128,           
    };

   
    ret = spi_bus_add_device(SPI3_HOST, &devcfg, &handle);
    if (ret != ESP_OK) {
        return ret;
    }

    xTaskCreate(display_radiation_task,"display_radiation_task",2048,NULL,1,NULL);
    return ESP_OK;
};

void indication_set_num(uint8_t num){

    uint16_t res=0xFFFF&(1<<num);
    uint8_t tx_buf[2] = { ~(res>>8),~(res&(0xFF)) };

    // SPI transaction
    spi_transaction_t transaction = {
        .flags = 0,
        .length = 16,                   
        .tx_buffer = tx_buf,            
        .rx_buffer = NULL              
    };

    spi_device_polling_transmit(handle, &transaction);

}


void set_num_to_disp(int num,bool uZv){
    if(!uZv){
        for(uint8_t i=0;i<4;i++){
            gpio_set_level(dymIndArr[i],1);
            indication_set_num(num%10);
            vTaskDelay(100/portTICK_PERIOD_MS);
            num/=10;
            gpio_set_level(dymIndArr[i],0);
        }
    }
    
}

static void display_radiation_task(void *pvParameters){
    while(1){
        int radiation=get_radiation();
        set_num_to_disp(radiation,false);
    }
}