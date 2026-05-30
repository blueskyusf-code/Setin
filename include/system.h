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

// --- Core Kernel Structures ---
struct SystemConfig {
    unsigned int boot_count;
    char username[32];
    char password[32];
    char hostname[32];
    unsigned char setup_done;
};

struct VFSNode {
    char path[64];
    void (*handler_ptr)(struct SystemConfig *);
    unsigned char is_directory;
    unsigned char exists;
};

// --- Core Kernel Functions ---
void safe_print(const char *str);
void kernel_panic(const char *message);
void deploy_system_root_directories(struct SystemConfig *target_env);

// --- CSSIEP Security Handler ---
int cssiep_check_permission(SecurityLevel current_level, SecurityLevel required_level, const char *context_name);

#endif
