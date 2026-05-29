#ifndef TM1637_H
#define TM1637_H

#include <stdint.h>

// Pinii conectati la display (Port C)
#define DIO_PIN PC0
#define CLK_PIN PC1

void tm1637_start(void);
void tm1637_stop(void);
void tm1637_write_byte(uint8_t data);
void afiseaza_custom(uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4);
void afiseaza_scor(uint16_t scor);
void afiseaza_mesaj_HI(void);

#endif