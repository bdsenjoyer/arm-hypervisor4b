#ifndef HYPERCALL_H
#define HYPERCALL_H

#include <stdint.h>

#define HVC_VERSION       0
#define HVC_VM_STATS      1
#define HVC_VM_INFO       2
#define HVC_DEBUG_PRINT   3
#define HVC_GET_TIME      4   
#define HVC_GET_CPU_ID    5   
#define HVC_MEM_INFO      6   
#define HVC_EXIT          0xFF

uint64_t hypercall_handler(uint64_t hvc_num, uint64_t arg1, uint64_t arg2);
void show_hypercall_stats(void);

#endif
