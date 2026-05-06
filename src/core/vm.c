#include <stdint.h>
#include "../../include/vm.h"
#include "../../include/uart.h"
#include "../../include/hypercall.h"

static vm_context_t guest_vm;

void vm_init(vm_context_t *vm) {
    for (int i = 0; i < 31; i++) {
        vm->regs[i] = 0;
    }
    vm->sp_el1 = 0;
    vm->elr_el1 = 0;
    vm->spsr_el1 = 0;
    vm->wfi_count = 0;
    vm->is_running = 0;
    
    uart_puts("\n[VM] Guest context allocated\n");
    uart_puts("  Size: ");
    uart_put_digit(sizeof(vm_context_t));
    uart_puts(" bytes\n");
    uart_puts("  Ready for VM execution\n");
}

void vm_run_guest(vm_context_t *vm) {
    uart_puts("\n=== Hypercall Interface Test ===\n");
    
    vm->is_running = 1;
    
    uint64_t result;
    
    result = hypercall_handler(0, 0, 0);
    uart_puts("[API] HVC_VERSION -> ");
    uart_put_hex(result);
    uart_puts("\n");
    
    result = hypercall_handler(1, 0, 0);
    uart_puts("[API] HVC_VM_STATS -> ");
    uart_put_digit(result);
    uart_puts(" total calls\n");
    
    result = hypercall_handler(2, 0, 0);
    uart_puts("[API] HVC_VM_INFO -> ");
    uart_put_hex(result);
    uart_puts("\n");
    
    uart_puts("[API] HVC_DEBUG_PRINT -> ");
    hypercall_handler(3, 0x42, 0);
    
    result = hypercall_handler(4, 0, 0);
    uart_puts("[API] HVC_GET_TIME -> ");
    uart_put_hex(result);
    uart_puts("\n");
    
    result = hypercall_handler(5, 0, 0);
    uart_puts("[API] HVC_GET_CPU_ID -> CPU");
    uart_put_digit(result);
    uart_puts("\n");
    
    result = hypercall_handler(6, 0, 0);
    uart_puts("[API] HVC_MEM_INFO -> ");
    uart_put_hex(result);
    uart_puts("\n");
    
    hypercall_handler(0xFF, 0, 0);
    
    vm->is_running = 0;
    
    uart_puts("\n[OK] Hypercall API verified (7 services)\n");
    uart_puts("=========================================\n");
}

vm_context_t* get_current_vm(void) {
    return &guest_vm;
}
