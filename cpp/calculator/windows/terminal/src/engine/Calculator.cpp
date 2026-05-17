/**
 * Calculator.cpp — Calculator Engine Implementation
 * CodeSamples · cpp/calculator/windows/terminal
 *
 * ── State machine overview ────────────────────────────────────────────────────
 *
 *  EnteringFirst ──[operator]──► OperatorPressed ──[digit]──► EnteringSecond
 *       ▲                              │                            │
 *       │                             │ [digit]→ EnteringSecond     │
 *       │                             │                            │
 *       └─────────────[=]─────────────┴────────────[=]─────────────┘
 *                   (result becomes new firstOperand_)
 *
 *  Any state ──[error]──► Error ──[CE or C]──► EnteringFirst
 *
 * ── Floating-point note ──────────────────────────────────────────────────────
 *
 *  This engine uses IEEE 754 double-precision (64-bit) floating-point, as does
 *  every mainstream language in this project.
 *
 *  The well-known consequence: 0.1 + 0.2 ≠ 0.3
 *  In binary floating-point: 0.1 + 0.2 = 0.30000000000000004
 *
 *  This is NOT a bug. It is a mathematical inevitability: most decimal fractions
 *  cannot be represented exactly in binary, just as 1/3 cannot be represented
 *  exactly in decimal.
 *
 *  Our fix: formatNumber() uses %.12g — 12 significant digits. The error only
 *  appears at the 16th–17th significant digit, so it never reaches the display.
 *  This is exactly what Windows Calculator does.
 *
 *  A real financial application would use decimal arithmetic (e.g., __int128
 *  scaled integers, or a proper Decimal library). For a calculator demo,
 *  %.12g is correct and standard.
 */

#include "Calculator.hpp"

#include <cmath>
#include <cassert>
#include <cstdio>
#include <stdexcept>

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────

Calculator::Calculator()
    : state_             (CalcState::EnteringFirst)
    , currentInput_      ("0")
    , firstOperand_      (0.0)
    , pendingOp_         (Operator::None)
    , justPressedEquals_ (false)
    , lastOp_            (Operator::None)
    , lastOperand_       (0.0)
    , memory_            (0.0)
    , memoryActive_      (false)
{}


// ─────────────────────────────────────────────────────────────────────────────
// pressDigit
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressDigit(char digit) {
    assert(digit >= '0' && digit <= '9');
    if (state_ == CalcState::Error) return;

    // After pressing =, the next digit begins a new calculation.
    // The previous result is discarded (not preserved as firstOperand_).
    if (justPressedEquals_) {
        currentInput_      = "";
        justPressedEquals_ = false;
        state_             = CalcState::EnteringFirst;
    }

    // Operator was pressed — the user is now starting the second number.
    if (state_ == CalcState::OperatorPressed) {
        currentInput_ = "";
        state_        = CalcState::EnteringSecond;
    }

    // Replace the initial "0" with the digit (unless the digit IS "0").
    // But keep "0." intact when a decimal point was already typed.
    if (currentInput_ == "0") {
        currentInput_ = (digit == '0') ? "0" : std::string(1, digit);
        return;
    }

    // Enforce a maximum of 12 digits to prevent display overflow.
    // Count only digit characters, not the sign or decimal point.
    int digitCount = 0;
    for (char c : currentInput_) {
        if (c >= '0' && c <= '9') digitCount++;
    }
    if (digitCount < 12) {
        currentInput_ += digit;
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pressDecimal
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressDecimal() {
    if (state_ == CalcState::Error) return;

    if (justPressedEquals_) {
        currentInput_      = "0";
        justPressedEquals_ = false;
        state_             = CalcState::EnteringFirst;
    }

    // First decimal digit after an operator starts the second number as "0."
    if (state_ == CalcState::OperatorPressed) {
        currentInput_ = "0";
        state_        = CalcState::EnteringSecond;
    }

    // Only add a decimal point if there isn't one already.
    if (currentInput_.find('.') == std::string::npos) {
        if (currentInput_.empty()) currentInput_ = "0";
        currentInput_ += '.';
    }
}


// ─────────────────────────────────────────────────────────────────────────────
// pressOperator
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressOperator(Operator op) {
    if (state_ == CalcState::Error) return;
    justPressedEquals_ = false;

    if (state_ == CalcState::EnteringSecond) {
        // Chaining: "3 + 4 ×" — evaluate the pending "3 + 4" first.
        // finishOperation() sets state to EnteringFirst with the result.
        finishOperation(parseInput());
        if (state_ == CalcState::Error) return; // division by zero etc.
    }

    // Capture the first operand and arm the operator.
    // If the user pressed a second operator without typing a second number
    // (state == OperatorPressed), we just change the operator — no evaluation.
    firstOperand_ = parseInput();
    pendingOp_    = op;
    state_        = CalcState::OperatorPressed;
}


// ─────────────────────────────────────────────────────────────────────────────
// pressEquals
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressEquals() {
    if (state_ == CalcState::Error) return;

    // ── Repeated = ────────────────────────────────────────────────────────────
    // After "5 + 3 =", pressing = again replays "+ 3" on the current result.
    // This matches the behaviour of Windows Calculator and most physical calculators.
    if (state_ == CalcState::EnteringFirst && justPressedEquals_) {
        if (lastOp_ == Operator::None) return;

        double current = parseInput();
        // Re-check division by zero for the replay
        if (lastOp_ == Operator::Divide && lastOperand_ == 0.0) {
            setError("Cannot divide by zero");
            return;
        }
        double result = current;
        switch (lastOp_) {
            case Operator::Add:      result = current + lastOperand_; break;
            case Operator::Subtract: result = current - lastOperand_; break;
            case Operator::Multiply: result = current * lastOperand_; break;
            case Operator::Divide:   result = current / lastOperand_; break;
            case Operator::None:     break;
        }
        if (std::isinf(result)) { setError("Overflow"); return; }

        history_.push_back(
            formatNumber(current) + " " + operatorStr(lastOp_) +
            " " + formatNumber(lastOperand_) + " = " + formatNumber(result)
        );
        currentInput_ = formatNumber(result);
        // justPressedEquals_ remains true so the next = repeats again
        return;
    }

    // ── Operator pressed but no second number entered yet ─────────────────────
    // e.g., "7 +" then "=" — behaves as "7 + 7 = 14"
    if (state_ == CalcState::OperatorPressed) {
        double second = firstOperand_; // use firstOperand_ as both operands
        lastOp_       = pendingOp_;
        lastOperand_  = second;
        finishOperation(second);
        return;
    }

    // ── Normal equals: first op second = result ────────────────────────────────
    double second = parseInput();
    lastOp_       = pendingOp_;
    lastOperand_  = second;
    finishOperation(second);
}


// ─────────────────────────────────────────────────────────────────────────────
// pressNegate
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressNegate() {
    if (state_ == CalcState::Error) return;

    // In OperatorPressed, we haven't started typing second number yet.
    // Negating here would negate firstOperand_ — potentially confusing.
    // Instead, start a negative second number.
    if (state_ == CalcState::OperatorPressed) {
        currentInput_ = "-0";
        state_        = CalcState::EnteringSecond;
        return;
    }

    double value  = parseInput();
    value         = -value;
    currentInput_ = formatNumber(value);
    justPressedEquals_ = false;
}


// ─────────────────────────────────────────────────────────────────────────────
// pressPercent
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressPercent() {
    if (state_ == CalcState::Error) return;

    double value = parseInput();

    if (state_ == CalcState::EnteringSecond || state_ == CalcState::OperatorPressed) {
        // Context-sensitive percentage — matches Windows Calculator:
        //   50 + 10% → adds 10% of 50 (= 5) → result 55
        //   50 - 10% → subtracts 10% of 50   → result 45
        //   50 × 10% → multiplies by 0.10     → result 5
        //   50 ÷ 10% → divides by 0.10        → result 500
        if (pendingOp_ == Operator::Add || pendingOp_ == Operator::Subtract) {
            value = firstOperand_ * (value / 100.0);
        } else {
            value = value / 100.0;
        }
    } else {
        // No operator — simple ÷ 100
        value = value / 100.0;
    }

    currentInput_      = formatNumber(value);
    justPressedEquals_ = false;
}


// ─────────────────────────────────────────────────────────────────────────────
// pressSqrt
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressSqrt() {
    if (state_ == CalcState::Error) return;

    double value = parseInput();
    if (value < 0.0) {
        setError("Invalid input for √");
        return;
    }

    double result = std::sqrt(value);
    history_.push_back("√(" + formatNumber(value) + ") = " + formatNumber(result));
    currentInput_      = formatNumber(result);
    justPressedEquals_ = false;
    // Stay in current state — user can continue using the result as an operand
}


// ─────────────────────────────────────────────────────────────────────────────
// pressReciprocal
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressReciprocal() {
    if (state_ == CalcState::Error) return;

    double value = parseInput();
    if (value == 0.0) {
        setError("Cannot divide by zero");
        return;
    }

    double result = 1.0 / value;
    history_.push_back("1/(" + formatNumber(value) + ") = " + formatNumber(result));
    currentInput_      = formatNumber(result);
    justPressedEquals_ = false;
}


// ─────────────────────────────────────────────────────────────────────────────
// pressClearEntry
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressClearEntry() {
    if (state_ == CalcState::Error) {
        // CE clears the error and returns to a usable state.
        // We can't restore what was being entered before the error,
        // so we default to "0" in EnteringFirst.
        errorMessage_ = "";
        state_        = CalcState::EnteringFirst;
        currentInput_ = "0";
        return;
    }

    // Clear only the current number being typed.
    // The pending operator and first operand are preserved.
    currentInput_      = "0";
    justPressedEquals_ = false;
    // state_ is unchanged — if in OperatorPressed, it stays there.
}


// ─────────────────────────────────────────────────────────────────────────────
// pressClear
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressClear() {
    // Full reset — but memory and history survive.
    state_             = CalcState::EnteringFirst;
    currentInput_      = "0";
    firstOperand_      = 0.0;
    pendingOp_         = Operator::None;
    lastOp_            = Operator::None;
    lastOperand_       = 0.0;
    justPressedEquals_ = false;
    errorMessage_      = "";
}


// ─────────────────────────────────────────────────────────────────────────────
// pressBackspace
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressBackspace() {
    if (state_ == CalcState::Error) {
        pressClearEntry();
        return;
    }

    // Nothing to delete when waiting for the second operand
    if (state_ == CalcState::OperatorPressed) return;

    if (currentInput_.empty() || currentInput_ == "0") return;

    currentInput_.pop_back();

    // If we deleted the last character, or only a minus sign remains, reset to "0"
    if (currentInput_.empty() || currentInput_ == "-") {
        currentInput_ = "0";
    }

    justPressedEquals_ = false;
}


// ─────────────────────────────────────────────────────────────────────────────
// Memory operations
// ─────────────────────────────────────────────────────────────────────────────

void Calculator::pressMemoryStore() {
    if (state_ == CalcState::Error) return;
    memory_       = parseInput();
    memoryActive_ = true;
}

void Calculator::pressMemoryRecall() {
    if (!memoryActive_) return;
    currentInput_      = formatNumber(memory_);
    justPressedEquals_ = false;
    // If we were waiting for the second number, start entering it now
    if (state_ == CalcState::OperatorPressed) {
        state_ = CalcState::EnteringSecond;
    }
}

void Calculator::pressMemoryAdd() {
    if (state_ == CalcState::Error) return;
    memory_      += parseInput();
    memoryActive_ = true;
}

void Calculator::pressMemorySubtract() {
    if (state_ == CalcState::Error) return;
    memory_      -= parseInput();
    memoryActive_ = true;
}

void Calculator::pressMemoryClear() {
    memory_       = 0.0;
    memoryActive_ = false;
}

void Calculator::clearHistory() {
    history_.clear();
}


// ─────────────────────────────────────────────────────────────────────────────
// getView — produces the snapshot the UI renders
// ─────────────────────────────────────────────────────────────────────────────

CalcView Calculator::getView() const {
    CalcView view;
    view.memoryActive = memoryActive_;
    view.history      = history_;

    if (state_ == CalcState::Error) {
        view.isError    = true;
        view.display    = errorMessage_;
        view.expression = "";
        return view;
    }

    view.isError = false;

    // ── Display ───────────────────────────────────────────────────────────────
    switch (state_) {
        case CalcState::EnteringFirst:
        case CalcState::EnteringSecond:
            view.display = currentInput_.empty() ? "0" : currentInput_;
            break;

        case CalcState::OperatorPressed:
            // Show the locked-in first operand while waiting for the second
            view.display = formatNumber(firstOperand_);
            break;

        default:
            view.display = "0";
    }

    // ── Expression ────────────────────────────────────────────────────────────
    if (state_ == CalcState::OperatorPressed || state_ == CalcState::EnteringSecond) {
        view.expression = formatNumber(firstOperand_) + " " + operatorStr(pendingOp_);
    } else {
        view.expression = "";
    }

    return view;
}


// ─────────────────────────────────────────────────────────────────────────────
// Private helpers
// ─────────────────────────────────────────────────────────────────────────────

double Calculator::parseInput() const {
    if (currentInput_.empty() || currentInput_ == "-") return 0.0;
    try {
        return std::stod(currentInput_);
    } catch (...) {
        return 0.0;
    }
}

std::string Calculator::formatNumber(double value) const {
    // Handle IEEE special values explicitly
    if (std::isinf(value)) return value > 0.0 ? "Infinity" : "-Infinity";
    if (std::isnan(value)) return "NaN";

    // %.12g: up to 12 significant digits.
    //   - No trailing zeros after the decimal point
    //   - No unnecessary decimal point (integer values show as "42", not "42.")
    //   - Scientific notation for very large/small values (e.g., "1.23456789012e+15")
    //
    // Why this hides floating-point noise:
    //   0.1 + 0.2 in double = 0.30000000000000004...
    //   With %.12g, only 12 significant digits are kept:
    //   → "0.3" (the noise is in the 16th digit, invisible here)
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.12g", value);
    return std::string(buf);
}

std::string Calculator::operatorStr(Operator op) {
    switch (op) {
        case Operator::Add:      return "+";
        case Operator::Subtract: return "−"; // − (Unicode MINUS SIGN, not hyphen)
        case Operator::Multiply: return "×"; // ×
        case Operator::Divide:   return "÷"; // ÷
        case Operator::None:     return "";
    }
    return "";
}

void Calculator::finishOperation(double second) {
    // Guard: division by zero
    if (pendingOp_ == Operator::Divide && second == 0.0) {
        setError("Cannot divide by zero");
        return;
    }

    double result = 0.0;
    switch (pendingOp_) {
        case Operator::Add:      result = firstOperand_ + second; break;
        case Operator::Subtract: result = firstOperand_ - second; break;
        case Operator::Multiply: result = firstOperand_ * second; break;
        case Operator::Divide:   result = firstOperand_ / second; break;
        case Operator::None:     result = second;                 break;
    }

    // Guard: overflow (IEEE infinity from e.g. 1e308 * 10)
    if (std::isinf(result)) {
        setError("Overflow");
        return;
    }

    // Record in history
    if (pendingOp_ != Operator::None) {
        history_.push_back(
            formatNumber(firstOperand_) + " " + operatorStr(pendingOp_) +
            " " + formatNumber(second) + " = " + formatNumber(result)
        );
    }

    // Update state: result becomes the new display value
    currentInput_      = formatNumber(result);
    firstOperand_      = result;    // ready if user chains another operator
    state_             = CalcState::EnteringFirst;
    pendingOp_         = Operator::None;
    justPressedEquals_ = true;
}

void Calculator::setError(const std::string& msg) {
    errorMessage_ = msg;
    state_        = CalcState::Error;
}
