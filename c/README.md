# C

---

## 🇬🇧 English

### What is C?

C is a general-purpose, low-level programming language that gives you direct access to memory and hardware. It is the language that runs your operating system. Linux, Windows, macOS — all written largely in C. The Python interpreter is written in C. The Node.js runtime is written in C and C++. When you use a high-level language, C is almost certainly running underneath.

C teaches you what a computer actually does. No garbage collector. No automatic memory management. You ask for memory, you use it, you free it. This directness is what makes C both powerful and dangerous.

If you understand C, you understand computing.

---

### A brief history

| Year | Event |
|------|-------|
| 1969–1973 | Dennis Ritchie develops C at Bell Labs alongside Unix |
| 1978 | "The C Programming Language" by Kernighan & Ritchie — one of the greatest programming books ever written |
| 1989 | ANSI C (C89) — the first standardized version |
| 1999 | C99 — inline functions, variable-length arrays, `//` comments |
| 2011 | C11 — multi-threading support, `_Generic` |
| 2018 | C17 — bug fixes and clarifications |
| Now | C23 is the latest standard, released 2024 |

> Dennis Ritchie, who created C, also co-created Unix. He died in 2011, just one week after Steve Jobs, but received far less public attention. His work lives in virtually every device on the planet. Remember his name.

---

### Best tools

| Tool | Purpose | Link |
|------|---------|-------|
| **GCC** | The GNU Compiler Collection — the standard on Linux/macOS | https://gcc.gnu.org |
| **Clang** | LLVM-based compiler, excellent error messages | https://clang.llvm.org |
| **MSVC** | Microsoft's compiler for Windows | Included in Visual Studio |
| **VS Code** + C/C++ extension | Lightweight editor | https://code.visualstudio.com |
| **CLion** | Full-featured C/C++ IDE | https://www.jetbrains.com/clion/ |
| **GDB** | GNU Debugger — learn to use it | https://www.gnu.org/software/gdb/ |
| **Valgrind** | Memory error detector — find leaks and bugs | https://valgrind.org |
| **Make / CMake** | Build systems | https://cmake.org |
| **AddressSanitizer** | Runtime memory error detection (built into GCC/Clang) | `-fsanitize=address` flag |

---

### Basic syntax

#### Hello, World

```c
#include <stdio.h>   /* standard input/output */

int main(void)
{
    printf("Hello, World!\n");
    return 0;   /* 0 means success */
}
```

#### Variables and types

```c
#include <stdio.h>

int main(void)
{
    /* Basic types */
    int age = 30;               /* integer */
    float pi = 3.14f;           /* single precision float */
    double precise = 3.14159;   /* double precision float */
    char letter = 'A';          /* single character */
    char name[] = "Alice";      /* string (array of chars) */

    /* Sizes vary by platform — use stdint.h for fixed sizes */
    #include <stdint.h>
    int8_t  byte  = 127;        /* exactly 8 bits */
    int32_t word  = 2147483647; /* exactly 32 bits */
    int64_t dword = 9223372036854775807LL;

    printf("Name: %s, Age: %d\n", name, age);
    return 0;
}
```

#### Conditionals

```c
int age = 20;

if (age >= 18)
{
    printf("Adult\n");
}
else if (age >= 13)
{
    printf("Teenager\n");
}
else
{
    printf("Child\n");
}

/* Switch */
int day = 3;
switch (day)
{
    case 1:  printf("Monday\n");    break;
    case 2:  printf("Tuesday\n");   break;
    case 3:  printf("Wednesday\n"); break;
    default: printf("Other\n");     break;
}
/* Always include break — falling through is a common bug */
```

#### Loops

```c
/* for loop */
for (int i = 0; i < 5; i++)
{
    printf("%d\n", i);
}

/* while loop */
int count = 0;
while (count < 5)
{
    printf("%d\n", count);
    count++;
}

/* do-while — body executes at least once */
do
{
    printf("%d\n", count);
    count--;
} while (count > 0);
```

#### Functions

```c
#include <stdio.h>

/* Function declaration (prototype) — must appear before use */
int add(int a, int b);
void greet(const char *name);

int main(void)
{
    int result = add(3, 4);
    printf("3 + 4 = %d\n", result);
    greet("Alice");
    return 0;
}

/* Function definition */
int add(int a, int b)
{
    return a + b;
}

void greet(const char *name)
{
    printf("Hello, %s!\n", name);
    /* void — returns nothing */
}
```

#### Arrays

```c
#include <stdio.h>

int main(void)
{
    /* Fixed-size array */
    int numbers[5] = {1, 2, 3, 4, 5};

    /* Access by index */
    printf("%d\n", numbers[0]); /* 1 */
    printf("%d\n", numbers[4]); /* 5 */

    /* Iterate */
    int length = sizeof(numbers) / sizeof(numbers[0]); /* 5 */
    for (int i = 0; i < length; i++)
    {
        printf("%d ", numbers[i]);
    }

    /* 2D array */
    int matrix[2][3] = {{1, 2, 3}, {4, 5, 6}};
    printf("\n%d\n", matrix[1][2]); /* 6 */

    return 0;
}
```

#### Pointers — the heart of C

```c
#include <stdio.h>

int main(void)
{
    int value = 42;

    /* A pointer stores a memory address */
    int *ptr = &value;   /* & gives the address of value */

    printf("Value:   %d\n", value);   /* 42 */
    printf("Address: %p\n", (void *)ptr);  /* e.g. 0x7ffd1234 */
    printf("Via ptr: %d\n", *ptr);    /* 42 — * dereferences the pointer */

    /* Modify value through the pointer */
    *ptr = 100;
    printf("New value: %d\n", value); /* 100 */

    return 0;
}
```

#### Pointers and functions

```c
#include <stdio.h>

/* Without pointer — value is copied, original unchanged */
void add_one_bad(int n)
{
    n++;  /* modifies local copy only */
}

/* With pointer — modifies the original */
void add_one(int *n)
{
    (*n)++;
}

int main(void)
{
    int x = 5;
    add_one_bad(x);
    printf("%d\n", x); /* still 5 */

    add_one(&x);
    printf("%d\n", x); /* 6 */

    return 0;
}
```

#### Dynamic memory allocation

```c
#include <stdio.h>
#include <stdlib.h>  /* malloc, free */

int main(void)
{
    int n = 5;

    /* Allocate memory for 5 integers */
    int *array = malloc(n * sizeof(int));

    if (array == NULL)
    {
        /* Always check if allocation succeeded */
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    /* Use the array */
    for (int i = 0; i < n; i++)
    {
        array[i] = i * 2;
    }

    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    /* Always free what you malloc */
    free(array);
    array = NULL;  /* avoid dangling pointer */

    return 0;
}
```

#### Structs — custom data types

```c
#include <stdio.h>
#include <string.h>

typedef struct
{
    char name[50];
    int age;
    double height;
} Person;

void print_person(const Person *p)
{
    printf("Name: %s, Age: %d, Height: %.2f\n", p->name, p->age, p->height);
}

int main(void)
{
    Person alice;
    strncpy(alice.name, "Alice", sizeof(alice.name) - 1);
    alice.name[sizeof(alice.name) - 1] = '\0';  /* ensure null termination */
    alice.age = 30;
    alice.height = 1.75;

    print_person(&alice);  /* pass by pointer — efficient */

    return 0;
}
```

#### A critical security warning: buffer overflow

```c
#include <stdio.h>
#include <string.h>

/* DANGEROUS — never do this */
void bad_input(void)
{
    char buffer[10];
    gets(buffer);           /* NEVER USE gets() — no bounds checking */
    scanf("%s", buffer);    /* Also dangerous — no length limit */
}

/* SAFE */
void safe_input(void)
{
    char buffer[10];
    fgets(buffer, sizeof(buffer), stdin);  /* limits input to buffer size */
    scanf("%9s", buffer);                  /* 9 = sizeof(buffer) - 1 */
}

/* Buffer overflow is the root cause of countless security vulnerabilities.
   Always know the size of your buffers. Always check bounds. */
```

---

## 🇫🇷 Français

### C'est quoi le C ?

Le C est un langage de programmation généraliste de bas niveau qui te donne un accès direct à la mémoire et au matériel. C'est le langage qui fait tourner ton système d'exploitation. Linux, Windows, macOS — tous écrits en grande partie en C. L'interpréteur Python est écrit en C. Quand tu utilises un langage de haut niveau, le C tourne presque certainement en dessous.

Le C t'apprend ce qu'un ordinateur fait réellement. Pas de ramasse-miettes. Pas de gestion automatique de la mémoire. Tu demandes de la mémoire, tu l'utilises, tu la libères. Cette proximité est ce qui rend le C à la fois puissant et dangereux.

Si tu comprends le C, tu comprends l'informatique.

---

### Un peu d'histoire

| Année | Événement |
|-------|-----------|
| 1969–1973 | Dennis Ritchie développe le C chez Bell Labs avec Unix |
| 1978 | "The C Programming Language" de Kernighan & Ritchie — l'un des meilleurs livres de programmation jamais écrits |
| 1989 | ANSI C (C89) — première version standardisée |
| 1999 | C99 — fonctions inline, tableaux de longueur variable, commentaires `//` |
| 2011 | C11 — support du multi-threading |
| 2024 | C23 — la dernière norme en date |

> Dennis Ritchie, créateur du C, a aussi co-créé Unix. Il est mort en 2011, une semaine après Steve Jobs, mais a reçu beaucoup moins d'attention médiatique. Son travail vit dans pratiquement chaque appareil sur la planète. Souviens-toi de son nom.

---

### Les meilleurs outils

| Outil | Utilité | Lien |
|-------|---------|------|
| **GCC** | Le compilateur GNU — standard sur Linux/macOS | https://gcc.gnu.org |
| **Clang** | Compilateur LLVM, excellents messages d'erreur | https://clang.llvm.org |
| **VS Code** + extension C/C++ | Éditeur léger | https://code.visualstudio.com |
| **CLion** | IDE complet C/C++ | https://www.jetbrains.com/clion/ |
| **GDB** | Débogueur GNU — apprends à l'utiliser | https://www.gnu.org/software/gdb/ |
| **Valgrind** | Détecteur d'erreurs mémoire | https://valgrind.org |

---

### Syntaxe de base

#### Hello, World

```c
#include <stdio.h>

int main(void)
{
    printf("Bonjour, Monde !\n");
    return 0;
}
```

#### Variables et types

```c
int age = 30;
float pi = 3.14f;
double precis = 3.14159;
char lettre = 'A';
char nom[] = "Alice";
```

#### Pointeurs — le cœur du C

```c
int valeur = 42;
int *ptr = &valeur;  /* & donne l'adresse de valeur */

printf("Valeur :   %d\n", valeur);
printf("Via ptr : %d\n", *ptr);   /* * déréférence le pointeur */

*ptr = 100;
printf("Nouvelle valeur : %d\n", valeur); /* 100 */
```

#### Allocation dynamique de mémoire

```c
#include <stdlib.h>

int n = 5;
int *tableau = malloc(n * sizeof(int));

if (tableau == NULL)
{
    fprintf(stderr, "Échec d'allocation mémoire\n");
    return 1;
}

/* Utiliser le tableau... */

free(tableau);     /* Toujours libérer ce qu'on a alloué */
tableau = NULL;    /* Éviter le pointeur fantôme */
```

#### Structures

```c
typedef struct
{
    char nom[50];
    int age;
    double taille;
} Personne;

void afficher_personne(const Personne *p)
{
    printf("Nom : %s, Âge : %d\n", p->nom, p->age);
}
```

#### Avertissement de sécurité critique

```c
/* DANGEREUX — ne jamais utiliser gets() */
char buffer[10];
gets(buffer);  /* pas de vérification des bornes — overflow garanti */

/* SÉCURISÉ */
fgets(buffer, sizeof(buffer), stdin);  /* limite à la taille du buffer */
```

> Le dépassement de tampon (buffer overflow) est à l'origine d'innombrables failles de sécurité. Connais toujours la taille de tes buffers. Vérifie toujours les bornes.
