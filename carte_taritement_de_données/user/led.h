#ifndef LED_PWM_DRIVER_H
#define LED_PWM_DRIVER_H

#include "stdint.h"
#include "stm32L4xx_hal.h" // Adjust according to your STM32 family

void LED_PWM_Init(TIM_HandleTypeDef *htim, uint32_t channel);
void LED_PWM_SetLevel(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t level);

#endif
