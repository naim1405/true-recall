#include "HotkeyManager.h"
#include <iostream>

// Static pointer for window procedure access
static HotkeyManager* g_hotkeyManager = nullptr;

HotkeyManager::HotkeyManager(MonitorManager* monitorManager, Config* config)
    : m_monitorManager(monitorManager)
    , m_config(config)
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
    
    // Get hotkey config
    HotkeyConfig hotkey = m_config->GetHotkeyConfig();
    
    // Register hotkey for monitor cycling
    if (!RegisterHotKey(m_messageWindow, HOTKEY_CYCLE_MONITOR, hotkey.modifiers, hotkey.vkey)) {
        std::wcerr << L"Failed to register hotkey " << m_config->GetHotkeyString() 
                   << L": " << GetLastError() << std::endl;
        std::wcerr << L"The hotkey may already be in use by another application." << std::endl;
        return false;
    }
    
    std::wcout << L"Hotkey registered: " << m_config->GetHotkeyString() 
               << L" = Cycle Monitor" << std::endl;
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
    
    std::cout << "\nSwitched to Monitor " << m_currentMonitor << std::endl;
    
    // Try to get a valid window from the focus stack
    // If the top window is invalid, try the next one
    HWND targetWindow = nullptr;
    const int MAX_ATTEMPTS = 5;  // Try up to 5 windows from the stack
    
    for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
        targetWindow = m_monitorManager->GetLastFocusedWindow(m_currentMonitor);
        
        if (!targetWindow) {
            break;  // No more windows in stack
        }
        
        // Validate window
        if (IsWindow(targetWindow) && IsWindowVisible(targetWindow) && !IsIconic(targetWindow)) {
            // Found a valid window, try to activate it
            if (ActivateWindow(targetWindow)) {
                return;  // Success!
            }
        }
        
        // Window is invalid or couldn't be activated, remove it and try next
        std::cout << "  Removing invalid/unactivatable window from stack" << std::endl;
        m_monitorManager->RemoveWindowFromStack(m_currentMonitor, targetWindow);
    }
    
    // If we got here, no valid windows were found
    std::cout << "  No valid windows found on Monitor " << m_currentMonitor << std::endl;
    
    // Try to find ANY visible window on this monitor as a fallback
    m_monitorManager->TryFindWindowOnMonitor(m_currentMonitor);
}

bool HotkeyManager::ActivateWindow(HWND hwnd) {
    // Validate window exists
    if (!IsWindow(hwnd)) {
        std::cout << "  Window no longer valid" << std::endl;
        return false;
    }
    
    // Validate window is visible
    if (!IsWindowVisible(hwnd)) {
        std::cout << "  Window not visible" << std::endl;
        return false;
    }
    
    // Don't focus minimized windows
    if (IsIconic(hwnd)) {
        std::cout << "  Window is minimized, restoring first" << std::endl;
        ShowWindow(hwnd, SW_RESTORE);
    }
    
    // Strategy 1: Direct SetForegroundWindow
    if (SetForegroundWindow(hwnd)) {
        std::cout << "  Activated successfully (direct)" << std::endl;
        return true;
    }
    
    // Strategy 2: AttachThreadInput workaround
    HWND currentForeground = GetForegroundWindow();
    if (currentForeground != nullptr) {
        DWORD foregroundThreadId = GetWindowThreadProcessId(currentForeground, nullptr);
        DWORD targetThreadId = GetWindowThreadProcessId(hwnd, nullptr);
        
        if (foregroundThreadId != 0 && targetThreadId != 0 && foregroundThreadId != targetThreadId) {
            if (AttachThreadInput(foregroundThreadId, targetThreadId, TRUE)) {
                SetForegroundWindow(hwnd);
                SetFocus(hwnd);
                AttachThreadInput(foregroundThreadId, targetThreadId, FALSE);
                
                if (GetForegroundWindow() == hwnd) {
                    std::cout << "  Activated successfully (AttachThreadInput)" << std::endl;
                    return true;
                }
            }
        }
    }
    
    // Strategy 3: BringWindowToTop + SetFocus fallback
    BringWindowToTop(hwnd);
    SetFocus(hwnd);
    
    // Final check
    bool success = (GetForegroundWindow() == hwnd);
    if (success) {
        std::cout << "  Activated successfully (fallback)" << std::endl;
    } else {
        std::cout << "  Warning: Could not fully activate window" << std::endl;
    }
    
    return success;
}

LRESULT CALLBACK HotkeyManager::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_HOTKEY && g_hotkeyManager) {
        g_hotkeyManager->HandleHotkey(static_cast<int>(wParam));
        return 0;
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
