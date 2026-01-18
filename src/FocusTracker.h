#pragma once

#include <windows.h>

class FocusTracker {
public:
    FocusTracker();
    ~FocusTracker();

    bool Start();  // Install hook
    void Stop();   // Remove hook

private:
    HWINEVENTHOOK m_hook;

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
