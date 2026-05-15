# C++ — C Plus Plus

---

## 🇬🇧 English

### What is C++?

C++ is C with classes, and then some. It started as an extension of C and grew into one of the most complex and powerful programming languages ever created. It gives you everything C offers — direct memory access, raw performance, low-level control — plus object-oriented programming, generic programming via templates, and a rich standard library.

C++ is used in game engines, operating systems, embedded systems, financial trading platforms, graphics rendering, compilers, and any domain where performance is not negotiable. If you play a AAA video game, you are almost certainly running C++ code.

Learning C++ is hard. It is also one of the most rewarding things you can do as a programmer. It will fundamentally change how you understand software.

---

### A brief history

| Year | Event |
|------|-------|
| 1979 | Bjarne Stroustrup starts "C with Classes" at Bell Labs |
| 1985 | C++ officially named; first commercial implementation |
| 1998 | C++98 — first ISO standard |
| 2003 | C++03 — bug fixes |
| 2011 | **C++11** — `auto`, lambdas, `nullptr`, smart pointers, move semantics. The modern era begins. |
| 2014 | C++14 — refinements |
| 2017 | C++17 — `std::optional`, `std::variant`, structured bindings |
| 2020 | C++20 — concepts, ranges, coroutines, modules |
| 2023 | C++23 — `std::expected`, `std::print`, more |

> Bjarne Stroustrup once said: *"C makes it easy to shoot yourself in the foot; C++ makes it harder, but when you do it blows your whole leg off."* He said this about his own language. Take it seriously.

---

### Best tools

| Tool | Purpose | Link |
|------|---------|-------|
| **GCC / G++** | Standard compiler on Linux/macOS | https://gcc.gnu.org |
| **Clang / Clang++** | LLVM-based, best error messages | https://clang.llvm.org |
| **MSVC** | Microsoft compiler for Windows | Included in Visual Studio |
| **Visual Studio** | Best Windows C++ IDE | https://visualstudio.microsoft.com |
| **CLion** | Cross-platform C++ IDE | https://www.jetbrains.com/clion/ |
| **VS Code** + C/C++ | Lightweight editor | https://code.visualstudio.com |
| **CMake** | Standard build system | https://cmake.org |
| **vcpkg / Conan** | Package managers for C++ | https://vcpkg.io |
| **AddressSanitizer** | Memory error detection at runtime | `-fsanitize=address` |
| **Valgrind** | Memory leak detection | https://valgrind.org |
| **Compiler Explorer** | See your C++ compiled to assembly, live | https://godbolt.org |

---

### Basic syntax

#### Hello, World

```cpp
#include <iostream>  // standard input/output
#include <string>    // std::string

int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
```

#### Variables and types

```cpp
#include <string>

// Primitive types (inherited from C)
int age = 30;
double height = 1.75;
bool is_active = true;
char letter = 'A';

// C++ types
std::string name = "Alice";    // proper string, not a char array

// auto — compiler infers the type
auto count = 42;               // int
auto message = std::string("Hi"); // std::string
auto ratio = 3.14;             // double

// const — value cannot change
const int MAX_SIZE = 100;
```

#### Classes — Object Oriented Programming

```cpp
#include <iostream>
#include <string>

class Animal
{
public:
    // Constructor
    Animal(const std::string& name, const std::string& sound)
        : name_(name), sound_(sound)  // initializer list — prefer this
    {}

    // Virtual method — can be overridden
    virtual std::string speak() const
    {
        return name_ + " says " + sound_;
    }

    // Virtual destructor — ALWAYS do this for base classes
    virtual ~Animal() = default;

    const std::string& name() const { return name_; }

private:
    std::string name_;
    std::string sound_;
};

// Inheritance
class Dog : public Animal
{
public:
    explicit Dog(const std::string& name)
        : Animal(name, "Woof")
    {}

    // Override
    std::string speak() const override
    {
        return Animal::speak() + "!";
    }

    std::string fetch() const
    {
        return name() + " fetches the ball!";
    }
};

int main()
{
    Dog dog("Rex");
    std::cout << dog.speak() << "\n";  // Rex says Woof!
    std::cout << dog.fetch() << "\n";  // Rex fetches the ball!
    return 0;
}
```

#### RAII — Resource Acquisition Is Initialization

```cpp
// RAII is the most important C++ concept.
// Tie resource lifetime to object lifetime.
// When the object is destroyed, the resource is released. Automatically.

#include <fstream>
#include <string>

void process_file(const std::string& path)
{
    std::ifstream file(path);  // file opened here (resource acquired)

    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + path);
    }

    std::string line;
    while (std::getline(file, line))
    {
        // process line...
    }

    // file closed HERE automatically when it goes out of scope
    // even if an exception is thrown
    // You don't have to remember to close it.
}
```

#### Smart pointers — forget raw `new` and `delete`

```cpp
#include <memory>
#include <iostream>

class Resource
{
public:
    Resource(int id) : id_(id)
    {
        std::cout << "Resource " << id_ << " created\n";
    }
    ~Resource()
    {
        std::cout << "Resource " << id_ << " destroyed\n";
    }
private:
    int id_;
};

int main()
{
    // unique_ptr — single owner, destroyed when out of scope
    {
        auto res = std::make_unique<Resource>(1);
        // use res...
    }  // Resource 1 destroyed here, automatically

    // shared_ptr — shared ownership, destroyed when last owner goes out of scope
    {
        auto r1 = std::make_shared<Resource>(2);
        {
            auto r2 = r1;  // r1 and r2 both own the resource
            // use r2...
        }  // r2 destroyed but resource survives (r1 still owns it)
    }  // Resource 2 destroyed here

    // Never use raw new/delete in modern C++
    // If you're writing delete, ask yourself why

    return 0;
}
```

#### Templates — generic programming

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

// A generic function — works with any type that supports <
template<typename T>
T maximum(const T& a, const T& b)
{
    return a > b ? a : b;
}

// A generic container
template<typename T>
class Stack
{
public:
    void push(const T& value) { data_.push_back(value); }
    void pop() { data_.pop_back(); }
    const T& top() const { return data_.back(); }
    bool empty() const { return data_.empty(); }
    std::size_t size() const { return data_.size(); }

private:
    std::vector<T> data_;
};

int main()
{
    std::cout << maximum(3, 7) << "\n";           // 7
    std::cout << maximum(3.14, 2.71) << "\n";     // 3.14
    std::cout << maximum(std::string("a"), std::string("z")) << "\n"; // z

    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    std::cout << stack.top() << "\n";  // 3

    return 0;
}
```

#### Standard library — use it

```cpp
#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

int main()
{
    std::vector<int> numbers = {5, 3, 1, 4, 2};

    // Sort
    std::sort(numbers.begin(), numbers.end());
    // numbers = {1, 2, 3, 4, 5}

    // Sum
    int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
    std::cout << "Sum: " << sum << "\n";  // 15

    // Find
    auto it = std::find(numbers.begin(), numbers.end(), 3);
    if (it != numbers.end())
    {
        std::cout << "Found: " << *it << "\n";  // 3
    }

    // Transform (like map)
    std::vector<int> doubled(numbers.size());
    std::transform(numbers.begin(), numbers.end(), doubled.begin(),
                   [](int n) { return n * 2; });

    // Range-based for (C++11)
    for (const auto& n : doubled)
    {
        std::cout << n << " ";  // 2 4 6 8 10
    }

    return 0;
}
```

#### Lambdas

```cpp
#include <vector>
#include <algorithm>
#include <iostream>

int main()
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Lambda syntax: [capture](parameters) -> return_type { body }
    auto is_even = [](int n) { return n % 2 == 0; };

    // Use with algorithm
    auto count = std::count_if(numbers.begin(), numbers.end(), is_even);
    std::cout << "Even count: " << count << "\n";  // 5

    // Capture local variables
    int threshold = 5;
    auto above_threshold = [threshold](int n) { return n > threshold; };

    std::vector<int> filtered;
    std::copy_if(numbers.begin(), numbers.end(),
                 std::back_inserter(filtered), above_threshold);

    return 0;
}
```

#### Error handling — exceptions

```cpp
#include <stdexcept>
#include <iostream>

double divide(double a, double b)
{
    if (b == 0.0)
    {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

int main()
{
    try
    {
        double result = divide(10.0, 0.0);
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected: " << e.what() << "\n";
    }

    return 0;
}
```

---

## 🇫🇷 Français

### C'est quoi le C++ ?

Le C++ est le C avec des classes, et bien davantage. Il a commencé comme une extension du C et est devenu l'un des langages de programmation les plus complexes et les plus puissants jamais créés. Il te donne tout ce que le C offre — accès direct à la mémoire, performance brute, contrôle bas niveau — plus la programmation orientée objet, la programmation générique via les templates, et une riche bibliothèque standard.

Le C++ est utilisé dans les moteurs de jeux, les systèmes d'exploitation, les systèmes embarqués, les plateformes de trading financier, le rendu graphique et tout domaine où la performance n'est pas négociable.

---

### Un peu d'histoire

| Année | Événement |
|-------|-----------|
| 1979 | Bjarne Stroustrup commence le "C avec classes" chez Bell Labs |
| 1985 | C++ nommé officiellement |
| 1998 | C++98 — première norme ISO |
| 2011 | **C++11** — `auto`, lambdas, `nullptr`, pointeurs intelligents. L'ère moderne commence. |
| 2020 | C++20 — concepts, ranges, coroutines, modules |
| 2023 | C++23 — `std::expected`, `std::print` |

> Bjarne Stroustrup a dit un jour : *"Le C facilite de te tirer une balle dans le pied ; le C++ le rend plus difficile, mais quand tu le fais, ça t'arrache toute la jambe."* Il parlait de son propre langage. Prends ça au sérieux.

---

### Les meilleurs outils

| Outil | Utilité | Lien |
|-------|---------|------|
| **GCC / G++** | Compilateur standard Linux/macOS | https://gcc.gnu.org |
| **Clang / Clang++** | Meilleurs messages d'erreur | https://clang.llvm.org |
| **Visual Studio** | Meilleur IDE C++ Windows | https://visualstudio.microsoft.com |
| **CLion** | IDE C++ cross-plateforme | https://www.jetbrains.com/clion/ |
| **CMake** | Système de build standard | https://cmake.org |
| **Compiler Explorer** | Voir son C++ compilé en assembleur, en direct | https://godbolt.org |

---

### Syntaxe de base

#### Hello, World

```cpp
#include <iostream>

int main()
{
    std::cout << "Bonjour, Monde !" << std::endl;
    return 0;
}
```

#### Classes

```cpp
#include <string>
#include <iostream>

class Animal
{
public:
    Animal(const std::string& nom, const std::string& son)
        : nom_(nom), son_(son) {}

    virtual std::string parler() const
    {
        return nom_ + " dit " + son_;
    }

    virtual ~Animal() = default;

private:
    std::string nom_;
    std::string son_;
};

class Chien : public Animal
{
public:
    explicit Chien(const std::string& nom)
        : Animal(nom, "Ouaf") {}

    std::string parler() const override
    {
        return Animal::parler() + " !";
    }
};
```

#### RAII — la règle d'or du C++

```cpp
#include <fstream>

void traiter_fichier(const std::string& chemin)
{
    std::ifstream fichier(chemin);  // ouverture ici

    if (!fichier.is_open())
    {
        throw std::runtime_error("Impossible d'ouvrir : " + chemin);
    }

    std::string ligne;
    while (std::getline(fichier, ligne))
    {
        // traiter la ligne...
    }

    // fichier fermé ICI automatiquement en sortie de portée
    // même en cas d'exception
}
```

#### Pointeurs intelligents

```cpp
#include <memory>

// unique_ptr — propriétaire unique, détruit en sortie de portée
auto ressource = std::make_unique<MonObjet>(42);

// shared_ptr — propriété partagée
auto r1 = std::make_shared<MonObjet>(1);
auto r2 = r1;  // r1 et r2 partagent la ressource

// N'utilise jamais new/delete brut en C++ moderne
```

#### Lambdas

```cpp
#include <vector>
#include <algorithm>

std::vector<int> nombres = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

auto est_pair = [](int n) { return n % 2 == 0; };

auto compte = std::count_if(nombres.begin(), nombres.end(), est_pair);

int seuil = 5;
auto au_dessus = [seuil](int n) { return n > seuil; };
```
