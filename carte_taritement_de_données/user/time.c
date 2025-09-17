#include "time.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static Time currentTime = {0, 0, 0};
static char time_buffer[9];

//00:00:00
void Time_Init(void) {
    currentTime.hours = 0;
    currentTime.minutes = 0;
    currentTime.seconds = 0;
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d:%02d", currentTime.hours, currentTime.minutes, currentTime.seconds);
}

// plus one second
void Time_Update(void) {
    currentTime.seconds++;
    if (currentTime.seconds >= 60) {
        currentTime.seconds = 0;
        currentTime.minutes++;
        if (currentTime.minutes >= 60) {
            currentTime.minutes = 0;
            currentTime.hours++;
            if (currentTime.hours >= 24) {
                currentTime.hours = 0;
            }
        }
    }

    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d:%02d", currentTime.hours, currentTime.minutes, currentTime.seconds);
}

void Time_Set(const char *time_str) {
    if (time_str != NULL && strlen(time_str) == 5) {
        int hours, minutes;
        if (sscanf(time_str, "%2d:%2d", &hours, &minutes) == 2) {
            if (hours < 24 && minutes < 60) {
                currentTime.hours = hours;
                currentTime.minutes = minutes;
                currentTime.seconds = 0;
                snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d:%02d", hours, minutes, currentTime.seconds);
            }
        }
    }
}

const char* Time_Get(void) {
    return time_buffer;
}
