// ==============================================================================
// SETIN - kernel/cpu.c (Low-Level CPU & Register Assembly Implementations)
// ==============================================================================

#include "system.h"
#include "cpu.h"

static struct GDTDescriptor gdt_desc;
static unsigned long long gdt_entries[3];

static struct IDTDescriptor idt_desc;
static unsigned long long idt_entries[256];

void cpu_disable_interrupts(void) {
    __asm__ __volatile__("cli");
}

void cpu_enable_interrupts(void) {
    __asm__ __volatile__("sti");
}

void cpu_halt(void) {
    __asm__ __volatile__("hlt");
}

void cpu_init_gdt(void) {
    gdt_entries[0] = 0;
    gdt_entries[1] = 0x00CF9A000000FFFFULL;
    gdt_entries[2] = 0x00CF92000000FFFFULL;

    gdt_desc.limit = (sizeof(unsigned long long) * 3) - 1;
    gdt_desc.base  = (unsigned int)&gdt_entries;

    __asm__ __volatile__(
        "lgdt %0\n\t"
        "ljmp $0x08, $.reload_segments\n\t"
        ".reload_segments:\n\t"
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        : : "m"(gdt_desc) : "ax"
    );
}

void cpu_init_idt(void) {
    unsigned char *ptr = (unsigned char *)idt_entries;
    unsigned int size = sizeof(idt_entries);
    while (size--) {
        *ptr++ = 0;
    }

    idt_desc.limit = (sizeof(unsigned long long) * 256) - 1;
    idt_desc.base  = (unsigned int)&idt_entries;

    __asm__ __volatile__("lidt %0" : : "m"(idt_desc));
}
