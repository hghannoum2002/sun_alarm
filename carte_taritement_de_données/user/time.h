#ifndef TIME_H
#define TIME_H

#include <stdint.h>

// Time structure
typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} Time;

void Time_Init(void);
void Time_Update(void);
void Time_Set(const char *time_str);
const char* Time_Get(void);

#endif
