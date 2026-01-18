# Build Plan: Per-Monitor Focus Switching Prototype

## Overview

This plan breaks down the implementation into **6 testable stages**. Each stage builds on the previous one and can be verified independently.

---

## Stage 1: Project Setup & Message Loop
**Goal:** Create CMake project structure with a working Win32 message loop

**Deliverables:**
- CMakeLists.txt
- main.cpp with WinMain and message loop
- Compiles and runs without errors
- Prints "Application started" and keeps running until Ctrl+C

**Testing:**
- Run executable, verify console output
- Verify it doesn't crash or exit immediately

---

## Stage 2: Focus Tracking
**Goal:** Track foreground window changes using SetWinEventHook

**Deliverables:**
- FocusTracker.cpp/.h
- Hook EVENT_SYSTEM_FOREGROUND events
- Print window handle + title when focus changes

**Testing:**
- Run executable
- Switch between different apps (browser, notepad, explorer)
- Verify console prints window handles and titles

---

## Stage 3: Monitor Detection
**Goal:** Detect which monitor a window is on

**Deliverables:**
- MonitorManager.cpp/.h
- Enumerate monitors on startup
- For each focus change, print which monitor the window is on

**Testing:**
- Run executable
- Open apps on different monitors
- Switch focus between them
- Verify console shows correct monitor index for each window

---

## Stage 4: Per-Monitor Focus Stacks
**Goal:** Maintain MRU (most recently used) focus history per monitor

**Deliverables:**
- Per-monitor focus stack implementation
- When focus changes, update the appropriate monitor's stack
- Print stack contents for debugging

**Testing:**
- Open multiple apps on Monitor 1, multiple on Monitor 2
- Switch focus in various orders
- Verify console shows correct stack state per monitor

---

## Stage 5: Hotkey & Monitor Cycling
**Goal:** Register Ctrl+Alt+N and cycle through monitors, focusing last active window

**Deliverables:**
- HotkeyManager.cpp/.h
- Register Ctrl+Alt+N hotkey
- On press: cycle to next monitor, focus its top-of-stack window
- Print which monitor was activated

**Testing:**
- Open apps on both monitors
- Press Ctrl+Alt+N repeatedly
- Verify focus switches between monitors
- Verify correct window is activated on each monitor

---

## Stage 6: Integration & Polish
**Goal:** End-to-end testing and minor refinements

**Deliverables:**
- Handle edge cases (no windows on monitor, invalid windows)
- Ensure clean shutdown
- Final stability testing

**Testing:**
- Multi-monitor workflow simulation
- Close windows while running
- Move windows between monitors
- Verify no crashes, expected behavior

---

## Build Requirements

- **Compiler:** MSVC (Visual Studio 2022 or Build Tools)
- **Build System:** CMake 3.15+
- **Target:** Windows 10/11, x64
- **APIs:** Win32 (no external dependencies)

---

## Project Structure

```
wayback/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── FocusTracker.cpp
│   ├── FocusTracker.h
│   ├── MonitorManager.cpp
│   ├── MonitorManager.h
│   ├── HotkeyManager.cpp
│   └── HotkeyManager.h
├── build/           (generated)
└── plan/            (this folder)
```

---

## Notes

- Each stage is **independently testable**
- Console output is verbose for debugging
- Focus is on **getting it working**, not perfection
- SetForegroundWindow may fail sometimes (expected)
