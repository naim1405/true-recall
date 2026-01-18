# Stage 2: Focus Tracking

## Context

You are continuing development of a **per-monitor focus switching utility** for Windows in C++.

**Previous Stage (Stage 1):** 
- Created project structure with CMakeLists.txt
- Implemented basic Win32 message loop in main.cpp
- Program runs and processes Windows messages

**Current Location:** `c:\Users\EZIO\Desktop\wayback`

**Existing Files:**
```
wayback/
├── CMakeLists.txt
└── src/
    └── main.cpp (contains message loop)
```

---

## What You're Building (Stage 2)

Implement focus tracking using Windows hooks to detect when the user switches between windows.

**Goal:** Print to console whenever the foreground window changes, showing the window handle and title.

---

## Implementation Requirements

### New Files

Create `src/FocusTracker.h` and `src/FocusTracker.cpp`

### FocusTracker Class

**Public Interface:**
```cpp
class FocusTracker {
public:
    FocusTracker();
    ~FocusTracker();
    
    bool Start();  // Install hook
    void Stop();   // Remove hook
};
```

**Implementation Details:**
- Use `SetWinEventHook` with `EVENT_SYSTEM_FOREGROUND` to track focus changes
- In the hook callback:
  - Get window handle that received focus
  - Get window title using `GetWindowText`
  - Print: `"Focus changed: HWND=0x[handle] Title=[title]"`
- Store hook handle as member variable
- In destructor, call `UnhookWinEvent`

### Integration with main.cpp

- Include FocusTracker.h
- Create FocusTracker instance before message loop
- Call `tracker.Start()` before entering message loop
- Keep the same message loop from Stage 1

### CMakeLists.txt Updates

- Add FocusTracker.cpp to the source files list

---

## Key Windows APIs to Use

- `SetWinEventHook` - Install the hook
- `UnhookWinEvent` - Remove the hook
- `GetWindowText` - Get window title
- `EVENT_SYSTEM_FOREGROUND` - Constant for focus events

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
   - Open multiple applications (e.g., Notepad, File Explorer, Browser)
   - Click between them to switch focus
   - Observe console output

4. **Verify:**
   - Each focus change is printed to console
   - Window handles (HWND) are shown
   - Window titles are displayed correctly
   - No crashes when switching between apps

---

## Git Commit

After successful testing, commit your changes:

```powershell
git add .
git commit -m "Stage 2: Implement focus tracking with SetWinEventHook"
```

---

## Expected Output Example

```
Wayback started. Press Ctrl+C to exit.
Focus changed: HWND=0x00050A1C Title=Untitled - Notepad
Focus changed: HWND=0x000B1234 Title=Mozilla Firefox
Focus changed: HWND=0x00050A1C Title=Untitled - Notepad
```

---

## Success Criteria

- ✅ FocusTracker compiles and links
- ✅ Hook is installed successfully
- ✅ Console prints every focus change
- ✅ Window titles are displayed correctly
- ✅ No crashes or errors during focus switching

---

## Notes

- The hook callback must be `static` or a standalone function
- Use `WINEVENT_OUTOFCONTEXT` flag for simplicity
- Don't filter by process ID (pass 0)
- Error handling: Check if SetWinEventHook returns NULL
