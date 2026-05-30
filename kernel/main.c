#include <system.h>

// Helper function to read a byte from a hardware I/O port
static inline unsigned char inportb(unsigned short port) {
    unsigned char result;
    // Inline Assembly to read from the specified hardware port
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}
static inline void outportl(unsigned short port, unsigned int value) {
    __asm__ __volatile__("outl %0, %1" : : "a"(value), "Nd"(port));
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
// Blocks execution until any key is pressed
void wait_for_keypress() {
    unsigned char status;
    unsigned char keycode;

    while (1) {
        status = inportb(0x64);
        if (status & 0x01) {
            keycode = inportb(0x60);
            if (keycode > 0) {
                break;
            }
        }
    }
}

// Scans the PCI bus for a specific Vendor ID and Device ID
int check_pci_device(unsigned short vendor_id, unsigned short device_id) {
    // Loop through 32 possible slots on the PCI bus
    for (int slot = 0; slot < 32; slot++) {
        // Build the target address according to standard PCI configuration space mapping
        // Bit 31: Enable bit, Bits 23-16: Bus 0, Bits 15-11: Slot, Bits 10-8: Function 0
        unsigned int address = (unsigned int)((slot << 11) | 0x80000000);

        // Config Address Port is 0xCF8
        outportl(0xCF8, address);
        
        // Config Data Port is 0xCFC. Reading here returns Vendor ID (low 16 bits) and Device ID (high 16 bits)
        unsigned int pci_data = inportl(0xCFC);

        // Extract IDs from the 32-bit response
        unsigned short current_vendor = (unsigned short)(pci_data & 0xFFFF);
        unsigned short current_device = (unsigned short)(pci_data >> 16);

        // If it matches what we are searching for, hardware is present!
        if (current_vendor == vendor_id && current_device == device_id) {
            return 1; 
        }
    }
    return 0; // Device not found on the bus
}
void kernel_main() {
  // Print Boot
  safe_print("Setin\n");
  safe_print("Criado por The Blu3sky\n");
  safe_print("[Press any key to init]");
  wait_for_keypress();

  int network_hardware_found = check_pci_device(0x10EC, 0x8139);
  if (!network_hardware_found) {
      kernel_panic("NHI: RTL8139 card not detected");
  }  
  // Case Press Enter and not network error, Continue
  
  while (1) {
    __asm__ __volatile__("hlt");
  }
}
