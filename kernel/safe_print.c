// ==============================================================================
// SETIN - safe_print.c (Screen Output Engine for Ring 0)
// ==============================================================================

#include "system.h"

// Screen dimensions for standard VGA text mode
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_DEFAULT_COLOR 0x07 // White text on Black background

// Keeps track of the current cursor position
static int cursor_x = 0;
static int cursor_y = 0;

// Direct pointer to the VGA text mode video memory
static char *video_memory = (char *)0xB8000;

void safe_print(const char *str) {
    int i = 0;

    while (str[i] != '\0') {
        // Handle newline character
        if (str[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
        } 
        else {
            // Calculate the memory offset (2 bytes per character: ASCII + Attribute)
            int offset = (cursor_y * VGA_WIDTH + cursor_x) * 2;
            
            video_memory[offset] = str[i];
            video_memory[offset + 1] = VGA_DEFAULT_COLOR;
            
            cursor_x++;
            
            // Automatic line wrapping if column limit is reached
            if (cursor_x >= VGA_WIDTH) {
                cursor_x = 0;
                cursor_y++;
            }
        }

        // Basic screen scroll reset if it reaches the bottom of the screen
        if (cursor_y >= VGA_HEIGHT) {
            cursor_x = 0;
            cursor_y = 0;
        }

        i++;
    }
}
