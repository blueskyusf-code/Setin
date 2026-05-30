// ==============================================================================
// SETIN - system.h (Core System Definitions)
// ==============================================================================

#ifndef SYSTEM_H
#define SYSTEM_H

// Safe text output to VGA video memory (Ring 0)
void safe_print(const char *str);

// Shuts down kernel execution safely during unrecoverable errors
void kernel_panic(const char *message);

#endif
