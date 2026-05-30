#include <system.h>
#include <cpu.h>

void kernel_panic(const char *message) {
  cpu_disable_interrupts();
  // Print Panic Message
  safe_print("KERNEL PANIC\n");
  safe_print("Details: ");
  safe_print(message);
  safe_print("\n");

  // Enter to Secure Mode
  while (1) {
     __asm__("cli; hlt");
  }
}
