/**
 * hello.cpp — C++ Hello World
 * CodeSamples Project · C++ Hello World
 *
 * Demonstrates: classes, constructors, const methods, operator overloading,
 *               std::vector, range-based for, std::string, lambdas, std::sort,
 *               std::optional, structured bindings, RAII via std::string/vector.
 *
 * Compile:
 *   g++ -std=c++17 -Wall -Wextra -g -o bin/hello src/hello.cpp
 *   ./bin/hello
 */

// Standard library headers — no raw C headers needed for modern C++
#include <iostream>     // std::cout, std::cin, std::cerr
#include <string>       // std::string
#include <vector>       // std::vector
#include <algorithm>    // std::sort, std::max_element
#include <numeric>      // std::accumulate
#include <optional>     // std::optional (C++17)
#include <iomanip>      // std::setprecision, std::fixed
#include <cmath>        // std::sqrt, M_PI
#include <sstream>      // std::ostringstream

// ─────────────────────────────────────────────────────────────────────────────
// Shape — abstract base class
// A class with a pure virtual method cannot be instantiated directly.
// ─────────────────────────────────────────────────────────────────────────────
class Shape {
protected:
    std::string name_;   // member variable — trailing _ is a common naming convention

public:
    // Constructor — 'explicit' prevents implicit conversions
    explicit Shape(std::string name) : name_(std::move(name)) {}

    // Virtual destructor — REQUIRED for base classes.
    // Without it, deleting a derived object through a base pointer
    // causes undefined behaviour (the derived destructor is never called).
    virtual ~Shape() = default;

    // Pure virtual method — every derived class MUST implement this
    virtual double area()      const = 0;
    virtual double perimeter() const = 0;

    // Non-pure virtual — has a default, can be overridden
    virtual std::string describe() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << name_
            << " | area=" << area()
            << " | perimeter=" << perimeter();
        return oss.str();
    }

    // const getter — 'const' after () means: "this method does not modify the object"
    const std::string& name() const { return name_; }

    // Operator overloading — compare shapes by area
    bool operator<(const Shape& other) const {
        return area() < other.area();
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Circle — concrete derived class
// ─────────────────────────────────────────────────────────────────────────────
class Circle : public Shape {   // 'public' inheritance: Circle IS-A Shape
    double radius_;

public:
    explicit Circle(double r)
        : Shape("Circle"), radius_(r) {}   // calls base constructor via initialiser list

    // 'override' — compiler checks we're genuinely overriding a virtual method
    double area()      const override { return M_PI * radius_ * radius_; }
    double perimeter() const override { return 2.0 * M_PI * radius_; }
};

// ─────────────────────────────────────────────────────────────────────────────
// Rectangle — another concrete derived class
// ─────────────────────────────────────────────────────────────────────────────
class Rectangle : public Shape {
    double width_, height_;

public:
    Rectangle(double w, double h)
        : Shape("Rectangle"), width_(w), height_(h) {}

    double area()      const override { return width_ * height_; }
    double perimeter() const override { return 2.0 * (width_ + height_); }

    // Additional method specific to Rectangle (not in Shape)
    double diagonal() const { return std::sqrt(width_*width_ + height_*height_); }
};

// ─────────────────────────────────────────────────────────────────────────────
// Triangle
// ─────────────────────────────────────────────────────────────────────────────
class Triangle : public Shape {
    double a_, b_, c_;   // three sides

public:
    Triangle(double a, double b, double c)
        : Shape("Triangle"), a_(a), b_(b), c_(c) {}

    double area() const override {
        // Heron's formula — no need for angles
        double s = (a_ + b_ + c_) / 2.0;
        return std::sqrt(s * (s-a_) * (s-b_) * (s-c_));
    }
    double perimeter() const override { return a_ + b_ + c_; }
};

// ─────────────────────────────────────────────────────────────────────────────
// Free functions
// ─────────────────────────────────────────────────────────────────────────────

void print_banner() {
    std::cout << "============================================\n"
              << "  C++ Hello World — CodeSamples Project\n"
              << "============================================\n\n";
}

// std::optional<T> — a return type that may or may not have a value (C++17)
// Better than returning -1.0 or throwing for "expected" empty cases
std::optional<double> safe_sqrt(double x) {
    if (x < 0.0) return std::nullopt;   // empty optional
    return std::sqrt(x);
}

// Template function — works for any numeric type
template<typename T>
T clamp_value(T value, T lo, T hi) {
    return value < lo ? lo : (value > hi ? hi : value);
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    print_banner();

    // ── 1. Greet the user ────────────────────────────────────────────────────
    std::string user_name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, user_name);   // reads the whole line, including spaces

    if (user_name.empty()) user_name = "World";

    for (int i = 1; i <= 3; ++i) {
        std::cout << " " << i << ". Hello, " << user_name << "!\n";
    }
    std::cout << '\n';

    // ── 2. Polymorphism demo ─────────────────────────────────────────────────
    // std::vector stores objects by value — but for polymorphism we need
    // pointers or references (otherwise slicing occurs: only the base part is copied).
    // We use unique_ptr here: sole ownership, automatic cleanup — no manual delete.
    std::cout << "--- Shapes (polymorphism + RAII via unique_ptr) ---\n";

    std::vector<std::unique_ptr<Shape>> shapes;

    // make_unique<T>(args) — creates a unique_ptr; NEVER use raw 'new' in modern C++
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
    shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0));
    shapes.push_back(std::make_unique<Circle>(2.5));
    shapes.push_back(std::make_unique<Rectangle>(10.0, 2.0));

    // Range-based for loop — preferred over index loops when index isn't needed
    for (const auto& s : shapes) {
        std::cout << "  " << s->describe() << '\n';
        // s->area() calls the correct override at RUNTIME (virtual dispatch)
    }

    // ── 3. Sort by area using a lambda ──────────────────────────────────────
    std::cout << "\n--- Sorted by area (lambda + std::sort) ---\n";

    std::sort(shapes.begin(), shapes.end(),
        // Lambda: [capture list](parameters) { body }
        [](const auto& a, const auto& b) {
            return a->area() < b->area();   // ascending by area
        }
    );

    for (const auto& s : shapes) {
        std::cout << "  " << s->name()
                  << "  area = " << std::fixed << std::setprecision(2) << s->area()
                  << '\n';
    }

    // ── 4. STL algorithms ────────────────────────────────────────────────────
    std::cout << "\n--- STL algorithms ---\n";

    std::vector<double> areas;
    areas.reserve(shapes.size());   // pre-allocate to avoid reallocations
    for (const auto& s : shapes) {
        areas.push_back(s->area());
    }

    // std::accumulate — sum all elements
    double total = std::accumulate(areas.begin(), areas.end(), 0.0);
    std::cout << "  Total area of all shapes: " << std::fixed << std::setprecision(2) << total << '\n';

    // std::max_element — returns iterator to the largest element
    auto max_it = std::max_element(areas.begin(), areas.end());
    std::cout << "  Largest area: " << *max_it << '\n';

    // ── 5. std::optional ─────────────────────────────────────────────────────
    std::cout << "\n--- std::optional (safe computation without exceptions) ---\n";

    for (double val : {16.0, -4.0, 9.0}) {
        if (auto result = safe_sqrt(val)) {
            // 'if (auto result = ...)' is an init-statement — C++17
            std::cout << "  sqrt(" << val << ") = " << *result << '\n';
        } else {
            std::cout << "  sqrt(" << val << ") = undefined (negative input)\n";
        }
    }

    // ── 6. Structured bindings (C++17) ───────────────────────────────────────
    std::cout << "\n--- Structured bindings (C++17) ---\n";
    std::vector<std::pair<std::string, int>> scores = {{"Alice",92},{"Bob",85},{"Carla",97}};

    for (const auto& [name, score] : scores) {
        // 'auto& [name, score]' unpacks the pair — no need for .first/.second
        std::cout << "  " << name << ": " << score << '\n';
    }

    // ── 7. Template function ─────────────────────────────────────────────────
    std::cout << "\n--- Template function ---\n";
    std::cout << "  clamp(150, 0, 100) = " << clamp_value(150, 0, 100) << '\n';
    std::cout << "  clamp(3.7, 0.0, 5.0) = " << clamp_value(3.7, 0.0, 5.0) << '\n';

    // ── 8. Memory safety note ────────────────────────────────────────────────
    std::cout << "\n--- Memory safety ---\n";
    std::cout << "  All shapes were stored in unique_ptr — no manual delete needed.\n"
              << "  When 'shapes' goes out of scope here, all Shape objects are\n"
              << "  automatically destroyed. This is RAII at work.\n";

    std::cout << "\nC++ — Power without sacrificing safety.\n";
    return 0;   // return 0 = success; OS exit code
}
