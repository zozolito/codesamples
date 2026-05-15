# BASIC Hello World — VS Code Project

A ready-to-run BASIC project using **QB64-PE** (the modern, actively maintained BASIC compiler) and **VS Code**.

---

## 🇬🇧 English

### What this project does

A small interactive BASIC program that:
- Displays a welcome banner via a `SUB` procedure
- Reads your name with `LINE INPUT` and greets you three times using a `FOR/NEXT` loop
- Reads a radius and computes the area of a circle using `PRINT USING` for formatted output
- Demonstrates `OPTION _EXPLICIT`, `DIM`, `SUB`, `TRIM$`, and the classic ATN(1)*4 trick for π

---

### Project structure

```
hello-world/
├── src/
│   └── hello.bas          ← BASIC source (QB64-PE / QB64 compatible)
├── bin/                   ← compiled binary (git-ignored)
├── .vscode/
│   ├── tasks.json         ← Build (QB64-PE / FreeBASIC) / Run / Clean
│   ├── launch.json        ← GDB debug via FreeBASIC
│   ├── extensions.json    ← recommended VS Code extensions
│   └── settings.json      ← .bas file association
├── .gitignore
└── README.md
```

---

### Which compiler to use?

| Compiler | Best for | Link |
|---|---|---|
| **QB64-PE** | QuickBASIC-compatible code, graphics, sound, GUI | https://github.com/QB64-Phoenix-Edition/QB64pe |
| **FreeBASIC** | Systems programming, GDB debugging, C interop | https://www.freebasic.net |

This project's source is compatible with both. **QB64-PE is recommended** for learning — it has the best compatibility with classic BASIC and the best Windows support out of the box.

---

### Prerequisites

#### Option A: QB64-PE (recommended)

**Windows**
1. Download the latest release from https://github.com/QB64-Phoenix-Edition/QB64pe/releases
2. Extract the archive (e.g. `C:\QB64PE\`)
3. Add `C:\QB64PE\` to your `PATH` environment variable
4. Verify: open a new terminal and type `qb64pe --version`

**Linux (Debian / Ubuntu)**
```bash
# Download and extract from the releases page, then:
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
# Add the extracted folder to PATH in ~/.bashrc
export PATH="$PATH:/opt/qb64pe"
```

**macOS**
```bash
# Download the macOS release from the releases page
# Extract and add to PATH in ~/.zshrc
export PATH="$PATH:/Applications/QB64PE"
```

#### Option B: FreeBASIC (better for GDB debugging)

**Linux (Debian / Ubuntu)**
```bash
sudo apt update && sudo apt install freebasic gdb
fbc --version
```

**macOS (Homebrew)**
```bash
brew install freebasic
```

**Windows**
Download from https://www.freebasic.net/wiki/CompilerInstalling

---

#### VS Code extension

When you open this folder VS Code will suggest the recommended extension. Accept it.

| Extension | ID | Purpose |
|---|---|---|
| **QB64** | `devblocks.qb64` | Syntax highlighting for `.bas` files |
| **C/C++** | `ms-vscode.cpptools` | GDB debugger (FreeBASIC only) |

---

### Running the project

1. Open `hello-world/` in VS Code
2. Select the right build task for your compiler:
   - `Ctrl+Shift+P` → `Tasks: Run Task` → choose `BASIC: Build (QB64-PE — ...)` or `BASIC: Build (FreeBASIC — ...)`
   - Or set one as default and use `Ctrl+Shift+B`
3. Run in the terminal:
   ```bash
   ./bin/hello
   ```

#### Expected output

```
============================================
  BASIC Hello World — CodeSamples Project
============================================

Enter your name: Alice
 1. Hello, Alice!
 2. Hello, Alice!
 3. Hello, Alice!

Enter a circle radius: 5
Area of circle with radius  5.00 =   78.5398

Welcome to BASIC. The language that started it all.
```

---

### Debugging (FreeBASIC + GDB)

FreeBASIC compiles to native code with debug symbols, making GDB debugging possible.

1. Build with the **"BASIC: Build (FreeBASIC)"** task (it passes `-g` debug flag automatically)
2. Set a **breakpoint** in `src/hello.bas`
3. Press **F5** to launch the GDB debugger

> QB64-PE does not produce GDB-compatible debug symbols directly. For step-through debugging in QB64-PE, use its built-in IDE (`qb64pe` without `-c` flag to open the IDE).

---

### Manual compilation

**QB64-PE:**
```bash
# Linux / macOS
qb64pe -c src/hello.bas -o bin/hello
./bin/hello

# Windows
qb64pe.exe -c src\hello.bas -o bin\hello.exe
bin\hello.exe
```

**FreeBASIC:**
```bash
# Compile with debug symbols
fbc -g -x bin/hello src/hello.bas

# Optimized
fbc -O 2 -x bin/hello src/hello.bas

./bin/hello
```

| FreeBASIC flag | Meaning |
|---|---|
| `-x <output>` | Output executable path |
| `-g` | Include debug symbols |
| `-O 2` | Optimization level 2 |
| `-Wall` | All warnings |
| `-lang qb` | QuickBASIC compatibility mode |

---

### Understanding the source code

```basic
OPTION _EXPLICIT          ' Forces all variables to be declared with DIM
                          ' (the QB64 equivalent of "implicit none")

DIM userName AS STRING    ' Explicit declaration — STRING type
DIM pi       AS DOUBLE    ' 64-bit float

pi = 4 * ATN(1)           ' Classic BASIC trick: atan(1) = π/4, so 4*atan(1) = π

LINE INPUT userName        ' Reads the entire line, including spaces
                          ' Unlike INPUT, it doesn't stop at commas

FOR counter = 1 TO 3      ' Counted loop — counter goes 1, 2, 3
    PRINT counter; ". Hello, "; TRIM$(userName); "!"
    '                           ^ removes leading and trailing spaces
NEXT counter

PRINT USING "...##.## = ####.####"; radius; area
'     ^ formatted output — # = digit placeholder, . = decimal point

SUB PrintBanner            ' Procedure definition (modern BASIC)
    PRINT "==="
END SUB

CALL PrintBanner           ' Call the SUB (CALL keyword is optional in QB64)
```

---

## 🇫🇷 Français

### Ce que fait ce projet

Un petit programme BASIC interactif qui :
- Affiche une bannière via un `SUB`
- Lit ton prénom avec `LINE INPUT` et te salue trois fois avec `FOR/NEXT`
- Lit un rayon et calcule l'aire d'un cercle avec `PRINT USING` pour le formatage
- Illustre `OPTION _EXPLICIT`, `DIM`, `SUB`, `TRIM$` et l'astuce `ATN(1)*4` pour π

---

### Quel compilateur choisir ?

| Compilateur | Idéal pour | Lien |
|---|---|---|
| **QB64-PE** | Code QuickBASIC-compatible, graphismes, son | https://github.com/QB64-Phoenix-Edition/QB64pe |
| **FreeBASIC** | Programmation système, débogage GDB, interop C | https://www.freebasic.net |

Le source de ce projet est compatible avec les deux. **QB64-PE est recommandé** pour l'apprentissage.

---

### Prérequis

#### Option A : QB64-PE (recommandé)

**Windows**
1. Télécharge la dernière version depuis https://github.com/QB64-Phoenix-Edition/QB64pe/releases
2. Extrais l'archive (ex : `C:\QB64PE\`)
3. Ajoute `C:\QB64PE\` à ta variable d'environnement `PATH`

**Linux (Debian / Ubuntu)**
```bash
sudo apt install libgl1-mesa-dev libglu1-mesa-dev
export PATH="$PATH:/opt/qb64pe"   # dans ~/.bashrc
```

#### Option B : FreeBASIC (meilleur pour le débogage GDB)

**Linux (Debian / Ubuntu)**
```bash
sudo apt update && sudo apt install freebasic gdb
```

**Windows** : télécharger depuis https://www.freebasic.net/wiki/CompilerInstalling

---

### Lancer le projet

1. Ouvrir `hello-world/` dans VS Code
2. Choisir la tâche de build : `Ctrl+Shift+P` → `Tasks: Run Task`
3. Exécuter dans le terminal : `./bin/hello`

### Débogage (FreeBASIC + GDB)

1. Builder avec **"BASIC: Build (FreeBASIC)"**
2. Poser un point d'arrêt dans `src/hello.bas`
3. Appuyer sur **F5**

### Compilation manuelle

**QB64-PE :**
```bash
qb64pe -c src/hello.bas -o bin/hello
./bin/hello
```

**FreeBASIC :**
```bash
fbc -g -x bin/hello src/hello.bas   # avec symboles de débogage
./bin/hello
```
