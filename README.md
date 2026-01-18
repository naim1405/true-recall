# True Recall - Per-Monitor Focus Switching for Windows

**Emulate niri-style per-monitor focus behavior on Windows**

True Recall is a lightweight background utility that adds per-monitor focus memory to Windows, making multi-monitor workflows more efficient and predictable.

---

## Features

✨ **Per-Monitor Focus Memory** - Each monitor remembers its last active window  
⌨️ **Hotkey-Based Switching** - Instantly jump between monitors (default: `Alt+N`)  
🖱️ **Mouse Cursor Movement** - Cursor follows you to the target monitor (optional)  
🔧 **Customizable** - Edit `true-recall.ini` to configure hotkeys and behavior  
🛡️ **Conflict Detection** - Warns if hotkeys conflict with Windows system shortcuts  
💻 **System Tray** - Runs silently in the background with tray icon  
🪶 **Lightweight** - ~500KB, minimal CPU/memory usage  

---

## Quick Start

### Installation

1. Download `true-recall.exe` from the [latest release](https://github.com/yourusername/true-recall/releases)
2. Place it anywhere you like (Desktop, Program Files, etc.)
3. Double-click to run
4. Look for the True Recall icon in your system tray
5. Use the default hotkey **Alt+N** to cycle between monitors

**That's it!** The config file (`true-recall.ini`) is created automatically on first run.

### Usage

1. Open applications on different monitors
2. Work naturally, switching focus between windows
3. Press **Alt+N** to jump to the next monitor
4. True Recall automatically focuses the last active window on that monitor
5. (Optional) Mouse cursor moves to the target monitor automatically

### Configuration

Edit `true-recall.ini` (created in the same folder as `true-recall.exe`):

```ini
; True Recall Configuration File
;
; Hotkey format: Modifier+Modifier+Key
; Modifiers: Ctrl, Alt, Shift, Win
; Keys: A-Z, 0-9, F1-F12

CycleMonitorHotkey=Alt+N

; Move mouse cursor to the monitor when switching
; Set to true or false
MoveMouseToMonitor=true
```

**Hotkey Examples:**
- `Alt+M` - Simple Alt+Key combination
- `Ctrl+Alt+M` - Different letter
- `Win+Shift+F1` - Use Win key with F1
- `Ctrl+Shift+9` - Use a number key

**Mouse Cursor Movement:**
- `MoveMouseToMonitor=true` - Cursor moves to center of target monitor (default)
- `MoveMouseToMonitor=false` - Cursor stays in place

**Note:** After editing `true-recall.ini`, restart True Recall for changes to take effect.

---

## Building from Source

### Requirements

- Windows 10 or later
- Visual Studio 2022 (or Build Tools)
- CMake 3.15+

### Build Steps

```powershell
# Clone or download the repository
cd true-recall

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build Release version (no console, GUI tray app)
cmake --build . --config Release

# The executable will be at: build/Release/true-recall.exe
```

### Build Types

- **Release**: GUI mode (no console window), system tray only
- **Debug**: Console window for debugging output

---

## Auto-Start with Windows

### Method 1: Startup Folder (Easiest)

1. Press `Win+R`
2. Type `shell:startup` and press Enter
3. Copy `true-recall.exe` to this folder
4. Done! True Recall will start on every login

### Method 2: Task Scheduler (Advanced)

1. Open Task Scheduler
2. Create Basic Task
3. Name: "True Recall"
4. Trigger: "When I log on"
5. Action: "Start a program"
6. Program: Browse to `true-recall.exe`
7. Finish

---

## How It Works

### Architecture

True Recall uses Windows accessibility hooks to track focus changes:

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
**Solution:** Edit `true-recall.ini` to use a different hotkey, then restart True Recall. Avoid common Windows hotkeys (Win+D, Win+L, Alt+Tab, etc.)

### "Failed to register hotkey" error
**Solution:** Choose a different key combination that's not already in use by Windows or another application.

### Window doesn't focus
This is expected for some apps (UWP, elevated processes) due to Windows security restrictions.

### Mouse cursor doesn't move
Set `MoveMouseToMonitor=true` in `true-recall.ini` and restart True Recall.

### Tray icon doesn't appear
Restart True Recall. If the issue persists, check Windows Event Viewer for errors.

---

## Uninstalling

1. Right-click the True Recall tray icon
2. Click "Exit"
3. Delete `true-recall.exe` and `true-recall.ini`
4. If added to Startup folder, remove it from there too

---

## Project Structure

```
true-recall/
├── true-recall.exe      # Main executable
├── true-recall.ini      # Configuration file (auto-created)
├── README.md            # User documentation
├── BUILDING.md          # Build and distribution guide
├── CHANGELOG.md         # Version history
└── LICENSE              # MIT License
```

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

## Version History

See [CHANGELOG.md](CHANGELOG.md) for detailed version history and changes.

---

## Support & Contributing

- Check this README and [CHANGELOG.md](CHANGELOG.md) first
- See [BUILDING.md](BUILDING.md) for build instructions
- Report bugs via GitHub Issues

---

**Made with ❤️ for better multi-monitor workflows on Windows**

*Current version: 1.1*
