#include "indication.h"
#include "driver/spi_common.h"
#include "freertos/projdefs.h"




static Indicaton indication;


// Callback to switch current lamp (called every 5ms)
static bool change_num_callback (gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(indication.xDisplayIndication, &xHigherPriorityTaskWoken);
	return false;
}

// Configures general timer so it generates ITR every 5ms
static void indication_timer_init(void)
{
	gptimer_handle_t gptimer = NULL;
	gptimer_config_t update_timer_config = {
		.clk_src       = GPTIMER_CLK_SRC_DEFAULT,
		.direction     = GPTIMER_COUNT_UP,
		.resolution_hz = 100000,
	};
	ESP_ERROR_CHECK(gptimer_new_timer(&update_timer_config, &gptimer));

	gptimer_alarm_config_t update_timer_alarm = {
		.alarm_count = 500,
		.reload_count = 0,
		.flags = {
			.auto_reload_on_alarm = pdTRUE,
		},
	};
	ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &update_timer_alarm));

	gptimer_event_callbacks_t callback = {
		.on_alarm = change_num_callback
	};
	ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &callback, NULL));

	ESP_ERROR_CHECK(gptimer_enable(gptimer));
	ESP_ERROR_CHECK(gptimer_start(gptimer));
}


// Configures corresponding gpio needed for dynamic indication
static void indication_gpio_init(void)
{
	gpio_config_t gpio_conf =
	{
		.pin_bit_mask = GPIO_PIN_MSK,
		.mode         = GPIO_MODE_OUTPUT,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.pull_up_en   = GPIO_PULLUP_DISABLE
	};
	ESP_ERROR_CHECK(gpio_config(&gpio_conf));

	GPIO.out_w1tc = GPIO_PIN_MSK;
}


// SPI init function to control shift registers
static void indication_spi_init(void)
{
	gpio_set_direction(OUT_EN, GPIO_MODE_OUTPUT);
	gpio_set_level(OUT_EN, 0);

	gpio_set_direction(LATCH,GPIO_MODE_OUTPUT);
	spi_bus_config_t buscfg = {
		.mosi_io_num = S_DATA,
		.miso_io_num = -1,          
		.sclk_io_num = SCK,
		.quadwp_io_num = -1,       
		.quadhd_io_num = -1,       
		.max_transfer_sz = 32      
	};
	ESP_ERROR_CHECK(spi_bus_initialize(SPI_NUM, &buscfg, SPI_DMA_CH_AUTO));

	spi_device_interface_config_t devcfg = {
		.mode = 0,                              
		.clock_speed_hz = 1000 * 1000,  
		.queue_size = 1,                     
	};

	ESP_ERROR_CHECK(spi_bus_add_device(SPI_NUM, &devcfg, &(indication.shiftRegistersDev)));
}


static void indication_set_line(uint8_t current_indicator)
{
	// Reset all lines
	GPIO.out_w1tc = GPIO_PIN_MSK;

	switch (current_indicator)
	{
		case 1:
			gpio_set_level(GPIO_OUT1, 1);
			break;
		case 2:
			gpio_set_level(GPIO_OUT2, 1);
			break;
		case 3:
			gpio_set_level(GPIO_OUT3, 1);
			break;
		case 4:
			gpio_set_level(GPIO_OUT4, 1);
			break;
		default:
			break;
	}
}


static void indication_spi_number_transmit(uint16_t bit_field)
{
	uint8_t tx_buf[2] = { (bit_field>>8),(bit_field&(0xFF)) };

	// SPI transaction
	spi_transaction_t transaction = {
			.flags = 0,
			.length = 16,                   
			.tx_buffer = tx_buf,            
			.rx_buffer = NULL              
	};

	spi_device_polling_transmit(indication.shiftRegistersDev, &transaction);
	gpio_set_level(LATCH, 1);
	gpio_set_level(LATCH,0);

}



static void indication_task(void *pvParameters)
{
	// Initialize all required components
	indication_gpio_init();
	indication_spi_init();
	indication_timer_init();

	indication.current_ind = 4;

	// Create set num mutex
	indication.set_num_mutex = xSemaphoreCreateMutex();
	assert(indication.set_num_mutex != NULL);
	
	indication.current_number_to_display = 1234;
	indication.current_number_to_diplay_new = 1234;
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		GPIO.out_w1tc = GPIO_PIN_MSK;


		uint16_t num_bit_field = 1 << (indication.current_number_to_display % 10);
		indication_spi_number_transmit(num_bit_field);
		indication.current_number_to_display /= 10;


		indication_set_line(indication.current_ind--);

		if(indication.current_ind == 0)
		{
			indication.current_ind = 4u;
			indication.current_number_to_display = indication.current_number_to_diplay_new;
		}
	}
}

void indication_register_component(void)
{
	xTaskCreate(indication_task, "indication_task", 2048, NULL, 1, &(indication.xDisplayIndication));
}

// static void indication_spi_init(void)
// {

// 	gpio_set_direction(OUT_EN, GPIO_MODE_OUTPUT);
// 	gpio_set_level(OUT_EN, 0);

// 	gpio_set_direction(LATCH,GPIO_MODE_OUTPUT);


	
// 	// SPI bus configuration
// 	spi_bus_config_t buscfg = {
// 			.mosi_io_num = S_DATA,
// 			.miso_io_num = -1,          
// 			.sclk_io_num = SCK,
// 			.quadwp_io_num = -1,       
// 			.quadhd_io_num = -1,       
// 			.max_transfer_sz = 32      
// 	};
	
 
// 	esp_err_t ret = spi_bus_initialize(SPI_NUM, &buscfg, SPI_DMA_CH_AUTO);
// 	if (ret != ESP_OK) {
// 			return ret;
// 	}

	
// 	spi_device_interface_config_t devcfg = {
// 			.mode = 0,                              
// 			.clock_speed_hz = 1000 * 1000,  
// 			.queue_size = 1,                     
// 	};

 
// 	ret = spi_bus_add_device(SPI_NUM, &devcfg, &handle);
// 	if (ret != ESP_OK) {
// 		return ret;
// 	}
// }