#include <stdint.h>
#include "../../include/uart.h"
#include "../../include/vm.h"

#define HVC_VERSION       0
#define HVC_VM_STATS      1
#define HVC_VM_INFO       2
#define HVC_DEBUG_PRINT   3
#define HVC_GET_TIME      4
#define HVC_GET_CPU_ID    5
#define HVC_MEM_INFO      6
#define HVC_EXIT          0xFF

static uint64_t total_hypercalls = 0;
static uint64_t hypercall_counts[7] = {0};

uint64_t hypercall_handler(uint64_t hvc_num, uint64_t arg1, uint64_t arg2) {
    total_hypercalls++;
    
    if (hvc_num < 7) {
        hypercall_counts[hvc_num]++;
    }
    
    switch(hvc_num) {
        case HVC_VERSION:
            return 0x00010000; // v1.0
        
        case HVC_VM_STATS:
            return total_hypercalls;
        
        case HVC_VM_INFO: {
            vm_context_t *vm = get_current_vm();
            return (uint64_t)vm;
        }
        
        case HVC_DEBUG_PRINT:
            uart_puts("[VM] Debug: 0x");
            uart_put_hex(arg1);
            uart_puts("\n");
            return 0;
        
        case HVC_GET_TIME: {
            uint64_t time;
            asm volatile("mrs %0, CNTPCT_EL0" : "=r"(time));
            return time;
        }
        
        case HVC_GET_CPU_ID: {
            uint64_t mpidr;
            asm volatile("mrs %0, MPIDR_EL1" : "=r"(mpidr));
            return mpidr & 0xFF; // CPU ID
        }
        
        case HVC_MEM_INFO:
            return 0x40000000 | (128 << 20); // Base | Size (128MB)
        
        case HVC_EXIT:
            uart_puts("[HV] VM requested exit\n");
            return 0xDEAD;
        
        default:
            return 0xFFFFFFFFFFFFFFFF;
    }
}

void show_hypercall_stats(void) {
    uart_puts("\n=== Hypercall Statistics ===\n");
    uart_puts("Total calls: ");
    uart_put_digit(total_hypercalls);
    uart_puts("\n");
    
    if (hypercall_counts[0] > 0) {
        uart_puts("  HVC_VERSION:     ");
        uart_put_digit(hypercall_counts[0]);
        uart_puts("\n");
    }
    if (hypercall_counts[1] > 0) {
        uart_puts("  HVC_VM_STATS:    ");
        uart_put_digit(hypercall_counts[1]);
        uart_puts("\n");
    }
    if (hypercall_counts[2] > 0) {
        uart_puts("  HVC_VM_INFO:     ");
        uart_put_digit(hypercall_counts[2]);
        uart_puts("\n");
    }
    if (hypercall_counts[3] > 0) {
        uart_puts("  HVC_DEBUG_PRINT: ");
        uart_put_digit(hypercall_counts[3]);
        uart_puts("\n");
    }
    if (hypercall_counts[4] > 0) {
        uart_puts("  HVC_GET_TIME:    ");
        uart_put_digit(hypercall_counts[4]);
        uart_puts("\n");
    }
    if (hypercall_counts[5] > 0) {
        uart_puts("  HVC_GET_CPU_ID:  ");
        uart_put_digit(hypercall_counts[5]);
        uart_puts("\n");
    }
    if (hypercall_counts[6] > 0) {
        uart_puts("  HVC_MEM_INFO:    ");
        uart_put_digit(hypercall_counts[6]);
        uart_puts("\n");
    }
    uart_puts("============================\n");
}
