#pragma once

#include <windows.h>
#include <string>

struct HotkeyConfig {
    UINT modifiers;  // MOD_CONTROL, MOD_ALT, MOD_SHIFT, MOD_WIN
    UINT vkey;       // Virtual key code (e.g., 'N', VK_F1, etc.)
    
    HotkeyConfig() : modifiers(MOD_ALT | MOD_NOREPEAT), vkey('N') {}
};

class Config {
public:
    Config();
    
    bool Load();  // Load from true-recall.ini
    bool Save();  // Save to true-recall.ini
    
    HotkeyConfig GetHotkeyConfig() const { return m_hotkey; }
    void SetHotkeyConfig(const HotkeyConfig& hotkey) { m_hotkey = hotkey; }
    
    bool GetMoveMouse() const { return m_moveMouse; }
    void SetMoveMouse(bool moveMouse) { m_moveMouse = moveMouse; }
    
    std::wstring GetHotkeyString() const;
    bool ParseHotkeyString(const std::wstring& hotkeyStr);
    
    // Check if hotkey conflicts with Windows system hotkeys
    bool IsHotkeyConflict(UINT modifiers, UINT vkey) const;

private:
    HotkeyConfig m_hotkey;
    bool m_moveMouse;
    std::wstring m_configPath;
    
    void CreateDefaultConfig();
    std::wstring GetModifierString(UINT modifiers) const;
    std::wstring GetKeyString(UINT vkey) const;
};
