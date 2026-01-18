# Stage 3: Monitor Detection

## Context

You are continuing development of a **per-monitor focus switching utility** for Windows in C++.

**Previous Stages:**
- **Stage 1:** Created project with Win32 message loop
- **Stage 2:** Implemented focus tracking using SetWinEventHook
  - FocusTracker prints window handle and title on focus changes

**Current Location:** `c:\Users\EZIO\Desktop\wayback`

**Existing Files:**
```
wayback/
├── CMakeLists.txt
└── src/
    ├── main.cpp
    ├── FocusTracker.h
    └── FocusTracker.cpp
```

---

## What You're Building (Stage 3)

Add monitor detection to determine which physical monitor each window is on.

**Goal:** When a window receives focus, print both the window title AND which monitor it's on.

---

## Implementation Requirements

### New Files

Create `src/MonitorManager.h` and `src/MonitorManager.cpp`

### MonitorManager Class

**Public Interface:**
```cpp
class MonitorManager {
public:
    MonitorManager();
    
    void EnumerateMonitors();  // Detect connected monitors
    int GetMonitorCount() const;
    int GetMonitorIndexForWindow(HWND hwnd) const;  // Which monitor is this window on?
    
    // For debugging
    void PrintMonitorInfo() const;
};
```

**Implementation Details:**
- Use `EnumDisplayMonitors` to enumerate all monitors on startup
- Store monitor handles (HMONITOR) in a vector/array with indices (0, 1, 2...)
- Implement `GetMonitorIndexForWindow`:
  - Use `MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST)`
  - Find the index of that HMONITOR in your stored list
  - Return the index (0-based)
- Print monitor count and basic info on startup

### Integration

**Update FocusTracker:**
- Add a `MonitorManager*` member to store reference
- Pass MonitorManager reference to FocusTracker constructor
- In focus change callback:
  - Get monitor index: `int monitorIdx = monitorManager->GetMonitorIndexForWindow(hwnd);`
  - Print: `"Focus changed: Monitor [idx] HWND=0x[handle] Title=[title]"`

**Update main.cpp:**
- Create `MonitorManager` instance
- Call `EnumerateMonitors()` after startup message
- Call `PrintMonitorInfo()` for debugging
- Pass MonitorManager reference to FocusTracker constructor

### CMakeLists.txt Updates

- Add MonitorManager.cpp to source files

---

## Key Windows APIs to Use

- `EnumDisplayMonitors` - Enumerate all monitors
- `MonitorFromWindow` - Get monitor for a specific window
- `GetMonitorInfo` - Get monitor details (optional, for debugging)

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

3. **Test:**
   - Verify monitor enumeration on startup
   - Open applications on **different monitors**
   - Switch focus between apps on Monitor 0 and Monitor 1
   - Observe console output

4. **Verify:**
   - Startup shows correct monitor count (should be 2)
   - Each focus change shows the correct monitor index
   - Windows on Monitor 0 show "Monitor 0"
   - Windows on Monitor 1 show "Monitor 1"

---

## Git Commit

After successful testing, commit your changes:

```powershell
git add .
git commit -m "Stage 3: Add monitor detection and window-to-monitor mapping"
```

---

## Expected Output Example

```
Wayback started. Press Ctrl+C to exit.
Detected 2 monitors
Focus changed: Monitor 0 HWND=0x00050A1C Title=Untitled - Notepad
Focus changed: Monitor 1 HWND=0x000B1234 Title=Mozilla Firefox
Focus changed: Monitor 0 HWND=0x00050A1C Title=Untitled - Notepad
```

---

## Success Criteria

- ✅ MonitorManager compiles and links
- ✅ Correct number of monitors detected on startup
- ✅ Each focus change shows correct monitor index
- ✅ Moving windows between monitors updates detection correctly
- ✅ No crashes

---

## Notes

- Use `MONITOR_DEFAULTTONEAREST` to handle edge cases
- Monitor indices should be stable (don't re-enumerate every frame)
- Primary monitor should typically be index 0, but don't assume this
- EnumDisplayMonitors callback must be static or standalone function
