#ifndef __OLED_H
#define __OLED_H

#include "stdint.h"
#include "stdbool.h"

void OLED_Init(void);
void OLED_TurnOn(void);
void OLED_TurnOff(void);

void OLED_SetMode(bool isDay);  // true for "J" (jour), false for "N" (nuit)
void OLED_SetTime(uint8_t hh, uint8_t mm, uint8_t ss);
void OLED_SetTempIAQ(float temperature, float iaq);
void OLED_UpdateDisplay(void);

#endif
