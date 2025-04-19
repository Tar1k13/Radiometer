#pragma once


#ifdef ESP_TESTING_BUILD

// PWM channels
#define PIPE_PWM GPIO_NUM_21
#define INDICATOR_PWM GPIO_NUM_4
#define SOUND_PWM GPIO_NUM_2

#else

// PWM channels
#define PIPE_PWM GPIO_NUM_21
#define INDICATOR_PWM GPIO_NUM_4
#define SOUND_PWM GPIO_NUM_2

#endif