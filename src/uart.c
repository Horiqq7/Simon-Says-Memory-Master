#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "uart.h"
#include <avr/io.h>
#include <stdlib.h>

void uart_init(uint16_t baud_rate) {
    uint16_t ubrr = (uint16_t)(F_CPU / 16UL / baud_rate - 1);
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void uart_print(const char *str) {
    while (*str) uart_transmit(*str++);
}

void uart_print_num(uint16_t num) {
    char buffer[6];
    itoa((int)num, buffer, 10);
    uart_print(buffer);
}