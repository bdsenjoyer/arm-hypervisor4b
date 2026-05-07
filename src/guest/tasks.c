#include "guest.h"

void task_timer(void) {
    hvc_debug_print(0xA001);
    for (int i = 0; i < 5; i++) {
        hvc_debug_print(hvc_get_time());
        sched_yield();
    }
    hvc_debug_print(0xA00F);
    sched_task_done();
}

void task_fibonacci(void) {
    hvc_debug_print(0xB001);
    uint64_t a = 0, b = 1;
    for (int i = 0; i < 8; i++) {
        hvc_debug_print(a);
        // shift window forward
        uint64_t next = a + b;
        a = b;
        b = next;
        sched_yield();
    }
    hvc_debug_print(0xB00F);
    sched_task_done();
}

void task_sysinfo(void) {
    hvc_debug_print(0xC001);
    // hypervisor version packed as major<<16 | minor
    hvc_debug_print(hvc_get_version());
    sched_yield();
    hvc_debug_print(hvc_get_cpu_id());
    sched_yield();
    // total hypercall count since boot
    hvc_debug_print(hvc_vm_stats());
    sched_yield();
    hvc_debug_print(hvc_get_time());
    hvc_debug_print(0xC00F);
    sched_task_done();
}
