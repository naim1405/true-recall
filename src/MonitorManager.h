#pragma once

#include <windows.h>
#include <vector>

class MonitorManager {
public:
    MonitorManager();
    
    void EnumerateMonitors();  // Detect connected monitors
    int GetMonitorCount() const;
    int GetMonitorIndexForWindow(HWND hwnd) const;  // Which monitor is this window on?
    
    // For debugging
    void PrintMonitorInfo() const;

private:
    std::vector<HMONITOR> m_monitors;
    
    // Callback for EnumDisplayMonitors
    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
};
