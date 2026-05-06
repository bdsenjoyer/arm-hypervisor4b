.section ".text"
.global exception_vector_table

.balign 2048
exception_vector_table:
    // Current EL with SP0
    .balign 128
    b hang
    .balign 128
    b hang
    .balign 128
    b hang
    .balign 128
    b hang

    // Current EL with SPx
    .balign 128
    b hang
    .balign 128
    b hang
    .balign 128
    b hang
    .balign 128
    b hang

    // Lower EL (AArch64)
    .balign 128
    b sync_from_vm
    .balign 128
    b irq_from_vm
    .balign 128
    b fiq_from_vm
    .balign 128
    b serror_from_vm

    // Lower EL (AArch32)
    .balign 128
    b hang
    .balign 128
    b hang
    .balign 128
    b hang
    .balign 128
    b hang

hang:
    wfe
    b hang

sync_from_vm:
    // Set flag immediately
    ldr x0, =exception_flag
    mov x1, #0xBEEF
    str x1, [x0]
    
    // Save minimal context
    stp x0, x1, [sp, #-16]!
    stp x2, x3, [sp, #-16]!
    stp x29, x30, [sp, #-16]!
    
    // Check ESR
    mrs x1, ESR_EL2
    lsr x2, x1, #26
    cmp x2, #0x16
    b.ne not_hvc
    
    // Call hypercall handler
    bl hypercall_handler
    
    // Restore context
    ldp x29, x30, [sp], #16
    ldp x2, x3, [sp], #16
    ldp x0, x1, [sp], #16
    
    eret

not_hvc:
    ldp x29, x30, [sp], #16
    ldp x2, x3, [sp], #16
    ldp x0, x1, [sp], #16
    eret

irq_from_vm:
    eret

fiq_from_vm:
    eret

serror_from_vm:
    eret

.section ".data"
.global exception_flag
exception_flag:
    .quad 0
