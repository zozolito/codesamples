; =============================================================================
; boot.asm — Legacy x86 BIOS Bootloader (MBR)
; CodeSamples Project — Bare Metal Series
;
; What this is:
;   The FIRST CODE the CPU executes after POST (Power-On Self-Test).
;   When you power on a PC, the BIOS/firmware searches bootable drives
;   for a 512-byte sector whose last two bytes are 0x55 0xAA (the boot
;   signature). It loads that sector at physical address 0x7C00 and jumps
;   to it. THIS FILE is that sector.
;
; CPU state when BIOS hands us control:
;   - 16-bit Real Mode (same as the Intel 8086 from 1978)
;   - CS:IP = 0x0000:0x7C00 (or 0x07C0:0x0000 — varies by BIOS)
;   - DL = boot drive number (0x80 for first HDD)
;   - Interrupts enabled
;   - A20 line may or may not be enabled
;   - No stack set up yet — we must do it ourselves
;
; Assemble:   nasm -f bin src/boot.asm -o bin/boot.bin
; Test:       qemu-system-x86_64 -drive format=raw,file=bin/boot.bin
; Write to USB (Linux, careful!):
;             sudo dd if=bin/boot.bin of=/dev/sdX bs=512 count=1
;
; =============================================================================

; ── Assembler directives ──────────────────────────────────────────────────────

[BITS 16]           ; Tell NASM to generate 16-bit code.
                    ; The CPU starts in Real Mode: 16-bit registers, 20-bit addresses.
                    ; Protected mode (32-bit) and Long mode (64-bit) must be entered
                    ; explicitly — we stay in real mode here for simplicity.

[ORG 0x7C00]        ; Tell NASM where this code will be loaded in memory.
                    ; The BIOS ALWAYS loads the MBR at physical address 0x7C00.
                    ; This affects how forward references and labels are resolved.

; ── Entry point ──────────────────────────────────────────────────────────────

start:
    ; Step 1: Disable interrupts while we set up segment registers.
    ; The CPU uses CS:IP for code, DS:SI for data, SS:SP for stack.
    ; BIOS may leave these in an inconsistent state.
    cli                         ; Clear Interrupt Flag — disable hardware interrupts
                                ; (safe to disable briefly while we configure SS:SP)

    ; Zero out segment registers.
    ; In real mode: effective address = segment_register × 16 + offset
    ; With all segments = 0: effective address = offset (flat zero-based addressing)
    xor  ax, ax                 ; ax = 0 (xor is faster than mov ax, 0)
    mov  ds, ax                 ; Data Segment = 0
    mov  es, ax                 ; Extra Segment = 0 (used by some string instructions)
    mov  ss, ax                 ; Stack Segment = 0

    ; Set up the stack.
    ; The stack grows DOWN. We point it at 0x7C00 — our code starts there,
    ; but the stack will grow into lower addresses (0x7BFE, 0x7BFC…).
    ; The BIOS reserved area ends around 0x500, so we have ~30 KB of usable stack.
    mov  sp, 0x7C00             ; Stack Pointer = 0x7C00

    sti                         ; Set Interrupt Flag — re-enable hardware interrupts

    ; Step 2: Print our message character by character using BIOS INT 10h.
    mov  si, msg                ; SI = address of message string (DS:SI)

.print_loop:
    ; LODSB: Load byte from [DS:SI] into AL, then increment SI.
    ; Equivalent to: AL = *SI; SI++;
    lodsb                       ; AL = next character; SI advances

    ; Check if we've reached the null terminator (end of string).
    test al, al                 ; bitwise AND with itself — sets Zero Flag if AL == 0
    jz   .halt                  ; Jump if Zero — string is done

    ; BIOS teletype output: INT 10h with AH = 0x0E
    ; The BIOS handles cursor positioning, scrolling, etc.
    mov  ah, 0x0E               ; AH = 0x0E: Teletype Output function
    xor  bh, bh                 ; BH = 0: display page 0 (standard VGA page)
    mov  bl, 0x0F               ; BL = 0x0F: white text (only matters in some BIOS)
    int  0x10                   ; Call BIOS video interrupt

    jmp  .print_loop            ; Repeat for next character

; ── Halt ─────────────────────────────────────────────────────────────────────
; When all characters are printed, we hang the CPU.
; A real OS would continue here to load a kernel from disk.

.halt:
    ; Print a final "Press any key" prompt
    mov  si, msg_wait
    call print_string

    ; Wait for a keystroke using BIOS INT 16h, AH=0: "get keystroke"
    xor  ah, ah
    int  0x16                   ; Blocks until a key is pressed

    ; Infinite loop — the CPU sleeps between interrupts
    cli                         ; Disable interrupts
    hlt                         ; Halt the CPU until next interrupt (which won't come)
    jmp  .halt                  ; If an NMI fires anyway, halt again

; ── Subroutine: print_string ─────────────────────────────────────────────────
; Prints a null-terminated string pointed to by SI.
; Uses BIOS INT 10h teletype (same as .print_loop above).

print_string:
    push ax                     ; Save registers we'll use (caller-save convention)
    push bx
.ps_loop:
    lodsb
    test al, al
    jz   .ps_done
    mov  ah, 0x0E
    xor  bh, bh
    int  0x10
    jmp  .ps_loop
.ps_done:
    pop  bx
    pop  ax
    ret                         ; Return to caller (pops IP from stack)

; ── Data ─────────────────────────────────────────────────────────────────────
; In real mode, data and code share the same segment.
; We place strings after the code.

; 0x0D = Carriage Return (CR), 0x0A = Line Feed (LF)
; 0x00 = null terminator
msg      db 'Hello my OS', 0x0D, 0x0A, 0x00
msg_wait db 'Press any key to halt...', 0x0D, 0x0A, 0x00

; ── Boot sector padding and magic number ─────────────────────────────────────
; A valid MBR sector is EXACTLY 512 bytes.
; The BIOS reads exactly 512 bytes and checks bytes 510–511 for 0x55 0xAA.
; If the signature is wrong, the BIOS does not boot from this sector.

; 'times' is a NASM directive that repeats the next expression.
; '$'  = current assembly position (absolute).
; '$$' = start of the current section (absolute).
; So ($ - $$) = number of bytes assembled so far.
; We fill with 0x00 from here to byte 510 (0-indexed: bytes 0..509).

times 510 - ($ - $$) db 0x00   ; Pad with zero bytes to reach byte offset 510

; Boot signature — MUST be the last two bytes of the sector.
; BIOS checks for this pattern at offset 510.
; Note: 'dw' writes a 16-bit little-endian word.
; dw 0xAA55 → stored as bytes: 0x55 then 0xAA (little-endian).
; That's why the BIOS sees 0x55 at offset 510 and 0xAA at offset 511.

dw 0xAA55                       ; Boot signature (little-endian → 55 AA on disk)

; ── End of file ──────────────────────────────────────────────────────────────
; Total: exactly 512 bytes.
; NASM will error if the code + data + padding exceeds 510 bytes before the signature.
