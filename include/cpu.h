// ==============================================================================
// SETIN - include/cpu.h (Low-Level CPU Control & Architecture Definitions)
// ==============================================================================

#ifndef CPU_H
#define CPU_H

// Structure pointing to the GDT descriptor (Required by LGDT assembly instruction)
struct GDTDescriptor {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// Structure pointing to the IDT descriptor (Required by LIDT assembly instruction)
struct IDTDescriptor {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// --- CPU Lifecycle & Execution Control ---

// Disables hardware interrupts on the current CPU core (CLI instruction wrapper)
void cpu_disable_interrupts(void);

// Enables hardware interrupts on the current CPU core (STI instruction wrapper)
void cpu_enable_interrupts(void);

// Halts the CPU execution until the next hardware interrupt occurs (HLT wrapper)
void cpu_halt(void);

// --- Architecture Tables Initialization ---

// Initializes the Global Descriptor Table to secure kernel segments
void cpu_init_gdt(void);

// Initializes the Interrupt Descriptor Table to hook processor exceptions
void cpu_init_idt(void);

#endif
