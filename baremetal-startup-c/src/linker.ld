OUTPUT_ARCH("riscv")

ENTRY(_enter)

MEMORY
{
    ram (arw!xi) : ORIGIN = 0x80070000, LENGTH = 0x4000
    rom (irx!wa) : ORIGIN = 0x80000000, LENGTH = 0x6a120
}

PHDRS
{
    rom PT_LOAD;
    ram_init PT_LOAD;
    tls PT_TLS;
    ram PT_LOAD;
    text PT_LOAD;
    lim_init PT_LOAD;
}

SECTIONS
{
    __stack_size = 0x400;
    __heap_size = 0x800;

    .init : {
        KEEP (*(.text.metal.init.enter))
    } >rom :rom

    .text : {
        *(.text .text.*)
        . = ALIGN(8);
    } >rom :text

    .data : ALIGN(8) {
        *(.data .data.*)
        . = ALIGN(8);
        PROVIDE( __global_pointer$ = . + 0x800 );
        *(.sdata .sdata.*)
    } >ram AT>rom :ram_init

    PROVIDE( data_source_start = LOADADDR(.data) );
    PROVIDE( data_target_start = ADDR(.data) );
    PROVIDE( data_target_end = ADDR(.data) + SIZEOF(.data) );

    .bss (NOLOAD): ALIGN(8) {
        *(.sbss*)
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(16);
    } >ram :ram

    PROVIDE( bss_target_start = ADDR(.bss) );
    PROVIDE( bss_target_end = ADDR(.bss) + SIZEOF(.bss) );

    .stack (NOLOAD) : ALIGN(16) {
        . += __stack_size; /* Hart 0 */
        PROVIDE( _sp = . );
    } >ram :ram

}
~   
