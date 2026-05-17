/**
 * HelloWorld.java — Java Hello World
 * CodeSamples · https://github.com/t.orbeck/codesamples
 *
 * This is not just "Hello, World!". It is a tour of Java's core idioms:
 *
 *   1.  Enums with fields and methods
 *   2.  Interfaces with default methods
 *   3.  Abstract classes and inheritance
 *   4.  Records (immutable data classes, Java 16+)
 *   5.  Generics — a typed pair, a bounded-type max function
 *   6.  Collections — ArrayList, HashMap, sorting with Comparator
 *   7.  Exception handling — checked, unchecked, custom, try-with-resources
 *   8.  Lambdas and functional interfaces
 *   9.  Streams API — filter, map, sorted, collect, reduce
 *  10.  Optional — explicit nullable values
 *  11.  Pattern matching instanceof (Java 16+)
 *  12.  String formatting and StringBuilder
 *  13.  Static factory methods
 *
 * Prerequisites: JDK 17 or later (LTS)
 * Compile: javac -d bin src/HelloWorld.java
 * Run:     java -cp bin HelloWorld
 * Or:      java src/HelloWorld.java   (single-file programs, Java 11+)
 */

import java.util.*;
import java.util.function.*;
import java.util.stream.*;

// ═══════════════════════════════════════════════════════════════════════════
// § 1 — ENUMS
//
// An enum is a class where all instances are defined at compile time.
// Enums can have fields, constructors, and methods — they are not just labels.
// ═══════════════════════════════════════════════════════════════════════════

enum Category {
    // Each constant is an instance of Category, constructed with these args
    FICTION    ("Fiction",     "🟦"),
    NON_FICTION("Non-Fiction", "🟩"),
    SCIENCE    ("Science",     "🔬"),
    HISTORY    ("History",     "📜");

    private final String displayName;
    private final String icon;

    // Enum constructor — package-private by default (cannot be public)
    Category(String displayName, String icon) {
        this.displayName = displayName;
        this.icon        = icon;
    }

    public String getDisplayName() { return displayName; }
    public String getIcon()        { return icon; }

    // Override toString so printing the enum gives a human-readable result
    @Override
    public String toString() { return icon + " " + displayName; }
}


// ═══════════════════════════════════════════════════════════════════════════
// § 2 — INTERFACE WITH DEFAULT METHOD
//
// An interface defines a contract. Any class implementing it MUST provide
// the abstract methods. Default methods provide a shared implementation
// that implementors may choose to override.
// ═══════════════════════════════════════════════════════════════════════════

interface Describable {
    /**
     * Returns a short description of the object.
     * This is an abstract method — implementing classes MUST provide it.
     */
    String describe();

    /**
     * Prints the description to stdout.
     * This is a default method — implementing classes get this for free
     * but may override it if they want different behaviour.
     */
    default void print() {
        System.out.println("  » " + describe());
    }
}

// A second interface — a class can implement multiple interfaces (unlike extends)
interface Rateable {
    // Encapsulates valid rating range as a named constant on the interface
    int MAX_RATING = 5; // implicitly public static final on interfaces

    int getRating();

    // Default method: derives a star string from the rating
    default String stars() {
        return "★".repeat(Math.max(0, Math.min(getRating(), MAX_RATING)))
             + "☆".repeat(Math.max(0, MAX_RATING - Math.min(getRating(), MAX_RATING)));
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// § 3 — ABSTRACT CLASS AND INHERITANCE
//
// An abstract class can have concrete methods (with bodies) and abstract
// methods (without bodies). Subclasses must implement all abstract methods.
//
// Use an abstract class when:
//   - You want to share code (fields, concrete methods) between related classes
//   - There is a natural "is-a" hierarchy
//
// Use an interface when:
//   - You're defining a capability ("can be described", "can be rated")
//   - Multiple unrelated classes should share the same contract
// ═══════════════════════════════════════════════════════════════════════════

abstract class LibraryItem implements Describable, Rateable {
    // protected — accessible in this class and all subclasses
    protected final String title;
    protected final String author;
    protected final int    year;
    protected final int    rating;   // 1–5

    // Abstract classes can have constructors (called via super() in subclasses)
    protected LibraryItem(String title, String author, int year, int rating) {
        this.title  = title;
        this.author = author;
        this.year   = year;
        this.rating = validateRating(rating);
    }

    // Concrete method — shared by all subclasses
    public String getTitle()  { return title; }
    public String getAuthor() { return author; }
    public int    getYear()   { return year; }

    @Override public int getRating() { return rating; }

    // Abstract method — each subclass must define what kind it is
    public abstract Category getCategory();

    // Concrete method using abstract method — this is the Template Method pattern:
    // the algorithm is defined here, the variable part is in getCategory()
    @Override
    public String describe() {
        return String.format("[%s] \"%s\" by %s (%d) %s",
                             getCategory(), title, author, year, stars());
    }

    // Static factory method — provides a named way to create instances
    // without exposing the constructor of a specific subtype
    public static LibraryItem createBook(String title, String author,
                                         int year, Category category, int rating) {
        return new Book(title, author, year, category, rating);
    }

    // Private helper — accessible only within this class
    private static int validateRating(int rating) {
        if (rating < 1 || rating > 5) {
            throw new IllegalArgumentException(
                "Rating must be between 1 and 5, got: " + rating
            );
        }
        return rating;
    }

    // toString — called when you print an object or concatenate it with a string
    @Override
    public String toString() {
        return String.format("LibraryItem[\"%s\" by %s]", title, author);
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// § 3b — CONCRETE SUBCLASS
//
// A concrete class extends an abstract class and provides implementations
// for all abstract methods. It inherits everything the abstract class has.
// ═══════════════════════════════════════════════════════════════════════════

class Book extends LibraryItem {
    private final Category category;
    private final int      pageCount;

    Book(String title, String author, int year, Category category, int rating) {
        super(title, author, year, rating); // call the abstract class constructor
        this.category  = category;
        this.pageCount = 0; // simplified — real code would take this as a parameter
    }

    // Provide the implementation of the one abstract method
    @Override
    public Category getCategory() { return category; }
}

// A different subclass — same abstract class, different behaviour
class AudioBook extends LibraryItem {
    private final int durationMinutes;

    AudioBook(String title, String author, int year, int durationMinutes, int rating) {
        super(title, author, year, rating);
        this.durationMinutes = durationMinutes;
    }

    @Override
    public Category getCategory() { return Category.NON_FICTION; } // simplified

    // Override describe() — subclasses CAN override concrete methods too
    @Override
    public String describe() {
        return super.describe() + String.format(" [Audio, %d min]", durationMinutes);
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// § 4 — RECORDS (Java 16+)
//
// A record is a special class for immutable data. The compiler generates:
//   - A canonical constructor
//   - Accessors for each component (named the same as the field, no "get" prefix)
//   - equals(), hashCode(), toString()
//
// Records cannot extend other classes (they implicitly extend java.lang.Record).
// They CAN implement interfaces.
// ═══════════════════════════════════════════════════════════════════════════

record Catalogue(String name, String libraryCode) implements Describable {
    // Compact constructor — for validation.
    // The assignments happen automatically after this block.
    public Catalogue {
        Objects.requireNonNull(name,        "name cannot be null");
        Objects.requireNonNull(libraryCode, "libraryCode cannot be null");
        if (name.isBlank())        throw new IllegalArgumentException("name cannot be blank");
        if (libraryCode.isBlank()) throw new IllegalArgumentException("code cannot be blank");
        name        = name.strip();        // can normalise before assignment
        libraryCode = libraryCode.strip().toUpperCase();
    }

    @Override
    public String describe() {
        return String.format("Catalogue: %s (code: %s)", name, libraryCode);
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// § 5 — GENERICS
//
// Generics let you write code that works with different types while
// maintaining compile-time type safety. No casting, no ClassCastException.
// ═══════════════════════════════════════════════════════════════════════════

// Generic class — T is a type parameter, resolved at each use site
class Pair<A, B> {
    private final A first;
    private final B second;

    public Pair(A first, B second) {
        this.first  = first;
        this.second = second;
    }

    public A getFirst()  { return first; }
    public B getSecond() { return second; }

    // Generic static factory — infers types from arguments
    public static <X, Y> Pair<X, Y> of(X first, Y second) {
        return new Pair<>(first, second);
    }

    @Override
    public String toString() {
        return "(" + first + ", " + second + ")";
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// § 6 — CUSTOM EXCEPTIONS
//
// Checked exception (extends Exception): caller MUST catch or declare `throws`.
// Unchecked exception (extends RuntimeException): no requirement to catch.
//
// Rule of thumb:
//   - Checked for recoverable conditions the caller can reasonably handle
//   - Unchecked for programming errors (wrong argument, invalid state)
// ═══════════════════════════════════════════════════════════════════════════

// Checked exception — callers must handle it
class ItemNotFoundException extends Exception {
    private final String itemTitle;

    public ItemNotFoundException(String itemTitle) {
        super("Item not found: \"" + itemTitle + "\"");
        this.itemTitle = itemTitle;
    }

    public String getItemTitle() { return itemTitle; }
}

// Unchecked exception — for programming errors (shouldn't need to be caught)
class DuplicateItemException extends RuntimeException {
    public DuplicateItemException(String title) {
        super("Duplicate item: \"" + title + "\" already exists in the library");
    }
}


// ═══════════════════════════════════════════════════════════════════════════
// MAIN CLASS
// ═══════════════════════════════════════════════════════════════════════════

public class HelloWorld {

    // ── The Library — uses ArrayList and HashMap (§ Collections) ─────────────
    // A simple in-memory library — not thread-safe (would need ConcurrentHashMap
    // or synchronised blocks for multi-threaded access).
    private final List<LibraryItem>        items;
    private final Map<String, LibraryItem> byTitle; // fast lookup by title

    public HelloWorld() {
        this.items   = new ArrayList<>();
        this.byTitle = new HashMap<>();
    }

    // ── Add item — throws unchecked DuplicateItemException ─────────────────────
    public void add(LibraryItem item) {
        if (byTitle.containsKey(item.getTitle())) {
            throw new DuplicateItemException(item.getTitle());
        }
        items.add(item);
        byTitle.put(item.getTitle(), item);
    }

    // ── Find item — throws checked ItemNotFoundException ────────────────────────
    // The `throws` declaration tells the compiler: this can fail in a way the
    // caller should decide how to handle.
    public LibraryItem find(String title) throws ItemNotFoundException {
        LibraryItem item = byTitle.get(title);
        if (item == null) {
            throw new ItemNotFoundException(title);
        }
        return item;
    }

    // ── Find with Optional — no exception, caller decides what absent means ────
    public Optional<LibraryItem> findOptional(String title) {
        return Optional.ofNullable(byTitle.get(title));
    }

    // ── List by category — Streams API ─────────────────────────────────────────
    public List<LibraryItem> findByCategory(Category category) {
        return items.stream()
            .filter(item -> item.getCategory() == category)
            .sorted(Comparator.comparing(LibraryItem::getTitle)) // alphabetical
            .collect(Collectors.toList());
    }

    // ── Top-rated items — Streams API ──────────────────────────────────────────
    public List<LibraryItem> topRated(int minRating) {
        return items.stream()
            .filter(item -> item.getRating() >= minRating)
            .sorted(Comparator.comparingInt(LibraryItem::getRating).reversed())
            .collect(Collectors.toList());
    }

    // ── Average rating — Streams with mapToInt ─────────────────────────────────
    public OptionalDouble averageRating() {
        return items.stream()
            .mapToInt(LibraryItem::getRating)
            .average();
    }

    // ── Items grouped by category — groupingBy collector ──────────────────────
    public Map<Category, List<LibraryItem>> groupByCategory() {
        return items.stream()
            .collect(Collectors.groupingBy(LibraryItem::getCategory));
    }

    // ── Count by category — Streams + EnumMap ──────────────────────────────────
    public Map<Category, Long> countByCategory() {
        return items.stream()
            .collect(Collectors.groupingBy(
                LibraryItem::getCategory,
                () -> new EnumMap<>(Category.class), // preserve enum order
                Collectors.counting()
            ));
    }

    // ── Generic method — bounded type parameter ─────────────────────────────────
    // T extends Comparable<T> means T must be comparable to itself.
    // This lets us call .compareTo() on T without casting.
    public static <T extends Comparable<T>> T max(T a, T b) {
        return a.compareTo(b) >= 0 ? a : b;
    }


    // ═══════════════════════════════════════════════════════════════════════════
    // ENTRY POINT
    // ═══════════════════════════════════════════════════════════════════════════

    public static void main(String[] args) {

        System.out.println("═══════════════════════════════════════════════════════");
        System.out.println("  Java Hello World — CodeSamples");
        System.out.println("  JDK version: " + System.getProperty("java.version"));
        System.out.println("═══════════════════════════════════════════════════════\n");


        // ── § 1: Enum usage ───────────────────────────────────────────────────
        printHeader("§ 1 — Enums");

        for (Category cat : Category.values()) {
            System.out.printf("  %-12s ordinal=%d%n", cat, cat.ordinal());
        }
        // Parse an enum from a string (throws IllegalArgumentException if not found)
        Category fromString = Category.valueOf("SCIENCE");
        System.out.println("  valueOf(\"SCIENCE\") → " + fromString);


        // ── § 2–3: Creating objects (abstract class + subclasses) ─────────────
        printHeader("§ 2–3 — Abstract Class, Interfaces, Inheritance");

        HelloWorld library = new HelloWorld();

        // Static factory method — named, descriptive, type-flexible
        library.add(LibraryItem.createBook(
            "Clean Code", "Robert C. Martin", 2008, Category.NON_FICTION, 5));
        library.add(LibraryItem.createBook(
            "Dune", "Frank Herbert", 1965, Category.FICTION, 5));
        library.add(LibraryItem.createBook(
            "A Brief History of Time", "Stephen Hawking", 1988, Category.SCIENCE, 4));
        library.add(LibraryItem.createBook(
            "Sapiens", "Yuval Noah Harari", 2011, Category.HISTORY, 4));
        library.add(LibraryItem.createBook(
            "The Pragmatic Programmer", "Hunt & Thomas", 1999, Category.NON_FICTION, 5));
        library.add(LibraryItem.createBook(
            "Foundation", "Isaac Asimov", 1951, Category.FICTION, 4));

        // AudioBook — different subclass, same describe() override
        library.add(new AudioBook(
            "Thinking, Fast and Slow", "Daniel Kahneman", 2011, 1140, 5));

        System.out.println("  Library contents:");
        for (LibraryItem item : library.items) {
            // Calls describe() — which calls getCategory() and stars()
            // The correct overridden version runs at runtime (polymorphism)
            item.print();
        }

        // Pattern matching instanceof (Java 16+)
        // Old way: if (item instanceof AudioBook) { AudioBook ab = (AudioBook) item; }
        System.out.println("\n  AudioBooks (pattern matching instanceof):");
        for (LibraryItem item : library.items) {
            if (item instanceof AudioBook ab) {
                // `ab` is already cast — clean, no separate cast needed
                System.out.printf("    \"%s\" — %d min audio%n",
                                  ab.getTitle(), ab.durationMinutes);
            }
        }


        // ── § 4: Records ──────────────────────────────────────────────────────
        printHeader("§ 4 — Records");

        Catalogue catalogue = new Catalogue("Main Collection", "mc-001");
        // Compiler generated: toString(), equals(), hashCode(), accessors
        System.out.println("  " + catalogue);             // toString()
        System.out.println("  name()        → " + catalogue.name());
        System.out.println("  libraryCode() → " + catalogue.libraryCode());

        // Records implement equals() by value (not reference)
        Catalogue same = new Catalogue("Main Collection", "mc-001");
        System.out.println("  catalogue.equals(same) → " + catalogue.equals(same)); // true

        catalogue.print(); // default method from Describable interface


        // ── § 5: Generics ─────────────────────────────────────────────────────
        printHeader("§ 5 — Generics");

        // Pair<A, B> — type parameters resolved at each use site
        Pair<String, Integer> nameAge  = Pair.of("Alice", 30);
        Pair<Category, Long>  catCount = Pair.of(Category.FICTION, 2L);

        System.out.println("  nameAge  → " + nameAge);    // (Alice, 30)
        System.out.println("  catCount → " + catCount);   // (🟦 Fiction, 2)

        // No casting — compiler knows the types
        String  name  = nameAge.getFirst();   // String — no cast needed
        Integer age   = nameAge.getSecond();  // Integer — no cast needed

        // Generic method — works with any Comparable type
        System.out.println("  max(3, 7)           → " + max(3, 7));
        System.out.println("  max(\"apple\", \"fig\") → " + max("apple", "fig"));
        System.out.println("  max(2.71, 3.14)     → " + max(2.71, 3.14));


        // ── § 6: Exception handling ───────────────────────────────────────────
        printHeader("§ 6 — Exception Handling");

        // Checked exception — must catch or declare throws
        System.out.println("  Finding existing item:");
        try {
            LibraryItem found = library.find("Dune");
            System.out.println("    Found: " + found.describe());
        } catch (ItemNotFoundException e) {
            System.err.println("  Error: " + e.getMessage());
        }

        System.out.println("  Finding missing item:");
        try {
            library.find("The Invisible Book");
        } catch (ItemNotFoundException e) {
            // e.getItemTitle() — custom field on our exception
            System.out.println("    Caught ItemNotFoundException for: \""
                               + e.getItemTitle() + "\"");
        }

        // Unchecked exception — optional to catch
        System.out.println("  Adding duplicate item:");
        try {
            library.add(LibraryItem.createBook("Dune", "F. Herbert", 1965, Category.FICTION, 4));
        } catch (DuplicateItemException e) {
            System.out.println("    Caught DuplicateItemException: " + e.getMessage());
        }

        // Optional — no exception, caller decides what absence means
        System.out.println("  Optional lookup (present):");
        library.findOptional("Sapiens")
            .map(item -> "    " + item.describe())
            .ifPresent(System.out::println);

        System.out.println("  Optional lookup (absent):");
        String result = library.findOptional("The Invisible Book")
            .map(LibraryItem::getTitle)
            .orElse("(not found)"); // default value when Optional is empty
        System.out.println("    Result: " + result);


        // ── § 7–8: Lambdas and Stream operations ──────────────────────────────
        printHeader("§ 7–8 — Lambdas, Streams, Functional Interfaces");

        // Lambda as a Comparator (BiFunction<T, T, int> effectively)
        Comparator<LibraryItem> byRating = (a, b) -> Integer.compare(b.getRating(), a.getRating());
        Comparator<LibraryItem> byTitle  = Comparator.comparing(LibraryItem::getTitle); // method reference

        // Stream: top-rated items
        System.out.println("  Top-rated items (≥ 4 stars), sorted by rating desc:");
        library.topRated(4).forEach(item -> System.out.println("    " + item.describe()));

        // Stream: fiction books only
        System.out.println("\n  Fiction books:");
        library.findByCategory(Category.FICTION)
               .forEach(item -> System.out.println("    " + item.describe()));

        // Stream: average rating
        library.averageRating().ifPresent(avg ->
            System.out.printf("%n  Average rating: %.2f / 5%n", avg)
        );

        // Stream: count by category
        System.out.println("\n  Count by category:");
        library.countByCategory().forEach((cat, count) ->
            System.out.printf("    %-20s %d item(s)%n", cat, count)
        );

        // Stream: reduce — build a comma-separated list of all titles
        System.out.println("\n  All titles (reduce to comma-separated):");
        String allTitles = library.items.stream()
            .map(LibraryItem::getTitle)
            .sorted()
            .reduce("", (a, b) -> a.isEmpty() ? b : a + ", " + b);
        System.out.println("    " + allTitles);

        // Chained lambdas using Function interface
        Function<String, String> trim        = String::strip;
        Function<String, String> toUpper     = String::toUpperCase;
        Function<String, String> addBrackets = s -> "[" + s + "]";
        Function<String, String> pipeline    = trim.andThen(toUpper).andThen(addBrackets);

        System.out.println("\n  Function pipeline (trim → upper → brackets):");
        System.out.println("    \"  clean code  \" → " + pipeline.apply("  clean code  "));


        // ── § 9: StringBuilder ────────────────────────────────────────────────
        printHeader("§ 9 — StringBuilder");

        // String concatenation in a loop creates many intermediate String objects.
        // StringBuilder accumulates characters in a mutable buffer — far more efficient.
        StringBuilder sb = new StringBuilder();
        sb.append("Library: ");
        for (int i = 0; i < library.items.size(); i++) {
            if (i > 0) sb.append(", ");
            sb.append('"').append(library.items.get(i).getTitle()).append('"');
        }
        System.out.println("  " + sb.toString());


        // ── § 10: String formatting ───────────────────────────────────────────
        printHeader("§ 10 — String Formatting");

        // String.format — printf-style, returns a String
        String summary = String.format(
            "Library \"%s\" has %d items with average rating %.1f",
            catalogue.name(),
            library.items.size(),
            library.averageRating().orElse(0.0)
        );
        System.out.println("  " + summary);

        // Formatted text block (Java 15+)
        String report = """
                ┌─────────────────────────────────────┐
                │  Library Report                     │
                │  Items:   %-4d                      │
                │  Catalogue: %-24s│
                └─────────────────────────────────────┘
                """.formatted(library.items.size(), catalogue.name());
        System.out.println(report);


        // ── § 11: switch expression ───────────────────────────────────────────
        printHeader("§ 11 — Switch Expression (Java 14+)");

        for (Category cat : Category.values()) {
            String description = switch (cat) {
                case FICTION     -> "Stories that never happened — but feel real";
                case NON_FICTION -> "Stories that did happen — or should have";
                case SCIENCE     -> "How the universe actually works";
                case HISTORY     -> "How we got here";
            };
            System.out.printf("  %-20s %s%n", cat, description);
        }


        System.out.println("\n═══════════════════════════════════════════════════════");
        System.out.println("  Done. Read the source — the code is the tutorial.");
        System.out.println("═══════════════════════════════════════════════════════");
    }


    // ── Utility: print a section header ──────────────────────────────────────
    private static void printHeader(String title) {
        System.out.println();
        System.out.println("  ── " + title + " " + "─".repeat(Math.max(0, 50 - title.length())));
    }
}
