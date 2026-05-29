#include "timer.h"
#include <avr/io.h>
#include <util/delay.h>

void beep(uint16_t frecventa, uint16_t durata_ms) {
    if (frecventa == 0) return;

    uint8_t top_val = (uint8_t)((31250U / frecventa) - 1U);
    TCCR0A = (1 << COM0A0) | (1 << WGM01); // CTC + toggle OC0A
    OCR0A  = top_val;
    TCCR0B = (1 << CS02);                  // porneste timer, prescaler 256

    for (uint16_t i = 0; i < durata_ms; i++) _delay_ms(1);

    TCCR0B = 0;
    TCCR0A = 0;
    PORTD &= ~(1 << BUZZER_PIN);
}