# Stage 5: Hotkey & Monitor Cycling

## Context

You are continuing development of a **per-monitor focus switching utility** for Windows in C++.

**Previous Stages:**
- **Stage 1:** Created project with Win32 message loop
- **Stage 2:** Implemented focus tracking (FocusTracker)
- **Stage 3:** Added monitor detection (MonitorManager)
- **Stage 4:** Implemented per-monitor focus stacks
  - Each monitor maintains MRU list of focused windows

**Current Location:** `c:\Users\EZIO\Desktop\wayback`

**Existing Files:**
```
wayback/
├── CMakeLists.txt
└── src/
    ├── main.cpp
    ├── FocusTracker.h
    ├── FocusTracker.cpp
    ├── MonitorManager.h
    └── MonitorManager.cpp
```

---

## What You're Building (Stage 5)

Implement hotkey registration and monitor cycling functionality. This is the **core feature**.

**Goal:** Press `Ctrl+Alt+N` to cycle through monitors and automatically focus the last active window on the target monitor.

---

## Implementation Requirements

### New Files

Create `src/HotkeyManager.h` and `src/HotkeyManager.cpp`

### HotkeyManager Class

**Public Interface:**
```cpp
class HotkeyManager {
public:
    HotkeyManager(MonitorManager* monitorManager);
    ~HotkeyManager();
    
    bool RegisterHotkeys();
    void UnregisterHotkeys();
    void HandleHotkey(int hotkeyId);
};
```

**Implementation Details:**
- Register `Ctrl+Alt+N` using `RegisterHotKey()`
  - Modifiers: `MOD_CONTROL | MOD_ALT`
  - Virtual key: `'N'`
  - Hotkey ID: Use a constant (e.g., `#define HOTKEY_CYCLE_MONITOR 1`)
- In `HandleHotkey()`:
  - Track current monitor index (start at 0)
  - Increment monitor index (cycle: `currentMonitor = (currentMonitor + 1) % monitorCount`)
  - Get the last focused window for the new monitor: `monitorManager->GetLastFocusedWindow(currentMonitor)`
  - Focus that window using `SetForegroundWindow(hwnd)`
  - Print: `"Switched to Monitor [idx]"`
- In destructor, call `UnregisterHotKey()`

### Window Activation

Create a helper function to reliably activate windows:

```cpp
bool ActivateWindow(HWND hwnd);
```

**Implementation:**
- Check if window is valid: `IsWindow(hwnd)`
- Check if window is visible: `IsWindowVisible(hwnd)`
- Try `SetForegroundWindow(hwnd)`
- Optionally try `AttachThreadInput` workaround if SetForegroundWindow fails
- Return true if successful

### Integration with main.cpp

**Update message loop:**
- Handle `WM_HOTKEY` messages in the message loop
- When received, call `hotkeyManager->HandleHotkey(wParam)`

**Startup:**
- Create HotkeyManager instance (pass MonitorManager reference)
- Call `RegisterHotkeys()` after MonitorManager initialization
- Print: `"Hotkeys registered: Ctrl+Alt+N = Cycle Monitor"`

### CMakeLists.txt Updates

- Add HotkeyManager.cpp to source files

---

## Key Windows APIs to Use

- `RegisterHotKey` - Register global hotkey
- `UnregisterHotKey` - Remove hotkey
- `SetForegroundWindow` - Focus a window
- `AttachThreadInput` - Workaround for focus restrictions (optional)
- `IsWindow`, `IsWindowVisible` - Validation

---

## Testing Instructions

1. **Build:**
   ```powershell
   cmake --build build --config Release
   ```

2. **Run:**
   ```powershell
   .\build\Release\wayback.exe
   ```

3. **Test Scenario:**
   - Open 2-3 apps on Monitor 0
   - Open 2-3 apps on Monitor 1
   - Focus a window on Monitor 0
   - Press `Ctrl+Alt+N`
     - Expected: Focus switches to last active window on Monitor 1
   - Press `Ctrl+Alt+N` again
     - Expected: Focus switches back to last active window on Monitor 0
   - Repeat several times

4. **Verify:**
   - Each press cycles to the next monitor
   - Correct window is focused on each monitor
   - Console prints which monitor was activated
   - No crashes or freezes

---

## Git Commit

After successful testing, commit your changes:

```powershell
git add .
git commit -m "Stage 5: Add hotkey registration and monitor cycling (core feature)"
```

---

## Expected Output Example

```
Wayback started. Press Ctrl+C to exit.
Detected 2 monitors
Hotkeys registered: Ctrl+Alt+N = Cycle Monitor

Focus changed: Monitor 0 HWND=0x00050A1C Title=Notepad
Focus changed: Monitor 1 HWND=0x000B1234 Title=Firefox
Switched to Monitor 1
Focus changed: Monitor 1 HWND=0x000B1234 Title=Firefox
Switched to Monitor 0
Focus changed: Monitor 0 HWND=0x00050A1C Title=Notepad
```

---

## Success Criteria

- ✅ Hotkey is registered successfully
- ✅ `Ctrl+Alt+N` cycles through monitors (0 → 1 → 0 → 1...)
- ✅ Correct window is focused on each monitor
- ✅ Focus stack is preserved and used correctly
- ✅ No crashes or errors

---

## Edge Cases to Handle

- No windows on target monitor → Print warning, don't crash
- Window in stack is no longer valid → Skip to next window in stack
- `SetForegroundWindow` fails → Print warning, but continue

---

## Notes

- This is the **core functionality** - the prototype should feel useful after this stage
- `SetForegroundWindow` may fail for security reasons - this is expected
- Consider using `BringWindowToTop` as fallback
- Cycling wraps around: Monitor 0 → Monitor 1 → Monitor 0...
