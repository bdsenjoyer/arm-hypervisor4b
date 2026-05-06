#ifndef VM_H
#define VM_H

#include <stdint.h>

typedef struct {
    uint64_t regs[31];
    uint64_t sp_el1;
    uint64_t elr_el1;
    uint64_t spsr_el1;
    uint64_t wfi_count;
    uint8_t is_running;
} vm_context_t;

void vm_init(vm_context_t *vm);
void vm_run_guest(vm_context_t *vm);
vm_context_t* get_current_vm(void);

#endif
