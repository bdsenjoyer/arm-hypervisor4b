#include "guest.h"
#include "hypercall.h"

static inline uint64_t hvc_call(uint64_t num, uint64_t arg1, uint64_t arg2) {
    register uint64_t x0 asm("x0") = num;
    register uint64_t x1 asm("x1") = arg1;
    register uint64_t x2 asm("x2") = arg2;
    __asm__ volatile (
        "hvc #0"
        : "+r"(x0)
        : "r"(x1), "r"(x2)
        : "memory"
    );
    return x0;
}

uint64_t hvc_get_version(void)       { return hvc_call(HVC_VERSION, 0, 0); }
uint64_t hvc_get_time(void)          { return hvc_call(HVC_GET_TIME, 0, 0); }
uint64_t hvc_get_cpu_id(void)        { return hvc_call(HVC_GET_CPU_ID, 0, 0); }
uint64_t hvc_vm_stats(void)          { return hvc_call(HVC_VM_STATS, 0, 0); }
void     hvc_debug_print(uint64_t v) { hvc_call(HVC_DEBUG_PRINT, v, 0); }
void     hvc_exit(void)              { hvc_call(HVC_EXIT, 0, 0); __builtin_unreachable(); }
