# C++ Memory Management — VS Code Project

> ⚠️ **The most important C++ project in this repository.**
> Memory errors are the #1 source of security vulnerabilities in C/C++ software.
> This project teaches you to recognise, avoid, and detect every class of memory bug.

---

## 🇬🇧 English

### What this project covers

Nine progressive parts, each executable and annotated:

| Part | Topic |
|---|---|
| 1 | Stack vs Heap — what lives where and why it matters |
| 2 | Raw pointers — pointer arithmetic, dereferencing, basics |
| 3 | Memory errors — leak, double-free, dangling pointer, buffer overflow, null dereference |
| 4 | RAII — the fundamental C++ solution to resource management |
| 5 | `unique_ptr` — sole ownership, move semantics, custom deleters |
| 6 | `shared_ptr` + `weak_ptr` — reference counting, cycle detection |
| 7 | Rule of 5 — writing your own RAII class with all 5 special member functions |
| 8 | `std::vector` vs raw arrays — why the standard library is always safer |
| 9 | Detection tools — AddressSanitizer, UBSan, Valgrind, cppcheck |

---

### Project structure

```
memory-management/
├── src/
│   └── memory.cpp         ← C++ source — one file, nine sections
├── bin/                   ← compiled binary (git-ignored)
├── .vscode/
│   ├── tasks.json         ← Build+ASan / Build for Valgrind / Run / Valgrind / cppcheck
│   ├── launch.json        ← GDB (Linux/WSL) and LLDB (macOS) debug
│   ├── extensions.json    ← recommended extensions
│   └── settings.json      ← C++17, sanitizer flags in IntelliSense
├── .gitignore
└── README.md
```

---

### Prerequisites

**Linux (Debian/Ubuntu)**
```bash
sudo apt update
sudo apt install g++ gdb build-essential valgrind cppcheck
g++ --version
valgrind --version
```

**macOS**
```bash
xcode-select --install           # clang++ + lldb
brew install valgrind cppcheck   # valgrind support on macOS is limited
```

**Windows (WSL — strongly recommended)**
```bash
# Inside WSL (Ubuntu):
sudo apt install g++ gdb build-essential valgrind cppcheck
```

**Windows (native — no Valgrind)**
```
MSYS2/MinGW-w64: pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb
Note: Valgrind is not available on Windows natively.
Use AddressSanitizer instead (-fsanitize=address).
```

---

### Building and running

#### Option A — with AddressSanitizer (recommended)

AddressSanitizer is built into g++/clang++. It catches memory bugs *as they happen* with precise diagnostics.

```bash
mkdir -p bin

g++ -std=c++17 -Wall -Wextra -Wpedantic \
    -g -fsanitize=address,undefined -fno-omit-frame-pointer \
    -o bin/memory src/memory.cpp

./bin/memory
```

Or use **`Ctrl+Shift+B`** in VS Code (default task).

**If ASan finds an error**, you'll see output like:
```
==12345==ERROR: AddressSanitizer: heap-use-after-free on address 0x...
  READ of size 4 at 0x... thread T0
    #0 0x... in main /path/to/memory.cpp:87
```
ASan prints the exact file and line number. Zero ambiguity.

#### Option B — with Valgrind (Linux)

Valgrind is more thorough but slower (~10–50×). Run it on a build **without** `-fsanitize`:

```bash
# Build without sanitizers
g++ -std=c++17 -g -O0 -o bin/memory src/memory.cpp

# Run under Valgrind
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         ./bin/memory
```

Or use the task **"C++: Run under Valgrind (Linux)"** in VS Code.

**Reading Valgrind output:**
```
==12345== HEAP SUMMARY:
==12345==     in use at exit: 4 bytes in 1 blocks      ← leak!
==12345==   total heap usage: 3 allocs, 2 frees

==12345== 4 bytes in 1 blocks are definitely lost
==12345==    at 0x...: operator new(unsigned long)
==12345==    by 0x...: main (memory.cpp:45)            ← where you allocated

definitely lost   = real leak — fix it
indirectly lost   = caused by another leak
possibly lost     = likely a leak
still reachable   = freed at exit — not technically a leak
```

---

### VS Code tasks summary

| Task | What it does |
|---|---|
| `C++: Build with AddressSanitizer` *(default)* | Builds with `-fsanitize=address,undefined` |
| `C++: Build for Valgrind` | Builds with `-g -O0`, no sanitizers |
| `C++: Build Windows` | Builds on Windows (no ASan) |
| `C++: Run` | Builds + runs |
| `C++: Run under Valgrind (Linux)` | Builds + runs under Valgrind |
| `C++: Static analysis (cppcheck)` | Static analysis without running |
| `C++: Clean` | Deletes the binary |

---

### Debugging memory code

1. Open `src/memory.cpp`
2. Set a breakpoint inside any `demo_*` function
3. Press **F5** (GDB or LLDB config)
4. In the **DEBUG CONSOLE**, inspect memory:

```gdb
(gdb) print *ptr              # dereference a pointer
(gdb) print ptr               # print the address
(gdb) x/4xw ptr               # examine 4 words (32-bit) in hex at ptr
(gdb) watch ptr               # break whenever ptr changes
(gdb) info locals             # all variables in current frame
(gdb) backtrace               # call stack
```

**Tip:** When you see a crash, type `backtrace` in GDB immediately — it shows you exactly which line caused the crash.

---

### Key concepts summary

#### The memory error matrix

| Error | Cause | Symptom | Fix |
|---|---|---|---|
| **Memory leak** | `new` without `delete` | Process grows, OS reclaims at exit | `unique_ptr` |
| **Double free** | `delete` called twice | Heap corruption, crash | `unique_ptr` |
| **Use-after-free** | Access after `delete` | UB, crash, security hole | `unique_ptr` |
| **Dangling pointer** | Pointer to destroyed object | UB, silent corruption | Smart pointers or references |
| **Buffer overflow** | Write beyond allocation | Adjacent memory corruption | `std::vector::at()`, `std::array` |
| **Null dereference** | Dereference `nullptr` | Segmentation fault | Null check, `optional` |
| **Wrong delete** | `delete` on `new[]` | UB | `make_unique<T[]>(n)` |
| **Uninitialised read** | Use before write | UB, random values | Always initialise |

#### RAII: why it eliminates most of the above

```
Without RAII:                    With RAII (smart pointers):
─────────────────────────────    ─────────────────────────────
int* p = new int(5);             auto p = make_unique<int>(5);
try {                            do_work(*p);
    do_work(*p);                 // p auto-deleted when scope exits
    delete p;    ← may never    // even if exception is thrown
} catch (...) {                  // no manual delete possible
    delete p;    ← duplicated    // no double-free possible
}                                // no leak possible
```

#### Smart pointer decision tree

```
Do you need heap allocation?
├── No  → use stack (int x, std::string s, std::array)
└── Yes → who owns it?
    ├── One owner → unique_ptr<T>   (make_unique<T>())
    ├── Multiple owners → shared_ptr<T>   (make_shared<T>())
    └── Non-owning observer → raw pointer or weak_ptr
```

---

### Manual compilation reference

```bash
# All safety checks on (use during learning)
g++ -std=c++17 -Wall -Wextra -Wpedantic \
    -g -fsanitize=address,undefined -fno-omit-frame-pointer \
    -o bin/memory src/memory.cpp

# For Valgrind (Linux — incompatible with -fsanitize)
g++ -std=c++17 -g -O0 -o bin/memory src/memory.cpp
valgrind --leak-check=full --track-origins=yes ./bin/memory

# Static analysis (no compilation needed)
cppcheck --enable=all --std=c++17 src/
```

| Flag | Meaning |
|---|---|
| `-fsanitize=address` | AddressSanitizer: heap/stack overflow, use-after-free, leak |
| `-fsanitize=undefined` | UBSan: signed overflow, null deref, misaligned access |
| `-fno-omit-frame-pointer` | Keeps frame pointers for accurate stack traces |
| `-O0` | No optimisation — required for accurate Valgrind results |
| `-g` | Debug symbols — required for file:line info in error reports |

---

### Understanding the source: key patterns

```cpp
// ── RAII in action ──
// The resource is tied to the object's lifetime.
// When the object dies, the resource is freed. Always.
class RAIIFile {
    FILE* file_;
public:
    RAIIFile(const char* path) : file_(fopen(path, "r")) {}
    ~RAIIFile() { if (file_) fclose(file_); }  // automatic, always runs
};

// ── unique_ptr sole ownership ──
auto w = std::make_unique<Widget>(42, "button");
// w owns the Widget. No one else can.
// When w leaves scope: ~Widget() is called. Automatically.

// ── Move semantics — transfer without copy ──
auto w2 = std::move(w);   // w is now null. w2 owns the Widget.
// O(1) — just a pointer swap. No allocation. No copy.

// ── Rule of 5 — the copy/move symmetry ──
// If you define ANY of: destructor, copy ctor, copy=, move ctor, move=
// you should define ALL FIVE.
// Asymmetry leads to double-free, shallow copy, or leaked resources.
```

---

## 🇫🇷 Français

### Ce que couvre ce projet

Neuf parties progressives sur la gestion mémoire en C++ :

| Partie | Sujet |
|---|---|
| 1 | Stack vs Heap — ce qui vit où et pourquoi c'est important |
| 2 | Pointeurs bruts — arithmétique, déréférencement |
| 3 | Erreurs mémoire — fuite, double libération, pointeur fantôme, débordement |
| 4 | RAII — la solution fondamentale du C++ |
| 5 | `unique_ptr` — propriété exclusive |
| 6 | `shared_ptr` + `weak_ptr` — propriété partagée et cycles |
| 7 | Règle des 5 — écrire sa propre classe RAII |
| 8 | `std::vector` vs tableaux bruts |
| 9 | Outils de détection — ASan, UBSan, Valgrind, cppcheck |

### Compiler et exécuter

```bash
# Linux / macOS / WSL
mkdir -p bin
g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined \
    -o bin/memory src/memory.cpp
./bin/memory
```

### Pourquoi la gestion mémoire est critique

Les erreurs mémoire sont responsables de la grande majorité des CVE (failles de sécurité) dans les logiciels C/C++ : buffer overflow, use-after-free, double-free. Ces failles permettent l'exécution de code arbitraire, l'escalade de privilèges, et les fuites de données.

C++ moderne avec les smart pointers et le RAII **élimine structurellement** la quasi-totalité de ces erreurs — pas par discipline, mais parce que le code correct devient le code naturel.

### Les règles d'or

1. **Ne jamais utiliser `new`/`delete` manuellement** — utiliser `make_unique` / `make_shared`
2. **Règle des 5** : si vous définissez un destructeur, définir aussi copy ctor, copy=, move ctor, move=
3. **`noexcept`** sur toutes les opérations de déplacement (move)
4. **`std::vector`** ou `std::array` au lieu des tableaux bruts
5. **Compiler avec `-fsanitize=address,undefined`** en développement
6. **Exécuter Valgrind** avant chaque mise en production
7. **0 erreur ASan = vous faites ça correctement**
