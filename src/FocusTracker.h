#pragma once

#include <windows.h>

// Forward declaration
class MonitorManager;

class FocusTracker {
public:
    FocusTracker(MonitorManager* monitorManager);
    ~FocusTracker();

    bool Start();  // Install hook
    void Stop();   // Remove hook

private:
    HWINEVENTHOOK m_hook;
    MonitorManager* m_monitorManager;

    // Static callback for the hook
    static void CALLBACK WinEventProc(
        HWINEVENTHOOK hWinEventHook,
        DWORD event,
        HWND hwnd,
        LONG idObject,
        LONG idChild,
        DWORD idEventThread,
        DWORD dwmsEventTime
    );
};
