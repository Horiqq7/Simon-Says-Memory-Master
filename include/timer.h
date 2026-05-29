#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define BUZZER_PIN PD6

void beep(uint16_t frecventa, uint16_t durata_ms);

#endif