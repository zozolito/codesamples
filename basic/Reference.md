# BASIC — Reference Manual

> A practical reference covering classic BASIC, QBasic/QuickBASIC, QB64/QB64-PE, and FreeBASIC.  
> All examples run in **QB64-PE** unless otherwise noted.

---

## 🇬🇧 English

### Table of Contents

1. [Dialects — Know Which BASIC You Are Using](#1-dialects--know-which-basic-you-are-using)
2. [Source Format and Program Structure](#2-source-format-and-program-structure)
3. [Variables and Data Types](#3-variables-and-data-types)
   - [Type sigils](#31-type-sigils)
   - [DIM — explicit declaration](#32-dim--explicit-declaration)
   - [OPTION _EXPLICIT](#33-option-_explicit)
   - [Constants](#34-constants)
4. [Operators](#4-operators)
5. [String Functions](#5-string-functions)
6. [Control Flow](#6-control-flow)
   - [IF / THEN / ELSE](#61-if--then--else)
   - [SELECT CASE](#62-select-case)
   - [FOR / NEXT](#63-for--next)
   - [WHILE / WEND](#64-while--wend)
   - [DO / LOOP](#65-do--loop)
   - [GOTO and GOSUB (legacy)](#66-goto-and-gosub-legacy)
   - [EXIT](#67-exit)
7. [Arrays](#7-arrays)
8. [Subroutines and Functions](#8-subroutines-and-functions)
   - [SUB](#81-sub)
   - [FUNCTION](#82-function)
   - [Passing by reference vs value](#83-passing-by-reference-vs-value)
9. [Input / Output](#9-input--output)
   - [PRINT and PRINT USING](#91-print-and-print-using)
   - [INPUT and LINE INPUT](#92-input-and-line-input)
   - [File I/O](#93-file-io)
10. [Math Functions](#10-math-functions)
11. [Type Conversion](#11-type-conversion)
12. [Error Handling](#12-error-handling)
13. [Graphics (QB64)](#13-graphics-qb64)
14. [QB64-PE Specific Features](#14-qb64-pe-specific-features)
15. [FreeBASIC Differences](#15-freebasic-differences)
16. [Classic BASIC Patterns](#16-classic-basic-patterns)
17. [Quick Reference Card](#17-quick-reference-card)

---

## 1. Dialects — Know Which BASIC You Are Using

BASIC is a family of languages, not one language. The dialect matters.

| Dialect | Year | Notes |
|---|---|---|
| **Dartmouth BASIC** | 1964 | The original. Line numbers, `GOTO`, very limited. |
| **GW-BASIC / BASICA** | 1983 | Microsoft's early BASIC for DOS. Line numbers required. |
| **QBasic** | 1991 | Included free with MS-DOS 5+. No line numbers, SUBs, structured. |
| **QuickBASIC 4.5** | 1988 | The professional version of QBasic. Compiled. |
| **QB64** | 2007 | Open-source QuickBASIC-compatible compiler for modern systems. |
| **QB64-PE** | 2022 | Active fork of QB64. **Use this one.** |
| **FreeBASIC** | 2004 | C-like BASIC, compiles to native, GDB-compatible. |
| **VBA** | 1993 | BASIC embedded in Microsoft Office. Different enough to be its own thing. |
| **Visual Basic .NET** | 2002 | Modern, object-oriented, part of the .NET ecosystem. |

> This reference focuses on **QBasic / QB64 / QB64-PE** syntax — the most common dialect for learning. FreeBASIC differences are noted in section 15.

---

## 2. Source Format and Program Structure

### Modern BASIC (QB64 / QBasic) — no line numbers needed

```basic
' This is a comment — apostrophe starts a comment anywhere on the line
REM This is also a comment — REM (REMark) is the old way

OPTION _EXPLICIT   ' Force variable declarations — always use this

' Declarations come first
DIM name AS STRING
DIM count AS INTEGER

' Executable code
name = "World"
PRINT "Hello, "; name; "!"

END   ' Optional in QB64 but makes intent clear

' Subroutines and functions after END (or at the bottom)
SUB MyRoutine
    PRINT "From subroutine"
END SUB
```

### Classic BASIC (GW-BASIC / line number style)

```basic
10 REM Classic BASIC — line numbers required
20 LET N$ = "World"
30 PRINT "Hello, "; N$; "!"
40 GOSUB 100
50 END
100 REM Subroutine
110 PRINT "From subroutine"
120 RETURN
```

> Line numbers are not required in QBasic and QB64. They are supported for backward compatibility and as `GOTO`/`GOSUB` targets, but you should not use them in new code.

### Multiple statements per line

```basic
x = 1 : y = 2 : z = 3   ' Colon separates statements on the same line
```

### Line continuation

```basic
result = firstValue + _
         secondValue + _
         thirdValue       ' Underscore _ continues to next line (QB64)
```

---

## 3. Variables and Data Types

### 3.1 Type sigils

Classic BASIC uses suffix characters to denote types. QB64 supports both sigils and explicit `AS TYPE` declarations.

| Sigil | Type | Size | Example |
|---|---|---|---|
| `$` | String | Variable | `name$` |
| `%` | Integer | 16-bit | `count%` |
| `&` | Long | 32-bit | `total&` |
| `!` | Single | 32-bit float | `ratio!` |
| `#` | Double | 64-bit float | `precise#` |
| `&&` | Integer64 | 64-bit (QB64) | `bignum&&` |
| none | Single | Default | `value` |

```basic
DIM name$    AS STRING    ' or just: name$ = "Alice"
DIM count%   AS INTEGER
DIM total&   AS LONG
DIM ratio!   AS SINGLE
DIM precise# AS DOUBLE

' Shorthand — sigil creates the variable implicitly (if OPTION _EXPLICIT is off)
name$ = "Alice"
count% = 42
```

### 3.2 DIM — explicit declaration

```basic
DIM x AS INTEGER
DIM y AS DOUBLE
DIM greeting AS STRING

' DIM with initial value
DIM counter AS INTEGER: counter = 0

' Multiple variables
DIM a AS INTEGER, b AS INTEGER, c AS DOUBLE

' String of fixed length
DIM code AS STRING * 5   ' exactly 5 characters, padded with spaces

' QB64 extended types
DIM big   AS INTEGER64   ' 64-bit signed integer
DIM ubig  AS _UNSIGNED LONG  ' unsigned 32-bit
DIM flag  AS _BIT        ' 1-bit value
```

### 3.3 OPTION _EXPLICIT

```basic
OPTION _EXPLICIT
```

**Always use this.** Without it, BASIC creates variables on first use with no warning — a typo creates a new variable silently, which is a classic bug source.

```basic
OPTION _EXPLICIT

DIM total AS INTEGER
total = 100
totla = 50   ' ERROR — "totla" not declared. Catches the typo.
```

### 3.4 Constants

```basic
CONST PI = 3.14159265358979
CONST MAX_SIZE = 1000
CONST APP_NAME = "My Program"
CONST DEBUG_MODE = 0   ' 0 = false, -1 = true (BASIC's true is -1, not 1)

' BASIC TRUE and FALSE
PRINT (1 = 1)   ' prints -1 (true)
PRINT (1 = 2)   ' prints  0 (false)
```

> In BASIC, boolean true is **-1** (all bits set), not 1. This surprises everyone. `IF x THEN` is true when x is any non-zero value.

---

## 4. Operators

### Arithmetic

| Operator | Meaning | Example |
|---|---|---|
| `+` | Addition | `a + b` |
| `-` | Subtraction | `a - b` |
| `*` | Multiplication | `a * b` |
| `/` | Division (float) | `a / b` |
| `\` | Integer division | `7 \ 2` → `3` |
| `MOD` | Modulo (remainder) | `7 MOD 2` → `1` |
| `^` | Exponentiation | `2 ^ 10` → `1024` |
| `-` | Unary minus | `-x` |

### Comparison

| Operator | Meaning |
|---|---|
| `=` | Equal |
| `<>` | Not equal |
| `<` | Less than |
| `<=` | Less than or equal |
| `>` | Greater than |
| `>=` | Greater than or equal |

### Logical

| Operator | Meaning |
|---|---|
| `AND` | Bitwise AND (also logical AND) |
| `OR` | Bitwise OR (also logical OR) |
| `NOT` | Bitwise NOT (also logical NOT) |
| `XOR` | Bitwise XOR |
| `EQV` | Logical equivalence |
| `IMP` | Logical implication |

```basic
' Logical usage
IF x > 0 AND x < 100 THEN PRINT "In range"
IF done OR error THEN GOTO cleanup

' Bitwise usage
DIM flags AS INTEGER
flags = flags OR 4     ' set bit 2
flags = flags AND (NOT 4)  ' clear bit 2
IF (flags AND 4) THEN PRINT "Bit 2 is set"
```

### String operator

```basic
full$ = first$ + last$     ' + concatenates strings
full$ = first$ + " " + last$
```

---

## 5. String Functions

```basic
DIM s AS STRING
s = "  Hello, World!  "

' Length
PRINT LEN(s)              ' 18

' Case
PRINT UCASE$(s)           ' "  HELLO, WORLD!  "
PRINT LCASE$(s)           ' "  hello, world!  "

' Trim
PRINT LTRIM$(s)           ' "Hello, World!  "  (removes leading spaces)
PRINT RTRIM$(s)           ' "  Hello, World!"  (removes trailing spaces)
PRINT TRIM$(s)            ' "Hello, World!"    (QB64 — removes both)

' Substrings
PRINT LEFT$(s, 7)         ' "  Hello"
PRINT RIGHT$(s, 8)        ' "World!  "
PRINT MID$(s, 3, 5)       ' "Hello"   (start pos, length)

' Search
PRINT INSTR(s, "World")   ' 10 (1-based position, 0 if not found)
PRINT INSTR(5, s, "o")    ' search starting at position 5

' Replace (QB64)
PRINT REPLACE$(s, "World", "BASIC")  ' "  Hello, BASIC!  "

' Build and fill
PRINT STRING$(5, "*")     ' "*****"
PRINT STRING$(5, 42)      ' "****" (ASCII code 42 = *)
PRINT SPACE$(10)          ' "          " (10 spaces)

' Character / ASCII
PRINT CHR$(65)            ' "A"
PRINT ASC("A")            ' 65

' Convert number to string
PRINT STR$(42)            ' " 42" (note the leading space for positive numbers)
PRINT STR$(-42)           ' "-42"
x$ = LTRIM$(STR$(42))    ' "42" without the leading space

' String comparison
IF s1$ = s2$ THEN PRINT "Equal"
IF s1$ < s2$ THEN PRINT "s1 comes first alphabetically"
```

---

## 6. Control Flow

### 6.1 IF / THEN / ELSE

```basic
' Single-line IF (no END IF needed)
IF x > 0 THEN PRINT "Positive"
IF x > 0 THEN PRINT "Positive" ELSE PRINT "Non-positive"

' Block IF — requires END IF
IF x > 0 THEN
    PRINT "Positive"
ELSEIF x < 0 THEN
    PRINT "Negative"
ELSE
    PRINT "Zero"
END IF

' Nested
IF score >= 90 THEN
    grade$ = "A"
ELSEIF score >= 80 THEN
    grade$ = "B"
ELSEIF score >= 70 THEN
    grade$ = "C"
ELSEIF score >= 60 THEN
    grade$ = "D"
ELSE
    grade$ = "F"
END IF
```

### 6.2 SELECT CASE

```basic
SELECT CASE score
    CASE 90 TO 100
        PRINT "A — Excellent"
    CASE 80 TO 89
        PRINT "B — Good"
    CASE 70, 71, 72, 73, 74, 75, 76, 77, 78, 79
        PRINT "C — Average"
    CASE IS < 70
        PRINT "F — Failing"
    CASE ELSE
        PRINT "Invalid score"
END SELECT

' Works with strings
SELECT CASE UCASE$(command$)
    CASE "QUIT", "EXIT", "Q"
        running = 0
    CASE "HELP", "H", "?"
        CALL ShowHelp
    CASE ELSE
        PRINT "Unknown command: "; command$
END SELECT
```

### 6.3 FOR / NEXT

```basic
' Basic loop
FOR i = 1 TO 10
    PRINT i
NEXT i

' With STEP
FOR i = 0 TO 100 STEP 5
    PRINT i       ' 0, 5, 10, ..., 100
NEXT i

' Count down — STEP must be negative
FOR i = 10 TO 1 STEP -1
    PRINT i       ' 10, 9, 8, ..., 1
NEXT i

' Float step
FOR x = 0.0 TO 1.0 STEP 0.1
    PRINT x
NEXT x

' Nested
FOR row = 1 TO 3
    FOR col = 1 TO 3
        PRINT row; col,
    NEXT col
    PRINT   ' newline
NEXT row

' EXIT FOR — break out of loop early
FOR i = 1 TO 100
    IF i * i > 500 THEN EXIT FOR
NEXT i
PRINT "Stopped at i ="; i
```

### 6.4 WHILE / WEND

```basic
' Classic — from GW-BASIC era
WHILE condition
    ' loop body
WEND

' Example
count = 1
WHILE count <= 10
    PRINT count
    count = count + 1
WEND
```

### 6.5 DO / LOOP

More powerful and flexible than WHILE/WEND. Prefer this in modern BASIC.

```basic
' DO WHILE — test at top (may not execute at all)
DO WHILE x > 0
    x = x / 2
LOOP

' DO UNTIL — test at top
DO UNTIL done = -1
    CALL ProcessItem
LOOP

' DO ... LOOP WHILE — test at bottom (executes at least once)
DO
    INPUT "Enter positive number: ", n
LOOP WHILE n <= 0

' DO ... LOOP UNTIL — test at bottom
DO
    INPUT "Command (Q=quit): ", cmd$
    cmd$ = UCASE$(cmd$)
    IF cmd$ <> "Q" THEN CALL Execute(cmd$)
LOOP UNTIL cmd$ = "Q"

' Infinite loop with EXIT DO
DO
    INPUT "> ", input$
    IF input$ = "quit" THEN EXIT DO
    CALL Process(input$)
LOOP
```

### 6.6 GOTO and GOSUB (legacy)

```basic
' GOTO — unconditional jump to a line label or number
' Avoid in modern code. Use SUB/FUNCTION and structured loops.

IF error THEN GOTO ErrorHandler

MainCode:
    PRINT "Running..."
    GOTO Done

ErrorHandler:
    PRINT "An error occurred"

Done:
    PRINT "Finished"
    END

' GOSUB — jump to subroutine, return with RETURN
' Use SUB instead in modern code.

GOSUB DrawHeader
PRINT "Content"
GOSUB DrawFooter
END

DrawHeader:
    PRINT "=== HEADER ==="
    RETURN

DrawFooter:
    PRINT "=== FOOTER ==="
    RETURN
```

### 6.7 EXIT

```basic
EXIT FOR       ' exit a FOR loop
EXIT WHILE     ' exit a WHILE loop (FreeBASIC)
EXIT DO        ' exit a DO loop
EXIT SUB       ' exit a SUB early (like return in C)
EXIT FUNCTION  ' exit a FUNCTION early
```

---

## 7. Arrays

```basic
' Fixed-size array (DIM)
' Indices run from 0 to n by default
DIM scores(10) AS INTEGER      ' 11 elements: 0 to 10

' Specify explicit bounds
DIM matrix(1 TO 5, 1 TO 5) AS DOUBLE   ' 5x5, indices 1..5
DIM grid(-5 TO 5) AS INTEGER           ' 11 elements, -5 to 5

' Change default lower bound to 1
OPTION BASE 1
DIM values(10) AS INTEGER   ' now indices 1 to 10

' Initialize
DIM primes(1 TO 5) AS INTEGER
primes(1) = 2
primes(2) = 3
primes(3) = 5
primes(4) = 7
primes(5) = 11

' Loop through
FOR i = 1 TO 5
    PRINT primes(i)
NEXT i

' Dynamic arrays (REDIM)
DIM SHARED dynArray() AS INTEGER   ' declare without size
REDIM dynArray(n)                  ' set size at runtime
REDIM PRESERVE dynArray(n * 2)     ' resize, keep existing data

' ERASE — free array memory or reset static array
ERASE dynArray

' Useful array operations
DIM a(1 TO 5) AS INTEGER
a(1) = 10: a(2) = 30: a(3) = 20: a(4) = 50: a(5) = 40

' BASIC has no built-in sort — you write it (or use a library)
' Simple bubble sort example:
DIM i AS INTEGER, j AS INTEGER, temp AS INTEGER
FOR i = 1 TO 4
    FOR j = 1 TO 5 - i
        IF a(j) > a(j + 1) THEN
            temp = a(j)
            a(j) = a(j + 1)
            a(j + 1) = temp
        END IF
    NEXT j
NEXT i

' LBOUND and UBOUND — get array bounds
PRINT LBOUND(a)   ' 1
PRINT UBOUND(a)   ' 5
```

---

## 8. Subroutines and Functions

### 8.1 SUB

A `SUB` performs an action. It does not return a value.

```basic
' Definition — placed after END or at bottom of file
SUB Greet (name AS STRING, times AS INTEGER)
    DIM i AS INTEGER
    FOR i = 1 TO times
        PRINT i; ". Hello, "; name; "!"
    NEXT i
END SUB

' Call with CALL (or without — both work)
CALL Greet("Alice", 3)
Greet "Alice", 3       ' CALL is optional in QB64

' SUB with no parameters
SUB PrintSeparator
    PRINT STRING$(40, "-")
END SUB

' Early exit
SUB ProcessValue (x AS INTEGER)
    IF x < 0 THEN
        PRINT "Skipping negative value"
        EXIT SUB
    END IF
    PRINT "Processing:"; x
END SUB
```

### 8.2 FUNCTION

A `FUNCTION` returns a value. Assign the result to the function's name.

```basic
' Definition
FUNCTION Add (a AS INTEGER, b AS INTEGER) AS INTEGER
    Add = a + b    ' assign to function name to return a value
END FUNCTION

FUNCTION Factorial (n AS INTEGER) AS LONG
    IF n <= 1 THEN
        Factorial = 1
    ELSE
        Factorial = n * Factorial(n - 1)   ' recursive
    END IF
END FUNCTION

FUNCTION IsPrime (n AS INTEGER) AS INTEGER
    DIM i AS INTEGER
    IF n < 2 THEN IsPrime = 0: EXIT FUNCTION
    FOR i = 2 TO INT(SQR(n))
        IF n MOD i = 0 THEN IsPrime = 0: EXIT FUNCTION
    NEXT i
    IsPrime = -1   ' -1 = true in BASIC
END FUNCTION

' Usage
PRINT Add(3, 4)               ' 7
PRINT Factorial(10)            ' 3628800
IF IsPrime(17) THEN PRINT "Prime"

' String function
FUNCTION Repeat$ (s AS STRING, times AS INTEGER)
    DIM result AS STRING, i AS INTEGER
    result = ""
    FOR i = 1 TO times
        result = result + s
    NEXT i
    Repeat$ = result           ' note the $ suffix on the function name
END FUNCTION

PRINT Repeat$("ab", 3)        ' "ababab"
```

### 8.3 Passing by reference vs value

```basic
' By default, BASIC passes by REFERENCE — the SUB can modify the caller's variable

SUB DoubleIt (x AS INTEGER)      ' x is a reference
    x = x * 2
END SUB

DIM n AS INTEGER: n = 5
DoubleIt n
PRINT n   ' 10 — n was modified!

' Pass by VALUE with BYVAL — SUB gets a copy, cannot modify original
SUB SafeDouble (BYVAL x AS INTEGER)
    x = x * 2     ' only modifies the local copy
    PRINT "Inside:"; x
END SUB

n = 5
SafeDouble n
PRINT n   ' still 5 — original not modified

' SHARED — access global variables inside a SUB without passing them
DIM SHARED globalCount AS INTEGER

SUB IncrementGlobal
    globalCount = globalCount + 1
END SUB
```

---

## 9. Input / Output

### 9.1 PRINT and PRINT USING

```basic
' Basic PRINT
PRINT "Hello"                       ' prints then newline
PRINT "A"; "B"; "C"                 ' "ABC" — semicolon = no space between
PRINT "A", "B", "C"                 ' tab-separated columns
PRINT                               ' blank line
PRINT "Value ="; 42                 ' "Value = 42"

' PRINT USING — formatted output
' # = digit placeholder
' . = decimal point
' , = thousands separator
' $ = dollar sign
' + = always show sign
' ^ = scientific notation (^^^^)
' & = string field (variable length)
' \ n spaces \ = fixed string field (n+2 chars wide)

PRINT USING "###.##"; 3.14159       ' "  3.14"
PRINT USING "$$$,$$$.##"; 12345.6  ' " $12,345.60"
PRINT USING "+###"; 42             ' " +42"
PRINT USING "+###"; -42            ' "  -42"
PRINT USING "####.##^^^^"; 12345.6 ' "1.23E+04"
PRINT USING "Name: \      \"; "Alice"  ' "Name: Alice     " (fixed field)
PRINT USING "Name: &"; "Alice"          ' "Name: Alice" (variable field)

' WRITE — like PRINT but adds quotes around strings and commas between items
WRITE "Alice", 30, "Paris"   ' "Alice",30,"Paris" — useful for CSV

' TAB and SPC
PRINT TAB(20); "Column 20"   ' move to column 20
PRINT SPC(10); "After 10 spaces"
```

### 9.2 INPUT and LINE INPUT

```basic
' INPUT — prompts and reads, stops at comma or Enter
INPUT "Enter your age: ", age
INPUT age    ' no prompt — cursor waits

' Warning: INPUT splits on commas
INPUT "Enter first and last name: ", fullName$
' if user types "Smith, Alice" — fullName$ = "Smith" only!
' Solution: use LINE INPUT

' LINE INPUT — reads the entire line including commas and spaces
LINE INPUT "Enter your full name: ", fullName$
LINE INPUT fullName$   ' no prompt

' INKEY$ — read a single keypress without waiting
DO
    k$ = INKEY$
    IF k$ <> "" THEN EXIT DO
LOOP

' _KEYHIT — QB64 — returns key code, 0 if no key pressed
DO
    k = _KEYHIT
    IF k <> 0 THEN EXIT DO
LOOP
```

### 9.3 File I/O

```basic
' Three file modes:
' SEQUENTIAL  — read or write text, record by record
' RANDOM      — fixed-length records, direct access by number
' BINARY      — raw bytes, complete control

' ── Sequential write ────────────────────────────────────────
OPEN "data.txt" FOR OUTPUT AS #1   ' creates or overwrites
PRINT #1, "Alice", 30, "Paris"
PRINT #1, "Bob", 25, "Lyon"
CLOSE #1

' ── Sequential read ─────────────────────────────────────────
DIM name AS STRING, age AS INTEGER, city AS STRING

OPEN "data.txt" FOR INPUT AS #1
DO WHILE NOT EOF(1)
    INPUT #1, name, age, city
    PRINT name; " ("; age; ") — "; city
LOOP
CLOSE #1

' ── Append ──────────────────────────────────────────────────
OPEN "data.txt" FOR APPEND AS #1
PRINT #1, "Carol", 35, "Marseille"
CLOSE #1

' ── Random access — fixed-length records ────────────────────
TYPE EmployeeRecord
    empName AS STRING * 20
    salary  AS DOUBLE
    active  AS INTEGER
END TYPE

DIM emp AS EmployeeRecord
DIM recLen AS INTEGER: recLen = LEN(emp)

OPEN "employees.dat" FOR RANDOM AS #2 LEN = recLen

' Write record 1
emp.empName = "Alice"
emp.salary  = 55000.00
emp.active  = -1   ' true
PUT #2, 1, emp     ' record number 1

' Read record 1
GET #2, 1, emp
PRINT emp.empName; emp.salary

CLOSE #2

' ── Binary ──────────────────────────────────────────────────
DIM b AS INTEGER
OPEN "raw.dat" FOR BINARY AS #3
PUT #3, , 255         ' write one byte (255)
PUT #3, , 128
SEEK #3, 1            ' go back to position 1
GET #3, , b           ' read the byte
PRINT b               ' 255
CLOSE #3

' ── Utility functions ───────────────────────────────────────
PRINT LOF(1)          ' Length Of File (bytes)
PRINT LOC(1)          ' current position
PRINT EOF(1)          ' -1 if at end of file, 0 otherwise
KILL "tempfile.txt"   ' delete a file
NAME "old.txt" AS "new.txt"  ' rename
```

---

## 10. Math Functions

```basic
' Trigonometric (all arguments in radians)
SIN(x)    COS(x)    TAN(x)
ATN(x)    ' arctangent — note: no ASIN or ACOS in classic BASIC

' Derived trig (using identities)
' ASIN(x) = ATN(x / SQR(1 - x^2))
' ACOS(x) = ATN(SQR(1 - x^2) / x)  [x > 0]
' ATN2(y, x) not built-in — use QB64 _ATAN2(y, x) or:
'   ATN(y/x) + PI * SGN(y) * (1 - SGN(x)) / 2  [approximate]

' Powers and roots
SQR(x)    ' square root
x ^ n     ' x to the power n

' Rounding
INT(x)    ' floor — largest integer <= x  (INT(-2.3) = -3)
FIX(x)    ' truncate toward zero          (FIX(-2.3) = -2)
CINT(x)   ' round to nearest integer      (CINT(2.5) = 3)
CLNG(x)   ' round to nearest long

' Absolute value / sign
ABS(x)    ' absolute value
SGN(x)    ' -1, 0, or 1 depending on sign

' Logarithm / exponential
LOG(x)    ' natural logarithm (base e)
EXP(x)    ' e ^ x
' Log base 10: LOG(x) / LOG(10)
' Log base n:  LOG(x) / LOG(n)

' Random numbers
RANDOMIZE TIMER           ' seed with current time — call once at start
x = RND                   ' 0.0 <= x < 1.0
n = INT(RND * 6) + 1      ' simulate a die roll: 1..6
n = INT(RND * (max - min + 1)) + min  ' integer in [min, max]

' Min / max (no built-in — use IIF or IF)
bigger  = IIF(a > b, a, b)  ' QB64 _IIF equivalent
smaller = IIF(a < b, a, b)
```

---

## 11. Type Conversion

```basic
' Numeric conversions
CInt(x)    ' to INTEGER (rounds)
CLng(x)    ' to LONG (rounds)
CSng(x)    ' to SINGLE
CDbl(x)    ' to DOUBLE
CStr(x)    ' to STRING (VBA / FreeBASIC)

' String ↔ number
STR$(42)          ' " 42"  (note leading space)
LTRIM$(STR$(42))  ' "42"   (remove the space)
VAL("42")         ' 42     (string to number)
VAL("3.14abc")    ' 3.14   (stops at first non-numeric character)

' Character ↔ ASCII code
CHR$(65)          ' "A"
CHR$(13)          ' carriage return
CHR$(10)          ' line feed
ASC("A")          ' 65
ASC("Hello")      ' 72 (ASCII of first character)

' HEX and OCT
HEX$(255)         ' "FF"
OCT$(255)         ' "377"
&HFF              ' 255 — hexadecimal literal
&O377             ' 255 — octal literal
&B11111111        ' 255 — binary literal (QB64)
```

---

## 12. Error Handling

```basic
' ON ERROR GOTO — catch all errors
ON ERROR GOTO ErrorHandler

DIM f AS INTEGER
OPEN "missing.txt" FOR INPUT AS #1   ' will trigger error if file not found
CLOSE #1
PRINT "File opened successfully"
END

ErrorHandler:
    PRINT "Error #"; ERR; " at line"; ERL
    PRINT "Message: "; ERROR$(ERR)
    RESUME NEXT     ' continue after the line that caused the error
    ' RESUME         ' retry the line that caused the error
    ' RESUME label   ' jump to a specific label

' Disable error trapping
ON ERROR GOTO 0

' Common error codes
' 2  — Syntax error
' 4  — Out of DATA
' 5  — Illegal function call
' 6  — Overflow
' 7  — Out of memory
' 11 — Division by zero
' 13 — Type mismatch
' 53 — File not found
' 57 — Device I/O error
' 62 — Input past end of file
' 64 — Bad file name
' 75 — Path/File access error

' ERR  — number of last error
' ERL  — line number of last error (only with line numbers)
' ERDEV — device error code (hardware errors)
```

---

## 13. Graphics (QB64)

BASIC's graphics capabilities were a huge part of its appeal for home users in the 80s and 90s. QB64 preserves and extends them.

```basic
' SCREEN mode — sets graphics mode
SCREEN 12    ' 640x480, 16 colors
SCREEN 13    ' 320x200, 256 colors
SCREEN 0     ' text mode (default)

' QB64 extended: _NEWIMAGE for custom resolutions
DIM myScreen AS LONG
myScreen = _NEWIMAGE(800, 600, 32)   ' 800x600, 32-bit color
SCREEN myScreen

' Colors
COLOR 14, 1         ' foreground: yellow (14), background: blue (1)
COLOR _RGB(255, 128, 0)   ' QB64 — orange via RGB

' Drawing primitives
PSET (100, 100)              ' plot a pixel
PSET (100, 100), 12          ' pixel with color 12 (red)
LINE (0, 0)-(100, 100)       ' line from (0,0) to (100,100)
LINE (50, 50)-(150, 150), 10 ' green line
LINE (50, 50)-(150, 150), 9, B   ' blue rectangle (B = box)
LINE (50, 50)-(150, 150), 9, BF  ' filled blue rectangle

CIRCLE (160, 100), 50          ' circle at (160,100), radius 50
CIRCLE (160, 100), 50, 12      ' red circle
CIRCLE (160, 100), 50, 12, 0, 2*3.14159  ' arc
CIRCLE (160, 100), 50, 12, , , 0.5       ' ellipse (aspect ratio)

PAINT (100, 100), 10           ' flood fill at (100,100) with color 10
PAINT (100, 100), 10, 15       ' fill stopping at color 15 border

' Text on graphics screen
LOCATE 5, 10           ' row 5, column 10 (text coordinates)
PRINT "Hello!"

_PRINTSTRING (200, 150), "Hello at pixel coords"  ' QB64

' Keyboard detection in graphics loop
DO
    IF INKEY$ = CHR$(27) THEN EXIT DO   ' ESC to quit
    ' ... draw stuff
    _DISPLAY    ' QB64: manually swap display buffer for smooth animation
LOOP

' Animation example (QB64)
DIM x AS INTEGER: x = 0
DO
    CLS
    CIRCLE (x, 240), 20, 12    ' red ball
    x = (x + 2) MOD 640        ' move right, wrap around
    _DISPLAY
    _LIMIT 60                   ' 60 FPS cap
LOOP UNTIL INKEY$ = CHR$(27)
```

---

## 14. QB64-PE Specific Features

```basic
' _EXPLICIT — same as OPTION _EXPLICIT
OPTION _EXPLICIT

' Extended types
DIM i64  AS INTEGER64          ' 64-bit integer
DIM u32  AS _UNSIGNED LONG     ' unsigned 32-bit
DIM u64  AS _UNSIGNED INTEGER64

' _NEWIMAGE — custom screen
DIM handle AS LONG
handle = _NEWIMAGE(1280, 720, 32)
SCREEN handle

' _RGB and _RGBA — true color
PSET (100, 100), _RGB(255, 0, 128)
PSET (200, 200), _RGBA(255, 0, 0, 128)   ' 50% transparent red

' _FONT — custom fonts
DIM myFont AS LONG
myFont = _LOADFONT("arial.ttf", 24, "BOLD")
_FONT myFont
PRINT "Large bold text"

' Sound
BEEP              ' system beep
PLAY "MBT180 L4 CDEFGAB5C"   ' music string — classic BASIC!
SOUND 440, 18     ' frequency (Hz), duration (ticks at 18.2/sec)

' Timer
PRINT TIMER       ' seconds since midnight (float)
t = TIMER
' ... do work ...
PRINT "Elapsed:"; TIMER - t; "seconds"

' _LIMIT — frame rate limiter in game loops
DO
    CLS
    ' draw frame
    _DISPLAY
    _LIMIT 60   ' max 60 iterations per second
LOOP

' _CLIPBOARD — read/write system clipboard
_CLIPBOARD$ = "Hello from BASIC!"
PRINT _CLIPBOARD$

' Shell commands
SHELL "ls -la"              ' Linux/macOS
SHELL "dir"                 ' Windows

' _ENVIRONM$ — environment variable
PRINT _ENVIRON$("PATH")

' Type alias
TYPE Vector2D
    x AS DOUBLE
    y AS DOUBLE
END TYPE

DIM v AS Vector2D
v.x = 3.0
v.y = 4.0
PRINT SQR(v.x^2 + v.y^2)   ' 5.0
```

---

## 15. FreeBASIC Differences

FreeBASIC is largely compatible with QBasic/QB64 but has important extensions and differences.

```basic
' FreeBASIC: compile and run
' fbc -x output source.bas
' fbc -g source.bas  (with debug symbols for GDB)

' Explicit lang mode for QBasic compatibility
' fbc -lang qb source.bas

' C-style comments (FreeBASIC extension)
/* This is a C-style comment */
// This too

' Pointers (not available in QB64)
DIM x AS INTEGER = 42
DIM ptr AS INTEGER PTR
ptr = @x      ' @ takes address of x
PRINT *ptr    ' 42 — dereference
*ptr = 100
PRINT x       ' 100

' Inline assembly (FreeBASIC)
ASM
    mov eax, 1
    add eax, 2
END ASM

' Namespaces
NAMESPACE MathUtils
    FUNCTION Square(x AS DOUBLE) AS DOUBLE
        RETURN x * x
    END FUNCTION
END NAMESPACE

PRINT MathUtils.Square(5)   ' 25

' RETURN statement (FreeBASIC — cleaner than assigning to function name)
FUNCTION Add(a AS INTEGER, b AS INTEGER) AS INTEGER
    RETURN a + b    ' FreeBASIC style
END FUNCTION

' FOR ... TO ... STEP with NEXT without variable
FOR i = 1 TO 10
    PRINT i
NEXT   ' variable name after NEXT is optional in FreeBASIC

' SCOPE block
SCOPE
    DIM localVar AS INTEGER = 99
    PRINT localVar
END SCOPE
' localVar is not accessible here

' OpenGL / graphics via libraries (FreeBASIC strength)
' #include "GL/gl.bi"   etc.
```

---

## 16. Classic BASIC Patterns

These patterns appear constantly in BASIC codebases — legacy and modern.

### Read program data

```basic
' DATA statements store constants read by READ
READ name$, score
PRINT name$; score

READ name$, score
PRINT name$; score

RESTORE   ' reset DATA pointer to beginning

DATA "Alice", 95
DATA "Bob", 82
```

### Menu loop

```basic
DIM choice AS INTEGER

DO
    PRINT ""
    PRINT "=== MAIN MENU ==="
    PRINT "1. New game"
    PRINT "2. Load game"
    PRINT "3. Options"
    PRINT "4. Quit"
    PRINT ""
    INPUT "Choice: ", choice

    SELECT CASE choice
        CASE 1: CALL NewGame
        CASE 2: CALL LoadGame
        CASE 3: CALL Options
        CASE 4: PRINT "Goodbye!": EXIT DO
        CASE ELSE: PRINT "Invalid choice. Try again."
    END SELECT
LOOP
```

### Simple state machine

```basic
CONST STATE_IDLE = 0
CONST STATE_RUNNING = 1
CONST STATE_PAUSED = 2
CONST STATE_DONE = 3

DIM state AS INTEGER: state = STATE_IDLE
DIM cmd$ AS STRING

DO
    SELECT CASE state
        CASE STATE_IDLE
            PRINT "Press S to start, Q to quit"
            LINE INPUT cmd$
            SELECT CASE UCASE$(cmd$)
                CASE "S": state = STATE_RUNNING
                CASE "Q": state = STATE_DONE
            END SELECT

        CASE STATE_RUNNING
            PRINT "Running... Press P to pause, Q to quit"
            LINE INPUT cmd$
            SELECT CASE UCASE$(cmd$)
                CASE "P": state = STATE_PAUSED
                CASE "Q": state = STATE_DONE
            END SELECT

        CASE STATE_PAUSED
            PRINT "Paused. Press R to resume."
            LINE INPUT cmd$
            IF UCASE$(cmd$) = "R" THEN state = STATE_RUNNING

        CASE STATE_DONE
            PRINT "Done."
            EXIT DO
    END SELECT
LOOP
```

### USER-DEFINED TYPE (record/struct)

```basic
TYPE Point
    x AS DOUBLE
    y AS DOUBLE
END TYPE

TYPE Circle
    center AS Point    ' nested type
    radius AS DOUBLE
END TYPE

DIM c AS Circle
c.center.x = 100.0
c.center.y = 150.0
c.radius = 50.0

PRINT "Area ="; 3.14159 * c.radius ^ 2

' Array of records
DIM points(1 TO 10) AS Point
points(1).x = 1.0
points(1).y = 2.0
```

### String parsing

```basic
FUNCTION CountWords (s AS STRING) AS INTEGER
    DIM count AS INTEGER: count = 0
    DIM inWord AS INTEGER: inWord = 0
    DIM i AS INTEGER

    FOR i = 1 TO LEN(s)
        IF MID$(s, i, 1) <> " " THEN
            IF NOT inWord THEN count = count + 1
            inWord = -1
        ELSE
            inWord = 0
        END IF
    NEXT i
    CountWords = count
END FUNCTION

PRINT CountWords("Hello World BASIC")   ' 3
```

---

## 17. Quick Reference Card

### Data types at a glance

| Declaration | Type | Range / Size |
|---|---|---|
| `AS INTEGER` or `%` | 16-bit signed | -32768 to 32767 |
| `AS LONG` or `&` | 32-bit signed | -2,147,483,648 to 2,147,483,647 |
| `AS INTEGER64` or `&&` | 64-bit signed | ±9.2×10¹⁸ |
| `AS SINGLE` or `!` | 32-bit float | ~7 significant digits |
| `AS DOUBLE` or `#` | 64-bit float | ~15 significant digits |
| `AS STRING` or `$` | Variable string | Up to ~2 billion characters |
| `AS STRING * n` | Fixed string | Exactly n characters |

### String functions quick lookup

| Function | Does |
|---|---|
| `LEN(s$)` | Length |
| `LEFT$(s$, n)` | First n characters |
| `RIGHT$(s$, n)` | Last n characters |
| `MID$(s$, p, n)` | n characters starting at position p |
| `INSTR(s$, t$)` | Position of t$ in s$, 0 if not found |
| `UCASE$(s$)` | Uppercase |
| `LCASE$(s$)` | Lowercase |
| `LTRIM$(s$)` | Remove leading spaces |
| `RTRIM$(s$)` | Remove trailing spaces |
| `TRIM$(s$)` | Remove both (QB64) |
| `STR$(n)` | Number to string |
| `VAL(s$)` | String to number |
| `CHR$(n)` | ASCII code to character |
| `ASC(s$)` | First character to ASCII code |
| `STRING$(n, c$)` | Repeat character n times |
| `SPACE$(n)` | n spaces |

### Loop patterns

```basic
FOR i = start TO stop [STEP n]  ...  NEXT [i]
WHILE condition  ...  WEND
DO [WHILE/UNTIL condition]  ...  LOOP [WHILE/UNTIL condition]
DO  ...  EXIT DO  ...  LOOP
```

### Frequently confused things

| | |
|---|---|
| `=` | Both assignment AND comparison |
| `-1` | TRUE in BASIC (all bits set) |
| `0` | FALSE in BASIC |
| `INT(-2.3)` | `-3` (floor toward -∞) |
| `FIX(-2.3)` | `-2` (truncate toward 0) |
| `\` | Integer division |
| `MOD` | Modulo (remainder) |
| `^` | Exponentiation |
| `PRINT a; b` | No separator between a and b |
| `PRINT a, b` | Tab-stop separator |
| `INPUT` | Splits on commas — use `LINE INPUT` for full lines |
| `BYVAL` | Pass by value — SUB cannot modify original |
| default | Pass by reference — SUB CAN modify original |

---

---

## 🇫🇷 Français

### Table des matières

1. [Dialectes — Sais quelle BASIC tu utilises](#1-dialectes--sais-quelle-basic-tu-utilises)
2. [Format source et structure](#2-format-source-et-structure)
3. [Variables et types de données](#3-variables-et-types-de-données)
4. [Opérateurs](#4-opérateurs-1)
5. [Fonctions de chaînes](#5-fonctions-de-chaînes)
6. [Structures de contrôle](#6-structures-de-contrôle-1)
7. [Tableaux](#7-tableaux-1)
8. [Sous-routines et fonctions](#8-sous-routines-et-fonctions)
9. [Entrées / Sorties](#9-entrées--sorties)
10. [Fonctions mathématiques](#10-fonctions-mathématiques)
11. [Conversion de types](#11-conversion-de-types)
12. [Gestion des erreurs](#12-gestion-des-erreurs)
13. [Graphismes (QB64)](#13-graphismes-qb64)
14. [Spécificités QB64-PE](#14-spécificités-qb64-pe)
15. [Différences FreeBASIC](#15-différences-freebasic)
16. [Patterns classiques BASIC](#16-patterns-classiques-basic)
17. [Carte de référence rapide](#17-carte-de-référence-rapide-1)

---

## 1. Dialectes — Sais quelle BASIC tu utilises

BASIC est une famille de langages, pas un seul langage. Le dialecte compte.

| Dialecte | Année | Notes |
|---|---|---|
| **Dartmouth BASIC** | 1964 | L'original. Numéros de ligne, `GOTO`, très limité. |
| **GW-BASIC / BASICA** | 1983 | Le BASIC Microsoft pour DOS. Numéros de ligne obligatoires. |
| **QBasic** | 1991 | Fourni avec MS-DOS 5+. Sans numéros de ligne, avec SUBs. |
| **QuickBASIC 4.5** | 1988 | La version professionnelle compilée de QBasic. |
| **QB64** | 2007 | Compilateur open-source compatible QuickBASIC. |
| **QB64-PE** | 2022 | Fork actif de QB64. **Utilise celui-là.** |
| **FreeBASIC** | 2004 | BASIC style C, compile en natif, compatible GDB. |
| **VBA** | 1993 | BASIC intégré à Microsoft Office. |

> Ce manuel se concentre sur la syntaxe **QBasic / QB64 / QB64-PE**. Les différences FreeBASIC sont indiquées en section 15.

---

## 2. Format source et structure

```basic
' Ceci est un commentaire — l'apostrophe commence un commentaire
REM Ceci aussi — REM (REMark) est l'ancienne syntaxe

OPTION _EXPLICIT   ' Force les déclarations — utilise toujours ça

DIM nom AS STRING
DIM compteur AS INTEGER

nom = "Monde"
PRINT "Bonjour, "; nom; "!"

END

SUB MaRoutine
    PRINT "Depuis la sous-routine"
END SUB
```

### BASIC classique (numéros de ligne)

```basic
10 REM BASIC classique — numéros de ligne obligatoires
20 LET N$ = "Monde"
30 PRINT "Bonjour, "; N$; "!"
50 END
```

> Les numéros de ligne ne sont **pas nécessaires** en QBasic et QB64. Ne les utilise pas dans du nouveau code.

---

## 3. Variables et types de données

### Suffixes de type (sigils)

| Suffixe | Type | Exemple |
|---|---|---|
| `$` | Chaîne | `nom$` |
| `%` | Integer 16 bits | `compteur%` |
| `&` | Long 32 bits | `total&` |
| `!` | Single 32 bits | `ratio!` |
| `#` | Double 64 bits | `precis#` |
| `&&` | Integer64 (QB64) | `grand&&` |

### DIM — déclaration explicite

```basic
DIM nom AS STRING
DIM compteur AS INTEGER
DIM total AS LONG
DIM ratio AS SINGLE
DIM precis AS DOUBLE
DIM code AS STRING * 5    ' chaîne de longueur fixe (5 caractères)
```

### OPTION _EXPLICIT

```basic
OPTION _EXPLICIT
```

**Utilise-le toujours.** Sans lui, BASIC crée des variables au premier usage sans avertissement — une faute de frappe crée silencieusement une nouvelle variable.

### Constantes

```basic
CONST PI = 3.14159265358979
CONST TAILLE_MAX = 1000
CONST NOM_APP = "Mon Programme"

' Booléens en BASIC : VRAI = -1, FAUX = 0
PRINT (1 = 1)   ' affiche -1 (vrai)
PRINT (1 = 2)   ' affiche  0 (faux)
```

> En BASIC, le booléen vrai est **-1** (tous les bits à 1), pas 1. Ça surprend tout le monde.

---

## 4. Opérateurs

### Arithmétiques

| Opérateur | Signification |
|---|---|
| `+` `-` `*` `/` | Addition, soustraction, multiplication, division |
| `\` | Division entière (`7 \ 2 = 3`) |
| `MOD` | Modulo (`7 MOD 2 = 1`) |
| `^` | Exponentiation (`2 ^ 10 = 1024`) |

### Comparaison

`=` `<>` `<` `<=` `>` `>=`

### Logiques

`AND`  `OR`  `NOT`  `XOR`  `EQV`

> `AND` et `OR` sont bitwise ET logiques à la fois. Attention avec les expressions complexes.

### Chaînes

`+` — concaténation : `"Bon" + "jour"` = `"Bonjour"`

---

## 5. Fonctions de chaînes

```basic
DIM s AS STRING
s = "  Bonjour, Monde !  "

PRINT LEN(s)             ' longueur totale
PRINT UCASE$(s)          ' majuscules
PRINT LCASE$(s)          ' minuscules
PRINT LTRIM$(s)          ' supprime les espaces en début
PRINT RTRIM$(s)          ' supprime les espaces en fin
PRINT TRIM$(s)           ' supprime les deux (QB64)

PRINT LEFT$(s, 9)        ' "  Bonjour"
PRINT RIGHT$(s, 7)       ' "Monde !"
PRINT MID$(s, 3, 7)      ' "Bonjour" (position 3, longueur 7)

PRINT INSTR(s, "Monde")  ' position (base 1), 0 si absent

PRINT STRING$(5, "*")    ' "*****"
PRINT SPACE$(10)         ' 10 espaces
PRINT CHR$(65)           ' "A"
PRINT ASC("A")           ' 65
PRINT STR$(42)           ' " 42" (espace devant les positifs)
PRINT VAL("3.14abc")     ' 3.14
```

---

## 6. Structures de contrôle

### IF / THEN / ELSE

```basic
' Ligne unique
IF x > 0 THEN PRINT "Positif"
IF x > 0 THEN PRINT "Positif" ELSE PRINT "Non-positif"

' Bloc — nécessite END IF
IF score >= 90 THEN
    note$ = "A"
ELSEIF score >= 80 THEN
    note$ = "B"
ELSEIF score >= 70 THEN
    note$ = "C"
ELSE
    note$ = "F"
END IF
```

### SELECT CASE

```basic
SELECT CASE UCASE$(commande$)
    CASE "QUITTER", "Q"
        actif = 0
    CASE "AIDE", "H", "?"
        CALL AfficherAide
    CASE ELSE
        PRINT "Commande inconnue : "; commande$
END SELECT

SELECT CASE score
    CASE 90 TO 100 : PRINT "Excellent"
    CASE 70 TO 89  : PRINT "Bien"
    CASE IS < 70   : PRINT "Insuffisant"
END SELECT
```

### FOR / NEXT

```basic
FOR i = 1 TO 10
    PRINT i
NEXT i

FOR i = 10 TO 1 STEP -1   ' décompte
    PRINT i
NEXT i

FOR i = 0 TO 100 STEP 5   ' pas de 5
    PRINT i
NEXT i

' Sortie anticipée
FOR i = 1 TO 100
    IF i * i > 500 THEN EXIT FOR
NEXT i
```

### DO / LOOP

```basic
' Test en début — peut ne pas s'exécuter
DO WHILE condition
    ' corps
LOOP

DO UNTIL termine = -1
    CALL TraiterElement
LOOP

' Test en fin — s'exécute au moins une fois
DO
    INPUT "Nombre positif : ", n
LOOP WHILE n <= 0

' Boucle infinie avec sortie
DO
    LINE INPUT "> ", saisie$
    IF saisie$ = "quitter" THEN EXIT DO
    CALL Traiter(saisie$)
LOOP
```

### GOTO et GOSUB (legacy)

```basic
' GOTO — évite dans le code moderne
IF erreur THEN GOTO GestionErreur

GestionErreur:
    PRINT "Une erreur s'est produite"
    END

' GOSUB — utilise SUB à la place dans le code moderne
GOSUB AfficherEntete
PRINT "Contenu"
GOSUB AfficherPied
END

AfficherEntete:
    PRINT "=== EN-TÊTE ==="
    RETURN

AfficherPied:
    PRINT "=== PIED ==="
    RETURN
```

---

## 7. Tableaux

```basic
' Tableau de taille fixe
DIM notes(10) AS INTEGER          ' indices 0 à 10 (11 éléments)
DIM matrice(1 TO 5, 1 TO 5) AS DOUBLE  ' 5×5, indices 1..5

' Changer la borne inférieure par défaut
OPTION BASE 1
DIM valeurs(10) AS INTEGER  ' indices 1 à 10

' REDIM — tableau dynamique
DIM tableau() AS INTEGER
REDIM tableau(n)
REDIM PRESERVE tableau(n * 2)  ' redimensionner en conservant les données

' Bornes
PRINT LBOUND(tableau)   ' borne inférieure
PRINT UBOUND(tableau)   ' borne supérieure

' Tableau de types définis
TYPE Point
    x AS DOUBLE
    y AS DOUBLE
END TYPE

DIM points(1 TO 10) AS Point
points(1).x = 1.0
points(1).y = 2.0
```

---

## 8. Sous-routines et fonctions

### SUB

```basic
SUB Saluer (nom AS STRING, fois AS INTEGER)
    DIM i AS INTEGER
    FOR i = 1 TO fois
        PRINT i; ". Bonjour, "; nom; " !"
    NEXT i
END SUB

CALL Saluer("Alice", 3)
Saluer "Alice", 3      ' CALL est optionnel en QB64

SUB TraiterValeur (x AS INTEGER)
    IF x < 0 THEN
        PRINT "Valeur négative ignorée"
        EXIT SUB
    END IF
    PRINT "Traitement :"; x
END SUB
```

### FUNCTION

```basic
FUNCTION Additionner (a AS INTEGER, b AS INTEGER) AS INTEGER
    Additionner = a + b    ' assigner au nom de la fonction = retourner
END FUNCTION

FUNCTION Factorielle (n AS INTEGER) AS LONG
    IF n <= 1 THEN
        Factorielle = 1
    ELSE
        Factorielle = n * Factorielle(n - 1)
    END IF
END FUNCTION

FUNCTION EstPremier (n AS INTEGER) AS INTEGER
    DIM i AS INTEGER
    IF n < 2 THEN EstPremier = 0: EXIT FUNCTION
    FOR i = 2 TO INT(SQR(n))
        IF n MOD i = 0 THEN EstPremier = 0: EXIT FUNCTION
    NEXT i
    EstPremier = -1   ' -1 = vrai en BASIC
END FUNCTION

PRINT Additionner(3, 4)          ' 7
PRINT Factorielle(10)             ' 3628800
IF EstPremier(17) THEN PRINT "Premier"
```

### Par référence vs par valeur

```basic
' Par défaut : passage PAR RÉFÉRENCE — la SUB peut modifier la variable originale
SUB Doubler (x AS INTEGER)
    x = x * 2
END SUB

DIM n AS INTEGER: n = 5
Doubler n
PRINT n   ' 10 — n a été modifié !

' BYVAL — passage par valeur — la SUB obtient une copie
SUB DoublerSafe (BYVAL x AS INTEGER)
    x = x * 2          ' modifie seulement la copie locale
    PRINT "Dedans :"; x
END SUB

n = 5
DoublerSafe n
PRINT n   ' toujours 5 — l'original n'est pas modifié
```

---

## 9. Entrées / Sorties

### PRINT et PRINT USING

```basic
PRINT "Bonjour"
PRINT "A"; "B"; "C"       ' "ABC" — point-virgule = sans espace
PRINT "A", "B", "C"       ' séparation par tabulation
PRINT                      ' ligne vide
PRINT "Valeur ="; 42

' PRINT USING — sortie formatée
' # = chiffre, . = point décimal, $ = signe dollar, , = séparateur milliers
PRINT USING "###.##"; 3.14159       ' "  3.14"
PRINT USING "$$$,$$$.##"; 12345.6  ' " $12,345.60"
PRINT USING "+###"; 42             ' " +42"
PRINT USING "+###"; -42            ' "  -42"
```

### INPUT et LINE INPUT

```basic
INPUT "Ton âge : ", age
LINE INPUT "Ton nom complet : ", nomComplet$
' LINE INPUT lit toute la ligne y compris les virgules et espaces
```

### Fichiers

```basic
' Écriture séquentielle
OPEN "donnees.txt" FOR OUTPUT AS #1
PRINT #1, "Alice", 30, "Paris"
CLOSE #1

' Lecture séquentielle
DIM nom AS STRING, age AS INTEGER, ville AS STRING
OPEN "donnees.txt" FOR INPUT AS #1
DO WHILE NOT EOF(1)
    INPUT #1, nom, age, ville
    PRINT nom; " ("; age; ") — "; ville
LOOP
CLOSE #1

' Accès aléatoire — enregistrements de longueur fixe
TYPE Employe
    nomEmp AS STRING * 20
    salaire AS DOUBLE
END TYPE

DIM emp AS Employe
OPEN "employes.dat" FOR RANDOM AS #2 LEN = LEN(emp)
emp.nomEmp = "Alice"
emp.salaire = 55000.0
PUT #2, 1, emp   ' écrire à la position 1
GET #2, 1, emp   ' lire depuis la position 1
CLOSE #2
```

---

## 10. Fonctions mathématiques

```basic
' Trigonométrie (radians)
SIN(x)  COS(x)  TAN(x)  ATN(x)  ' pas d'ASIN/ACOS en BASIC classique

' Astuce pi : pi = 4 * ATN(1)

SQR(x)            ' racine carrée
x ^ n             ' x à la puissance n
ABS(x)            ' valeur absolue
SGN(x)            ' signe : -1, 0 ou 1
INT(x)            ' plancher (vers -∞) : INT(-2.3) = -3
FIX(x)            ' troncature (vers 0) : FIX(-2.3) = -2
CINT(x)           ' arrondi à l'entier le plus proche
LOG(x)            ' logarithme naturel
EXP(x)            ' e ^ x

RANDOMIZE TIMER   ' initialiser le générateur — appeler une fois au départ
x = RND           ' 0.0 <= x < 1.0
n = INT(RND * 6) + 1  ' dé : 1..6
```

---

## 11. Conversion de types

```basic
CInt(x)   CLng(x)   CSng(x)   CDbl(x)   ' conversions numériques

STR$(42)          ' " 42" (espace devant les positifs)
LTRIM$(STR$(42))  ' "42"
VAL("42")         ' 42
VAL("3.14abc")    ' 3.14

CHR$(65)          ' "A"
ASC("A")          ' 65
HEX$(255)         ' "FF"
OCT$(255)         ' "377"
&HFF              ' 255 — littéral hexadécimal
&B11111111        ' 255 — littéral binaire (QB64)
```

---

## 12. Gestion des erreurs

```basic
ON ERROR GOTO GestionErreur

OPEN "manquant.txt" FOR INPUT AS #1   ' déclenchera une erreur
CLOSE #1
PRINT "Succès"
END

GestionErreur:
    PRINT "Erreur n°"; ERR
    PRINT "Message : "; ERROR$(ERR)
    RESUME NEXT     ' continue après la ligne en erreur
    ' RESUME         ' réessaie la ligne en erreur
    ' RESUME label   ' saute à une étiquette

ON ERROR GOTO 0   ' désactive la gestion d'erreur

' Codes d'erreur courants
' 11 — Division par zéro
' 13 — Type incompatible
' 53 — Fichier non trouvé
' 62 — Lecture au-delà de la fin du fichier
```

---

## 13. Graphismes (QB64)

```basic
SCREEN 12    ' 640x480, 16 couleurs
SCREEN 13    ' 320x200, 256 couleurs

' QB64 — résolution personnalisée
DIM ecran AS LONG
ecran = _NEWIMAGE(800, 600, 32)
SCREEN ecran

' Couleurs
COLOR 14, 1                   ' jaune sur bleu
COLOR _RGB(255, 128, 0)        ' orange

' Primitives graphiques
PSET (100, 100), 12            ' pixel rouge
LINE (0, 0)-(100, 100), 10    ' ligne verte
LINE (50, 50)-(150, 150), 9, B  ' rectangle bleu
LINE (50, 50)-(150, 150), 9, BF ' rectangle bleu plein
CIRCLE (160, 100), 50, 12      ' cercle rouge
PAINT (100, 100), 10           ' remplissage

' Boucle d'animation (QB64)
DIM x AS INTEGER: x = 0
DO
    CLS
    CIRCLE (x, 240), 20, 12    ' balle rouge
    x = (x + 2) MOD 640
    _DISPLAY                   ' swap du buffer — animation fluide
    _LIMIT 60                  ' 60 images/seconde
LOOP UNTIL INKEY$ = CHR$(27)  ' Echap pour quitter
```

---

## 14. Spécificités QB64-PE

```basic
OPTION _EXPLICIT           ' comme "implicit none"

DIM i64 AS INTEGER64       ' entier 64 bits
DIM u32 AS _UNSIGNED LONG  ' non-signé 32 bits

_CLIPBOARD$ = "Texte"      ' écrire dans le presse-papiers
PRINT _CLIPBOARD$          ' lire depuis le presse-papiers

PRINT TIMER                ' secondes depuis minuit

PLAY "MBT180 L4 CDEFGAB"   ' jouer de la musique (chaîne musicale classique !)
SOUND 440, 18              ' fréquence (Hz), durée (ticks à 18.2/s)

SHELL "dir"                ' commande shell

TYPE Vecteur2D
    x AS DOUBLE
    y AS DOUBLE
END TYPE
DIM v AS Vecteur2D
v.x = 3.0: v.y = 4.0
PRINT SQR(v.x^2 + v.y^2)  ' 5.0
```

---

## 15. Différences FreeBASIC

```basic
' Commentaires style C
/* Commentaire C */
// Commentaire C++

' RETURN (plus propre que d'assigner au nom de la fonction)
FUNCTION Additionner(a AS INTEGER, b AS INTEGER) AS INTEGER
    RETURN a + b
END FUNCTION

' Pointeurs
DIM x AS INTEGER = 42
DIM ptr AS INTEGER PTR
ptr = @x       ' @ prend l'adresse
PRINT *ptr     ' 42
*ptr = 100
PRINT x        ' 100

' Espaces de noms
NAMESPACE Utils
    FUNCTION Carre(x AS DOUBLE) AS DOUBLE
        RETURN x * x
    END FUNCTION
END NAMESPACE
PRINT Utils.Carre(5)   ' 25.0

' SCOPE — bloc de portée locale
SCOPE
    DIM varLocale AS INTEGER = 99
    PRINT varLocale
END SCOPE
' varLocale inaccessible ici
```

---

## 16. Patterns classiques BASIC

### Lire des données intégrées (DATA / READ)

```basic
READ nom$, score
PRINT nom$; score

DATA "Alice", 95
DATA "Bob", 82

RESTORE   ' remet le pointeur DATA au début
```

### Boucle de menu

```basic
DO
    PRINT "=== MENU PRINCIPAL ==="
    PRINT "1. Nouvelle partie"
    PRINT "2. Charger"
    PRINT "3. Quitter"
    INPUT "Choix : ", choix

    SELECT CASE choix
        CASE 1 : CALL NouvellePartie
        CASE 2 : CALL Charger
        CASE 3 : PRINT "Au revoir !": EXIT DO
        CASE ELSE : PRINT "Choix invalide."
    END SELECT
LOOP
```

### Type défini (structure)

```basic
TYPE Cercle
    cx AS DOUBLE
    cy AS DOUBLE
    rayon AS DOUBLE
END TYPE

DIM c AS Cercle
c.cx = 100.0
c.cy = 150.0
c.rayon = 50.0

PRINT "Aire ="; 3.14159 * c.rayon ^ 2

DIM formes(1 TO 10) AS Cercle
formes(1).cx = 0.0
formes(1).rayon = 25.0
```

---

## 17. Carte de référence rapide

### Types de données en un coup d'œil

| Déclaration | Type | Taille / Plage |
|---|---|---|
| `AS INTEGER` ou `%` | Entier signé | 16 bits, -32768 à 32767 |
| `AS LONG` ou `&` | Entier signé | 32 bits |
| `AS INTEGER64` ou `&&` | Entier signé | 64 bits |
| `AS SINGLE` ou `!` | Flottant | 32 bits, ~7 décimales |
| `AS DOUBLE` ou `#` | Flottant | 64 bits, ~15 décimales |
| `AS STRING` ou `$` | Chaîne variable | Jusqu'à ~2 milliards de caractères |
| `AS STRING * n` | Chaîne fixe | Exactement n caractères |

### Fonctions de chaînes en un coup d'œil

| Fonction | Rôle |
|---|---|
| `LEN(s$)` | Longueur |
| `LEFT$(s$, n)` | n premiers caractères |
| `RIGHT$(s$, n)` | n derniers caractères |
| `MID$(s$, p, n)` | n caractères à partir de la position p |
| `INSTR(s$, t$)` | Position de t$ dans s$ (0 si absent) |
| `UCASE$(s$)` / `LCASE$(s$)` | Majuscules / minuscules |
| `TRIM$(s$)` | Supprimer espaces début et fin |
| `STR$(n)` | Nombre → chaîne |
| `VAL(s$)` | Chaîne → nombre |
| `CHR$(n)` | Code ASCII → caractère |
| `ASC(s$)` | Premier caractère → code ASCII |

### Ce qu'on confond souvent

| | |
|---|---|
| `=` | Affectation ET comparaison à la fois |
| `-1` | VRAI en BASIC |
| `0` | FAUX en BASIC |
| `INT(-2.3)` | `-3` (plancher vers -∞) |
| `FIX(-2.3)` | `-2` (troncature vers 0) |
| `\` | Division entière |
| `^` | Exponentiation |
| `PRINT a; b` | Sans séparateur entre a et b |
| `PRINT a, b` | Séparation par tabulation |
| `INPUT` | Coupe sur les virgules — utilise `LINE INPUT` pour les lignes complètes |
| Par défaut | Passage **par référence** — la SUB peut modifier l'original |
| `BYVAL` | Passage **par valeur** — la SUB obtient une copie |
