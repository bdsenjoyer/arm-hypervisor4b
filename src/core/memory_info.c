#include <stdint.h>
#include "../../include/uart.h"

void show_memory_layout(void) {
    uart_puts("\n=== Memory Layout ===\n");
    
    uart_puts("Hypervisor base: 0x40000000\n");
    
    uint64_t sp;
    asm volatile("mov %0, sp" : "=r"(sp));
    uart_puts("Stack pointer:   ");
    uart_put_hex(sp);
    uart_puts("\n");
    
    uart_puts("Total size:      ~12KB\n");
    uart_puts("=====================\n");
}
