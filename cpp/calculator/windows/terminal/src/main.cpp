/**
 * main.cpp — Windows Terminal UI
 * CodeSamples · cpp/calculator/windows/terminal
 *
 * This is the ONLY file that knows about the terminal.
 * It calls Calculator::press*() methods to send input to the engine.
 * It calls Calculator::getView() to get state for rendering.
 * It knows nothing about how the engine does its calculations.
 *
 * ── Windows-specific APIs used ────────────────────────────────────────────────
 *
 *   <windows.h>  SetConsoleMode      — enable ANSI escape codes (VT processing)
 *                SetConsoleOutputCP  — set UTF-8 output (for ×, ÷, √, etc.)
 *                SetConsoleTitle     — set the terminal window title
 *                GetConsoleScreenBufferInfo — read current console size
 *
 *   <conio.h>    _getch()            — read a single keypress without Enter
 *                                      returns 0x00 or 0xE0 for special keys
 *                                      (function keys, arrows) — read a second
 *                                      byte to get the actual key code
 *
 * ── Key mapping ───────────────────────────────────────────────────────────────
 *
 *   Digits:    0–9
 *   Operators: + - * /
 *   Equals:    = or Enter (13)
 *   Decimal:   .
 *   Negate:    n
 *   Percent:   %
 *   Sqrt:      s
 *   Reciprocal: i
 *   Backspace: Backspace (8)
 *   CE:        Delete (0xE0 + 0x53)
 *   Clear:     Escape (27)
 *   Memory:    F1=MC  F2=MR  F3=M+  F4=M-  F5=MS
 *   History:   h (clear)
 *   Quit:      q or Ctrl+C
 */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "engine/Calculator.hpp"


// ─────────────────────────────────────────────────────────────────────────────
// ANSI escape codes
//
// Windows 10 (v1511+) supports ANSI escape sequences when VT processing is
// enabled via SetConsoleMode. We enable it in setupConsole() below.
//
// The codes are standard VT100/ANSI — the same ones used by Linux terminals.
// ─────────────────────────────────────────────────────────────────────────────

namespace ansi {
    // Cursor control
    constexpr const char* HOME         = "\033[H";        // move cursor to top-left
    constexpr const char* CLEAR        = "\033[2J\033[H"; // clear screen + home
    constexpr const char* HIDE_CURSOR  = "\033[?25l";
    constexpr const char* SHOW_CURSOR  = "\033[?25h";

    // Text attributes
    constexpr const char* RESET        = "\033[0m";
    constexpr const char* BOLD         = "\033[1m";
    constexpr const char* DIM          = "\033[2m";

    // Foreground colours (standard 16-colour palette)
    constexpr const char* FG_BLACK     = "\033[30m";
    constexpr const char* FG_WHITE     = "\033[37m";
    constexpr const char* FG_GRAY      = "\033[90m";  // bright black = dark gray
    constexpr const char* FG_RED       = "\033[91m";
    constexpr const char* FG_GREEN     = "\033[92m";
    constexpr const char* FG_YELLOW    = "\033[93m";
    constexpr const char* FG_BLUE      = "\033[94m";
    constexpr const char* FG_CYAN      = "\033[96m";
    constexpr const char* FG_BWHITE    = "\033[97m";  // bright white
}


// ─────────────────────────────────────────────────────────────────────────────
// Layout constants
// ─────────────────────────────────────────────────────────────────────────────

// The inner width of the calculator box (between the │ borders).
// Total terminal width required: INNER_W + 2 (for borders) = 58 columns.
static constexpr int INNER_W = 56;

// How many history entries to show
static constexpr int HISTORY_ROWS = 6;


// ─────────────────────────────────────────────────────────────────────────────
// Console setup
// ─────────────────────────────────────────────────────────────────────────────

static void setupConsole() {
    // Enable Virtual Terminal Processing so ANSI escape codes work
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD  mode = 0;
    if (GetConsoleMode(hOut, &mode)) {
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }

    // UTF-8 output — required for ×, ÷, √, −, ⌫ to render correctly
    SetConsoleOutputCP(CP_UTF8);

    // Set the terminal window title
    SetConsoleTitleA("C++ Calculator - Windows Terminal [CodeSamples]");
}


// ─────────────────────────────────────────────────────────────────────────────
// Rendering helpers
// ─────────────────────────────────────────────────────────────────────────────

// Pad string s to exactly `width` visible characters.
// rightAlign = true → right-align within the field.
// NOTE: `s` must NOT contain ANSI escape codes — they have zero visible width
//       but non-zero string length, which would break the padding calculation.
static std::string pad(const std::string& s, int width, bool rightAlign = false) {
    // Clamp: if the string is already wider, truncate from the left
    if ((int)s.size() >= width) return s.substr(s.size() - width);
    int spaces = width - (int)s.size();
    return rightAlign
        ? std::string(spaces, ' ') + s
        : s + std::string(spaces, ' ');
}

// Print one bordered line. `content` must be plain text (no ANSI).
// `colorPrefix` is applied to the content; it must end with ansi::RESET or
// be empty. The border itself is always dim.
static void borderLine(const std::string& content,
                       const std::string& colorPrefix = "",
                       bool rightAlign = false) {
    // The content is padded BEFORE adding colour codes, so the width is exact.
    std::string padded = pad(content, INNER_W - 1, rightAlign);
    std::cout
        << ansi::GRAY << "│" << ansi::RESET
        << " " << colorPrefix << padded << ansi::RESET
        << ansi::GRAY << "│\n" << ansi::RESET;
}

// Print a horizontal rule, e.g.: ├────────────────────────────────────────────┤
static void separator(char left = '\xC3', char right = '\xB4') {
    // The left and right chars are the codepage-437 box characters, but
    // since we're in UTF-8 mode we use the UTF-8 box drawing characters.
    (void)left; (void)right; // suppress unused warnings
    std::cout << ansi::GRAY
              << "├" << std::string(INNER_W, '─') << "┤\n"
              << ansi::RESET;
}


// ─────────────────────────────────────────────────────────────────────────────
// render() — draw the entire calculator UI
//
// Called after every keypress. Moves the cursor to the top of the screen
// (HOME) and overwrites the previous frame — no flicker, no clear-screen flash.
//
// All rendering is buffered in an ostringstream and written in a single
// std::cout call at the end to minimise screen tearing.
// ─────────────────────────────────────────────────────────────────────────────

static void render(const Calculator& calc) {
    CalcView view = calc.getView();

    std::ostringstream buf;
    // Redirect std::cout to our buffer for this function
    std::streambuf* origBuf = std::cout.rdbuf(buf.rdbuf());

    // Move to top-left without clearing — overwrites previous content
    std::cout << ansi::HOME;

    // ── Top border ───────────────────────────────────────────────────────────
    std::cout << ansi::GRAY << "┌" << std::string(INNER_W, '─') << "┐\n" << ansi::RESET;

    // ── Title ─────────────────────────────────────────────────────────────────
    borderLine("  C++ Calculator  ·  Windows Terminal", ansi::BOLD + std::string(ansi::FG_YELLOW));

    // ── Expression line ───────────────────────────────────────────────────────
    separator();
    {
        // [M] indicator occupies 4 characters (3 visible + 1 space)
        std::string memTag  = view.memoryActive ? "[M]" : "   ";
        std::string memCol  = view.memoryActive ? ansi::CYAN : ansi::GRAY;
        std::string expr    = pad(view.expression, INNER_W - 5, true);

        std::cout << ansi::GRAY << "│ " << ansi::RESET
                  << memCol << memTag << ansi::RESET
                  << " " << ansi::DIM << ansi::FG_WHITE << expr << ansi::RESET
                  << " " << ansi::GRAY << "│\n" << ansi::RESET;
    }

    // ── Main display (right-aligned, large) ───────────────────────────────────
    {
        std::string col = view.isError
            ? std::string(ansi::BOLD) + ansi::FG_RED
            : std::string(ansi::BOLD) + ansi::FG_BWHITE;
        borderLine(view.display, col, /*rightAlign=*/true);
    }

    // ── Memory buttons ────────────────────────────────────────────────────────
    separator();
    {
        const char* mc = view.memoryActive ? ansi::FG_CYAN : ansi::GRAY;
        std::cout << ansi::GRAY << "│ " << ansi::RESET
                  << ansi::YELLOW << "F1" << ansi::RESET << " " << mc    << "MC" << ansi::RESET
                  << "   "
                  << ansi::YELLOW << "F2" << ansi::RESET << " " << mc    << "MR" << ansi::RESET
                  << "   "
                  << ansi::YELLOW << "F3" << ansi::RESET << " " << mc    << "M+" << ansi::RESET
                  << "   "
                  << ansi::YELLOW << "F4" << ansi::RESET << " " << mc    << "M−" << ansi::RESET
                  << "   "
                  << ansi::YELLOW << "F5" << ansi::RESET << " " << ansi::FG_BWHITE << "MS" << ansi::RESET
                  << std::string(INNER_W - 39, ' ')
                  << ansi::GRAY << "│\n" << ansi::RESET;
    }

    // ── Function buttons ──────────────────────────────────────────────────────
    // Row: CE  C  ±  %  √  1/x  ÷
    separator();
    {
        std::cout << ansi::GRAY << "│ " << ansi::RESET
                  << ansi::YELLOW << "Del" << ansi::RESET << " " << ansi::FG_BWHITE << "CE" << ansi::RESET
                  << "   "
                  << ansi::YELLOW << "Esc" << ansi::RESET << " " << ansi::FG_BWHITE << "C" << ansi::RESET
                  << "    "
                  << ansi::YELLOW << "n"   << ansi::RESET << " " << ansi::FG_BWHITE << "±" << ansi::RESET
                  << "    "
                  << ansi::YELLOW << "%"   << ansi::RESET << " " << ansi::FG_BWHITE << "%" << ansi::RESET
                  << "    "
                  << ansi::YELLOW << "s"   << ansi::RESET << " " << ansi::FG_BWHITE << "√" << ansi::RESET
                  << "    "
                  << ansi::YELLOW << "i"   << ansi::RESET << " " << ansi::FG_BWHITE << "1/x" << ansi::RESET
                  << "  "
                  << ansi::YELLOW << "/"   << ansi::RESET << " " << ansi::FG_YELLOW << "÷" << ansi::RESET
                  << std::string(INNER_W - 53, ' ')
                  << ansi::GRAY << "│\n" << ansi::RESET;
    }

    // ── Digit + operator grid ─────────────────────────────────────────────────
    //
    //  Layout:
    //   7   8   9   ×
    //   4   5   6   −
    //   1   2   3   +
    //   0   .   ⌫   =
    //
    // Each row is drawn as: [key] digit    [key] digit    [key] digit    [key] op

    auto digitRow = [&](const std::string& k1, const std::string& d1,
                        const std::string& k2, const std::string& d2,
                        const std::string& k3, const std::string& d3,
                        const std::string& kop, const std::string& op,
                        const std::string& opCol = ansi::FG_YELLOW) {
        std::cout << ansi::GRAY << "│ " << ansi::RESET
                  << ansi::YELLOW << k1  << ansi::RESET << " " << ansi::FG_BWHITE << d1 << ansi::RESET
                  << "           "
                  << ansi::YELLOW << k2  << ansi::RESET << " " << ansi::FG_BWHITE << d2 << ansi::RESET
                  << "           "
                  << ansi::YELLOW << k3  << ansi::RESET << " " << ansi::FG_BWHITE << d3 << ansi::RESET
                  << "           "
                  << ansi::YELLOW << kop << ansi::RESET << " " << opCol << op << ansi::RESET
                  << std::string(INNER_W - 52, ' ')
                  << ansi::GRAY << "│\n" << ansi::RESET;
    };

    separator();
    digitRow("7","7",  "8","8",  "9","9",  "*","×");
    digitRow("4","4",  "5","5",  "6","6",  "-","−");
    digitRow("1","1",  "2","2",  "3","3",  "+","+");

    // Last row: 0  .  ⌫  =  — uses different key names
    {
        std::cout << ansi::GRAY << "│ " << ansi::RESET
                  << ansi::YELLOW << "0"     << ansi::RESET << " " << ansi::FG_BWHITE << "0" << ansi::RESET
                  << "           "
                  << ansi::YELLOW << "."     << ansi::RESET << " " << ansi::FG_BWHITE << "." << ansi::RESET
                  << "           "
                  << ansi::YELLOW << "Bksp"  << ansi::RESET << " " << ansi::FG_BWHITE << "⌫" << ansi::RESET
                  << "        "
                  << ansi::YELLOW << "Enter" << ansi::RESET << " " << ansi::FG_GREEN << "=" << ansi::RESET
                  << std::string(INNER_W - 50, ' ')
                  << ansi::GRAY << "│\n" << ansi::RESET;
    }

    // ── History ───────────────────────────────────────────────────────────────
    separator();
    {
        // Header row with [h] clear button
        std::string header   = "History";
        std::string clearKey = "[h] clear";
        int gap = INNER_W - 1 - (int)header.size() - (int)clearKey.size();
        std::cout << ansi::GRAY << "│ " << ansi::RESET
                  << ansi::BOLD << ansi::FG_CYAN << header << ansi::RESET
                  << std::string(gap, ' ')
                  << ansi::GRAY << clearKey << ansi::RESET
                  << ansi::GRAY << "│\n" << ansi::RESET;
    }

    // History entries — show the most recent HISTORY_ROWS entries
    const auto& hist  = view.history;
    int start = static_cast<int>(hist.size()) - HISTORY_ROWS;
    if (start < 0) start = 0;

    if (hist.empty()) {
        borderLine("  (no history yet)", ansi::GRAY);
        for (int i = 1; i < HISTORY_ROWS; ++i) borderLine("");
    } else {
        for (int i = start; i < static_cast<int>(hist.size()); ++i) {
            borderLine("  " + hist[i], ansi::GRAY);
        }
        // Pad with blank lines to keep the box height constant
        for (int i = static_cast<int>(hist.size()) - start; i < HISTORY_ROWS; ++i) {
            borderLine("");
        }
    }

    // ── Bottom border ─────────────────────────────────────────────────────────
    std::cout << ansi::GRAY << "└" << std::string(INNER_W, '─') << "┘\n" << ansi::RESET;

    // ── Status line (below the box) ───────────────────────────────────────────
    std::cout << ansi::GRAY
              << "  [q] quit    floating-point: 0.1+0.2 = "
              << ansi::RESET << ansi::YELLOW << "0.3"
              << ansi::GRAY << " (displayed) / "
              << ansi::RESET << ansi::FG_RED << "0.30000000000000004"
              << ansi::GRAY << " (exact double)\n"
              << ansi::RESET;

    // Flush the buffer to the real stdout in one write
    std::cout.rdbuf(origBuf);
    std::cout << buf.str();
    std::cout.flush();
}


// ─────────────────────────────────────────────────────────────────────────────
// handleKey() — translate a raw keycode to a Calculator press*() call
//
// Returns false when the user wants to quit, true to keep running.
// ─────────────────────────────────────────────────────────────────────────────

static bool handleKey(Calculator& calc) {
    int ch = _getch();

    // ── Special key prefix ────────────────────────────────────────────────────
    // _getch() returns 0x00 or 0xE0 for extended keys (function keys, arrows,
    // Delete, etc.). A second _getch() gives the actual scan code.
    if (ch == 0x00 || ch == 0xE0) {
        int scan = _getch();
        switch (scan) {
            case 0x3B: calc.pressMemoryClear();    break; // F1 → MC
            case 0x3C: calc.pressMemoryRecall();   break; // F2 → MR
            case 0x3D: calc.pressMemoryAdd();      break; // F3 → M+
            case 0x3E: calc.pressMemorySubtract(); break; // F4 → M-
            case 0x3F: calc.pressMemoryStore();    break; // F5 → MS
            case 0x53: calc.pressClearEntry();     break; // Delete → CE
            // Arrow keys and other extended keys are silently ignored
            default: break;
        }
        return true;
    }

    // ── Regular keys ──────────────────────────────────────────────────────────
    switch (ch) {
        // Digits
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            calc.pressDigit(static_cast<char>(ch));
            break;

        // Decimal point
        case '.': case ',': // , is the decimal separator in many European locales
            calc.pressDecimal();
            break;

        // Binary operators
        case '+': calc.pressOperator(Operator::Add);      break;
        case '-': calc.pressOperator(Operator::Subtract); break;
        case '*': calc.pressOperator(Operator::Multiply); break;
        case '/': calc.pressOperator(Operator::Divide);   break;

        // Equals
        case '=':
        case '\r': // Enter (Carriage Return in console mode)
        case '\n':
            calc.pressEquals();
            break;

        // Single-operand
        case 'n': case 'N': calc.pressNegate();      break;
        case '%':           calc.pressPercent();     break;
        case 's': case 'S': calc.pressSqrt();        break;
        case 'i': case 'I': calc.pressReciprocal();  break;

        // Correction
        case '\b': // Backspace (ASCII 8)
            calc.pressBackspace();
            break;
        case 27:   // Escape
            calc.pressClear();
            break;

        // History
        case 'h': case 'H':
            calc.clearHistory();
            break;

        // Quit
        case 'q': case 'Q':
        case 3:    // Ctrl+C
            return false;

        default:
            // Unknown key — silently ignored
            break;
    }

    return true;
}


// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────

int main() {
    setupConsole();

    // Hide the blinking cursor — it would distract from the calculator display
    std::cout << ansi::HIDE_CURSOR;

    // Clear the screen once at startup
    std::cout << ansi::CLEAR;

    Calculator calc;

    // Draw the initial state
    render(calc);

    // ── Main loop ──────────────────────────────────────────────────────────────
    // Each keypress:
    //   1. handleKey() translates the key and calls the appropriate press*() method
    //   2. render() redraws the entire UI from the new state
    //
    // No partial updates, no dirty flags — just redraw everything.
    // With INNER_W=56 and ~25 rows, this is ~1400 characters per frame:
    // negligible for a console at human typing speed.
    while (true) {
        bool keepRunning = handleKey(calc);
        render(calc);
        if (!keepRunning) break;
    }

    // Restore cursor before exiting
    std::cout << ansi::SHOW_CURSOR << '\n';
    std::cout << "Goodbye.\n";

    return 0;
}
