#include "count_rad.h"

#include "driver/gpio.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/gptimer.h"
#include "driver/ledc.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

static gptimer_handle_t gptimer = NULL;
static TaskHandle_t countRadTask;
static TaskHandle_t changeSpeedTask;

static QueueHandle_t pulseQueue;

static volatile uint32_t count=0;
static volatile uint32_t speedCount=0;
static volatile uint8_t currentTimeDelay=DEFAULT_MONITOR_PERIOD;

static uint32_t perNum=0;
static int radDat[10];
static uint8_t pos=0;

static int outRadiation;

static void play_sound(void *pvParameters){
    gpio_set_direction(2,GPIO_MODE_OUTPUT);
    while(1){
        uint32_t val;
        xQueueReceive(pulseQueue,&val,portMAX_DELAY);
        ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1,255/2);
        ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1);
        vTaskDelay(10/portTICK_PERIOD_MS);

        // ledc_set_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1,0);
        // ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1);
        // ledc_stop(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1,0);
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

static bool main_timer(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx){
    vTaskNotifyGiveFromISR(countRadTask,0);
    return true;
}

static bool speed_check_timer(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx){
    vTaskNotifyGiveFromISR(changeSpeedTask,0);
    return true;
}

static void tim_init(){
    gptimer_config_t timer_conf={
        .clk_src=GPTIMER_CLK_SRC_DEFAULT,
        .resolution_hz=2000,
        .direction=GPTIMER_COUNT_UP,
    }; 
    gptimer_new_timer(&timer_conf,&gptimer);
    gptimer_alarm_config_t alarm_conf={
        .alarm_count=5*2000,
        .flags.auto_reload_on_alarm=true
    };
    gptimer_set_alarm_action(gptimer,&alarm_conf);
    gptimer_event_callbacks_t cbs = {
        .on_alarm = main_timer // register user callback
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
    ESP_ERROR_CHECK(gptimer_enable(gptimer));
    gptimer_start(gptimer);

    //Speed check timer
    gptimer_handle_t gptimer1 = NULL;
    gptimer_new_timer(&timer_conf,&gptimer1);
    gptimer_alarm_config_t alarm_conf1={
        .alarm_count=1*2000,
        .flags.auto_reload_on_alarm=true
    };
    gptimer_set_alarm_action(gptimer1,&alarm_conf1);
    gptimer_event_callbacks_t cbs1 = {
        .on_alarm = speed_check_timer // register user callback
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer1, &cbs1, NULL));
    ESP_ERROR_CHECK(gptimer_enable(gptimer1));
    gptimer_start(gptimer1);

    //Pulse sound task 
    // pulseQueue = xQueueCreate(100, sizeof(uint32_t));
    // xTaskCreate(play_sound,"play_sound",1024,NULL,3,NULL);
}

static void IRAM_ATTR regRad(void* arg){
    count++;
    speedCount++;
    BaseType_t xTaskWokenByReceive = pdTRUE;
    xQueueSendFromISR(pulseQueue,&count,&xTaskWokenByReceive);
    // uint64_t start_time=esp_timer_get_time();
    // while((esp_timer_get_time()-start_time)<250);
    // ESP_DRAM_LOGI("I", "+");
    
}

static void count_rad(void *pvParameters){
    
    while (1){
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        int radiation=((float)count/currentTimeDelay)*45;
        perNum++;
        if(perNum>10){
            int calibRad;
            for (size_t i = 0; i < 10-1; i++){
                calibRad+=radDat[i];
                radDat[i]=radDat[i+1];
            }
            calibRad=(calibRad+radiation)/10;
            radDat[10-1]=radiation;
            ESP_LOGI("I","%d",calibRad);
            outRadiation=calibRad;

        }else{
            ESP_LOGI("RR","%d",radiation);
            radDat[pos++]=radiation;
        }
        // perNum++;
        count=0;
        /* code */
    }
    
}

static void changeSpeed(void *pvParameters){
    while(1){
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        if(speedCount>6 && currentTimeDelay!=3){
            ESP_DRAM_LOGI("SPEED IVOGED","%d",speedCount);
            gptimer_stop(gptimer);
            gptimer_disable(gptimer);
            gptimer_alarm_config_t alarm_conf={
                .alarm_count=3*2000,
                .flags.auto_reload_on_alarm=true
            };
            gptimer_set_alarm_action(gptimer,&alarm_conf);
            gptimer_event_callbacks_t cbs = {
                .on_alarm = main_timer // register user callback
            };
            ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
            ESP_ERROR_CHECK(gptimer_enable(gptimer));
            gptimer_start(gptimer);
            currentTimeDelay=3;

            // ESP_LOGI("I","Change speed invoked");
        }
        else if(currentTimeDelay==3 && speedCount<3){  //STEP CHANGE OF RADIATION IN LOW DIRECTION
            for(uint8_t i=0;i<10;i++){
                radDat[i]=0;
            }
            gptimer_stop(gptimer);
            gptimer_disable(gptimer);
            gptimer_alarm_config_t alarm_conf={
                .alarm_count=5*2000,
                .flags.auto_reload_on_alarm=true
            };
            gptimer_set_alarm_action(gptimer,&alarm_conf);
            gptimer_event_callbacks_t cbs = {
                .on_alarm = main_timer // register user callback
            };
            ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
            ESP_ERROR_CHECK(gptimer_enable(gptimer));
            gptimer_start(gptimer);
            currentTimeDelay=5;

        }
        // else if(currentTimeDelay!=5){
        //     gptimer_stop(gptimer);
        //     gptimer_disable(gptimer);
        //     gptimer_alarm_config_t alarm_conf={
        //         .alarm_count=5*2000,
        //         .flags.auto_reload_on_alarm=true
        //     };
        //     gptimer_set_alarm_action(gptimer,&alarm_conf);
        //     gptimer_event_callbacks_t cbs = {
        //         .on_alarm = main_timer // register user callback
        //     };
        //     ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
        //     ESP_ERROR_CHECK(gptimer_enable(gptimer));
        //     gptimer_start(gptimer);
        //     currentTimeDelay=5;
        // }
        speedCount=0;
    }
}

void count_rad_init(){
    gpio_install_isr_service(0);
    gpio_set_direction(TUBE_PIN,GPIO_MODE_INPUT);
    gpio_pullup_en(TUBE_PIN);
    gpio_isr_handler_add(TUBE_PIN,regRad,NULL);
    gpio_set_intr_type(TUBE_PIN, GPIO_INTR_NEGEDGE);
    gpio_intr_enable(TUBE_PIN);


    tim_init();

    pulseQueue = xQueueCreate(100, sizeof(uint32_t));
    xTaskCreate(play_sound,"play_sound",1024,NULL,3,NULL);
    xTaskCreate(changeSpeed,"changeSpeed",1024,NULL,3,&changeSpeedTask);
    xTaskCreate(count_rad,"count_rad",2048,NULL,2,&countRadTask);
}

int get_radiation(){
    return outRadiation;
}