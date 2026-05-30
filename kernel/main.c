#include <system.h>

// Helper function to read a byte from a hardware I/O port
static inline unsigned char inportb(unsigned short port) {
    unsigned char result;
    // Inline Assembly to read from the specified hardware port
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Function that blocks execution until any key is pressed
void wait_for_keypress() {
    unsigned char status;
    unsigned char keycode;

    // Read the current status of the keyboard controller
    // Port 0x64 is the Status Register for the PS/2 controller
    while (1) {
        status = inportb(0x64);
        
        // Check if the lowest bit (0) is 1, meaning data is ready to be read
        if (status & 0x01) {
            // Port 0x60 is the Data Register where the key scancode lives
            keycode = inportb(0x60);
            
            // If keycode is greater than 0, a key event happened!
            // We break the loop to continue boot execution
            if (keycode > 0) {
                break;
            }
        }
    }
}

void kernel_main() {
  // Print Boot
  safe_print("Setin\n");
  safe_print("Criado por The Blu3sky\n");
  safe_print("[Press any key to init]");
  wait_for_keypress();

  // Case Press Enter, Continue
  
  while (1) {
    __asm__ __volatile__("hlt");
  }
}
