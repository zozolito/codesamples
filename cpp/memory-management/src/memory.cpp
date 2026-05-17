/**
 * memory.cpp — C++ Memory Management Masterclass
 * CodeSamples Project
 *
 * This file walks through EVERY important memory management concept in C++:
 *
 *   Part 1 · Stack vs Heap
 *   Part 2 · Raw pointers — the dangerous way
 *   Part 3 · Memory errors demonstrated (leaks, double-free, dangling, overflow)
 *   Part 4 · RAII — the fundamental solution
 *   Part 5 · unique_ptr — sole ownership
 *   Part 6 · shared_ptr + weak_ptr — shared ownership
 *   Part 7 · Rule of 5 — writing your own RAII class
 *   Part 8 · std::vector vs raw array
 *   Part 9 · Detecting errors: AddressSanitizer + Valgrind
 *
 * Compile (development — sanitizers enabled):
 *   g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined \
 *       -o bin/memory src/memory.cpp
 *
 * Compile (release):
 *   g++ -std=c++17 -O2 -o bin/memory src/memory.cpp
 *
 * Run Valgrind (Linux — no sanitizers in compile flags):
 *   g++ -std=c++17 -g -o bin/memory src/memory.cpp
 *   valgrind --leak-check=full --track-origins=yes ./bin/memory
 */

#include <iostream>
#include <memory>       // unique_ptr, shared_ptr, weak_ptr, make_unique, make_shared
#include <string>
#include <vector>
#include <array>
#include <cassert>
#include <algorithm>
#include <cstring>      // memcpy, strlen
#include <utility>      // std::move, std::exchange

// ─────────────────────────────────────────────────────────────────────────────
// Utility
// ─────────────────────────────────────────────────────────────────────────────

void section(const char* title) {
    std::cout << "\n╔══════════════════════════════════════════════════════╗\n"
              << "║  " << title << "\n"
              << "╚══════════════════════════════════════════════════════╝\n";
}

void subsection(const char* title) {
    std::cout << "\n  ── " << title << " ──\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// PART 1 · STACK VS HEAP
// ─────────────────────────────────────────────────────────────────────────────

void demo_stack_vs_heap() {
    section("PART 1 · STACK vs HEAP");

    // ── STACK ──
    // Variables declared here live on the stack.
    // They are created when execution enters the block,
    // and AUTOMATICALLY destroyed when execution leaves the block.
    // No cleanup code needed. Fast. Size must be known at compile time (for arrays).
    {
        int    stack_int  = 42;
        double stack_dbl  = 3.14;
        char   stack_arr[8] = "hello";   // 8-byte array on the stack

        // std::string and std::vector LOOK like stack objects but they
        // internally allocate their buffer on the heap — however, THEY manage
        // that heap memory themselves (RAII). You never call delete on them.
        std::string stack_str = "world";
        std::vector<int> stack_vec = {1, 2, 3};

        std::cout << "  [Stack] int=" << stack_int
                  << " double=" << stack_dbl
                  << " string=" << stack_str << '\n';

        // &stack_int is the address on the stack (usually high addresses on x86)
        std::cout << "  [Stack] address of stack_int: " << &stack_int << '\n';

    }   // <-- ALL of the above is automatically freed here. No delete needed.

    // ── HEAP ──
    // Objects allocated with 'new' live on the heap until you 'delete' them.
    // The heap is a large pool of memory managed by the OS/runtime.
    // You are responsible for freeing it — the compiler won't remind you.
    {
        int*  heap_int = new int(42);          // allocate ONE int on heap
        int*  heap_arr = new int[5]{1,2,3,4,5}; // allocate ARRAY of 5 ints

        std::cout << "  [Heap] *heap_int=" << *heap_int << '\n';
        std::cout << "  [Heap] address of heap_int data: " << heap_int << '\n';

        // MUST free heap memory — or it leaks until the process exits
        delete   heap_int;    // free a single object
        delete[] heap_arr;    // free an array — MUST match new[]

        heap_int = nullptr;   // good practice: null the pointer after delete
        heap_arr = nullptr;   // prevents accidental use-after-free
    }

    std::cout << "\n  Summary:\n"
              << "  Stack: fast, automatic, limited size (~1–8 MB)\n"
              << "  Heap:  flexible size, manual management (or use smart pointers)\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// PART 2 · RAW POINTERS — THE DANGEROUS WAY
// ─────────────────────────────────────────────────────────────────────────────

void demo_raw_pointers() {
    section("PART 2 · RAW POINTERS — the dangerous way");

    // Pointer basics
    int value = 100;
    int* ptr  = &value;    // ptr holds the ADDRESS of 'value'

    std::cout << "  value  = " << value  << '\n';
    std::cout << "  ptr    = " << ptr    << "  (address)\n";
    std::cout << "  *ptr   = " << *ptr   << "  (dereference — same as value)\n";

    *ptr = 200;     // modifying value through the pointer
    std::cout << "  After *ptr=200: value=" << value << '\n';

    // Pointer arithmetic — only valid within an array
    int arr[5] = {10, 20, 30, 40, 50};
    int* p = arr;   // arr decays to pointer to first element

    std::cout << "\n  Pointer arithmetic over array:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "  *(p+" << i << ") = " << *(p + i) << '\n';
    }

    // Function pointer
    auto add = [](int a, int b) { return a + b; };
    int result = add(3, 4);
    std::cout << "\n  Lambda result: " << result << '\n';
}

// ─────────────────────────────────────────────────────────────────────────────
// PART 3 · MEMORY ERRORS — WHAT GOES WRONG
// Each sub-demo is carefully crafted to show the error WITHOUT actually
// triggering it (the buggy code is commented out with ⚠️ markers).
// Compile with -fsanitize=address to have the sanitizer catch them.
// ─────────────────────────────────────────────────────────────────────────────

void demo_memory_errors() {
    section("PART 3 · MEMORY ERRORS (commented — would crash/leak)");

    // ──────────────────────────────────────────────────────
    // 3a. MEMORY LEAK / FUITE MÉMOIRE
    // ──────────────────────────────────────────────────────
    subsection("3a · Memory Leak");
    std::cout << "  Code:\n"
              << "    int* p = new int(5);\n"
              << "    // ... forgot to delete p\n"
              << "    // p goes out of scope → LEAK: 4 bytes lost forever\n\n"
              << "  ⚠️  Not executed — would leak silently.\n"
              << "  Detection: valgrind OR -fsanitize=address\n";

    // ACTUAL SAFE CODE:
    {
        int* p = new int(5);
        std::cout << "  Safe: allocated " << *p << ", now deleting.\n";
        delete p;   // properly freed
        p = nullptr;
    }

    // ──────────────────────────────────────────────────────
    // 3b. DOUBLE FREE / DOUBLE LIBÉRATION
    // ──────────────────────────────────────────────────────
    subsection("3b · Double Free");
    std::cout << "  Code:\n"
              << "    int* p = new int(5);\n"
              << "    delete p;    // first delete — OK\n"
              << "    delete p;    // second delete — CRASH / undefined behaviour\n\n"
              << "  ⚠️  Not executed — would corrupt the heap allocator.\n"
              << "  This is a critical security vulnerability (CVE-class bug).\n";

    // ──────────────────────────────────────────────────────
    // 3c. DANGLING POINTER / POINTEUR FANTÔME
    // ──────────────────────────────────────────────────────
    subsection("3c · Dangling Pointer");
    std::cout << "  Code:\n"
              << "    int* p;\n"
              << "    {\n"
              << "        int local = 42;\n"
              << "        p = &local;   // p points to local\n"
              << "    }               // local is destroyed!\n"
              << "    *p = 99;          // ⚠️ writing to destroyed stack memory\n\n"
              << "  Or with heap:\n"
              << "    int* q = new int(5);\n"
              << "    delete q;\n"
              << "    *q = 10;          // ⚠️ use-after-free — security hole!\n\n"
              << "  ⚠️  Not executed — undefined behaviour, possible crash.\n"
              << "  Fix: set pointer to nullptr after delete.\n"
              << "  Fix: use smart pointers — they prevent this entirely.\n";

    // ──────────────────────────────────────────────────────
    // 3d. BUFFER OVERFLOW / DÉBORDEMENT DE TAMPON
    // ──────────────────────────────────────────────────────
    subsection("3d · Buffer Overflow");
    std::cout << "  Code:\n"
              << "    int* arr = new int[3];\n"
              << "    arr[0] = 1;   // OK\n"
              << "    arr[3] = 99;  // ⚠️ index 3 is OUT OF BOUNDS (0,1,2 valid)\n"
              << "                  // writes beyond allocated block\n"
              << "                  // corrupts adjacent memory → crash or exploit\n\n"
              << "  ⚠️  Not executed.\n"
              << "  Fix: use std::vector::at() (throws) or std::array (compile-time bounds).\n"
              << "  Detection: -fsanitize=address (AddressSanitizer)\n";

    // SAFE DEMONSTRATION of bounds checking
    {
        std::vector<int> safe_arr = {1, 2, 3};
        try {
            int bad = safe_arr.at(5);   // .at() throws instead of corrupting memory
            (void)bad;
        } catch (const std::out_of_range& e) {
            std::cout << "\n  Safe demo: safe_arr.at(5) threw: " << e.what() << '\n';
        }
    }

    // ──────────────────────────────────────────────────────
    // 3e. WRONG DELETE / MAUVAIS DELETE
    // ──────────────────────────────────────────────────────
    subsection("3e · Wrong delete (delete vs delete[])");
    std::cout << "  Code:\n"
              << "    int* arr = new int[10];\n"
              << "    delete arr;    // ⚠️ WRONG — must use delete[]\n"
              << "    // Only frees the first element — rest leaks / UB\n\n"
              << "  Rule: new[]  → delete[]\n"
              << "        new    → delete\n"
              << "  Better: use make_unique<int[]>(10) — no delete needed.\n";

    // ──────────────────────────────────────────────────────
    // 3f. NULL DEREFERENCE / DÉRÉFÉRENCEMENT NULL
    // ──────────────────────────────────────────────────────
    subsection("3f · Null Dereference");
    std::cout << "  Code:\n"
              << "    int* p = nullptr;\n"
              << "    *p = 42;     // ⚠️ CRASH — segmentation fault\n\n"
              << "  Always check pointers before dereferencing:\n"
              << "    if (p != nullptr) { *p = 42; }\n"
              << "  With smart pointers: check with if(ptr) before *ptr.\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// PART 4 · RAII — THE FUNDAMENTAL SOLUTION
// ─────────────────────────────────────────────────────────────────────────────

// A simple RAII file wrapper — demonstrates the principle with a real resource
class RAIIFile {
    FILE* file_;
    std::string path_;

public:
    explicit RAIIFile(const std::string& path, const char* mode)
        : file_(std::fopen(path.c_str(), mode)), path_(path) {
        if (!file_) {
            throw std::runtime_error("Cannot open file: " + path);
        }
        std::cout << "    [RAIIFile] opened: " << path << '\n';
    }

    // Destructor — automatically called when the object leaves scope
    // This is the heart of RAII: cleanup is tied to lifetime
    ~RAIIFile() {
        if (file_) {
            std::fclose(file_);
            std::cout << "    [RAIIFile] closed: " << path_
                      << " (automatic — even if exception was thrown)\n";
        }
    }

    // Delete copy — file handles shouldn't be copied
    RAIIFile(const RAIIFile&)            = delete;
    RAIIFile& operator=(const RAIIFile&) = delete;

    // Allow move
    RAIIFile(RAIIFile&& other) noexcept
        : file_(std::exchange(other.file_, nullptr)), path_(std::move(other.path_)) {}

    FILE* get() const { return file_; }
};

void demo_raii() {
    section("PART 4 · RAII — Resource Acquisition Is Initialisation");

    std::cout << "  RAII principle:\n"
              << "  • Constructor acquires the resource\n"
              << "  • Destructor releases it\n"
              << "  • Resource is ALWAYS released — even if an exception occurs\n\n";

    // Without RAII (dangerous):
    std::cout << "  Without RAII:\n"
              << "    FILE* f = fopen(\"x.txt\", \"r\");\n"
              << "    do_work();       // if this throws → f is never closed → LEAK\n"
              << "    fclose(f);       // only reached if no exception\n\n";

    // With RAII:
    std::cout << "  With RAII:\n";
    {
        try {
            RAIIFile f("/dev/null", "r");   // works on Linux/macOS; skipped on Windows
            std::cout << "    Working with file...\n";
            // Destructor closes f automatically here — exception or not
        } catch (const std::runtime_error& e) {
            // On Windows /dev/null doesn't exist — that's fine for the demo
            std::cout << "    (demo note: " << e.what() << " — still shows RAII pattern)\n";
        }
    }   // RAIIFile destructor called here regardless

    std::cout << "\n  std::string, std::vector, std::fstream are all RAII classes.\n"
              << "  Smart pointers are RAII classes for heap memory.\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// PART 5 · UNIQUE_PTR — SOLE OWNERSHIP
// ─────────────────────────────────────────────────────────────────────────────

struct Widget {
    int id;
    std::string name;
    Widget(int i, std::string n) : id(i), name(std::move(n)) {
        std::cout << "    [Widget " << id << "] constructed\n";
    }
    ~Widget() {
        std::cout << "    [Widget " << id << "] destroyed (automatic)\n";
    }
};

void demo_unique_ptr() {
    section("PART 5 · unique_ptr — Sole Ownership");

    std::cout << "  Creating widgets with make_unique:\n";
    {
        // make_unique<T>(args) — ALWAYS use this instead of `new`
        // It is exception-safe and makes ownership crystal-clear
        auto w1 = std::make_unique<Widget>(1, "Button");
        auto w2 = std::make_unique<Widget>(2, "TextBox");

        std::cout << "  Using w1: " << w1->name << '\n';
        std::cout << "  Using w2: " << w2->name << '\n';

        // Cannot copy a unique_ptr — ownership is exclusive
        // auto w3 = w1;           // COMPILE ERROR

        // CAN move — transfers ownership, w1 becomes null
        auto w3 = std::move(w1);
        std::cout << "  After move: w1 is " << (w1 ? "not null" : "null") << '\n';
        std::cout << "  w3 now owns: " << w3->name << '\n';

        // Check before use
        if (w1) { std::cout << "  w1 still alive\n"; }
        else    { std::cout << "  w1 is null — cannot use it\n"; }

    }   // w2 and w3 go out of scope here — Widget destructors are called automatically
        // No delete needed anywhere

    std::cout << "\n  Array variant:\n";
    {
        // unique_ptr can manage arrays too
        auto arr = std::make_unique<int[]>(5);
        for (int i = 0; i < 5; ++i) arr[i] = i * i;
        std::cout << "  arr[3] = " << arr[3] << '\n';
    }   // delete[] called automatically

    // Passing unique_ptr to functions
    auto pass_by_ref = [](const std::unique_ptr<Widget>& w) {
        // Pass by const ref — borrow without transferring ownership
        std::cout << "  Borrowed: " << w->name << '\n';
    };

    auto take_ownership = [](std::unique_ptr<Widget> w) {
        // Pass by value (requires std::move at call site) — caller loses ownership
        std::cout << "  Took ownership of: " << w->name << '\n';
    };   // w destroyed here

    std::cout << "\n  Function parameter patterns:\n";
    auto w = std::make_unique<Widget>(3, "Checkbox");
    pass_by_ref(w);
    take_ownership(std::move(w));    // w is now null
}

// ─────────────────────────────────────────────────────────────────────────────
// PART 6 · SHARED_PTR + WEAK_PTR
// ─────────────────────────────────────────────────────────────────────────────

void demo_shared_ptr() {
    section("PART 6 · shared_ptr + weak_ptr — Shared & Non-owning References");

    subsection("shared_ptr — reference counting");
    {
        auto sp1 = std::make_shared<Widget>(10, "SharedWidget");
        std::cout << "  sp1 use_count: " << sp1.use_count() << '\n';   // 1

        {
            auto sp2 = sp1;    // copy — both own the Widget; ref count = 2
            auto sp3 = sp1;    // ref count = 3
            std::cout << "  sp1 use_count after 2 copies: " << sp1.use_count() << '\n';   // 3
        }   // sp2 and sp3 go out of scope; ref count drops to 1

        std::cout << "  sp1 use_count after sp2/sp3 gone: " << sp1.use_count() << '\n';  // 1
    }   // sp1 out of scope — ref count 0 — Widget destroyed

    subsection("Circular reference problem (memory leak with shared_ptr)");
    std::cout << "  Code:\n"
              << "    struct Node {\n"
              << "        std::shared_ptr<Node> next;  // ← circular!\n"
              << "    };\n"
              << "    auto a = make_shared<Node>();\n"
              << "    auto b = make_shared<Node>();\n"
              << "    a->next = b;  // b ref_count = 2\n"
              << "    b->next = a;  // a ref_count = 2\n"
              << "    // Both go out of scope → ref count drops to 1, not 0 → LEAK\n\n"
              << "  Fix: use weak_ptr for back-references\n";

    subsection("weak_ptr — breaks cycles, non-owning observation");
    {
        auto sp = std::make_shared<Widget>(20, "Observed");
        std::weak_ptr<Widget> wp = sp;    // does NOT increment ref count

        std::cout << "  sp use_count: " << sp.use_count() << '\n';  // still 1

        // Must call .lock() to get a temporary shared_ptr before accessing
        if (auto locked = wp.lock()) {
            std::cout << "  Accessed via weak_ptr: " << locked->name << '\n';
        }

        // Simulate shared_ptr going away
        sp.reset();   // releases ownership — Widget is destroyed

        // After reset, weak_ptr is expired
        if (wp.expired()) {
            std::cout << "  weak_ptr is expired — Widget was destroyed\n";
        }
        if (auto locked = wp.lock()) {
            std::cout << "  Still alive\n";
        } else {
            std::cout << "  wp.lock() returned nullptr — safe, no crash\n";
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// PART 7 · RULE OF 5 — WRITING YOUR OWN RAII CLASS
// ─────────────────────────────────────────────────────────────────────────────

class DynArray {
    int*   data_;
    size_t size_;

    void log(const char* what) const {
        std::cout << "    [DynArray size=" << size_ << "] " << what << '\n';
    }

public:
    // Constructor — acquires the resource
    explicit DynArray(size_t n)
        : data_(n > 0 ? new int[n]() : nullptr), size_(n) {
        // int[n]() — value-initialises to zero (the () matters!)
        log("constructed");
    }

    // 1. Destructor — releases the resource
    ~DynArray() {
        delete[] data_;
        log("destroyed");
    }

    // 2. Copy constructor — DEEP copy (not shallow)
    DynArray(const DynArray& other)
        : data_(other.size_ > 0 ? new int[other.size_] : nullptr)
        , size_(other.size_) {
        if (data_) std::copy(other.data_, other.data_ + size_, data_);
        log("copy-constructed");
    }

    // 3. Copy assignment — free old resource, deep copy new
    DynArray& operator=(const DynArray& other) {
        if (this == &other) return *this;   // self-assignment guard — CRITICAL
        delete[] data_;
        size_ = other.size_;
        data_ = size_ > 0 ? new int[size_] : nullptr;
        if (data_) std::copy(other.data_, other.data_ + size_, data_);
        log("copy-assigned");
        return *this;
    }

    // 4. Move constructor — STEAL the resource (O(1), no allocation)
    // noexcept is critical: STL containers can only use move ops that are noexcept
    DynArray(DynArray&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))   // std::exchange: set other.data_ to nullptr, return old value
        , size_(std::exchange(other.size_, 0)) {
        log("move-constructed");
    }

    // 5. Move assignment
    DynArray& operator=(DynArray&& other) noexcept {
        if (this == &other) return *this;
        delete[] data_;   // free current resource before stealing
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        log("move-assigned");
        return *this;
    }

    // Accessors
    int&       operator[](size_t i)       { return data_[i]; }
    const int& operator[](size_t i) const { return data_[i]; }
    size_t     size()                const { return size_; }
};

void demo_rule_of_5() {
    section("PART 7 · Rule of 5 — Writing Your Own RAII Class");

    std::cout << "  Creating original:\n";
    DynArray original(4);
    original[0] = 10; original[1] = 20; original[2] = 30; original[3] = 40;

    std::cout << "  Deep copy:\n";
    DynArray copy = original;   // copy constructor
    copy[0] = 999;              // does NOT affect original

    std::cout << "  original[0]=" << original[0] << " copy[0]=" << copy[0] << '\n';
    std::cout << "  (different memory — deep copy worked)\n";

    std::cout << "  Move:\n";
    DynArray moved = std::move(original);   // move constructor — O(1)
    std::cout << "  original.size() after move: " << original.size()
              << " (data stolen, source is empty)\n";
    std::cout << "  moved[2]=" << moved[2] << '\n';

    std::cout << "  Leaving scope — destructors will run:\n";
}   // copy and moved destroyed here (original is already empty)

// ─────────────────────────────────────────────────────────────────────────────
// PART 8 · STD::VECTOR VS RAW ARRAY
// ─────────────────────────────────────────────────────────────────────────────

void demo_vector_vs_array() {
    section("PART 8 · std::vector vs raw array");

    subsection("Raw C array (avoid for dynamic sizes)");
    {
        // Fixed size — known at compile time
        int raw[5] = {1, 2, 3, 4, 5};
        std::cout << "  raw[4] = " << raw[4] << '\n';
        // raw[10] = 99;  // ⚠️ buffer overflow — no bounds check!

        // Cannot resize raw arrays — must allocate new array and copy
        // This is exactly what std::vector does internally (and correctly)
    }

    subsection("std::vector (use this)");
    {
        std::vector<int> v;
        v.reserve(10);    // hint: pre-allocate 10 elements' worth of memory
                          // avoids reallocations as we push_back

        for (int i = 0; i < 8; ++i) v.push_back(i * i);

        std::cout << "  size=" << v.size() << " capacity=" << v.capacity() << '\n';
        // size = number of elements actually stored
        // capacity = number of elements that can be stored without reallocation

        v.at(3);    // bounds-checked — throws std::out_of_range
        v[3];       // unchecked — use only when you're certain of bounds

        // Iteration — prefer range-for or iterators
        for (int x : v) std::cout << "  " << x;
        std::cout << '\n';

        // std::vector manages its own memory — no delete needed
    }   // vector's destructor frees the heap buffer

    subsection("std::array (fixed size, stack-allocated, zero overhead)");
    {
        std::array<int, 5> a = {10, 20, 30, 40, 50};
        a.at(2);    // bounds-checked
        std::cout << "  array size (compile-time): " << a.size() << '\n';
        // a is on the stack — no heap allocation at all
    }

    std::cout << "\n  Rule:\n"
              << "  • Size known at compile time → std::array\n"
              << "  • Size known at runtime      → std::vector\n"
              << "  • Never use raw new int[n]   → use vector or make_unique<int[]>(n)\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// PART 9 · DETECTING ERRORS: ASAN + VALGRIND
// ─────────────────────────────────────────────────────────────────────────────

void demo_detection_tools() {
    section("PART 9 · Detecting Memory Errors — AddressSanitizer & Valgrind");

    std::cout << R"(
  ── AddressSanitizer (ASan) ──
  Compile:  g++ -std=c++17 -g -fsanitize=address,undefined -o prog main.cpp
  Run:      ./prog
  Detects:  heap/stack/global buffer overflow, use-after-free,
            use-after-scope, double-free, memory leaks (with ASAN_OPTIONS)
  Cost:     ~2× slowdown — use only for testing, not in production

  ── UndefinedBehaviorSanitizer (UBSan) ──
  Flag:     -fsanitize=undefined
  Detects:  signed integer overflow, null dereference, misaligned access,
            invalid shift, out-of-bounds array access (for std::array)

  ── Valgrind (Linux) ──
  Compile:  g++ -std=c++17 -g -o prog main.cpp   (NO -fsanitize)
  Run:      valgrind --leak-check=full --track-origins=yes ./prog
  Detects:  all leaks, invalid reads/writes, uninitialised values
  Cost:     ~10–50× slowdown

  ── Combine for thorough testing ──
  1. Debug build with ASan+UBSan (fast iteration, catches most bugs)
  2. Valgrind run on release build (catches everything ASan misses)
  3. Static analysis: cppcheck, clang-tidy (no execution needed)

  ── Example ASan output for a use-after-free ──
  ERROR: AddressSanitizer: heap-use-after-free on address 0x...
    READ of size 4 at 0x... thread T0
    #0 0x... in main /path/to/file.cpp:42
  Previously allocated by thread T0 here:
    #0 0x... in operator new /...
  Previously freed by thread T0 here:
    #0 0x... in operator delete /...

  ── Smart pointer rule ──
  Use smart pointers → most of these tools report 0 errors.
  That is the goal.
)";
}

// ─────────────────────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────────────────────

int main() {
    std::cout << "╔══════════════════════════════════════════════════════╗\n"
              << "║   C++ MEMORY MANAGEMENT — CodeSamples Project       ║\n"
              << "╚══════════════════════════════════════════════════════╝\n";

    demo_stack_vs_heap();
    demo_raw_pointers();
    demo_memory_errors();
    demo_raii();
    demo_unique_ptr();
    demo_shared_ptr();
    demo_rule_of_5();
    demo_vector_vs_array();
    demo_detection_tools();

    std::cout << "\n╔══════════════════════════════════════════════════════╗\n"
              << "║  GOLDEN RULES / RÈGLES D'OR                         ║\n"
              << "╠══════════════════════════════════════════════════════╣\n"
              << "║  1. Never use raw new/delete — use smart pointers   ║\n"
              << "║  2. Rule of 0: prefer RAII members, define nothing  ║\n"
              << "║  3. unique_ptr for sole ownership                   ║\n"
              << "║  4. shared_ptr for shared ownership                 ║\n"
              << "║  5. weak_ptr to break cycles                        ║\n"
              << "║  6. noexcept on all move operations                 ║\n"
              << "║  7. std::vector/array instead of raw arrays         ║\n"
              << "║  8. Compile with -fsanitize=address during dev      ║\n"
              << "║  9. Run Valgrind before shipping                    ║\n"
              << "║ 10. If ASan reports 0 errors → you're doing it right║\n"
              << "╚══════════════════════════════════════════════════════╝\n";

    return 0;
}
