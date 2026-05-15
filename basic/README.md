# BASIC — Beginner's All-purpose Symbolic Instruction Code

---

## 🇬🇧 English

### What is BASIC?

BASIC is the language that taught programming to a generation. Created to be accessible to everyone — not just mathematicians and scientists — it introduced millions of people to the idea that they could make a computer do what they wanted. Before the internet, before smartphones, BASIC was how ordinary people first touched software creation.

If you grew up in the 1970s, 80s, or early 90s and had a home computer, you probably typed BASIC programs from magazines, modified them, broke them, and learned from that process. That was how it was meant to work.

Today, BASIC in its classic form is largely gone from professional software. But it lives on through Visual Basic, VBA (still widely used in Excel), and as a foundational concept in computing history.

---

### A brief history

| Year | Event |
|------|-------|
| 1964 | John Kemeny and Thomas Kurtz create BASIC at Dartmouth College |
| 1975 | Bill Gates and Paul Allen write Altair BASIC — Microsoft's first product |
| 1977 | Apple II ships with Integer BASIC (then Applesoft BASIC) |
| 1981 | IBM PC ships with Microsoft BASIC (GW-BASIC) |
| 1982 | Sinclair ZX Spectrum, Commodore 64, BBC Micro — BASIC everywhere |
| 1985 | Atari ST, Amiga — more BASIC variants |
| 1991 | Microsoft Visual Basic 1.0 — RAD development for Windows |
| 1993 | Turbo Basic reborn as PowerBASIC |
| 2001 | Visual Basic .NET — a complete redesign |
| Now | VBA still runs millions of Excel macros; QB64 keeps classic BASIC alive |

> John Kemeny and Thomas Kurtz designed BASIC specifically so that non-technical students at Dartmouth could use computers. Their original goal was **democratizing computing**. They succeeded beyond anything they could have imagined.

---

### Best tools (modern BASIC)

| Tool | Purpose | Link |
|------|---------|-------|
| **QB64** | Modern QuickBASIC compatible compiler | https://www.qb64.org |
| **FreeBASIC** | Free, open source BASIC compiler | https://www.freebasic.net |
| **Visual Basic .NET** | Modern VB in the .NET ecosystem | Included in Visual Studio |
| **Excel VBA** | BASIC embedded in Microsoft Office | In Excel: Alt+F11 |
| **Small Basic** | Microsoft's beginner BASIC for kids | https://smallbasic.com |
| **DarkBASIC** | BASIC for game development | https://www.thegamecreators.com |

---

### Classic BASIC syntax

> These examples use a syntax close to GW-BASIC / QBasic / QB64. Line numbers are shown where they were historically required, though modern BASIC variants do not need them.

#### Hello, World

```basic
10 PRINT "Hello, World!"
20 END
```

Modern QB64 / FreeBASIC (no line numbers required):

```basic
PRINT "Hello, World!"
END
```

#### Variables

```basic
' Classic BASIC uses sigils for types:
' $ for strings
' % for integers
' ! for single precision
' # for double precision

LET name$ = "Alice"
LET age% = 30
LET height! = 1.75

PRINT name$
PRINT age%
PRINT height!
```

#### Input from the user

```basic
PRINT "What is your name?"
INPUT name$

PRINT "How old are you?"
INPUT age%

PRINT "Hello, "; name$; "! You are "; age%; " years old."
```

#### Conditionals

```basic
INPUT "Enter your age: ", age%

IF age% >= 18 THEN
    PRINT "You are an adult."
ELSEIF age% >= 13 THEN
    PRINT "You are a teenager."
ELSE
    PRINT "You are a child."
END IF
```

#### Loops

```basic
' FOR loop
FOR i% = 1 TO 10
    PRINT i%
NEXT i%

' FOR with STEP
FOR i% = 0 TO 100 STEP 10
    PRINT i%
NEXT i%

' WHILE loop
LET count% = 1
WHILE count% <= 5
    PRINT count%
    LET count% = count% + 1
WEND

' DO LOOP (modern BASIC)
DO
    INPUT "Enter a positive number (0 to quit): ", n%
    IF n% > 0 THEN PRINT "You entered: "; n%
LOOP WHILE n% <> 0
```

#### Subroutines and functions

```basic
' GOSUB — jump to a subroutine and return
GOSUB PrintHeader
PRINT "Main program running."
GOSUB PrintFooter
END

PrintHeader:
    PRINT "=== PROGRAM START ==="
    RETURN

PrintFooter:
    PRINT "=== PROGRAM END ==="
    RETURN
```

Modern QB64 style with proper SUBs and FUNCTIONs:

```basic
DECLARE SUB Greet(name AS STRING)
DECLARE FUNCTION Add(a AS INTEGER, b AS INTEGER) AS INTEGER

Greet "Alice"
PRINT Add(3, 4)
END

SUB Greet(name AS STRING)
    PRINT "Hello, "; name; "!"
END SUB

FUNCTION Add(a AS INTEGER, b AS INTEGER) AS INTEGER
    Add = a + b
END FUNCTION
```

#### Arrays

```basic
' Declare an array
DIM scores%(9)   ' 10 elements: index 0 to 9

' Fill it
FOR i% = 0 TO 9
    scores%(i%) = i% * 10
NEXT i%

' Print it
FOR i% = 0 TO 9
    PRINT scores%(i%)
NEXT i%
```

#### A classic BASIC program — simple number guessing game

```basic
' Number Guessing Game
RANDOMIZE TIMER   ' seed the random number generator
secret% = INT(RND * 100) + 1   ' random number 1-100

PRINT "I'm thinking of a number between 1 and 100."
PRINT "Can you guess it?"
PRINT ""

attempts% = 0

DO
    INPUT "Your guess: ", guess%
    attempts% = attempts% + 1

    IF guess% < secret% THEN
        PRINT "Too low! Try again."
    ELSEIF guess% > secret% THEN
        PRINT "Too high! Try again."
    ELSE
        PRINT "Correct! You got it in "; attempts%; " attempt(s)!"
    END IF
LOOP UNTIL guess% = secret%

END
```

---

## 🇫🇷 Français

### C'est quoi le BASIC ?

Le BASIC est le langage qui a appris la programmation à toute une génération. Conçu pour être accessible à tous — pas seulement aux mathématiciens et aux scientifiques — il a introduit des millions de personnes à l'idée qu'ils pouvaient faire faire à un ordinateur ce qu'ils voulaient. Avant internet, avant les smartphones, le BASIC était la façon dont les gens ordinaires touchaient pour la première fois à la création de logiciels.

Si tu as grandi dans les années 70, 80 ou au début des 90 avec un ordinateur à la maison, tu as probablement tapé des programmes BASIC depuis des magazines, les as modifiés, cassés, et appris de ce processus. C'est exactement comme ça que c'était censé fonctionner.

Aujourd'hui, le BASIC classique a largement disparu du développement professionnel. Mais il survit à travers Visual Basic, VBA (encore largement utilisé dans Excel), et comme concept fondateur dans l'histoire de l'informatique.

---

### Un peu d'histoire

| Année | Événement |
|-------|-----------|
| 1964 | John Kemeny et Thomas Kurtz créent le BASIC à Dartmouth College |
| 1975 | Bill Gates et Paul Allen écrivent Altair BASIC — le premier produit de Microsoft |
| 1977 | Apple II livré avec Integer BASIC |
| 1981 | IBM PC livré avec GW-BASIC |
| 1982 | ZX Spectrum, Commodore 64, BBC Micro — BASIC partout |
| 1991 | Microsoft Visual Basic 1.0 |
| Maintenant | VBA fait encore tourner des millions de macros Excel |

> Kemeny et Kurtz ont conçu le BASIC spécifiquement pour que les étudiants non techniques de Dartmouth puissent utiliser des ordinateurs. Leur objectif initial était de **démocratiser l'informatique**. Ils ont réussi au-delà de tout ce qu'ils auraient pu imaginer.

---

### Les meilleurs outils (BASIC moderne)

| Outil | Utilité | Lien |
|-------|---------|------|
| **QB64** | Compilateur QuickBASIC moderne compatible | https://www.qb64.org |
| **FreeBASIC** | Compilateur BASIC libre et open source | https://www.freebasic.net |
| **Visual Basic .NET** | VB moderne dans l'écosystème .NET | Inclus dans Visual Studio |
| **Excel VBA** | BASIC intégré dans Microsoft Office | Dans Excel : Alt+F11 |
| **Small Basic** | BASIC de Microsoft pour débutants | https://smallbasic.com |

---

### Syntaxe de base

#### Hello, World

```basic
10 PRINT "Bonjour, Monde !"
20 END
```

Version moderne QB64 :

```basic
PRINT "Bonjour, Monde !"
END
```

#### Variables

```basic
' Le BASIC classique utilise des suffixes pour les types :
' $ pour les chaînes de caractères
' % pour les entiers
' ! pour la simple précision

LET nom$ = "Alice"
LET age% = 30
LET taille! = 1.75

PRINT nom$
PRINT age%
```

#### Saisie utilisateur

```basic
PRINT "Quel est ton nom ?"
INPUT nom$

PRINT "Quel est ton âge ?"
INPUT age%

PRINT "Bonjour, "; nom$; " ! Tu as "; age%; " ans."
```

#### Conditions

```basic
INPUT "Entre ton âge : ", age%

IF age% >= 18 THEN
    PRINT "Tu es adulte."
ELSEIF age% >= 13 THEN
    PRINT "Tu es adolescent."
ELSE
    PRINT "Tu es enfant."
END IF
```

#### Boucles

```basic
' Boucle FOR
FOR i% = 1 TO 10
    PRINT i%
NEXT i%

' Boucle WHILE
LET compteur% = 1
WHILE compteur% <= 5
    PRINT compteur%
    LET compteur% = compteur% + 1
WEND
```

#### Sous-programmes et fonctions

```basic
DECLARE SUB Saluer(nom AS STRING)
DECLARE FUNCTION Additionner(a AS INTEGER, b AS INTEGER) AS INTEGER

Saluer "Alice"
PRINT Additionner(3, 4)
END

SUB Saluer(nom AS STRING)
    PRINT "Bonjour, "; nom; " !"
END SUB

FUNCTION Additionner(a AS INTEGER, b AS INTEGER) AS INTEGER
    Additionner = a + b
END FUNCTION
```

#### Un programme classique — jeu de devinette

```basic
' Jeu de devinette
RANDOMIZE TIMER
secret% = INT(RND * 100) + 1

PRINT "Je pense à un nombre entre 1 et 100."
PRINT "Peux-tu le deviner ?"
PRINT ""

tentatives% = 0

DO
    INPUT "Ton essai : ", essai%
    tentatives% = tentatives% + 1

    IF essai% < secret% THEN
        PRINT "Trop petit ! Réessaie."
    ELSEIF essai% > secret% THEN
        PRINT "Trop grand ! Réessaie."
    ELSE
        PRINT "Correct ! Tu as réussi en "; tentatives%; " essai(s) !"
    END IF
LOOP UNTIL essai% = secret%

END
```
