# UEFI Hello World — "Hello my OS"

> UEFI replaced the legacy BIOS on all PC hardware after ~2012.
> This is how modern firmware boots. No kernel, no OS — just your code
> and the UEFI firmware's rich API.

---

## 🇬🇧 English

### What this is

A minimal **UEFI application** written in C using the [GNU-EFI](https://sourceforge.net/projects/gnu-efi/) library.

UEFI (Unified Extensible Firmware Interface) is the firmware standard that replaced BIOS. When a modern PC boots, the firmware looks for `.EFI` executables on a FAT32 **EFI System Partition (ESP)** — specifically at `/EFI/BOOT/BOOTX64.EFI` for x86_64.

This project compiles a C file into a **PE32+ executable** (the Windows `.exe` format), places it where UEFI expects it, and boots it in QEMU using **OVMF** (Open Virtual Machine Firmware — an open-source UEFI implementation).

### Key differences from legacy BIOS

| Legacy BIOS | UEFI |
|---|---|
| 16-bit real mode at start | 32/64-bit protected mode from the start |
| 512-byte MBR | PE32+ executable on FAT32 ESP |
| INT 10h for text output | `EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL` |
| No C standard library | Rich firmware API (boot services, runtime services) |
| One sector of code | No size limit on application |
| `0x55 0xAA` boot signature | Standard PE32+ headers |
| Disappears at OS load | Runtime services survive into OS |

### Build pipeline

```
src/main.c
    │
    │  gcc (cross-compile ELF with EFI flags)
    ▼
bin/main.o          (ELF object file)
    │
    │  ld (link with gnu-efi crt0 + linker script)
    ▼
bin/main.so         (ELF shared object — intermediate)
    │
    │  objcopy (extract PE32+ sections)
    ▼
bin/BOOTX64.EFI     (PE32+ executable — what UEFI loads)
    │
    │  QEMU + OVMF
    ▼
🖥️  "Hello my OS" on screen
```

---

### Prerequisites

**Linux (Debian/Ubuntu)**
```bash
sudo apt update
sudo apt install gnu-efi ovmf qemu-system-x86
```

**Linux (Fedora/RHEL)**
```bash
sudo dnf install gnu-efi gnu-efi-devel edk2-ovmf qemu-system-x86
```

**macOS** — cross-compilation is complex; use Linux or WSL.

**Windows** — use WSL (Ubuntu) as above.

---

### Building and running

```bash
# Install dependencies first (see above)

# Build the .EFI application
make

# Run in QEMU with OVMF
make run

# Inspect the PE32+ binary
make inspect

# Clean
make clean
```

Or use VS Code: **`Ctrl+Shift+B`** to build, then the "UEFI: Run in QEMU" task.

#### Expected output

```
  ============================================
    Hello my OS
    UEFI Bare Metal — CodeSamples Project
  ============================================

  Firmware vendor:   EDK II
  Firmware revision: 00010000
  UEFI revision:     2.70

  To load a real kernel, the next steps would be:
    1. Call GetMemoryMap() — discover available RAM
    2. Call ExitBootServices() — take control from UEFI
    3. Set up GDT, paging, IDT
    4. Jump to the kernel entry point

  Press any key to exit...
```

---

### Understanding the UEFI environment

#### The EFI System Table

Everything in UEFI goes through the `EFI_SYSTEM_TABLE`:

```c
// Your entry point receives this pointer from firmware
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)

// Print text
SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello\r\n");
//                   ^^^
//           ConOut is EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*
//           Protocol = interface with a fixed set of function pointers

// Boot services (before ExitBootServices)
SystemTable->BootServices->AllocatePool(...)  // memory allocation
SystemTable->BootServices->GetMemoryMap(...)  // memory map
SystemTable->BootServices->ExitBootServices(...)  // hand off to kernel

// Runtime services (available even after ExitBootServices)
SystemTable->RuntimeServices->GetTime(...)
SystemTable->RuntimeServices->SetVariable(...)
```

#### Why `L"..."` (wide strings)?

UEFI uses **UTF-16LE** (16-bit Unicode) for all text. The `L` prefix creates a `wchar_t*` literal. In UEFI, `CHAR16` = `uint16_t` = 2 bytes per character. That's why the compiler flag `-fshort-wchar` is required — it makes `sizeof(wchar_t) == 2`.

#### Why PE32+ format?

UEFI firmware is designed to run on any architecture. It adopted the **Portable Executable** format (Windows `.exe`/`.dll`) because it supports:
- Architecture-independent metadata
- Relocations (the image can be loaded at any address)
- Section layout compatible with position-independent code

On Linux, we cross-compile: gcc produces ELF → `objcopy` converts to PE32+.

#### Microsoft x64 ABI

Even on Linux hardware, UEFI uses the **Microsoft x64 calling convention** (not the System V ABI used by Linux). This means:
- First 4 arguments: `RCX`, `RDX`, `R8`, `R9` (not `RDI`, `RSI`, `RDX`, `RCX`)
- No 128-byte red zone (hence `-mno-red-zone`)
- The `EFIAPI` macro expands to `__attribute__((ms_abi))`

---

### Compiler flags explained

```bash
gcc \
  -I/usr/include/efi \        # UEFI type definitions (EFI_HANDLE, EFI_STATUS...)
  -I/usr/include/efi/x86_64 \ # Architecture-specific UEFI types
  -fpic \                     # Position-Independent Code
  -ffreestanding \            # No C standard library (no printf, malloc, etc.)
  -fno-stack-protector \      # UEFI has no stack canary support
  -fshort-wchar \             # wchar_t = 2 bytes (UEFI uses UTF-16LE)
  -mno-red-zone \             # Disable System V red zone — UEFI uses MS x64 ABI
  -DEFI_FUNCTION_WRAPPER \    # Enable uefi_call_wrapper macro
  -DGNU_EFI_USE_MS_ABI        # Use Microsoft calling convention
```

---

### Going further — a real OS bootloader would

After printing "Hello my OS", a real OS loader does:

```c
// 1. Get the memory map
UINTN mapSize = 0, mapKey, descSize;
UINT32 descVersion;
EFI_MEMORY_DESCRIPTOR *map;
// Call GetMemoryMap() to find usable RAM

// 2. Load the kernel (read from disk, parse ELF, map into memory)
// Use EFI_LOADED_IMAGE_PROTOCOL to find the ESP volume

// 3. ExitBootServices — point of no return!
// After this: no more BIOS/UEFI boot services available
// Your code is now in full control of the hardware
SystemTable->BootServices->ExitBootServices(ImageHandle, mapKey);

// 4. Set up your own environment
// - GDT (Global Descriptor Table)
// - IDT (Interrupt Descriptor Table)
// - Page tables
// - Stack

// 5. Jump to kernel
// typedef void (*KernelEntry)(void);
// ((KernelEntry)kernel_start)();
// — and NEVER RETURN —
```

**Projects to study next:**
- [UEFI Bare Bones — OSDev Wiki](https://wiki.osdev.org/UEFI_Bare_Bones)
- [TianoCore EDK2](https://github.com/tianocore/edk2) — the reference UEFI implementation
- [limine](https://github.com/limine-bootloader/limine) — a modern production UEFI bootloader

---

## 🇫🇷 Français

### Ce qu'est ce projet

Une **application UEFI minimale** écrite en C avec la bibliothèque GNU-EFI.

UEFI a remplacé le BIOS sur tous les PC modernes (~2012+). Le firmware cherche les fichiers `.EFI` sur une partition FAT32 appelée **EFI System Partition**, précisément à `/EFI/BOOT/BOOTX64.EFI` sur x86_64.

Ce projet compile un fichier C en **exécutable PE32+** (le même format que les `.exe` Windows), le place où UEFI l'attend, et le boot dans QEMU avec OVMF (une implémentation open-source d'UEFI).

### Prérequis

```bash
# Linux / WSL (Ubuntu)
sudo apt install gnu-efi ovmf qemu-system-x86
```

### Compiler et exécuter

```bash
make        # compile le .EFI
make run    # lance dans QEMU + OVMF
make clean  # nettoie
```

### Pipeline de compilation

```
src/main.c → [gcc] → bin/main.o → [ld] → bin/main.so → [objcopy] → bin/BOOTX64.EFI
```

1. `gcc` compile le C en objet ELF avec des flags spéciaux (pas de libc, wchar_t=2 octets)
2. `ld` lie avec le code de démarrage gnu-efi (`crt0-efi-x86_64.o`) et le script de lien UEFI
3. `objcopy` extrait les sections utiles et les convertit en format PE32+ (format Windows `.exe`)
4. QEMU + OVMF charge ce fichier comme un vrai firmware UEFI

### Concepts clés

- **L"..."** — chaîne UTF-16LE (UEFI utilise Unicode 16 bits, pas ASCII)
- **EFI_SYSTEM_TABLE** — table maître : accès à tous les services UEFI (E/S, mémoire, disque…)
- **PE32+** — format des exécutables Windows, adopté par UEFI pour sa portabilité
- **ABI Microsoft x64** — convention d'appel différente de Linux (RCX/RDX/R8/R9 au lieu de RDI/RSI/RDX/RCX)
- **`-mno-red-zone`** — zone rouge désactivée (ABI Microsoft n'en a pas)
- **ExitBootServices()** — après cet appel, l'OS prend le contrôle total du matériel

### Pourquoi UEFI est plus puissant que BIOS

| Avantage | Explication |
|---|---|
| Mode 64 bits natif | Pas besoin de passer par le mode 16/32 bits |
| API riche | Accès disque, réseau, variables non-volatiles, horloge |
| Taille illimitée | Pas de contrainte des 512 octets |
| Démarrage sécurisé | Secure Boot vérifie la signature cryptographique des bootloaders |
| Architecture indépendante | Le même format PE32+ fonctionne sur x86, ARM, RISC-V |
