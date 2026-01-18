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
    HWINEVENTHOOK m_focusHook;
    HWINEVENTHOOK m_destroyHook;
    MonitorManager* m_monitorManager;

    // Static callback for focus events
    static void CALLBACK FocusEventProc(
        HWINEVENTHOOK hWinEventHook,
        DWORD event,
        HWND hwnd,
        LONG idObject,
        LONG idChild,
        DWORD idEventThread,
        DWORD dwmsEventTime
    );
    
    // Static callback for window destruction
    static void CALLBACK DestroyEventProc(
        HWINEVENTHOOK hWinEventHook,
        DWORD event,
        HWND hwnd,
        LONG idObject,
        LONG idChild,
        DWORD idEventThread,
        DWORD dwmsEventTime
    );
};
