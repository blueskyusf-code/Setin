// ==============================================================================
// SETIN - kernel/cssiep.c (CSSIEP Engine with Module Lifecycle Control)
// ==============================================================================

#include "system.h"

// Global internal static tracker to monitor repetitive unauthorized access
static unsigned int consecutive_violations = 0;

// Dynamic hardware state bitmap (1 = Active/Enabled, 0 = Disabled/Shutdown)
// All modules start enabled (1) by default on boot execution
static unsigned char module_states[MAX_SYSTEM_MODULES] = {1, 1, 1};

int cssiep_check_permission(SecurityLevel current_level, SecurityLevel required_level, const char *context_name) {
    
    // 1. AUTHORIZED ACCESS CHECK
    if (current_level <= required_level) {
        consecutive_violations = 0;
        return 1; 
    }

    // 2. SECURITY BREACH DETECTED
    consecutive_violations++;

    safe_print("Error: Denial Permission");
    
    // 3. PERSISTENCE ENFORCEMENT BLOCK
    if (consecutive_violations >= MAX_VIOLATION_ATTEMPTS) {
        kernel_panic("CSSIEP: Persist Permission");
    }

    // 4. INSTANT RING 0 BREACH PROTECTION
    if (required_level == PRIV_RING0_KERNEL) {
        kernel_panic("CSSIEP: Unauthorized Ring 0 hardware access attempt.");
    }

    return 0; // Access Denied
}

/**
 * Turns off a specific system module registry directly inside the security layer.
 * @param module_id         The target enum ID of the module to shutdown.
 * @param calling_privilege The security ring level of the execution caller.
 * @return int              Returns 1 on success, 0 if denied or invalid.
 */
int cssiep_shutdown_module(SystemModuleID module_id, SecurityLevel calling_privilege) {
    
    // Safety check for boundary index corruption
    if (module_id >= MAX_SYSTEM_MODULES) {
        return 0;
    }

    // Secure the operation: Only Ring 0 Kernel can disable core architecture blocks
    if (!cssiep_check_permission(calling_privilege, PRIV_RING0_KERNEL, "CSSIEP_Module_Shutdown")) {
        return 0; // Blocked by security constraints
    }

    // Silently disable the target infrastructure driver state
    module_states[module_id] = 0;
    return 1;
}

/**
 * Read-only status verification helper for core drivers.
 */
int cssiep_is_module_active(SystemModuleID module_id) {
    if (module_id >= MAX_SYSTEM_MODULES) {
        return 0; // Out of bounds components are considered dead
    }
    return module_states[module_id];
}
