/**
 * main.c — UEFI Hello World Application
 * CodeSamples Project — Bare Metal Series
 *
 * What this is:
 *   A minimal UEFI application — the modern way to boot x86_64/ARM firmware.
 *   UEFI replaced legacy BIOS on all PC hardware after ~2012.
 *
 *   This code runs BEFORE any operating system. It executes in the UEFI
 *   environment provided by the firmware (e.g., Intel TianoCore / EDK2).
 *
 * How it works:
 *   1. UEFI firmware loads this compiled .EFI file from the EFI System Partition
 *      (FAT32, at /EFI/BOOT/BOOTX64.EFI on x86_64).
 *   2. Firmware calls efi_main(ImageHandle, SystemTable).
 *   3. SystemTable->ConOut gives us a text output interface.
 *   4. We print "Hello my OS" and wait for a key press.
 *
 * Key UEFI concepts:
 *   - Everything goes through the SystemTable (boot services, protocols, I/O).
 *   - Text output uses CHAR16 (UTF-16LE) — that's why strings are L"...".
 *   - The calling convention is Microsoft x64 ABI (even on Linux hardware).
 *   - No C standard library — no printf, no malloc, no string.h.
 *     Everything must go through UEFI protocols.
 *
 * Build (requires gnu-efi — see README for install):
 *   make
 *
 * Test with QEMU + OVMF:
 *   make run
 *
 * Dependencies:
 *   sudo apt install gnu-efi ovmf    (Debian/Ubuntu)
 *   sudo dnf install gnu-efi-devel edk2-ovmf   (Fedora)
 */

#include <efi.h>      /* UEFI types and protocol definitions — from gnu-efi */
#include <efilib.h>   /* InitializeLib, Print, uefi_call_wrapper — gnu-efi helpers */

/* ─────────────────────────────────────────────────────────────────────────────
   UEFI APPLICATION ENTRY POINT
   ─────────────────────────────────────────────────────────────────────────────
   This is the UEFI equivalent of main().

   Signature is mandated by the UEFI specification:
     EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
                                 EFI_SYSTEM_TABLE *SystemTable)

   EFI_STATUS: 64-bit integer. 0 = success, anything else = error.
   EFIAPI:     expands to __attribute__((ms_abi)) — Microsoft x64 calling convention.
   EFI_HANDLE: opaque handle for this loaded image.
   EFI_SYSTEM_TABLE: the master table — gateway to ALL UEFI services.
*/

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    /* ── Step 1: Initialise the gnu-efi helper library ── */
    /*
     * InitializeLib sets up:
     *   - ST  (global pointer to SystemTable)
     *   - BS  (global pointer to BootServices)
     *   - RT  (global pointer to RuntimeServices)
     *
     * This is a gnu-efi convenience. Without it, we'd write:
     *   SystemTable->ConOut->OutputString(SystemTable->ConOut, L"hello");
     * With it, we can use Print() which handles formatting like printf.
     */
    InitializeLib(ImageHandle, SystemTable);

    /* ── Step 2: Clear the screen ── */
    /*
     * uefi_call_wrapper() is a gnu-efi macro that handles the calling
     * convention switch on 32-bit systems. On 64-bit it's a direct call.
     * Arguments: (function_pointer, arg_count, arg1, arg2, ...)
     *
     * ConOut->Reset(ConOut, ExtendedVerification):
     *   Resets the text output device. FALSE = soft reset (faster).
     */
    uefi_call_wrapper(SystemTable->ConOut->Reset, 2,
                      SystemTable->ConOut,
                      FALSE);

    /* ── Step 3: Set text attributes — bright white on blue ── */
    /*
     * ConOut->SetAttribute(ConOut, Attribute):
     *   Attribute is a bitfield: (background << 4) | foreground
     *   Colors: 0=Black 1=Blue 2=Green 3=Cyan 4=Red 5=Magenta 6=Brown 7=White
     *           8-15: bright versions (foreground only)
     *   EFI_BACKGROUND_BLUE = 0x40 = (1 << 4) = blue background
     *   EFI_WHITE = 0x0F = bright white foreground
     */
    uefi_call_wrapper(SystemTable->ConOut->SetAttribute, 2,
                      SystemTable->ConOut,
                      EFI_BACKGROUND_BLUE | EFI_WHITE);

    uefi_call_wrapper(SystemTable->ConOut->ClearScreen, 1,
                      SystemTable->ConOut);

    /* ── Step 4: Print our message ── */
    /*
     * Print() is a gnu-efi wrapper around ConOut->OutputString.
     * It accepts CHAR16* (UTF-16LE) format strings, just like wprintf.
     * The L"..." prefix creates a UTF-16 wide string literal in C.
     *
     * \n\r: UEFI text mode uses CR+LF for newlines (unlike Unix LF-only).
     *       The convention is actually \r\n, but \n\r works too with most
     *       UEFI firmware implementations.
     */
    Print(L"\n\r");
    Print(L"  ============================================\n\r");
    Print(L"    Hello my OS\n\r");
    Print(L"    UEFI Bare Metal — CodeSamples Project\n\r");
    Print(L"  ============================================\n\r");
    Print(L"\n\r");

    /* ── Step 5: Show UEFI system information ── */
    /*
     * The SystemTable contains rich information about the firmware.
     * FirmwareVendor is a CHAR16* (UTF-16) string.
     * FirmwareRevision is a UINT32 version number.
     */
    Print(L"  Firmware vendor:   %s\n\r",  SystemTable->FirmwareVendor);
    Print(L"  Firmware revision: %08X\n\r", SystemTable->FirmwareRevision);
    Print(L"  UEFI revision:     %d.%d\n\r",
          SystemTable->Hdr.Revision >> 16,     /* major version in high 16 bits */
          SystemTable->Hdr.Revision & 0xFFFF); /* minor version in low 16 bits */
    Print(L"\n\r");

    /* ── Step 6: Show memory map summary ── */
    /*
     * In a real OS, you'd call GetMemoryMap() here to discover usable RAM
     * before calling ExitBootServices(). We just print a hint.
     */
    Print(L"  To load a real kernel, the next steps would be:\n\r");
    Print(L"    1. Call GetMemoryMap() — discover available RAM\n\r");
    Print(L"    2. Call ExitBootServices() — take control from UEFI\n\r");
    Print(L"    3. Set up GDT, paging, IDT\n\r");
    Print(L"    4. Jump to the kernel entry point\n\r");
    Print(L"\n\r");

    /* ── Step 7: Wait for a key press before halting ── */
    /*
     * ConIn->ReadKeyStroke() blocks until a key is pressed.
     * We use the BootServices->WaitForEvent() for a clean wait.
     * Simpler: just call WaitForKey which gnu-efi provides.
     */
    Print(L"  Press any key to exit...\n\r");

    /*
     * WaitForKey: waits for a keystroke event.
     * ST is a global gnu-efi variable = SystemTable.
     * ST->ConIn->WaitForKey is an EFI_EVENT (UEFI's async event handle).
     */
    UINTN          index;
    EFI_INPUT_KEY  key;

    /* WaitForEvent blocks until at least one of the events fires */
    uefi_call_wrapper(SystemTable->BootServices->WaitForEvent, 3,
                      1,                              /* number of events to wait on */
                      &SystemTable->ConIn->WaitForKey,/* pointer to event array */
                      &index);                        /* which event fired (out) */

    /* Read and discard the key that was pressed */
    uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2,
                      SystemTable->ConIn, &key);

    /* ── Step 8: Return to UEFI firmware ── */
    /*
     * EFI_SUCCESS = 0. Returning here gives control back to the UEFI shell
     * or firmware boot manager (which may then try the next boot entry,
     * show a menu, or power off — depends on firmware settings).
     *
     * A real OS loader would call ExitBootServices() here and NEVER return.
     */
    return EFI_SUCCESS;
}
