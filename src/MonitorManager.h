#pragma once

#include <windows.h>
#include <vector>
#include <map>

class MonitorManager {
public:
    MonitorManager();
    
    void EnumerateMonitors();  // Detect connected monitors
    int GetMonitorCount() const;
    int GetMonitorIndexForWindow(HWND hwnd) const;  // Which monitor is this window on?
    
    // Focus stack management
    void OnWindowFocused(HWND hwnd);  // Called when window gets focus
    HWND GetLastFocusedWindow(int monitorIndex) const;  // Top of stack
    void PrintFocusStacks() const;  // Debug output
    
    // For debugging
    void PrintMonitorInfo() const;

private:
    std::vector<HMONITOR> m_monitors;
    
    // Per-monitor focus stacks (monitor index -> vector of HWNDs, most recent first)
    std::map<int, std::vector<HWND>> m_focusStacks;
    
    static const size_t MAX_STACK_SIZE = 10;  // Limit stack size per monitor
    
    // Callback for EnumDisplayMonitors
    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
};
