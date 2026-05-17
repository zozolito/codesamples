# Java Reference

> *"Java is to JavaScript what Car is to Carpet."* — a joke that accurately describes how unrelated they are.
> This reference exists so you understand what you write — not just that it compiles.

---

## 🇬🇧 English

### Table of Contents

1. [Types & Variables](#1-types--variables)
2. [Strings & StringBuilder](#2-strings--stringbuilder)
3. [Arrays](#3-arrays)
4. [Control Flow](#4-control-flow)
5. [Methods](#5-methods)
6. [Classes & Objects](#6-classes--objects)
7. [Inheritance](#7-inheritance)
8. [Interfaces & Abstract Classes](#8-interfaces--abstract-classes)
9. [Access Modifiers](#9-access-modifiers)
10. [Generics](#10-generics)
11. [Collections Framework](#11-collections-framework)
12. [Exception Handling](#12-exception-handling)
13. [Enums](#13-enums)
14. [Lambdas & Functional Interfaces](#14-lambdas--functional-interfaces)
15. [Streams API](#15-streams-api)
16. [Optional](#16-optional)
17. [Records (Java 16+)](#17-records-java-16)
18. [Concurrency Basics](#18-concurrency-basics)
19. [Memory Management & GC](#19-memory-management--gc)
20. [Security Pitfalls](#20-security-pitfalls)

---

### 1. Types & Variables

Java is a **statically typed** language: every variable has a type known at compile time. This catches entire categories of bugs before the code ever runs.

```java
// ── Primitive Types (stored by value, not as objects) ────────────────────────
byte    b  = 127;              // 8-bit  integer: -128 to 127
short   s  = 32_767;           // 16-bit integer: -32,768 to 32,767
int     i  = 2_147_483_647;    // 32-bit integer (most common integer type)
long    l  = 9_223_372_036L;   // 64-bit integer — suffix L required
float   f  = 3.14f;            // 32-bit floating point — suffix f required
double  d  = 3.141592653589;   // 64-bit floating point (default for decimals)
boolean ok = true;             // true or false only
char    c  = 'A';              // 16-bit Unicode character (UTF-16 code unit)

// Underscores in numeric literals (Java 7+) — for readability
int million    = 1_000_000;
long bigNumber = 9_999_999_999L;
int hexColor   = 0xFF_A5_00;   // hex literals with underscores

// ── Wrapper Classes (object versions of primitives) ──────────────────────────
// Required for generics, collections, and nullable values.
Integer  boxed = 42;           // autoboxing — auto-converts int to Integer
int      unboxed = boxed;      // unboxing  — auto-converts Integer to int

// ⚠️ Autoboxing trap — Integer caches -128 to 127, so == works by accident
Integer a = 127; Integer b = 127; a == b; // true (cached)
Integer x = 128; Integer y = 128; x == y; // false (different objects!)
// Always use .equals() to compare Integer values
x.equals(y); // true ← correct

// ── Type Casting ─────────────────────────────────────────────────────────────
// Widening (implicit, safe — no data loss)
int  num  = 100;
long big  = num;     // int → long, automatic
double d2 = num;     // int → double, automatic

// Narrowing (explicit, potentially lossy)
double pi     = 3.14159;
int    piInt  = (int) pi;  // 3 — decimal part truncated, no rounding
long   big2   = 9_999_999_999L;
int    small  = (int) big2; // data loss — truncates to lower 32 bits

// ── var — local type inference (Java 10+) ────────────────────────────────────
// The compiler infers the type. Only works for local variables.
var name    = "Alice";               // inferred as String
var numbers = new ArrayList<Int>();  // inferred as ArrayList<Integer>
var count   = 0;                     // inferred as int
// var is not dynamic typing — the type is fixed at compile time
```

---

### 2. Strings & StringBuilder

Strings in Java are **immutable** — once created, their content cannot change. Every "modification" creates a new String object.

```java
// ── String creation ───────────────────────────────────────────────────────────
String s1 = "Hello";           // string literal — stored in string pool
String s2 = new String("Hi");  // new object — avoid, wastes memory

// ⚠️ String comparison — always use .equals(), never ==
String a = "hello";
String b = "hello";
a == b;       // true (both from string pool — lucky coincidence, not reliable!)
a.equals(b);  // true ← always correct

String c = new String("hello");
a == c;       // false (c is a new object, not from the pool)
a.equals(c);  // true ← still correct

// Case-insensitive comparison
a.equalsIgnoreCase("HELLO"); // true

// ── Useful String methods ─────────────────────────────────────────────────────
String text = "  Hello, World!  ";

text.length()                    // 17
text.trim()                      // "Hello, World!" — remove leading/trailing whitespace
text.strip()                     // same but Unicode-aware (Java 11+)
text.toUpperCase()               // "  HELLO, WORLD!  "
text.toLowerCase()               // "  hello, world!  "
text.contains("World")           // true
text.startsWith("  Hello")       // true
text.endsWith("!  ")             // true
text.indexOf("World")            // 9
text.replace("World", "Java")    // "  Hello, Java!  "
text.replaceAll("\\s+", "_")     // replace all whitespace sequences
text.split(",")                  // ["  Hello", " World!  "]
text.substring(2, 7)             // "Hello"
text.charAt(2)                   // 'H'
text.isEmpty()                   // false (length > 0)
text.isBlank()                   // false (Java 11+: true only if all whitespace)

// String.format (like printf)
String msg = String.format("User %s has %d points", "Alice", 250);

// Text blocks (Java 15+) — multi-line strings with clean indentation
String json = """
        {
            "name": "Alice",
            "age": 30
        }
        """;

// ── StringBuilder — for building strings in loops ────────────────────────────
// ⚠️ String concatenation in a loop creates many intermediate objects:
String result = "";
for (int i = 0; i < 1000; i++) {
    result += i;  // Creates 1000 intermediate String objects — very slow
}

// ✅ Use StringBuilder instead:
StringBuilder sb = new StringBuilder();
for (int i = 0; i < 1000; i++) {
    sb.append(i);
}
String result2 = sb.toString(); // convert once at the end

// StringBuilder is mutable — useful methods:
StringBuilder sb2 = new StringBuilder("Hello");
sb2.append(", World");       // "Hello, World"
sb2.insert(5, " Java");      // "Hello Java, World"
sb2.delete(5, 10);           // "Hello, World"
sb2.reverse();               // "dlroW ,olleH"
sb2.replace(0, 5, "Bye");    // "Bye ,olleH"
sb2.length();                // current length

// Note: StringBuffer is the thread-safe version of StringBuilder.
// Use StringBuilder in single-threaded code (it's faster).
```

---

### 3. Arrays

```java
// ── Declaration and initialisation ───────────────────────────────────────────
int[] numbers = new int[5];          // [0, 0, 0, 0, 0] — zero-initialised
int[] primes  = {2, 3, 5, 7, 11};   // array literal
String[] names = new String[3];      // [null, null, null]

// ── Access ────────────────────────────────────────────────────────────────────
primes[0];                  // 2 (0-indexed)
primes[primes.length - 1];  // 11 (last element)
// primes[5];               // ArrayIndexOutOfBoundsException — runtime error

// ── Iteration ─────────────────────────────────────────────────────────────────
// Traditional for loop
for (int i = 0; i < primes.length; i++) {
    System.out.println(i + ": " + primes[i]);
}

// Enhanced for loop (for-each) — when index is not needed
for (int prime : primes) {
    System.out.println(prime);
}

// ── Useful utilities ──────────────────────────────────────────────────────────
import java.util.Arrays;

Arrays.sort(numbers);                     // sort in-place (ascending)
Arrays.sort(numbers, 1, 4);              // sort index 1..3 only
Arrays.fill(numbers, 0);                 // fill all elements with 0
Arrays.copyOf(primes, 3);               // [2, 3, 5] — copy first 3
Arrays.copyOfRange(primes, 1, 4);       // [3, 5, 7] — copy [1, 4)
Arrays.equals(primes, numbers);          // element-by-element comparison
Arrays.toString(primes);                 // "[2, 3, 5, 7, 11]" — for printing

// ── 2D arrays ──────────────────────────────────────────────────────────────────
int[][] matrix = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
matrix[1][2];  // 6 — row 1, column 2

// Jagged arrays (rows of different lengths)
int[][] triangle = new int[3][];
triangle[0] = new int[]{1};
triangle[1] = new int[]{1, 2};
triangle[2] = new int[]{1, 2, 3};
```

---

### 4. Control Flow

```java
// ── if / else if / else ───────────────────────────────────────────────────────
int score = 85;

if (score >= 90) {
    System.out.println("A");
} else if (score >= 80) {
    System.out.println("B");
} else if (score >= 70) {
    System.out.println("C");
} else {
    System.out.println("F");
}

// Ternary operator
String grade = score >= 60 ? "Pass" : "Fail";

// ── switch ────────────────────────────────────────────────────────────────────
// Traditional switch (fall-through by default — requires break)
String day = "MONDAY";
switch (day) {
    case "MONDAY":
    case "TUESDAY":
    case "WEDNESDAY":
    case "THURSDAY":
    case "FRIDAY":
        System.out.println("Weekday");
        break;
    case "SATURDAY":
    case "SUNDAY":
        System.out.println("Weekend");
        break;
    default:
        System.out.println("Unknown");
}

// Switch expression (Java 14+) — no fall-through, returns a value
String type = switch (day) {
    case "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY" -> "Weekday";
    case "SATURDAY", "SUNDAY"                                    -> "Weekend";
    default                                                       -> "Unknown";
};

// ── Loops ─────────────────────────────────────────────────────────────────────
// while — check before each iteration
int i = 0;
while (i < 5) {
    System.out.println(i);
    i++;
}

// do-while — executes at least once
int count = 0;
do {
    count++;
} while (count < 5);

// for — initialise; condition; update
for (int j = 0; j < 10; j++) {
    if (j == 3) continue; // skip to next iteration
    if (j == 7) break;    // exit loop entirely
    System.out.println(j);
}

// Enhanced for (for-each) — works on arrays and Iterables
for (String name : new String[]{"Alice", "Bob", "Carol"}) {
    System.out.println(name);
}

// Labelled break — breaks out of a named outer loop
outer:
for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
        if (row == 1 && col == 1) break outer; // exits both loops
        System.out.println(row + "," + col);
    }
}
```

---

### 5. Methods

```java
// ── Basic method ──────────────────────────────────────────────────────────────
// accessModifier returnType methodName(parameters) { body }
public static int add(int a, int b) {
    return a + b;
}

// void — no return value
public static void printGreeting(String name) {
    System.out.println("Hello, " + name + "!");
}

// ── Method Overloading ────────────────────────────────────────────────────────
// Same name, different parameter types/count. Resolved at compile time.
public static double area(double radius) {
    return Math.PI * radius * radius;
}
public static double area(double width, double height) {
    return width * height;
}
// Calls resolve based on argument types:
area(5.0);       // first method
area(4.0, 3.0);  // second method

// ── Varargs ───────────────────────────────────────────────────────────────────
// Allows calling with any number of arguments — treated as an array inside
public static int sum(int... numbers) {
    int total = 0;
    for (int n : numbers) total += n;
    return total;
}
sum(1, 2, 3);        // 6
sum(1, 2, 3, 4, 5);  // 15
sum();               // 0

// ── Pass by value ─────────────────────────────────────────────────────────────
// Java ALWAYS passes by value. For objects, the value is the reference.
void tryToModifyPrimitive(int x) {
    x = 99; // only modifies the local copy
}

void modifyObject(StringBuilder sb) {
    sb.append(" world"); // modifies the actual object (reference is copied, not the object)
}

void tryToReassignObject(StringBuilder sb) {
    sb = new StringBuilder("new");  // only changes local reference — no effect on caller
}
```

---

### 6. Classes & Objects

```java
// ── Class definition ──────────────────────────────────────────────────────────
public class BankAccount {
    // Instance fields — each object has its own copy
    private String owner;      // private — only accessible within this class
    private double balance;
    private final String id;   // final — set once, never changed

    // Static field — shared by ALL instances, belongs to the class
    private static int nextId = 1000;

    // ── Constructors ───────────────────────────────────────────────────────────
    // Constructor name MUST match class name. No return type.
    public BankAccount(String owner, double initialBalance) {
        this.owner   = owner;   // `this` distinguishes field from parameter
        this.balance = initialBalance;
        this.id      = "ACC-" + nextId++;  // assign and increment
    }

    // Constructor chaining — one constructor calls another
    public BankAccount(String owner) {
        this(owner, 0.0);  // delegates to the full constructor
    }

    // ── Instance Methods ────────────────────────────────────────────────────────
    public void deposit(double amount) {
        if (amount <= 0) throw new IllegalArgumentException("Deposit must be positive");
        this.balance += amount;
    }

    public void withdraw(double amount) {
        if (amount <= 0)         throw new IllegalArgumentException("Amount must be positive");
        if (amount > this.balance) throw new IllegalStateException("Insufficient funds");
        this.balance -= amount;
    }

    // ── Getters & Setters ───────────────────────────────────────────────────────
    public String  getOwner()   { return owner; }
    public double  getBalance() { return balance; }
    public String  getId()      { return id; }

    public void setOwner(String owner) {
        if (owner == null || owner.isBlank()) throw new IllegalArgumentException("Name required");
        this.owner = owner;
    }

    // ── Static method ───────────────────────────────────────────────────────────
    // Belongs to the class, not any instance. Cannot access `this`.
    public static BankAccount openJointAccount(String owner1, String owner2) {
        return new BankAccount(owner1 + " & " + owner2, 0.0);
    }

    // ── toString — called when you print an object or concatenate with a string ─
    @Override
    public String toString() {
        return String.format("BankAccount[id=%s, owner=%s, balance=%.2f]",
                             id, owner, balance);
    }

    // ── equals & hashCode — always override together ────────────────────────────
    // If two objects are equal, they MUST have the same hashCode.
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;           // same reference
        if (!(obj instanceof BankAccount other)) return false; // wrong type
        return this.id.equals(other.id);        // logical equality
    }

    @Override
    public int hashCode() {
        return id.hashCode();
    }
}

// ── Using the class ────────────────────────────────────────────────────────────
BankAccount acc = new BankAccount("Alice", 1000.0);
acc.deposit(500);
acc.withdraw(200);
System.out.println(acc);           // calls toString()
System.out.println(acc.getBalance()); // 1300.0

BankAccount joint = BankAccount.openJointAccount("Alice", "Bob");
```

---

### 7. Inheritance

```java
// ── Base class (superclass) ────────────────────────────────────────────────────
public class Shape {
    protected String color; // protected — accessible in subclasses

    public Shape(String color) {
        this.color = color;
    }

    // Method intended to be overridden
    public double area() {
        return 0.0; // base implementation — subclasses should override
    }

    public String describe() {
        return String.format("%s[color=%s, area=%.2f]",
                             getClass().getSimpleName(), color, area());
    }
}

// ── Subclass (inherits Shape) ─────────────────────────────────────────────────
public class Circle extends Shape {
    private final double radius;

    public Circle(String color, double radius) {
        super(color); // MUST call super() as first statement if superclass has a constructor
        this.radius = radius;
    }

    @Override  // annotation — compiler verifies you're actually overriding
    public double area() {
        return Math.PI * radius * radius;
    }

    public double getRadius() { return radius; }
}

public class Rectangle extends Shape {
    private final double width, height;

    public Rectangle(String color, double width, double height) {
        super(color);
        this.width  = width;
        this.height = height;
    }

    @Override
    public double area() { return width * height; }
}

// ── Polymorphism ───────────────────────────────────────────────────────────────
// A Shape reference can hold any Shape subtype.
// The correct override is called at runtime — this is dynamic dispatch.
Shape[] shapes = {
    new Circle("red", 5.0),
    new Rectangle("blue", 4.0, 3.0),
    new Circle("green", 2.5),
};

for (Shape shape : shapes) {
    System.out.println(shape.area()); // calls the correct subclass method
    System.out.println(shape.describe());
}

// ── instanceof + pattern matching (Java 16+) ──────────────────────────────────
for (Shape shape : shapes) {
    if (shape instanceof Circle c) {
        // `c` is already cast — no separate cast needed
        System.out.println("Circle radius: " + c.getRadius());
    } else if (shape instanceof Rectangle r) {
        System.out.println("Rectangle: " + r.area());
    }
}

// ── final — prevent inheritance or overriding ─────────────────────────────────
public final class ImmutablePoint {  // cannot be subclassed
    private final double x, y;       // cannot be reassigned
    public ImmutablePoint(double x, double y) { this.x = x; this.y = y; }
}

// Preventing a specific method from being overridden:
public class Base {
    public final void criticalMethod() { /* cannot be overridden */ }
}
```

---

### 8. Interfaces & Abstract Classes

```java
// ── Interface — a contract: "any class implementing this MUST provide these" ──
public interface Drawable {
    // All methods are implicitly public abstract
    void draw();

    // Default method (Java 8+) — provides an implementation
    default String getDescription() {
        return "A drawable object";
    }

    // Static method (Java 8+) — belongs to the interface, not implementations
    static Drawable noOp() {
        return () -> {};  // lambda implementing the single abstract method
    }
}

public interface Resizable {
    void resize(double factor);
    default double scaledArea(double factor) { return 0; }
}

// A class can implement multiple interfaces (unlike extends — only one class)
public class Square extends Shape implements Drawable, Resizable {
    private double side;

    public Square(String color, double side) {
        super(color);
        this.side = side;
    }

    @Override public double area()          { return side * side; }
    @Override public void   draw()          { System.out.println("Drawing square"); }
    @Override public void   resize(double f) { this.side *= f; }
}

// ── Abstract Class — partially implemented, cannot be instantiated ─────────────
public abstract class Vehicle {
    protected String make;
    protected int    year;

    public Vehicle(String make, int year) {
        this.make = make;
        this.year = year;
    }

    // Concrete method — shared implementation
    public int age() {
        return java.time.Year.now().getValue() - year;
    }

    // Abstract method — subclasses MUST implement this
    public abstract double fuelEfficiency(); // returns km/litre or km/kWh

    // Template method pattern — algorithm skeleton in the base class
    public final void startJourney() {
        prepare();      // concrete step
        depart();       // abstract step
        monitor();      // concrete step
    }

    protected void   prepare() { System.out.println("Pre-journey checks"); }
    protected abstract void depart();
    protected void   monitor() { System.out.println("Monitoring journey"); }
}

// ── Interface vs Abstract Class ────────────────────────────────────────────────
// Use an interface when:
//   - You're defining a capability ("can be drawn", "can be serialised")
//   - Multiple unrelated classes should share the same contract
//   - You need multiple inheritance of type
//
// Use an abstract class when:
//   - You want to share code (concrete methods, fields) between related classes
//   - There is a natural "is-a" hierarchy
//   - You need constructors or non-public members
```

---

### 9. Access Modifiers

| Modifier | Same Class | Same Package | Subclass | Everywhere |
|---|---|---|---|---|
| `private` | ✅ | ❌ | ❌ | ❌ |
| *(package-private)* | ✅ | ✅ | ❌ | ❌ |
| `protected` | ✅ | ✅ | ✅ | ❌ |
| `public` | ✅ | ✅ | ✅ | ✅ |

```java
public class AccessDemo {
    private   int secretField;     // only accessible within this class
    int       packageField;        // accessible in same package (no modifier)
    protected int familyField;     // accessible in same package + subclasses
    public    int openField;       // accessible everywhere

    // Rule of thumb: make fields private, provide public methods as needed.
    // Exposing fields directly prevents you from adding validation later.
}
```

---

### 10. Generics

Generics allow you to write type-safe code that works with different types without casting.

```java
// ── Generic class ─────────────────────────────────────────────────────────────
public class Pair<A, B> {
    private final A first;
    private final B second;

    public Pair(A first, B second) {
        this.first  = first;
        this.second = second;
    }

    public A getFirst()  { return first; }
    public B getSecond() { return second; }

    @Override
    public String toString() { return "(" + first + ", " + second + ")"; }
}

Pair<String, Integer> p = new Pair<>("Alice", 30);
String name = p.getFirst();  // no cast needed — compiler knows the type
int    age  = p.getSecond(); // safe

// ── Generic method ────────────────────────────────────────────────────────────
public static <T extends Comparable<T>> T max(T a, T b) {
    return a.compareTo(b) >= 0 ? a : b;
}
max(3, 7);         // 7 (Integer)
max("apple", "fig"); // "fig" (String — alphabetical comparison)

// ── Bounded type parameters ────────────────────────────────────────────────────
// <T extends Number> — T must be Number or a subtype (Integer, Double, etc.)
public static <T extends Number> double sum(List<T> list) {
    return list.stream().mapToDouble(Number::doubleValue).sum();
}

// ── Wildcards ─────────────────────────────────────────────────────────────────
// ? extends T — "some subtype of T" — read-only (PECS: Producer Extends)
public static double sumList(List<? extends Number> list) {
    return list.stream().mapToDouble(Number::doubleValue).sum();
}

// ? super T — "some supertype of T" — write-friendly (PECS: Consumer Super)
public static void addNumbers(List<? super Integer> list) {
    list.add(1); list.add(2); list.add(3);
}

// ⚠️ Type erasure — generic type info is removed at compile time
// List<String> and List<Integer> are both just List at runtime
// Cannot do: new T[], instanceof List<String>
```

---

### 11. Collections Framework

```java
import java.util.*;
import java.util.stream.*;

// ── List — ordered, allows duplicates ────────────────────────────────────────
// ArrayList — backed by array, fast random access O(1), slow insert/remove at middle
List<String> list = new ArrayList<>();
list.add("Alice");
list.add("Bob");
list.add("Carol");
list.add(1, "Dave");       // insert at index 1
list.get(0);               // "Alice"
list.remove("Bob");        // remove by value
list.remove(0);            // remove by index
list.size();               // current size
list.contains("Carol");    // true
list.indexOf("Carol");     // index or -1
list.subList(0, 2);        // view of first 2 elements

// Immutable list (Java 9+) — throws on modification
List<String> fixed = List.of("a", "b", "c");

// LinkedList — fast insert/remove at head/tail O(1), slow random access O(n)
// Use as a Queue or Deque, not a general-purpose List
LinkedList<String> queue = new LinkedList<>();
queue.addFirst("first");
queue.addLast("last");
queue.removeFirst();

// ── Set — unique elements, no defined order ───────────────────────────────────
// HashSet — O(1) add/remove/contains (average), no order guaranteed
Set<String> set = new HashSet<>();
set.add("Alice");
set.add("Alice"); // silently ignored — already present
set.contains("Alice"); // true
set.size();            // 1

// LinkedHashSet — maintains insertion order
Set<String> ordered = new LinkedHashSet<>();

// TreeSet — sorted natural order, O(log n) operations
Set<Integer> sorted = new TreeSet<>();
sorted.add(5); sorted.add(1); sorted.add(3);
// iteration order: 1, 3, 5

// Immutable set (Java 9+)
Set<String> fixedSet = Set.of("a", "b", "c");

// ── Map — key→value pairs, keys are unique ────────────────────────────────────
// HashMap — O(1) average, no order guarantee
Map<String, Integer> scores = new HashMap<>();
scores.put("Alice", 95);
scores.put("Bob",   88);
scores.put("Alice", 99); // replaces previous value

scores.get("Alice");                 // 99
scores.get("Eve");                   // null
scores.getOrDefault("Eve", 0);      // 0 — safe default
scores.containsKey("Bob");          // true
scores.putIfAbsent("Carol", 75);    // only adds if key not present
scores.remove("Bob");               // remove by key

// Iteration
for (Map.Entry<String, Integer> entry : scores.entrySet()) {
    System.out.println(entry.getKey() + ": " + entry.getValue());
}
scores.forEach((k, v) -> System.out.println(k + ": " + v)); // Java 8+

// Compute patterns
scores.compute("Alice", (k, v) -> v == null ? 0 : v + 10);
scores.merge("Alice", 5, Integer::sum); // add 5 to Alice's score

// LinkedHashMap — maintains insertion order
Map<String, Integer> linkedMap = new LinkedHashMap<>();

// TreeMap — sorted by key
Map<String, Integer> sortedMap = new TreeMap<>();

// Immutable map (Java 9+)
Map<String, Integer> fixedMap = Map.of("a", 1, "b", 2, "c", 3);

// ── Queue & Deque ──────────────────────────────────────────────────────────────
// Queue — FIFO (First In, First Out)
Queue<String> q = new ArrayDeque<>();
q.offer("first");   // add to tail
q.offer("second");
q.peek();           // "first" — look without removing
q.poll();           // "first" — remove and return head

// Deque (double-ended queue) — add/remove from both ends
Deque<String> deque = new ArrayDeque<>();
deque.addFirst("front");
deque.addLast("back");
deque.peekFirst();
deque.pollLast();

// ── Sorting ────────────────────────────────────────────────────────────────────
List<String> names = new ArrayList<>(List.of("Charlie", "Alice", "Bob"));
Collections.sort(names);                          // alphabetical
Collections.sort(names, Comparator.reverseOrder()); // reverse
names.sort(Comparator.comparingInt(String::length)); // by length

// Sorting a list of objects
record Person(String name, int age) {}
List<Person> people = List.of(new Person("Alice", 30), new Person("Bob", 25));
people.stream()
      .sorted(Comparator.comparing(Person::age))
      .forEach(System.out::println);
```

---

### 12. Exception Handling

Java distinguishes between **checked exceptions** (must be declared or caught) and **unchecked exceptions** (runtime errors that can propagate freely).

```java
// ── Exception hierarchy ────────────────────────────────────────────────────────
// Throwable
// ├── Error              — serious JVM problems (OutOfMemoryError) — don't catch
// └── Exception
//     ├── RuntimeException   — unchecked: NullPointerException, ArrayIndexOutOfBounds
//     └── IOException        — checked: must declare with throws or catch

// ── try / catch / finally ──────────────────────────────────────────────────────
try {
    String text = readFile("data.txt"); // throws IOException (checked)
    int    num  = Integer.parseInt(text.trim()); // throws NumberFormatException (unchecked)
    System.out.println(num * 2);
} catch (IOException e) {
    System.err.println("File error: " + e.getMessage());
} catch (NumberFormatException e) {
    System.err.println("Not a number: " + e.getMessage());
} catch (Exception e) {
    // Catch-all — catches any remaining exception type
    // ⚠️ Use sparingly — swallowing exceptions hides bugs
    System.err.println("Unexpected: " + e);
    throw e; // re-throw — don't just swallow
} finally {
    // Always executes — even if try returns or catch throws
    // Use for cleanup that must happen regardless
    System.out.println("This always runs");
}

// Multi-catch (Java 7+) — handle multiple exception types the same way
try {
    riskyOperation();
} catch (IOException | IllegalArgumentException e) {
    log(e);
    throw new RuntimeException("Operation failed", e); // wrap with context
}

// ── try-with-resources (Java 7+) ──────────────────────────────────────────────
// Automatically closes resources that implement AutoCloseable.
// The resource is closed even if an exception is thrown.
try (var reader = new BufferedReader(new FileReader("data.txt"))) {
    String line;
    while ((line = reader.readLine()) != null) {
        System.out.println(line);
    }
} catch (IOException e) {
    System.err.println("Read error: " + e.getMessage());
}
// reader.close() called automatically here — no finally needed

// ── Checked vs Unchecked ────────────────────────────────────────────────────────
// Checked: compiler forces you to handle or declare (IOException, SQLException)
// Unchecked (RuntimeException): no forced handling (NullPointerException, etc.)

// Declaring checked exceptions with `throws`
public String readFile(String path) throws IOException {
    return Files.readString(Path.of(path));
}

// ── Custom exceptions ──────────────────────────────────────────────────────────
// Checked custom exception
public class InsufficientFundsException extends Exception {
    private final double shortfall;

    public InsufficientFundsException(double requested, double available) {
        super(String.format("Requested %.2f but only %.2f available", requested, available));
        this.shortfall = requested - available;
    }

    public double getShortfall() { return shortfall; }
}

// Unchecked custom exception
public class InvalidConfigException extends RuntimeException {
    public InvalidConfigException(String key, String message) {
        super(String.format("Config error for '%s': %s", key, message));
    }

    // Cause chaining — preserves the original exception
    public InvalidConfigException(String key, String message, Throwable cause) {
        super(String.format("Config error for '%s': %s", key, message), cause);
    }
}
```

---

### 13. Enums

```java
// ── Basic enum ────────────────────────────────────────────────────────────────
public enum Direction { NORTH, SOUTH, EAST, WEST }

Direction d = Direction.NORTH;
d.name();    // "NORTH"
d.ordinal(); // 0 (position in declaration)

Direction.valueOf("EAST"); // Direction.EAST — parse from string

// ── Enum with fields and methods ──────────────────────────────────────────────
public enum Planet {
    MERCURY(3.303e+23, 2.4397e6),
    VENUS  (4.869e+24, 6.0518e6),
    EARTH  (5.976e+24, 6.37814e6),
    MARS   (6.421e+23, 3.3972e6);

    private final double mass;   // kg
    private final double radius; // metres

    Planet(double mass, double radius) {
        this.mass   = mass;
        this.radius = radius;
    }

    static final double G = 6.67300E-11;

    public double surfaceGravity() {
        return G * mass / (radius * radius);
    }

    public double surfaceWeight(double otherMass) {
        return otherMass * surfaceGravity();
    }
}

double earthWeight = 75.0; // kg on Earth
for (Planet p : Planet.values()) {
    System.out.printf("Weight on %s: %.2f%n", p, p.surfaceWeight(earthWeight));
}

// ── Enum in switch ────────────────────────────────────────────────────────────
Direction direction = Direction.NORTH;
String move = switch (direction) {
    case NORTH -> "Go up";
    case SOUTH -> "Go down";
    case EAST  -> "Go right";
    case WEST  -> "Go left";
};

// ── EnumSet and EnumMap ───────────────────────────────────────────────────────
import java.util.EnumSet;
import java.util.EnumMap;

// EnumSet — very efficient set for enums (backed by bit vector)
EnumSet<Direction> horizontal = EnumSet.of(Direction.EAST, Direction.WEST);
EnumSet<Direction> all        = EnumSet.allOf(Direction.class);

// EnumMap — efficient map with enum keys
EnumMap<Direction, String> labels = new EnumMap<>(Direction.class);
labels.put(Direction.NORTH, "North ↑");
labels.put(Direction.SOUTH, "South ↓");
```

---

### 14. Lambdas & Functional Interfaces

A lambda is an anonymous function — a function without a name that can be passed as an argument.

```java
import java.util.function.*;

// ── Syntax ────────────────────────────────────────────────────────────────────
// (parameters) -> expression
// (parameters) -> { statements; }

Runnable r  = () -> System.out.println("Running!");  // no params, no return
Comparator<String> cmp = (a, b) -> a.compareTo(b);   // two params

// ── Built-in Functional Interfaces ───────────────────────────────────────────
// Function<T, R> — takes T, returns R
Function<String, Integer> length = String::length;       // method reference
Function<String, Integer> parse  = Integer::parseInt;
length.apply("hello"); // 5
parse.apply("42");     // 42

// Function composition
Function<Integer, Integer> times2  = x -> x * 2;
Function<Integer, Integer> plus3   = x -> x + 3;
Function<Integer, Integer> times2ThenPlus3 = times2.andThen(plus3);
times2ThenPlus3.apply(5); // (5*2)+3 = 13

// Predicate<T> — takes T, returns boolean
Predicate<String> isLong  = s -> s.length() > 5;
Predicate<String> isEmpty = String::isEmpty;
Predicate<String> isLongAndNotEmpty = isLong.and(isEmpty.negate());
isLong.test("Hello World"); // true

// Consumer<T> — takes T, returns void
Consumer<String> print = System.out::println;
Consumer<String> log   = s -> System.err.println("[LOG] " + s);
Consumer<String> both  = print.andThen(log);
both.accept("test");

// Supplier<T> — takes nothing, returns T
Supplier<List<String>> listFactory = ArrayList::new;
List<String> newList = listFactory.get();

// BiFunction<T, U, R> — takes two inputs, returns one output
BiFunction<String, Integer, String> repeat = (s, n) -> s.repeat(n);
repeat.apply("ha", 3); // "hahaha"

// UnaryOperator<T> — Function<T, T>
UnaryOperator<String> shout  = s -> s.toUpperCase() + "!";
UnaryOperator<String> trim   = String::strip;
UnaryOperator<String> format = trim.andThen(shout);
format.apply("  hello  "); // "HELLO!"

// BinaryOperator<T> — BiFunction<T, T, T>
BinaryOperator<Integer> max = (a, b) -> a > b ? a : b;
max.apply(3, 7); // 7

// ── Method References ─────────────────────────────────────────────────────────
// Shorter lambda syntax when the lambda just calls a method:
//   ClassName::staticMethod
//   instance::instanceMethod
//   ClassName::instanceMethod  (first param becomes the receiver)
//   ClassName::new              (constructor reference)

Function<String, String> upper1 = s -> s.toUpperCase();  // lambda
Function<String, String> upper2 = String::toUpperCase;   // method reference — same

Comparator<String> cmp2 = String::compareTo;       // instance method reference
Supplier<ArrayList<String>> make = ArrayList::new; // constructor reference
```

---

### 15. Streams API

Streams provide a declarative way to process sequences of elements. They are lazy — work only happens when a terminal operation is called.

```java
import java.util.stream.*;
import java.util.List;

List<Integer> numbers = List.of(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

// ── Intermediate operations (lazy — return a new Stream) ─────────────────────
// filter  — keep elements where predicate is true
// map     — transform each element
// flatMap — like map, but flattens nested streams
// distinct — remove duplicates
// sorted  — sort elements
// limit   — take first N
// skip    — skip first N
// peek    — inspect elements (for debugging — don't use for side effects)

// ── Terminal operations (eager — trigger processing, return a result) ─────────
// forEach   — consume each element
// collect   — gather into collection
// reduce    — fold into single value
// count     — count elements
// findFirst — Optional<T> of first element
// anyMatch  — boolean: any match?
// allMatch  — boolean: all match?
// noneMatch — boolean: none match?
// min/max   — Optional<T>
// toList    — collect to immutable list (Java 16+)

// ── Examples ─────────────────────────────────────────────────────────────────
// Sum of even numbers
int sumEvens = numbers.stream()
    .filter(n -> n % 2 == 0)    // 2, 4, 6, 8, 10
    .mapToInt(Integer::intValue) // IntStream (avoids boxing overhead)
    .sum();                      // 30

// Collect to list — names of adults, sorted
record Person(String name, int age) {}
List<Person> people = List.of(
    new Person("Alice", 30), new Person("Bob", 17),
    new Person("Carol", 25), new Person("Dave", 15)
);

List<String> adults = people.stream()
    .filter(p -> p.age() >= 18)
    .sorted(Comparator.comparing(Person::name))
    .map(Person::name)
    .collect(Collectors.toList());
// ["Alice", "Carol"]

// Grouping
Map<Boolean, List<Person>> byAdult = people.stream()
    .collect(Collectors.partitioningBy(p -> p.age() >= 18));

Map<String, Long> countByFirstLetter = people.stream()
    .collect(Collectors.groupingBy(
        p -> String.valueOf(p.name().charAt(0)),
        Collectors.counting()
    ));

// flatMap — flatten nested structure
List<List<Integer>> nested = List.of(List.of(1, 2), List.of(3, 4), List.of(5));
List<Integer> flat = nested.stream()
    .flatMap(Collection::stream)
    .collect(Collectors.toList()); // [1, 2, 3, 4, 5]

// reduce — custom fold
Optional<Integer> product = numbers.stream()
    .reduce((a, b) -> a * b); // 3628800 (10!)

// String joining
String joined = people.stream()
    .map(Person::name)
    .collect(Collectors.joining(", ", "[", "]")); // "[Alice, Bob, Carol, Dave]"

// Parallel stream — for CPU-bound work on large datasets
// ⚠️ Only use when: data is large, operation is expensive, order doesn't matter
long count = numbers.parallelStream()
    .filter(n -> isPrime(n))
    .count();
```

---

### 16. Optional

`Optional<T>` is a container that may or may not hold a value. It makes nullability explicit in the type system and forces callers to handle the absent case.

```java
import java.util.Optional;

// ── Creating Optionals ────────────────────────────────────────────────────────
Optional<String> present = Optional.of("hello");   // value present (throws if null)
Optional<String> absent  = Optional.empty();        // explicitly absent
Optional<String> maybe   = Optional.ofNullable(getPossiblyNullValue()); // null → empty

// ── Consuming safely ──────────────────────────────────────────────────────────
// ❌ Anti-pattern — defeats the purpose
if (present.isPresent()) {
    System.out.println(present.get()); // can throw NoSuchElementException if wrong
}

// ✅ Preferred patterns
present.ifPresent(System.out::println);                 // only runs if present
String value = present.orElse("default");               // default if absent
String value2 = present.orElseGet(() -> computeDefault()); // lazy default
String value3 = present.orElseThrow(
    () -> new IllegalStateException("Expected a value")  // throw if absent
);

// ── Transforming ──────────────────────────────────────────────────────────────
Optional<Integer> length = present.map(String::length);   // Optional<Integer>
Optional<String>  upper  = present.map(String::toUpperCase);

// flatMap — when the mapping function itself returns Optional
Optional<String> trimmed = maybe.flatMap(s -> s.isBlank() ? Optional.empty() : Optional.of(s.trim()));

// filter — keep value only if condition is met
Optional<String> longName = present.filter(s -> s.length() > 3);

// ── Chaining ──────────────────────────────────────────────────────────────────
Optional<String> result = getUserById(id)
    .map(User::getProfile)
    .flatMap(Profile::getEmail)
    .filter(email -> email.contains("@"))
    .map(String::toLowerCase);

// ── Pitfalls ──────────────────────────────────────────────────────────────────
// Optional.get() without checking — throws NoSuchElementException if empty
// Using Optional for fields/method parameters — adds overhead without benefit
// Optional<int> — doesn't work (primitives); use OptionalInt, OptionalDouble
```

---

### 17. Records (Java 16+)

Records are immutable data classes. The compiler generates the constructor, getters, `equals()`, `hashCode()`, and `toString()` automatically.

```java
// ── Declaring a record ────────────────────────────────────────────────────────
// Everything in the header is a component: final field + getter (same name, no get prefix)
public record Point(double x, double y) {
    // Compact constructor — for validation (no need to assign, done automatically)
    public Point {
        if (Double.isNaN(x) || Double.isNaN(y)) {
            throw new IllegalArgumentException("Coordinates cannot be NaN");
        }
        // x and y are assigned automatically after this block
    }

    // Additional methods are fine
    public double distanceTo(Point other) {
        double dx = this.x - other.x;
        double dy = this.y - other.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    public Point translate(double dx, double dy) {
        return new Point(x + dx, y + dy); // return new record (immutable!)
    }
}

Point p1 = new Point(3.0, 4.0);
Point p2 = new Point(0.0, 0.0);

p1.x();               // 3.0 — accessor (not getX)
p1.distanceTo(p2);    // 5.0
p1.translate(1, 1);   // Point[x=4.0, y=5.0]

System.out.println(p1);    // Point[x=3.0, y=4.0] — automatic toString
p1.equals(new Point(3.0, 4.0)); // true — automatic equals

// Records are perfect for:
// - DTOs (Data Transfer Objects)
// - Value objects (coordinate, money amount, date range)
// - Return types carrying multiple values
// - Map keys (reliable hashCode and equals)
```

---

### 18. Concurrency Basics

```java
import java.util.concurrent.*;

// ── Thread — unit of execution ────────────────────────────────────────────────
// Runnable — a task without a return value
Runnable task = () -> System.out.println("Hello from " + Thread.currentThread().getName());
Thread t = new Thread(task, "my-thread");
t.start(); // starts the thread — runs concurrently
t.join();  // wait for the thread to finish

// ── ExecutorService — thread pool management ──────────────────────────────────
// Prefer ExecutorService over raw Thread — it manages the pool lifecycle
ExecutorService executor = Executors.newFixedThreadPool(4); // 4 worker threads

executor.submit(() -> System.out.println("Task 1")); // submit Runnable
Future<Integer> future = executor.submit(() -> {     // submit Callable (returns value)
    Thread.sleep(100);
    return 42;
});

int result = future.get(); // blocks until result is available
// future.get(5, TimeUnit.SECONDS) — with timeout

executor.shutdown(); // stop accepting new tasks
executor.awaitTermination(10, TimeUnit.SECONDS); // wait for current tasks

// ── synchronized — mutual exclusion ─────────────────────────────────────────
public class SafeCounter {
    private int count = 0;

    public synchronized void increment() { count++; } // only one thread at a time
    public synchronized int  getCount()  { return count; }

    // Or use a synchronized block for finer control:
    public void increment2() {
        synchronized (this) {
            count++;
        }
    }
}

// ── Atomic classes — lock-free thread safety for single variables ─────────────
import java.util.concurrent.atomic.*;

AtomicInteger atomicCount = new AtomicInteger(0);
atomicCount.incrementAndGet(); // atomic ++ without synchronised
atomicCount.compareAndSet(1, 2); // atomic CAS

// ── Virtual Threads (Java 21) ─────────────────────────────────────────────────
// Lightweight threads — you can have millions of them (vs thousands of OS threads)
Thread vt = Thread.ofVirtual().start(() -> System.out.println("Virtual thread!"));

// With ExecutorService:
try (var vtExecutor = Executors.newVirtualThreadPerTaskExecutor()) {
    vtExecutor.submit(() -> handleRequest());
}

// ⚠️ Concurrency pitfalls to avoid:
// - Race conditions: multiple threads reading and writing shared state unsynchronised
// - Deadlock: thread A holds lock 1 and waits for lock 2; thread B holds lock 2 and waits for lock 1
// - Memory visibility: changes by one thread may not be visible to another without synchronisation
```

---

### 19. Memory Management & GC

Java manages memory automatically through the **Garbage Collector** (GC). You do not call `free()`. The GC reclaims memory occupied by objects that are no longer reachable.

```
JVM Memory Layout
─────────────────
┌─────────────────────────────────────────────────────────┐
│  Heap (shared by all threads)                           │
│  ┌──────────────────────────────────────────────────┐   │
│  │  Young Generation         │  Old Generation      │   │
│  │  ┌─────────┬─────────────┐│                      │   │
│  │  │  Eden   │  Survivor   ││  Long-lived objects  │   │
│  │  │  (new   │  (survived  ││  (survived many GCs) │   │
│  │  │  objects)│  one GC)   ││                      │   │
│  │  └─────────┴─────────────┘│                      │   │
│  └──────────────────────────────────────────────────┘   │
│                                                         │
│  Metaspace (class metadata — outside heap in Java 8+)   │
└─────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────┐
│  Stack (one per thread)                                │
│  Each method call creates a stack frame:               │
│  - local variables (primitive values stored directly)  │
│  - references to heap objects                          │
│  - return address                                      │
└────────────────────────────────────────────────────────┘
```

```java
// Objects live on the heap. References live on the stack.
String s = new String("hello"); // object on heap, reference `s` on stack

// An object becomes eligible for GC when no references point to it
s = null; // the String "hello" is now unreachable — eligible for GC

// ── Common GC-related patterns ─────────────────────────────────────────────────
// Memory leaks in Java — objects still referenced but no longer needed
class Cache {
    private Map<String, byte[]> data = new HashMap<>();

    public void add(String key, byte[] value) {
        data.put(key, value); // keeps growing — never cleared
    }
    // ✅ Fix: use WeakHashMap, set a max size, or use an LRU cache
}

// WeakReference — does not prevent GC
WeakReference<ExpensiveObject> ref = new WeakReference<>(new ExpensiveObject());
ExpensiveObject obj = ref.get(); // returns null if GC has collected it
if (obj != null) { /* use it */ }

// ── try-with-resources prevents resource leaks ────────────────────────────────
// FileDescriptors, DB connections, sockets — these are NOT managed by GC
// They have native OS resources that must be explicitly released
try (Connection conn = dataSource.getConnection();
     PreparedStatement stmt = conn.prepareStatement("SELECT ...")) {
    // resources closed automatically in reverse order
}

// ── Tuning GC (for information — don't tune blindly) ─────────────────────────
// -Xmx512m      set max heap size to 512 MB
// -Xms256m      set initial heap size to 256 MB
// -XX:+UseG1GC  use G1 garbage collector (default since Java 9)
// -XX:+UseZGC   use ZGC (Java 15+) — very low pause times
```

---

### 20. Security Pitfalls

```java
// ── 1. SQL Injection ──────────────────────────────────────────────────────────
// ❌ DANGEROUS — user input directly in SQL string
String query = "SELECT * FROM users WHERE name = '" + userName + "'";
// If userName = "'; DROP TABLE users; --" → database is destroyed

// ✅ SAFE — PreparedStatement with parameterised queries
PreparedStatement stmt = conn.prepareStatement(
    "SELECT * FROM users WHERE name = ?"
);
stmt.setString(1, userName); // parameter, not concatenated
ResultSet rs = stmt.executeQuery();

// ── 2. Deserialisation of untrusted data ──────────────────────────────────────
// ❌ DANGEROUS — Java deserialisation of user-controlled bytes can execute arbitrary code
ObjectInputStream ois = new ObjectInputStream(userInputStream);
Object obj = ois.readObject(); // Remote Code Execution vulnerability

// ✅ SAFE — use JSON/XML with schema validation instead of Java serialisation
// If you must deserialise, use a look-ahead ObjectInputStream that whitelists classes

// ── 3. Hardcoded credentials ──────────────────────────────────────────────────
// ❌ DANGEROUS — credentials in source code end up in version control
String dbPassword = "s3cr3t!"; // visible to anyone with repo access
String apiKey     = "sk-abc123";

// ✅ SAFE — read from environment variables or a secrets manager
String dbPassword2 = System.getenv("DB_PASSWORD");
String apiKey2     = System.getenv("API_KEY");

// ── 4. Path Traversal ─────────────────────────────────────────────────────────
// ❌ DANGEROUS — user controls the file path
String filename = request.getParameter("file");
File f = new File("/app/uploads/" + filename);
// filename = "../../etc/passwd" → reads system files!

// ✅ SAFE — resolve and validate the canonical path
File base      = new File("/app/uploads/").getCanonicalFile();
File requested = new File(base, filename).getCanonicalFile();
if (!requested.getPath().startsWith(base.getPath() + File.separator)) {
    throw new SecurityException("Path traversal detected");
}

// ── 5. Logging sensitive data ─────────────────────────────────────────────────
// ❌ DANGEROUS — passwords, tokens, PII in logs
logger.debug("Login attempt: user={}, password={}", username, password);
logger.info("Payment: card={}", creditCardNumber);

// ✅ SAFE — never log credentials or PII
logger.debug("Login attempt: user={}", username);
// Log the event, not the sensitive values

// ── 6. Integer overflow ───────────────────────────────────────────────────────
int max = Integer.MAX_VALUE; // 2,147,483,647
int overflow = max + 1;      // -2,147,483,648 — silent wrapping, no exception!

// ✅ Use Math.addExact() which throws ArithmeticException on overflow
int safe = Math.addExact(max, 1); // throws ArithmeticException
// Or use long / BigInteger for values that might overflow int

// ── 7. Random number generation ───────────────────────────────────────────────
// ❌ DANGEROUS for security purposes
Random rand = new Random();
String token = String.valueOf(rand.nextInt()); // predictable!

// ✅ SAFE — use SecureRandom for security-sensitive random values
import java.security.SecureRandom;
SecureRandom secureRand = new SecureRandom();
byte[] tokenBytes = new byte[32];
secureRand.nextBytes(tokenBytes); // cryptographically random
```

---

## 🇫🇷 Français

### Points essentiels à retenir

Ce document est la référence anglaise complète. Voici les points critiques en français.

---

#### Types

- Java est **statiquement typé** : les erreurs de type sont détectées à la compilation, pas à l'exécution.
- Les **primitifs** (`int`, `double`, etc.) sont stockés par valeur. Les **objets** sont stockés par référence.
- **Autoboxing** : Java convertit automatiquement `int` ↔ `Integer`. Attention : `Integer a = 128; Integer b = 128; a == b;` est `false` — toujours utiliser `.equals()` pour comparer des objets.

#### Strings

- Les `String` sont **immuables** — toute "modification" crée un nouvel objet.
- **Toujours** utiliser `.equals()` pour comparer des chaînes, jamais `==`.
- Dans une boucle, utiliser `StringBuilder` — la concaténation `+` crée des objets intermédiaires inutiles.

#### OOP

- **`extends`** : héritage de classe (une seule classe parente).
- **`implements`** : implémentation d'interface (plusieurs possibles).
- **`@Override`** : annotation obligatoire pour confirmer qu'on surcharge bien une méthode existante.
- **`super()`** doit être le premier appel dans un constructeur de sous-classe.

#### Exceptions

- **Checked** (hérite d'`Exception` sans passer par `RuntimeException`) : le compilateur oblige à les gérer ou les déclarer avec `throws`.
- **Unchecked** (`RuntimeException` et sous-classes) : pas d'obligation.
- **`try-with-resources`** : ferme automatiquement les ressources (`Connection`, `InputStream`, etc.) — utiliser systématiquement à la place de `finally { close(); }`.

#### Streams et lambdas

- Les streams sont **lazy** : rien n'est calculé avant l'opération terminale.
- `parallelStream()` n'est pas toujours plus rapide — le coût de coordination dépasse le gain pour les petits volumes.
- `Optional` sert à rendre la nullabilité explicite dans le type. Ne jamais appeler `.get()` sans vérification — utiliser `.orElse()`, `.ifPresent()`, `.map()`.

#### Sécurité

- **SQL Injection** : toujours utiliser `PreparedStatement` avec des `?`.
- **Déserialisation** : ne jamais désérialiser des données utilisateur avec `ObjectInputStream`.
- **`SecureRandom`** et non `Random` pour tout ce qui touche à la sécurité (tokens, mots de passe).
- Ne jamais mettre d'identifiants en dur dans le code source.

---

## License

MIT — part of the [CodeSamples](https://github.com/t.orbeck/codesamples) project.
