# Stage 1: Project Setup & Message Loop

## Context

You are building a **per-monitor focus switching utility** for Windows in C++. This is a personal-use prototype that tracks focus history per monitor and allows keyboard-driven monitor switching.

**Project Requirements:**
- Language: C++
- Build System: CMake
- Target: Windows 10/11 x64
- Console application with verbose output for debugging

**Project Location:** `c:\Users\EZIO\Desktop\wayback`

---

## What You're Building (Stage 1)

Create the basic project structure with a Win32 message loop that:
1. Initializes a console application
2. Sets up a Win32 message loop (required for hotkeys and hooks later)
3. Runs continuously until user presses Ctrl+C
4. Prints startup message to console

---

## Implementation Requirements

### File Structure
Create these files in the wayback folder:

```
wayback/
├── CMakeLists.txt
└── src/
    └── main.cpp
```

### CMakeLists.txt
- Target name: `wayback`
- C++17 or later
- Link against `user32.lib` (for Win32 APIs)
- Console subsystem (not Windows subsystem)

### main.cpp
- Use `WinMain` or `main` entry point
- Initialize Win32 message loop using `GetMessage` / `DispatchMessage`
- Print "Wayback started. Press Ctrl+C to exit." to console
- Handle Ctrl+C gracefully (optional: use SetConsoleCtrlHandler)
- Keep message loop running

---

## Testing Instructions

After implementation:

1. **Build:**
   ```powershell
   cmake -B build -G "Visual Studio 17 2022" -A x64
   cmake --build build --config Release
   ```

2. **Run:**
   ```powershell
   .\build\Release\wayback.exe
   ```

3. **Verify:**
   - Console displays startup message
   - Program keeps running (doesn't exit immediately)
   - No errors or crashes
   - Ctrl+C terminates the program

---

## Git Commit

After successful testing, commit your changes:

```powershell
git add .
git commit -m "Stage 1: Project setup and Win32 message loop"
```

---

## Success Criteria

- ✅ Project compiles without errors
- ✅ Executable runs and prints startup message
- ✅ Message loop keeps program alive
- ✅ Clean termination on Ctrl+C

---

## Notes

- This stage establishes the foundation for hooks and hotkeys in later stages
- The message loop is essential for processing Windows messages
- Keep code simple and direct - no over-engineering
