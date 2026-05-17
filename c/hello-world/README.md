# C Hello World — VS Code Project

A ready-to-run C project using **gcc** (or **clang**) and **VS Code**.

---

## 🇬🇧 English

### What this project does

A C program demonstrating modern C11 idioms:
- **Structs + typedef** — `Shape`, `ShapeList`, `Employee`
- **Dynamic array** — manual `malloc`/`realloc`/`free` with correct realloc pattern
- **Function pointers** — `MathFunc`, array of function pointers, `qsort` callback
- **Safe string handling** — `fgets`, `snprintf`, `strncpy` with explicit null-termination
- **Enums** — `Status`, `ShapeType`
- **Error handling** — return-code convention, `goto`-cleanup pattern
- **`calloc`** — zero-initialised allocation
- **Fixed-width integers** — `uint32_t`, `int64_t`, `PRId64`
- **`assert()`** — debug invariant checking
- **`const` correctness** — read-only parameters

### Project structure

```
hello-world/
├── src/
│   └── hello.c           ← C source (C11), heavily annotated
├── bin/                  ← compiled binary (git-ignored)
├── .vscode/
│   ├── tasks.json        ← Build (gcc+ASan / release / clang) / Run / Valgrind
│   ├── launch.json       ← GDB (Linux/WSL) and LLDB (macOS) debug
│   ├── extensions.json   ← C/C++ extension
│   └── settings.json     ← C11 standard, IntelliSense
├── .gitignore
└── README.md
```

### Prerequisites

**Linux (Debian/Ubuntu)**
```bash
sudo apt install gcc gdb build-essential valgrind
gcc --version
```

**macOS**
```bash
xcode-select --install
```

**Windows (WSL)**
```bash
sudo apt install gcc gdb build-essential valgrind
```

**Windows (MSYS2/MinGW-w64)**
```
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb
```

### Building and running

```bash
mkdir -p bin

# Development (AddressSanitizer enabled)
gcc -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 \
    -g -fsanitize=address,undefined -fstack-protector-strong \
    -lm \
    -o bin/hello src/hello.c

./bin/hello
```

Or **`Ctrl+Shift+B`** in VS Code (default task).

### Debugging

1. Set a breakpoint in `src/hello.c`
2. Press **F5** → GDB (Linux/WSL) or LLDB (macOS)
3. **F10** step over · **F11** step into · **F5** continue

#### Useful GDB commands for C

```gdb
(gdb) print shapes           # print struct
(gdb) print shapes.count     # struct member
(gdb) print shapes.items[0]  # array element
(gdb) x/10d shapes.items     # examine 10 ints as decimal
(gdb) info locals            # all local variables
(gdb) watch ptr              # break when pointer changes
```

### Compiler flags

| Flag | Purpose |
|---|---|
| `-std=c11` | C11 standard |
| `-Wall -Wextra -Wpedantic` | All warnings — fix every one |
| `-Wshadow` | Detect variable shadowing |
| `-Wformat=2` | Strict printf/scanf format checking |
| `-g` | Debug symbols |
| `-fsanitize=address` | AddressSanitizer — catches leaks, overflow, use-after-free |
| `-fsanitize=undefined` | UBSan — catches integer overflow, null deref |
| `-fstack-protector-strong` | Stack canary — detects stack buffer overflows |
| `-lm` | Link math library (for `sqrt`, `M_PI`) |

---

## 🇫🇷 Français

### Ce que fait ce projet

Un programme C démontrant les idiomes modernes C11 :
- **Structs + typedef** — `Shape`, `ShapeList`
- **Tableau dynamique** — `malloc`/`realloc`/`free` avec le bon pattern `realloc`
- **Pointeurs de fonctions** — `MathFunc`, tableau de fonctions, callback pour `qsort`
- **Gestion de chaînes sûre** — `fgets`, `snprintf`, `strncpy` avec null-termination explicite
- **Gestion d'erreurs** — convention codes de retour, pattern `goto`-nettoyage
- **Entiers à largeur fixe** — `uint32_t`, `int64_t`, `PRId64`

### Compiler et exécuter

```bash
mkdir -p bin
gcc -std=c11 -Wall -Wextra -g -fsanitize=address,undefined \
    -lm -o bin/hello src/hello.c
./bin/hello
```

### Sécurité

Ce code utilise :
- `fgets()` — pas `gets()` (supprimé en C11)
- `snprintf()` — pas `sprintf()`
- `strncpy()` avec null-termination explicite
- `calloc()` pour les buffers initialisés à zéro
- Compile avec `-fsanitize=address` pour détecter les erreurs mémoire à l'exécution
