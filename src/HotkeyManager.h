#pragma once

#include <windows.h>
#include "MonitorManager.h"

// Hotkey IDs
#define HOTKEY_CYCLE_MONITOR 1

class HotkeyManager {
public:
    HotkeyManager(MonitorManager* monitorManager);
    ~HotkeyManager();
    
    bool RegisterHotkeys();
    void UnregisterHotkeys();
    void HandleHotkey(int hotkeyId);

private:
    MonitorManager* m_monitorManager;
    int m_currentMonitor;
    HWND m_messageWindow;  // Hidden window for receiving hotkey messages
    
    // Helper function to activate a window
    bool ActivateWindow(HWND hwnd);
    
    // Create hidden message-only window
    bool CreateMessageWindow();
    void DestroyMessageWindow();
    
    // Window procedure for message window
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
