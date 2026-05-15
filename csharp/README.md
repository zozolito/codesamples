# C# — C Sharp

---

## 🇬🇧 English

### What is C#?

C# (pronounced "C sharp") is a modern, strongly typed, object-oriented language developed by Microsoft. It runs on the .NET platform and is the primary language for Windows application development, enterprise software, game development (Unity), and web APIs via ASP.NET Core.

C# strikes a balance between productivity and performance. It has the safety of a garbage-collected language, the expressiveness of modern functional features, and the raw capability to write high-performance code when needed. It is a language that rewards learning properly.

---

### A brief history

| Year | Event |
|------|-------|
| 1999 | Anders Hejlsberg starts designing C# at Microsoft |
| 2002 | C# 1.0 ships with .NET Framework 1.0 |
| 2005 | C# 2.0 — generics, nullable types, anonymous methods |
| 2007 | C# 3.0 — LINQ, lambda expressions, anonymous types. Game changer. |
| 2012 | C# 5.0 — `async`/`await`. Async programming becomes actually usable. |
| 2016 | .NET Core 1.0 — C# goes cross-platform (Windows, Linux, macOS) |
| 2020 | .NET 5 — unification of .NET Framework and .NET Core |
| Now | C# 12/13 on .NET 8/9 — primary constructors, collection expressions, and more |

> Anders Hejlsberg also designed Turbo Pascal and led the development of Delphi before joining Microsoft. He is one of the most influential language designers in history.

---

### Best tools

| Tool | Purpose | Link |
|------|---------|-------|
| **Visual Studio** | The full IDE for .NET, unmatched for C# | https://visualstudio.microsoft.com |
| **VS Code** + C# Dev Kit | Lightweight alternative | https://code.visualstudio.com |
| **JetBrains Rider** | Cross-platform .NET IDE, excellent refactoring | https://www.jetbrains.com/rider/ |
| **.NET SDK** | Compiler, runtime, CLI tools | https://dotnet.microsoft.com |
| **NuGet** | Package manager for .NET | Integrated in the SDK |
| **LINQPad** | Interactive C# scratchpad — invaluable for learning | https://www.linqpad.net |
| **dotnet-watch** | Auto-reload on file changes | Built into .NET SDK |

---

### Basic syntax

#### Variables and types

```csharp
// C# is statically typed — types are known at compile time
string name = "Alice";
int age = 30;
double height = 1.75;
bool isActive = true;
decimal price = 9.99m;  // m suffix for decimal literals

// var — type inferred by compiler (not dynamic, still strongly typed)
var greeting = "Hello";  // compiler knows this is a string
var count = 42;          // compiler knows this is an int

// Nullable types — explicitly allow null
string? maybeNull = null;  // can be null
int? maybeInt = null;       // nullable int
```

#### String interpolation

```csharp
string name = "Alice";
int age = 30;

// String interpolation — the modern way
string message = $"Hello, {name}! You are {age} years old.";

// Verbatim strings — no escape sequences
string path = @"C:\Users\Alice\Documents";

// Raw string literals (C# 11+)
string json = """
    {
        "name": "Alice",
        "age": 30
    }
    """;
```

#### Conditionals

```csharp
int age = 20;

if (age >= 18)
{
    Console.WriteLine("Adult");
}
else if (age >= 13)
{
    Console.WriteLine("Teenager");
}
else
{
    Console.WriteLine("Child");
}

// Ternary
string label = age >= 18 ? "Adult" : "Minor";

// Switch expression (C# 8+) — very expressive
string category = age switch
{
    >= 18 => "Adult",
    >= 13 => "Teenager",
    _     => "Child"   // _ is the default case
};
```

#### Loops

```csharp
// for loop
for (int i = 0; i < 5; i++)
{
    Console.WriteLine(i);
}

// foreach — preferred for collections
var fruits = new[] { "apple", "banana", "cherry" };
foreach (var fruit in fruits)
{
    Console.WriteLine(fruit);
}

// while
int count = 0;
while (count < 5)
{
    Console.WriteLine(count);
    count++;
}
```

#### Collections

```csharp
// Array — fixed size
int[] numbers = { 1, 2, 3, 4, 5 };

// List<T> — dynamic size
var fruits = new List<string> { "apple", "banana", "cherry" };
fruits.Add("mango");
fruits.Remove("apple");

// Dictionary<TKey, TValue>
var user = new Dictionary<string, object>
{
    ["name"] = "Alice",
    ["age"] = 30
};

// Access with safety
if (user.TryGetValue("email", out var email))
{
    Console.WriteLine(email);
}
```

#### Methods (Functions)

```csharp
// Basic method
public static string Greet(string name)
{
    return $"Hello, {name}!";
}

// Expression-bodied method (shorter syntax)
public static string Greet(string name) => $"Hello, {name}!";

// Optional parameters
public static string Greet(string name = "stranger") => $"Hello, {name}!";

// Out parameters — return multiple values
public static bool TryParse(string input, out int result)
{
    return int.TryParse(input, out result);
}
```

#### Classes

```csharp
// Modern C# class with primary constructor (C# 12)
public class Person(string name, int age)
{
    public string Name { get; } = name;
    public int Age { get; } = age;

    public string Greet() => $"Hi, I'm {Name} and I'm {Age} years old.";

    public override string ToString() => $"Person({Name}, {Age})";
}

// Inheritance
public class Employee(string name, int age, string company)
    : Person(name, age)
{
    public string Company { get; } = company;

    public override string ToString() => $"Employee({Name} at {Company})";
}

// Usage
var person = new Person("Alice", 30);
Console.WriteLine(person.Greet());
```

#### LINQ — Language Integrated Query

```csharp
// LINQ is one of C#'s superpowers — query collections like SQL
var numbers = new[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

// Method syntax (preferred)
var evenDoubled = numbers
    .Where(n => n % 2 == 0)   // filter
    .Select(n => n * 2)        // transform
    .ToList();                 // materialize

// Useful LINQ methods
var sum = numbers.Sum();                     // 55
var average = numbers.Average();             // 5.5
var max = numbers.Max();                     // 10
var first = numbers.First(n => n > 5);      // 6
var any = numbers.Any(n => n > 8);          // true
var all = numbers.All(n => n > 0);          // true
var ordered = numbers.OrderByDescending(n => n).ToList();
```

#### Async / Await

```csharp
using System.Net.Http;
using System.Text.Json;

// Async method — always returns Task or Task<T>
public async Task<string> GetDataAsync(string url)
{
    using var client = new HttpClient();

    try
    {
        var response = await client.GetAsync(url);
        response.EnsureSuccessStatusCode();     // throws if not 2xx
        return await response.Content.ReadAsStringAsync();
    }
    catch (HttpRequestException ex)
    {
        Console.WriteLine($"Request failed: {ex.Message}");
        throw;
    }
}
```

#### Error handling

```csharp
try
{
    int result = Divide(10, 0);
}
catch (DivideByZeroException ex)
{
    Console.WriteLine($"Math error: {ex.Message}");
}
catch (Exception ex)
{
    Console.WriteLine($"Unexpected error: {ex.Message}");
    throw;  // rethrow — don't silently swallow exceptions
}
finally
{
    Console.WriteLine("Cleanup always runs here");
}

// Custom exception
public class ValidationException(string message) : Exception(message);
```

#### Records — immutable data types

```csharp
// Records are perfect for data transfer objects
// Equality is value-based, not reference-based
public record Point(double X, double Y);

var p1 = new Point(1.0, 2.0);
var p2 = new Point(1.0, 2.0);
Console.WriteLine(p1 == p2);  // true — value equality

// Non-destructive mutation
var p3 = p1 with { Y = 5.0 };  // new Point(1.0, 5.0)
```

---

## 🇫🇷 Français

### C'est quoi C# ?

C# (prononcé "C sharp") est un langage moderne, fortement typé et orienté objet développé par Microsoft. Il tourne sur la plateforme .NET et est le langage principal pour le développement d'applications Windows, les logiciels d'entreprise, le développement de jeux vidéo (Unity) et les APIs web via ASP.NET Core.

C# trouve un équilibre entre productivité et performance. Il a la sécurité d'un langage avec ramasse-miettes, l'expressivité de fonctionnalités fonctionnelles modernes, et la capacité brute d'écrire du code performant quand nécessaire.

---

### Un peu d'histoire

| Année | Événement |
|-------|-----------|
| 1999 | Anders Hejlsberg commence à concevoir C# chez Microsoft |
| 2002 | C# 1.0 sort avec .NET Framework 1.0 |
| 2005 | C# 2.0 — génériques, types nullables, méthodes anonymes |
| 2007 | C# 3.0 — LINQ, expressions lambda. Un tournant majeur. |
| 2012 | C# 5.0 — `async`/`await`. La programmation async devient enfin utilisable. |
| 2016 | .NET Core 1.0 — C# devient cross-plateforme |
| 2020 | .NET 5 — unification de .NET Framework et .NET Core |
| Maintenant | C# 12/13 sur .NET 8/9 |

---

### Les meilleurs outils

| Outil | Utilité | Lien |
|-------|---------|------|
| **Visual Studio** | L'IDE complet pour .NET | https://visualstudio.microsoft.com |
| **VS Code** + C# Dev Kit | Alternative légère | https://code.visualstudio.com |
| **JetBrains Rider** | IDE .NET cross-plateforme | https://www.jetbrains.com/rider/ |
| **.NET SDK** | Compilateur, runtime, outils CLI | https://dotnet.microsoft.com |
| **LINQPad** | Bac à sable interactif C# — inestimable pour apprendre | https://www.linqpad.net |

---

### Syntaxe de base

#### Variables et types

```csharp
string nom = "Alice";
int age = 30;
double taille = 1.75;
bool estActif = true;
decimal prix = 9.99m;

// var — type inféré par le compilateur (toujours fortement typé)
var salutation = "Bonjour";

// Types nullables
string? peutEtreNull = null;
```

#### Interpolation de chaînes

```csharp
string nom = "Alice";
int age = 30;

string message = $"Bonjour, {nom} ! Tu as {age} ans.";
string chemin = @"C:\Utilisateurs\Alice\Documents";
```

#### Conditions

```csharp
int age = 20;

if (age >= 18) { Console.WriteLine("Adulte"); }
else if (age >= 13) { Console.WriteLine("Adolescent"); }
else { Console.WriteLine("Enfant"); }

// Switch expression (C# 8+)
string categorie = age switch
{
    >= 18 => "Adulte",
    >= 13 => "Adolescent",
    _     => "Enfant"
};
```

#### Collections

```csharp
// Liste dynamique
var fruits = new List<string> { "pomme", "banane", "cerise" };
fruits.Add("mangue");

// Dictionnaire
var utilisateur = new Dictionary<string, object>
{
    ["nom"] = "Alice",
    ["age"] = 30
};
```

#### Classes

```csharp
public class Personne(string nom, int age)
{
    public string Nom { get; } = nom;
    public int Age { get; } = age;

    public string Saluer() => $"Bonjour, je suis {Nom} et j'ai {Age} ans.";
}
```

#### LINQ

```csharp
var nombres = new[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

var pairsDoubles = nombres
    .Where(n => n % 2 == 0)
    .Select(n => n * 2)
    .ToList();

var somme = nombres.Sum();       // 55
var max = nombres.Max();         // 10
var tousPositifs = nombres.All(n => n > 0); // true
```

#### Async / Await

```csharp
public async Task<string> RecupererDonneesAsync(string url)
{
    using var client = new HttpClient();
    try
    {
        var response = await client.GetAsync(url);
        response.EnsureSuccessStatusCode();
        return await response.Content.ReadAsStringAsync();
    }
    catch (HttpRequestException ex)
    {
        Console.WriteLine($"Requête échouée : {ex.Message}");
        throw;
    }
}
```

#### Records — types de données immuables

```csharp
// Égalité basée sur les valeurs, parfait pour les DTOs
public record Point(double X, double Y);

var p1 = new Point(1.0, 2.0);
var p2 = new Point(1.0, 2.0);
Console.WriteLine(p1 == p2);  // true

// Mutation non destructive
var p3 = p1 with { Y = 5.0 };
```
