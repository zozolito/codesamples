# Fortran Hello World — VS Code Project

A ready-to-run Fortran project using **GNU Fortran (gfortran)** and **VS Code**.

---

## 🇬🇧 English

### What this project does

A small interactive Fortran program that:
- Displays a welcome banner via an internal subroutine
- Reads your name and greets you three times using a `DO` loop
- Reads a radius and computes the area of a circle
- Demonstrates `implicit none`, format strings, `trim()`, and `contains`

Read every line. Understand it. Then modify it and see what breaks.

---

### Project structure

```
hello-world/
├── src/
│   └── hello.f90          ← Fortran source (free format, modern Fortran 90+)
├── bin/                   ← compiled binary (git-ignored)
├── .vscode/
│   ├── tasks.json         ← Build / Build optimized / Run / Clean
│   ├── launch.json        ← GDB and LLDB debug configurations
│   ├── extensions.json    ← recommended VS Code extensions
│   └── settings.json      ← file associations, linter config
├── .gitignore
└── README.md
```

---

### Prerequisites

#### 1. Install GNU Fortran (gfortran)

**Linux (Debian / Ubuntu)**
```bash
sudo apt update && sudo apt install gfortran gdb
gfortran --version
```

**Linux (Fedora / RHEL)**
```bash
sudo dnf install gcc-gfortran gdb
```

**macOS (Homebrew)**
```bash
brew install gcc
gfortran --version
# gfortran is included in the gcc Homebrew package
```

**Windows — Option A: WSL (recommended)**
```powershell
# In PowerShell (Administrator)
wsl --install
```
Then inside Ubuntu (WSL):
```bash
sudo apt update && sudo apt install gfortran gdb
```
Open VS Code, install the **WSL** extension, and use **"Reopen in WSL"**.

**Windows — Option B: MSYS2 / MinGW**
1. Download MSYS2 from https://www.msys2.org
2. In the MSYS2 terminal:
   ```bash
   pacman -S mingw-w64-x86_64-gcc-fortran mingw-w64-x86_64-gdb
   ```
3. Add `C:\msys64\mingw64\bin` to your `PATH`

> On Windows without WSL, the output binary is `hello.exe`. The **Clean** task handles both automatically.

---

#### 2. Install VS Code extensions

When you open this folder VS Code will suggest the recommended extensions. Accept them.

| Extension | ID | Purpose |
|---|---|---|
| **Modern Fortran** | `fortran-lang.linter-gfortran` | Syntax highlighting, linting, IntelliSense |
| **Fortran Breakpoint** | `ekibun.fortranbreaker` | Breakpoint support in Fortran source |
| **C/C++** | `ms-vscode.cpptools` | GDB / LLDB debugger integration |

---

### Running the project

1. Open the `hello-world/` folder in VS Code:
   ```
   File → Open Folder → select hello-world/
   ```

2. **Build**: `Ctrl+Shift+B` — compiles with debug symbols into `bin/hello`

3. **Run** in the integrated terminal:
   ```bash
   ./bin/hello
   ```
   Or use the **"Fortran: Run"** task (`Ctrl+Shift+P` → `Tasks: Run Task`).

#### Expected output

```
============================================
  Fortran Hello World — CodeSamples Project
============================================

Enter your name: Alice
1. Hello, Alice!
2. Hello, Alice!
3. Hello, Alice!

Enter a circle radius: 5.0
Area of circle with radius   5.00 =    78.5398

Welcome to Fortran. The language that sent humans to the Moon.
```

---

### Debugging

1. Open `src/hello.f90`
2. Click in the gutter to set a **breakpoint** (red dot)
3. Press **F5** — VS Code builds and launches GDB
4. The program pauses at your breakpoint

| Shortcut | Action |
|---|---|
| `F10` | Step over |
| `F11` | Step into |
| `F5` | Continue |
| Hover | Inspect variable value |

> `"stopAtEntry": true` in `launch.json` makes the debugger pause at the very first line — useful to step through the whole program from the start. Set it to `false` if you only want to stop at your own breakpoints.

---

### Manual compilation

```bash
# Debug build (for development)
gfortran -g -Wall -Wextra -fcheck=all -o bin/hello src/hello.f90

# Optimized build (for performance)
gfortran -O2 -Wall -o bin/hello src/hello.f90

# Run
./bin/hello
```

| Flag | Meaning |
|---|---|
| `-g` | Include debug symbols (required for GDB) |
| `-Wall` | Enable all standard warnings |
| `-Wextra` | Enable extra warnings |
| `-fcheck=all` | Runtime checks: array bounds, pointer status, etc. |
| `-O2` | Optimization level 2 (do not combine with `-g` for production profiling) |

---

### Understanding the source code

```fortran
program hello_world        ! program unit — every Fortran program starts here
    implicit none          ! ALWAYS — disables implicit typing (i-n = integer rule)

    character(len=50) :: user_name   ! fixed-length string
    integer           :: counter     ! loop variable
    real              :: pi = 3.14159265
    real              :: radius, area

    call print_banner()    ! call internal subroutine (defined in contains)

    write(*, '(A)', advance='no') 'Enter your name: '
    !       ^  ^           ^
    !       |  |           no newline after prompt
    !       |  format descriptor: A = character string
    !       * = default unit (stdout)

    read(*, '(A)') user_name     ! read a string from stdin

    do counter = 1, 3            ! DO loop: counter goes 1, 2, 3
        write(*, '(I1, A, A, A)') counter, '. Hello, ', trim(user_name), '!'
        !                                               ^ removes trailing spaces
    end do

    area = pi * radius**2        ! ** is the exponentiation operator

contains
    ! Internal subroutine — only visible inside this program unit
    subroutine print_banner()
        write(*, '(A)') '====...'
    end subroutine print_banner

end program hello_world
```

---

## 🇫🇷 Français

### Ce que fait ce projet

Un petit programme Fortran interactif qui :
- Affiche une bannière via une sous-routine interne
- Lit ton prénom et te salue trois fois avec une boucle `DO`
- Lit un rayon et calcule l'aire d'un cercle
- Illustre `implicit none`, les formats d'écriture, `trim()` et `contains`

---

### Prérequis

#### 1. Installer GNU Fortran (gfortran)

**Linux (Debian / Ubuntu)**
```bash
sudo apt update && sudo apt install gfortran gdb
```

**macOS (Homebrew)**
```bash
brew install gcc
```

**Windows — Option A : WSL (recommandé)**
```powershell
wsl --install
```
Dans Ubuntu (WSL) :
```bash
sudo apt update && sudo apt install gfortran gdb
```
Dans VS Code, installe l'extension **WSL** et utilise **"Reopen in WSL"**.

**Windows — Option B : MSYS2**
1. Télécharge MSYS2 depuis https://www.msys2.org
2. Dans le terminal MSYS2 :
   ```bash
   pacman -S mingw-w64-x86_64-gcc-fortran mingw-w64-x86_64-gdb
   ```
3. Ajoute `C:\msys64\mingw64\bin` à ta variable `PATH`

---

#### 2. Extensions VS Code

| Extension | ID |
|---|---|
| **Modern Fortran** | `fortran-lang.linter-gfortran` |
| **Fortran Breakpoint** | `ekibun.fortranbreaker` |
| **C/C++** | `ms-vscode.cpptools` |

---

### Lancer le projet

1. Ouvre le dossier `hello-world/` dans VS Code
2. **Compiler** : `Ctrl+Shift+B`
3. **Exécuter** dans le terminal :
   ```bash
   ./bin/hello
   ```

### Déboguer

1. Pose un point d'arrêt dans `src/hello.f90`
2. Appuie sur **F5**

| Raccourci | Action |
|---|---|
| `F10` | Pas à pas |
| `F11` | Entrer dans |
| `F5` | Continuer |

### Compilation manuelle

```bash
# Build debug
gfortran -g -Wall -Wextra -fcheck=all -o bin/hello src/hello.f90

# Build optimisé
gfortran -O2 -Wall -o bin/hello src/hello.f90
```

| Option | Signification |
|---|---|
| `-g` | Symboles de débogage |
| `-Wall` | Tous les avertissements |
| `-Wextra` | Avertissements supplémentaires |
| `-fcheck=all` | Vérifications runtime : bornes tableaux, pointeurs, etc. |
| `-O2` | Optimisation niveau 2 |
