#include "FocusTracker.h"
#include <iostream>

FocusTracker::FocusTracker()
    : m_hook(nullptr)
{
}

FocusTracker::~FocusTracker() {
    Stop();
}

bool FocusTracker::Start() {
    if (m_hook != nullptr) {
        std::cerr << "FocusTracker already started" << std::endl;
        return false;
    }

    // Install hook for foreground window changes
    m_hook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND,     // eventMin
        EVENT_SYSTEM_FOREGROUND,     // eventMax
        nullptr,                     // hmodWinEventProc
        WinEventProc,                // callback function
        0,                           // idProcess (0 = all processes)
        0,                           // idThread (0 = all threads)
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS  // dwFlags
    );

    if (m_hook == nullptr) {
        std::cerr << "Failed to install focus tracking hook" << std::endl;
        return false;
    }

    std::cout << "Focus tracking started" << std::endl;
    return true;
}

void FocusTracker::Stop() {
    if (m_hook != nullptr) {
        UnhookWinEvent(m_hook);
        m_hook = nullptr;
        std::cout << "Focus tracking stopped" << std::endl;
    }
}

void CALLBACK FocusTracker::WinEventProc(
    HWINEVENTHOOK hWinEventHook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD idEventThread,
    DWORD dwmsEventTime
) {
    // Only process if it's a window object
    if (idObject != OBJID_WINDOW || idChild != CHILDID_SELF) {
        return;
    }

    // Skip if no valid window handle
    if (hwnd == nullptr || !IsWindow(hwnd)) {
        return;
    }

    // Get window title
    wchar_t title[256] = L"";
    int titleLength = GetWindowTextW(hwnd, title, sizeof(title) / sizeof(title[0]));

    // Print focus change
    if (titleLength > 0) {
        std::wcout << L"Focus changed: HWND=0x" << std::hex << reinterpret_cast<uintptr_t>(hwnd)
                   << std::dec << L" Title=" << title << std::endl;
    } else {
        std::cout << "Focus changed: HWND=0x" << std::hex << reinterpret_cast<uintptr_t>(hwnd)
                  << std::dec << " Title=(no title)" << std::endl;
    }
}
