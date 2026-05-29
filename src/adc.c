#include "adc.h"
#include <avr/io.h>

void adc_init(void) {
    // REFS0: referinta VCC | MUX1: canal ADC2 = PC2 (pin flotant)
    ADMUX  = (1 << REFS0) | (1 << MUX1);
    // ADEN: activeaza ADC | ADPS2+1+0: prescaler 128 => 125kHz
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(void) {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}