#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_putc(char c);
void uart_puts(const char* str);
void uart_put_digit(uint64_t num);
void uart_put_hex(uint64_t num);

#endif
