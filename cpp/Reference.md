# C++ Language Reference / Référence du langage C++

> **Bilingual reference — English 🇬🇧 / Français 🇫🇷**
> Quick-access companion to the `hello-world` and `memory-management` example projects.

---

## Table of contents / Table des matières

1. [Standards & compilation](#1--standards--compilation)
2. [Basic types & variables](#2--basic-types--variables)
3. [Type system & casting](#3--type-system--casting)
4. [References & pointers (raw)](#4--references--pointers-raw)
5. [Memory management ⚠️](#5--memory-management-)
6. [RAII & smart pointers](#6--raii--smart-pointers)
7. [Rule of 0 / 3 / 5](#7--rule-of-0--3--5)
8. [Classes & OOP](#8--classes--oop)
9. [Inheritance & polymorphism](#9--inheritance--polymorphism)
10. [Templates & generics](#10--templates--generics)
11. [STL containers](#11--stl-containers)
12. [STL algorithms](#12--stl-algorithms)
13. [Lambdas & closures](#13--lambdas--closures)
14. [Move semantics & rvalue references](#14--move-semantics--rvalue-references)
15. [Error handling](#15--error-handling)
16. [Namespaces & modules](#16--namespaces--modules)
17. [Modern C++ idioms (C++17/20)](#17--modern-c-idioms-c1720)
18. [Compiler flags & tools](#18--compiler-flags--tools)
19. [Quick reference card](#19--quick-reference-card)

---

## 1 · Standards & compilation

### EN — C++ standards timeline

| Standard | Year | Key additions |
|---|---|---|
| C++98/03 | 1998/2003 | First ISO standard; STL, templates, exceptions |
| **C++11** | 2011 | `auto`, move semantics, smart pointers, lambdas, `nullptr`, range-for, `constexpr` |
| C++14 | 2014 | Generic lambdas, `make_unique`, relaxed `constexpr` |
| C++17 | 2017 | Structured bindings, `if constexpr`, `std::optional`, `std::variant`, `std::string_view` |
| C++20 | 2020 | Concepts, ranges, coroutines, `std::span`, modules (partial), `<format>` |
| C++23 | 2023 | `std::print`, `std::mdspan`, stacktrace, `std::expected` |

> **Target C++17 minimum** for new projects — it's supported everywhere and eliminates most legacy C++ pain.

### FR — Frise chronologique des standards C++

> **Cible minimum C++17** pour tout nouveau projet — supporté partout, élimine la plupart des problèmes du C++ historique.

---

### Compiling / Compiler

```bash
# Minimal — do NOT use in production
g++ hello.cpp -o hello

# Recommended for development / Recommandé pour le développement
g++ -std=c++17 -Wall -Wextra -Wpedantic -g -o hello hello.cpp

# Optimised release / Release optimisé
g++ -std=c++17 -O2 -DNDEBUG -o hello hello.cpp

# Memory analysis with AddressSanitizer (catches leaks, use-after-free, buffer overflow)
g++ -std=c++17 -g -fsanitize=address,undefined -o hello hello.cpp

# clang++ (same flags, better error messages)
clang++ -std=c++17 -Wall -Wextra -g -o hello hello.cpp
```

| Flag | Meaning |
|---|---|
| `-std=c++17` | Use C++17 standard |
| `-Wall` | Common warnings |
| `-Wextra` | Extra warnings |
| `-Wpedantic` | ISO-conformance warnings |
| `-g` | Debug symbols (for GDB/LLDB) |
| `-O0` | No optimisation (default with `-g`) |
| `-O2` | Speed optimisation |
| `-fsanitize=address` | AddressSanitizer — runtime memory error detection |
| `-fsanitize=undefined` | UndefinedBehaviorSanitizer |
| `-DNDEBUG` | Disables `assert()` in release builds |

---

## 2 · Basic types & variables

```cpp
// Fundamental types / Types fondamentaux
bool        flag    = true;       // 1 byte, true or false
char        c       = 'A';        // 1 byte, character
int         n       = 42;         // typically 4 bytes
long        l       = 1'000'000L; // digit separator since C++14
long long   ll      = 9'223'372'036'854'775'807LL;
float       f       = 3.14f;      // 4 bytes, ~7 significant digits
double      d       = 3.14159;    // 8 bytes, ~15 significant digits
long double ld      = 3.14159L;   // 10–16 bytes, platform-dependent

// Fixed-width integers (prefer these) / Entiers à largeur fixe (à préférer)
#include <cstdint>
int8_t   i8  = -128;
uint8_t  u8  = 255;
int32_t  i32 = 0;
uint64_t u64 = 0ULL;

// std::string (not char arrays!) / std::string (pas de tableaux de char !)
#include <string>
std::string name = "Alice";
std::string greeting = "Hello, " + name + "!";

// auto — compiler deduces the type / le compilateur déduit le type
auto x    = 42;       // int
auto pi   = 3.14;     // double
auto msg  = "hello"s; // std::string (with `using namespace std::literals`)

// const vs constexpr
const int MAX = 100;              // value fixed at runtime
constexpr int SIZE = 256;         // value computed at compile time — prefer this

// nullptr (C++11) — replaces NULL and 0 for pointers
int* p = nullptr;   // safe zero initialisation of pointer
```

---

## 3 · Type system & casting

```cpp
// Prefer C++ casts over C-style casts (T)x — they are explicit about intent
// Préférer les casts C++ aux casts C — ils sont explicites sur l'intention

// static_cast — safe compile-time conversions (numeric conversions, upcasts)
double d = 3.9;
int i = static_cast<int>(d);   // 3 — truncates (no implicit narrowing)

// dynamic_cast — safe downcasts in class hierarchies (requires virtual)
Base* b = new Derived();
Derived* d = dynamic_cast<Derived*>(b);   // returns nullptr if cast fails
if (d) { /* safe to use */ }

// const_cast — adds or removes const (rarely needed; a code smell if frequent)
const char* s = "hello";
char* mutable_s = const_cast<char*>(s);   // dangerous: modifying string literals is UB

// reinterpret_cast — low-level bit reinterpretation (hardware/serialisation only)
uint64_t bits = reinterpret_cast<uint64_t>(ptr);   // treat pointer as integer

// Avoid: (int)x, (void*)p — C-style casts bypass type safety completely
```

---

## 4 · References & pointers (raw)

### EN — The critical distinction

A **reference** is an alias — always valid, cannot be null, cannot be reseated.
A **pointer** holds an address — can be null, can be reseated, can do arithmetic.

### FR — La distinction critique

Une **référence** est un alias — toujours valide, ne peut pas être null, ne peut pas être réassignée.
Un **pointeur** contient une adresse — peut être null, peut être réassigné, peut faire de l'arithmétique.

```cpp
// References / Références
int  x = 10;
int& ref = x;   // ref IS x — same memory location
ref = 20;       // x is now 20
// int& bad;    // ERROR: references must be initialised

// const reference — cheap read-only access (avoids copying)
void print(const std::string& s) { std::cout << s; }  // no copy made

// Pointers / Pointeurs
int* p = &x;    // p holds the address of x
*p = 30;        // dereference: change the value AT the address p points to
p = nullptr;    // p no longer points to anything

// Pointer arithmetic — valid only within arrays
int arr[5] = {1, 2, 3, 4, 5};
int* ptr = arr;       // arr decays to pointer to first element
*(ptr + 2) == 3;      // true — element at index 2
ptr++;                // advance by one int (4 bytes on most platforms)

// Common pointer mistakes / Erreurs courantes avec les pointeurs
// 1. Dangling pointer — pointer to freed/out-of-scope memory
int* dangling;
{
    int local = 42;
    dangling = &local;
}   // local is destroyed — dangling now points to invalid memory
// *dangling = 5; // UNDEFINED BEHAVIOUR — stack corruption

// 2. Null dereference
int* null_ptr = nullptr;
// *null_ptr = 5; // CRASH — segmentation fault

// 3. Uninitialized pointer
int* uninit;   // contains garbage address
// *uninit = 5; // UNDEFINED BEHAVIOUR
```

---

## 5 · Memory management ⚠️

> **EN — This section is critical.**
> Most C++ security vulnerabilities stem from memory errors: buffer overflows, use-after-free,
> double-free, and memory leaks. Modern C++ (C++11+) makes most manual memory management unnecessary.
> **Prefer smart pointers. Always.**

> **FR — Cette section est critique.**
> La majorité des vulnérabilités C++ viennent d'erreurs mémoire : débordements de tampon, accès après libération,
> double libération, fuites mémoire. Le C++ moderne (C++11+) rend la gestion mémoire manuelle
> quasiment inutile. **Préférer les smart pointers. Toujours.**

### Stack vs Heap / Pile vs Tas

```
STACK (pile)                      HEAP (tas)
─────────────────────────         ────────────────────────
• Automatic lifetime              • Manual lifetime (new/delete)
• Size fixed at compile time      • Size determined at runtime
• Very fast allocation            • Slower allocation (malloc/new)
• Freed when scope exits          • Freed only when you say so
• ~1–8 MB (OS limit)              • Limited only by RAM
• No fragmentation                • Can fragment over time
─────────────────────────         ────────────────────────
int x = 5;          ✓ stack       int* p = new int(5);  heap
std::string s;      ✓ stack       new std::string(...)  heap
int arr[1000];      ✓ stack       new int[1'000'000]    heap
```

### Raw `new` / `delete` — the dangerous way / la façon dangereuse

```cpp
// Single object
int* p = new int(42);       // allocate on heap, initialise to 42
std::cout << *p;            // dereference to read value
delete p;                   // free memory — MUST happen exactly once
p = nullptr;                // good habit: prevent dangling pointer use

// Array
int* arr = new int[10];     // allocate array of 10 ints
arr[0] = 1;
delete[] arr;               // MUST use delete[] for arrays, not delete!
arr = nullptr;

// ⚠️ Common failures with raw new/delete:
// 1. Forget to delete  → memory leak (process keeps consuming RAM)
// 2. Delete twice      → double-free → undefined behaviour → crash / security hole
// 3. Use after delete  → use-after-free → undefined behaviour → security hole
// 4. delete instead of delete[] → undefined behaviour
// 5. Throw before delete in try block → leak if exception not handled

// NEVER DO THIS:
// void risky() {
//     int* p = new int(5);
//     might_throw();   // if this throws, p leaks forever
//     delete p;
// }
```

### Memory errors visualised / Erreurs mémoire visualisées

```
MEMORY LEAK / FUITE MÉMOIRE
    int* p = new int(5);  → [heap: 5] ← p
    p = new int(6);       → [heap: 5]    [heap: 6] ← p
                               ↑ no pointer left = LEAK

DOUBLE FREE / DOUBLE LIBÉRATION
    int* p = new int(5);
    delete p;             → [heap: freed]
    delete p;             → CRASH / UB — freeing already freed memory

USE AFTER FREE
    int* p = new int(5);
    delete p;             → [heap: freed] ← p (dangling!)
    *p = 10;              → UB — writing to freed memory (security hole)

BUFFER OVERFLOW
    int* arr = new int[3];
    arr[5] = 99;          → writing beyond allocated block → UB / crash
```

---

## 6 · RAII & smart pointers

### EN — RAII (Resource Acquisition Is Initialisation)

The most important C++ idiom. A resource (memory, file, socket, mutex…) is acquired in a constructor and released in the destructor. Since destructors run automatically when an object goes out of scope — even if an exception is thrown — resources are **never leaked**.

### FR — RAII (l'acquisition d'une ressource, c'est son initialisation)

L'idiome C++ le plus important. Une ressource (mémoire, fichier, socket, mutex…) est acquise dans un constructeur et libérée dans le destructeur. Comme les destructeurs s'exécutent automatiquement quand un objet sort de sa portée — même si une exception est lancée — les ressources **ne fuient jamais**.

```cpp
#include <memory>   // for smart pointers

// ──────────────────────────────────────────────────────
// std::unique_ptr — sole ownership / propriété exclusive
// ──────────────────────────────────────────────────────
// Exactly one unique_ptr owns the object at any time.
// When the unique_ptr goes out of scope, the object is deleted automatically.

std::unique_ptr<int> up = std::make_unique<int>(42);   // C++14 — prefer make_unique
std::cout << *up;       // dereference as if it were a raw pointer
// delete is called automatically when `up` leaves scope — no manual delete needed

// Cannot be copied (enforces sole ownership)
// auto up2 = up;       // COMPILE ERROR — copy is deleted
auto up2 = std::move(up);   // MOVE ownership: up is now null, up2 owns the int

// Custom deleters — for non-memory resources
auto filePtr = std::unique_ptr<FILE, decltype(&fclose)>(fopen("x.txt","r"), &fclose);
// fclose() is called automatically when filePtr leaves scope

// ──────────────────────────────────────────────────────
// std::shared_ptr — shared ownership / propriété partagée
// ──────────────────────────────────────────────────────
// Multiple shared_ptrs can own the same object.
// A reference count tracks owners; the object is deleted when count reaches 0.

auto sp1 = std::make_shared<std::string>("hello");
auto sp2 = sp1;           // both own the same string; ref count = 2
sp1.reset();              // sp1 releases ownership; ref count = 1
// string still alive, owned by sp2
// sp2 leaves scope → ref count = 0 → string is deleted

// ⚠️ Circular references cause leaks with shared_ptr!
// struct Node { std::shared_ptr<Node> next; };  // cycle → never freed

// ──────────────────────────────────────────────────────
// std::weak_ptr — non-owning observer / observateur non-propriétaire
// ──────────────────────────────────────────────────────
// Breaks circular shared_ptr cycles.
// Does NOT prevent the object from being destroyed.
// Must call .lock() to get a temporary shared_ptr before use.

std::weak_ptr<std::string> wp = sp2;
if (auto locked = wp.lock()) {   // .lock() returns shared_ptr or nullptr if expired
    std::cout << *locked;        // safe to use
}
// After sp2 is destroyed, wp.lock() returns nullptr

// ──────────────────────────────────────────────────────
// Decision guide / Guide de décision
// ──────────────────────────────────────────────────────
// ┌────────────────────────────────────────────────────┐
// │ Is there one clear owner?    → unique_ptr          │
// │ Multiple owners needed?      → shared_ptr          │
// │ Need to observe without own? → weak_ptr            │
// │ Non-owning access in scope?  → raw pointer or ref  │
// └────────────────────────────────────────────────────┘
```

---

## 7 · Rule of 0 / 3 / 5

### EN — When you manage resources manually, you must define all special member functions consistently.

| Rule | When to apply | Define |
|---|---|---|
| **Rule of 0** | Class uses only RAII members (smart pointers, std containers) | Nothing — compiler-generated defaults are correct |
| **Rule of 3** | Class manually manages a resource (pre-C++11) | Destructor + Copy constructor + Copy assignment |
| **Rule of 5** | Rule of 3 + you want move efficiency (C++11+) | + Move constructor + Move assignment |

### FR — Quand vous gérez des ressources manuellement, vous devez définir toutes les fonctions membres spéciales de façon cohérente.

```cpp
// ── Rule of 0 ── (PREFERRED / PRÉFÉRÉ)
class Modern {
    std::unique_ptr<int[]> data_;    // smart pointer manages memory
    std::string name_;               // std::string manages its memory
    // No destructor, no copy/move — compiler generates correct defaults
};

// ── Rule of 5 ── (when you own raw memory / quand vous gérez de la mémoire brute)
class Buffer {
    int*   data_;
    size_t size_;

public:
    // Constructor
    Buffer(size_t n) : data_(new int[n]), size_(n) {}

    // 1. Destructor — frees the resource
    ~Buffer() { delete[] data_; }

    // 2. Copy constructor — deep copy
    Buffer(const Buffer& other) : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 3. Copy assignment — free old, deep copy new
    Buffer& operator=(const Buffer& other) {
        if (this == &other) return *this;   // self-assignment guard
        delete[] data_;
        size_ = other.size_;
        data_ = new int[size_];
        std::copy(other.data_, other.data_ + size_, data_);
        return *this;
    }

    // 4. Move constructor — steal the resource (O(1), no copy)
    Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;   // leave source in valid but empty state
        other.size_ = 0;
    }

    // 5. Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this == &other) return *this;
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
        return *this;
    }
};

// ⚠️ If you define a destructor but NOT the copy/move operations,
// the compiler-generated copy does a SHALLOW COPY → double-free on destruction!
```

---

## 8 · Classes & OOP

```cpp
class Animal {
private:                    // private: only accessible within the class
    std::string name_;      // _ suffix: convention for member variables
    int         age_;

protected:                  // protected: accessible in derived classes
    int energy_ = 100;

public:                     // public: accessible everywhere
    // Constructor with member initialiser list (preferred over assignment in body)
    Animal(std::string name, int age)
        : name_(std::move(name)), age_(age) {}

    // Destructor (virtual if this class will be a base class)
    virtual ~Animal() = default;

    // Const member function — guarantees it won't modify the object
    std::string name() const { return name_; }
    int         age()  const { return age_; }

    // Non-const member function — can modify object
    void birthday() { ++age_; }

    // Pure virtual — makes Animal abstract (cannot be instantiated directly)
    virtual std::string speak() const = 0;

    // Non-pure virtual — has a default implementation, can be overridden
    virtual std::string describe() const {
        return name_ + " (age " + std::to_string(age_) + ")";
    }

    // Operator overloading
    bool operator<(const Animal& other) const { return age_ < other.age_; }

    // friend function — access private members from outside the class
    friend std::ostream& operator<<(std::ostream& os, const Animal& a) {
        return os << a.describe();
    }
};

// Struct vs Class: the ONLY difference is default access (public vs private)
struct Point {              // members are public by default
    double x, y;
    double length() const { return std::sqrt(x*x + y*y); }
};

// Static members — shared across ALL instances / partagés entre toutes les instances
class Counter {
    static int count_;          // declaration
public:
    Counter()  { ++count_; }
    ~Counter() { --count_; }
    static int count() { return count_; }
};
int Counter::count_ = 0;        // definition (required outside the class)
```

---

## 9 · Inheritance & polymorphism

```cpp
// public inheritance: "Dog IS-A Animal"
class Dog : public Animal {
    std::string breed_;

public:
    Dog(std::string name, int age, std::string breed)
        : Animal(std::move(name), age), breed_(std::move(breed)) {}

    // override keyword — compiler checks that we're actually overriding
    std::string speak() const override { return "Woof!"; }

    // final — prevents further overriding
    std::string describe() const override final {
        return Animal::describe() + " [" + breed_ + "]";
    }
};

// Virtual dispatch / Dispatch virtuel
Animal* a = new Dog("Rex", 3, "Labrador");
a->speak();    // calls Dog::speak() — NOT Animal::speak()
               // determined at RUNTIME via vtable (virtual table)
delete a;      // calls ~Dog() then ~Animal() because ~Animal is virtual
               // WITHOUT virtual destructor: only ~Animal() would be called → resource leak

// Slicing problem / Problème de découpage
Animal copy = *a;   // copies only the Animal part — Dog-specific data is LOST
                    // always use references or pointers for polymorphism

// CRTP — Curiously Recurring Template Pattern (compile-time polymorphism)
template<typename Derived>
class Base {
public:
    void interface() { static_cast<Derived*>(this)->implementation(); }
};
class Concrete : public Base<Concrete> {
public:
    void implementation() { /* ... */ }
};
```

---

## 10 · Templates & generics

```cpp
// Function template / Template de fonction
template<typename T>
T maximum(T a, T b) { return a > b ? a : b; }

maximum(3, 5);          // T deduced as int
maximum(3.14, 2.71);    // T deduced as double
maximum<std::string>("alpha", "beta");  // explicit instantiation

// Class template / Template de classe
template<typename T, size_t N>
class Array {
    T data_[N];
public:
    T&       operator[](size_t i)       { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }
    size_t   size()                const { return N; }
};

Array<int, 5> a;    // stack-allocated fixed-size array

// Concepts (C++20) — constrain template parameters
#include <concepts>
template<std::integral T>
T add(T a, T b) { return a + b; }    // only compiles for integer types

template<typename T>
concept Printable = requires(T t) { std::cout << t; };  // custom concept

// Template specialisation
template<typename T>
struct is_pointer { static constexpr bool value = false; };

template<typename T>
struct is_pointer<T*> { static constexpr bool value = true; };

// Variadic templates (C++11)
template<typename... Args>
void print_all(Args&&... args) {
    (std::cout << ... << args);   // fold expression (C++17)
}
```

---

## 11 · STL containers

```cpp
#include <vector>       // dynamic array
#include <array>        // fixed-size array (stack, zero-overhead)
#include <list>         // doubly-linked list
#include <deque>        // double-ended queue
#include <map>          // sorted key-value (red-black tree), O(log n)
#include <unordered_map>// hash map, O(1) average
#include <set>          // sorted unique values
#include <unordered_set>// hash set
#include <stack>        // LIFO adapter
#include <queue>        // FIFO adapter
#include <string>       // sequence of characters

// ── std::vector — use by default for sequences
std::vector<int> v = {1, 2, 3};
v.push_back(4);             // append — may reallocate (amortised O(1))
v.emplace_back(5);          // construct in-place — avoids a copy
v.reserve(100);             // pre-allocate to avoid reallocation
v.size();                   // number of elements
v.capacity();               // allocated space
v[0];                       // unchecked access — UB if out of bounds
v.at(0);                    // checked access — throws std::out_of_range

// Range-based for loop (C++11)
for (const auto& elem : v) { std::cout << elem << ' '; }

// ── std::array — fixed size, stack allocated, no overhead
std::array<int, 5> arr = {1, 2, 3, 4, 5};
arr.size();     // 5, constexpr
arr.at(10);     // throws — unlike raw arrays which overflow silently

// ── std::map — sorted associative container
std::map<std::string, int> ages;
ages["Alice"] = 30;
ages.emplace("Bob", 25);
if (auto it = ages.find("Alice"); it != ages.end()) {
    std::cout << it->second;    // structured binding alternative below
}

// ── std::unordered_map — O(1) average lookup
std::unordered_map<std::string, int> fast_map;
fast_map["key"] = 42;

// ── Structured bindings (C++17) — unpack pairs/tuples/structs
for (const auto& [name, age] : ages) {
    std::cout << name << ": " << age << '\n';
}

// ── Container choice guide / Guide de choix
// ┌──────────────────────────────────────────────────────┐
// │ Random access, mostly append?     → vector           │
// │ Fixed size known at compile time? → array            │
// │ Fast insert/remove in middle?     → list (rarely)    │
// │ Key-value, ordered?               → map              │
// │ Key-value, fast lookup?           → unordered_map    │
// │ Unique values?                    → set/unordered_set│
// └──────────────────────────────────────────────────────┘
```

---

## 12 · STL algorithms

```cpp
#include <algorithm>
#include <numeric>
#include <ranges>   // C++20

std::vector<int> v = {5, 3, 1, 4, 2};

// Sorting / Tri
std::sort(v.begin(), v.end());                          // ascending
std::sort(v.begin(), v.end(), std::greater<int>());     // descending
std::sort(v.begin(), v.end(), [](int a, int b){ return a > b; }); // lambda

// Searching / Recherche
auto it = std::find(v.begin(), v.end(), 3);             // O(n) linear
auto it2 = std::lower_bound(v.begin(), v.end(), 3);     // O(log n) — requires sorted

// Transforming / Transformation
std::vector<int> doubled(v.size());
std::transform(v.begin(), v.end(), doubled.begin(), [](int x){ return x * 2; });

// Accumulation
int sum = std::accumulate(v.begin(), v.end(), 0);       // 0 is the initial value
int product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());

// Partitioning, removing
std::vector<int> evens;
std::copy_if(v.begin(), v.end(), std::back_inserter(evens), [](int x){ return x%2==0; });

auto new_end = std::remove(v.begin(), v.end(), 3);      // "erase-remove" idiom
v.erase(new_end, v.end());

// C++20 Ranges — cleaner, no iterator pairs
std::ranges::sort(v);
auto view = v | std::views::filter([](int x){ return x > 2; })
              | std::views::transform([](int x){ return x * 10; });
```

---

## 13 · Lambdas & closures

```cpp
// Syntax: [capture](parameters) -> return_type { body }
//         [capture](parameters) { body }  — return type deduced

// Basic lambda / Lambda de base
auto square = [](int x) { return x * x; };
square(5);   // 25

// Capture by value [=] — captures a COPY of all local variables
int multiplier = 3;
auto times = [=](int x) { return x * multiplier; };    // multiplier captured by copy

// Capture by reference [&] — captures reference to local variables
// ⚠️ Danger: if the lambda outlives the local variable, dangling reference!
int count = 0;
auto increment = [&count]() { ++count; };   // captures only 'count' by reference
increment();
// count == 1

// Capture specific variables
auto mixed = [multiplier, &count](int x) { count += x; return x * multiplier; };

// Mutable lambda — allows modifying captured-by-value variables
auto counter = [n = 0]() mutable { return ++n; };
counter();   // 1
counter();   // 2

// Generic lambda (C++14) — auto parameters create a template
auto add = [](auto a, auto b) { return a + b; };
add(1, 2);         // int
add(1.5, 2.5);     // double
add("a"s, "b"s);   // string

// Immediately invoked lambda
int result = [](int x, int y){ return x + y; }(10, 20);   // 30

// Storing lambdas
std::function<int(int)> fn = [](int x) { return x * 2; };  // type-erased
auto fn2 = [](int x) { return x * 2; };                    // concrete type, faster
```

---

## 14 · Move semantics & rvalue references

```cpp
// lvalue: has a name, has an address → can appear on LEFT of =
// rvalue: temporary, no persistent address → can appear only on RIGHT of =

int x = 5;          // x is an lvalue
int y = x + 3;      // x+3 is an rvalue (temporary)

// rvalue reference (T&&) — binds to temporaries, enables "stealing"
std::string make_greeting() { return "Hello, World!"; }

std::string&& rref = make_greeting();   // extends lifetime of temporary

// std::move — casts lvalue to rvalue (tells compiler: "I'm done with this")
std::string a = "expensive string";
std::string b = std::move(a);   // MOVE: b steals a's buffer — O(1)
// a is now in a "valid but unspecified" state — don't use it without reassigning

// std::forward — perfect forwarding (preserves value category)
template<typename T>
void wrapper(T&& arg) {
    real_function(std::forward<T>(arg));  // forwards lvalue as lvalue, rvalue as rvalue
}

// When is move called automatically?
// • Return value (NRVO may elide the copy entirely)
// • Inserting a temporary into a container
// • Passing a temporary to a function expecting &&

// Move semantics performance impact
std::vector<std::string> vec;
std::string big(1'000'000, 'x');   // 1 MB string
vec.push_back(big);                // COPY: 1 MB allocation + copy
vec.push_back(std::move(big));     // MOVE: just pointer swap — O(1)
```

---

## 15 · Error handling

```cpp
#include <stdexcept>
#include <system_error>

// Exceptions — the idiomatic C++ error handling mechanism
// ─────────────────────────────────────────────────────────
try {
    if (value < 0) throw std::invalid_argument("value must be non-negative");
    if (value > 100) throw std::out_of_range("value exceeds maximum");
    risky_operation();
}
catch (const std::out_of_range& e) {
    std::cerr << "Range error: " << e.what() << '\n';
}
catch (const std::invalid_argument& e) {
    std::cerr << "Bad argument: " << e.what() << '\n';
}
catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';   // catch-all for std exceptions
}
catch (...) {
    std::cerr << "Unknown error\n";    // catch-all for anything thrown
}

// Standard exception hierarchy (common ones)
// std::exception
// ├── std::runtime_error  — errors detectable only at runtime
// │   ├── std::overflow_error
// │   ├── std::underflow_error
// │   └── std::out_of_range
// └── std::logic_error    — violations of logical preconditions
//     ├── std::invalid_argument
//     ├── std::domain_error
//     └── std::length_error

// Custom exception
struct NetworkError : public std::runtime_error {
    int status_code;
    NetworkError(int code, const std::string& msg)
        : std::runtime_error(msg), status_code(code) {}
};

// noexcept — promise that a function never throws
// Critical for move operations (enables optimisations in STL)
void safe_swap(Buffer& a, Buffer& b) noexcept {
    std::swap(a, b);
}

// std::optional (C++17) — a value that may or may not exist (no exceptions needed)
#include <optional>
std::optional<int> parse_int(const std::string& s) {
    try   { return std::stoi(s); }
    catch (...) { return std::nullopt; }
}
if (auto result = parse_int("42")) {
    std::cout << *result;
}

// std::expected (C++23) — result OR error, without exceptions
#include <expected>
std::expected<int, std::string> divide(int a, int b) {
    if (b == 0) return std::unexpected("division by zero");
    return a / b;
}
```

---

## 16 · Namespaces & modules

```cpp
// Namespaces prevent name collisions
namespace mylib {
    namespace math {
        double pi = 3.14159265358979;
        double circle_area(double r) { return pi * r * r; }
    }
}

mylib::math::circle_area(5.0);   // fully qualified

// using declaration — imports one name
using mylib::math::pi;
double area = pi * r * r;

// using namespace — imports ALL names (avoid in headers — pollutes caller's namespace)
using namespace std;   // fine in .cpp files, NEVER in .h/.hpp files

// Anonymous namespace — internal linkage (like static in C)
namespace {
    void helper() { /* visible only in this translation unit */ }
}

// Inline namespace (C++11) — versioning
namespace lib {
    inline namespace v2 {
        void function() { /* v2 implementation */ }
    }
    namespace v1 {
        void function() { /* v1 implementation */ }
    }
}
lib::function();     // calls v2 (inline)
lib::v1::function(); // explicitly v1

// Modules (C++20) — replace #include, faster compilation, no header guards needed
// export module mylib;
// export void hello() { std::cout << "hello\n"; }
// ───
// import mylib;
// hello();
```

---

## 17 · Modern C++ idioms (C++17/20)

```cpp
// ── if/switch with initialiser (C++17)
if (auto it = map.find(key); it != map.end()) {
    use(it->second);
}   // `it` goes out of scope here — not accessible outside the if

// ── std::optional (C++17)
std::optional<std::string> find_user(int id);
auto user = find_user(42);
user.value_or("unknown");   // default if empty
if (user) { std::cout << *user; }

// ── std::variant (C++17) — type-safe union
std::variant<int, double, std::string> v = "hello";
std::get<std::string>(v);                // access by type
std::holds_alternative<std::string>(v); // check type
std::visit([](auto&& val){ std::cout << val; }, v);  // visitor pattern

// ── std::string_view (C++17) — non-owning string reference (no allocation)
void process(std::string_view sv) { /* read-only, no copy */ }
process("hello");                    // no std::string construction
process(some_string.substr(0, 5));   // substring — no copy

// ── Structured bindings (C++17)
auto [min, max] = std::minmax_element(v.begin(), v.end());
auto& [key, value] = *map.begin();

// ── std::span (C++20) — non-owning view over contiguous memory
#include <span>
void sum(std::span<const int> data) {
    return std::accumulate(data.begin(), data.end(), 0);
}
sum(vec);        // from vector
sum(raw_array);  // from C array

// ── Designated initialisers (C++20)
struct Config { int width=800; int height=600; bool fullscreen=false; };
Config c{ .width=1920, .height=1080 };

// ── consteval (C++20) — MUST be evaluated at compile time
consteval int square(int n) { return n * n; }
constexpr int s = square(5);   // 25 — computed at compile time

// ── [[likely]] / [[unlikely]] (C++20) — branch prediction hint
if [[unlikely]] (ptr == nullptr) { handle_null(); }
```

---

## 18 · Compiler flags & tools

### Recommended toolchain / Outillage recommandé

| Tool | Purpose | Command |
|---|---|---|
| **g++ / clang++** | Compiler | `g++ -std=c++17 -Wall -Wextra -g` |
| **GDB** | Debugger (Linux/WSL) | `gdb ./program` |
| **LLDB** | Debugger (macOS) | `lldb ./program` |
| **Valgrind** | Memory leak detector | `valgrind --leak-check=full ./program` |
| **AddressSanitizer** | Runtime memory errors | `-fsanitize=address,undefined` |
| **UBSan** | Undefined behaviour | `-fsanitize=undefined` |
| **clang-format** | Code formatter | `clang-format -i *.cpp` |
| **clang-tidy** | Static analyser | `clang-tidy *.cpp` |
| **cppcheck** | Static analyser (free) | `cppcheck --enable=all src/` |
| **Compiler Explorer** | Live disassembly | https://godbolt.org |
| **cpp reference** | Standard library docs | https://cppreference.com |

### GDB Quick Reference

```bash
gdb ./program          # start GDB
run                    # run the program
break main             # breakpoint at main()
break file.cpp:42      # breakpoint at line 42
next (n)               # step over
step (s)               # step into
continue (c)           # run until next breakpoint
print x                # print value of x
print *ptr             # dereference and print
info locals            # show all local variables
backtrace (bt)         # call stack
frame 2                # switch to stack frame 2
watch x                # break when x changes
quit (q)               # exit GDB
```

### Valgrind Memory Check

```bash
# Compile with debug symbols first
g++ -std=c++17 -g -o program main.cpp

# Run under Valgrind
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program

# Output interpretation:
# "definitely lost" = clear leak — you must fix this
# "indirectly lost" = caused by "definitely lost"
# "possibly lost"   = likely a leak (complex cases)
# "still reachable" = freed at exit — technically not a leak
```

---

## 19 · Quick reference card

```
MEMORY                              SMART POINTERS
──────────────────────────────────  ────────────────────────────────────
stack: int x; std::string s;        sole owner:  unique_ptr<T>
heap (raw):  new / delete           shared:      shared_ptr<T>
heap (SAFE): make_unique<T>()       non-owning:  weak_ptr<T>
array:       make_unique<T[]>(n)    preferred:   make_unique / make_shared

RULE OF 5                           CAST OPERATORS
──────────────────────────────────  ────────────────────────────────────
~Dtor                               numeric:   static_cast<T>(x)
copy ctor                           hierarchy: dynamic_cast<T*>(p)
copy =                              const:     const_cast<T>(x)
move ctor          noexcept         bits:      reinterpret_cast<T>(x)
move =             noexcept

CONTAINERS (default choice)         RANGES
──────────────────────────────────  ────────────────────────────────────
sequence:   vector<T>               std::ranges::sort(v)
key-value:  unordered_map<K,V>      v | views::filter(pred)
ordered:    map<K,V>                v | views::transform(fn)
unique set: unordered_set<T>        v | views::take(n)

MODERN IDIOMS                       ERROR HANDLING
──────────────────────────────────  ────────────────────────────────────
optional<T>     — nullable value    throw / try / catch
variant<A,B,C>  — safe union        noexcept on move ops
string_view     — no-alloc string   std::optional (no exception)
span<T>         — no-alloc slice    std::expected (C++23)
structured bind — auto [a,b]=pair
```

---

*C++ CodeSamples — see also `hello-world/` and `memory-management/` projects.*
*See: https://cppreference.com · https://isocpp.github.io/CppCoreGuidelines/*
