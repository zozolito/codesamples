# C++ Calculator — Windows Terminal

> Part of the **CodeSamples Calculator Project** · see [`/CALCULATOR.md`](../../../../CALCULATOR.md) for the full spec and cross-language comparison.

A desktop calculator for the Windows console, written in C++17. It implements the full feature set from the calculator specification: memory functions, operation history, CE/C/⌫ correction, %, √, and 1/x — all running inside a coloured, box-drawn terminal UI.

---

## What you will learn from this project

- **Engine/UI separation** — the `Calculator` class knows nothing about terminals, colours, or keyboards. It could power a Qt GUI or a web server without changing a line.
- **State machine design** — four states (`EnteringFirst`, `OperatorPressed`, `EnteringSecond`, `Error`) cover every calculator behaviour precisely.
- **Windows console APIs** — enabling ANSI VT processing, UTF-8 output, `_getch()` for raw keystrokes.
- **Output buffering** — why writing to `ostringstream` first and flushing once eliminates screen flicker.
- **Floating-point formatting** — why `%.12g` hides `0.1 + 0.2 = 0.30000000000000004` without special-casing.
- **`enum class`** — why scoped enums prevent bugs that plain `enum` silently allows.

---

## Prerequisites

| Tool | Version | Notes |
|---|---|---|
| **MinGW-w64** (g++) | 13+ | Provides `g++`, `gdb`. Must be on `PATH`. |
| **Windows** | 10 1511+ | ANSI escape sequences require Windows 10 Threshold 2 or later. |
| **VS Code** | Any recent | Optional — you can build from the command line too. |
| **C/C++ extension** | ms-vscode.cpptools | Required for IntelliSense and the debugger inside VS Code. |

### Installing MinGW-w64 (recommended method)

```powershell
# Via winget (Windows Package Manager — built into Windows 11, available for 10)
winget install -e --id MSYS2.MSYS2

# Then, in the MSYS2 terminal:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb

# Add to PATH: C:\msys64\mingw64\bin
```

Alternatively, download directly from [winlibs.com](https://winlibs.com/) — pre-built MinGW-w64 without requiring MSYS2.

Verify the installation:
```
g++ --version
gdb --version
```

---

## Building

### Option 1 — VS Code (recommended)

1. Open the `terminal/` folder in VS Code (`File → Open Folder`)
2. VS Code will prompt to install recommended extensions — accept
3. Press **Ctrl+Shift+B** to run the default build task (`build (debug)`)
4. The compiled binary is placed in `bin/calculator.exe`

To run immediately after building:
- Open the Command Palette (`Ctrl+Shift+P`) → `Tasks: Run Task` → `run`

### Option 2 — Command line

```bash
# From the terminal/ folder:

# Create output directory (first time only)
mkdir bin

# Debug build (with debug symbols — use this for development)
g++ -std=c++17 -Wall -Wextra -g -o bin/calculator.exe src/engine/Calculator.cpp src/main.cpp

# Release build (optimised, stripped)
g++ -std=c++17 -Wall -Wextra -O2 -DNDEBUG -s -o bin/calculator.exe src/engine/Calculator.cpp src/main.cpp

# Run
bin\calculator.exe
```

### Option 3 — Single-line (quick test)

```bash
g++ -std=c++17 -o calc.exe src/engine/Calculator.cpp src/main.cpp && calc.exe
```

---

## Running

The calculator **must run in a real Windows console** (Command Prompt, PowerShell, or Windows Terminal). It uses:
- `_getch()` — raw keypress reading (no Enter required)
- ANSI escape sequences — colours and cursor positioning
- Unicode characters — `×`, `÷`, `√`, `−`, `⌫`

> **Windows Terminal** (the modern one from the Microsoft Store) gives the best results. The classic `cmd.exe` window works but may render box-drawing characters less crisply.

```
bin\calculator.exe
```

Press **Q** to quit.

---

## Key Mapping

| Key | Action |
|---|---|
| `0` – `9` | Digit |
| `.` | Decimal point |
| `+` `-` `*` `/` | Add / Subtract / Multiply / Divide |
| `=` or `Enter` | Evaluate |
| `N` | ± Negate (flip sign) |
| `%` | Percent (context-sensitive — see below) |
| `S` | √ Square root |
| `I` | 1/x Reciprocal |
| `Backspace` (`⌫`) | Delete last digit |
| `Escape` | C — full clear |
| `Delete` (or `F2`) | CE — clear current entry |
| `F1` | MS — memory store |
| `F2` | MR — memory recall |
| `F3` | M+ — memory add |
| `F4` | M− — memory subtract |
| `F5` | MC — memory clear |
| `H` | Clear history |
| `Q` | Quit |

### Percent behaviour

The `%` key follows Windows Calculator semantics:

| Context | Calculation | Example |
|---|---|---|
| After `+` or `−` | `first × (second ÷ 100)` | `200 + 10%` → `200 + 20` → `220` |
| After `×` or `÷` | `second ÷ 100` | `200 × 10%` → `200 × 0.1` → `20` |
| No operator | `0` | |

---

## Project Structure

```
terminal/
├── src/
│   ├── engine/
│   │   ├── Calculator.hpp   ← Public interface (the contract)
│   │   └── Calculator.cpp   ← Engine logic (pure C++, no Windows headers)
│   └── main.cpp             ← Windows terminal UI (rendering, keyboard input)
├── bin/                     ← Created by build, ignored by git
│   └── calculator.exe
├── .vscode/
│   ├── tasks.json           ← Build tasks (Ctrl+Shift+B)
│   ├── launch.json          ← Debugger (F5)
│   ├── extensions.json      ← Recommended extensions
│   └── settings.json        ← IntelliSense configuration
├── .gitignore
└── README.md
```

---

## Architecture

```
┌──────────────────────────────────────┐
│  main.cpp  (UI layer)                │
│                                      │
│  setupConsole()  — Windows APIs      │
│  render()        — draws the screen  │
│  handleKey()     — reads keystrokes  │
│                                      │
│  Calls: calc.pressDigit('3')         │
│         calc.pressOperator(Op::Add)  │
│         calc.getView() → CalcView    │
└──────────────────┬───────────────────┘
                   │  CalcView (snapshot)
                   │  press*() (events)
                   ▼
┌──────────────────────────────────────┐
│  Calculator  (engine)                │
│                                      │
│  State machine:                      │
│    EnteringFirst                     │
│    OperatorPressed                   │
│    EnteringSecond                    │
│    Error                             │
│                                      │
│  No Windows headers.                 │
│  No terminal knowledge.              │
│  Fully testable in isolation.        │
└──────────────────────────────────────┘
```

### Why this separation matters

The engine (`Calculator.cpp`) compiles cleanly on Linux with no changes. The UI (`main.cpp`) is the only file that touches Windows-specific APIs. If you wanted to add a Qt GUI or a web API endpoint, you would write a new UI file and reuse the engine unchanged.

This is the principle that makes large software maintainable: **boundaries between what something does and how it is presented.**

---

## Key implementation notes

### `enum class` instead of `enum`

```cpp
// Old style — dangerous
enum Operator { None, Add, Subtract };
int x = Add;   // compiles silently — integer comparison is too easy

// New style (C++11) — scoped, explicit
enum class Operator { None, Add, Subtract };
Operator op = Operator::Add;   // must use the scope prefix
// int x = Operator::Add;      // ← compiler error: no implicit conversion
```

Scoped enums prevent entire classes of bugs where two unrelated enums happen to share a numeric value and compare equal by accident.

### Output buffering

The render loop builds the entire frame into an `std::ostringstream`, then writes it to `std::cout` in one call. Without this, each individual `<<` call triggers a write syscall, and you see the screen partially redrawn — a visible "tear" between the old and new frames.

```cpp
std::ostringstream buf;
// ... build everything into buf ...
std::cout << buf.str();   // one atomic write
std::cout.flush();
```

### Floating-point display

```cpp
snprintf(buf, 64, "%.12g", value);
```

`%.12g` prints up to 12 significant digits, dropping trailing zeros. The infamous `0.1 + 0.2 = 0.30000000000000004` only differs from `0.3` at the 17th significant digit — `%.12g` never reaches it, so the display shows `0.3` naturally.

This is a display fix, not a computation fix. The internal `double` still holds the imprecise value. See `CALCULATOR.md` for the full floating-point discussion.

### ANSI on Windows

ANSI escape sequences have worked in the Windows console since Windows 10 version 1511 (November 2015), but they must be explicitly enabled:

```cpp
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD mode = 0;
GetConsoleMode(hOut, &mode);
SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
```

Without this call, the escape sequences print as literal characters instead of moving the cursor or setting colours.

---

## Debugging tips

**Set a breakpoint in `Calculator::pressEquals()`** and step through it (F10). You will see exactly which branch of the state machine handles the repeated-`=` case (`justPressedEquals_`), the operator-then-`=` case, and the normal case.

**Watch `state_` in the Variables panel** while pressing keys. The state machine transitions are the heart of how the calculator decides what each keystroke means.

**Inspect `history_`** to see how each operation is recorded as a string.

To start with execution paused at `main()`, use the **"Debug: Calculator (stop on entry)"** launch configuration from the Run and Debug panel (Ctrl+Shift+D).

---

*Part of the [CodeSamples](../../../../README.md) project — free, explained code for people who want to actually understand what they are writing.*
