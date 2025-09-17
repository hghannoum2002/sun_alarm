#include "uart.h"
#include <string.h>
#include <stdio.h>

UART_HandleTypeDef huart2;

char rx_buffer[64];
char time_buffer[6];
char alarm_buffer[6];

volatile uint8_t data_received_flag = 0;

void UART_BT_Init(UART_HandleTypeDef *huart) {
    huart2 = *huart;
    HAL_UART_Receive_IT(&huart2, (uint8_t *)rx_buffer, sizeof(rx_buffer));
}

void UART_BT_ProcessReceivedData(void) {
    if (strncmp(rx_buffer, "TIME:", 5) == 0) {
        strncpy(time_buffer, rx_buffer + 5, 5);
        time_buffer[5] = '\0';
    } else if (strncmp(rx_buffer, "ALARM:", 6) == 0) {
        strncpy(alarm_buffer, rx_buffer + 6, 5);
        alarm_buffer[5] = '\0';
    }

    memset(rx_buffer, 0, sizeof(rx_buffer));
    data_received_flag = 0;

    HAL_UART_Receive_IT(&huart2, (uint8_t *)rx_buffer, sizeof(rx_buffer));
}

void UART_BT_UpdateAlarmTime(const char *new_alarm_time) {
    strncpy(alarm_buffer, new_alarm_time, 5);
    alarm_buffer[5] = '\0';
}

void UART_BT_Connect(void) {
	 HAL_UART_Transmit(&huart2, (uint8_t*)"READY", 5, 1000);
    while (strlen(time_buffer) == 0 || strlen(alarm_buffer) == 0) {
        if (data_received_flag) {
            UART_BT_ProcessReceivedData();
        }
    }
}

const char* UART_BT_GetTime(void) {
    return time_buffer;
}

const char* UART_BT_GetAlarmTime(void) {
    return alarm_buffer;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        data_received_flag = 1;
    }
}
