# =============================================================================
# Makefile — UEFI Hello World Application
# CodeSamples Project — Bare Metal Series
#
# Build pipeline:
#   src/main.c  →  [gcc]    →  bin/main.o   (ELF object)
#   bin/main.o  →  [ld]     →  bin/main.so  (ELF shared object)
#   bin/main.so →  [objcopy]→  bin/BOOTX64.EFI  (PE32+ executable)
#
# The UEFI firmware loads PE32+ format executables (same format as Windows .exe).
# On Linux, we cross-compile from ELF to PE32+ using objcopy.
#
# Dependencies:
#   sudo apt install gnu-efi ovmf     (Debian/Ubuntu)
#   sudo dnf install gnu-efi-devel edk2-ovmf   (Fedora)
# =============================================================================

# ── Tools ────────────────────────────────────────────────────────────────────
CC      = gcc
LD      = ld
OBJCOPY = objcopy
QEMU    = qemu-system-x86_64

# ── GNU-EFI paths ─────────────────────────────────────────────────────────────
# These are the standard installation paths on Debian/Ubuntu.
# Adjust if your distribution installs them elsewhere.
# Find with: dpkg -L gnu-efi | grep -E "(include|lib)"

EFI_INC     = /usr/include/efi
EFI_LIB     = /usr/lib

# gnu-efi startup object: provides _start, sets up EFI calling convention,
# calls efi_main, and returns control to UEFI.
EFI_CRT0    = $(EFI_LIB)/crt0-efi-x86_64.o

# Linker script: tells ld how to lay out the EFI PE32+ sections.
EFI_LDS     = $(EFI_LIB)/elf_x86_64_efi.lds

# OVMF: Open Virtual Machine Firmware — an open-source UEFI implementation.
# Used by QEMU to emulate UEFI boot. Install with: apt install ovmf
OVMF        = /usr/share/ovmf/OVMF.fd

# Fallback OVMF paths (distribution-dependent)
OVMF_ALT1   = /usr/share/edk2/ovmf/OVMF_CODE.fd
OVMF_ALT2   = /usr/share/qemu/OVMF.fd

# ── Compiler flags ────────────────────────────────────────────────────────────
CFLAGS  = \
    -I$(EFI_INC)             \
    -I$(EFI_INC)/x86_64      \
    -fpic                    \# Position-Independent Code (required for UEFI .so)
    -ffreestanding           \# No C standard library
    -fno-stack-protector     \# UEFI doesn't provide stack canary support
    -fno-stack-check         \
    -fshort-wchar            \# wchar_t = 2 bytes (UEFI uses UTF-16LE, sizeof(CHAR16)=2)
    -mno-red-zone            \# Disable the 128-byte red zone (System V x64 ABI only)
                             \# UEFI uses Microsoft x64 ABI — no red zone
    -maccumulate-outgoing-args \
    -Wall                    \
    -Wextra                  \
    -DGNU_EFI_USE_MS_ABI     \# Use Microsoft x64 calling convention for EFI calls
    -DEFI_FUNCTION_WRAPPER     # Enable uefi_call_wrapper macro

# ── Linker flags ──────────────────────────────────────────────────────────────
LDFLAGS = \
    -nostdlib                \# No C standard library startup code
    -znocombreloc            \# Don't merge relocation sections
    -T $(EFI_LDS)            \# Use the EFI linker script
    -shared                  \# Output a shared object (required for EFI)
    -Bsymbolic               \# Bind symbols locally (resolve at link time)
    -L$(EFI_LIB)             \# Library search path
    $(EFI_CRT0)              \# gnu-efi C runtime startup object

# ── Output files ──────────────────────────────────────────────────────────────
OBJ       = bin/main.o
SO        = bin/main.so
EFI_APP   = bin/BOOTX64.EFI    # Name required by UEFI for default boot on x86_64

# ── Disk image layout ─────────────────────────────────────────────────────────
# UEFI reads from a FAT32 EFI System Partition (ESP).
# QEMU can simulate this with the "fat:rw:<dir>" drive format.
ESP_DIR   = iso

.PHONY: all run inspect clean

# ── Build targets ─────────────────────────────────────────────────────────────

all: $(EFI_APP)

# Step 1: Compile C source to ELF object
$(OBJ): src/main.c | bin
	$(CC) $(CFLAGS) -c -o $@ $<

# Step 2: Link object + crt0 into an ELF shared object
$(SO): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $< -lefi -lgnuefi

# Step 3: Extract PE32+ sections from the ELF and create the .EFI file
# objcopy strips the ELF headers and keeps only the sections UEFI needs.
$(EFI_APP): $(SO)
	$(OBJCOPY) \
	    -j .text    \
	    -j .sdata   \
	    -j .data    \
	    -j .rodata  \
	    -j .dynamic \
	    -j .dynsym  \
	    -j .rel     \
	    -j .rela    \
	    -j .rel.*   \
	    -j .rela.*  \
	    -j .reloc   \
	    --strip-debug \
	    --target=efi-app-x86_64 \
	    $< $@
	@echo ""
	@echo "  Built: $(EFI_APP)"
	@file $(EFI_APP)

bin:
	mkdir -p bin

# ── QEMU test target ──────────────────────────────────────────────────────────

run: $(EFI_APP)
	@# Set up the fake EFI System Partition directory structure
	@mkdir -p $(ESP_DIR)/EFI/BOOT
	@cp $(EFI_APP) $(ESP_DIR)/EFI/BOOT/BOOTX64.EFI

	@# Find OVMF firmware
	@OVMF_PATH="" ; \
	for f in $(OVMF) $(OVMF_ALT1) $(OVMF_ALT2); do \
	    if [ -f "$$f" ]; then OVMF_PATH="$$f"; break; fi; \
	done; \
	if [ -z "$$OVMF_PATH" ]; then \
	    echo "ERROR: OVMF not found. Install with: sudo apt install ovmf"; \
	    exit 1; \
	fi; \
	echo "Using UEFI firmware: $$OVMF_PATH"; \
	echo "Starting QEMU... (close window or press any key to exit)"; \
	$(QEMU) \
	    -bios "$$OVMF_PATH" \
	    -drive format=raw,file=fat:rw:$(ESP_DIR) \
	    -net none \
	    -m 256M \
	    -no-reboot

# Run with serial console (no GUI — text terminal only)
run-serial: $(EFI_APP)
	@mkdir -p $(ESP_DIR)/EFI/BOOT
	@cp $(EFI_APP) $(ESP_DIR)/EFI/BOOT/BOOTX64.EFI
	@OVMF_PATH="" ; \
	for f in $(OVMF) $(OVMF_ALT1) $(OVMF_ALT2); do \
	    if [ -f "$$f" ]; then OVMF_PATH="$$f"; break; fi; \
	done; \
	$(QEMU) \
	    -bios "$$OVMF_PATH" \
	    -drive format=raw,file=fat:rw:$(ESP_DIR) \
	    -net none \
	    -m 256M \
	    -display none \
	    -serial stdio \
	    -no-reboot

# ── Inspect target ────────────────────────────────────────────────────────────

inspect: $(EFI_APP)
	@echo "=== EFI file info ==="
	file $(EFI_APP)
	@echo ""
	@echo "=== PE32+ headers ==="
	objdump -f $(EFI_APP)
	@echo ""
	@echo "=== Sections ==="
	objdump -h $(EFI_APP)

# ── Clean ─────────────────────────────────────────────────────────────────────

clean:
	rm -rf bin $(ESP_DIR)
