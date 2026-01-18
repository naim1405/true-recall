# Wayback - Per-Monitor Focus Switching for Windows

**Emulate niri-style per-monitor focus behavior on Windows**

Wayback is a lightweight background utility that adds per-monitor focus memory to Windows, making multi-monitor workflows more efficient and predictable.

---

## Features

✨ **Per-Monitor Focus Memory** - Each monitor remembers its last active window  
⌨️ **Hotkey-Based Switching** - Instantly jump between monitors with configurable hotkeys  
🔧 **Customizable** - Edit `wayback.ini` to change hotkeys  
🛡️ **Conflict Detection** - Warns if hotkeys conflict with Windows system shortcuts  
💻 **System Tray** - Runs silently in the background with tray icon  
🪶 **Lightweight** - <2MB, minimal CPU/memory usage  

---

## Quick Start

### Installation

1. Download `wayback.exe` from the latest release
2. Run `wayback.exe`
3. Look for the Wayback icon in your system tray
4. Use the default hotkey **Alt+N** to cycle between monitors

### Usage

1. Open applications on different monitors
2. Work naturally, switching focus between windows
3. Press **Alt+N** to jump to the next monitor
4. Wayback automatically focuses the last active window on that monitor
5. (Optional) Mouse cursor moves to the target monitor automatically

### Configuration

Edit `wayback.ini` (created in the same folder as `wayback.exe`):

```ini
; Wayback Configuration File
;
; Hotkey format: Modifier+Modifier+Key
; Modifiers: Ctrl, Alt, Shift, Win
; Keys: A-ZAlt+M

CycleMonitorHotkey=Alt+N

; Move mouse cursor to the monitor when switching
; Set to true or false
MoveMouseToMonitor=true
CycleMonitorHotkey=Ctrl+Alt+N
```

**Supported Keys:**
- Letters: A-Z
- Numbers: 0-9
- Function keys: Add Ctrl modifier
- `Win+Shift+F1` - Use Win key with F1
- `Ctrl+Shift+9` - Use a number key
- `Alt+M` - Simple Alt+Key combination

**Mouse Cursor Movement:**
- `MoveMouseToMonitor=true` - Cursor moves to center of target monitor (default)
- `MoveMouseToMonitor=false` - Cursor stays in place
**Examples:**
- `Ctrl+Alt+M` - Different letter
- `Win+Shift+F1` - Use Win key with F1
- `Ctrl+Shift+9` - Use a number key

**Note:** After editing `wayback.ini`, restart Wayback for changes to take effect.

---

## Building from Source

### Requirements

- Windows 10 or later
- Visual Studio 2022 (or Build Tools)
- CMake 3.15+

### Build Steps

```powershell
# Clone or download the repository
cd wayback

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build Release version (no console, GUI tray app)
cmake --build . --config Release

# The executable will be at: build/Release/wayback.exe
```

### Build Types

- **Release**: GUI mode (no console window), system tray only
- **Debug**: Console window for debugging output

---

## Auto-Start with Windows

### Method 1: Startup Folder (Easiest)

1. Press `Win+R`
2. Type `shell:startup` and press Enter
3. Copy `wayback.exe` to this folder
4. Done! Wayback will start on every login

### Method 2: Task Scheduler (Advanced)

1. Open Task Scheduler
2. Create Basic Task
3. Name: "Wayback"
4. Trigger: "When I log on"
5. Action: "Start a program"
6. Program: Browse to `wayback.exe`
7. Finish

---

## How It Works

### Architecture

Wayback uses Windows accessibility hooks to track focus changes:

1. **EVENT_SYSTEM_FOREGROUND** - Tracks when windows gain focus
2. **EVENT_OBJECT_DESTROY** - Cleans up when windows are closed
3. **RegisterHotKey** - Captures global hotkey presses
4. **System Tray** - Provides GUI presence and exit menu

### Focus Stack

Each monitor maintains a focus stack (MRU - Most Recently Used):

- Stack size limited to 10 windows per monitor
- Automatic cleanup of closed/invalid windows
- Window validation before activation

### Window Activation

3-tier activation strategy for maximum reliability:

1. **Direct** - `SetForegroundWindow()`
2. **AttachThreadInput** - Workaround for cross-thread focus
3. **Fallback** - `BringWindowToTop()` + `SetFocus()`

---

## Known Limitations

These are Windows API limitations, not bugs:

- **Some apps resist focus** - UWP apps, elevated apps may not cooperate
- **SetForegroundWindow restrictions** - Windows security policy may prevent focus theft
- **No virtual desktop support** - Tracks physical monitors only
- **Fullscreen exclusive games** - May not work correctly

All limitations are expected and accepted per the design philosophy.

---

## Troubleshooting

### Hotkey doesn't work

**Cause:** Hotkey already in use by another application

**Solution:**
1. Edit `wayback.ini` to use a different hotkey
2. Restart Wayback
3. Avoid common Windows hotkeys (Win+D, Win+L, Alt+Tab, etc.)

### "Failed to register hotkey" error

**Cause:** Hotkey conflict

**Solution:** Choose a different key combination that's not system-reserved

### Window doesn't focus

**Cause:** Application-specific restriction or Windows security policy

**Solution:** This is expected behavior for some apps (UWP, elevated processes)

### Tray icon doesn't appear

**Cause:** Tray icon creation failed (rare)

**Solution:** Restart Wayback. If persists, check Windows Event Viewer

---

## Uninstalling

1. Right-click the Wayback tray icon
2. Click "Exit"
3. Delete `wayback.exe` and `wayback.ini`
4. If added to Startup folder, remove it from there too

---

##File Structure

```
wayback/
├── wayback.exe          # Main executable
├── wayback.ini          # Configuration file (auto-created)
└── README.md            # This file
```

---

## Technical Details

- **Language:** C++17
- **Platform:** Windows 10/11 (x64)
- **APIs:** Win32 API (user32, shell32)
- **Build System:** CMake
- **Compiler:** MSVC

### Dependencies

- **Runtime:** None (fully self-contained)
- **Build-time:** Windows SDK, CMake, MSVC

---

## Project Philosophy

> **Minimal. Direct. Pragmatic.**

- Focus on solving one problem well
- Stable over clever
- Silent failure over wrong behavior
- Best-effort within Windows limitations

This is a personal-use utility that became shareable, not enterprise software.

---

## License

MIT License - See [LICENSE](LICENSE) file

---

## Credits

Inspired by [niri](https://github.com/YaLTeR/niri) Wayland compositor's per-monitor focus model.

---

## Support

For issues, questions, or suggestions:
- Check this README and troubleshooting section first
- Report bugs via GitHub Issues (if applicable)

---

**Made with ❤️ for better multi-monitor workflows on Windows**
