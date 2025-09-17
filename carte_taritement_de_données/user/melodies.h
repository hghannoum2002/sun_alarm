#ifndef MELODIES_H
#define MELODIES_H

#include "buzzer.h"

extern const buzzernote mario[];
extern const buzzernote nokia[];
extern const buzzernote zelda[];

#define NUM_MELODIES 3

extern const buzzernote* const melodies[NUM_MELODIES];
extern const int melody_lengths[NUM_MELODIES];

#endif // MELODIES_H
