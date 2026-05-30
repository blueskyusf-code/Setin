// ==============================================================================
// SETIN - kernel/configure_root.c (Silent Virtual Root Filesystem Initializer)
// ==============================================================================

#include "system.h"

// Custom inline block memory copy for kernel space to avoid external library dependencies
static inline void kernel_memcpy(void *dest, const void *src, unsigned int n) {
    char *d = dest;
    const char *s = src;
    while (n--) {
        *d++ = *s++;
    }
}

// Initializes and populates the root environmental structure in total silence
void configure_root_vfs(struct SystemConfig *target_env, const char *user, const char *pass, const char *host) {
    // Ensure the baseline environment structure is completely cleared
    unsigned char *ptr = (unsigned char *)target_env;
    unsigned int size = sizeof(struct SystemConfig);
    while (size--) {
        *ptr++ = 0;
    }

    // Unpack and transfer setup wizard data into the structural system configuration
    unsigned int user_len = 0;
    while (user[user_len] != '\0' && user_len < 31) {
        target_env->username[user_len] = user[user_len];
        user_len++;
    }
    target_env->username[user_len] = '\0';

    unsigned int pass_len = 0;
    while (pass[pass_len] != '\0' && pass_len < 31) {
        target_env->password[pass_len] = pass[pass_len];
        pass_len++;
    }
    target_env->password[pass_len] = '\0';

    unsigned int host_len = 0;
    while (host[host_len] != '\0' && host_len < 31) {
        target_env->hostname[host_len] = host[host_len];
        host_len++;
    }
    target_env->hostname[host_len] = '\0';

    // Mark system environment deployment states
    target_env->boot_count = 1;
    target_env->setup_done = 1;
    
    // No output logs or visual operations are triggered here (Absolute Silent Mode)
}
