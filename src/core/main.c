#include <stdint.h>
#include "../../include/uart.h"
#include "../../include/hypervisor.h"
#include "../../include/cpu_info.h"
#include "../../include/memory_info.h"
#include "../../include/vm.h"
#include "../../include/hypercall.h"

extern void exception_vector_table(void);

static uint64_t boot_start_time = 0;

uint64_t get_current_el(void) {
    uint64_t el;
    asm volatile("mrs %0, CurrentEL" : "=r"(el));
    return (el >> 2) & 0x3;
}

uint64_t get_mpidr(void) {
    uint64_t mpidr;
    asm volatile("mrs %0, MPIDR_EL1" : "=r"(mpidr));
    return mpidr;
}

uint64_t get_stack_pointer(void) {
    uint64_t sp;
    asm volatile("mov %0, sp" : "=r"(sp));
    return sp;
}

void start_boot_timer(void) {
    asm volatile("mrs %0, CNTPCT_EL0" : "=r"(boot_start_time));
}

void show_boot_time(void) {
    uint64_t end_time, freq;
    asm volatile("mrs %0, CNTPCT_EL0" : "=r"(end_time));
    asm volatile("mrs %0, CNTFRQ_EL0" : "=r"(freq));
    
    uint64_t elapsed = end_time - boot_start_time;
    uint64_t microseconds = (elapsed * 1000000) / freq;
    
    uart_puts("\n[PERF] Boot time: ");
    uart_put_digit(microseconds);
    uart_puts(" microseconds\n");
}

void print_system_info(void) {
    uart_puts("\n--- System Information ---\n");
    uart_puts("Exception Level: EL");
    uart_put_digit(get_current_el());
    uart_puts("\n");
    uart_puts("Processor MPIDR: ");
    uart_put_hex(get_mpidr());
    uart_puts("\n");
    uart_puts("Stack Pointer: ");
    uart_put_hex(get_stack_pointer());
    uart_puts("\n");
    uart_puts("--------------------------\n");
}

void setup_exceptions(void) {
    uart_puts("\n[INIT] Setting up exceptions...\n");
    
    uint64_t addr = (uint64_t)&exception_vector_table;
    uart_puts("  Vector table: ");
    uart_put_hex(addr);
    uart_puts("\n");
    
    asm volatile("msr VBAR_EL2, %0" :: "r"(addr));
    asm volatile("isb");
    
    uint64_t vbar;
    asm volatile("mrs %0, VBAR_EL2" : "=r"(vbar));
    uart_puts("  VBAR_EL2:     ");
    uart_put_hex(vbar);
    uart_puts("\n");
    uart_puts("[OK] Exceptions ready\n");
}

void hypervisor_main(void) {
    start_boot_timer();
    
    uart_puts("\n=================================\n");
    uart_puts("ARM Hypervisor v0.1\n");
    uart_puts("=================================\n");
    
    print_system_info();
    
    uint64_t el = get_current_el();
    if (el == 2) {
        uart_puts("\n[OK] Running in EL2\n");
    } else {
        uart_puts("\n[ERROR] Not in EL2!\n");
        while(1) { asm volatile("wfe"); }
    }
    
    hypervisor_init();
    setup_exceptions();
    show_virtualization_status();
    
    show_cpu_features();
    
    vm_context_t *vm = get_current_vm();
    vm_init(vm);
    
    show_boot_time();
    
    vm_run_guest(vm);

    show_hypercall_stats();
    
    uart_puts("\n*** HYPERVISOR INITIALIZED ***\n");
    uart_puts("Ready to accept VM in EL1\n");
    uart_puts("Hypercall interface operational\n\n");
    
    while (1) {
        asm volatile("wfe");
    }
}
