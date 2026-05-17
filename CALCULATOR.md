# The Calculator Project

> *"A calculator is the 'Hello, World!' of real applications — small enough to finish,
> complex enough to reveal how a language actually thinks."*

---

## 🇬🇧 English

### The idea

Every language in this repository will implement **the same calculator**.

Not a toy — a real desktop calculator with memory functions, operation history, and
proper decimal handling. The kind of calculator you'd find on Windows or macOS.
Simple enough to complete in an afternoon. Complex enough that every language will
approach it differently.

The goal is **comparison**. Once you have the same application written in C, C++, Java,
Python, C#, and JavaScript, you can read them side by side and ask real questions:

- How does memory management work in C vs Java?
- How does Python's dynamic typing change the architecture vs Java's static typing?
- What does a WinForms calculator look like vs a GTK calculator — same logic, different world?
- How much ceremony does each language require for the same result?
- Which language makes the business logic clearest?
- Which makes the UI code most readable?

These are the questions that make you a better developer — not "which language is best"
(there is no answer), but "what does this language make *easy* and what does it make *hard*?"

---

### Why a calculator?

A calculator is the ideal comparison vehicle because it hits every fundamental:

| Requirement | What it tests |
|---|---|
| State management | How does the language handle mutable state? |
| Input parsing | String-to-number conversion, decimal validation |
| Floating-point arithmetic | Every language handles this differently |
| Business logic separation | Can you isolate the engine from the UI? |
| Error handling | Division by zero, overflow, invalid input |
| History/log | Collections, data structures |
| Memory functions | State that persists across operations |
| Event-driven UI | How does the language wire buttons to logic? |

It is complex enough to be meaningful. It is simple enough that you can read the entire
source of any version in under an hour. That is exactly what you want for comparison.

---

### Feature specification

Every calculator in this project — regardless of language or platform — implements the
same feature set. This is the **canonical specification**. If it's not in this list,
it's optional.

#### Core arithmetic
- **Addition** `+`
- **Subtraction** `−`
- **Multiplication** `×`
- **Division** `÷`
- **Equals** `=` — evaluate the current expression
- **Decimal point** `.` — enter decimal numbers
- **Positive/Negative toggle** `±` — negate the current number

#### Correction
- **Clear Entry** `CE` — clear the current input only (keep the running operation)
- **Clear All** `C` — reset the calculator completely
- **Backspace** `⌫` — delete the last entered digit

#### Advanced
- **Percentage** `%` — convert current value to percentage (÷ 100)
- **Square root** `√` — square root of current value
- **Reciprocal** `1/x` — reciprocal of current value

#### Memory functions
- **Memory Store** `MS` — save current value to memory
- **Memory Recall** `MR` — recall value from memory
- **Memory Add** `M+` — add current value to memory
- **Memory Subtract** `M−` — subtract current value from memory
- **Memory Clear** `MC` — clear memory
- A visual indicator that memory contains a value

#### Operation history
- A scrollable log showing previous calculations
- Format: `expression = result`
- Ability to clear the history

#### Error handling
- Division by zero → display error message, do not crash
- Square root of a negative number → display error message
- Overflow → display error message
- Invalid input → gracefully ignored or flagged

#### Precision
- Display up to 12 significant digits
- Use the language's native 64-bit floating-point type (`double` in Java/C/C++/C#,
  `float` in Python, `Number` in JavaScript)
- Acknowledge floating-point limitations in comments (see the note below)

#### The floating-point note (important)

Every implementation must include a comment explaining this:

```
0.1 + 0.2 ≠ 0.3

In every language that uses IEEE 754 double-precision floating-point (which is all of
them in this project), 0.1 + 0.2 = 0.30000000000000004.

This is not a bug in the calculator. It is not a bug in the language.
It is a fundamental consequence of representing decimal fractions in binary.
A real-world calculator application rounds the display to a reasonable number of
significant digits to hide this from the user. Our implementations do the same.
```

Understanding this is not optional. It has caused real financial bugs in production code.

---

### Platform matrix

The same calculator engine runs everywhere. The UI layer changes. The business logic
should not.

| Platform | JavaScript | Java | Python | C# | C | C++ |
|---|---|---|---|---|---|---|
| **Console** | ✅ Node.js | ✅ | ✅ | ✅ .NET | ✅ | ✅ |
| **Web (browser)** | ✅ HTML+CSS+JS | — | — | — | — | — |
| **Windows GUI** | ✅ Electron | ✅ Swing / JavaFX | ✅ Tkinter | ✅ WPF | ✅ Win32 API | ✅ Qt |
| **Linux GUI** | ✅ Electron | ✅ Swing / JavaFX | ✅ Tkinter | ✅ .NET + GTK | ✅ GTK3 | ✅ Qt |
| **macOS GUI** | ✅ Electron | ✅ Swing / JavaFX | ✅ Tkinter | ✅ .NET MAUI | — | ✅ Qt |
| **Android** | — | ✅ Native Android | — | ✅ .NET MAUI | — | — |
| **iOS** | — | — | — | ✅ .NET MAUI | — | — |

**Legend:**
- ✅ = implemented or planned in this project
- — = not a natural target for this language in this context (or not planned)

**Notes:**
- JavaScript on Android/iOS via React Native or Capacitor is possible but out of scope here — we prioritise what each language does *natively*.
- C on Linux GUI via GTK is planned. C on Windows GUI via Win32 API demonstrates the raw Windows API — educational but verbose by design.
- C++ via Qt covers Windows, Linux, and macOS from the same codebase — Qt's cross-platform GUI is one of its strongest selling points.
- Java via Swing covers all desktop platforms. JavaFX is the modern replacement. Android is covered separately.
- Python via Tkinter is built into the standard library — zero additional dependencies.
- C# via WPF is Windows-only. C# via .NET MAUI covers Windows, macOS, Android, and iOS from the same codebase.

---

### Architecture requirement: separation of concerns

Every implementation must separate the **calculator engine** from the **user interface**.

```
┌──────────────────────────────────────────────────────────────────┐
│  Calculator Engine (platform-agnostic)                           │
│                                                                  │
│  - State: current value, operand, operator, memory, history      │
│  - Methods: pressDigit(), pressOperator(), pressEquals(),        │
│             pressMemoryStore(), pressRecall(), pressClear()...   │
│  - Returns: display string, history list, memory indicator       │
│  - Has NO knowledge of buttons, windows, or screen coordinates   │
└──────────────────────────────────────────────────────────────────┘
         ▲                              ▲
         │ calls                        │ calls
┌────────┴──────────┐       ┌──────────┴──────────┐
│  Console UI       │       │  Graphical UI        │
│                   │       │                      │
│  Reads keyboard   │       │  Renders buttons     │
│  Prints output    │       │  Handles click events│
│  Runs REPL loop   │       │  Redraws display     │
└───────────────────┘       └──────────────────────┘
```

This separation is not just good practice — it is the entire point. When you can run
the same engine under a console UI and a graphical UI, you have proven that the business
logic is genuinely decoupled from the presentation layer.

Every version that does NOT do this separation is a teaching moment — you can see exactly
what makes tightly coupled code harder to test, extend, and port.

---

### Project naming convention

Calculator projects live inside each language's folder, organised by platform:

```
<language>/
└── calculator/
    ├── console/        ← command-line REPL (all platforms)
    ├── web/            ← browser (JavaScript only)
    ├── windows/        ← native Windows GUI
    ├── linux/          ← native Linux GUI
    ├── android/        ← Android app (Java, C# MAUI)
    └── cross-platform/ ← single codebase, multiple OS (Qt, Electron, MAUI)
```

Not every language has every folder. The matrix above shows what exists.

Each calculator subfolder is a self-contained project:
- Its own `README.md` explaining how to build and run
- Its own build configuration (tasks.json, Makefile, .csproj, etc.)
- Source code with the engine clearly separated from the UI
- Comments explaining platform-specific decisions

---

### What to look for when comparing

When you read two implementations side by side, these are the questions worth asking:

#### On the engine
- How is state represented? A class? A struct? A set of global variables? Closures?
- How does the language handle the absence of a value (no current operand)?
  `null`? `Optional`? A sentinel value? An algebraic type?
- How verbose is the error handling? Does the language make it easy or hard to be explicit?
- How many lines of code does the engine take?

#### On the UI
- How does button wiring work in this toolkit? Event listeners? Delegates? Signals/slots?
- How does the toolkit handle layout? Absolute positions? Constraint-based? Grid?
- How much boilerplate is required before you write a single line of business logic?
- What does the toolkit give you for free (keyboard shortcuts, accessibility, theming)?

#### On floating-point
- Where does rounding happen? In the engine or in the display layer?
- How does each language format numbers? `printf`? `String.format`? `toFixed`?
- Try entering `0.1 + 0.2 =` in each version. What do you see?

#### On the architecture
- Is the engine genuinely decoupled? Can you swap the UI without touching the engine?
- How does the console version differ from the GUI version? (They should share an engine)
- Where did the developer make shortcuts? Why? Was it necessary?

---

### The floating-point problem — a real story

In 1994, the **Intel Pentium FDIV bug** was discovered by professor Thomas Nicely.
The Pentium's floating-point unit returned incorrect results for certain division
operations. The error was small — typically in the 9th significant digit — but it
was systematic and reproducible.

Intel initially claimed the bug only affected "theoretical mathematicians."
Nicely calculated that for the average spreadsheet user, the bug would produce a
wrong answer once every 27,000 years. Intel's position: this is not a real problem.

IBM ran its own tests and concluded the bug would produce wrong results in financial
calculations roughly once every 24 days. IBM stopped shipping Pentiums.

The public backlash was severe. Intel replaced all affected chips at a cost of
approximately **$475 million**. The lesson: floating-point errors that seem negligible
in theory can be catastrophic in practice. Know what your numbers are doing.

Our calculator is not a safety-critical financial system. But the comments about
floating-point in every implementation are there to remind you that they could be.

---

### Contributing a calculator

To add a new calculator implementation:

1. Create the folder structure: `<language>/calculator/<platform>/`
2. Implement the full feature specification above — all items, no shortcuts
3. Separate the engine from the UI
4. Include the floating-point comment
5. Write a `README.md` explaining prerequisites, build steps, and what platform APIs are used
6. Open a pull request — see [CODE_OF_CONDUCT.md](./CODE_OF_CONDUCT.md)

The most valuable contributions are the ones that are most different from what already
exists. A GTK calculator in C and a Win32 calculator in C, sitting next to each other,
teach more than any tutorial ever could.

---

## 🇫🇷 Français

### L'idée

Chaque langage du dépôt va implémenter **la même calculatrice**.

Pas un jouet — une vraie calculatrice de bureau avec fonctions mémoire, historique des opérations et gestion correcte des décimales. Le genre qu'on trouve sous Windows ou macOS. Assez simple pour être terminée en une après-midi. Assez complexe pour que chaque langage l'aborde différemment.

Le but : **la comparaison**. Une fois qu'on a la même application écrite en C, C++, Java, Python, C# et JavaScript, on peut les lire côte à côte et poser des vraies questions :

- Comment la gestion mémoire change-t-elle entre C et Java ?
- En quoi le typage dynamique de Python modifie-t-il l'architecture par rapport au typage statique de Java ?
- À quoi ressemble une calculatrice WinForms vs une calculatrice GTK — même logique, mondes différents ?
- Quelle quantité de cérémonial chaque langage exige-t-il pour le même résultat ?

Ce sont ces questions qui font de meilleurs développeurs — pas "quel est le meilleur langage" (il n'y a pas de réponse), mais "qu'est-ce que ce langage rend *facile* et qu'est-ce qu'il rend *difficile* ?"

---

### Spécification fonctionnelle

Toute calculatrice de ce projet — quel que soit le langage ou la plateforme — implémente le même ensemble de fonctionnalités :

#### Arithmétique de base
- Addition `+`, soustraction `−`, multiplication `×`, division `÷`
- Égal `=`, virgule décimale `.`, bascule signe `±`

#### Correction
- **Effacement entrée** `CE` — efface la saisie en cours uniquement
- **Effacement total** `C` — réinitialisation complète
- **Retour arrière** `⌫` — supprime le dernier chiffre saisi

#### Fonctions avancées
- Pourcentage `%`, racine carrée `√`, inverse `1/x`

#### Mémoire
- `MS` stocker, `MR` rappeler, `M+` ajouter, `M−` soustraire, `MC` effacer
- Indicateur visuel que la mémoire contient une valeur

#### Historique
- Journal défilant des calculs précédents (`expression = résultat`)
- Possibilité d'effacer l'historique

#### Gestion des erreurs
- Division par zéro → message d'erreur, pas de crash
- Racine carrée d'un nombre négatif → message d'erreur
- Dépassement de capacité → message d'erreur

---

### Architecture exigée : séparation des responsabilités

Toute implémentation doit séparer le **moteur de calcul** de l'**interface utilisateur**.

```
┌─────────────────────────────────────────────────────────┐
│  Moteur (aucune connaissance de l'interface)            │
│  État, opérations, mémoire, historique, erreurs         │
└─────────────────────────────────────────────────────────┘
         ▲                            ▲
┌────────┴───────┐         ┌──────────┴──────────┐
│  UI console    │         │  UI graphique        │
│  (REPL texte)  │         │  (boutons, fenêtre)  │
└────────────────┘         └─────────────────────┘
```

Le moteur doit pouvoir tourner sous une UI console et une UI graphique **sans modification**. C'est la preuve que la logique métier est réellement découplée de la couche de présentation.

---

### Matrice des plateformes

| Plateforme | JavaScript | Java | Python | C# | C | C++ |
|---|---|---|---|---|---|---|
| **Console** | ✅ Node.js | ✅ | ✅ | ✅ .NET | ✅ | ✅ |
| **Web (navigateur)** | ✅ HTML+CSS+JS | — | — | — | — | — |
| **Windows GUI** | ✅ Electron | ✅ Swing / JavaFX | ✅ Tkinter | ✅ WPF | ✅ Win32 API | ✅ Qt |
| **Linux GUI** | ✅ Electron | ✅ Swing / JavaFX | ✅ Tkinter | ✅ .NET + GTK | ✅ GTK3 | ✅ Qt |
| **macOS GUI** | ✅ Electron | ✅ Swing / JavaFX | ✅ Tkinter | ✅ .NET MAUI | — | ✅ Qt |
| **Android** | — | ✅ Android natif | — | ✅ .NET MAUI | — | — |
| **iOS** | — | — | — | ✅ .NET MAUI | — | — |

---

### Convention de nommage des projets

```
<langage>/
└── calculator/
    ├── console/        ← REPL en ligne de commande (toutes plateformes)
    ├── web/            ← navigateur (JavaScript uniquement)
    ├── windows/        ← GUI Windows natif
    ├── linux/          ← GUI Linux natif
    ├── android/        ← application Android (Java, C# MAUI)
    └── cross-platform/ ← une base de code, plusieurs OS (Qt, Electron, MAUI)
```

Chaque sous-dossier est un projet autonome avec son propre `README.md`.

---

### Le problème des nombres à virgule flottante — une vraie histoire

En 1994, le **bug FDIV du Pentium d'Intel** fut découvert par le professeur Thomas Nicely. L'unité virgule flottante du Pentium retournait des résultats incorrects pour certaines divisions. L'erreur apparaissait généralement au 9ème chiffre significatif.

Intel affirma initialement que le bug n'affectait que "les mathématiciens théoriques". IBM mena ses propres tests et conclut que le bug produirait des résultats erronés dans des calculs financiers environ une fois toutes les 24 heures. IBM cessa de livrer des Pentiums.

Le tollé fut immense. Intel remplaça toutes les puces défectueuses pour un coût d'environ **475 millions de dollars**.

La leçon : les erreurs en virgule flottante qui semblent négligeables en théorie peuvent être catastrophiques en pratique. **Notre calculatrice n'est pas un système financier critique. Mais les commentaires sur la virgule flottante dans chaque implémentation sont là pour te rappeler qu'elle pourrait l'être.**

---

### Contribuer une calculatrice

1. Créer la structure de dossiers : `<langage>/calculator/<plateforme>/`
2. Implémenter la spécification complète — toutes les fonctionnalités, sans raccourcis
3. Séparer le moteur de l'interface
4. Inclure le commentaire sur la virgule flottante
5. Écrire un `README.md` expliquant les prérequis, la compilation et les APIs de plateforme utilisées
6. Ouvrir une pull request — voir [CODE_OF_CONDUCT.md](./CODE_OF_CONDUCT.md)

Les contributions les plus précieuses sont celles qui sont les plus différentes de ce qui existe déjà. Une calculatrice GTK en C et une calculatrice Win32 en C, côte à côte, enseignent plus que n'importe quel tutoriel.
