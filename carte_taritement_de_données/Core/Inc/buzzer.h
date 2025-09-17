#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>
extern uint32_t count540;
typedef enum {
    BUZZER_OK,
    BUZZER_ERROR
} buzzer_status_t;

typedef struct {
    uint8_t note;
    uint8_t sharp;
    uint8_t duration;
    uint8_t reserved[2]; // For alignment / future use
} buzzernote;

uint8_t buzzer_isIdle(void);
void buzzer_vInit();
buzzer_status_t buzzer_ePlaySong( uint16_t tempo);
void buzzer_vStop(void);



#endif // BUZZER_H
