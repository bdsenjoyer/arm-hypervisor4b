#include "guest.h"

void task_timer(void);
void task_fibonacci(void);
void task_sysinfo(void);
void sched_task_done(void);

void guest_main(void) {
    hvc_debug_print(0xDEAD);

    uint64_t ver = hvc_get_version();
    hvc_debug_print(ver);

    sched_init();

    sched_add_task(task_timer,     "timer");
    sched_add_task(task_fibonacci, "fibonacci");
    sched_add_task(task_sysinfo,   "sysinfo");

    sched_run();

    hvc_debug_print(0xBEEF);
    hvc_exit();
}
