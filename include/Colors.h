#ifndef COLORS_H
#define COLORS_H

#include <iostream>
#include <string>
using namespace std;

// ANSI Color Codes for Windows and Unix-like systems
namespace Colors {
    // Reset
    const string RESET = "\033[0m";
    
    // Regular Colors
    const string BLACK = "\033[0;30m";
    const string RED = "\033[0;31m";
    const string GREEN = "\033[0;32m";
    const string YELLOW = "\033[0;33m";
    const string BLUE = "\033[0;34m";
    const string MAGENTA = "\033[0;35m";
    const string CYAN = "\033[0;36m";
    const string WHITE = "\033[0;37m";
    
    // Bold Colors
    const string BOLD_BLACK = "\033[1;30m";
    const string BOLD_RED = "\033[1;31m";
    const string BOLD_GREEN = "\033[1;32m";
    const string BOLD_YELLOW = "\033[1;33m";
    const string BOLD_BLUE = "\033[1;34m";
    const string BOLD_MAGENTA = "\033[1;35m";
    const string BOLD_CYAN = "\033[1;36m";
    const string BOLD_WHITE = "\033[1;37m";
    
    // Background Colors
    const string BG_BLACK = "\033[40m";
    const string BG_RED = "\033[41m";
    const string BG_GREEN = "\033[42m";
    const string BG_YELLOW = "\033[43m";
    const string BG_BLUE = "\033[44m";
    const string BG_MAGENTA = "\033[45m";
    const string BG_CYAN = "\033[46m";
    const string BG_WHITE = "\033[47m";
    
    // Professional Theme Colors
    namespace Theme {
        const string HEADER = BOLD_CYAN;           // Cyan for headers
        const string SUCCESS = BOLD_GREEN;         // Green for success messages
        const string WARNING = BOLD_YELLOW;       // Yellow for warnings
        const string ERR = BOLD_RED;               // Red for errors (ERR instead of ERROR to avoid Windows macro)
        const string INFO = BOLD_BLUE;             // Blue for info
        const string DATA = BOLD_WHITE;            // White for data
        const string MENU_ITEM = CYAN;             // Cyan for menu items
        const string PROMPT = YELLOW;              // Yellow for prompts
        const string URGENT = BOLD_RED;            // Red for urgent items
        const string SEPARATOR = MAGENTA;          // Magenta for separators
    }
}

// Helper function to enable ANSI colors on Windows
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
inline void enableColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
}
#else
inline void enableColors() {
    // Colors work by default on Unix-like systems
}
#endif

#endif // COLORS_H

