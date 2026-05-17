# =============================================================================
# Makefile — Legacy x86 BIOS Bootloader
# CodeSamples Project — Bare Metal Series
# =============================================================================

# Tools
NASM   = nasm
QEMU   = qemu-system-x86_64

# Output
BIN    = bin/boot.bin

# Source
SRC    = src/boot.asm

.PHONY: all run run-curses clean

# Default target
all: $(BIN)

# Assemble: -f bin produces a flat binary (no ELF headers — raw machine code)
$(BIN): $(SRC) | bin
	$(NASM) -f bin $(SRC) -o $(BIN)
	@echo ""
	@echo "  Built: $(BIN)"
	@ls -la $(BIN)
	@echo "  Size: $$(wc -c < $(BIN)) bytes (must be exactly 512)"

# Create output directory
bin:
	mkdir -p bin

# Run in QEMU — opens a window with VGA text output
# -drive format=raw,file=... : treat the binary as a raw disk image
# -boot c                    : boot from the first "hard disk" (our image)
run: $(BIN)
	@echo "Starting QEMU... (close window or press Ctrl+C to exit)"
	$(QEMU) \
	    -drive format=raw,file=$(BIN),if=ide,media=disk \
	    -boot order=c \
	    -m 128M \
	    -no-reboot

# Run in curses mode (no GUI window — terminal only)
run-curses: $(BIN)
	$(QEMU) \
	    -drive format=raw,file=$(BIN),if=ide,media=disk \
	    -boot order=c \
	    -m 128M \
	    -display curses \
	    -no-reboot

# Run headless with serial output (useful in CI / no display)
run-serial: $(BIN)
	$(QEMU) \
	    -drive format=raw,file=$(BIN),if=ide,media=disk \
	    -boot order=c \
	    -m 128M \
	    -display none \
	    -serial stdio \
	    -no-reboot

# Show the raw bytes of the boot sector (debugging)
inspect: $(BIN)
	@echo "=== First 16 bytes (jump instruction + NOP) ==="
	xxd -l 16 $(BIN)
	@echo ""
	@echo "=== Last 4 bytes (must be 00 00 55 AA) ==="
	xxd -s 508 $(BIN)
	@echo ""
	@echo "=== Disassembly (16-bit real mode) ==="
	ndisasm -b 16 $(BIN) | head -40

clean:
	rm -rf bin
