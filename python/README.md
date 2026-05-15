# Python

---

## 🇬🇧 English

### What is Python?

Python is a general-purpose, high-level programming language designed for readability and simplicity. Its philosophy is that code is read far more often than it is written — so it should read almost like English. It is one of the best first languages to learn, and one of the most powerful languages in professional use.

Python is used in web development, data science, machine learning, automation, scripting, scientific computing, and more. It is the language of NumPy, TensorFlow, Django, Flask, Pandas, and countless tools that run large parts of the modern world.

---

### A brief history

| Year | Event |
|------|-------|
| 1989 | Guido van Rossum starts working on Python during Christmas holidays |
| 1991 | Python 0.9.0 released — exceptions, functions, modules already present |
| 1994 | Python 1.0 — `lambda`, `map`, `filter`, `reduce` added |
| 2000 | Python 2.0 — list comprehensions, garbage collection |
| 2008 | Python 3.0 — a clean break, not fully backward compatible. The right decision. |
| 2020 | Python 2 officially reaches end of life. If you're still on Python 2, stop. |
| Now | Python consistently ranks in the top 3 most used languages in the world |

> Guido van Rossum was the "Benevolent Dictator For Life" (BDFL) of Python until 2018, when he stepped down. The language is now governed by the Python Steering Council.

---

### Best tools

| Tool | Purpose | Link |
|------|---------|-------|
| **VS Code** + Python extension | Excellent editor for Python | https://code.visualstudio.com |
| **PyCharm** | Full IDE dedicated to Python | https://www.jetbrains.com/pycharm/ |
| **pip** | Package installer (comes with Python) | Built-in |
| **venv** | Virtual environments — isolate project dependencies | Built-in |
| **Black** | Opinionated code formatter — no debates | https://black.readthedocs.io |
| **Ruff** | Extremely fast linter | https://docs.astral.sh/ruff/ |
| **Jupyter Notebook** | Interactive notebooks, great for data/learning | https://jupyter.org |
| **pyenv** | Manage multiple Python versions | https://github.com/pyenv/pyenv |

---

### Basic syntax

#### Variables and types

```python
# Python is dynamically typed — no need to declare types
name = "Alice"
age = 30
height = 1.75
is_active = True
nothing = None

# Type hints (Python 3.5+) — optional but recommended for clarity
username: str = "alice"
score: int = 0
ratio: float = 0.95
enabled: bool = True
```

#### Strings

```python
name = "Alice"

# f-strings — the modern way (Python 3.6+)
greeting = f"Hello, {name}!"
calculation = f"2 + 2 = {2 + 2}"

# Multiline strings
message = """
This is a
multiline string.
"""

# Useful string methods
text = "  Hello, World!  "
text.strip()        # "Hello, World!" — removes whitespace
text.lower()        # "  hello, world!  "
text.upper()        # "  HELLO, WORLD!  "
text.replace("World", "Python")  # "  Hello, Python!  "
text.split(",")     # ["  Hello", " World!  "]
```

#### Conditionals

```python
age = 20

if age >= 18:
    print("Adult")
elif age >= 13:
    print("Teenager")
else:
    print("Child")

# One-liner (ternary equivalent)
label = "Adult" if age >= 18 else "Minor"

# Truthiness — Python has sensible defaults
# These are all falsy: None, 0, "", [], {}, set()
if not name:
    print("Name is empty")
```

#### Loops

```python
# for loop — iterate over a sequence
fruits = ["apple", "banana", "cherry"]
for fruit in fruits:
    print(fruit)

# range — generate numbers
for i in range(5):       # 0, 1, 2, 3, 4
    print(i)

for i in range(1, 6):   # 1, 2, 3, 4, 5
    print(i)

# enumerate — get index AND value
for index, fruit in enumerate(fruits):
    print(f"{index}: {fruit}")

# while loop
count = 0
while count < 5:
    print(count)
    count += 1
```

#### Functions

```python
# Basic function
def greet(name):
    return f"Hello, {name}!"

# Default parameters
def greet(name="stranger"):
    return f"Hello, {name}!"

# Type hints
def add(a: int, b: int) -> int:
    return a + b

# *args — variable positional arguments
def sum_all(*numbers):
    return sum(numbers)

sum_all(1, 2, 3, 4)  # 10

# **kwargs — variable keyword arguments
def describe(**info):
    for key, value in info.items():
        print(f"{key}: {value}")

describe(name="Alice", age=30, city="Paris")
```

#### Lists

```python
numbers = [1, 2, 3, 4, 5]

# List comprehensions — concise and readable
doubled = [n * 2 for n in numbers]           # [2, 4, 6, 8, 10]
evens = [n for n in numbers if n % 2 == 0]   # [2, 4]

# Slicing
numbers[0]    # 1 (first)
numbers[-1]   # 5 (last)
numbers[1:3]  # [2, 3] (index 1 to 2)
numbers[::-1] # [5, 4, 3, 2, 1] (reversed)

# Common operations
numbers.append(6)       # add to end
numbers.insert(0, 0)    # insert at index
numbers.remove(3)       # remove first occurrence of value
numbers.pop()           # remove and return last element
len(numbers)            # length
sorted(numbers)         # returns a new sorted list
numbers.sort()          # sorts in place
```

#### Dictionaries

```python
user = {
    "name": "Alice",
    "age": 30,
    "city": "Paris"
}

# Access
user["name"]            # "Alice"
user.get("email")       # None (safe, no KeyError)
user.get("email", "N/A") # "N/A" (with default)

# Modify
user["email"] = "alice@example.com"
del user["city"]

# Iterate
for key, value in user.items():
    print(f"{key}: {value}")

# Dict comprehension
squared = {n: n**2 for n in range(5)}  # {0:0, 1:1, 2:4, 3:9, 4:16}
```

#### Classes

```python
class Animal:
    def __init__(self, name: str, sound: str):
        self.name = name
        self.sound = sound

    def speak(self) -> str:
        return f"{self.name} says {self.sound}"

    def __repr__(self) -> str:
        return f"Animal(name={self.name!r})"


class Dog(Animal):
    def __init__(self, name: str):
        super().__init__(name, "Woof")

    def fetch(self) -> str:
        return f"{self.name} fetches the ball!"


dog = Dog("Rex")
print(dog.speak())   # Rex says Woof
print(dog.fetch())   # Rex fetches the ball!
```

#### Error handling

```python
# Basic try/except
try:
    result = 10 / 0
except ZeroDivisionError:
    print("Cannot divide by zero")

# Multiple exceptions
try:
    value = int("not a number")
except (ValueError, TypeError) as e:
    print(f"Conversion error: {e}")
finally:
    print("This always runs")

# Raise your own exceptions
def divide(a, b):
    if b == 0:
        raise ValueError("Divisor cannot be zero")
    return a / b
```

#### File handling

```python
# Always use context manager (with) — it closes the file automatically
with open("data.txt", "r", encoding="utf-8") as f:
    content = f.read()

# Write
with open("output.txt", "w", encoding="utf-8") as f:
    f.write("Hello, file!")

# Read line by line (memory efficient for large files)
with open("data.txt", "r", encoding="utf-8") as f:
    for line in f:
        print(line.strip())
```

#### A common security pitfall

```python
# DANGEROUS — never use eval() on user input
user_input = input("Enter expression: ")
result = eval(user_input)  # user can run ANY Python code

# SAFE — validate and parse input explicitly
try:
    value = int(user_input)
except ValueError:
    print("Please enter a valid number")
```

---

## 🇫🇷 Français

### C'est quoi Python ?

Python est un langage de programmation généraliste de haut niveau conçu pour la lisibilité et la simplicité. Sa philosophie est que le code est lu bien plus souvent qu'il n'est écrit — il devrait donc se lire presque comme de l'anglais. C'est l'un des meilleurs premiers langages à apprendre, et l'un des plus puissants en usage professionnel.

Python est utilisé en développement web, science des données, machine learning, automatisation, scripting, calcul scientifique, et bien plus. C'est le langage de NumPy, TensorFlow, Django, Flask, Pandas, et d'innombrables outils qui font tourner une grande partie du monde moderne.

---

### Un peu d'histoire

| Année | Événement |
|-------|-----------|
| 1989 | Guido van Rossum commence à travailler sur Python pendant les vacances de Noël |
| 1991 | Python 0.9.0 — exceptions, fonctions, modules déjà présents |
| 1994 | Python 1.0 — `lambda`, `map`, `filter`, `reduce` ajoutés |
| 2000 | Python 2.0 — compréhensions de listes, ramasse-miettes |
| 2008 | Python 3.0 — une rupture propre, pas entièrement rétrocompatible. La bonne décision. |
| 2020 | Python 2 atteint officiellement sa fin de vie. Si tu es encore sur Python 2, arrête. |
| Maintenant | Python se classe régulièrement dans le top 3 des langages les plus utilisés au monde |

---

### Les meilleurs outils

| Outil | Utilité | Lien |
|-------|---------|------|
| **VS Code** + extension Python | Excellent éditeur pour Python | https://code.visualstudio.com |
| **PyCharm** | IDE complet dédié à Python | https://www.jetbrains.com/pycharm/ |
| **pip** | Installateur de paquets (fourni avec Python) | Intégré |
| **venv** | Environnements virtuels — isole les dépendances | Intégré |
| **Black** | Formateur de code sans débat | https://black.readthedocs.io |
| **Ruff** | Linter extrêmement rapide | https://docs.astral.sh/ruff/ |
| **Jupyter Notebook** | Notebooks interactifs, idéal pour les données | https://jupyter.org |

---

### Syntaxe de base

#### Variables et types

```python
# Python est typé dynamiquement — pas besoin de déclarer les types
nom = "Alice"
age = 30
taille = 1.75
est_actif = True
rien = None

# Annotations de type (Python 3.5+) — optionnel mais recommandé
nom: str = "alice"
score: int = 0
ratio: float = 0.95
```

#### Chaînes de caractères

```python
nom = "Alice"

# f-strings — la façon moderne (Python 3.6+)
salutation = f"Bonjour, {nom} !"
calcul = f"2 + 2 = {2 + 2}"

# Méthodes utiles
texte = "  Bonjour le Monde !  "
texte.strip()               # "Bonjour le Monde !"
texte.lower()               # "  bonjour le monde !  "
texte.replace("Monde", "Python")  # "  Bonjour Python !  "
texte.split(" ")            # liste des mots
```

#### Conditions

```python
age = 20

if age >= 18:
    print("Adulte")
elif age >= 13:
    print("Adolescent")
else:
    print("Enfant")

# Version courte
label = "Adulte" if age >= 18 else "Mineur"
```

#### Boucles

```python
fruits = ["pomme", "banane", "cerise"]
for fruit in fruits:
    print(fruit)

# range
for i in range(5):   # 0, 1, 2, 3, 4
    print(i)

# enumerate — index ET valeur
for index, fruit in enumerate(fruits):
    print(f"{index}: {fruit}")
```

#### Fonctions

```python
def saluer(nom: str = "inconnu") -> str:
    return f"Bonjour, {nom} !"

# Arguments variables
def somme_tout(*nombres):
    return sum(nombres)

# Arguments nommés variables
def decrire(**infos):
    for cle, valeur in infos.items():
        print(f"{cle}: {valeur}")
```

#### Listes

```python
nombres = [1, 2, 3, 4, 5]

# Compréhensions de listes
doubles = [n * 2 for n in nombres]
pairs = [n for n in nombres if n % 2 == 0]

# Slicing
nombres[0]    # 1 (premier)
nombres[-1]   # 5 (dernier)
nombres[1:3]  # [2, 3]
nombres[::-1] # [5, 4, 3, 2, 1] (inversé)
```

#### Dictionnaires

```python
utilisateur = {
    "nom": "Alice",
    "age": 30,
    "ville": "Paris"
}

# Accès sécurisé
utilisateur.get("email", "N/A")  # "N/A" si clé absente

# Itérer
for cle, valeur in utilisateur.items():
    print(f"{cle}: {valeur}")
```

#### Gestion des erreurs

```python
try:
    resultat = 10 / 0
except ZeroDivisionError:
    print("Division par zéro impossible")
finally:
    print("Ceci s'exécute toujours")
```

#### Fichiers

```python
# Toujours utiliser le gestionnaire de contexte (with)
with open("donnees.txt", "r", encoding="utf-8") as f:
    contenu = f.read()

with open("sortie.txt", "w", encoding="utf-8") as f:
    f.write("Bonjour fichier !")
```
