#ifndef TOF_H
#define TOF_H

#include "stm32l4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

extern volatile uint8_t snooze_alarm;

void TOF_Init(I2C_HandleTypeDef *hi2c);
void TOF_Update(void);

#endif
