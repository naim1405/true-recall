# Building and Distributing Wayback

## For End Users (Standalone Binary)

The **Release** build creates a fully standalone executable that can be distributed as-is.

### What's Included

- `wayback.exe` - Single executable (~500KB)
- `wayback.ini` - Auto-generated configuration file

### No Dependencies Required

The Release build is **statically linked** and requires NO additional DLLs or runtimes.

Users can simply:
1. Download `wayback.exe`
2. Run it
3. It works!

---

## Building the Distributable

### Quick Build

```powershell
cd wayback\build
cmake --build . --config Release
```

The standalone executable will be at:
```
build\Release\wayback.exe
```

### Creating a Distribution Package

```powershell
# Create distribution folder
mkdir wayback-dist
cd wayback-dist

# Copy executable
copy ..\build\Release\wayback.exe .

# Copy README
copy ..\README.md .

# Create example config (optional, app will auto-create)
echo ; Wayback Configuration > wayback.ini
echo CycleMonitorHotkey=Ctrl+Alt+N >> wayback.ini

# Done! Zip and share
Compress-Archive -Path * -DestinationPath wayback-v1.0.zip
```

---

## Build Configurations

### Release (Recommended for Distribution)

```powershell
cmake --build . --config Release
```

**Characteristics:**
- `/SUBSYSTEM:WINDOWS` - No console window
- GUI mode with system tray only
- Optimized for size and speed
- Silent operation

**Use case:** End-user distribution

### Debug

```powershell
cmake --build . --config Release
```

**Characteristics:**
- `/SUBSYSTEM:CONSOLE` - Console window visible
- Debug symbols included
- Verbose logging output
- Easier troubleshooting

**Use case:** Development and debugging

---

## Checking the Build

### Verify No External Dependencies

```powershell
# Use Dependency Walker or similar
dumpbin /dependents wayback.exe
```

Should only show system DLLs:
- KERNEL32.dll
- USER32.dll
- SHELL32.dll

### Test Standalone

1. Copy `wayback.exe` to a fresh folder
2. Run it
3. Should work immediately without any setup

---

## Creating an Installer (Optional)

### Using Inno Setup

1. Download [Inno Setup](https://jrsoftware.org/isinfo.php)
2. Create `wayback-installer.iss`:

```iss
[Setup]
AppName=Wayback
AppVersion=1.0
DefaultDirName={autopf}\Wayback
DefaultGroupName=Wayback
OutputBaseFilename=wayback-setup
Compression=lzma2
SolidCompression=yes

[Files]
Source: "wayback.exe"; DestDir: "{app}"
Source: "README.md"; DestDir: "{app}"

[Icons]
Name: "{group}\Wayback"; Filename: "{app}\wayback.exe"
Name: "{commonstartup}\Wayback"; Filename: "{app}\wayback.exe"

[Run]
Filename: "{app}\wayback.exe"; Description: "Launch Wayback"; Flags: postinstall nowait
```

3. Compile with Inno Setup
4. Creates `wayback-setup.exe`

### Using NSIS

Similar process with [NSIS](https://nsis.sourceforge.io/)

---

## Distribution Checklist

Before releasing:

- [ ] Build in **Release** mode
- [ ] Test on clean Windows install (VM recommended)
- [ ] Verify no missing DLL errors
- [ ] Test hotkey registration
- [ ] Test tray icon appears
- [ ] Test config file creation
- [ ] Test exit via tray menu
- [ ] Check file size (~500KB reasonable)
- [ ] Include README.md
- [ ] Optional: Sign executable (for SmartScreen)

---

## Code Signing (Optional)

To avoid Windows SmartScreen warnings:

1. Obtain code signing certificate
2. Sign the executable:

```powershell
signtool sign /f certificate.pfx /p password /t http://timestamp.digicert.com wayback.exe
```

**Note:** Code signing certificates cost money. For personal use, users can ignore SmartScreen.

---

## GitHub Releases (Recommended)

### Creating a Release

1. Tag the version:
```powershell
git tag v1.0
git push origin v1.0
```

2. Create GitHub Release
3. Upload `wayback.exe` as release asset
4. Include README.md and CHANGELOG

### Release Assets

Upload:
- `wayback.exe` - Standalone executable
- `wayback-v1.0.zip` - Zip with exe + README
- `wayback-setup.exe` - Optional installer

---

## Versioning

Update version in:
- `TrayIcon.cpp` - About dialog
- `README.md` - Documentation
- Git tags
- Installer scripts (if using)

---

## Size Optimization

Current size: ~500KB (Release build)

To reduce further:
```cmake
# In CMakeLists.txt
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /O1")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF")
endif()
```

Or use UPX packer (not recommended - may trigger antivirus):
```powershell
upx --best wayback.exe
```

---

## Testing the Distribution

### Minimal Test

1. Copy `wayback.exe` to Desktop
2. Double-click to run
3. Check system tray for icon
4. Test hotkey (Ctrl+Alt+N)
5. Right-click tray → Exit

### Full Test

1. Clean Windows 10/11 VM
2. Copy only `wayback.exe`
3. Run it
4. Verify all functionality
5. Check Task Manager (should be ~5MB RAM)
6. Leave running for 10 minutes
7. Verify no memory leaks

---

## Distribution Formats

Choose based on audience:

1. **Standalone EXE** - Simplest, for technical users
2. **ZIP Package** - EXE + README, easy to share
3. **Installer** - Best UX, for non-technical users
4. **Portable** - No installation, run from USB

Recommendation: **Start with ZIP**, add installer later if needed.

---

## Sharing the Binary

Safe to share via:
- GitHub Releases
- Personal website
- Direct download link
- Cloud storage (OneDrive, Dropbox)

**Security note:** Users will see SmartScreen warning for unsigned binaries. This is normal for new applications.

---

## Final Notes

The Release build is **production-ready** and can be shared immediately.

No additional runtime dependencies means **maximum compatibility** across Windows 10/11 systems.

For most users, just share `wayback.exe` + `README.md` in a ZIP file.
