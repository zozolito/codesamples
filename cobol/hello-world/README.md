# COBOL Hello World — VS Code Project

A ready-to-run COBOL project using **GnuCOBOL** as the compiler and **VS Code** as the editor.

---

## 🇬🇧 English

### What this project does

This project compiles and runs a small interactive COBOL program that:
- Displays a welcome banner
- Asks for your name
- Greets you three times using a loop and the `STRING` verb
- Introduces you to paragraphs, `WORKING-STORAGE`, and `PERFORM`

It is intentionally minimal. Read the source. Understand every line. Then break it.

---

### Project structure

```
hello-world/
├── src/
│   └── hello.cbl          ← the COBOL source file
├── bin/                   ← compiled binary goes here (git-ignored)
├── .vscode/
│   ├── tasks.json         ← build / run / clean tasks
│   ├── launch.json        ← debug configuration (GDB)
│   ├── extensions.json    ← recommended VS Code extensions
│   └── settings.json      ← COBOL-specific editor settings
└── README.md
```

---

### Prerequisites

#### 1. Install GnuCOBOL

GnuCOBOL is the free, open-source COBOL compiler. It compiles COBOL to C, then to a native binary.

**Linux (Debian / Ubuntu)**
```bash
sudo apt update && sudo apt install gnucobol
cobc --version
```

**Linux (Fedora / RHEL)**
```bash
sudo dnf install gnucobol
```

**macOS (Homebrew)**
```bash
brew install gnu-cobol
cobc --version
```

**Windows — Option A: WSL (recommended)**

WSL (Windows Subsystem for Linux) is the smoothest path for COBOL development on Windows.

1. Open PowerShell as Administrator and run:
   ```powershell
   wsl --install
   ```
2. Reboot, then open Ubuntu from the Start menu
3. Inside WSL:
   ```bash
   sudo apt update && sudo apt install gnucobol gdb
   ```
4. In VS Code, install the **WSL** extension and use **"Reopen in WSL"** to work inside your Linux environment

**Windows — Option B: Native GnuCOBOL**

Download the Windows binary from the GnuCOBOL releases page:  
https://gnucobol.sourceforge.io

Add the `bin/` folder to your `PATH` environment variable after installation.

> **Note**: On Windows without WSL, the compiled binary will be `hello.exe`. The `COBOL: Clean` task handles both automatically.

---

#### 2. Install VS Code extensions

When you open this folder in VS Code, you will be prompted to install the recommended extensions. Accept.

If not prompted, install them manually:

| Extension | ID | Purpose |
|---|---|---|
| **COBOL Language Support** | `broadcomMFD.cobol-language-support` | Syntax highlighting, IntelliSense, column guides |
| **C/C++** | `ms-vscode.cpptools` | GDB debugger integration |

---

### Running the project

#### Build and run (quickest way)

1. Open the `hello-world/` folder in VS Code:
   ```
   File → Open Folder → select hello-world/
   ```

2. **Build**: `Ctrl+Shift+B` — compiles `src/hello.cbl` into `bin/hello`

3. **Run**: open the terminal (`Ctrl+`` `) and run:
   ```bash
   ./bin/hello
   ```
   Or use the **"COBOL: Run"** task from the Command Palette (`Ctrl+Shift+P` → `Tasks: Run Task`).

#### Expected output

```
============================================
  COBOL Hello World — CodeSamples Project  
============================================
 
Enter your name: Alice
1. Hello, Alice!
2. Hello, Alice!
3. Hello, Alice!
 
Welcome to COBOL. The language that runs the world.
```

---

### Debugging the project

> Debugging requires GDB. On Linux/macOS it is usually pre-installed. On Windows, use WSL.

1. Open `src/hello.cbl`
2. Click in the gutter to the left of a line number to set a **breakpoint** (red dot)
3. Press **F5** — VS Code will build the project and launch the GDB debugger
4. The program stops at your breakpoint. You can now:
   - **Step over**: `F10`
   - **Step into**: `F11`
   - **Continue**: `F5`
   - **Inspect variables**: hover over them or check the **Variables** panel on the left

> The `launch.json` is configured with `"stopAtEntry": true`, which means the debugger always pauses at the very first instruction. This lets you step through the entire program from the start.

---

### Manual compilation (without VS Code)

If you prefer the terminal:

```bash
# Compile
cobc -x -g -Wall -o bin/hello src/hello.cbl

# Run
./bin/hello

# On Windows (native)
bin\hello.exe
```

Flags explained:
| Flag | Meaning |
|------|---------|
| `-x` | Produce an executable (not a shared library) |
| `-g` | Include debug symbols (required for GDB) |
| `-Wall` | Enable all compiler warnings |
| `-o bin/hello` | Output file path |

---

### Understanding the source code

Open [`src/hello.cbl`](src/hello.cbl). Here is what each part does:

```
IDENTIFICATION DIVISION   → metadata: program name, author
DATA DIVISION             → all variables live here, under WORKING-STORAGE
PROCEDURE DIVISION        → the logic
  PERFORM DISPLAY-BANNER  → calls the DISPLAY-BANNER paragraph (like a function call)
  ACCEPT WS-NAME          → reads a line from stdin into WS-NAME
  STRING ... INTO ...     → concatenates strings into WS-GREETING
  PERFORM VARYING ...     → loop: runs 3 times, incrementing WS-COUNTER
  DISPLAY-BANNER.         → paragraph definition (called via PERFORM above)
```

Key things to notice:
- **Column 7** is where `*>` comments go (the `*>` style works in free-format; `*` in column 7 is the fixed-format equivalent)
- **`PIC X(30)`** means a 30-character alphanumeric field
- **`FUNCTION TRIM()`** removes trailing spaces — important because COBOL strings are fixed-length and always padded
- **`WITH NO ADVANCING`** on `DISPLAY` keeps the cursor on the same line (like `print(..., end="")` in Python)

---

## 🇫🇷 Français

### Ce que fait ce projet

Ce projet compile et exécute un petit programme COBOL interactif qui :
- Affiche une bannière de bienvenue
- Demande ton prénom
- Te salue trois fois à l'aide d'une boucle et du verbe `STRING`
- T'introduit aux paragraphes, à `WORKING-STORAGE` et à `PERFORM`

---

### Prérequis

#### 1. Installer GnuCOBOL

**Linux (Debian / Ubuntu)**
```bash
sudo apt update && sudo apt install gnucobol
cobc --version
```

**macOS (Homebrew)**
```bash
brew install gnu-cobol
```

**Windows — Option A : WSL (recommandé)**

1. Dans PowerShell (administrateur) :
   ```powershell
   wsl --install
   ```
2. Redémarre, ouvre Ubuntu depuis le menu Démarrer
3. Dans WSL :
   ```bash
   sudo apt update && sudo apt install gnucobol gdb
   ```
4. Dans VS Code, installe l'extension **WSL** et utilise **"Reopen in WSL"**

**Windows — Option B : GnuCOBOL natif**

Télécharge le binaire Windows depuis :  
https://gnucobol.sourceforge.io

Puis ajoute son dossier `bin/` à ta variable d'environnement `PATH`.

---

#### 2. Installer les extensions VS Code

Quand tu ouvres ce dossier dans VS Code, accepte l'installation des extensions recommandées. Si tu n'es pas invité :

| Extension | ID |
|---|---|
| **COBOL Language Support** | `broadcomMFD.cobol-language-support` |
| **C/C++** | `ms-vscode.cpptools` |

---

### Lancer le projet

1. Ouvre le dossier `hello-world/` dans VS Code
2. **Compiler** : `Ctrl+Shift+B`
3. **Exécuter** dans le terminal :
   ```bash
   ./bin/hello
   ```

### Déboguer le projet

1. Ouvre `src/hello.cbl`
2. Clique dans la gouttière pour poser un **point d'arrêt**
3. Appuie sur **F5**
4. VS Code compile et lance GDB — le programme s'arrête à ton point d'arrêt

| Raccourci | Action |
|-----------|--------|
| `F10` | Pas à pas (step over) |
| `F11` | Entrer dans (step into) |
| `F5` | Continuer |
| Survol | Inspecter la valeur d'une variable |

### Compilation manuelle

```bash
# Compiler
cobc -x -g -Wall -o bin/hello src/hello.cbl

# Lancer
./bin/hello
```

| Option | Signification |
|--------|---------------|
| `-x` | Produire un exécutable |
| `-g` | Inclure les symboles de débogage (requis pour GDB) |
| `-Wall` | Activer tous les avertissements |
