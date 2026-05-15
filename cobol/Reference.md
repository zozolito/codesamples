# COBOL — Reference Manual

> A practical reference for GnuCOBOL and standard COBOL (COBOL 85 / COBOL 2002 / COBOL 2014).  
> All examples compile with `cobc -x -g source.cbl`.

---

## 🇬🇧 English

### Table of Contents

1. [Source Format](#1-source-format)
2. [Program Structure — The Four Divisions](#2-program-structure--the-four-divisions)
3. [IDENTIFICATION DIVISION](#3-identification-division)
4. [ENVIRONMENT DIVISION](#4-environment-division)
5. [DATA DIVISION](#5-data-division)
   - [Level Numbers](#51-level-numbers)
   - [PICTURE Clause (PIC)](#52-picture-clause-pic)
   - [VALUE Clause](#53-value-clause)
   - [REDEFINES](#54-redefines)
   - [OCCURS — Arrays](#55-occurs--arrays)
   - [88-Level Condition Names](#56-88-level-condition-names)
   - [COPY — Copybooks](#57-copy--copybooks)
6. [PROCEDURE DIVISION](#6-procedure-division)
   - [MOVE](#61-move)
   - [ADD / SUBTRACT / MULTIPLY / DIVIDE / COMPUTE](#62-add--subtract--multiply--divide--compute)
   - [IF / ELSE / END-IF](#63-if--else--end-if)
   - [EVALUATE](#64-evaluate)
   - [PERFORM](#65-perform)
   - [GO TO](#66-go-to)
   - [ACCEPT / DISPLAY](#67-accept--display)
   - [STRING / UNSTRING](#68-string--unstring)
   - [INSPECT](#69-inspect)
   - [CALL — Subprograms](#610-call--subprograms)
   - [STOP RUN / EXIT PROGRAM / GOBACK](#611-stop-run--exit-program--goback)
7. [File Handling](#7-file-handling)
   - [Sequential Files](#71-sequential-files)
   - [Indexed Files (VSAM-style)](#72-indexed-files-vsam-style)
8. [Intrinsic Functions](#8-intrinsic-functions)
9. [Scope Terminators](#9-scope-terminators)
10. [Common Patterns and Idioms](#10-common-patterns-and-idioms)
11. [Compiler Directives (GnuCOBOL)](#11-compiler-directives-gnucobol)
12. [Error Handling — ON ERROR / ON EXCEPTION](#12-error-handling--on-error--on-exception)
13. [Quick Reference Card](#13-quick-reference-card)

---

## 1. Source Format

### Fixed format (traditional — COBOL 74/85)

```
Col 1–6   : Sequence numbers (optional, ignored by compiler)
Col 7     : Indicator
              ' '  normal code
              '*'  comment line
              '/'  page eject (comment in GnuCOBOL)
              '-'  continuation of previous line
              'D'  debug line (compiled only with WITH DEBUGGING MODE)
Col 8–11  : Area A — DIVISION, SECTION, paragraph names, level 01 and 77
Col 12–72 : Area B — all other statements
Col 73–80 : Identification (optional, ignored)
```

### Free format (modern — COBOL 2002+)

```cobol
>>SOURCE FORMAT FREE
```

Place this directive at the top of the file. No column restrictions apply. `*>` is the comment marker.

```cobol
>>SOURCE FORMAT FREE
*> This is a free-format comment

IDENTIFICATION DIVISION.
PROGRAM-ID. FREE-FORMAT-DEMO.
```

> GnuCOBOL accepts `-free` as a compiler flag: `cobc -x -free source.cbl`

---

## 2. Program Structure — The Four Divisions

Every COBOL program follows this skeleton, in this exact order:

```cobol
IDENTIFICATION DIVISION.          *> required
    PROGRAM-ID. program-name.     *> required

ENVIRONMENT DIVISION.             *> optional
    CONFIGURATION SECTION.
    INPUT-OUTPUT SECTION.

DATA DIVISION.                    *> optional
    FILE SECTION.
    WORKING-STORAGE SECTION.
    LOCAL-STORAGE SECTION.
    LINKAGE SECTION.

PROCEDURE DIVISION.               *> required
    [paragraphs and sections]
    STOP RUN.
```

---

## 3. IDENTIFICATION DIVISION

```cobol
IDENTIFICATION DIVISION.
PROGRAM-ID.    PAYROLL-CALC.
AUTHOR.        Grace Hopper.
DATE-WRITTEN.  1960-04-01.
DATE-COMPILED. 2024-01-01.
SECURITY.      CONFIDENTIAL.
```

Only `PROGRAM-ID` is required. The other entries are informational and ignored by most compilers (but useful for human readers and auditors — which was the point).

---

## 4. ENVIRONMENT DIVISION

```cobol
ENVIRONMENT DIVISION.

CONFIGURATION SECTION.
    SOURCE-COMPUTER.  LINUX.
    OBJECT-COMPUTER.  LINUX.
    SPECIAL-NAMES.
        DECIMAL-POINT IS COMMA.   *> use comma as decimal separator (European style)

INPUT-OUTPUT SECTION.
    FILE-CONTROL.
        SELECT EMPLOYEE-FILE
            ASSIGN TO "employees.dat"
            ORGANIZATION IS LINE SEQUENTIAL
            ACCESS MODE IS SEQUENTIAL
            FILE STATUS IS WS-FILE-STATUS.
```

### File organizations

| Organization | Description |
|---|---|
| `LINE SEQUENTIAL` | Text file, records separated by newline |
| `SEQUENTIAL` | Binary sequential, fixed-length records |
| `INDEXED` | Keyed access (VSAM on mainframes, file-based on GnuCOBOL) |
| `RELATIVE` | Access by record number |

### Access modes

| Mode | Works with |
|---|---|
| `SEQUENTIAL` | All organizations |
| `RANDOM` | INDEXED, RELATIVE |
| `DYNAMIC` | INDEXED, RELATIVE (can switch between sequential and random) |

---

## 5. DATA DIVISION

### 5.1 Level Numbers

Level numbers define the hierarchy of data. Think of them as nesting.

```cobol
DATA DIVISION.
WORKING-STORAGE SECTION.

*> Level 01 — top-level group or elementary item
    01  WS-EMPLOYEE.
*>      Level 05 — subordinate group
        05  WS-EMP-NAME.
*>          Level 10 — elementary items
            10  WS-FIRST-NAME    PIC X(20).
            10  WS-LAST-NAME     PIC X(20).
        05  WS-EMP-ID            PIC 9(6).
        05  WS-SALARY            PIC 9(7)V99.

*> Level 77 — standalone elementary item (not part of a group)
    77  WS-COUNTER               PIC 9(4) VALUE ZERO.

*> Level 66 — RENAMES (rarely used)
    66  WS-FULL-NAME RENAMES WS-FIRST-NAME THRU WS-LAST-NAME.

*> Level 88 — condition name (see section 5.6)
    01  WS-STATUS                PIC X.
        88  STATUS-ACTIVE        VALUE "A".
        88  STATUS-INACTIVE      VALUE "I".
```

**Level number rules:**
- `01` and `77` must start in Area A (col 8–11 in fixed format)
- `02`–`49` must be strictly increasing within a group (gaps allowed: 01, 05, 10, 15 is idiomatic)
- `66`, `77`, `88` are special — not part of the 01–49 hierarchy
- `01` in FILE SECTION describes a file record

---

### 5.2 PICTURE Clause (PIC)

`PIC` (or `PICTURE`) describes the type and shape of a field.

#### Character symbols

| Symbol | Meaning |
|---|---|
| `9` | Numeric digit (0–9) |
| `A` | Alphabetic character (A–Z, a–z, space) |
| `X` | Alphanumeric (any character) |
| `V` | Implied decimal point (not stored, just a marker) |
| `S` | Sign (positive or negative) — must be leftmost |
| `P` | Implied scaling position (rarely used) |

#### Repetition shorthand

```cobol
PIC 9(6)      *> same as PIC 999999
PIC X(50)     *> same as PIC XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
PIC 9(7)V99   *> 7 integer digits, implied decimal, 2 decimal digits
PIC S9(5)V99  *> signed, 5 integer digits, 2 decimal digits
```

#### Edited pictures — for display/print only

Edited fields cannot be used in arithmetic. They format output.

```cobol
01  WS-AMOUNT-DISPLAY   PIC $,$$$,$$9.99.
01  WS-DATE-DISPLAY     PIC 99/99/9999.
01  WS-PHONE-DISPLAY    PIC (999)999-9999.
01  WS-SIGNED-DISPLAY   PIC +$$$,$$$,$$9.99.
01  WS-CREDIT-DISPLAY   PIC $,$$$,$$9.99CR.  *> shows CR if negative

*> Z suppresses leading zeros
01  WS-ZERO-SUPPRESSED  PIC ZZZ,ZZ9.99.     *> "    123.45" not "000,123.45"

*> * fills leading zeros with asterisks (cheque protection)
01  WS-CHEQUE-AMOUNT    PIC ***,**9.99.      *> "***,123.45"
```

#### USAGE clause — internal storage format

```cobol
01  WS-PACKED           PIC S9(7)V99 USAGE PACKED-DECIMAL.  *> BCD, efficient for arithmetic
01  WS-BINARY           PIC S9(9)    USAGE BINARY.           *> native integer
01  WS-FLOAT            USAGE FLOAT-SHORT.                   *> 32-bit IEEE 754
01  WS-DOUBLE           USAGE FLOAT-LONG.                    *> 64-bit IEEE 754
01  WS-DISPLAY          PIC X(10)    USAGE DISPLAY.          *> default, stored as characters
```

> **PACKED-DECIMAL** (`COMP-3` in IBM dialect) is the traditional COBOL workhorse for numeric processing. Two digits per byte, very efficient on mainframes. Use it for any field that will be used heavily in arithmetic.

---

### 5.3 VALUE Clause

```cobol
01  WS-NAME        PIC X(20)  VALUE "Alice".
01  WS-COUNTER     PIC 9(4)   VALUE ZERO.
01  WS-TOTAL       PIC 9(9)   VALUE ZEROS.    *> same as ZERO
01  WS-FLAG        PIC X      VALUE SPACE.    *> or SPACES
01  WS-ALL-NINES   PIC 9(6)   VALUE 999999.
01  WS-ALL-HIGH    PIC X(5)   VALUE HIGH-VALUES.  *> x"FF" × 5, useful for max comparisons
01  WS-ALL-LOW     PIC X(5)   VALUE LOW-VALUES.   *> x"00" × 5
01  WS-ALL-QUOTES  PIC X(5)   VALUE QUOTE.        *> '"' × 5

*> Figurative constants reference
*> ZERO / ZEROS / ZEROES  → 0 or "0" or SPACES depending on type
*> SPACE / SPACES         → " "
*> HIGH-VALUE / HIGH-VALUES → highest collating sequence character
*> LOW-VALUE  / LOW-VALUES  → lowest collating sequence character
*> QUOTE / QUOTES         → quotation mark character
*> ALL "x"                → the character x repeated to fill the field
```

---

### 5.4 REDEFINES

`REDEFINES` allows two different views of the same memory area.

```cobol
01  WS-DATE-NUMERIC     PIC 9(8).           *> 20241231
01  WS-DATE-PARTS       REDEFINES WS-DATE-NUMERIC.
    05  WS-YEAR         PIC 9(4).           *> 2024
    05  WS-MONTH        PIC 9(2).           *> 12
    05  WS-DAY          PIC 9(2).           *> 31

*> Both refer to the same 8-byte storage.
*> Writing to WS-DATE-NUMERIC changes WS-YEAR/WS-MONTH/WS-DAY and vice versa.

*> Union-like usage — parse different record types from the same buffer
01  WS-RECORD-BUFFER    PIC X(100).
01  WS-RECORD-TYPE-A    REDEFINES WS-RECORD-BUFFER.
    05  WS-A-ID         PIC 9(6).
    05  WS-A-NAME       PIC X(30).
    05  FILLER          PIC X(64).
01  WS-RECORD-TYPE-B    REDEFINES WS-RECORD-BUFFER.
    05  WS-B-CODE       PIC X(3).
    05  WS-B-AMOUNT     PIC 9(9)V99.
    05  FILLER          PIC X(85).
```

---

### 5.5 OCCURS — Arrays

```cobol
*> Fixed-size array
01  WS-SCORES.
    05  WS-SCORE        PIC 9(3) OCCURS 10 TIMES.

*> Access with subscript (1-based)
    MOVE 95 TO WS-SCORE(1)
    MOVE 87 TO WS-SCORE(10)

*> With INDEXED BY — use SET to move the index
01  WS-TABLE.
    05  WS-ENTRY        PIC X(20) OCCURS 100 TIMES
                        INDEXED BY WS-IDX.

    SET WS-IDX TO 1
    MOVE "Alice" TO WS-ENTRY(WS-IDX)
    SET WS-IDX UP BY 1

*> Variable-length array (OCCURS DEPENDING ON)
01  WS-DYNAMIC-TABLE.
    05  WS-ITEM-COUNT   PIC 9(4).
    05  WS-ITEMS        PIC X(30) OCCURS 1 TO 200 TIMES
                        DEPENDING ON WS-ITEM-COUNT.

*> Multi-dimensional array
01  WS-MATRIX.
    05  WS-ROW          OCCURS 5 TIMES.
        10  WS-COL      PIC 9(4) OCCURS 5 TIMES.

    MOVE 42 TO WS-COL(3, 4)    *> row 3, column 4
```

---

### 5.6 88-Level Condition Names

Level 88 items are named conditions attached to a parent field. They read like English.

```cobol
01  WS-RESPONSE-CODE    PIC 9(3).
    88  SUCCESS         VALUE 200.
    88  CREATED         VALUE 201.
    88  NOT-FOUND       VALUE 404.
    88  SERVER-ERROR    VALUE 500 THRU 599.
    88  CLIENT-ERROR    VALUE 400 THRU 499.

01  WS-GENDER           PIC X.
    88  MALE            VALUE "M".
    88  FEMALE          VALUE "F".
    88  VALID-GENDER    VALUE "M" "F".   *> multiple values

*> Usage in PROCEDURE DIVISION
    MOVE 404 TO WS-RESPONSE-CODE
    IF NOT-FOUND
        DISPLAY "Resource not found"
    END-IF

    IF SERVER-ERROR
        DISPLAY "Something went wrong on the server"
    END-IF

*> SET a condition to TRUE (moves the first VALUE to the parent)
    SET SUCCESS TO TRUE          *> MOVE 200 TO WS-RESPONSE-CODE
    SET NOT-FOUND TO TRUE        *> MOVE 404 TO WS-RESPONSE-CODE
```

---

### 5.7 COPY — Copybooks

Copybooks are reusable DATA DIVISION fragments, stored in separate files (typically `.cpy`).

```cobol
*> employee-record.cpy
    05  EMP-ID           PIC 9(6).
    05  EMP-FIRST-NAME   PIC X(20).
    05  EMP-LAST-NAME    PIC X(20).
    05  EMP-SALARY       PIC 9(7)V99 PACKED-DECIMAL.
    05  EMP-STATUS       PIC X.
        88  EMP-ACTIVE   VALUE "A".
        88  EMP-RETIRED  VALUE "R".
        88  EMP-FIRED    VALUE "F".
```

```cobol
*> main.cbl
DATA DIVISION.
FILE SECTION.
    FD  EMPLOYEE-FILE.
    01  EMPLOYEE-RECORD.
        COPY "employee-record.cpy".

WORKING-STORAGE SECTION.
    01  WS-EMPLOYEE.
        COPY "employee-record.cpy"
            REPLACING EMP- BY WS-EMP-.    *> prefix replacement
```

---

## 6. PROCEDURE DIVISION

### 6.1 MOVE

`MOVE` is the assignment operator in COBOL.

```cobol
*> Basic move
MOVE "Alice"      TO WS-NAME
MOVE 42           TO WS-COUNTER
MOVE ZERO         TO WS-TOTAL
MOVE SPACES       TO WS-BUFFER
MOVE WS-A         TO WS-B

*> Move to multiple destinations
MOVE ZERO TO WS-COUNT WS-TOTAL WS-AVERAGE

*> MOVE CORRESPONDING — copies fields with matching names between groups
01  WS-SOURCE.
    05  NAME    PIC X(20).
    05  AGE     PIC 9(3).
    05  CITY    PIC X(30).

01  WS-TARGET.
    05  NAME    PIC X(20).
    05  AGE     PIC 9(3).
    05  PHONE   PIC X(15).     *> no match — not moved

MOVE CORRESPONDING WS-SOURCE TO WS-TARGET
*> copies NAME and AGE, skips CITY (no match) and PHONE (no match)
```

**Move rules (important):**

| Source → Destination | Behavior |
|---|---|
| Numeric → Numeric | Aligned by decimal point, truncated/zero-filled |
| Numeric → Alphanumeric | Right-justified in the receiving field |
| Alphanumeric → Alphanumeric | Left-justified, space-filled on the right |
| Alphanumeric → Numeric | Only if source contains digits only |
| Edited → Alphanumeric | Moves the formatted characters |
| Group → Group | Treated as alphanumeric (raw byte copy) |

---

### 6.2 ADD / SUBTRACT / MULTIPLY / DIVIDE / COMPUTE

```cobol
*> ADD
ADD 1            TO WS-COUNTER
ADD WS-PRICE     TO WS-TOTAL
ADD WS-A, WS-B   TO WS-C                 *> C = C + A + B
ADD WS-A TO WS-B GIVING WS-RESULT        *> RESULT = A + B (B unchanged)
ADD CORRESPONDING WS-GROUP-A TO WS-GROUP-B

*> SUBTRACT
SUBTRACT 1            FROM WS-COUNTER
SUBTRACT WS-TAX       FROM WS-GROSS GIVING WS-NET
SUBTRACT CORRESPONDING WS-A FROM WS-B

*> MULTIPLY
MULTIPLY 2         BY WS-VALUE
MULTIPLY WS-HOURS  BY WS-RATE GIVING WS-AMOUNT

*> DIVIDE
DIVIDE 2            INTO WS-VALUE
DIVIDE WS-TOTAL     BY WS-COUNT GIVING WS-AVERAGE
DIVIDE WS-AMOUNT    BY 3 GIVING WS-SHARE REMAINDER WS-LEFT-OVER

*> ROUNDED — rounds result to receiving field's decimal precision
ADD WS-A TO WS-B GIVING WS-RESULT ROUNDED

*> ON SIZE ERROR — catch overflow
ADD WS-A TO WS-B
    ON SIZE ERROR
        DISPLAY "Overflow!"
        MOVE ZERO TO WS-B
END-ADD

*> COMPUTE — expression syntax, usually clearest
COMPUTE WS-RESULT = WS-A + WS-B
COMPUTE WS-AREA   = WS-PI * WS-RADIUS ** 2          *> ** is exponentiation
COMPUTE WS-NET    = (WS-GROSS - WS-DEDUCTIONS) * 0.87 ROUNDED
COMPUTE WS-BMI    = WS-WEIGHT / (WS-HEIGHT ** 2)
    ON SIZE ERROR
        DISPLAY "BMI calculation overflow"
END-COMPUTE

*> Operator precedence in COMPUTE: () > ** > * / > + -
```

---

### 6.3 IF / ELSE / END-IF

```cobol
*> Basic
IF WS-AGE >= 18
    DISPLAY "Adult"
ELSE
    DISPLAY "Minor"
END-IF

*> Nested
IF WS-SCORE >= 90
    DISPLAY "A"
ELSE
    IF WS-SCORE >= 80
        DISPLAY "B"
    ELSE
        IF WS-SCORE >= 70
            DISPLAY "C"
        ELSE
            DISPLAY "F"
        END-IF
    END-IF
END-IF

*> Multiple conditions
IF WS-STATUS = "A" AND WS-BALANCE > ZERO
    PERFORM PROCESS-ACTIVE-ACCOUNT
END-IF

IF WS-CODE = "E" OR WS-CODE = "W"
    PERFORM LOG-WARNING
END-IF

*> NOT
IF NOT (WS-STATUS = "A")
    DISPLAY "Not active"
END-IF

*> Class conditions
IF WS-INPUT IS NUMERIC
    MOVE WS-INPUT TO WS-AMOUNT
END-IF

IF WS-NAME IS ALPHABETIC
    DISPLAY "Valid name"
END-IF

IF WS-NAME IS ALPHABETIC-LOWER
    DISPLAY "All lowercase"
END-IF

*> Sign conditions
IF WS-BALANCE IS NEGATIVE
    DISPLAY "Overdrawn"
END-IF

IF WS-AMOUNT IS POSITIVE
    PERFORM PROCESS-CREDIT
END-IF

IF WS-VALUE IS ZERO
    MOVE 1 TO WS-VALUE
END-IF
```

---

### 6.4 EVALUATE

`EVALUATE` is COBOL's switch/case — but far more powerful.

```cobol
*> Evaluate a single value
EVALUATE WS-GRADE
    WHEN "A"
        DISPLAY "Excellent"
    WHEN "B" "C"
        DISPLAY "Good"
    WHEN "D"
        DISPLAY "Passing"
    WHEN OTHER
        DISPLAY "Failing"
END-EVALUATE

*> Evaluate with ranges
EVALUATE WS-SCORE
    WHEN 90 THRU 100
        DISPLAY "A"
    WHEN 80 THRU 89
        DISPLAY "B"
    WHEN 70 THRU 79
        DISPLAY "C"
    WHEN ZERO THRU 69
        DISPLAY "F"
END-EVALUATE

*> Evaluate TRUE — equivalent to IF/ELSE chain, reads like English
EVALUATE TRUE
    WHEN WS-STATUS = "A" AND WS-BALANCE > ZERO
        PERFORM PROCESS-ACTIVE
    WHEN WS-STATUS = "S"
        PERFORM PROCESS-SUSPENDED
    WHEN WS-BALANCE < ZERO
        PERFORM PROCESS-OVERDRAWN
    WHEN OTHER
        PERFORM PROCESS-DEFAULT
END-EVALUATE

*> Multiple ALSO — tuple matching
EVALUATE WS-TYPE ALSO WS-STATUS
    WHEN "CORP" ALSO "A"
        PERFORM CORP-ACTIVE
    WHEN "CORP" ALSO "I"
        PERFORM CORP-INACTIVE
    WHEN "INDV" ALSO ANY
        PERFORM INDIVIDUAL
    WHEN OTHER
        PERFORM UNKNOWN
END-EVALUATE
```

---

### 6.5 PERFORM

`PERFORM` is used for loops and calling paragraphs.

```cobol
*> Call a paragraph (subroutine)
PERFORM PRINT-HEADER
PERFORM PROCESS-RECORDS
PERFORM PRINT-FOOTER

*> PERFORM n TIMES
PERFORM 5 TIMES
    ADD 1 TO WS-COUNTER
END-PERFORM

*> PERFORM UNTIL
PERFORM UNTIL WS-EOF = "Y"
    READ EMPLOYEE-FILE
        AT END MOVE "Y" TO WS-EOF
        NOT AT END PERFORM PROCESS-RECORD
    END-READ
END-PERFORM

*> PERFORM VARYING — indexed loop
PERFORM VARYING WS-IDX FROM 1 BY 1
    UNTIL WS-IDX > WS-MAX-RECORDS
    PERFORM PROCESS-ITEM
END-PERFORM

*> PERFORM VARYING inline (with nested logic)
PERFORM VARYING WS-I FROM 1 BY 1 UNTIL WS-I > 10
    PERFORM VARYING WS-J FROM 1 BY 1 UNTIL WS-J > 10
        COMPUTE WS-MATRIX(WS-I, WS-J) = WS-I * WS-J
    END-PERFORM
END-PERFORM

*> PERFORM ... TEST AFTER — do/while equivalent (executes at least once)
PERFORM UNTIL EXIT
    DISPLAY "Enter command (Q to quit): " WITH NO ADVANCING
    ACCEPT WS-COMMAND
    IF WS-COMMAND = "Q"
        EXIT PERFORM
    END-IF
    PERFORM EXECUTE-COMMAND
END-PERFORM

*> PERFORM a range of paragraphs
PERFORM OPEN-FILES THRU OPEN-FILES-EXIT
```

---

### 6.6 GO TO

```cobol
*> Unconditional — avoid in modern COBOL; use PERFORM instead
GO TO END-OF-PROGRAM

*> GO TO DEPENDING ON — computed GOTO (select paragraph by number)
GO TO PROC-A PROC-B PROC-C DEPENDING ON WS-SELECTOR
*> if WS-SELECTOR = 1 → PROC-A, = 2 → PROC-B, = 3 → PROC-C
```

> `GO TO` is a relic. You will see it in legacy code constantly. In new code, use `PERFORM` and structured flow instead.

---

### 6.7 ACCEPT / DISPLAY

```cobol
*> DISPLAY — write to stdout
DISPLAY "Hello, World!"
DISPLAY "Name: " WS-NAME
DISPLAY WS-AMOUNT
DISPLAY "Total: " WS-TOTAL " units"
DISPLAY "No newline" WITH NO ADVANCING
DISPLAY "" *> blank line

*> DISPLAY to stderr
DISPLAY "Error: " WS-MSG UPON SYSERR

*> ACCEPT — read from stdin
ACCEPT WS-NAME
ACCEPT WS-AGE

*> ACCEPT from the environment
ACCEPT WS-DATE FROM DATE        *> YYMMDD format
ACCEPT WS-DATE FROM DATE YYYYMMDD  *> YYYYMMDD format
ACCEPT WS-TIME FROM TIME        *> HHMMSSss (hundredths)
ACCEPT WS-DAY  FROM DAY         *> YYDDD (Julian)
ACCEPT WS-DAY  FROM DAY YYYYDDD
```

---

### 6.8 STRING / UNSTRING

```cobol
*> STRING — concatenate into one field
*> DELIMITED SIZE — use the full content of the field
*> DELIMITED SPACE — stop at first space

MOVE SPACES TO WS-FULL-NAME
STRING WS-FIRST-NAME DELIMITED SPACE
       " "            DELIMITED SIZE
       WS-LAST-NAME  DELIMITED SPACE
    INTO WS-FULL-NAME
    WITH POINTER WS-PTR    *> optional: WS-PTR points past last char written
    ON OVERFLOW
        DISPLAY "String too long"
END-STRING

*> Example: build a CSV line
STRING WS-ID    DELIMITED SIZE
       ","      DELIMITED SIZE
       WS-NAME  DELIMITED SPACE
       ","      DELIMITED SIZE
       WS-DEPT  DELIMITED SPACE
    INTO WS-CSV-LINE
END-STRING


*> UNSTRING — split a field into multiple destinations
*> Inverse of STRING

MOVE "Alice,30,Paris" TO WS-INPUT

UNSTRING WS-INPUT
    DELIMITED BY ","
    INTO WS-NAME
         WS-AGE
         WS-CITY
    WITH POINTER WS-PTR
    TALLYING IN WS-FIELD-COUNT
    ON OVERFLOW
        DISPLAY "Too many fields"
END-UNSTRING
*> WS-NAME = "Alice", WS-AGE = "30", WS-CITY = "Paris"
```

---

### 6.9 INSPECT

`INSPECT` counts or replaces characters in a field.

```cobol
*> TALLYING — count characters
INSPECT WS-TEXT TALLYING WS-COUNT FOR ALL "A"
INSPECT WS-TEXT TALLYING WS-COUNT FOR LEADING SPACES
INSPECT WS-TEXT TALLYING WS-COUNT FOR CHARACTERS  *> count all characters

*> REPLACING — replace characters in place
INSPECT WS-TEXT REPLACING ALL "," BY ";"
INSPECT WS-TEXT REPLACING LEADING SPACES BY ZEROS
INSPECT WS-TEXT REPLACING FIRST "X" BY "Y"

*> CONVERTING — translate characters (like tr)
INSPECT WS-TEXT CONVERTING
    "abcdefghijklmnopqrstuvwxyz"
    TO
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
```

---

### 6.10 CALL — Subprograms

```cobol
*> Static CALL — linked at compile time
CALL "CALC-TAX"
    USING BY REFERENCE WS-GROSS-AMOUNT
          BY REFERENCE WS-TAX-AMOUNT
          BY VALUE     WS-TAX-RATE
    ON EXCEPTION
        DISPLAY "CALC-TAX not found"
END-CALL

*> Dynamic CALL — program name in a variable
MOVE "CALC-TAX" TO WS-PROGRAM-NAME
CALL WS-PROGRAM-NAME USING WS-AMOUNT
    ON EXCEPTION
        DISPLAY "Program not found: " WS-PROGRAM-NAME
END-CALL

*> RETURNING — get a single return value
CALL "VALIDATE-ID"
    USING BY REFERENCE WS-EMP-ID
    RETURNING WS-VALIDATION-RESULT
END-CALL
```

**Subprogram skeleton (called program):**

```cobol
IDENTIFICATION DIVISION.
PROGRAM-ID. CALC-TAX.

DATA DIVISION.
LINKAGE SECTION.                         *> parameters declared here
    01  LS-GROSS     PIC 9(7)V99.
    01  LS-TAX       PIC 9(7)V99.
    01  LS-RATE      PIC V999.

PROCEDURE DIVISION USING LS-GROSS LS-TAX LS-RATE.
    COMPUTE LS-TAX = LS-GROSS * LS-RATE
    GOBACK.                              *> return to caller
```

**BY REFERENCE vs BY VALUE:**

| Mode | Behavior |
|---|---|
| `BY REFERENCE` | Caller's storage is shared — changes affect the caller |
| `BY VALUE` | A copy is passed — changes do not affect the caller |
| `BY CONTENT` | Like BY VALUE in most implementations |

---

### 6.11 STOP RUN / EXIT PROGRAM / GOBACK

| Statement | Effect |
|---|---|
| `STOP RUN` | Terminates the entire run unit (all programs in the call stack) |
| `EXIT PROGRAM` | Returns to the caller; if there is no caller, acts like `STOP RUN` |
| `GOBACK` | Returns to the caller; preferred modern alternative to `EXIT PROGRAM` |

```cobol
*> In the main program
STOP RUN.

*> In a called subprogram
GOBACK.

*> Conditional exit from a paragraph
PERFORM UNTIL WS-DONE = "Y"
    ...
    IF WS-ERROR = "Y"
        EXIT PERFORM         *> break out of the PERFORM loop
    END-IF
    ...
END-PERFORM
```

---

## 7. File Handling

### 7.1 Sequential Files

```cobol
ENVIRONMENT DIVISION.
INPUT-OUTPUT SECTION.
FILE-CONTROL.
    SELECT INPUT-FILE
        ASSIGN TO "input.dat"
        ORGANIZATION IS LINE SEQUENTIAL
        FILE STATUS IS WS-INPUT-STATUS.

    SELECT OUTPUT-FILE
        ASSIGN TO "output.dat"
        ORGANIZATION IS LINE SEQUENTIAL
        FILE STATUS IS WS-OUTPUT-STATUS.

DATA DIVISION.
FILE SECTION.
    FD  INPUT-FILE.
    01  INPUT-RECORD       PIC X(100).

    FD  OUTPUT-FILE.
    01  OUTPUT-RECORD      PIC X(100).

WORKING-STORAGE SECTION.
    01  WS-INPUT-STATUS    PIC XX.
        88  INPUT-OK       VALUE "00".
        88  INPUT-EOF      VALUE "10".

    01  WS-OUTPUT-STATUS   PIC XX.
        88  OUTPUT-OK      VALUE "00".

    01  WS-EOF-FLAG        PIC X VALUE "N".
        88  END-OF-FILE    VALUE "Y".

PROCEDURE DIVISION.
    OPEN INPUT  INPUT-FILE
    OPEN OUTPUT OUTPUT-FILE

    PERFORM UNTIL END-OF-FILE
        READ INPUT-FILE INTO INPUT-RECORD
            AT END
                MOVE "Y" TO WS-EOF-FLAG
            NOT AT END
                PERFORM PROCESS-RECORD
        END-READ
    END-PERFORM

    CLOSE INPUT-FILE
    CLOSE OUTPUT-FILE

    STOP RUN.

PROCESS-RECORD.
    *> do something with INPUT-RECORD
    MOVE INPUT-RECORD TO OUTPUT-RECORD
    WRITE OUTPUT-RECORD.

*> Open modes
*> OPEN INPUT    — read only
*> OPEN OUTPUT   — write only (creates or overwrites)
*> OPEN EXTEND   — append to existing file
*> OPEN I-O      — read and write (sequential: rewrite current record)
```

**File status codes (most common):**

| Status | Meaning |
|---|---|
| `00` | Success |
| `10` | End of file reached |
| `23` | Record not found (indexed) |
| `30` | Permanent I/O error |
| `35` | File not found |
| `39` | File attribute mismatch |
| `41` | File already open |
| `42` | File not open |
| `47` | READ not allowed (not opened INPUT or I-O) |
| `48` | WRITE not allowed |

---

### 7.2 Indexed Files (VSAM-style)

```cobol
ENVIRONMENT DIVISION.
FILE-CONTROL.
    SELECT CUSTOMER-FILE
        ASSIGN TO "customers"
        ORGANIZATION IS INDEXED
        ACCESS MODE IS DYNAMIC
        RECORD KEY IS CUST-ID
        ALTERNATE RECORD KEY IS CUST-EMAIL
            WITH DUPLICATES
        FILE STATUS IS WS-FILE-STATUS.

DATA DIVISION.
FILE SECTION.
    FD  CUSTOMER-FILE.
    01  CUSTOMER-RECORD.
        05  CUST-ID       PIC 9(8).
        05  CUST-NAME     PIC X(40).
        05  CUST-EMAIL    PIC X(50).
        05  CUST-BALANCE  PIC S9(9)V99 PACKED-DECIMAL.

PROCEDURE DIVISION.
*> Random read by primary key
    MOVE 12345678 TO CUST-ID
    READ CUSTOMER-FILE
        INVALID KEY
            DISPLAY "Customer not found"
        NOT INVALID KEY
            DISPLAY "Found: " CUST-NAME
    END-READ

*> Write a new record
    MOVE 99999999 TO CUST-ID
    MOVE "Bob Smith" TO CUST-NAME
    WRITE CUSTOMER-RECORD
        INVALID KEY
            DISPLAY "Duplicate key"
    END-WRITE

*> Update existing record (must READ first)
    MOVE 12345678 TO CUST-ID
    READ CUSTOMER-FILE
        INVALID KEY DISPLAY "Not found"
    END-READ
    ADD 500 TO CUST-BALANCE
    REWRITE CUSTOMER-RECORD
        INVALID KEY DISPLAY "Rewrite failed"
    END-REWRITE

*> Delete
    MOVE 12345678 TO CUST-ID
    DELETE CUSTOMER-FILE RECORD
        INVALID KEY DISPLAY "Not found"
    END-DELETE

*> Sequential read (DYNAMIC mode)
    MOVE 00000000 TO CUST-ID
    START CUSTOMER-FILE KEY >= CUST-ID
        INVALID KEY DISPLAY "Start failed"
    END-START

    PERFORM UNTIL WS-FILE-STATUS = "10"
        READ CUSTOMER-FILE NEXT
            AT END CONTINUE
            NOT AT END DISPLAY CUST-ID " " CUST-NAME
        END-READ
    END-PERFORM

    STOP RUN.
```

---

## 8. Intrinsic Functions

Called with `FUNCTION function-name(arguments)`.

### Numeric functions

```cobol
COMPUTE WS-ABS    = FUNCTION ABS(WS-VALUE)
COMPUTE WS-SQRT   = FUNCTION SQRT(WS-VALUE)
COMPUTE WS-POWER  = FUNCTION POWER(WS-BASE, WS-EXP)
COMPUTE WS-LOG    = FUNCTION LOG(WS-VALUE)
COMPUTE WS-LOG10  = FUNCTION LOG10(WS-VALUE)
COMPUTE WS-EXP    = FUNCTION EXP(WS-VALUE)
COMPUTE WS-MOD    = FUNCTION MOD(WS-DIVIDEND, WS-DIVISOR)
COMPUTE WS-REM    = FUNCTION REM(WS-DIVIDEND, WS-DIVISOR)
COMPUTE WS-INT    = FUNCTION INTEGER(WS-VALUE)        *> truncate to integer
COMPUTE WS-ROUND  = FUNCTION INTEGER-PART(WS-VALUE)
COMPUTE WS-MAX    = FUNCTION MAX(WS-A, WS-B, WS-C)
COMPUTE WS-MIN    = FUNCTION MIN(WS-A, WS-B, WS-C)
COMPUTE WS-MEAN   = FUNCTION MEAN(WS-A, WS-B, WS-C)

*> Random number (0 ≤ result < 1)
COMPUTE WS-RAND = FUNCTION RANDOM
COMPUTE WS-RAND = FUNCTION RANDOM(WS-SEED)  *> seeded
```

### String functions

```cobol
MOVE FUNCTION UPPER-CASE(WS-NAME)       TO WS-NAME
MOVE FUNCTION LOWER-CASE(WS-NAME)       TO WS-NAME
MOVE FUNCTION TRIM(WS-NAME)             TO WS-NAME          *> removes trailing spaces
MOVE FUNCTION TRIM(WS-NAME LEADING)     TO WS-NAME          *> removes leading spaces
MOVE FUNCTION REVERSE(WS-TEXT)          TO WS-REVERSED

COMPUTE WS-LEN    = FUNCTION LENGTH(WS-TEXT)
COMPUTE WS-LEN    = FUNCTION LENGTH(FUNCTION TRIM(WS-TEXT))

*> Extract a substring
MOVE WS-TEXT(3:5) TO WS-SUB            *> starting at char 3, length 5
MOVE WS-TEXT(3:)  TO WS-SUB            *> from char 3 to end

*> Search
COMPUTE WS-POS = FUNCTION FIND-STRING(WS-TEXT, WS-SEARCH)  *> GnuCOBOL extension

*> Concatenation (COBOL 2014)
MOVE FUNCTION CONCATENATE(WS-A, " ", WS-B) TO WS-RESULT
```

### Date functions

```cobol
*> Current date as integer (days since day zero — December 31, 1600)
COMPUTE WS-TODAY = FUNCTION CURRENT-DATE    *> returns 21-char string
*> Format: YYYYMMDDHHMMSSCC+HHMM

MOVE FUNCTION CURRENT-DATE(1:8)  TO WS-DATE-YYYYMMDD
MOVE FUNCTION CURRENT-DATE(9:6)  TO WS-TIME-HHMMSS

*> Date arithmetic
COMPUTE WS-DATE-INT  = FUNCTION INTEGER-OF-DATE(20241231)    *> YYYYMMDD → integer
COMPUTE WS-DATE-BACK = FUNCTION DATE-OF-INTEGER(WS-DATE-INT) *> integer → YYYYMMDD
COMPUTE WS-DAYS-DIFF = FUNCTION INTEGER-OF-DATE(20241231)
                     - FUNCTION INTEGER-OF-DATE(20240101)     *> days between dates

COMPUTE WS-DAY-OF-WEEK = FUNCTION DAY-OF-INTEGER(WS-DATE-INT)
*> 1=Monday, 2=Tuesday, ..., 7=Sunday
```

### Conversion functions

```cobol
COMPUTE WS-ORD   = FUNCTION ORD("A")          *> character → ordinal position
MOVE    (FUNCTION CHAR(66)) TO WS-CHAR        *> ordinal → character ("B")

COMPUTE WS-NUM   = FUNCTION NUMVAL("  42.5 ") *> string → numeric
COMPUTE WS-NUM   = FUNCTION NUMVAL-C("$1,234.56") *> currency string → numeric
```

---

## 9. Scope Terminators

Explicit scope terminators end a structured statement. They are required when nesting statements and when using `NOT AT END`, `NOT INVALID KEY` etc.

| Statement | Terminator |
|---|---|
| `IF` | `END-IF` |
| `EVALUATE` | `END-EVALUATE` |
| `PERFORM` | `END-PERFORM` |
| `READ` | `END-READ` |
| `WRITE` | `END-WRITE` |
| `REWRITE` | `END-REWRITE` |
| `DELETE` | `END-DELETE` |
| `START` | `END-START` |
| `ADD` | `END-ADD` |
| `SUBTRACT` | `END-SUBTRACT` |
| `MULTIPLY` | `END-MULTIPLY` |
| `DIVIDE` | `END-DIVIDE` |
| `COMPUTE` | `END-COMPUTE` |
| `STRING` | `END-STRING` |
| `UNSTRING` | `END-UNSTRING` |
| `CALL` | `END-CALL` |
| `ACCEPT` | `END-ACCEPT` |

> Always use explicit terminators in modern COBOL. The period (`.`) also terminates scope — but a misplaced period in old code ended all nested conditions, causing silent bugs that are legendary in the industry.

---

## 10. Common Patterns and Idioms

### Read-Process-Write (the COBOL heartbeat)

```cobol
PROCEDURE DIVISION.
    PERFORM INITIALIZATION
    PERFORM MAIN-LOOP UNTIL END-OF-FILE
    PERFORM FINALIZATION
    STOP RUN.

INITIALIZATION.
    OPEN INPUT  INPUT-FILE
    OPEN OUTPUT OUTPUT-FILE
    MOVE ZERO TO WS-RECORD-COUNT WS-TOTAL-AMOUNT
    READ INPUT-FILE
        AT END MOVE "Y" TO WS-EOF
    END-READ.

MAIN-LOOP.
    ADD 1 TO WS-RECORD-COUNT
    PERFORM PROCESS-RECORD
    READ INPUT-FILE
        AT END MOVE "Y" TO WS-EOF
    END-READ.

PROCESS-RECORD.
    ADD INPUT-AMOUNT TO WS-TOTAL-AMOUNT
    MOVE INPUT-RECORD TO OUTPUT-RECORD
    WRITE OUTPUT-RECORD.

FINALIZATION.
    CLOSE INPUT-FILE OUTPUT-FILE
    DISPLAY "Records processed: " WS-RECORD-COUNT
    DISPLAY "Total amount     : " WS-TOTAL-AMOUNT.
```

### Safe numeric input validation

```cobol
PERFORM UNTIL WS-VALID-INPUT = "Y"
    DISPLAY "Enter amount: " WITH NO ADVANCING
    ACCEPT WS-RAW-INPUT
    IF WS-RAW-INPUT IS NUMERIC
        MOVE WS-RAW-INPUT TO WS-AMOUNT
        MOVE "Y" TO WS-VALID-INPUT
    ELSE
        DISPLAY "Invalid input. Numbers only."
    END-IF
END-PERFORM
```

### Report accumulator pattern

```cobol
DATA DIVISION.
WORKING-STORAGE SECTION.
    01  WS-TOTALS.
        05  WS-RECORD-COUNT    PIC 9(7)  VALUE ZERO.
        05  WS-DEPT-TOTAL      PIC 9(9)V99 VALUE ZERO.
        05  WS-GRAND-TOTAL     PIC 9(11)V99 VALUE ZERO.
        05  WS-CURRENT-DEPT    PIC X(10) VALUE SPACES.
        05  WS-PREVIOUS-DEPT   PIC X(10) VALUE SPACES.

PROCEDURE DIVISION.
    PERFORM READ-FIRST-RECORD
    MOVE WS-DEPT TO WS-PREVIOUS-DEPT

    PERFORM UNTIL END-OF-FILE
        IF WS-DEPT NOT = WS-PREVIOUS-DEPT
            PERFORM PRINT-DEPT-TOTAL
            MOVE ZERO TO WS-DEPT-TOTAL
            MOVE WS-DEPT TO WS-PREVIOUS-DEPT
        END-IF

        ADD WS-AMOUNT TO WS-DEPT-TOTAL WS-GRAND-TOTAL
        ADD 1 TO WS-RECORD-COUNT
        PERFORM READ-NEXT-RECORD
    END-PERFORM

    PERFORM PRINT-DEPT-TOTAL
    PERFORM PRINT-GRAND-TOTAL.
```

---

## 11. Compiler Directives (GnuCOBOL)

```cobol
*> Source format
>>SOURCE FORMAT FIXED
>>SOURCE FORMAT FREE

*> Conditional compilation
>>IF COBOL-SYSTEM = "GNUCOBOL"
    DISPLAY "Running on GnuCOBOL"
>>END-IF

*> Turn compiler features on/off
>>DEFINE MY-FLAG AS "Y"
>>IF MY-FLAG = "Y"
    PERFORM SPECIAL-PROCESSING
>>END-IF

*> SET (used in copybook compatibility)
>>SET SOURCEFORMAT TO FREE
```

**GnuCOBOL compiler flags:**

| Flag | Meaning |
|---|---|
| `-x` | Compile to executable |
| `-m` | Compile to dynamic module (`.so`) |
| `-c` | Compile to object file only |
| `-g` | Include debug symbols |
| `-O2` | Optimization level 2 |
| `-Wall` | All warnings |
| `-free` | Free-format source |
| `-fixed` | Fixed-format source (default) |
| `-std=cobol2014` | Standard conformance |
| `-fsyntax-only` | Check syntax without producing output |
| `cobc -E` | Preprocess only (expand COPYbooks) |

---

## 12. Error Handling — ON ERROR / ON EXCEPTION

```cobol
*> Arithmetic overflow
COMPUTE WS-RESULT = WS-A * WS-B
    ON SIZE ERROR
        DISPLAY "SIZE ERROR in computation"
        PERFORM ERROR-RECOVERY
    NOT ON SIZE ERROR
        DISPLAY "Result: " WS-RESULT
END-COMPUTE

*> File errors — always check FILE STATUS
OPEN INPUT CUSTOMER-FILE
IF NOT INPUT-OK
    DISPLAY "Cannot open file, status: " WS-FILE-STATUS
    STOP RUN
END-IF

READ CUSTOMER-FILE
    AT END
        MOVE "Y" TO WS-EOF
    NOT AT END
        PERFORM PROCESS-RECORD
END-READ

IF NOT INPUT-OK AND NOT INPUT-EOF
    DISPLAY "Read error, status: " WS-FILE-STATUS
    PERFORM FILE-ERROR-HANDLING
END-IF

*> CALL exception
CALL "EXTERNAL-ROUTINE"
    USING WS-DATA
    ON EXCEPTION
        DISPLAY "Routine not available"
        PERFORM FALLBACK-PROCESSING
    NOT ON EXCEPTION
        DISPLAY "Call succeeded"
END-CALL

*> String overflow
STRING WS-A DELIMITED SIZE
       WS-B DELIMITED SIZE
    INTO WS-TARGET
    ON OVERFLOW
        DISPLAY "Target field too small"
END-STRING
```

---

## 13. Quick Reference Card

### Figurative constants

| Constant | Value |
|---|---|
| `ZERO` / `ZEROS` / `ZEROES` | 0 |
| `SPACE` / `SPACES` | `" "` |
| `HIGH-VALUE` / `HIGH-VALUES` | `x"FF"` (max collating char) |
| `LOW-VALUE` / `LOW-VALUES` | `x"00"` (min collating char) |
| `QUOTE` / `QUOTES` | `"` |
| `ALL "x"` | Repeats `x` to fill field |
| `NULL` / `NULLS` | Null pointer |

### Condition tests

```
IS NUMERIC          IS NOT NUMERIC
IS ALPHABETIC       IS NOT ALPHABETIC
IS ALPHABETIC-UPPER
IS ALPHABETIC-LOWER
IS POSITIVE         IS NOT POSITIVE
IS NEGATIVE         IS NOT NEGATIVE
IS ZERO             IS NOT ZERO
IS [NOT] = <> < > <= >=
AND  OR  NOT
```

### Data Division sections (in order)

```
FILE SECTION           — FD entries for declared files
WORKING-STORAGE SECTION — persistent program variables
LOCAL-STORAGE SECTION  — re-initialized on each CALL (like local variables)
LINKAGE SECTION        — parameters received from a caller
```

### Frequently confused things

| | |
|---|---|
| `.` period | Terminates a paragraph, a SECTION, a DIVISION header — a misplaced period ends all open scope |
| `MOVE A TO B` | B = A |
| `ADD A TO B` | B = B + A (B is changed) |
| `ADD A TO B GIVING C` | C = A + B (B is NOT changed) |
| `PERFORM P` | Call paragraph P once |
| `PERFORM P UNTIL c` | Loop: execute P until condition c is true (test BEFORE) |
| `EXIT PERFORM` | Break out of a PERFORM loop |
| `GOBACK` | Return to caller |
| `STOP RUN` | End the whole run unit |

---

---

## 🇫🇷 Français

### Table des matières

1. [Format source](#1-format-source)
2. [Structure du programme — Les quatre DIVISIONS](#2-structure-du-programme--les-quatre-divisions)
3. [IDENTIFICATION DIVISION](#3-identification-division-1)
4. [ENVIRONMENT DIVISION](#4-environment-division-1)
5. [DATA DIVISION](#5-data-division-1)
6. [PROCEDURE DIVISION](#6-procedure-division-1)
7. [Gestion des fichiers](#7-gestion-des-fichiers)
8. [Fonctions intrinsèques](#8-fonctions-intrinsèques)
9. [Terminateurs de portée](#9-terminateurs-de-portée)
10. [Patterns courants](#10-patterns-courants)
11. [Directives compilateur](#11-directives-compilateur-gnucobol)
12. [Gestion des erreurs](#12-gestion-des-erreurs)
13. [Carte de référence rapide](#13-carte-de-référence-rapide)

---

## 1. Format source

### Format fixe (traditionnel — COBOL 74/85)

```
Col 1–6   : Numéros de séquence (optionnels, ignorés par le compilateur)
Col 7     : Indicateur
              ' '  code normal
              '*'  ligne de commentaire
              '/'  saut de page
              '-'  continuation de la ligne précédente
Col 8–11  : Zone A — DIVISION, SECTION, noms de paragraphe, niveaux 01 et 77
Col 12–72 : Zone B — toutes les autres instructions
Col 73–80 : Identification (optionnel, ignoré)
```

### Format libre (moderne — COBOL 2002+)

```cobol
>>SOURCE FORMAT FREE
```

Placer cette directive en tête de fichier. Aucune restriction de colonne. `*>` est le marqueur de commentaire.

> GnuCOBOL accepte `-free` comme option : `cobc -x -free source.cbl`

---

## 2. Structure du programme — Les quatre DIVISIONS

Tout programme COBOL suit ce squelette, dans cet ordre exact :

```cobol
IDENTIFICATION DIVISION.          *> obligatoire
    PROGRAM-ID. nom-du-programme. *> obligatoire

ENVIRONMENT DIVISION.             *> optionnel
    CONFIGURATION SECTION.
    INPUT-OUTPUT SECTION.

DATA DIVISION.                    *> optionnel
    FILE SECTION.
    WORKING-STORAGE SECTION.
    LOCAL-STORAGE SECTION.
    LINKAGE SECTION.

PROCEDURE DIVISION.               *> obligatoire
    [paragraphes et sections]
    STOP RUN.
```

---

## 3. IDENTIFICATION DIVISION

```cobol
IDENTIFICATION DIVISION.
PROGRAM-ID.    CALCUL-PAIE.
AUTHOR.        Grace Hopper.
DATE-WRITTEN.  1960-04-01.
SECURITY.      CONFIDENTIEL.
```

Seul `PROGRAM-ID` est obligatoire. Les autres entrées sont informatives — conçues pour être lues par des auditeurs comptables, pas uniquement par des développeurs.

---

## 4. ENVIRONMENT DIVISION

```cobol
ENVIRONMENT DIVISION.

CONFIGURATION SECTION.
    SPECIAL-NAMES.
        DECIMAL-POINT IS COMMA.   *> virgule comme séparateur décimal (style européen)

INPUT-OUTPUT SECTION.
    FILE-CONTROL.
        SELECT FICHIER-EMPLOYES
            ASSIGN TO "employes.dat"
            ORGANIZATION IS LINE SEQUENTIAL
            FILE STATUS IS WS-STATUT-FICHIER.
```

### Organisations de fichiers

| Organisation | Description |
|---|---|
| `LINE SEQUENTIAL` | Fichier texte, enregistrements séparés par saut de ligne |
| `SEQUENTIAL` | Séquentiel binaire, enregistrements de longueur fixe |
| `INDEXED` | Accès par clé (VSAM sur mainframes) |
| `RELATIVE` | Accès par numéro d'enregistrement |

---

## 5. DATA DIVISION

### Numéros de niveau

Les numéros de niveau définissent la hiérarchie des données. Pense-y comme une imbrication.

```cobol
DATA DIVISION.
WORKING-STORAGE SECTION.

    01  WS-EMPLOYE.
        05  WS-NOM.
            10  WS-PRENOM        PIC X(20).
            10  WS-NOM-FAMILLE   PIC X(20).
        05  WS-ID-EMP            PIC 9(6).
        05  WS-SALAIRE           PIC 9(7)V99.

    77  WS-COMPTEUR              PIC 9(4) VALUE ZERO.

    01  WS-STATUT                PIC X.
        88  STATUT-ACTIF         VALUE "A".
        88  STATUT-INACTIF       VALUE "I".
```

**Règles des numéros de niveau :**
- `01` et `77` doivent être en Zone A
- `02`–`49` doivent être strictement croissants dans un groupe (les espaces sont permis : 01, 05, 10 est idiomatique)
- `66`, `77`, `88` sont spéciaux — hors de la hiérarchie 01–49

### Clause PICTURE (PIC)

| Symbole | Signification |
|---|---|
| `9` | Chiffre numérique (0–9) |
| `A` | Caractère alphabétique (A–Z, a–z, espace) |
| `X` | Alphanumérique (tout caractère) |
| `V` | Point décimal implicite (non stocké) |
| `S` | Signe (positif ou négatif) — doit être le plus à gauche |

```cobol
PIC 9(6)      *> 6 chiffres entiers
PIC X(50)     *> 50 caractères alphanumériques
PIC 9(7)V99   *> 7 chiffres entiers, 2 décimales
PIC S9(5)V99  *> signé, 5 entiers, 2 décimales

*> Images éditées — affichage uniquement
01  WS-MONTANT-AFFICHE   PIC $,$$$,$$9.99.
01  WS-DATE-AFFICHEE     PIC 99/99/9999.
01  WS-ZERO-SUPPRIME     PIC ZZZ,ZZ9.99.   *> supprime les zéros de tête
01  WS-CHEQUE            PIC ***,**9.99.   *> remplissage par astérisques
```

### Clause VALUE

```cobol
01  WS-NOM          PIC X(20)  VALUE "Alice".
01  WS-COMPTEUR     PIC 9(4)   VALUE ZERO.
01  WS-FLAG         PIC X      VALUE SPACE.
01  WS-HAUT         PIC X(5)   VALUE HIGH-VALUES.
01  WS-BAS          PIC X(5)   VALUE LOW-VALUES.
```

### REDEFINES

```cobol
01  WS-DATE-NUM         PIC 9(8).          *> 20241231
01  WS-DATE-PARTIES     REDEFINES WS-DATE-NUM.
    05  WS-ANNEE        PIC 9(4).          *> 2024
    05  WS-MOIS         PIC 9(2).          *> 12
    05  WS-JOUR         PIC 9(2).          *> 31
```

### OCCURS — Tableaux

```cobol
*> Tableau de taille fixe
01  WS-NOTES.
    05  WS-NOTE          PIC 9(3) OCCURS 10 TIMES.

MOVE 95 TO WS-NOTE(1)   *> accès en base 1

*> Tableau indexé
01  WS-TABLE.
    05  WS-ENTREE        PIC X(20) OCCURS 100 TIMES
                         INDEXED BY WS-IDX.

SET WS-IDX TO 1
MOVE "Alice" TO WS-ENTREE(WS-IDX)

*> Tableau à taille variable
01  WS-TABLE-DYN.
    05  WS-NB-ITEMS      PIC 9(4).
    05  WS-ITEMS         PIC X(30) OCCURS 1 TO 200 TIMES
                         DEPENDING ON WS-NB-ITEMS.

*> Tableau à deux dimensions
01  WS-MATRICE.
    05  WS-LIGNE         OCCURS 5 TIMES.
        10  WS-COL       PIC 9(4) OCCURS 5 TIMES.

MOVE 42 TO WS-COL(3, 4)
```

### Niveaux 88 — Noms de conditions

```cobol
01  WS-CODE-REPONSE      PIC 9(3).
    88  SUCCES            VALUE 200.
    88  CREE              VALUE 201.
    88  NON-TROUVE        VALUE 404.
    88  ERREUR-SERVEUR    VALUE 500 THRU 599.

MOVE 404 TO WS-CODE-REPONSE
IF NON-TROUVE
    DISPLAY "Ressource introuvable"
END-IF

*> Activer une condition par SET
SET SUCCES TO TRUE       *> équivaut à MOVE 200 TO WS-CODE-REPONSE
```

---

## 6. PROCEDURE DIVISION

### MOVE

```cobol
MOVE "Alice"       TO WS-NOM
MOVE 42            TO WS-COMPTEUR
MOVE ZERO          TO WS-TOTAL WS-SOUS-TOTAL WS-MOYENNE
MOVE CORRESPONDING WS-SOURCE TO WS-CIBLE
```

**Règles importantes :**
- Alphanumérique → Alphanumérique : cadré à gauche, complété par des espaces à droite
- Numérique → Numérique : aligné par la virgule décimale, tronqué ou complété par des zéros
- Groupe → Groupe : copie brute d'octets (traité comme alphanumérique)

### Arithmétique

```cobol
ADD 1              TO WS-COMPTEUR
ADD WS-A TO WS-B  GIVING WS-RESULTAT    *> RESULTAT = A + B (B inchangé)
SUBTRACT WS-TAX   FROM WS-BRUT GIVING WS-NET
MULTIPLY WS-HEURES BY WS-TAUX GIVING WS-MONTANT
DIVIDE WS-TOTAL   BY WS-NB GIVING WS-MOYENNE REMAINDER WS-RESTE

*> COMPUTE — syntaxe d'expression
COMPUTE WS-RESULTAT = (WS-A + WS-B) * WS-C ROUNDED
COMPUTE WS-AIRE     = WS-PI * WS-RAYON ** 2
    ON SIZE ERROR
        DISPLAY "Dépassement de capacité"
END-COMPUTE
```

### IF / ELSE / END-IF

```cobol
IF WS-AGE >= 18
    DISPLAY "Adulte"
ELSE IF WS-AGE >= 13
    DISPLAY "Adolescent"
ELSE
    DISPLAY "Enfant"
END-IF

IF WS-STATUT = "A" AND WS-SOLDE > ZERO
    PERFORM TRAITER-COMPTE-ACTIF
END-IF

*> Tests de classe
IF WS-SAISIE IS NUMERIC
    MOVE WS-SAISIE TO WS-MONTANT
END-IF

*> Tests de signe
IF WS-SOLDE IS NEGATIVE
    DISPLAY "Compte à découvert"
END-IF
```

### EVALUATE

```cobol
EVALUATE WS-NOTE
    WHEN 90 THRU 100  DISPLAY "Très bien"
    WHEN 80 THRU 89   DISPLAY "Bien"
    WHEN 70 THRU 79   DISPLAY "Assez bien"
    WHEN OTHER        DISPLAY "Insuffisant"
END-EVALUATE

*> EVALUATE TRUE — remplace une chaîne IF/ELSE
EVALUATE TRUE
    WHEN WS-STATUT = "A" AND WS-SOLDE > ZERO
        PERFORM TRAITER-ACTIF
    WHEN WS-STATUT = "S"
        PERFORM TRAITER-SUSPENDU
    WHEN OTHER
        PERFORM TRAITEMENT-PAR-DEFAUT
END-EVALUATE

*> Correspondance de tuples avec ALSO
EVALUATE WS-TYPE ALSO WS-STATUT
    WHEN "CORP" ALSO "A"   PERFORM CORP-ACTIF
    WHEN "CORP" ALSO "I"   PERFORM CORP-INACTIF
    WHEN "INDV" ALSO ANY   PERFORM INDIVIDUEL
    WHEN OTHER             PERFORM INCONNU
END-EVALUATE
```

### PERFORM

```cobol
*> Appel de paragraphe
PERFORM AFFICHER-EN-TETE

*> n fois
PERFORM 5 TIMES
    ADD 1 TO WS-COMPTEUR
END-PERFORM

*> UNTIL (condition testée avant)
PERFORM UNTIL WS-FIN-FICHIER = "O"
    READ FICHIER-EMPLOYES
        AT END MOVE "O" TO WS-FIN-FICHIER
        NOT AT END PERFORM TRAITER-ENREGISTREMENT
    END-READ
END-PERFORM

*> VARYING — boucle indexée
PERFORM VARYING WS-IDX FROM 1 BY 1
    UNTIL WS-IDX > WS-MAX
    PERFORM TRAITER-ELEMENT
END-PERFORM

*> EXIT PERFORM — sortie de boucle
PERFORM UNTIL EXIT
    ACCEPT WS-COMMANDE
    IF WS-COMMANDE = "Q" EXIT PERFORM END-IF
    PERFORM EXECUTER-COMMANDE
END-PERFORM
```

### ACCEPT / DISPLAY

```cobol
DISPLAY "Bonjour, Monde !"
DISPLAY "Nom : " WS-NOM
DISPLAY "Saisir un nombre : " WITH NO ADVANCING
ACCEPT WS-SAISIE

ACCEPT WS-DATE FROM DATE YYYYMMDD    *> date du jour
ACCEPT WS-HEURE FROM TIME            *> HHMMSSss
```

### STRING / UNSTRING

```cobol
*> Concaténation
MOVE SPACES TO WS-NOM-COMPLET
STRING WS-PRENOM  DELIMITED SPACE
       " "        DELIMITED SIZE
       WS-NOM-FAM DELIMITED SPACE
    INTO WS-NOM-COMPLET
    ON OVERFLOW DISPLAY "Champ trop petit"
END-STRING

*> Découpage
MOVE "Alice,30,Paris" TO WS-SAISIE
UNSTRING WS-SAISIE
    DELIMITED BY ","
    INTO WS-NOM WS-AGE WS-VILLE
    TALLYING IN WS-NB-CHAMPS
END-UNSTRING
```

### INSPECT

```cobol
*> Compter des caractères
INSPECT WS-TEXTE TALLYING WS-NB FOR ALL ","

*> Remplacer des caractères
INSPECT WS-TEXTE REPLACING ALL "," BY ";"
INSPECT WS-TEXTE REPLACING LEADING SPACES BY ZEROS

*> Convertir (comme tr)
INSPECT WS-TEXTE CONVERTING
    "abcdefghijklmnopqrstuvwxyz"
    TO
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
```

### CALL — Sous-programmes

```cobol
CALL "CALC-IMPOT"
    USING BY REFERENCE WS-BRUT
          BY REFERENCE WS-IMPOT
          BY VALUE     WS-TAUX
    ON EXCEPTION
        DISPLAY "Routine introuvable"
END-CALL
```

**Squelette du sous-programme appelé :**

```cobol
IDENTIFICATION DIVISION.
PROGRAM-ID. CALC-IMPOT.

DATA DIVISION.
LINKAGE SECTION.
    01  LS-BRUT      PIC 9(7)V99.
    01  LS-IMPOT     PIC 9(7)V99.
    01  LS-TAUX      PIC V999.

PROCEDURE DIVISION USING LS-BRUT LS-IMPOT LS-TAUX.
    COMPUTE LS-IMPOT = LS-BRUT * LS-TAUX
    GOBACK.
```

---

## 7. Gestion des fichiers

### Fichiers séquentiels

```cobol
*> Modes d'ouverture
OPEN INPUT   FICHIER-ENTREE    *> lecture seule
OPEN OUTPUT  FICHIER-SORTIE    *> écriture (crée ou écrase)
OPEN EXTEND  FICHIER-SORTIE    *> ajout en fin de fichier
OPEN I-O     FICHIER-PRINCIPAL *> lecture et écriture

*> Lecture avec gestion de fin de fichier
PERFORM UNTIL FIN-FICHIER
    READ FICHIER-ENTREE INTO WS-ENREG
        AT END
            MOVE "O" TO WS-FIN
        NOT AT END
            PERFORM TRAITER-ENREGISTREMENT
    END-READ
END-PERFORM

CLOSE FICHIER-ENTREE FICHIER-SORTIE
```

**Codes de statut fichier courants :**

| Statut | Signification |
|---|---|
| `00` | Succès |
| `10` | Fin de fichier atteinte |
| `23` | Enregistrement non trouvé (indexé) |
| `35` | Fichier non trouvé |
| `41` | Fichier déjà ouvert |
| `42` | Fichier non ouvert |

---

## 8. Fonctions intrinsèques

```cobol
*> Numériques
COMPUTE WS-ABS   = FUNCTION ABS(WS-VALEUR)
COMPUTE WS-RACINE = FUNCTION SQRT(WS-VALEUR)
COMPUTE WS-MAX   = FUNCTION MAX(WS-A, WS-B, WS-C)
COMPUTE WS-MOD   = FUNCTION MOD(WS-DIVIDENDE, WS-DIVISEUR)
COMPUTE WS-ALEA  = FUNCTION RANDOM

*> Chaînes
MOVE FUNCTION UPPER-CASE(WS-NOM)    TO WS-NOM
MOVE FUNCTION LOWER-CASE(WS-NOM)    TO WS-NOM
MOVE FUNCTION TRIM(WS-NOM)          TO WS-NOM
MOVE FUNCTION REVERSE(WS-TEXTE)     TO WS-INVERSE
COMPUTE WS-LEN = FUNCTION LENGTH(FUNCTION TRIM(WS-TEXTE))

*> Extraction de sous-chaîne
MOVE WS-TEXTE(3:5) TO WS-SOUS-CHAINE   *> depuis la position 3, longueur 5

*> Dates
MOVE FUNCTION CURRENT-DATE(1:8) TO WS-DATE-AUJDHUI  *> AAAAMMJJ
COMPUTE WS-ECART = FUNCTION INTEGER-OF-DATE(20241231)
                 - FUNCTION INTEGER-OF-DATE(20240101)

*> Conversion
COMPUTE WS-NUM = FUNCTION NUMVAL("  42.5 ")           *> chaîne → numérique
```

---

## 9. Terminateurs de portée

| Instruction | Terminateur |
|---|---|
| `IF` | `END-IF` |
| `EVALUATE` | `END-EVALUATE` |
| `PERFORM` | `END-PERFORM` |
| `READ` | `END-READ` |
| `WRITE` | `END-WRITE` |
| `COMPUTE` | `END-COMPUTE` |
| `ADD` | `END-ADD` |
| `STRING` | `END-STRING` |
| `UNSTRING` | `END-UNSTRING` |
| `CALL` | `END-CALL` |

> Utilise toujours les terminateurs explicites. Un point (`.`) mal placé dans du COBOL ancien terminait toutes les portées imbriquées — source de bugs silencieux légendaires dans l'industrie.

---

## 10. Patterns courants

### Lire-Traiter-Écrire (le rythme cardiaque du COBOL)

```cobol
PROCEDURE DIVISION.
    PERFORM INITIALISATION
    PERFORM BOUCLE-PRINCIPALE UNTIL FIN-FICHIER
    PERFORM FINALISATION
    STOP RUN.

INITIALISATION.
    OPEN INPUT FICHIER-ENTREE
    OPEN OUTPUT FICHIER-SORTIE
    MOVE ZERO TO WS-NB-ENREG WS-TOTAL
    READ FICHIER-ENTREE AT END MOVE "O" TO WS-FIN END-READ.

BOUCLE-PRINCIPALE.
    ADD 1 TO WS-NB-ENREG
    PERFORM TRAITER-ENREGISTREMENT
    READ FICHIER-ENTREE
        AT END MOVE "O" TO WS-FIN
    END-READ.

FINALISATION.
    CLOSE FICHIER-ENTREE FICHIER-SORTIE
    DISPLAY "Enregistrements traités : " WS-NB-ENREG
    DISPLAY "Total                   : " WS-TOTAL.
```

---

## 11. Directives compilateur (GnuCOBOL)

```cobol
>>SOURCE FORMAT FREE
>>SOURCE FORMAT FIXED

>>DEFINE MON-FLAG AS "O"
>>IF MON-FLAG = "O"
    PERFORM TRAITEMENT-SPECIAL
>>END-IF
```

**Options courantes du compilateur :**

| Option | Signification |
|---|---|
| `-x` | Compiler en exécutable |
| `-g` | Inclure les symboles de débogage |
| `-Wall` | Tous les avertissements |
| `-free` | Source en format libre |
| `-fsyntax-only` | Vérifier la syntaxe sans produire de sortie |

---

## 12. Gestion des erreurs

```cobol
*> Dépassement arithmétique
COMPUTE WS-RESULTAT = WS-A * WS-B
    ON SIZE ERROR
        DISPLAY "Dépassement de capacité"
        PERFORM RECUPERATION-ERREUR
    NOT ON SIZE ERROR
        DISPLAY "Résultat : " WS-RESULTAT
END-COMPUTE

*> Erreurs fichier — toujours vérifier le FILE STATUS
OPEN INPUT FICHIER-CLIENTS
IF NOT OUVERTURE-OK
    DISPLAY "Impossible d'ouvrir le fichier : " WS-STATUT
    STOP RUN
END-IF

*> Exception CALL
CALL "ROUTINE-EXTERNE"
    USING WS-DONNEES
    ON EXCEPTION
        DISPLAY "Routine non disponible"
        PERFORM TRAITEMENT-REPLI
END-CALL
```

---

## 13. Carte de référence rapide

### Constantes figuratives

| Constante | Valeur |
|---|---|
| `ZERO` / `ZEROS` / `ZEROES` | 0 |
| `SPACE` / `SPACES` | `" "` |
| `HIGH-VALUE` / `HIGH-VALUES` | `x"FF"` (caractère le plus haut) |
| `LOW-VALUE` / `LOW-VALUES` | `x"00"` (caractère le plus bas) |
| `QUOTE` / `QUOTES` | `"` |
| `ALL "x"` | Répète `x` pour remplir le champ |

### Tests de condition

```
IS NUMERIC          IS NOT NUMERIC
IS ALPHABETIC       IS NOT ALPHABETIC
IS POSITIVE         IS NOT POSITIVE
IS NEGATIVE         IS NOT NEGATIVE
IS ZERO             IS NOT ZERO
= <> < > <= >=
AND  OR  NOT
```

### Ce qu'on confond souvent

| | |
|---|---|
| `MOVE A TO B` | B = A |
| `ADD A TO B` | B = B + A (B est modifié) |
| `ADD A TO B GIVING C` | C = A + B (B est **inchangé**) |
| `PERFORM P` | Appelle le paragraphe P une fois |
| `PERFORM P UNTIL c` | Boucle : exécute P jusqu'à ce que c soit vrai (test avant) |
| `EXIT PERFORM` | Sortir d'une boucle PERFORM |
| `GOBACK` | Retourner à l'appelant |
| `STOP RUN` | Terminer toute l'unité d'exécution |
| `.` point | Termine un paragraphe — un point mal placé ferme toutes les portées ouvertes |
