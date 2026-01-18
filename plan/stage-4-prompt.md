# Stage 4: Per-Monitor Focus Stacks

## Context

You are continuing development of a **per-monitor focus switching utility** for Windows in C++.

**Previous Stages:**
- **Stage 1:** Created project with Win32 message loop
- **Stage 2:** Implemented focus tracking (FocusTracker)
- **Stage 3:** Added monitor detection (MonitorManager)
  - Each focus change now shows which monitor the window is on

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

## What You're Building (Stage 4)

Implement per-monitor focus history stacks. Each monitor should maintain a list of recently focused windows (MRU order).

**Goal:** When a window receives focus, add it to the top of that monitor's focus stack.

---

## Implementation Requirements

### Update MonitorManager

Add focus stack functionality to the existing MonitorManager class:

**New Public Methods:**
```cpp
void OnWindowFocused(HWND hwnd);  // Called when window gets focus
HWND GetLastFocusedWindow(int monitorIndex) const;  // Top of stack
void PrintFocusStacks() const;  // Debug output
```

**Implementation Details:**
- Store a **separate stack/vector per monitor**
  - e.g., `std::map<int, std::vector<HWND>> focusStacks;`
  - Key = monitor index, Value = vector of HWNDs (most recent first)
- In `OnWindowFocused(hwnd)`:
  - Determine which monitor the window is on
  - Remove hwnd from that monitor's stack if already present (avoid duplicates)
  - Add hwnd to the front of that monitor's stack
  - Limit stack size to ~10 windows per monitor (optional)
- In `GetLastFocusedWindow(monitorIndex)`:
  - Return the first HWND in that monitor's stack (most recent)
  - Return NULL if stack is empty
- In `PrintFocusStacks()`:
  - Print each monitor's stack for debugging

### Integration

**Update FocusTracker:**
- In focus change callback, call `monitorManager->OnWindowFocused(hwnd)`

**Update main.cpp:**
- Optionally add a timer or periodic call to `PrintFocusStacks()` for debugging
- Or print stack after each focus change (can be verbose)

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
   - Open 3 apps on Monitor 0 (e.g., Notepad, Calculator, File Explorer)
   - Open 2 apps on Monitor 1 (e.g., Browser, Terminal)
   - Click through apps in this order:
     - Monitor 0: Notepad → Calculator → Notepad → File Explorer
     - Monitor 1: Browser → Terminal
   - Observe console output

4. **Verify:**
   - Each monitor maintains its own stack
   - Most recently focused window is at top of stack
   - Switching between apps on same monitor reorders that monitor's stack
   - Stacks don't interfere with each other

---

## Git Commit

After successful testing, commit your changes:

```powershell
git add .
git commit -m "Stage 4: Implement per-monitor focus stacks (MRU tracking)"
```

---

## Expected Output Example

```
Wayback started. Press Ctrl+C to exit.
Detected 2 monitors

Focus changed: Monitor 0 HWND=0x00050A1C Title=Untitled - Notepad
Focus changed: Monitor 0 HWND=0x00060B2D Title=Calculator
Focus changed: Monitor 1 HWND=0x000C3456 Title=Mozilla Firefox

--- Focus Stacks ---
Monitor 0: [Calculator] [Notepad]
Monitor 1: [Firefox]
--------------------
```

---

## Success Criteria

- ✅ Each monitor maintains separate focus stack
- ✅ Most recent window is always at top of stack
- ✅ Duplicate windows are removed before re-adding (no duplicates in stack)
- ✅ Stacks update correctly as user switches windows
- ✅ No crashes

---

## Notes

- Use `std::vector` or `std::deque` for stacks (front = most recent)
- Check if HWND is still valid before using (use `IsWindow(hwnd)`)
- Limit stack size to prevent unbounded growth
- This stage is pure tracking - no focus switching yet
