#include <stdint.h>

#define UART0_BASE 0x09000000
#define UART0_DR   *((volatile uint32_t*)(UART0_BASE + 0x00))
#define UART0_FR   *((volatile uint32_t*)(UART0_BASE + 0x18))

void uart_putc(char c) {
    while (UART0_FR & (1 << 5)) {}
    UART0_DR = c;
}

void uart_puts(const char* str) {
    while (*str) {
        uart_putc(*str++);
    }
}

void uart_put_digit(uint64_t num) {
    if (num > 9) {
        uart_put_digit(num / 10);
    }
    uart_putc('0' + (num % 10));
}

void uart_put_hex(uint64_t num) {
    uart_puts("0x");
    for (int i = 60; i >= 0; i -= 4) {
        uint64_t digit = (num >> i) & 0xF;
        if (digit < 10) {
            uart_putc('0' + digit);
        } else {
            uart_putc('A' + digit - 10);
        }
    }
}
