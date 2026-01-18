#include "MonitorManager.h"
#include <iostream>
#include <algorithm>

MonitorManager::MonitorManager() {
}

void MonitorManager::EnumerateMonitors() {
    m_monitors.clear();
    
    // Use EnumDisplayMonitors to detect all monitors
    EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(this));
    
    std::cout << "Detected " << m_monitors.size() << " monitor(s)" << std::endl;
}

int MonitorManager::GetMonitorCount() const {
    return static_cast<int>(m_monitors.size());
}

int MonitorManager::GetMonitorIndexForWindow(HWND hwnd) const {
    if (hwnd == nullptr || !IsWindow(hwnd)) {
        return -1;
    }
    
    // Get the monitor this window is on
    HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    
    // Find the index of this monitor in our list
    for (size_t i = 0; i < m_monitors.size(); ++i) {
        if (m_monitors[i] == hMonitor) {
            return static_cast<int>(i);
        }
    }
    
    return -1;  // Not found (shouldn't happen with MONITOR_DEFAULTTONEAREST)
}

void MonitorManager::PrintMonitorInfo() const {
    for (size_t i = 0; i < m_monitors.size(); ++i) {
        MONITORINFO info = {};
        info.cbSize = sizeof(MONITORINFO);
        
        if (GetMonitorInfo(m_monitors[i], &info)) {
            std::cout << "Monitor " << i << ": "
                      << "Rect=[" << info.rcMonitor.left << "," << info.rcMonitor.top << ","
                      << info.rcMonitor.right << "," << info.rcMonitor.bottom << "]";
            
            if (info.dwFlags & MONITORINFOF_PRIMARY) {
                std::cout << " (Primary)";
            }
            
            std::cout << std::endl;
        }
    }
}

BOOL CALLBACK MonitorManager::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
    MonitorManager* pThis = reinterpret_cast<MonitorManager*>(dwData);
    pThis->m_monitors.push_back(hMonitor);
    return TRUE;  // Continue enumeration
}

void MonitorManager::OnWindowFocused(HWND hwnd) {
    if (hwnd == nullptr || !IsWindow(hwnd)) {
        return;
    }
    
    // Determine which monitor this window is on
    int monitorIndex = GetMonitorIndexForWindow(hwnd);
    if (monitorIndex < 0) {
        return;  // Invalid monitor
    }
    
    // Get or create the focus stack for this monitor
    std::vector<HWND>& stack = m_focusStacks[monitorIndex];
    
    // Remove hwnd if it's already in the stack (avoid duplicates)
    auto it = std::find(stack.begin(), stack.end(), hwnd);
    if (it != stack.end()) {
        stack.erase(it);
    }
    
    // Add hwnd to the front (most recent)
    stack.insert(stack.begin(), hwnd);
    
    // Limit stack size
    if (stack.size() > MAX_STACK_SIZE) {
        stack.resize(MAX_STACK_SIZE);
    }
}

HWND MonitorManager::GetLastFocusedWindow(int monitorIndex) const {
    auto it = m_focusStacks.find(monitorIndex);
    if (it == m_focusStacks.end() || it->second.empty()) {
        return nullptr;
    }
    
    // Return the first (most recent) window
    return it->second[0];
}

void MonitorManager::RemoveWindowFromStack(int monitorIndex, HWND hwnd) {
    auto it = m_focusStacks.find(monitorIndex);
    if (it == m_focusStacks.end()) {
        return;
    }
    
    std::vector<HWND>& stack = it->second;
    auto windowIt = std::find(stack.begin(), stack.end(), hwnd);
    
    if (windowIt != stack.end()) {
        stack.erase(windowIt);
        std::cout << "  Removed window 0x" << std::hex << hwnd << std::dec 
                  << " from Monitor " << monitorIndex << " stack" << std::endl;
    }
}

void MonitorManager::RemoveWindowFromAllStacks(HWND hwnd) {
    for (auto& pair : m_focusStacks) {
        std::vector<HWND>& stack = pair.second;
        auto it = std::find(stack.begin(), stack.end(), hwnd);
        
        if (it != stack.end()) {
            stack.erase(it);
            std::cout << "  Removed destroyed window 0x" << std::hex << hwnd << std::dec 
                      << " from Monitor " << pair.first << " stack" << std::endl;
        }
    }
}

// Helper struct for EnumWindows callback
struct FindWindowData {
    HMONITOR targetMonitor;
    HWND foundWindow;
};

static BOOL CALLBACK FindWindowOnMonitorProc(HWND hwnd, LPARAM lParam) {
    FindWindowData* data = reinterpret_cast<FindWindowData*>(lParam);
    
    // Check if window is visible and not minimized
    if (!IsWindowVisible(hwnd) || IsIconic(hwnd)) {
        return TRUE;  // Continue enumeration
    }
    
    // Check if window has a title (skip invisible/system windows)
    wchar_t title[256];
    if (GetWindowTextW(hwnd, title, sizeof(title) / sizeof(title[0])) == 0) {
        return TRUE;  // Continue enumeration
    }
    
    // Check if window is on the target monitor
    HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    if (hMonitor == data->targetMonitor) {
        data->foundWindow = hwnd;
        return FALSE;  // Stop enumeration, found a window
    }
    
    return TRUE;  // Continue enumeration
}

void MonitorManager::TryFindWindowOnMonitor(int monitorIndex) {
    if (monitorIndex < 0 || monitorIndex >= static_cast<int>(m_monitors.size())) {
        return;
    }
    
    FindWindowData data;
    data.targetMonitor = m_monitors[monitorIndex];
    data.foundWindow = nullptr;
    
    // Enumerate all top-level windows
    EnumWindows(FindWindowOnMonitorProc, reinterpret_cast<LPARAM>(&data));
    
    if (data.foundWindow) {
        std::cout << "  Found window on monitor, attempting to focus..." << std::endl;
        SetForegroundWindow(data.foundWindow);
    }
}

void MonitorManager::PrintFocusStacks() const {
    std::cout << "\n--- Focus Stacks ---" << std::endl;
    
    for (const auto& pair : m_focusStacks) {
        int monitorIndex = pair.first;
        const std::vector<HWND>& stack = pair.second;
        
        std::cout << "Monitor " << monitorIndex << ": ";
        
        if (stack.empty()) {
            std::cout << "(empty)";
        } else {
            for (size_t i = 0; i < stack.size(); ++i) {
                HWND hwnd = stack[i];
                
                // Verify window is still valid
                if (!IsWindow(hwnd)) {
                    continue;  // Skip invalid windows
                }
                
                // Get window title
                wchar_t title[256] = L"";
                int titleLength = GetWindowTextW(hwnd, title, sizeof(title) / sizeof(title[0]));
                
                if (i > 0) std::cout << " ";
                
                // Always show HWND, optionally show title if available
                std::cout << "[0x" << std::hex << reinterpret_cast<uintptr_t>(hwnd) << std::dec;
                
                if (titleLength > 0) {
                    std::wcout << L": " << title << L"]";
                } else {
                    std::cout << "]";
                }
            }
        }
        
        std::cout << std::endl;
    }
    
    std::cout << "--------------------\n" << std::endl;
}
