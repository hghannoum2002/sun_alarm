#include "stm32l4xx_hal.h"
#include "buzzer.h"

extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim6; // Timer base pour les interruptions (TIM6 utilisé ici)

// ----- Configuration -----
#define BUZZER_TIMER       htim8
#define BUZZER_CHANNEL     TIM_CHANNEL_4


// ----- Mélodie type Nokia -----
const buzzernote nokia[] = {
    {16, 0, 1, {0, 0}}, {14, 0, 1, {0, 0}}, {9, 0, 1, {0, 0}}, {11, 0, 1, {0, 0}},
    {13, 0, 1, {0, 0}}, {11, 0, 1, {0, 0}}, {2, 0, 1, {0, 0}}, {3, 0, 1, {0, 0}},
    {0, 0, 0, {0, 0}} // fin
};

// ----- Fréquences des notes -----
static const float note_freqs[18] = {
    0.0,    // silence
    261.63, // C4
    277.18, // C#4
    293.66, // D4
    311.13, // D#4
    329.63, // E4
    349.23, // F4
    369.99, // F#4
    392.00, // G4
    415.30, // G#4
    440.00, // A4
    466.16, // A#4
    493.88, // B4
    523.25, // C5
    554.37, // C#5
    587.33, // D5
    622.25, // D#5
    659.25  // E5
};

// ----- Contrôle du buzzer -----
typedef struct {
    const buzzernote* song;
    uint16_t tempo;
    uint16_t index;
    uint8_t repeat;
    uint8_t current_repeat;
    uint8_t is_playing;
} BuzzerPlayer;

volatile BuzzerPlayer buzzer_player = {0};
volatile uint8_t buzzer_stop_flag = 0;
static uint32_t note_timer = 0;

// ----- Initialisation du PWM du buzzer -----
void buzzer_vInit()
{
    HAL_TIM_PWM_Start(&BUZZER_TIMER, BUZZER_CHANNEL);
}

// ----- Génère la fréquence PWM -----
static void buzzer_set_freq(float freq)
{
    if (freq == 0.0f) {
        __HAL_TIM_SET_COMPARE(&BUZZER_TIMER, BUZZER_CHANNEL, 0);
        return;
    }

    uint32_t timer_clk = HAL_RCC_GetPCLK1Freq() * 2;
    uint32_t prescaler = BUZZER_TIMER.Init.Prescaler + 1;
    uint32_t period = (uint32_t)((float)timer_clk / (freq * prescaler));
    uint32_t pulse = period / 2;

    __HAL_TIM_SET_AUTORELOAD(&BUZZER_TIMER, period);
    __HAL_TIM_SET_COMPARE(&BUZZER_TIMER, BUZZER_CHANNEL, pulse);
    BUZZER_TIMER.Instance->CNT = 0;
}

// ----- Tâche appelée toutes les 10ms par interruption TIM6 -----
void buzzer_task(void)
{
    if (!buzzer_player.is_playing || buzzer_stop_flag)
        return;

    if (note_timer > 0) {
        note_timer--;
        if (note_timer == 1) {
            buzzer_set_freq(0); // court silence à la fin de la note
        }
        return;
    }

    const buzzernote* note = &buzzer_player.song[buzzer_player.index];
    if (note->duration == 0) {
        // Fin de morceau
        buzzer_player.current_repeat++;
        if (buzzer_player.current_repeat >= buzzer_player.repeat) {
            buzzer_player.is_playing = 0;
            buzzer_set_freq(0);
            return;
        } else {
            buzzer_player.index = 0;
            return;
        }
    }

    // Lire la note
    float freq = 0.0;
    if (note->note > 0 && note->note < 18) {
        freq = note_freqs[note->note];
        if (note->sharp && (note->note + 1 < 18)) {
            freq = note_freqs[note->note + 1];
        }
    }

    buzzer_set_freq(freq);

    uint32_t beat_ms = 60000 / buzzer_player.tempo;
    note_timer = (beat_ms * note->duration) / 10;
    if (note_timer > 2) note_timer -= 1; // courte pause
    buzzer_player.index++;
}

// ----- Lancer une mélodie -----
buzzer_status_t buzzer_ePlaySong(uint16_t tempo)
{
    buzzer_stop_flag = 0;
    buzzer_vInit();

    buzzer_player.song = nokia;
    buzzer_player.tempo = tempo;
    buzzer_player.index = 0;
    buzzer_player.repeat = 5;
    buzzer_player.current_repeat = 0;
    buzzer_player.is_playing = 1;
    note_timer = 0;

    return BUZZER_OK;
}


uint8_t buzzer_isIdle(void)
{
    return (buzzer_player.is_playing == 0);
}

// ----- Arrêter immédiatement -----
void buzzer_vStop(void)
{
    buzzer_stop_flag = 1;
    buzzer_player.is_playing = 0;
    buzzer_set_freq(0);
}
