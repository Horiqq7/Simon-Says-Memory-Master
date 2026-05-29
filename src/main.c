#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/eeprom.h>

#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "tm1637.h"

#define LED_0 PB0
#define LED_1 PB1
#define LED_2 PB2
#define LED_3 PB3

#define BTN_0 PD2
#define BTN_1 PD3
#define BTN_2 PD4
#define BTN_3 PD5

#define MASCA_BUTOANE ((1 << BTN_0) | (1 << BTN_1) | (1 << BTN_2) | (1 << BTN_3))
#define EEPROM_HS_ADDR ((uint16_t*)10)
#define LUNGIME_MAX_SECVENTA 100

// ============================================================
// MAIN
// ============================================================

int main(void) {
    // Configurare GPIO
    DDRB |=  (1 << LED_0) | (1 << LED_1) | (1 << LED_2) | (1 << LED_3); 
    DDRD &= ~MASCA_BUTOANE;   
    PORTD |=  MASCA_BUTOANE;  
    
    DDRD |=  (1 << BUZZER_PIN);
    DDRC |=  (1 << CLK_PIN) | (1 << DIO_PIN);

    uart_init(9600);
    adc_init();

    const uint8_t pini_led[]     = {LED_0, LED_1, LED_2, LED_3};
    const uint8_t pini_butoane[] = {BTN_0, BTN_1, BTN_2, BTN_3};

    uart_print("\r\nSimon Says\r\n");

    uint16_t high_score = eeprom_read_word(EEPROM_HS_ADDR);
    if (high_score == 0xFFFF || high_score > 9999U) high_score = 0;

    while (1) {
        uart_print("High Score: ");
        uart_print_num(high_score);
        uart_print("\r\n");
        afiseaza_mesaj_HI();

        uint16_t seed_counter = 0;

        while (1) {
            uint8_t stare = PIND & MASCA_BUTOANE;

            if (stare != MASCA_BUTOANE) {
                _delay_ms(20); // Debounce
                stare = PIND & MASCA_BUTOANE;

                if (stare != MASCA_BUTOANE) {
                    if (!(PIND & (1 << BTN_0)) && !(PIND & (1 << BTN_3))) {
                        uart_print("EEPROM Reset.\r\n");
                        high_score = 0;
                        eeprom_update_word(EEPROM_HS_ADDR, high_score);
                        afiseaza_custom(0x00, 0x00, 0x00, 0x00);
                        beep(200, 800);
                        afiseaza_scor(0);
                        _delay_ms(1000);
                        afiseaza_mesaj_HI();
                        while (!(PIND & (1 << BTN_0)) || !(PIND & (1 << BTN_3)));
                        _delay_ms(50);
                    } else {
                        break; 
                    }
                }
            }
            seed_counter++;
        }

        uint16_t seed_zgomot = 0;
        for (uint8_t i = 0; i < 10; i++) seed_zgomot += adc_read();
        srand((unsigned int)(seed_counter + seed_zgomot));

        beep(1500, 200);
        _delay_ms(500);

        uint16_t scor      = 0;
        uint8_t  game_over = 0;
        uint8_t  secventa[LUNGIME_MAX_SECVENTA];

        while (!game_over) {
            afiseaza_scor(scor);
            _delay_ms(1000);

            uart_print("Nivel: ");
            uart_print_num((uint16_t)(scor + 1U));
            uart_print("\r\n");

            uint8_t led_nou;
            while (1) {
                led_nou = (uint8_t)(rand() % 4);
                uint8_t mutari_de_verificat = (scor >= 4U) ? 4U : (uint8_t)scor;
                uint8_t repetari = 0;
                for (uint8_t j = (uint8_t)(scor - mutari_de_verificat); j < (uint8_t)scor; j++) {
                    if (secventa[j] == led_nou) repetari++;
                }
                if (repetari < 2) break;
            }
            secventa[scor] = led_nou;

            uint16_t viteza_afisare    = (scor < 15U) ? (uint16_t)(400U - scor * 20U) : 100U;
            uint16_t timp_reactie_maxim = (scor < 22U) ? (uint16_t)(3000U - scor * 100U) : 800U;

            for (uint8_t i = 0; i <= (uint8_t)scor; i++) {
                PORTB |= (1 << pini_led[secventa[i]]);
                beep((uint16_t)(600U + secventa[i] * 200U), (uint16_t)(viteza_afisare / 2U));
                for (uint16_t d = 0; d < viteza_afisare; d++) _delay_ms(1);
                PORTB &= ~(1 << pini_led[secventa[i]]);
                for (uint16_t d = 0; d < viteza_afisare / 2U; d++) _delay_ms(1);
            }

            uint8_t corect = 1;
            for (uint8_t i = 0; i <= (uint8_t)scor; i++) {
                int8_t   apasat     = -1;
                uint16_t timp_scurs =  0;

                while (apasat == -1 && timp_scurs < timp_reactie_maxim) {
                    for (uint8_t b = 0; b < 4; b++) {
                        if (!(PIND & (1 << pini_butoane[b]))) {
                            _delay_ms(20); 
                            if (!(PIND & (1 << pini_butoane[b]))) {
                                apasat = (int8_t)b;
                                PORTB |= (1 << pini_led[b]);
                                beep((uint16_t)(600U + b * 200U), 150);
                                while (!(PIND & (1 << pini_butoane[b]))); 
                                _delay_ms(50);
                                PORTB &= ~(1 << pini_led[b]);
                            }
                        }
                    }
                    _delay_ms(1);
                    timp_scurs++;
                }

                if (timp_scurs >= timp_reactie_maxim || (uint8_t)apasat != secventa[i]) {
                    corect = 0;
                    break;
                }
            }

            if (corect) {
                scor++;
                _delay_ms(300);
                beep(1500, 100);
                beep(2000, 150);
            } else {
                uart_print("Game Over. Scor: ");
                uart_print_num(scor);
                uart_print("\r\n");

                if (scor > high_score) {
                    uart_print("Nou Record!\r\n");
                    high_score = scor;
                    eeprom_update_word(EEPROM_HS_ADDR, high_score);
                    for (uint8_t k = 0; k < 5; k++) {
                        afiseaza_scor(scor);
                        beep((uint16_t)(1000U + k * 200U), 100);
                        _delay_ms(100);
                        afiseaza_custom(0, 0, 0, 0);
                        _delay_ms(100);
                    }
                } else {
                    for (uint8_t k = 0; k < 3; k++) {
                        afiseaza_custom(0, 0, 0, 0);
                        beep(300, 200);
                        _delay_ms(200);
                        afiseaza_scor(scor);
                        _delay_ms(300);
                    }
                }

                _delay_ms(1500);
                game_over = 1;
            }
        }
    }

    return 0;
}