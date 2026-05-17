/**
 * hello.c — C Hello World
 * CodeSamples Project
 *
 * Demonstrates: structs, dynamic memory (malloc/free), function pointers,
 *               string handling, error handling, safe I/O, enums, typedef,
 *               and the goto-cleanup pattern.
 *
 * Compile:
 *   gcc -std=c11 -Wall -Wextra -g -fsanitize=address,undefined \
 *       -o bin/hello src/hello.c
 *   ./bin/hello
 */

/* Standard library headers — C uses separate headers for each domain */
#include <stdio.h>      /* printf, fprintf, fgets, FILE, stderr */
#include <stdlib.h>     /* malloc, calloc, free, exit, qsort */
#include <string.h>     /* strlen, strncpy, memset, strcmp */
#include <math.h>       /* sqrt, M_PI — link with -lm */
#include <stdbool.h>    /* bool, true, false (C99) */
#include <stdint.h>     /* uint32_t, int64_t — fixed-width types */
#include <assert.h>     /* assert() — debug checks */
#include <ctype.h>      /* isspace, toupper */
#include <errno.h>      /* errno, strerror */

/* ─────────────────────────────────────────────────────────────────────────────
   ENUMS & TYPEDEFS
   ───────────────────────────────────────────────────────────────────────── */

/* typedef enum gives the enum a name so we don't need to write "enum Status" */
typedef enum {
    STATUS_OK      =  0,
    STATUS_ERROR   = -1,
    STATUS_INVALID = -2,
    STATUS_OOM     = -3    /* Out Of Memory */
} Status;

/* Shape type enumeration */
typedef enum {
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_TRIANGLE
} ShapeType;

/* ─────────────────────────────────────────────────────────────────────────────
   STRUCTS
   A struct groups related data into a single type.
   ───────────────────────────────────────────────────────────────────────── */

typedef struct {
    ShapeType type;
    char      name[32];    /* fixed-size char array — no malloc needed */
    double    dim[3];      /* dimensions: radius, OR width+height, OR a+b+c */
} Shape;

/* A dynamic list of shapes — manages its own heap memory */
typedef struct {
    Shape*  items;     /* pointer to heap-allocated array of Shape */
    size_t  count;     /* number of items currently stored */
    size_t  capacity;  /* number of items we can store without realloc */
} ShapeList;

/* ─────────────────────────────────────────────────────────────────────────────
   FUNCTION PROTOTYPES
   In C, functions must be declared before their first use.
   The prototype tells the compiler about the function's signature.
   ───────────────────────────────────────────────────────────────────────── */

static void        print_banner(void);
static double      shape_area(const Shape* s);
static double      shape_perimeter(const Shape* s);
static const char* shape_type_name(ShapeType t);
static void        shape_print(const Shape* s);

static Status      list_init(ShapeList* list, size_t initial_capacity);
static Status      list_push(ShapeList* list, Shape shape);
static void        list_free(ShapeList* list);
static void        list_sort_by_area(ShapeList* list);

static char*       trim_string(char* s);   /* modifies in place, returns s */
static Status      read_name(const char* prompt, char* out, size_t size);

/* ─────────────────────────────────────────────────────────────────────────────
   SHAPE FUNCTIONS
   ───────────────────────────────────────────────────────────────────────── */

static double shape_area(const Shape* s) {
    /* 'const Shape*' — we promise not to modify *s through this pointer */
    assert(s != NULL);    /* debug check — abort if NULL in debug mode */

    switch (s->type) {
        case SHAPE_CIRCLE:
            /* dim[0] = radius */
            return M_PI * s->dim[0] * s->dim[0];
        case SHAPE_RECTANGLE:
            /* dim[0] = width, dim[1] = height */
            return s->dim[0] * s->dim[1];
        case SHAPE_TRIANGLE: {
            /* Heron's formula */
            double a = s->dim[0], b = s->dim[1], c = s->dim[2];
            double semi = (a + b + c) / 2.0;
            return sqrt(semi * (semi-a) * (semi-b) * (semi-c));
        }
        default:
            return 0.0;
    }
}

static double shape_perimeter(const Shape* s) {
    assert(s != NULL);
    switch (s->type) {
        case SHAPE_CIRCLE:    return 2.0 * M_PI * s->dim[0];
        case SHAPE_RECTANGLE: return 2.0 * (s->dim[0] + s->dim[1]);
        case SHAPE_TRIANGLE:  return s->dim[0] + s->dim[1] + s->dim[2];
        default:              return 0.0;
    }
}

static const char* shape_type_name(ShapeType t) {
    /* Return string literal — read-only, no malloc needed, no free needed */
    switch (t) {
        case SHAPE_CIRCLE:    return "Circle";
        case SHAPE_RECTANGLE: return "Rectangle";
        case SHAPE_TRIANGLE:  return "Triangle";
        default:              return "Unknown";
    }
}

static void shape_print(const Shape* s) {
    printf("  %-12s | area = %8.3f | perimeter = %8.3f\n",
           shape_type_name(s->type),
           shape_area(s),
           shape_perimeter(s));
}

/* ─────────────────────────────────────────────────────────────────────────────
   DYNAMIC LIST (manual dynamic array)
   This demonstrates how C manages growing arrays manually.
   (In C++ this would be std::vector.)
   ───────────────────────────────────────────────────────────────────────── */

static Status list_init(ShapeList* list, size_t initial_capacity) {
    assert(list != NULL);
    assert(initial_capacity > 0);

    /* calloc(n, size): allocate n items of 'size' bytes, zero-initialised */
    list->items    = calloc(initial_capacity, sizeof(Shape));
    if (!list->items) {
        fprintf(stderr, "list_init: out of memory\n");
        return STATUS_OOM;
    }
    list->count    = 0;
    list->capacity = initial_capacity;
    return STATUS_OK;
}

static Status list_push(ShapeList* list, Shape shape) {
    assert(list != NULL);

    /* Grow the array if full */
    if (list->count == list->capacity) {
        size_t  new_cap = list->capacity * 2;
        /* ⚠️ Never: list->items = realloc(list->items, ...) — if realloc fails,
           the original pointer is lost and we can't free it (leak). */
        Shape* tmp = realloc(list->items, new_cap * sizeof(Shape));
        if (!tmp) {
            fprintf(stderr, "list_push: out of memory\n");
            return STATUS_OOM;
        }
        list->items    = tmp;
        list->capacity = new_cap;
    }

    /* Copy the shape into the array — structs are copied by value in C */
    list->items[list->count++] = shape;
    return STATUS_OK;
}

static void list_free(ShapeList* list) {
    if (!list) return;
    free(list->items);    /* free the heap-allocated array */
    list->items    = NULL;
    list->count    = 0;
    list->capacity = 0;
}

/* ── Comparator for qsort — must match: int cmp(const void*, const void*) ── */
static int compare_by_area(const void* a, const void* b) {
    /* qsort passes void* pointers — we must cast to the correct type */
    const Shape* sa = (const Shape*)a;
    const Shape* sb = (const Shape*)b;
    double da = shape_area(sa);
    double db = shape_area(sb);
    if (da < db) return -1;
    if (da > db) return  1;
    return 0;
}

static void list_sort_by_area(ShapeList* list) {
    assert(list != NULL);
    /* qsort: standard library sort using a comparison callback (function pointer) */
    qsort(list->items, list->count, sizeof(Shape), compare_by_area);
}

/* ─────────────────────────────────────────────────────────────────────────────
   STRING UTILITIES
   ───────────────────────────────────────────────────────────────────────── */

/* Trim leading and trailing whitespace in-place */
static char* trim_string(char* s) {
    if (!s) return s;

    /* Trim leading whitespace */
    char* start = s;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    /* Trim trailing whitespace (including '\n' from fgets) */
    size_t len = strlen(start);
    while (len > 0 && isspace((unsigned char)start[len - 1])) {
        start[--len] = '\0';
    }

    /* Move trimmed string to the beginning (in-place via memmove) */
    if (start != s) {
        memmove(s, start, len + 1);   /* +1 for null terminator */
    }
    return s;
}

/* Read a line of text safely */
static Status read_name(const char* prompt, char* out, size_t size) {
    if (!out || size == 0) return STATUS_INVALID;

    printf("%s", prompt);
    fflush(stdout);    /* flush to ensure prompt appears before blocking */

    /* fgets: reads at most (size-1) chars; always null-terminates */
    if (!fgets(out, (int)size, stdin)) {
        return STATUS_ERROR;
    }

    trim_string(out);
    return STATUS_OK;
}

/* ─────────────────────────────────────────────────────────────────────────────
   FUNCTION POINTER DEMO
   Functions in C are data — pointers to them can be stored and passed.
   ───────────────────────────────────────────────────────────────────────── */

/* Define a type for a function that takes a double and returns a double */
typedef double (*MathFunc)(double);

static double square(double x)   { return x * x; }
static double cube(double x)     { return x * x * x; }

static void apply_and_print(const char* name, MathFunc fn, double x) {
    printf("  %s(%.2f) = %.4f\n", name, x, fn(x));
}

/* ─────────────────────────────────────────────────────────────────────────────
   BANNER
   ───────────────────────────────────────────────────────────────────────── */

static void print_banner(void) {
    puts("============================================");
    puts("  C Hello World — CodeSamples Project");
    puts("============================================");
    puts("");
}

/* ─────────────────────────────────────────────────────────────────────────────
   MAIN
   ───────────────────────────────────────────────────────────────────────── */

int main(void) {
    print_banner();

    /* ── 1. User greeting ────────────────────────────────────────────────── */
    char name[64];
    Status st = read_name("Enter your name: ", name, sizeof(name));
    if (st != STATUS_OK || strlen(name) == 0) {
        strncpy(name, "World", sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';   /* ensure null termination */
    }

    for (int i = 1; i <= 3; i++) {
        printf(" %d. Hello, %s!\n", i, name);
    }
    putchar('\n');

    /* ── 2. Dynamic list of shapes ───────────────────────────────────────── */
    puts("--- Shapes (dynamic array + function pointers) ---");

    ShapeList shapes;
    /* Use goto-cleanup pattern: single exit point that always frees shapes */
    if (list_init(&shapes, 4) != STATUS_OK) {
        fprintf(stderr, "Failed to initialise shape list\n");
        return 1;
    }

    /* Compound literal initialisation — C99 */
    list_push(&shapes, (Shape){ SHAPE_CIRCLE,    "Big Circle",    {5.0} });
    list_push(&shapes, (Shape){ SHAPE_RECTANGLE, "Wide Rect",     {8.0, 3.0} });
    list_push(&shapes, (Shape){ SHAPE_TRIANGLE,  "3-4-5 Triangle",{3.0, 4.0, 5.0} });
    list_push(&shapes, (Shape){ SHAPE_CIRCLE,    "Small Circle",  {2.0} });
    list_push(&shapes, (Shape){ SHAPE_RECTANGLE, "Square",        {4.0, 4.0} });

    printf("  Added %zu shapes:\n", shapes.count);
    for (size_t i = 0; i < shapes.count; i++) {
        shape_print(&shapes.items[i]);
    }

    /* Sort using qsort + comparison callback */
    list_sort_by_area(&shapes);
    printf("\n  Sorted by area:\n");
    for (size_t i = 0; i < shapes.count; i++) {
        shape_print(&shapes.items[i]);
    }

    /* ── 3. Manual memory management demo ───────────────────────────────── */
    puts("\n--- Manual memory (malloc/calloc/realloc/free) ---");
    {
        /* malloc: uninitialized heap memory */
        int* arr = malloc(5 * sizeof(int));
        if (!arr) {
            fputs("malloc failed\n", stderr);
            list_free(&shapes);
            return 1;
        }
        for (int i = 0; i < 5; i++) arr[i] = i * i;
        printf("  malloc array: ");
        for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
        printf("\n");

        /* realloc: grow the array */
        int* tmp = realloc(arr, 10 * sizeof(int));
        if (!tmp) { free(arr); }  /* realloc failed; arr still valid */
        else {
            arr = tmp;
            for (int i = 5; i < 10; i++) arr[i] = i * i;
            printf("  after realloc (10 ints): ");
            for (int i = 0; i < 10; i++) printf("%d ", arr[i]);
            printf("\n");
            free(arr);
            arr = NULL;    /* prevent accidental use-after-free */
        }

        /* calloc: zero-initialised */
        char* buf = calloc(64, sizeof(char));
        if (buf) {
            /* buf is guaranteed to be all zeros — no memset needed */
            snprintf(buf, 64, "Hello from calloc, %s!", name);
            printf("  calloc string: %s\n", buf);
            free(buf);
            buf = NULL;
        }
    }

    /* ── 4. Function pointers ─────────────────────────────────────────────── */
    puts("\n--- Function pointers ---");
    apply_and_print("square", square, 5.0);
    apply_and_print("cube",   cube,   3.0);
    apply_and_print("sqrt",   sqrt,   16.0);  /* sqrt is already a MathFunc */

    /* Array of function pointers */
    MathFunc transforms[] = { square, cube, sqrt };
    const char* tnames[]  = { "square", "cube", "sqrt" };
    printf("  Array of function pointers on 9.0:\n");
    for (size_t i = 0; i < 3; i++) {
        printf("    %s(9.0) = %.4f\n", tnames[i], transforms[i](9.0));
    }

    /* ── 5. Fixed-width integers ─────────────────────────────────────────── */
    puts("\n--- Fixed-width integers (stdint.h) ---");
    uint32_t flags = 0xDEADBEEF;
    int64_t  big   = 9223372036854775807LL;   /* INT64_MAX */
    printf("  uint32_t flags = 0x%08X\n", flags);
    printf("  int64_t  max   = %" PRId64 "\n", big);

    /* ── 6. Security note ─────────────────────────────────────────────────── */
    puts("\n--- Security notes ---");
    puts("  This code uses:");
    puts("  fgets()   — not gets()    (bounds-checked)");
    puts("  snprintf() — not sprintf() (bounds-checked)");
    puts("  strncpy() with explicit null-termination");
    puts("  calloc()  for zero-initialised buffers");
    puts("  Compile with -fsanitize=address to verify no memory errors.");

    /* ── Cleanup ──────────────────────────────────────────────────────────── */
    list_free(&shapes);   /* frees the heap array inside the struct */

    puts("\nC — The language the world runs on.");
    return 0;   /* return 0 = success */
}
