.section ".text.boot"
.global _start

_start:
    // Налаштування стеку (без перевірки EL)
    ldr     x0, =stack_top
    mov     sp, x0
    
    // Перехід до C коду
    bl      hypervisor_main
    
hang:
    wfe
    b       hang

// Стек (8KB)
.section ".bss"
.align 12
stack_bottom:
    .space 8192
stack_top:
