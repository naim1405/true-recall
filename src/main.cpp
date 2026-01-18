#include <windows.h>
#include <iostream>

// Global flag for clean shutdown
volatile bool g_running = true;

// Console control handler for Ctrl+C
BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType) {
    if (dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_CLOSE_EVENT) {
        std::cout << "\nShutting down..." << std::endl;
        g_running = false;
        return TRUE;
    }
    return FALSE;
}

int main() {
    // Set up console control handler
    if (!SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE)) {
        std::cerr << "Failed to set console control handler" << std::endl;
        return 1;
    }

    std::cout << "Wayback started. Press Ctrl+C to exit." << std::endl;

    // Win32 message loop
    MSG msg = {};
    while (g_running) {
        // Use PeekMessage with timeout to allow checking g_running flag
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            // Sleep briefly to avoid busy-waiting
            Sleep(10);
        }
    }

    std::cout << "Wayback terminated." << std::endl;
    return 0;
}
