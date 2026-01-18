# Changelog

All notable changes to Wayback will be documented in this file.

## [1.1] - 2026-01-18

### Changed
- **Default hotkey simplified:** Changed from `Ctrl+Alt+N` to `Alt+N` for easier single-hand use
- Updated About dialog to reflect new default hotkey
- Updated documentation with new defaults

### Added
- **Mouse cursor repositioning:** Cursor now moves to center of target monitor when switching
- New config option `MoveMouseToMonitor` (boolean, default: `true`)
- `GetMonitorHandle()` method in MonitorManager for monitor positioning

### Configuration
Default `wayback.ini` now includes:
```ini
CycleMonitorHotkey=Alt+N
MoveMouseToMonitor=true
```

---

## [1.0] - 2026-01-18

### Added
- System tray icon with context menu (About, Exit)
- Configuration file system (`wayback.ini`)
- Customizable hotkeys via INI file
- Hotkey conflict detection with Windows system shortcuts
- Support for Ctrl, Alt, Shift, Win modifiers
- Support for A-Z, 0-9, F1-F12 keys
- GUI mode (no console window in Release builds)
- Auto-generated config file with documentation
- Comprehensive README and BUILDING documentation
- MIT License

### Features from Prototype (Stages 1-6)
- Per-monitor focus tracking
- Focus stack management (MRU - Most Recently Used)
- Hotkey-based monitor cycling
- Window validation and cleanup
- EVENT_OBJECT_DESTROY hook for automatic cleanup
- 3-tier window activation strategy
- Graceful shutdown and resource management
- Edge case handling (invalid windows, empty stacks)

### Build System
- Debug builds: Console window for debugging
- Release builds: GUI mode, standalone executable
- No external dependencies required

---

## Development History

The project was built in 6 stages:

1. **Stage 1:** Project setup and Win32 message loop
2. **Stage 2:** Focus tracking with SetWinEventHook
3. **Stage 3:** Monitor detection and window-to-monitor mapping
4. **Stage 4:** Per-monitor focus stacks (MRU lists)
5. **Stage 5:** Hotkey registration and monitor cycling
6. **Stage 6:** Edge case handling, window validation, cleanup

---

## Future Considerations

Potential features for future versions:
- Direct monitor selection (Alt+1, Alt+2, etc.)
- Custom tray icon
- Settings UI (instead of editing INI)
- Per-application hotkeys
- Window position memory
- Multi-desktop support
