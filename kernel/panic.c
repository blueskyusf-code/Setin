#include "system.h"
#include "safe_print.c"
#include "cpu.h"

void kernel_panic(const char *message) {
  cpu_disable_interrupts();
  cssiep_shutdown_all_modules();
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
