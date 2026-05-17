#pragma once

/**
 * Calculator.hpp — Calculator Engine
 * CodeSamples · cpp/calculator/windows/terminal
 *
 * This file defines the ENTIRE PUBLIC CONTRACT of the calculator.
 * It has zero knowledge of terminals, colours, keyboards, or windows.
 * It takes abstract input events and exposes state through CalcView.
 *
 * This separation is the point: the same engine can power a console UI,
 * a GUI, a test harness, or a web server — without changing a single line.
 *
 * ── Architecture ─────────────────────────────────────────────────────────────
 *
 *   main.cpp (UI layer)
 *       │  calls pressDigit(), pressOperator(), etc.
 *       │  calls getView() to get a snapshot for rendering
 *       ▼
 *   Calculator (engine)
 *       │  maintains state_ machine
 *       │  computes results, records history
 *       └─ knows nothing about the screen
 */

#include <string>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// Operator — the pending binary operation
//
// Using `enum class` (scoped enum, C++11) forces us to write Operator::Add
// instead of just Add, preventing name collisions with anything else named Add.
// The underlying type is int by default — we keep that.
// ─────────────────────────────────────────────────────────────────────────────
enum class Operator {
    None,
    Add,
    Subtract,
    Multiply,
    Divide
};

// ─────────────────────────────────────────────────────────────────────────────
// CalcState — where we are in the input state machine
//
//  EnteringFirst:
//    The user is typing the first operand.
//    Display shows currentInput_.
//    No operator has been pressed yet.
//
//  OperatorPressed:
//    An operator was pressed. firstOperand_ is locked in.
//    Display shows the locked first operand.
//    Waiting for the user to start typing the second operand.
//
//  EnteringSecond:
//    The user has started typing the second operand.
//    Display shows currentInput_ (the second number being built).
//
//  Error:
//    Something went wrong (÷0, √ of negative, overflow).
//    Display shows the error message.
//    Most input is ignored until CE or C is pressed.
// ─────────────────────────────────────────────────────────────────────────────
enum class CalcState {
    EnteringFirst,
    OperatorPressed,
    EnteringSecond,
    Error
};

// ─────────────────────────────────────────────────────────────────────────────
// CalcView — everything the UI needs to render one frame
//
// The engine produces this snapshot. The UI consumes it.
// No shared mutable state crosses this boundary.
//
// This is the View Model pattern: the engine computes *what* to display,
// the UI decides *how* to display it.
// ─────────────────────────────────────────────────────────────────────────────
struct CalcView {
    std::string display;                  // number shown on the main display
    std::string expression;               // e.g., "3 +" or "" when idle
    bool        memoryActive = false;     // true if memory register contains a value
    bool        isError      = false;     // true if in error state
    std::vector<std::string> history;     // operation log, oldest first
};

// ─────────────────────────────────────────────────────────────────────────────
// Calculator — the engine
// ─────────────────────────────────────────────────────────────────────────────
class Calculator {
public:
    Calculator();

    // ── Digit input ───────────────────────────────────────────────────────────
    void pressDigit(char digit);    // '0'–'9'
    void pressDecimal();            // '.' — ignored if one already present

    // ── Single-operand operations ─────────────────────────────────────────────
    // These transform the current display value in place and record in history.
    void pressNegate();             // ± — flip sign
    void pressPercent();            // % — context-sensitive (see impl for details)
    void pressSqrt();               // √ — fails with error if value < 0
    void pressReciprocal();         // 1/x — fails with error if value == 0

    // ── Binary operations ──────────────────────────────────────────────────────
    void pressOperator(Operator op); // + − × ÷ — chains if already building
    void pressEquals();              // = or Enter — evaluates; repeated = replays

    // ── Correction ────────────────────────────────────────────────────────────
    void pressClearEntry();         // CE — clears current input, keeps operator
    void pressClear();              // C  — full reset (memory and history kept)
    void pressBackspace();          // ⌫  — removes last digit

    // ── Memory ────────────────────────────────────────────────────────────────
    void pressMemoryStore();        // MS — save current value
    void pressMemoryRecall();       // MR — restore saved value to display
    void pressMemoryAdd();          // M+ — add current value to memory
    void pressMemorySubtract();     // M− — subtract current value from memory
    void pressMemoryClear();        // MC — erase memory

    // ── Misc ──────────────────────────────────────────────────────────────────
    void clearHistory();

    // ── State snapshot ────────────────────────────────────────────────────────
    // The UI calls this once per frame to get everything it needs to render.
    CalcView getView() const;

private:
    // ── Core state ────────────────────────────────────────────────────────────
    CalcState   state_;
    std::string currentInput_;       // the string being typed, e.g., "3.14"
    double      firstOperand_;       // locked left side of the pending operation
    Operator    pendingOp_;          // the pending operator
    bool        justPressedEquals_;  // true right after = — next digit starts fresh

    // Saved for repeated = (e.g., "5 + 3 = = =" → 8, 11, 14)
    Operator    lastOp_;
    double      lastOperand_;

    // ── Memory register ───────────────────────────────────────────────────────
    double      memory_;
    bool        memoryActive_;

    // ── Error ─────────────────────────────────────────────────────────────────
    std::string errorMessage_;

    // ── History ───────────────────────────────────────────────────────────────
    std::vector<std::string> history_;

    // ── Private helpers ───────────────────────────────────────────────────────
    double      parseInput()                       const;
    std::string formatNumber(double value)         const;
    static std::string operatorStr(Operator op);

    // Evaluate firstOperand_ <pendingOp_> second, record history, update state.
    // The meat of the calculation happens here.
    void        finishOperation(double second);

    void        setError(const std::string& msg);
};
