#include "MonitorManager.h"
#include <iostream>

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
