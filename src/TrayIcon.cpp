#include "TrayIcon.h"
#include <iostream>

static TrayIcon* g_trayIcon = nullptr;

TrayIcon::TrayIcon()
    : m_created(false) {
    g_trayIcon = this;
    ZeroMemory(&m_nid, sizeof(NOTIFYICONDATA));
}

TrayIcon::~TrayIcon() {
    Destroy();
    g_trayIcon = nullptr;
}

bool TrayIcon::Create(HWND hwnd) {
    if (m_created) {
        return true;
    }
    
    m_nid.cbSize = sizeof(NOTIFYICONDATA);
    m_nid.hWnd = hwnd;
    m_nid.uID = 1;
    m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage = WM_TRAYICON;
    
    // Load default application icon
    m_nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    
    // Set tooltip text
    lstrcpy(m_nid.szTip, TEXT("Wayback - Per-Monitor Focus Switching"));
    
    // Add icon to system tray
    if (!Shell_NotifyIcon(NIM_ADD, &m_nid)) {
        std::cerr << "Failed to create tray icon" << std::endl;
        return false;
    }
    
    m_created = true;
    std::cout << "System tray icon created" << std::endl;
    return true;
}

void TrayIcon::Destroy() {
    if (m_created) {
        Shell_NotifyIcon(NIM_DELETE, &m_nid);
        m_created = false;
    }
}

void TrayIcon::HandleContextMenu(HWND hwnd) {
    POINT pt;
    GetCursorPos(&pt);
    
    HMENU hMenu = CreatePopupMenu();
    if (hMenu) {
        InsertMenu(hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_ABOUT, TEXT("About"));
        InsertMenu(hMenu, -1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
        InsertMenu(hMenu, -1, MF_BYPOSITION | MF_STRING, ID_TRAY_EXIT, TEXT("Exit"));
        
        // Required for proper menu behavior
        SetForegroundWindow(hwnd);
        
        TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
        DestroyMenu(hMenu);
    }
}

void TrayIcon::ShowAboutDialog(HWND hwnd) {
    MessageBox(hwnd, 
        TEXT("Wayback v1.0\n\n")
        TEXT("Per-Monitor Focus Switching for Windows\n\n")
        TEXT("Hotkey: Ctrl+Alt+N (or custom)\n")
        TEXT("Cycles between monitors and focuses the last active window.\n\n")
        TEXT("Configure hotkeys in wayback.ini"),
        TEXT("About Wayback"),
        MB_OK | MB_ICONINFORMATION);
}

LRESULT CALLBACK TrayIcon::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_TRAYICON) {
        if (lParam == WM_RBUTTONUP) {
            if (g_trayIcon) {
                g_trayIcon->HandleContextMenu(hwnd);
            }
        }
        return 0;
    }
    
    if (msg == WM_COMMAND) {
        switch (LOWORD(wParam)) {
            case ID_TRAY_EXIT:
                PostQuitMessage(0);
                return 0;
            case ID_TRAY_ABOUT:
                if (g_trayIcon) {
                    g_trayIcon->ShowAboutDialog(hwnd);
                }
                return 0;
        }
    }
    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
