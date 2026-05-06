#include <stdint.h>
#include "../../include/uart.h"

void show_cpu_features(void) {
    uint64_t id_aa64pfr0;
    asm volatile("mrs %0, ID_AA64PFR0_EL1" : "=r"(id_aa64pfr0));
    
    uint64_t el2 = (id_aa64pfr0 >> 8) & 0xF;
    
    uart_puts("\n[CPU] Virtualization support: ");
    if (el2 > 0) {
        uart_puts("EL2 available\n");
    } else {
        uart_puts("EL2 NOT available\n");
    }
}	
