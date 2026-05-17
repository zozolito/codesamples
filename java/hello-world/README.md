# Java Hello World — VS Code Project

A single-file Java project demonstrating the language's core idioms.
No build tool (Maven/Gradle) required — plain `javac` and `java`.

---

## 🇬🇧 English

### What this project demonstrates

| Concept | Where |
|---|---|
| **Enums** with fields and methods | `enum Category` |
| **Interfaces** with default methods | `interface Describable`, `interface Rateable` |
| **Abstract class** and inheritance | `abstract class LibraryItem`, `class Book`, `class AudioBook` |
| **Polymorphism** — dynamic dispatch | `item.describe()` calls the correct subclass override at runtime |
| **Records** — immutable data classes | `record Catalogue(...)` |
| **Generics** — type-safe, no casting | `class Pair<A, B>`, `<T extends Comparable<T>> T max(...)` |
| **Custom exceptions** — checked and unchecked | `ItemNotFoundException`, `DuplicateItemException` |
| **Collections** — ArrayList, HashMap, EnumMap | `library.items`, `library.byTitle` |
| **Optional** — explicit nullable values | `findOptional()` |
| **Lambdas** and functional interfaces | `Function`, `Comparator`, method references |
| **Streams API** | `filter`, `map`, `sorted`, `collect`, `reduce`, `groupingBy` |
| **Pattern matching instanceof** (Java 16+) | `if (item instanceof AudioBook ab)` |
| **Switch expressions** (Java 14+) | `switch (cat) { case FICTION -> "..." }` |
| **StringBuilder** | Building strings in loops efficiently |
| **String formatting** | `String.format()`, text blocks (Java 15+) |
| **Static factory methods** | `LibraryItem.createBook(...)`, `Pair.of(...)` |

### Project structure

```
hello-world/
├── src/
│   └── HelloWorld.java    ← all source (all classes in one file for clarity)
├── bin/                   ← compiled .class files (git-ignored)
├── .vscode/
│   ├── tasks.json         ← Compile / Run / Single-file / Disassemble / JVM info
│   ├── launch.json        ← Debug (F5), with and without stop-on-entry
│   └── extensions.json    ← Extension Pack for Java (recommended)
├── .gitignore
└── README.md
```

### Prerequisites

**Install a JDK** — Java Development Kit, version 17 or 21 (LTS recommended).

**Windows (Winget)**
```powershell
winget install Microsoft.OpenJDK.21
# or
winget install EclipseAdoptium.Temurin.21.JDK
```

**macOS (Homebrew)**
```bash
brew install openjdk@21
# then follow the symlink instructions printed by brew
```

**Linux (Debian/Ubuntu)**
```bash
sudo apt update
sudo apt install openjdk-21-jdk
```

**Verify installation**
```bash
java  --version    # should print "openjdk 21..." or similar
javac --version    # should print "javac 21..."
```

### Building and running

**Option 1 — VS Code (recommended)**

Install the **Extension Pack for Java** (`vscjava.vscode-java-pack`) when prompted.
Then press **`Ctrl+Shift+B`** — this runs the default task: compile then run.

**Option 2 — Command line**
```bash
# Create the output directory
mkdir -p bin   # Linux/macOS
md bin         # Windows CMD

# Compile source to bytecode
javac -d bin src/HelloWorld.java

# Run
java -cp bin HelloWorld
```

**Option 3 — Single-file shortcut (Java 11+)**
```bash
# No separate compile step — Java does it automatically
java src/HelloWorld.java
```

### Debugging

1. Set a breakpoint by clicking the gutter (left of the line number)
2. Press **F5** — the debugger starts
3. **F10** step over · **F11** step into · **Shift+F11** step out · **F5** continue

Good breakpoints to try:
- First line of `main()` — step through the whole program
- Inside `describe()` on `LibraryItem` — watch polymorphism in action (check which subclass is executing)
- The `catch (ItemNotFoundException e)` block — inspect the exception object
- Inside the stream `.filter()` lambda — see each item being tested

### Understanding the bytecode

The JVM does not run Java source code — it runs **bytecode** (`.class` files).
The `javap` task lets you see the bytecode for any class:

```bash
javac -d bin src/HelloWorld.java
javap -c -p -classpath bin HelloWorld
```

This shows the JVM instructions. For example, a Java `for` loop compiles to a series
of `iload`, `iadd`, `if_icmplt`, and `goto` instructions. Understanding this helps you
understand why `StringBuilder` is faster than `+` in a loop — you can count the object
allocations in the bytecode.

### Compiler flags used

| Flag | Purpose |
|---|---|
| `-d bin` | Place `.class` files in the `bin/` directory |
| `-encoding UTF-8` | Source files are UTF-8 encoded |
| `-source 17` | Compile as Java 17 (enables all language features up to Java 17) |
| `-target 17` | Generate bytecode compatible with Java 17 JVM |

---

## 🇫🇷 Français

### Ce que démontre ce projet

Un seul fichier source — `HelloWorld.java` — démontrant les idiomes essentiels du Java :
énumérations, classes abstraites, interfaces, polymorphisme, génériques, records, streams,
lambdas, gestion des exceptions, et Optional.

### Prérequis

**Installer un JDK 21 (LTS)**

```bash
# Linux (Debian/Ubuntu)
sudo apt install openjdk-21-jdk

# macOS
brew install openjdk@21

# Windows
winget install Microsoft.OpenJDK.21
```

Vérification :
```bash
java --version   # doit afficher openjdk 21...
javac --version  # doit afficher javac 21...
```

### Compiler et exécuter

**VS Code** — installer l'Extension Pack for Java, puis **`Ctrl+Shift+B`** (tâche par défaut).

**Ligne de commande**
```bash
mkdir -p bin
javac -d bin src/HelloWorld.java
java -cp bin HelloWorld
```

**Fichier unique (Java 11+)**
```bash
java src/HelloWorld.java
```

### Points clés à lire dans le code

| Ligne | Concept clé |
|---|---|
| `enum Category` | Enum avec champs, constructeur et `toString()` |
| `abstract class LibraryItem implements Describable, Rateable` | Classe abstraite + interfaces multiples |
| `class Book extends LibraryItem` | Héritage, `super()`, `@Override` |
| `record Catalogue(...)` | Record Java 16+ — equals/hashCode/toString automatiques |
| `class Pair<A, B>` | Classe générique — sécurité de type sans cast |
| `throws ItemNotFoundException` | Exception checkée — le compilateur impose la gestion |
| `library.topRated(4).forEach(...)` | Stream pipeline — filter → sorted → collect |
| `if (item instanceof AudioBook ab)` | Pattern matching `instanceof` (Java 16+) |
| `Function<String, String> pipeline = trim.andThen(...)` | Composition de fonctions |
