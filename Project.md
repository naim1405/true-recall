# Project Context: Per-Monitor Focus Switching (Windows Prototype)

## Intent

Build a **personal-use prototype** that emulates **niri-style per-monitor focus behavior** on Windows.

This is **not a product**, not a window manager, and not a release-ready tool.
The goal is to quickly achieve a workflow improvement for the author’s daily multi-monitor usage.

---

## Core Idea

Windows uses a **global focus model**, which makes multi-monitor multitasking inefficient compared to Wayland window managers like **niri**, where each monitor maintains its own focus history.

This project builds a **small background utility** that:

* Tracks **focus history per physical monitor**
* Lets the user **switch monitors using hotkeys**
* When switching monitors, **automatically focuses the last active window on that monitor**
* Makes it feel like each monitor “remembers” what was active there

This is a **focus memory + restoration layer**, not a window manager.

---

## Target User Experience

* Keyboard-driven
* No visible UI during normal use
* Fast and predictable
* Switching monitors instantly brings the correct app into focus
* Actions primarily affect the currently selected monitor

The experience should feel **close to niri**, within Windows limitations.

---

## Scope

### In Scope

* Track foreground window changes
* Associate windows with monitors
* Maintain a per-monitor MRU (focus stack)
* Define hotkeys for monitor switching
* Best-effort window activation
* Optional mouse repositioning

### Out of Scope

* Tiling or layout management
* Replacing DWM or system Alt-Tab
* Virtual desktops
* Fullscreen game handling
* UI configuration systems
* Release, packaging, installers

---

## Reality Constraints (Windows)

* Focus is **global**, not per monitor
* `SetForegroundWindow` may fail
* Some apps cannot be focused reliably
* Reliability is **best-effort**, not guaranteed

These limitations are accepted.

---

## Design Philosophy

* Minimal
* Direct
* Pragmatic
* Stable over clever
* Silent failure over wrong behavior

---

## DO NOT OVERENGINEER (HARD RULES)

* Do NOT attempt to become a window manager
* Do NOT fight Windows focus restrictions aggressively
* Do NOT solve edge cases first (UWP, games, elevation)
* Do NOT add UI, config systems, plugins, or abstractions
* Do NOT over-optimize or over-architect
* Do NOT aim for perfect niri parity

If unsure, **do nothing** rather than doing the wrong thing.

---

## Success Criteria (Prototype)

* Hotkeys switch between monitors
* The last active window on that monitor is focused most of the time
* The workflow feels meaningfully better than stock Windows
* The tool is usable daily by the author

---

## Guiding Principle

> This project exists to reduce cognitive friction during multi-monitor multitasking, not to explore OS internals or build a framework.
