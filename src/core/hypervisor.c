#include <stdint.h>
#include "../../include/uart.h"

#define HCR_RW  (1UL << 31)
#define HCR_TWI (1UL << 13)
#define HCR_VM  (1UL << 0)

uint64_t read_hcr_el2(void) {
    uint64_t hcr;
    asm volatile("mrs %0, HCR_EL2" : "=r"(hcr));
    return hcr;
}

void write_hcr_el2(uint64_t hcr) {
    asm volatile("msr HCR_EL2, %0" :: "r"(hcr));
    asm volatile("isb");
}

void hypervisor_init(void) {
    uint64_t hcr_el2;
    
    uart_puts("\n[INIT] Configuring hypervisor...\n");
    
    // 1. CPTR_EL2: Disable all coprocessor traps
    asm volatile("msr CPTR_EL2, xzr");
    asm volatile("isb");
    uart_puts("  CPTR_EL2: 0 (no traps)\n");
    
    // 2. CNTHCTL_EL2: Allow EL1/EL0 timer access
    asm volatile("mov x0, #3");
    asm volatile("msr CNTHCTL_EL2, x0");
    asm volatile("isb");
    uart_puts("  CNTHCTL_EL2: 3 (EL1 timer access)\n");
    
    // 3. CNTVOFF_EL2: Zero offset
    asm volatile("msr CNTVOFF_EL2, xzr");
    uart_puts("  CNTVOFF_EL2: 0\n");
    
    // 4. HSTR_EL2: No system register traps
    asm volatile("msr HSTR_EL2, xzr");
    uart_puts("  HSTR_EL2: 0\n");
    
    // 5. HCR_EL2: Enable virtualization
    hcr_el2 = 0x80002039;  // RW + TWI + AMO + IMO + FMO + VM
    hcr_el2 &= ~(1UL << 29); // HCD=0 (HVC enabled)
    
    asm volatile("msr HCR_EL2, %0" :: "r"(hcr_el2));
    asm volatile("isb");
    
    uart_puts("  HCR_EL2: ");
    uart_put_hex(hcr_el2);
    uart_puts("\n");
    uart_puts("[OK] Hypervisor configured\n");
    uart_puts("  64-bit VM: Enabled\n");
    uart_puts("  WFI trap: Enabled\n");
    uart_puts("  Exception routing: Enabled\n");
    uart_puts("  HVC: Enabled\n");
}

void show_virtualization_status(void) {
    uart_puts("\n--- Virtualization Status ---\n");
    
    uint64_t hcr = read_hcr_el2();
    
    uart_puts("HCR_EL2.RW (64-bit VMs): ");
    if (hcr & HCR_RW) {
        uart_puts("Enabled\n");
    } else {
        uart_puts("Disabled\n");
    }
    
    uart_puts("HCR_EL2.TWI (Trap WFI):  ");
    if (hcr & HCR_TWI) {
        uart_puts("Enabled\n");
    } else {
        uart_puts("Disabled\n");
    }
    
    uart_puts("HCR_EL2.VM (Stage-2):    ");
    if (hcr & HCR_VM) {
        uart_puts("Enabled\n");
    } else {
        uart_puts("Disabled\n");
    }
    
    uart_puts("-----------------------------\n");
}
