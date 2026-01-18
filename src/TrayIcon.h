#pragma once

#include <windows.h>
#include <shellapi.h>

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_ABOUT 1002

class TrayIcon {
public:
    TrayIcon();
    ~TrayIcon();
    
    bool Create(HWND hwnd);
    void Destroy();
    void HandleContextMenu(HWND hwnd);
    
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    NOTIFYICONDATA m_nid;
    bool m_created;
    
    void ShowAboutDialog(HWND hwnd);
};
