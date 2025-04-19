#pragma once

#include <driver/ledc.h>
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "freertos/task.h"
#include "driver/gptimer.h"
#include "driver/gpio.h"
#include "soc/gpio_struct.h"


// To be moved

#define OUT1_CHANNEL LEDC_CHANNEL_2
#define OUT2_CHANNEL LEDC_CHANNEL_3
#define OUT3_CHANNEL LEDC_CHANNEL_4
#define OUT4_CHANNEL LEDC_CHANNEL_5



#ifdef ESP_TESTING_BUILD

#define LATCH 	GPIO_NUM_23
#define SCK 		GPIO_NUM_17
#define S_DATA 	GPIO_NUM_16
#define OUT_EN 	GPIO_NUM_19

#define GPIO_OUT1 GPIO_NUM_26
#define GPIO_OUT2 GPIO_NUM_12
#define GPIO_OUT3 GPIO_NUM_14
#define GPIO_OUT4 GPIO_NUM_27
#define GPIO_PIN_MSK ((uint64_t)((1ULL << GPIO_OUT1) | (1ULL << GPIO_OUT2) | (1ULL << GPIO_OUT3) | (1ULL << GPIO_OUT4)))

#define SPI_NUM HSPI_HOST

#else

#endif



typedef struct
{
	uint8_t current_ind;					 		// current indicator that rendered
	uint16_t current_number_to_display;  		// current number to display
	uint16_t current_number_to_diplay_new;  // next number to be displayed
	SemaphoreHandle_t set_num_mutex;				// mutex to set current number
	TaskHandle_t xDisplayIndication;				// task handle of display num task      
	spi_device_handle_t shiftRegistersDev;  // Spi device handle for shift registers      
} Indicaton;


// Register and init component
// returns: queue of that component
void indication_register_component(void);

// Set number to display function
void indication_set_number(uint16_t number);





