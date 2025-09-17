#ifndef UART_H
#define UART_H

#include "stm32l4xx_hal.h"
void UART_BT_Init(UART_HandleTypeDef *huart);
void UART_BT_Connect(void);
const char* UART_BT_GetTime(void);
const char* UART_BT_GetAlarmTime(void);
void UART_BT_UpdateAlarmTime(const char *new_alarm_time);
void UART_BT_ProcessReceivedData(void);

#endif
