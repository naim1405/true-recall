#include <windows.h>
#include <iostream>
#include "FocusTracker.h"
#include "MonitorManager.h"
#include "HotkeyManager.h"
#include "TrayIcon.h"
#include "Config.h"

// Global flag for clean shutdown
volatile bool g_running = true;

// Global monitor manager pointer for callback access
MonitorManager* g_monitorManager = nullptr;

// Global tray icon
TrayIcon* g_trayIcon = nullptr;

// Main window handle
HWND g_mainWindow = nullptr;

// Console control handler for Ctrl+C
BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType) {
    if (dwCtrlType == CTRL_C_EVENT || dwCtrlType == CTRL_CLOSE_EVENT) {
        std::cout << "\nShutting down..." << std::endl;
        g_running = false;
        return TRUE;
    }
    return FALSE;
}

// Window procedure for main window (handles tray icon messages)
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // Let TrayIcon handle its messages
    LRESULT result = TrayIcon::WndProc(hwnd, msg, wParam, lParam);
    
    if (msg == WM_QUIT || msg == WM_DESTROY) {
        g_running = false;
        PostQuitMessage(0);
        return 0;
    }
    
    return result;
}

bool CreateMainWindow() {
    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "TrueRecallMainWindow";
    
    if (!RegisterClassEx(&wc)) {
        DWORD error = GetLastError();
        if (error != ERROR_CLASS_ALREADY_EXISTS) {
            std::cerr << "Failed to register main window class: " << error << std::endl;
            return false;
        }
    }
    
    // Create message-only window
    g_mainWindow = CreateWindowEx(
        0,
        "TrueRecallMainWindow",
        "True Recall",
        0,
        0, 0, 0, 0,
        HWND_MESSAGE,  // Message-only window
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    
    if (!g_mainWindow) {
        std::cerr << "Failed to create main window: " << GetLastError() << std::endl;
        return false;
    }
    
    return true;
}

int main() {
    // Allocate console for debugging (can be removed for silent operation)
    #ifdef _DEBUG
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    #endif

    // Set up console control handler
    if (!SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE)) {
        std::cerr << "Failed to set console control handler" << std::endl;
        return 1;
    }

    std::cout << "True Recall started." << std::endl;
    
    // Create main window for tray icon
    if (!CreateMainWindow()) {
        return 1;
    }
    
    // Load configuration
    Config config;
    if (!config.Load()) {
        std::cerr << "Failed to load configuration" << std::endl;
        return 1;
    }

    // Create and enumerate monitors
    MonitorManager monitorManager;
    g_monitorManager = &monitorManager;
    monitorManager.EnumerateMonitors();
    monitorManager.PrintMonitorInfo();

    // Create and start focus tracker
    FocusTracker tracker(&monitorManager);
    if (!tracker.Start()) {
        std::cerr << "Failed to start focus tracker" << std::endl;
        return 1;
    }

    // Create and register hotkeys
    HotkeyManager hotkeyManager(&monitorManager, &config);
    if (!hotkeyManager.RegisterHotkeys()) {
        std::cerr << "Failed to register hotkeys" << std::endl;
        return 1;
    }
    
    // Create system tray icon
    TrayIcon trayIcon;
    g_trayIcon = &trayIcon;
    if (!trayIcon.Create(g_mainWindow)) {
        std::cerr << "Failed to create tray icon" << std::endl;
        // Continue anyway, not critical
    }

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

    // Clean shutdown
    std::cout << "\nCleaning up..." << std::endl;
    
    // Destroy tray icon
    trayIcon.Destroy();
    
    // Stop focus tracker (unhooks events)
    tracker.Stop();
    
    // Unregister hotkeys
    hotkeyManager.UnregisterHotkeys();
    
    std::cout << "True Recall terminated cleanly." << std::endl;
    
    #ifdef _DEBUG
    FreeConsole();
    #endif
    
    return 0;
}
