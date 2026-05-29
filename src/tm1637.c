#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "tm1637.h"
#include <avr/io.h>
#include <util/delay.h>

static const uint8_t cifre_hex[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

void tm1637_start(void) {
    PORTC |=  (1 << CLK_PIN) | (1 << DIO_PIN); _delay_us(2);
    PORTC &= ~(1 << DIO_PIN); _delay_us(2);
}

void tm1637_stop(void) {
    PORTC &= ~(1 << CLK_PIN); _delay_us(2);
    PORTC &= ~(1 << DIO_PIN); _delay_us(2);
    PORTC |=  (1 << CLK_PIN); _delay_us(2);
    PORTC |=  (1 << DIO_PIN); _delay_us(2);
}

void tm1637_write_byte(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        PORTC &= ~(1 << CLK_PIN);
        if (data & 0x01) PORTC |=  (1 << DIO_PIN);
        else             PORTC &= ~(1 << DIO_PIN);
        data >>= 1;
        _delay_us(2);
        PORTC |= (1 << CLK_PIN); _delay_us(2);
    }
    PORTC &= ~(1 << CLK_PIN);
    PORTC |=  (1 << DIO_PIN);
    PORTC |=  (1 << CLK_PIN);
}

void afiseaza_custom(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4) {
    tm1637_start(); tm1637_write_byte(0x40); tm1637_stop(); 
    tm1637_start(); tm1637_write_byte(0xC0);                
    tm1637_write_byte(c1); tm1637_write_byte(c2);
    tm1637_write_byte(c3); tm1637_write_byte(c4);
    tm1637_stop();
    tm1637_start(); tm1637_write_byte(0x8F); tm1637_stop(); 
}

void afiseaza_scor(uint16_t scor) {
    afiseaza_custom(
        cifre_hex[(scor / 1000U) % 10U],
        cifre_hex[(scor / 100U)  % 10U],
        cifre_hex[(scor / 10U)   % 10U],
        cifre_hex[ scor          % 10U]
    );
}

void afiseaza_mesaj_HI(void) {
    afiseaza_custom(0x00, 0x76, 0x06, 0x00);
}