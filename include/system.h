// ==============================================================================
// SETIN - include/system.h (Unified Core System & Security Definitions)
// ==============================================================================

#ifndef SYSTEM_H
#define SYSTEM_H

// --- CSSIEP Security Privilege Rings ---
typedef enum {
    PRIV_RING0_KERNEL = 0, // Maximum hardware, CPU and VFS control
    PRIV_RING3_USER   = 1  // Restricted application execution space
} SecurityLevel;

#define MAX_VIOLATION_ATTEMPTS 3

// --- System Modules Tracking IDs (Replacing the deleted configure_root) ---
typedef enum {
    MODULE_NETWORK = 0,    // Realtek RTL8139 Driver
    MODULE_PANIC,          // Emergency Rescue Module
    MODULE_CPU,            // GDT/IDT Core Controller
    MAX_SYSTEM_MODULES     // Boundary marker
} SystemModuleID;

// --- Core Kernel Structures ---
struct SystemConfig {
    unsigned int boot_count;
    char username[32];
    char password[32];
    char hostname[32];
    unsigned char setup_done;
};

// --- Core Kernel Functions ---
void safe_print(const char *str);
void kernel_panic(const char *error_message);

// --- CSSIEP Security & Module Management Handlers ---
int cssiep_check_permission(SecurityLevel current_level, SecurityLevel required_level, const char *context_name);

// New function to disable a module directly inside the security engine
int cssiep_shutdown_module(SystemModuleID module_id, SecurityLevel calling_privilege);

// New function for drivers to check if they are still allowed to run
int cssiep_is_module_active(SystemModuleID module_id);

#endif
