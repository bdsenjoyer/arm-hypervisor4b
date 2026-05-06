ENTRY(_start)

SECTIONS
{
    . = 0x40000000;
    
_text_start = .;
.text : {
    *(.text.boot)
    *(.text.guest)  /* ← ДОДАЙ ЯВНО ПЕРЕД *(.text*) */
    *(.text*)
    KEEP(*(.text.guest))  /* ← І ДОДАЙ KEEP */
}
_text_end = .;
    
    _rodata_start = .;
    .rodata : {
        *(.rodata*)
    }
    _rodata_end = .;
    
    _data_start = .;
    .data : {
        *(.data*)
    }
    _data_end = .;
    
    _bss_start = .;
    .bss : {
        *(.bss*)
    }
    _bss_end = .;
}
