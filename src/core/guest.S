.section ".text.guest"
.global guest_start

.balign 4096
guest_start:
    // Just set a flag
    ldr x0, =guest_alive_flag
    mov x1, #0xABCD
    str x1, [x0]
    
    // Loop forever
loop:
    b loop

.section ".data"
.global guest_alive_flag
guest_alive_flag:
    .quad 0
