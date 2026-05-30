// ==============================================================================
// SETIN - system.h (Core System Definitions)
// ==============================================================================

#ifndef SYSTEM_H
#define SYSTEM_H

// Safe text output to VGA video memory (Ring 0)
void safe_print(const char *str);

// Shuts down kernel execution safely during unrecoverable errors
void kernel_panic(const char *message);

// Verify Initialize
struct SystemConfig {
    unsigned int boot_count;     // Count System initialize
    char username[32];           // Name of User
    char hostname[32];           // Name of Machine (Example: Setin)
    unsigned char setup_done;    // 0 = Require Setup Wizard, 1 = Comum initialize
};

#endif
