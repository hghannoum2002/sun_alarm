#include "oled.h"
#include "ssd1306.h"
#include "string.h"
#include "stdio.h"

static bool displayOn = true;
static bool isDay = true;
static uint8_t hour = 0, minute = 0, second = 0;
static float temperature = 0.0f;
static float iaq = 0.0f;

void OLED_Init(void) {
    ssd1306_Init();
    OLED_TurnOn();
}

void OLED_TurnOn(void) {
    displayOn = true;
    ssd1306_DisplayOn();
    OLED_UpdateDisplay();
}

void OLED_TurnOff(void) {
    displayOn = false;
    ssd1306_DisplayOff();
}

void OLED_SetMode(bool mode) {
    isDay = mode;
}

void OLED_SetTime(uint8_t hh, uint8_t mm, uint8_t ss) {
    hour = hh;
    minute = mm;
    second = ss;
}

void OLED_SetTempIAQ(float temp, float airQuality) {
    temperature = temp;
    iaq = airQuality;
}

void OLED_UpdateDisplay(void) {
    if (!displayOn) return;

    char buffer[32];

    ssd1306_Fill(Black);

    // Line 1: Day/Night
    sprintf(buffer, "Mode: %c", isDay ? 'J' : 'N');
    ssd1306_SetCursor(0, 0);
    ssd1306_WriteString(buffer, Font_7x10, White);

    // Line 2: Time
    sprintf(buffer, "Heure: %02d:%02d:%02d", hour, minute, second);
    ssd1306_SetCursor(0, 12);
    ssd1306_WriteString(buffer, Font_7x10, White);

    // Line 3: Temp
    sprintf(buffer, "Temp: %.1f C", temperature);
    ssd1306_SetCursor(0, 24);
    ssd1306_WriteString(buffer, Font_7x10, White);

    // Line 4: IAQ
    sprintf(buffer, "IAQ: %.1f", iaq);
    ssd1306_SetCursor(0, 36);
    ssd1306_WriteString(buffer, Font_7x10, White);

    ssd1306_UpdateScreen();
}
