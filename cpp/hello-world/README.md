# C++ Hello World — VS Code Project

A ready-to-run C++ project using **g++** (GCC) or **clang++** and **VS Code**.

---

## 🇬🇧 English

### What this project does

A C++ program demonstrating modern C++17 idioms:
- **Abstract base class** (`Shape`) with pure virtual methods
- **Derived classes** (`Circle`, `Rectangle`, `Triangle`) with `override`
- **Polymorphism** via `std::unique_ptr<Shape>` vector — no slicing, no manual `delete`
- **RAII** — all memory is managed automatically by `unique_ptr` and `std::string`
- **Lambdas** — sorting shapes by area
- **STL algorithms** — `std::sort`, `std::accumulate`, `std::max_element`
- **`std::optional`** — safe computation without exceptions
- **Structured bindings** (C++17) — `auto& [name, score]`
- **Templates** — `clamp_value<T>()`

---

### Project structure

```
hello-world/
├── src/
│   └── hello.cpp         ← C++ source (C++17)
├── bin/                  ← compiled binary (git-ignored)
├── .vscode/
│   ├── tasks.json        ← Build (g++/clang++) / Release / Run / Clean
│   ├── launch.json       ← GDB (Linux/WSL) and LLDB (macOS) debug
│   ├── extensions.json   ← recommended VS Code extensions
│   └── settings.json     ← C++ standard, IntelliSense, rulers
├── .gitignore
└── README.md
```

---

### Prerequisites

#### Compiler

**Linux (Debian/Ubuntu)**
```bash
sudo apt update && sudo apt install g++ gdb build-essential
g++ --version
```

**macOS**
```bash
xcode-select --install        # installs clang++ and LLDB
# or with Homebrew:
brew install gcc llvm
```

**Windows — Option A: WSL (recommended)**
```bash
# Inside WSL (Ubuntu):
sudo apt install g++ gdb build-essential
```

**Windows — Option B: MSYS2 / MinGW-w64**
```
1. Download MSYS2 from https://www.msys2.org/
2. Run: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb
3. Add C:\msys64\mingw64\bin to your PATH
```

#### VS Code extensions

Accept the recommended extensions when VS Code prompts you, or install manually:

| Extension | ID | Purpose |
|---|---|---|
| **C/C++** | `ms-vscode.cpptools` | IntelliSense, GDB/LLDB debugger |
| **C/C++ Extension Pack** | `ms-vscode.cpptools-extension-pack` | Bundle with CMake tools |
| **clangd** | `llvm-vs-code-extensions.vscode-clangd` | Better code completion (optional) |

---

### Running the project

1. Open `hello-world/` in VS Code
2. Create the `bin/` directory if it doesn't exist:
   ```bash
   mkdir -p bin
   ```
3. Build: `Ctrl+Shift+B` (default task — g++ with AddressSanitizer)
4. Run in terminal:
   ```bash
   ./bin/hello
   ```

#### Expected output

```
============================================
  C++ Hello World — CodeSamples Project
============================================

Enter your name: Alice

 1. Hello, Alice!
 2. Hello, Alice!
 3. Hello, Alice!

--- Shapes (polymorphism + RAII via unique_ptr) ---
  Circle     | area=78.54 | perimeter=31.42
  Rectangle  | area=24.00 | perimeter=20.00
  Triangle   | area=6.00  | perimeter=12.00
  Circle     | area=19.63 | perimeter=15.71
  Rectangle  | area=20.00 | perimeter=24.00

--- Sorted by area ---
  Triangle    area = 6.00
  ...

--- std::optional ---
  sqrt(16.0) = 4.00
  sqrt(-4.0) = undefined (negative input)
  ...
```

---

### Debugging

1. Set a breakpoint by clicking to the left of a line number in `src/hello.cpp`
2. Press **F5** → selects the GDB config (Linux/WSL) or LLDB (macOS)
3. Execution stops at `main()` (`stopAtEntry: true`)
4. Use:
   - **F10** — step over
   - **F11** — step into
   - **F5** — continue
   - Hover over variables to inspect their values

#### Watching STL containers in GDB

```gdb
(gdb) print shapes            # prints the vector
(gdb) print shapes[0]         # first element (unique_ptr)
(gdb) print *shapes[0]        # dereference the unique_ptr
(gdb) info locals             # all local variables
```

---

### Manual compilation

```bash
# Development (sanitizers catch memory errors)
g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined \
    -o bin/hello src/hello.cpp

# Release (optimised)
g++ -std=c++17 -O2 -DNDEBUG -o bin/hello src/hello.cpp

# clang++
clang++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined \
    -o bin/hello src/hello.cpp

./bin/hello
```

| Flag | Effect |
|---|---|
| `-std=c++17` | Use C++17 standard |
| `-Wall -Wextra` | Enable all warnings — treat warnings seriously |
| `-g` | Debug symbols |
| `-fsanitize=address` | AddressSanitizer — catches memory bugs at runtime |
| `-fsanitize=undefined` | UBSan — catches undefined behaviour |
| `-O2` | Optimise for speed (release builds) |

---

### Understanding the source code

```cpp
// Pure virtual method — class cannot be instantiated
virtual double area() const = 0;

// override — compiler verifies we're genuinely overriding
double area() const override { return M_PI * r_ * r_; }

// Virtual destructor — REQUIRED on base classes
// Without it: deleting via base pointer = undefined behaviour
virtual ~Shape() = default;

// make_unique — never use raw 'new' in modern C++
auto circle = std::make_unique<Circle>(5.0);
// circle is deleted automatically when it leaves scope

// Lambda — inline function object
std::sort(shapes.begin(), shapes.end(),
    [](const auto& a, const auto& b) { return a->area() < b->area(); }
);

// std::optional — a value that might not exist
std::optional<double> result = safe_sqrt(-4.0);
if (result) { use(*result); }   // check before use
```

---

## 🇫🇷 Français

### Ce que fait ce projet

Un programme C++ démontrant les idiomes modernes C++17 :
- **Classe de base abstraite** (`Shape`) avec méthodes virtuelles pures
- **Classes dérivées** (`Circle`, `Rectangle`, `Triangle`) avec `override`
- **Polymorphisme** via `std::unique_ptr<Shape>` — pas de slicing, pas de `delete` manuel
- **RAII** — toute la mémoire est gérée automatiquement
- **Lambdas** — tri des formes par aire
- **Algorithmes STL** — `std::sort`, `std::accumulate`, `std::max_element`
- **`std::optional`** — calcul sûr sans exception
- **Structured bindings** (C++17)
- **Templates** — `clamp_value<T>()`

### Prérequis

**Linux (Debian/Ubuntu)**
```bash
sudo apt install g++ gdb build-essential
```

**macOS**
```bash
xcode-select --install   # ou: brew install gcc
```

**Windows** : utiliser WSL (Ubuntu) ou MSYS2/MinGW-w64.

### Compiler et exécuter

```bash
mkdir -p bin
g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined \
    -o bin/hello src/hello.cpp
./bin/hello
```

### Pourquoi `-fsanitize=address` ?

AddressSanitizer (ASan) est intégré à g++ et clang++. Il détecte en temps réel :
- les fuites mémoire
- les accès après libération (use-after-free)
- les débordements de tampon (buffer overflow)

Coût : ~2× plus lent — à utiliser pendant le développement, pas en production.

### Déboguer

1. Poser un point d'arrêt dans `src/hello.cpp`
2. Appuyer sur **F5** → GDB (Linux/WSL) ou LLDB (macOS)
3. **F10** = pas à pas, **F11** = entrer dans une fonction, **F5** = continuer
