
#include"fonts.h"
 #include"ssd1306.h"
typedef struct {
    int hours;
    int minutes;
    int seconds;
} Time;
void update_display(Time t, uint32_t ldr);
void update_display(Time t, uint32_t ldr) {
    char display[20];

    SSD1306_Clear();
    SSD1306_GotoXY(0, 0);
    if (ldr > 3000) {
    SSD1306_Puts("Time running:", &Font_7x10, 1);

    SSD1306_GotoXY(0, 15);
    sprintf(display, "Hour: %02d", t.hours);
    SSD1306_Puts(display, &Font_7x10, 1);

    SSD1306_GotoXY(0, 30);
    sprintf(display, "Min:  %02d", t.minutes);
    SSD1306_Puts(display, &Font_7x10, 1);

    SSD1306_GotoXY(0, 45);
    sprintf(display, "Sec:  %02d", t.seconds);
    SSD1306_Puts(display, &Font_7x10, 1);}
else{

    SSD1306_Puts("Time running:", &Font_7x10, 0);

    SSD1306_GotoXY(0, 15);
    sprintf(display, "Hour: %02d", t.hours);
    SSD1306_Puts(display, &Font_7x10, 0);

    SSD1306_GotoXY(0, 30);
    sprintf(display, "Min:  %02d", t.minutes);
    SSD1306_Puts(display, &Font_7x10, 0);

    SSD1306_GotoXY(0, 45);
    sprintf(display, "Sec:  %02d", t.seconds);
    SSD1306_Puts(display, &Font_7x10, 0);
}

    SSD1306_UpdateScreen();
}
