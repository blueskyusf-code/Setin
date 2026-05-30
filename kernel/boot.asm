; ==============================================================================
; SETIN - boot.asm (The Beginning of Everything)
; ==============================================================================

[bits 16]           ; Real Mode (16-bit processor execution)
[org 0x7c00]        ; Default memory address where BIOS loads the bootloader

; Entry points and constants
KERNEL_OFFSET equ 0x1000 ; Memory address where main.c will be loaded

jmp boot_start

; ------------------------------------------------------------------------------
; GDT (Global Descriptor Table) - Required for Ring 0 and Ring 3 execution
; ------------------------------------------------------------------------------
gdt_start:
    dd 0x0          ; Mandatory null descriptor (8 bytes)
    dd 0x0

gdt_code:           ; Kernel Code Segment (Ring 0)
    dw 0xffff       ; Limit (0-15 bits)
    dw 0x0          ; Base (0-15 bits)
    db 0x0          ; Base (16-23 bits)
    db 10011010b    ; Access rights (Kernel, Executable, Readable)
    db 11001111b    ; Granularity
    db 0x0          ; Base (24-31 bits)

gdt_data:           ; Kernel Data Segment (Ring 0)
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b    ; Access rights (Kernel, Data, Writable)
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; GDT size
    dd gdt_start               ; GDT start address

; Constants for GDT segment selectors
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start


; ------------------------------------------------------------------------------
; 16-Bit Initialization Code
; ------------------------------------------------------------------------------
boot_start:
    ; Setup initial segment registers
    cli             ; Disable interrupts temporarily
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00  ; Set stack pointer below bootloader code
    sti             ; Re-enable interrupts

    ; Note: Kernel disk-reading routine (BIOS int 0x13) should be placed here
    ; to load the kernel payload into KERNEL_OFFSET.

    ; Transition to Protected Mode (32-bit)
    cli
    lgdt [gdt_descriptor]   ; Load the GDT descriptor into the CPU
    
    mov eax, cr0
    or eax, 0x1             ; Enable Protected Mode bit in CR0
    mov cr0, eax

    jmp CODE_SEG:init_pm    ; Far jump to flush 16-bit prefetch queue


; ------------------------------------------------------------------------------
; Protected Mode Initialization (32-bit)
; ------------------------------------------------------------------------------
[bits 32]
init_pm:
    ; Update segment registers with Kernel Data Selector
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Setup stack for 32-bit operation
    mov ebp, 0x90000
    mov esp, ebp

    ; Transfer control to the C kernel entry point
    extern kernel_main
    call kernel_main

    ; Infinite loop in case kernel_main unexpectedly returns
    cli
halt_loop:
    hlt
    jmp halt_loop


; ------------------------------------------------------------------------------
; Boot Signature
; ------------------------------------------------------------------------------
times 510-($-$$) db 0   ; Pad remaining bytes to 510 with zeros
dw 0xaa55               ; Standard boot signature (magic bytes for BIOS)
