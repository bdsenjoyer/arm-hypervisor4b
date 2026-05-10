#ifndef GUEST_H
#define GUEST_H

#include <stdint.h>

#define MAX_TASKS       8
#define TASK_STACK_SIZE 1024

typedef enum {
    TASK_READY   = 0,
    TASK_RUNNING = 1,
    TASK_DONE    = 2,
} task_state_t;

// saved CPU state for context switch
typedef struct {
    uint64_t x[31];
    uint64_t sp;
    uint64_t pc;
} task_context_t;

typedef void (*task_fn_t)(void);

typedef struct {
    task_context_t  ctx;
    task_state_t    state;
    task_fn_t       entry;
    uint8_t         stack[TASK_STACK_SIZE];
    const char     *name;
    uint32_t        runs;
} task_t;

// scheduler API
void sched_init(void);
void sched_add_task(task_fn_t fn, const char *name);
void sched_run(void);
void sched_yield(void);
void sched_task_done(void);

// hypercall wrappers
uint64_t hvc_get_version(void);
uint64_t hvc_get_time(void);
uint64_t hvc_get_cpu_id(void);
uint64_t hvc_vm_stats(void);
void     hvc_debug_print(uint64_t value);
void     hvc_exit(void);

#endif
