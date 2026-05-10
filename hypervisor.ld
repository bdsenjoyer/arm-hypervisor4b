ENTRY(_start)
SECTIONS
{
    . = 0x40000000;
    _text_start = .;
    .text : {
        *(.text.boot)
        *(.text*)
    }
    _text_end = .;

    _rodata_start = .;
    .rodata : { *(.rodata*) }
    _rodata_end = .;

    _data_start = .;
    .data : { *(.data*) }
    _data_end = .;

    _bss_start = .;
    .bss : { *(.bss*) }
    _bss_end = .;

    . = 0x40200000;
    .guest_code : { *(.guest_code) }

    .guest_bss (NOLOAD) : {
        __guest_bss_start = .;
        *(.guest_bss)
        __guest_bss_end = .;
    }

    .guest_stack (NOLOAD) : {
        . = ALIGN(16);
        . = . + 4096;
        __guest_stack_top = .;
    }
}
