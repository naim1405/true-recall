# Stage 6: Integration & Polish

## Context

You are finishing development of a **per-monitor focus switching utility** for Windows in C++.

**Previous Stages:**
- **Stage 1:** Created project with Win32 message loop
- **Stage 2:** Implemented focus tracking (FocusTracker)
- **Stage 3:** Added monitor detection (MonitorManager)
- **Stage 4:** Implemented per-monitor focus stacks
- **Stage 5:** Added hotkey registration and monitor cycling
  - `Ctrl+Alt+N` switches between monitors and focuses last active window

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
    ├── MonitorManager.cpp
    ├── HotkeyManager.h
    └── HotkeyManager.cpp
```

---

## What You're Building (Stage 6)

Final integration testing, edge case handling, and minor polish to make the prototype stable for daily use.

**Goal:** Ensure the prototype is robust and reliable in real-world usage.

---

## Tasks

### 1. Edge Case Handling

**Empty Focus Stacks:**
- When switching to a monitor with no windows in its stack
- Fallback: Try to find any visible window on that monitor
- Print: `"No recent windows on Monitor [idx]"`

**Invalid Windows:**
- Before focusing a window, validate it:
  - `IsWindow(hwnd)` - Still exists?
  - `IsWindowVisible(hwnd)` - Visible?
  - `!IsIconic(hwnd)` - Not minimized?
- If invalid, remove from stack and try next window

**Window Destruction:**
- Consider adding a hook for `EVENT_OBJECT_DESTROY` to remove destroyed windows from stacks
- Or: Validate windows lazily when trying to focus them

### 2. Focus Reliability Improvements

Improve `SetForegroundWindow` success rate:

**Try multiple approaches:**
```cpp
bool ActivateWindow(HWND hwnd) {
    // 1. Try direct SetForegroundWindow
    if (SetForegroundWindow(hwnd)) return true;
    
    // 2. Try AttachThreadInput trick
    DWORD foreThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    DWORD targetThread = GetWindowThreadProcessId(hwnd, NULL);
    if (foreThread != targetThread) {
        AttachThreadInput(foreThread, targetThread, TRUE);
        SetForegroundWindow(hwnd);
        AttachThreadInput(foreThread, targetThread, FALSE);
        return true;
    }
    
    // 3. Fallback to BringWindowToTop + SetFocus
    BringWindowToTop(hwnd);
    SetFocus(hwnd);
    
    return false;
}
```

### 3. Graceful Shutdown

- Ensure all resources are cleaned up:
  - Unhook event hooks
  - Unregister hotkeys
- Handle `WM_QUIT` or Ctrl+C gracefully
- Print: `"Shutting down..."`

### 4. Debugging Output Cleanup

**Optional improvements:**
- Add verbosity levels (verbose vs. quiet mode)
- Or keep current console output for now (fine for prototype)

---

## Integration Testing

### Test 1: Multi-Monitor Workflow Simulation

1. Open multiple apps on each monitor:
   - Monitor 0: VSCode, Terminal, File Explorer
   - Monitor 1: Browser, Notepad, Calculator

2. Use the workflow:
   - Work in VSCode (Monitor 0)
   - Press `Ctrl+Alt+N` → Should focus Browser (Monitor 1)
   - Work in Browser
   - Press `Ctrl+Alt+N` → Should focus VSCode (Monitor 0)
   - Click Terminal (Monitor 0)
   - Press `Ctrl+Alt+N` → Should focus Browser (Monitor 1)
   - Press `Ctrl+Alt+N` → Should focus Terminal (Monitor 0)

3. Verify focus switching feels natural and predictable

### Test 2: Window Closure

1. Open 3 apps on Monitor 0
2. Focus them in order: App1 → App2 → App3
3. Close App3 (the most recent)
4. Switch to Monitor 1, then back to Monitor 0 with hotkey
5. Verify: App2 is focused (next in stack)

### Test 3: Window Movement

1. Open app on Monitor 0, focus it
2. Drag window to Monitor 1
3. Switch to Monitor 0 with hotkey
4. Verify: Previous window on Monitor 0 is focused (not the moved window)

### Test 4: Long Running Stability

1. Run the tool for 10+ minutes
2. Perform normal multitasking
3. Switch monitors frequently with hotkey
4. Verify: No crashes, memory leaks, or weird behavior

---

## Git Commit

After successful testing, commit your changes:

```powershell
git add .
git commit -m "Stage 6: Polish and edge case handling - prototype complete"
```

---

## Success Criteria

- ✅ All edge cases handled gracefully (no crashes)
- ✅ Focus switching works reliably in multi-monitor workflow
- ✅ Invalid/closed windows don't cause errors
- ✅ Clean shutdown with no resource leaks
- ✅ Tool feels stable for daily use
- ✅ Prototype meets the success criteria from Project.md

---

## Final Deliverables

After Stage 6, you should have:
- A working prototype that improves multi-monitor workflow
- Stable hotkey-based monitor switching
- Per-monitor focus memory that feels like niri
- Console application ready for daily use

---

## Next Steps (Future, Outside This Plan)

After the prototype is validated:
- Add system tray icon with Exit option
- Consider additional hotkeys (direct monitor selection)
- Optional: Config file for hotkey customization
- Optional: Build/release packaging

For now, **focus on getting Stage 6 working reliably**.
