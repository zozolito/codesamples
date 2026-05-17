# C Language Reference / Référence du langage C

> **Bilingual reference — English 🇬🇧 / Français 🇫🇷**
> Companion to the `hello-world/` and `bare-metal/` example projects.

---

## Table of contents / Table des matières

1. [Standards & history](#1--standards--history)
2. [Compilation](#2--compilation)
3. [Data types & sizes](#3--data-types--sizes)
4. [Variables, constants & scope](#4--variables-constants--scope)
5. [Operators](#5--operators)
6. [Control flow](#6--control-flow)
7. [Functions & function pointers](#7--functions--function-pointers)
8. [Pointers ⚠️](#8--pointers-)
9. [Arrays & strings](#9--arrays--strings)
10. [Memory management ⚠️](#10--memory-management-)
11. [Structs, unions, enums & bitfields](#11--structs-unions-enums--bitfields)
12. [Preprocessor](#12--preprocessor)
13. [File I/O](#13--file-io)
14. [Standard library overview](#14--standard-library-overview)
15. [Error handling](#15--error-handling)
16. [Security pitfalls ⚠️](#16--security-pitfalls-)
17. [Compiler flags & tools](#17--compiler-flags--tools)
18. [Quick reference card](#18--quick-reference-card)

---

## 1 · Standards & history

### EN — C standards timeline

| Standard | Year | Key additions |
|---|---|---|
| **K&R C** | 1978 | *The C Programming Language* by Kernighan & Ritchie — the original |
| **C89 / ANSI C** | 1989 | First ANSI standard; function prototypes, `void`, `const` |
| **C90** | 1990 | ISO adoption of C89 — identical content |
| **C99** | 1999 | `//` comments, `<stdint.h>`, `<stdbool.h>`, VLAs, `inline`, `restrict`, designated initialisers |
| **C11** | 2011 | `_Generic`, `_Static_assert`, `_Atomic`, threads (`<threads.h>`), anonymous structs/unions |
| **C17** | 2018 | Bug-fix release of C11; no new features |
| **C23** | 2023 | `bool`/`true`/`false` as keywords, `typeof`, `#embed`, `[[attributes]]`, `nullptr` |

> **Target C11 minimum** for new projects. C99 is the lowest acceptable standard for embedded work.
> C89 should only appear in legacy codebases.

### FR — C is the language of operating systems, kernels, compilers, and embedded firmware.
> Everything that runs "close to the metal" is written in C.
> Its power is its direct access to memory. Its danger is exactly the same.

---

## 2 · Compilation

```bash
# Minimal (never use for real work)
gcc hello.c -o hello

# Recommended development build
gcc -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 \
    -g -fsanitize=address,undefined \
    -o hello hello.c

# Release build
gcc -std=c11 -O2 -DNDEBUG -Wall -o hello hello.c

# Multiple source files
gcc -std=c11 -Wall -g -c src/main.c -o obj/main.o
gcc -std=c11 -Wall -g -c src/utils.c -o obj/utils.o
gcc -o hello obj/main.o obj/utils.o

# clang (better error messages, same flags)
clang -std=c11 -Wall -Wextra -g -fsanitize=address,undefined -o hello hello.c
```

| Flag | Effect |
|---|---|
| `-std=c11` | Use C11 standard |
| `-Wall` | Enable common warnings |
| `-Wextra` | Extra warnings (unused params, sign compare…) |
| `-Wpedantic` | Strict ISO compliance |
| `-Wshadow` | Warn when local variable shadows outer variable |
| `-Wformat=2` | Strict format string checking (catches printf bugs) |
| `-g` | Debug symbols for GDB |
| `-O0` | No optimisation (default with `-g`) |
| `-O2` | Speed optimisation |
| `-DNDEBUG` | Disables `assert()` in release |
| `-fsanitize=address` | AddressSanitizer — runtime memory error detection |
| `-fsanitize=undefined` | UBSan — catches undefined behaviour |
| `-fstack-protector-strong` | Stack canary (detects stack overflows) |

---

## 3 · Data types & sizes

```c
/* Standard types — sizes are platform-dependent (!) */
char          c  = 'A';      /* at least 8 bits; signed or unsigned — platform-dependent */
signed char   sc = -1;       /* always signed,   8 bits */
unsigned char uc = 255;      /* always unsigned, 8 bits */
short         s  = 32767;    /* at least 16 bits */
int           i  = 42;       /* at least 16 bits, typically 32 */
long          l  = 1000000L; /* at least 32 bits */
long long     ll = 9e18;     /* at least 64 bits (C99) */
float         f  = 3.14f;    /* IEEE 754 single: ~7 significant digits */
double        d  = 3.14;     /* IEEE 754 double: ~15 significant digits */
long double   ld = 3.14L;    /* 80-bit extended on x86; 64-bit on some platforms */
_Bool         b  = 1;        /* C99: 0 or 1; use stdbool.h for bool/true/false */

/* Fixed-width integers (C99) — use these for portable code */
#include <stdint.h>
int8_t   i8  = -128;         /* exactly 8-bit signed */
uint8_t  u8  = 255;          /* exactly 8-bit unsigned */
int16_t  i16 = -32768;
uint16_t u16 = 65535;
int32_t  i32 = 0;
uint32_t u32 = 0xFFFFFFFF;
int64_t  i64 = 0;
uint64_t u64 = 0xFFFFFFFFFFFFFFFFULL;

/* Size types */
size_t   sz  = sizeof(int);  /* result of sizeof — unsigned, platform-sized */
ptrdiff_t pd = ptr2 - ptr1;  /* difference between pointers — signed */
intptr_t ip  = (intptr_t)ptr; /* pointer stored as integer */

/* Check sizes at compile time (C11) */
_Static_assert(sizeof(int) == 4, "int must be 32-bit on this platform");

/* printf format specifiers for fixed-width types */
#include <inttypes.h>
printf("%" PRId32 "\n", i32);   /* PRId32 = "d" on 32-bit, same result everywhere */
printf("%" PRIu64 "\n", u64);
```

### Type size summary (typical x86-64 Linux/Windows)

| Type | Size | Range |
|---|---|---|
| `char` | 1 byte | -128…127 (signed) / 0…255 (unsigned) |
| `short` | 2 bytes | -32768…32767 |
| `int` | 4 bytes | -2,147,483,648…2,147,483,647 |
| `long` | 4 bytes (Windows) / 8 bytes (Linux) | platform-dependent |
| `long long` | 8 bytes | ±9.2×10¹⁸ |
| `float` | 4 bytes | ~±3.4×10³⁸ |
| `double` | 8 bytes | ~±1.8×10³⁰⁸ |
| pointer | 8 bytes (64-bit) | all of virtual address space |

> **Lesson:** Never assume the size of `int` or `long`. Always use `<stdint.h>` types for portable code.

---

## 4 · Variables, constants & scope

```c
/* Declaration vs definition */
int x;          /* declaration + definition (storage allocated) */
extern int y;   /* declaration only — defined in another file */

/* Initialisation — ALWAYS initialise your variables in C */
int n = 0;          /* good */
int m;              /* bad — contains garbage (indeterminate value) */
/* *m = 5;          ← undefined behaviour */

/* const — makes a variable read-only */
const int MAX = 100;        /* cannot be modified after initialisation */
const char* s = "hello";    /* pointer to const char — cannot modify *s */
char* const p = buf;        /* const pointer — cannot modify p itself */
const char* const q = "hi"; /* both const */

/* Storage class specifiers */
static int counter = 0;     /* static local: retains value between function calls */
                            /* static global: internal linkage (only this .c file) */
register int i = 0;         /* hint to compiler: keep in CPU register (mostly ignored today) */
volatile int hw_reg;        /* tells compiler: don't optimise reads/writes (hardware registers) */
extern int shared;          /* defined elsewhere */

/* Scope */
int global_var = 1;         /* file scope — visible from declaration to end of file */

void func(void) {
    int local = 2;          /* block scope — visible from declaration to closing } */
    {
        int inner = 3;      /* nested block scope */
        /* global_var, local, inner all visible here */
    }
    /* inner is gone here — accessing it is UB */
}

/* Linkage */
static void internal(void) {}     /* internal linkage — not visible to other .c files */
void        external(void) {}     /* external linkage — visible to other .c files */
```

---

## 5 · Operators

```c
/* Arithmetic */
+  -  *  /  %        /* standard; / truncates for integers (5/2 = 2, not 2.5) */
++i  i++             /* pre-increment (returns new value) / post-increment (returns old) */
--i  i--

/* Comparison — return int 0 (false) or 1 (true) */
==  !=  <  >  <=  >=

/* Logical — short-circuit evaluation */
&&   /* AND: right side only evaluated if left is true */
||   /* OR:  right side only evaluated if left is false */
!    /* NOT */

/* Bitwise — operate on individual bits */
&    /* AND:  1010 & 1100 = 1000 */
|    /* OR:   1010 | 1100 = 1110 */
^    /* XOR:  1010 ^ 1100 = 0110 */
~    /* NOT (bitwise complement) */
<<   /* left shift:  x << n = x * 2^n  (watch for overflow on signed types!) */
>>   /* right shift: x >> n = x / 2^n  (arithmetic or logical — implementation-defined for signed) */

/* Assignment */
=   +=  -=  *=  /=  %=  &=  |=  ^=  <<=  >>=

/* sizeof — compile-time size in bytes */
sizeof(int)         /* 4 on most platforms */
sizeof x            /* no parentheses needed for expressions */
sizeof(char[100])   /* 100 */

/* Ternary / conditional */
int abs_x = x >= 0 ? x : -x;   /* condition ? if_true : if_false */

/* Comma operator (rare) */
for (int i = 0, j = 10; i < j; i++, j--) { }

/* Pointer operators */
&x      /* address-of: returns pointer to x */
*ptr    /* dereference: value at address ptr */
ptr->member  /* equivalent to (*ptr).member for structs */

/* Cast */
double d = (double)5 / 2;   /* 2.5 — without cast: 5/2 = 2 (integer division) */
```

---

## 6 · Control flow

```c
/* if / else if / else */
if (x > 0) {
    printf("positive\n");
} else if (x < 0) {
    printf("negative\n");
} else {
    printf("zero\n");
}

/* switch — only works with integer types */
switch (day) {
    case 1: printf("Monday\n");   break;  /* break is REQUIRED — otherwise falls through */
    case 2: printf("Tuesday\n");  break;
    case 6:
    case 7: printf("Weekend\n");  break;  /* intentional fallthrough (two cases, one action) */
    default: printf("Unknown\n"); break;
}

/* while */
while (condition) {
    /* ... */
}

/* do-while — body executes at least once */
do {
    /* ... */
} while (condition);

/* for */
for (int i = 0; i < n; i++) {  /* C99: declaration inside for loop */
    /* ... */
    continue;   /* skip rest of loop body, go to i++ */
    break;      /* exit the loop entirely */
}

/* goto — acceptable in C for error cleanup in functions (rare but legitimate) */
int result = process_file();
if (result < 0) goto cleanup;
/* ... */
cleanup:
    free(buffer);
    fclose(file);
    return result;
```

---

## 7 · Functions & function pointers

```c
/* Function declaration (prototype) — must appear before first use */
int add(int a, int b);   /* prototype */
void print_message(const char* msg);

/* Function definition */
int add(int a, int b) {
    return a + b;
}

/* Parameters are passed BY VALUE — modifications don't affect the caller */
void double_it(int x) { x *= 2; }    /* caller's value unchanged */
void double_ptr(int* x) { *x *= 2; } /* pointer: CAN modify caller's variable */

/* Variadic functions (variable arguments) */
#include <stdarg.h>
void my_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);      /* initialise; last named param is 'fmt' */
    vprintf(fmt, args);       /* hand off to vprintf */
    va_end(args);             /* clean up */
}

/* Inline functions (C99) */
static inline int max(int a, int b) { return a > b ? a : b; }

/* Function pointers — functions are data in C */
/* Syntax: return_type (*pointer_name)(param_types) */
int (*fp)(int, int);         /* pointer to function taking 2 ints, returning int */
fp = add;                    /* assign function to pointer */
int result = fp(3, 4);       /* call through pointer */
result = (*fp)(3, 4);        /* equivalent — dereference syntax */

/* Typedef for cleaner syntax */
typedef int (*BinaryOp)(int, int);
BinaryOp operations[] = { add, subtract, multiply };
operations[0](5, 3);     /* calls add(5, 3) */

/* Callback pattern */
void apply(int* arr, int n, void (*transform)(int*)) {
    for (int i = 0; i < n; i++) transform(&arr[i]);
}

void double_elem(int* x) { *x *= 2; }
apply(arr, 10, double_elem);   /* pass function as argument */

/* qsort uses a comparison callback */
int compare_ints(const void* a, const void* b) {
    return (*(const int*)a) - (*(const int*)b);
}
qsort(arr, n, sizeof(int), compare_ints);
```

---

## 8 · Pointers ⚠️

> **EN — Pointers are the most powerful and dangerous feature of C.**
> A correct C program has no undefined behaviour from pointer misuse.
> Most CVEs in C software trace back to a pointer error.

> **FR — Les pointeurs sont la fonctionnalité la plus puissante et la plus dangereuse du C.**
> La plupart des CVE dans les logiciels C remontent à une erreur de pointeur.

```c
/* Declaration and initialisation */
int  x   = 42;
int* ptr = &x;     /* ptr holds the ADDRESS of x; & = "address-of" operator */
int  val = *ptr;   /* dereference: val = value AT the address ptr holds = 42 */

/* CRITICAL: always initialise pointers */
int* good = NULL;  /* explicit null — safe, will crash (not silently corrupt) on deref */
int* bad;          /* uninitialised — contains garbage address — NEVER USE */

/* Null check before dereference */
if (ptr != NULL) {
    *ptr = 99;     /* safe */
}

/* Pointer arithmetic — valid ONLY within an array (including one past the end) */
int arr[5] = {10, 20, 30, 40, 50};
int* p = arr;          /* arr decays to &arr[0] */
*(p + 2) == 30;        /* true — pointer + n advances by n × sizeof(int) bytes */
p++;                   /* advance to next int (adds sizeof(int) = 4 bytes to address) */

/* Common pointer errors / Erreurs courantes */

/* 1. Dangling pointer — pointer to freed or out-of-scope memory */
int* dangle;
{
    int local = 5;
    dangle = &local;
}   /* local destroyed — dangle is now dangling */
/* *dangle = 10; */ /* ← UNDEFINED BEHAVIOUR — stack corruption */

/* 2. Null dereference */
int* np = NULL;
/* *np = 5; */  /* ← SEGFAULT */

/* 3. Buffer overflow via pointer arithmetic */
int arr2[3];
int* beyond = arr2 + 4; /* outside the array */
/* *beyond = 5; */  /* ← UNDEFINED BEHAVIOUR */

/* 4. Type aliasing violation — illegal punning */
float f = 3.14f;
int*  bad_cast = (int*)&f;
/* *bad_cast = 5; */  /* ← strict aliasing violation (UB) */
/* Correct way: use memcpy or a union */

/* Pointer to pointer */
char* str = "hello";
char** pp = &str;   /* pointer to the pointer */
**pp;               /* dereference twice — gets 'h' */

/* void pointer — generic pointer, must be cast before use */
void* vp = malloc(100);
int*  ip = (int*)vp;   /* explicit cast required in C++ but optional in C */
*ip = 42;

/* const correctness with pointers */
const int* cptr = &x;    /* cannot modify *cptr (data is const) */
int* const pcon = &x;    /* cannot modify pcon (pointer is const) */
/* pass read-only data: */ void read_only(const int* data, size_t n);

/* restrict keyword (C99) — promise to compiler: no other pointer aliases this */
void vec_add(float* restrict a, const float* restrict b, size_t n) {
    for (size_t i = 0; i < n; i++) a[i] += b[i];
    /* Compiler can vectorise aggressively — a and b guaranteed non-overlapping */
}
```

---

## 9 · Arrays & strings

```c
/* Arrays */
int  arr[5] = {1, 2, 3, 4, 5};
int  zeros[100] = {0};        /* all zeros — partial init zero-fills the rest */
int  sized[] = {1, 2, 3};     /* size deduced from initialiser — 3 elements */

arr[0];       /* access — 0-indexed */
/* arr[5];    ← BUFFER OVERFLOW — index 5 is out of bounds (0–4 valid) */

/* sizeof for arrays — only works when the array is in scope */
size_t len = sizeof(arr) / sizeof(arr[0]);  /* 5 — classic idiom */
/* When arr is passed to a function, it decays to a pointer — sizeof gives pointer size! */

/* Multi-dimensional arrays */
int matrix[3][4];             /* 3 rows, 4 columns — row-major in memory */
matrix[1][2] = 99;            /* row 1, column 2 */
int* flat = (int*)matrix;     /* treat as 1D array: flat[1*4 + 2] == matrix[1][2] */

/* Variable Length Arrays (C99) — avoid in practice: no dynamic allocation tracking */
/* int vla[n];  ← size known only at runtime; stack overflow risk for large n */
/* Prefer: malloc(n * sizeof(int)) for dynamic sizes */

/* ── Strings in C ── */
/* Strings are null-terminated char arrays — no string type! */
char s1[] = "hello";         /* {'h','e','l','l','o','\0'} — 6 bytes total */
char s2[20] = "hello";       /* 20-byte buffer, 6 used, rest zero */
const char* s3 = "hello";    /* pointer to string literal (read-only memory) */
/* s3[0] = 'H'; */           /* UNDEFINED BEHAVIOUR — string literals are read-only */

/* strlen — does NOT include the null terminator */
#include <string.h>
size_t len2 = strlen(s1);    /* 5 — does not count '\0' */

/* ⚠️ NEVER use these unsafe functions */
/* gets(buf)           — no bounds checking — removed from C11 */
/* strcpy(dst, src)    — no bounds checking — use strncpy or strlcpy */
/* strcat(dst, src)    — no bounds checking — use strncat */
/* sprintf(buf, fmt)   — no bounds checking — use snprintf */

/* ✅ Safe alternatives */
char buf[32];
strncpy(buf, src, sizeof(buf) - 1);  /* limit copy to buffer size */
buf[sizeof(buf) - 1] = '\0';         /* ensure null termination */

snprintf(buf, sizeof(buf), "Hello, %s!", name);  /* bounds-checked formatting */

/* String functions overview */
strlen(s)              /* length (not including '\0') */
strcmp(a, b)           /* compare: 0 if equal, <0 if a<b, >0 if a>b */
strncmp(a, b, n)       /* compare at most n chars */
strchr(s, c)           /* find first occurrence of char c */
strstr(haystack, needle)  /* find first occurrence of substring */
memcpy(dst, src, n)    /* copy n bytes — regions must NOT overlap */
memmove(dst, src, n)   /* copy n bytes — regions MAY overlap */
memset(buf, 0, n)      /* set n bytes to value (0 = zero out buffer) */
memcmp(a, b, n)        /* compare n bytes */
```

---

## 10 · Memory management ⚠️

> **EN — The most error-prone aspect of C.**
> C has no garbage collector, no destructors, no smart pointers.
> Every byte you allocate, you must free. Exactly once. At the right time.

> **FR — L'aspect le plus sujet aux erreurs du C.**
> Le C n'a ni ramasse-miettes, ni destructeurs, ni smart pointers.
> Chaque octet alloué doit être libéré. Exactement une fois. Au bon moment.

```c
#include <stdlib.h>

/* ── malloc — allocate uninitialized memory ── */
/* Returns NULL on failure — ALWAYS CHECK */
int* arr = malloc(10 * sizeof(int));
if (arr == NULL) {
    perror("malloc failed");
    return -1;     /* handle the error */
}
arr[0] = 42;       /* use the allocated memory */
free(arr);         /* MUST free — or memory leaks */
arr = NULL;        /* good habit: null after free to prevent use-after-free */

/* ── calloc — allocate zero-initialized memory ── */
/* Safer than malloc for arrays: no uninitialised reads */
int* zeros = calloc(10, sizeof(int));   /* 10 ints, all set to 0 */
if (!zeros) { /* handle error */ }
free(zeros);
zeros = NULL;

/* ── realloc — resize an existing allocation ── */
int* p = malloc(5 * sizeof(int));
/* ⚠️ Never: p = realloc(p, 10 * sizeof(int));
   If realloc fails, it returns NULL AND does NOT free p → memory leak! */
int* tmp = realloc(p, 10 * sizeof(int));
if (tmp == NULL) {
    /* realloc failed — p is still valid */
    free(p);
    return -1;
}
p = tmp;   /* safe: only assign after confirming success */
free(p);

/* ── Memory error taxonomy ── */

/* 1. MEMORY LEAK / FUITE MÉMOIRE */
void leak(void) {
    int* p = malloc(100);
    if (some_condition) return;    /* ← leak: returned without freeing */
    /* Even with free at end of function, early return causes leak */
    free(p);
}
/* Fix: use goto-based cleanup pattern (see below) */

/* 2. DOUBLE FREE / DOUBLE LIBÉRATION */
int* dp = malloc(4);
free(dp);
/* free(dp); */   /* ← CRASH / HEAP CORRUPTION — freeing already freed memory */
/* Fix: dp = NULL after free; free(NULL) is safe (no-op) */

/* 3. USE AFTER FREE / ACCÈS APRÈS LIBÉRATION */
int* uaf = malloc(4);
free(uaf);
/* *uaf = 5; */  /* ← UB: writing to freed memory — security exploit possible */

/* 4. BUFFER OVERFLOW / DÉBORDEMENT DE TAMPON */
char* buf = malloc(10);
/* memcpy(buf, data, 20); */  /* ← writing 20 bytes into 10-byte buffer */

/* 5. UNINITIALISED READ */
int* ui = malloc(4);
/* int x = *ui; */  /* ← reading uninitialised memory — random value */
/* Fix: use calloc or explicitly initialise */

/* ── goto-based cleanup — the idiomatic C error handling pattern ── */
int process(const char* path) {
    int result = -1;
    FILE* f = NULL;
    char* buf = NULL;

    f = fopen(path, "r");
    if (!f) { goto cleanup; }

    buf = malloc(1024);
    if (!buf) { goto cleanup; }

    /* ... do work ... */
    result = 0;

cleanup:          /* single exit point — always frees everything */
    free(buf);    /* free(NULL) is safe — no check needed */
    if (f) fclose(f);
    return result;
}

/* ── Dynamic array pattern ── */
typedef struct {
    int*   data;
    size_t size;
    size_t capacity;
} IntVec;

IntVec vec_new(void) {
    return (IntVec){ .data = NULL, .size = 0, .capacity = 0 };
}

int vec_push(IntVec* v, int value) {
    if (v->size == v->capacity) {
        size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
        int* tmp = realloc(v->data, new_cap * sizeof(int));
        if (!tmp) return -1;
        v->data = tmp;
        v->capacity = new_cap;
    }
    v->data[v->size++] = value;
    return 0;
}

void vec_free(IntVec* v) {
    free(v->data);
    v->data = NULL;
    v->size = v->capacity = 0;
}
```

---

## 11 · Structs, unions, enums & bitfields

```c
/* ── struct — group related data ── */
struct Point {
    double x;
    double y;
};

/* typedef to avoid writing 'struct' every time */
typedef struct {
    char   name[64];
    int    age;
    double salary;
} Employee;

/* Initialisation */
Employee e1 = { "Alice", 30, 75000.0 };       /* positional */
Employee e2 = { .name = "Bob", .age = 25 };   /* designated (C99) — remaining = 0 */
Employee e3 = {0};                             /* zero-initialise all fields */

/* Access */
e1.age = 31;
Employee* pe = &e1;
pe->age = 31;     /* arrow: shorthand for (*pe).age */

/* Struct padding and alignment */
struct Padded {
    char  a;     /* 1 byte */
    /* 3 bytes padding */
    int   b;     /* 4 bytes — must be 4-byte aligned */
    char  c;     /* 1 byte */
    /* 3 bytes padding */
};  /* sizeof = 12, not 6 */

struct Packed {
    int   b;     /* 4 bytes */
    char  a;     /* 1 byte */
    char  c;     /* 1 byte */
    /* 2 bytes padding */
};  /* sizeof = 8 — reordering reduces padding */

/* ── union — same memory, multiple interpretations ── */
typedef union {
    uint32_t raw;       /* access all 4 bytes as one integer */
    uint8_t  bytes[4];  /* access each byte individually */
    float    f;         /* interpret same bytes as float */
} Word32;

Word32 w;
w.f = 3.14f;
printf("float bytes: %02X %02X %02X %02X\n",
       w.bytes[0], w.bytes[1], w.bytes[2], w.bytes[3]);

/* ── enum ── */
typedef enum {
    STATUS_OK      = 0,
    STATUS_ERROR   = -1,
    STATUS_PENDING = 1,
    STATUS_DONE    = 2
} Status;

Status s = STATUS_OK;
if (s == STATUS_ERROR) { /* ... */ }

/* ── Bitfields — pack multiple boolean flags into one int ── */
typedef struct {
    unsigned int is_visible   : 1;   /* 1 bit */
    unsigned int is_enabled   : 1;
    unsigned int priority     : 4;   /* 4 bits → values 0–15 */
    unsigned int color_index  : 8;   /* 8 bits → values 0–255 */
} Flags;   /* packed into a single 32-bit int */

Flags f = { .is_visible = 1, .priority = 3 };
```

---

## 12 · Preprocessor

```c
/* ── #include ── */
#include <stdio.h>      /* system header — search standard include paths */
#include "mylib.h"      /* local header — search current directory first */

/* ── #define — object-like macros ── */
#define MAX_SIZE   1024
#define PI         3.14159265358979
#define VERSION    "1.0.0"
#undef  MAX_SIZE   /* undefine a macro */

/* ── #define — function-like macros ── */
/* ⚠️ Always parenthesise arguments AND the whole expression */
#define MAX(a, b)   ((a) > (b) ? (a) : (b))   /* correct */
#define BAD_MAX(a, b)  a > b ? a : b           /* wrong — operator precedence issues */

/* ⚠️ Macro pitfalls */
int x = 5;
MAX(x++, 3);   /* expands to: ((x++) > (3) ? (x++) : (3)) — x incremented TWICE! */
/* Fix: use inline functions instead of macros for computations */

/* ── Conditional compilation ── */
#ifdef DEBUG
    printf("debug: value = %d\n", value);
#endif

#ifndef HEADER_GUARD_H
#define HEADER_GUARD_H
/* header contents */
#endif   /* HEADER_GUARD_H */

/* C99 _Pragma / C11 _Static_assert */
_Static_assert(sizeof(int) == 4, "int must be 32-bit");

/* ── Predefined macros ── */
__FILE__    /* current source filename as string literal */
__LINE__    /* current line number as integer */
__DATE__    /* compilation date: "Jan  1 2024" */
__TIME__    /* compilation time: "12:00:00" */
__func__    /* current function name (C99) */

/* ── Stringification and token pasting ── */
#define STRINGIFY(x)    #x           /* converts x to a string literal */
#define CONCAT(a, b)    a##b         /* pastes tokens: CONCAT(foo, bar) → foobar */

/* ── X-macros — data-driven code generation ── */
#define COLOR_LIST \
    X(RED,   0xFF0000) \
    X(GREEN, 0x00FF00) \
    X(BLUE,  0x0000FF)

/* Generate enum */
typedef enum {
    #define X(name, value) COLOR_##name,
    COLOR_LIST
    #undef X
} Color;

/* Generate names array */
const char* color_names[] = {
    #define X(name, value) #name,
    COLOR_LIST
    #undef X
};
```

---

## 13 · File I/O

```c
#include <stdio.h>

/* ── Opening and closing ── */
FILE* f = fopen("file.txt", "r");   /* returns NULL on failure */
if (!f) { perror("fopen"); return -1; }
/* ... */
fclose(f);   /* always close — resources leak if you don't */

/* Modes */
/* "r"  — read text (file must exist) */
/* "w"  — write text (creates or truncates) */
/* "a"  — append text (creates if not exists) */
/* "rb" — read binary */
/* "wb" — write binary */
/* "r+" — read+write (file must exist) */

/* ── Text I/O ── */
/* Reading */
char line[256];
while (fgets(line, sizeof(line), f) != NULL) {
    /* fgets includes the '\n' if the line fits; checks buffer size */
    printf("Line: %s", line);
}

int n;
fscanf(f, "%d", &n);   /* format-directed read (fragile — prefer fgets + sscanf) */

/* Writing */
fprintf(f, "Value: %d\n", n);
fputs("hello\n", f);

/* ── Binary I/O ── */
int arr[10];
size_t read  = fread(arr,  sizeof(int), 10, f);   /* returns elements read */
size_t wrote = fwrite(arr, sizeof(int), 10, f);   /* returns elements written */

/* ── Position ── */
fseek(f, 0, SEEK_SET);   /* go to beginning */
fseek(f, 0, SEEK_END);   /* go to end */
long pos = ftell(f);     /* current position in bytes */
rewind(f);               /* equivalent to fseek(f, 0, SEEK_SET) */

/* ── Error checking ── */
if (ferror(f)) { /* I/O error occurred */ }
if (feof(f))   { /* end of file reached */ }
clearerr(f);   /* clear error and EOF flags */

/* ── Standard streams ── */
stdin    /* standard input  (keyboard) */
stdout   /* standard output (terminal) */
stderr   /* standard error  (terminal, unbuffered) */

/* Always write errors to stderr */
fprintf(stderr, "Error: %s\n", message);
```

---

## 14 · Standard library overview

| Header | Key functions |
|---|---|
| `<stdio.h>` | `printf`, `scanf`, `fopen`, `fclose`, `fread`, `fwrite`, `sprintf`, `snprintf` |
| `<stdlib.h>` | `malloc`, `calloc`, `realloc`, `free`, `exit`, `atoi`, `strtol`, `qsort`, `bsearch`, `rand` |
| `<string.h>` | `strlen`, `strcpy`, `strncpy`, `strcmp`, `strcat`, `strstr`, `memcpy`, `memmove`, `memset` |
| `<math.h>` | `sqrt`, `pow`, `floor`, `ceil`, `fabs`, `sin`, `cos`, `log`, `exp` (link with `-lm`) |
| `<stdint.h>` | `int8_t`…`uint64_t`, `intptr_t`, `SIZE_MAX`, `INT32_MAX` |
| `<stdbool.h>` | `bool`, `true`, `false` (C99) |
| `<stddef.h>` | `size_t`, `ptrdiff_t`, `NULL`, `offsetof` |
| `<inttypes.h>` | `PRId32`, `PRIu64` — printf format macros for fixed-width types |
| `<limits.h>` | `INT_MAX`, `INT_MIN`, `CHAR_MAX`, `ULONG_MAX`… |
| `<ctype.h>` | `isdigit`, `isalpha`, `isspace`, `toupper`, `tolower` |
| `<assert.h>` | `assert(expr)` — aborts with message if expr is false |
| `<errno.h>` | `errno`, `EINVAL`, `ENOMEM`… — error codes from system calls |
| `<time.h>` | `time`, `clock`, `difftime`, `strftime` |
| `<signal.h>` | `signal`, `raise`, `SIGINT`, `SIGSEGV` |
| `<setjmp.h>` | `setjmp`, `longjmp` — non-local jumps (exception-like, rarely needed) |

---

## 15 · Error handling

```c
#include <errno.h>
#include <string.h>

/* errno — global variable set by system calls on error */
FILE* f = fopen("nonexistent.txt", "r");
if (!f) {
    /* errno contains the error code */
    fprintf(stderr, "fopen: %s\n", strerror(errno));  /* human-readable message */
    perror("fopen");   /* equivalent: prints "fopen: No such file or directory" */
}

/* errno is only meaningful IMMEDIATELY after a failed call */
/* It is NOT cleared on success — always check the return value first */

/* Return value conventions */
/* Most C functions use these patterns: */
int func1(void);           /* returns 0 on success, -1 on error */
int func2(void);           /* returns 0 on success, non-zero error code */
void* func3(size_t n);     /* returns NULL on error */
ssize_t func4(void);       /* returns -1 on error */

/* assert — for invariants that MUST be true (disabled in release with -DNDEBUG) */
#include <assert.h>
assert(ptr != NULL);             /* abort + message if ptr is NULL */
assert(index < array_size);      /* bounds check in debug mode */

/* Static assertion (C11) — checked at compile time */
_Static_assert(sizeof(long) == 8, "This code requires 64-bit longs");

/* Custom error propagation pattern */
typedef enum { ERR_OK = 0, ERR_NULL, ERR_OOM, ERR_IO } Error;

Error read_data(const char* path, char** out) {
    if (!path || !out) return ERR_NULL;
    *out = malloc(1024);
    if (!*out) return ERR_OOM;
    FILE* f = fopen(path, "r");
    if (!f) { free(*out); *out = NULL; return ERR_IO; }
    fread(*out, 1, 1023, f);
    fclose(f);
    return ERR_OK;
}
```

---

## 16 · Security pitfalls ⚠️

> **EN — C security pitfalls cause real-world CVEs every year.**
> These are not theoretical problems. They are exploited in production software.

> **FR — Les failles de sécurité C causent de vrais CVE chaque année.**
> Ce ne sont pas des problèmes théoriques. Ils sont exploités dans des logiciels en production.

```c
/* ─── 1. Buffer overflow — the most exploited C vulnerability ─── */
/* ⚠️ DANGEROUS */
char buf[16];
gets(buf);          /* NO bounds check — overflow possible — REMOVED in C11 */
strcpy(buf, input); /* NO bounds check — if input > 15 chars: stack overflow */
sprintf(buf, "%s", input); /* NO bounds check */

/* ✅ SAFE */
fgets(buf, sizeof(buf), stdin);            /* limit: sizeof(buf) - 1 chars */
strncpy(buf, input, sizeof(buf) - 1);
buf[sizeof(buf) - 1] = '\0';              /* ensure null termination */
snprintf(buf, sizeof(buf), "%s", input);  /* always null-terminates */

/* ─── 2. Integer overflow / underflow ─── */
/* ⚠️ DANGEROUS — signed overflow is UNDEFINED BEHAVIOUR */
int a = INT_MAX;
int b = a + 1;      /* UB — may wrap to negative OR do anything */

/* ⚠️ Dangerous length computation */
size_t len = strlen(s1) + strlen(s2) + 1;
char* buf2 = malloc(len);   /* fine IF len doesn't overflow */

/* ✅ Check for overflow before computing */
if (len1 > SIZE_MAX - len2 - 1) { /* overflow would occur */ return ERR_OOM; }

/* ─── 3. Format string injection ─── */
char user_input[] = "%s%s%s%s";
/* ⚠️ DANGEROUS */
printf(user_input);           /* attacker controls format string → memory read/write */
fprintf(logfile, user_input); /* same issue */

/* ✅ SAFE — always use a format string literal */
printf("%s", user_input);
fprintf(logfile, "%s", user_input);

/* ─── 4. Use-after-free ─── */
int* p = malloc(4);
free(p);
/* ⚠️ */ *p = 5;   /* UB — may overwrite heap metadata → security exploit */
/* ✅ */ p = NULL; free(p); /* free(NULL) is a safe no-op */

/* ─── 5. Uninitialised memory ─── */
char secret[32];   /* stack buffer — contains PREVIOUS stack contents */
char buf3[32];
memcpy(buf3, secret, 32);   /* ⚠️ leaks previous stack data (CVE class!) */
/* ✅ */ memset(secret, 0, sizeof(secret));  /* always zero sensitive buffers */

/* ─── 6. Signed/unsigned comparison ─── */
int len3 = get_length();    /* may be negative! */
char data[100];
/* ⚠️ */ if (len3 <= 100) { memcpy(data, src, len3); }
/* If len3 = -1: converted to SIZE_MAX in memcpy → OVERFLOW */
/* ✅ */ if (len3 > 0 && (size_t)len3 <= sizeof(data)) { memcpy(data, src, len3); }

/* ─── 7. Secure memory clearing ─── */
char password[64];
/* Compute with password... */
/* ⚠️ */ memset(password, 0, sizeof(password));   /* compiler may OPTIMISE THIS AWAY */
/* ✅ */ explicit_bzero(password, sizeof(password)); /* or use SecureZeroMemory on Windows */
/* ✅ */ volatile char* vp = password;
         while (*vp) *vp++ = 0;   /* volatile prevents optimisation */

/* ─── 8. Path traversal ─── */
char path[256];
snprintf(path, sizeof(path), "/data/%s", user_filename);
/* ⚠️ user_filename = "../../etc/passwd" → path = "/data/../../etc/passwd" */
/* ✅ Sanitise: reject filenames containing '/' or ".." */
```

---

## 17 · Compiler flags & tools

| Tool | Purpose | Command |
|---|---|---|
| **gcc** | Compiler | `gcc -std=c11 -Wall -Wextra -g` |
| **clang** | Compiler (better diagnostics) | `clang -std=c11 -Wall -Wextra -g` |
| **GDB** | Debugger | `gdb ./program` |
| **Valgrind** | Memory leak + error detector | `valgrind --leak-check=full ./program` |
| **AddressSanitizer** | Runtime memory errors | `-fsanitize=address,undefined` |
| **cppcheck** | Static analyser (C/C++) | `cppcheck --enable=all src/` |
| **splint** | Strict static analyser for C | `splint +posixlib file.c` |
| **scan-build** | clang static analyser | `scan-build make` |
| **make** | Build system | `make -f Makefile` |
| **nm / objdump** | Symbol inspection | `nm program`, `objdump -d program` |
| **strace** | System call tracer (Linux) | `strace ./program` |
| **ltrace** | Library call tracer (Linux) | `ltrace ./program` |

### GDB quick reference

```bash
gdb ./program       # start
run [args]          # run the program
break main          # breakpoint at function
break file.c:42     # breakpoint at line
next  (n)           # step over
step  (s)           # step into
continue (c)        # run to next breakpoint
print var           # print value of variable
print *ptr          # dereference pointer
print arr[0]@10     # print 10 elements of array
x/10xb ptr          # examine 10 bytes in hex at ptr
info locals         # all local variables
backtrace (bt)      # call stack
frame 2             # switch to frame 2
watch x             # break when x changes
display x           # print x after every step
quit (q)
```

### Makefile template

```makefile
CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Wpedantic -g \
          -fsanitize=address,undefined
LDFLAGS =
TARGET  = program
SRCS    = $(wildcard src/*.c)
OBJS    = $(SRCS:src/%.c=obj/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c -o $@ $<

obj:
	mkdir -p obj

clean:
	rm -rf obj $(TARGET)

.PHONY: all clean
```

---

## 18 · Quick reference card

```
TYPES                               MEMORY
──────────────────────────────────  ────────────────────────────────────
char, short, int, long, long long   stack: int x; char buf[32];
float, double, long double          heap:  malloc / calloc / realloc
bool (stdbool.h)                    free:  always once; set ptr = NULL
uint8_t…uint64_t (stdint.h)         calloc: zero-initialised (safer)
size_t (unsigned, use for sizes)

POINTERS                            STRINGS
──────────────────────────────────  ────────────────────────────────────
int* p = &x;  /* address-of */      char s[] = "hello";  /* mutable */
*p = 5;       /* dereference */     const char* s = "hi"; /* read-only */
p++;          /* advance */         strlen / strncpy / snprintf
p->field      /* arrow */           NEVER: gets strcpy sprintf strcat
const int* cp = &x; /* read-only */ memcpy / memmove / memset / memcmp

SECURITY CHECKLIST                  BUILD FLAGS (development)
──────────────────────────────────  ────────────────────────────────────
□ snprintf not sprintf              -std=c11 -Wall -Wextra -Wpedantic
□ strncpy not strcpy                -Wshadow -Wformat=2
□ fgets not gets (removed C11)      -g -fsanitize=address,undefined
□ format string literal             -fstack-protector-strong
□ check malloc return != NULL       valgrind --leak-check=full ./prog
□ free exactly once; set NULL
□ check array bounds before access
□ check signed/unsigned comparisons
□ memset sensitive data when done
```

---

*C CodeSamples — see also `hello-world/` and `bare-metal/` projects.*
*See: https://en.cppreference.com/w/c · https://wiki.sei.cmu.edu/confluence/display/c*
