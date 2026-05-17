# Legacy x86 BIOS Bootloader — "Hello my OS"

> You are about to run code **before any operating system exists**.
> No kernel. No libc. No printf. Just you, 512 bytes, and the raw CPU.

---

## 🇬🇧 English

### What this is

A **Master Boot Record (MBR)** bootloader written in x86 NASM assembly.

When a PC powers on, the BIOS/firmware performs POST (Power-On Self-Test), then scans each storage device for a bootable sector — exactly **512 bytes** whose last two bytes are `0x55 0xAA`. It loads that sector at physical address **`0x7C00`** and jumps to it in **16-bit Real Mode**.

**This file is that sector.**

The CPU at this point thinks it's a 1978 Intel 8086:
- 16-bit registers (`AX`, `BX`, `CX`, `DX`, `SI`, `DI`, `SP`, `BP`)
- 20-bit addressing (segment × 16 + offset = 1 MB maximum)
- No protected memory, no virtual memory, no paging
- No kernel, no syscalls — BIOS interrupts are all we have

### Project structure

```
legacy-x86/
├── src/
│   └── boot.asm       ← NASM assembly source, annotated line-by-line
├── bin/               ← output directory (git-ignored)
│   ├── boot.bin       ← flat binary: exactly 512 bytes
│   └── boot.lst       ← listing file (addresses + opcodes + source)
├── Makefile           ← assemble, run under QEMU, inspect
├── .vscode/
│   └── tasks.json     ← Build / Run (VGA) / Run (curses) / Inspect
├── .gitignore
└── README.md
```

---

### Prerequisites

**Linux (Debian/Ubuntu)**
```bash
sudo apt install nasm qemu-system-x86 xxd
nasm --version    # 2.15 or newer
```

**macOS**
```bash
brew install nasm qemu
```

**Windows (WSL — strongly recommended)**
```bash
# Inside WSL:
sudo apt install nasm qemu-system-x86
```

> **Note:** QEMU requires a graphical display for VGA output.
> In headless environments (WSL without X11), use the `run-curses` target.

---

### Building and running

```bash
# Assemble
make

# Run in QEMU (opens a VGA window)
make run

# Run in terminal only (no GUI needed)
make run-curses

# Inspect the binary
make inspect
```

Or use VS Code: **`Ctrl+Shift+B`** to assemble, then run the QEMU task.

#### Expected output

```
Hello my OS
Press any key to halt...
```

---

### The 512-byte constraint

```
Offset 0       → Your code starts here (CPU jumps to 0x7C00)
Offset 0–509   → Code + data (max 510 bytes)
Offset 510–511 → Boot signature: 0x55 0xAA (BIOS checks this)
```

The NASM directive `times 510 - ($ - $$) db 0` fills unused bytes with zeros.
If your code + data is larger than 510 bytes, NASM will error.

#### Verify the binary

```bash
# Should show exactly 512 bytes
wc -c bin/boot.bin

# Should show "55 aa" at the end
xxd -s 508 bin/boot.bin
# Expected: 00000ffc: 0000 55aa

# Disassemble (16-bit mode)
ndisasm -b 16 bin/boot.bin | head -30
```

---

### How it works — step by step

```asm
[BITS 16]        ; 16-bit real mode — CPU starts here after reset
[ORG 0x7C00]     ; BIOS loads the sector at this physical address

cli              ; Disable interrupts — safe to reconfigure segments
xor ax, ax       ; ax = 0 (faster than mov ax, 0)
mov ds, ax       ; Data Segment = 0
mov ss, ax       ; Stack Segment = 0
mov sp, 0x7C00   ; Stack grows DOWN from our load address
sti              ; Re-enable interrupts

mov si, msg      ; SI points to our message string

.loop:
lodsb            ; AL = *SI; SI++  (loads one byte, advances pointer)
test al, al      ; is AL == 0? (null terminator)
jz .halt
mov ah, 0x0E     ; BIOS INT 10h function: teletype output
int 0x10         ; Call BIOS video interrupt — prints AL
jmp .loop

msg db 'Hello my OS', 13, 10, 0   ; CR + LF + null

times 510-($ - $$) db 0  ; pad to byte 510
dw 0xAA55                ; boot signature (little-endian)
```

| Instruction | What it does |
|---|---|
| `[BITS 16]` | Tells NASM to output 16-bit opcodes |
| `[ORG 0x7C00]` | Tells NASM where code will be in memory (for address resolution) |
| `cli` / `sti` | Clear/Set Interrupt Flag — disable/enable hardware interrupts |
| `xor ax, ax` | Zero AX register (faster than `mov ax, 0`) |
| `lodsb` | Load Byte at `[DS:SI]` into `AL`, increment `SI` |
| `INT 10h` | BIOS software interrupt — video services |
| `AH=0x0E` | Teletype Output — print character in AL to current cursor position |
| `hlt` | Halt the CPU until next interrupt |
| `times N db val` | Fill N bytes with value `val` |
| `dw 0xAA55` | Define 16-bit word — stored as `55 AA` (little-endian) |

---

### Going further — a real OS loader would

1. **Load a kernel from disk** — using INT 13h (BIOS disk services) to read sectors
2. **Enable A20 line** — unlocks the 21st address bit (access > 1 MB)
3. **Enter Protected Mode** — load a GDT, set CR0.PE bit → 32-bit addressing
4. **Or enter Long Mode** — set up page tables, enter 64-bit mode
5. **Call the kernel entry point** — jump to your C `kernel_main()`
6. **Never return** — `hlt` in an infinite loop if the kernel exits

---

## 🇫🇷 Français

### Ce qu'est ce projet

Un **chargeur d'amorçage MBR** écrit en assembleur NASM x86.

Quand un PC démarre, le BIOS cherche un secteur d'amorçage de **512 octets** dont les deux derniers octets sont `0x55 0xAA`. Il charge ce secteur à l'adresse physique **`0x7C00`** et y saute en **Mode Réel 16 bits**.

**Ce fichier est ce secteur.**

Le CPU se croit en 1978 :
- Registres 16 bits uniquement
- Adressage 20 bits (segment × 16 + offset, max 1 Mo)
- Aucune protection mémoire, aucune pagination
- Pas de noyau, pas de syscalls — seulement les interruptions BIOS

### Prérequis

```bash
# Linux / WSL
sudo apt install nasm qemu-system-x86

# macOS
brew install nasm qemu
```

### Compiler et exécuter

```bash
make          # assembler
make run      # lancer dans QEMU (fenêtre VGA)
make run-curses  # mode terminal (sans GUI)
make inspect  # inspecter le binaire
```

### Comment ça marche

1. `nasm -f bin boot.asm -o boot.bin` — produit un binaire plat (aucun en-tête ELF)
2. QEMU charge ce binaire comme un disque brut
3. Le BIOS émulé de QEMU vérifie les octets 510-511 (`55 AA`)
4. Le BIOS charge les 512 octets à `0x7C00` et saute à cette adresse
5. Notre code s'exécute : configure les registres, affiche le message via INT 10h
6. Le CPU est mis en veille avec `hlt`

### La contrainte des 512 octets

```
Octets 0–509   → code + données (510 octets max)
Octets 510–511 → signature d'amorçage : 0x55 0xAA
```

`times 510-($ - $$) db 0` remplit les octets inutilisés avec des zéros.

### Pour aller plus loin

Un vrai chargeur OS ferait ensuite :
1. Charger le noyau depuis le disque (INT 13h)
2. Activer la ligne A20 (accès au-delà de 1 Mo)
3. Entrer en mode protégé (32 bits) ou en mode long (64 bits)
4. Appeler le point d'entrée du noyau C
