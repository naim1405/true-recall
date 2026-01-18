#include "Config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Config::Config() {
    // Get config file path in the same directory as the executable
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    
    std::wstring exeDir = exePath;
    size_t lastSlash = exeDir.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        exeDir = exeDir.substr(0, lastSlash);
    }
    
    m_configPath = exeDir + L"\\wayback.ini";
}

bool Config::Load() {
    std::wifstream file(m_configPath);
    
    if (!file.is_open()) {
        std::wcout << L"Config file not found, creating default: " << m_configPath << std::endl;
        CreateDefaultConfig();
        return true;
    }
    
    std::wstring line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == L';' || line[0] == L'#') {
            continue;
        }
        
        // Parse key=value
        size_t equalsPos = line.find(L'=');
        if (equalsPos == std::wstring::npos) {
            continue;
        }
        
        std::wstring key = line.substr(0, equalsPos);
        std::wstring value = line.substr(equalsPos + 1);
        
        // Trim whitespace
        key.erase(0, key.find_first_not_of(L" \t"));
        key.erase(key.find_last_not_of(L" \t") + 1);
        value.erase(0, value.find_first_not_of(L" \t"));
        value.erase(value.find_last_not_of(L" \t") + 1);
        
        if (key == L"CycleMonitorHotkey") {
            if (!ParseHotkeyString(value)) {
                std::wcerr << L"Invalid hotkey format: " << value << std::endl;
            }
        }
    }
    
    file.close();
    std::wcout << L"Config loaded: " << GetHotkeyString() << std::endl;
    return true;
}

bool Config::Save() {
    std::wofstream file(m_configPath);
    
    if (!file.is_open()) {
        std::wcerr << L"Failed to save config file: " << m_configPath << std::endl;
        return false;
    }
    
    file << L"; Wayback Configuration File\n";
    file << L"; \n";
    file << L"; Hotkey format: Modifier+Modifier+Key\n";
    file << L"; Modifiers: Ctrl, Alt, Shift, Win\n";
    file << L"; Keys: A-Z, 0-9, F1-F12, or special keys\n";
    file << L"; Example: Ctrl+Alt+N\n";
    file << L"\n";
    file << L"CycleMonitorHotkey=" << GetHotkeyString() << L"\n";
    
    file.close();
    std::wcout << L"Config saved: " << m_configPath << std::endl;
    return true;
}

void Config::CreateDefaultConfig() {
    // Use default hotkey (Ctrl+Alt+N)
    m_hotkey.modifiers = MOD_CONTROL | MOD_ALT | MOD_NOREPEAT;
    m_hotkey.vkey = 'N';
    
    Save();
}

std::wstring Config::GetModifierString(UINT modifiers) const {
    std::wstring result;
    
    if (modifiers & MOD_CONTROL) {
        if (!result.empty()) result += L"+";
        result += L"Ctrl";
    }
    if (modifiers & MOD_ALT) {
        if (!result.empty()) result += L"+";
        result += L"Alt";
    }
    if (modifiers & MOD_SHIFT) {
        if (!result.empty()) result += L"+";
        result += L"Shift";
    }
    if (modifiers & MOD_WIN) {
        if (!result.empty()) result += L"+";
        result += L"Win";
    }
    
    return result;
}

std::wstring Config::GetKeyString(UINT vkey) const {
    // Handle letters and numbers
    if ((vkey >= 'A' && vkey <= 'Z') || (vkey >= '0' && vkey <= '9')) {
        return std::wstring(1, static_cast<wchar_t>(vkey));
    }
    
    // Handle function keys
    if (vkey >= VK_F1 && vkey <= VK_F24) {
        return L"F" + std::to_wstring(vkey - VK_F1 + 1);
    }
    
    // Handle special keys
    switch (vkey) {
        case VK_SPACE: return L"Space";
        case VK_RETURN: return L"Enter";
        case VK_TAB: return L"Tab";
        case VK_ESCAPE: return L"Esc";
        case VK_INSERT: return L"Insert";
        case VK_DELETE: return L"Delete";
        case VK_HOME: return L"Home";
        case VK_END: return L"End";
        case VK_PRIOR: return L"PageUp";
        case VK_NEXT: return L"PageDown";
        case VK_LEFT: return L"Left";
        case VK_RIGHT: return L"Right";
        case VK_UP: return L"Up";
        case VK_DOWN: return L"Down";
        default: return L"Unknown";
    }
}

std::wstring Config::GetHotkeyString() const {
    std::wstring result = GetModifierString(m_hotkey.modifiers);
    
    if (!result.empty()) {
        result += L"+";
    }
    
    result += GetKeyString(m_hotkey.vkey);
    
    return result;
}

bool Config::ParseHotkeyString(const std::wstring& hotkeyStr) {
    UINT modifiers = MOD_NOREPEAT;
    UINT vkey = 0;
    
    // Split by '+'
    std::wstring str = hotkeyStr;
    size_t pos = 0;
    std::wstring token;
    
    while ((pos = str.find(L'+')) != std::wstring::npos) {
        token = str.substr(0, pos);
        
        // Trim whitespace
        token.erase(0, token.find_first_not_of(L" \t"));
        token.erase(token.find_last_not_of(L" \t") + 1);
        
        // Convert to lowercase for comparison
        std::transform(token.begin(), token.end(), token.begin(), ::towlower);
        
        if (token == L"ctrl" || token == L"control") {
            modifiers |= MOD_CONTROL;
        } else if (token == L"alt") {
            modifiers |= MOD_ALT;
        } else if (token == L"shift") {
            modifiers |= MOD_SHIFT;
        } else if (token == L"win" || token == L"windows") {
            modifiers |= MOD_WIN;
        }
        
        str.erase(0, pos + 1);
    }
    
    // Last token is the key
    token = str;
    token.erase(0, token.find_first_not_of(L" \t"));
    token.erase(token.find_last_not_of(L" \t") + 1);
    
    // Parse key
    if (token.length() == 1) {
        wchar_t ch = towupper(token[0]);
        if ((ch >= L'A' && ch <= L'Z') || (ch >= L'0' && ch <= L'9')) {
            vkey = ch;
        }
    } else {
        // Check for function keys
        if (token[0] == L'f' || token[0] == L'F') {
            int fnum = _wtoi(token.substr(1).c_str());
            if (fnum >= 1 && fnum <= 24) {
                vkey = VK_F1 + (fnum - 1);
            }
        }
        // Add more special key parsing if needed
    }
    
    if (vkey == 0) {
        return false;  // Invalid key
    }
    
    // Check for conflicts
    if (IsHotkeyConflict(modifiers, vkey)) {
        std::wcerr << L"Warning: Hotkey may conflict with Windows system hotkeys" << std::endl;
    }
    
    m_hotkey.modifiers = modifiers;
    m_hotkey.vkey = vkey;
    
    return true;
}

bool Config::IsHotkeyConflict(UINT modifiers, UINT vkey) const {
    // Strip MOD_NOREPEAT for comparison
    UINT mods = modifiers & ~MOD_NOREPEAT;
    
    // Common Windows hotkey conflicts
    // Win+D (Show Desktop)
    if ((mods & MOD_WIN) && vkey == 'D') return true;
    
    // Win+E (Explorer)
    if ((mods & MOD_WIN) && vkey == 'E') return true;
    
    // Win+L (Lock)
    if ((mods & MOD_WIN) && vkey == 'L') return true;
    
    // Win+R (Run)
    if ((mods & MOD_WIN) && vkey == 'R') return true;
    
    // Win+Tab (Task View)
    if ((mods & MOD_WIN) && vkey == VK_TAB) return true;
    
    // Ctrl+Alt+Delete (obviously)
    if ((mods & MOD_CONTROL) && (mods & MOD_ALT) && vkey == VK_DELETE) return true;
    
    // Alt+Tab (Task Switcher)
    if ((mods & MOD_ALT) && !(mods & MOD_CONTROL) && vkey == VK_TAB) return true;
    
    // Alt+F4 (Close window)
    if ((mods & MOD_ALT) && vkey == VK_F4) return true;
    
    return false;  // No known conflict
}
