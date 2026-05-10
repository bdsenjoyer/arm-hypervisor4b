#include "guest.h"

static task_t         tasks[MAX_TASKS];
static int            task_count = 0;
static int            current    = -1;

void sched_init(void) {
    task_count = 0;
    current    = -1;
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].state = TASK_DONE;
        tasks[i].runs  = 0;
    }
}

void sched_add_task(task_fn_t fn, const char *name) {
    if (task_count >= MAX_TASKS) return;
    task_t *t = &tasks[task_count];
    t->entry  = fn;
    t->state  = TASK_READY;
    t->name   = name;
    t->runs   = 0;
    t->ctx.sp = (uint64_t)(t->stack + TASK_STACK_SIZE) & ~0xFULL;
    t->ctx.pc = (uint64_t)fn;
    task_count++;
}

static int sched_next(void) {
    int start = (current + 1) % task_count;
    for (int i = 0; i < task_count; i++) {
        int idx = (start + i) % task_count;
        if (tasks[idx].state == TASK_READY) return idx;
    }
    return -1;
}

void sched_run(void) { (void)sched_next(); }
void sched_yield(void) {}
void sched_task_done(void) {}
