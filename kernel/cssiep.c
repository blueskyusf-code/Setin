// ==============================================================================
// SETIN - kernel/cssiep.c (Control Secure System in Error Permission Engine)
// ==============================================================================

#include "system.h"

// Global internal static tracker to monitor repetitive unauthorized access
static unsigned int consecutive_violations = 0;

int cssiep_check_permission(SecurityLevel current_level, SecurityLevel required_level, const char *context_name) {
    
    // 1. AUTHORIZED ACCESS CHECK
    if (current_level <= required_level) {
        // Clear history of failures immediately upon an authorized call
        consecutive_violations = 0;
        return 1; 
    }

    // 2. SECURITY BREACH DETECTED
    consecutive_violations++;

    // Standard permission denial output using pure kernel safe_print
    safe_print("Error: Denial Permission (41)\n");
  
    // 3. PERSISTENCE ENFORCEMENT BLOCK
    if (consecutive_violations >= MAX_VIOLATION_ATTEMPTS) {
        kernel_panic("CSSIEP: Persist Permission");
    }

    // 4. INSTANT RING 0 BREACH PROTECTION
    if (required_level == PRIV_RING0_KERNEL) {
        kernel_panic("CSSIEP: Unauthorized Ring 0 hardware access attempt.");
    }

    return 0; // Access Denied for this specific instance
}
