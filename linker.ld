/* ==============================================================================
   SETIN - linker.ld (Linker Script for x86 Kernel)
   ============================================================================== */

ENTRY(kernel_main)
OUTPUT_FORMAT("binary")

SECTIONS
{
    /* O Bootloader precisa ser carregado estritamente no endereço mágico 0x7C00 */
    . = 0x7C00;

    .boot :
    {
        *(.boot)
    }

    /* O restante do Kernel (código C) será alocado logo em seguida */
    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(.bss)
    }
}
