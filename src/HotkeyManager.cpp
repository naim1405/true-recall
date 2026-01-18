#include "HotkeyManager.h"
#include <iostream>

// Static pointer for window procedure access
static HotkeyManager* g_hotkeyManager = nullptr;

HotkeyManager::HotkeyManager(MonitorManager* monitorManager)
    : m_monitorManager(monitorManager)
    , m_currentMonitor(0)
    , m_messageWindow(nullptr) {
    g_hotkeyManager = this;
}

HotkeyManager::~HotkeyManager() {
    UnregisterHotkeys();
    DestroyMessageWindow();
    g_hotkeyManager = nullptr;
}

bool HotkeyManager::CreateMessageWindow() {
    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "WaybackHotkeyWindow";
    
    if (!RegisterClassEx(&wc)) {
        DWORD error = GetLastError();
        if (error != ERROR_CLASS_ALREADY_EXISTS) {
            std::cerr << "Failed to register window class: " << error << std::endl;
            return false;
        }
    }
    
    // Create message-only window
    m_messageWindow = CreateWindowEx(
        0,
        "WaybackHotkeyWindow",
        "Wayback Hotkey Window",
        0,
        0, 0, 0, 0,
        HWND_MESSAGE,  // Message-only window
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    
    if (!m_messageWindow) {
        std::cerr << "Failed to create message window: " << GetLastError() << std::endl;
        return false;
    }
    
    return true;
}

void HotkeyManager::DestroyMessageWindow() {
    if (m_messageWindow) {
        DestroyWindow(m_messageWindow);
        m_messageWindow = nullptr;
    }
}

bool HotkeyManager::RegisterHotkeys() {
    // Create hidden window for receiving hotkey messages
    if (!CreateMessageWindow()) {
        return false;
    }
    
    // Register Ctrl+Alt+N for monitor cycling
    if (!RegisterHotKey(m_messageWindow, HOTKEY_CYCLE_MONITOR, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'N')) {
        std::cerr << "Failed to register hotkey Ctrl+Alt+N: " << GetLastError() << std::endl;
        return false;
    }
    
    std::cout << "Hotkeys registered: Ctrl+Alt+N = Cycle Monitor" << std::endl;
    return true;
}

void HotkeyManager::UnregisterHotkeys() {
    if (m_messageWindow) {
        UnregisterHotKey(m_messageWindow, HOTKEY_CYCLE_MONITOR);
    }
}

void HotkeyManager::HandleHotkey(int hotkeyId) {
    if (hotkeyId != HOTKEY_CYCLE_MONITOR) {
        return;
    }
    
    int monitorCount = m_monitorManager->GetMonitorCount();
    if (monitorCount == 0) {
        std::cerr << "No monitors detected" << std::endl;
        return;
    }
    
    // Cycle to next monitor
    m_currentMonitor = (m_currentMonitor + 1) % monitorCount;
    
    std::cout << "Switched to Monitor " << m_currentMonitor << std::endl;
    
    // Get last focused window on this monitor
    HWND targetWindow = m_monitorManager->GetLastFocusedWindow(m_currentMonitor);
    
    if (!targetWindow) {
        std::cout << "  No windows on Monitor " << m_currentMonitor << std::endl;
        return;
    }
    
    // Try to activate the window
    if (!ActivateWindow(targetWindow)) {
        std::cout << "  Warning: Failed to activate window 0x" 
                  << std::hex << targetWindow << std::dec << std::endl;
    }
}

bool HotkeyManager::ActivateWindow(HWND hwnd) {
    // Validate window
    if (!IsWindow(hwnd)) {
        std::cout << "  Window no longer valid" << std::endl;
        return false;
    }
    
    if (!IsWindowVisible(hwnd)) {
        std::cout << "  Window not visible" << std::endl;
        return false;
    }
    
    // Try to bring window to foreground
    // First attempt: direct SetForegroundWindow
    if (SetForegroundWindow(hwnd)) {
        return true;
    }
    
    // Second attempt: Use AttachThreadInput workaround
    DWORD foregroundThreadId = GetWindowThreadProcessId(GetForegroundWindow(), nullptr);
    DWORD targetThreadId = GetWindowThreadProcessId(hwnd, nullptr);
    
    if (foregroundThreadId != targetThreadId) {
        AttachThreadInput(foregroundThreadId, targetThreadId, TRUE);
        SetForegroundWindow(hwnd);
        AttachThreadInput(foregroundThreadId, targetThreadId, FALSE);
    }
    
    // Try BringWindowToTop as fallback
    BringWindowToTop(hwnd);
    
    // Check if we succeeded
    return (GetForegroundWindow() == hwnd);
}

LRESULT CALLBACK HotkeyManager::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_HOTKEY && g_hotkeyManager) {
        g_hotkeyManager->HandleHotkey(static_cast<int>(wParam));
        return 0;
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
