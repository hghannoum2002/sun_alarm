#include "led_pwm_driver.h"

#define PWM_MAX_DUTY 1000 // Max PWM compare value
#define INTENSITY_LEVELS 10

void LED_PWM_Init(TIM_HandleTypeDef *htim, uint32_t channel)
{
    HAL_TIM_PWM_Start(htim, channel);
    LED_PWM_SetLevel(htim, channel, 0); // Start with LED off
}

void LED_PWM_SetLevel(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t level)
{
    if (level > INTENSITY_LEVELS) level = INTENSITY_LEVELS;

    uint32_t duty = (level * PWM_MAX_DUTY) / INTENSITY_LEVELS;

    __HAL_TIM_SET_COMPARE(htim, channel, duty);
}
