' ============================================================
' HELLO WORLD — QB64 / QB64-PE sample project
' Build : Ctrl+Shift+B
' Run   : Ctrl+F5  (or: ./bin/hello)
' ============================================================

' QB64 is modern BASIC — no line numbers needed in free-form style
' Use them only when GOTO/GOSUB targets require it (avoid when possible)

OPTION _EXPLICIT   ' equivalent of "implicit none" — forces variable declaration

' ── Variable declarations ────────────────────────────────────
DIM userName  AS STRING
DIM counter   AS INTEGER
DIM radius    AS DOUBLE
DIM area      AS DOUBLE
DIM pi        AS DOUBLE

pi = 4 * ATN(1)   ' pi = atan(1) * 4 — classic BASIC trick for exact pi

' ── Banner ──────────────────────────────────────────────────
CALL PrintBanner

' ── Interactive greeting ─────────────────────────────────────
PRINT "Enter your name: ";
LINE INPUT userName          ' LINE INPUT reads the whole line including spaces

FOR counter = 1 TO 3
    PRINT counter; ". Hello, "; TRIM$(userName); "!"
NEXT counter

PRINT ""   ' blank line

' ── A taste of what BASIC can do: simple maths ───────────────
INPUT "Enter a circle radius: ", radius

area = pi * radius ^ 2

PRINT USING "Area of circle with radius ##.## = ####.####"; radius; area

PRINT ""
PRINT "Welcome to BASIC. The language that started it all."

END

' ── Subroutines ──────────────────────────────────────────────

SUB PrintBanner
    PRINT "============================================"
    PRINT "  BASIC Hello World — CodeSamples Project  "
    PRINT "============================================"
    PRINT ""
END SUB
