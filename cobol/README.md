# COBOL — Common Business-Oriented Language

---

## 🇬🇧 English

### What is COBOL?

COBOL is the language that runs the world's money. Not metaphorically — literally. An estimated **$3 trillion in daily commerce** flows through COBOL systems. The US Social Security Administration runs COBOL. The IRS runs COBOL. Most major banks run COBOL. Your ATM almost certainly talks to a COBOL backend. When the COVID-19 pandemic hit and unemployment systems collapsed under load, several US states issued emergency calls for COBOL developers because their systems — written in the 1970s — were still the backbone of their operations.

COBOL was designed for business: processing records, calculating payroll, managing transactions, producing reports. It was not designed to be elegant. It was designed to be readable by non-programmers, auditable by accountants, and to handle decimal arithmetic exactly — something most modern languages still struggle with.

The banking system does not need saving. It needs maintenance. And right now, there are not enough people who can do it.

---

### A brief history

| Year | Event |
|------|-------|
| 1959 | Grace Hopper and the CODASYL committee design COBOL — built to run on any machine |
| 1960 | First COBOL compilers run on UNIVAC II and RCA 501 — proving portability works |
| 1968 | COBOL-68 — first ANSI standard |
| 1974 | COBOL-74 |
| 1985 | COBOL-85 — structured programming features |
| 1997 | Gartner estimates there are **more lines of COBOL in production than any other language** |
| 2002 | COBOL 2002 — object-oriented features (rarely used in practice) |
| 2014 | COBOL 2014 |
| 2020 | New Jersey, Kansas, and other US states put out emergency job postings for COBOL developers during COVID-19 pandemic |
| Now | An estimated **800 billion lines of COBOL** run in production worldwide. That number is not shrinking. |

> Grace Hopper, who led the development of COBOL, was also the person who coined the term **"debugging"** — after removing an actual moth from a relay in a Harvard Mark II computer in 1947. She was a US Navy Rear Admiral, a computer science pioneer, and one of the most consequential technologists of the 20th century. She received the Presidential Medal of Freedom posthumously in 2016.

---

### Why should you care in 2024?

- **Job security**: There are hundreds of thousands of COBOL jobs worldwide. The developers who wrote these systems are retiring. The skills are not being replaced.
- **Compensation**: COBOL developers command very high salaries precisely because the supply is shrinking while the systems remain critical.
- **Understanding legacy systems**: If you work in finance, insurance, or government IT, you will encounter COBOL. Not knowing it is a liability.
- **Respect for history**: COBOL solved real business problems at scale before most programming concepts we take for granted existed.

---

### Best tools

| Tool | Purpose | Link |
|------|---------|-------|
| **GnuCOBOL (OpenCOBOL)** | Free, open source COBOL compiler — start here | https://gnucobol.sourceforge.io |
| **IBM Z Open Editor** | VS Code extension for COBOL on IBM Z | https://marketplace.visualstudio.com/items?itemName=broadcomMFD.cobol-language-support |
| **Micro Focus Visual COBOL** | Industry-standard enterprise COBOL | https://www.microfocus.com/en-us/products/visual-cobol |
| **IBM Enterprise COBOL** | The standard on IBM mainframes (z/OS) | https://www.ibm.com/products/cobol-compiler-zos |
| **COBOL for VS Code** | Broadcom's extension for VS Code | https://marketplace.visualstudio.com/items?itemName=broadcomMFD.cobol-language-support |
| **Hercules** | IBM mainframe emulator — run z/OS locally | http://www.hercules-390.eu |
| **IBM Z Learning** | Free mainframe learning resources from IBM | https://www.ibm.com/training/mainframe |

---

### Understanding COBOL's structure

COBOL programs are divided into four **DIVISIONS**, in order:

```
IDENTIFICATION DIVISION   — Who wrote this, what is it, when
ENVIRONMENT DIVISION      — The machine it runs on, files it uses
DATA DIVISION             — All variables and file layouts
PROCEDURE DIVISION        — The actual program logic
```

This rigid structure was designed so that an auditor — not a programmer — could read and understand what a program does. That idea was radical in 1959. It is still useful today.

---

### Basic syntax

#### Hello, World

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. HELLO-WORLD.

       PROCEDURE DIVISION.
           DISPLAY "Hello, World!"
           STOP RUN.
```

> Notice the column formatting. Classic COBOL used fixed columns:
> - Columns 1-6: line numbers (optional)
> - Column 7: indicator (`*` for comment, `-` for continuation)
> - Columns 8-11: Area A (DIVISION, SECTION, paragraph headers)
> - Columns 12-72: Area B (statements)
>
> Modern COBOL compilers support free-format source (`>>SOURCE FORMAT FREE`), but you will see fixed-format code in legacy systems.

#### Variables — the DATA DIVISION

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. DATA-TYPES.

       DATA DIVISION.
       WORKING-STORAGE SECTION.

      *> PIC (PICTURE) describes the shape of the data
      *> 9 = numeric digit
      *> A = alphabetic character
      *> X = alphanumeric character
      *> V = implied decimal point
      *> S = sign (positive/negative)

           01 WS-NAME            PIC X(50)       VALUE "Alice".
           01 WS-AGE             PIC 99          VALUE 30.
           01 WS-SALARY          PIC 9(7)V99     VALUE 0.
           01 WS-IS-ACTIVE       PIC X           VALUE "Y".
           01 WS-COUNTER         PIC 9(4)        VALUE ZEROS.

      *> Group item — like a struct
           01 WS-ADDRESS.
               05 WS-STREET      PIC X(50).
               05 WS-CITY        PIC X(30).
               05 WS-POSTCODE    PIC X(10).

      *> Edited picture — for display formatting
           01 WS-FORMATTED-AMOUNT  PIC $,$$$,$$9.99.

       PROCEDURE DIVISION.
           MOVE "123 Main Street" TO WS-STREET
           MOVE "Paris"           TO WS-CITY
           MOVE "75001"           TO WS-POSTCODE

           DISPLAY "Name    : " WS-NAME
           DISPLAY "Age     : " WS-AGE
           DISPLAY "Street  : " WS-STREET
           DISPLAY "City    : " WS-CITY

           STOP RUN.
```

#### Arithmetic

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. ARITHMETIC.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
           01 WS-A      PIC 9(5) VALUE 100.
           01 WS-B      PIC 9(5) VALUE 25.
           01 WS-RESULT PIC 9(7) VALUE ZERO.
           01 WS-REM    PIC 9(5) VALUE ZERO.

       PROCEDURE DIVISION.
           ADD WS-A TO WS-B GIVING WS-RESULT
           DISPLAY "100 + 25 = " WS-RESULT         *> 125

           SUBTRACT WS-B FROM WS-A GIVING WS-RESULT
           DISPLAY "100 - 25 = " WS-RESULT          *> 75

           MULTIPLY WS-A BY WS-B GIVING WS-RESULT
           DISPLAY "100 * 25 = " WS-RESULT          *> 2500

           DIVIDE WS-A BY WS-B GIVING WS-RESULT REMAINDER WS-REM
           DISPLAY "100 / 25 = " WS-RESULT          *> 4
           DISPLAY "Remainder: " WS-REM              *> 0

      *> COMPUTE — like a normal expression
           COMPUTE WS-RESULT = (WS-A + WS-B) * 2
           DISPLAY "Compute: " WS-RESULT             *> 250

           STOP RUN.
```

#### Conditionals

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. CONDITIONALS.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
           01 WS-AGE     PIC 99 VALUE 20.
           01 WS-SCORE   PIC 9(3) VALUE 75.

       PROCEDURE DIVISION.
      *> Basic IF
           IF WS-AGE >= 18
               DISPLAY "Adult"
           ELSE IF WS-AGE >= 13
               DISPLAY "Teenager"
           ELSE
               DISPLAY "Child"
           END-IF

      *> EVALUATE — like switch/case
           EVALUATE WS-SCORE
               WHEN 90 THRU 100
                   DISPLAY "Grade: A"
               WHEN 80 THRU 89
                   DISPLAY "Grade: B"
               WHEN 70 THRU 79
                   DISPLAY "Grade: C"
               WHEN 60 THRU 69
                   DISPLAY "Grade: D"
               WHEN OTHER
                   DISPLAY "Grade: F"
           END-EVALUATE

      *> 88 level — condition names (very COBOL)
           STOP RUN.
```

#### 88-level condition names — a COBOL superpower

```cobol
       DATA DIVISION.
       WORKING-STORAGE SECTION.
           01 WS-STATUS        PIC X.
               88 ACTIVE       VALUE "A".
               88 INACTIVE     VALUE "I".
               88 PENDING      VALUE "P".

           01 WS-GRADE         PIC 9(3).
               88 PASSING      VALUE 60 THRU 100.
               88 FAILING      VALUE 0  THRU 59.
               88 EXCELLENT    VALUE 90 THRU 100.

       PROCEDURE DIVISION.
           MOVE "A" TO WS-STATUS
           IF ACTIVE
               DISPLAY "Account is active"
           END-IF

           MOVE 85 TO WS-GRADE
           IF PASSING
               DISPLAY "Student passed"
           END-IF
           IF EXCELLENT
               DISPLAY "Excellent result!"
           END-IF

           STOP RUN.
```

#### Loops

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. LOOPS.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
           01 WS-COUNTER   PIC 9(3) VALUE 1.
           01 WS-TOTAL     PIC 9(6) VALUE ZERO.
           01 WS-INDEX     PIC 9(3).

       PROCEDURE DIVISION.
      *> PERFORM UNTIL — like a while loop
           PERFORM UNTIL WS-COUNTER > 10
               ADD WS-COUNTER TO WS-TOTAL
               ADD 1 TO WS-COUNTER
           END-PERFORM
           DISPLAY "Sum 1 to 10: " WS-TOTAL        *> 55

      *> PERFORM VARYING — like a for loop
           PERFORM VARYING WS-INDEX FROM 1 BY 1
               UNTIL WS-INDEX > 5
               DISPLAY "Index: " WS-INDEX
           END-PERFORM

      *> PERFORM n TIMES
           PERFORM 3 TIMES
               DISPLAY "Repeating..."
           END-PERFORM

           STOP RUN.
```

#### Paragraphs and PERFORM

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. PARAGRAPHS.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
           01 WS-NAME   PIC X(30).
           01 WS-AMOUNT PIC 9(7)V99.

       PROCEDURE DIVISION.
           PERFORM INITIALIZE-DATA
           PERFORM PROCESS-DATA
           PERFORM DISPLAY-RESULTS
           STOP RUN.

       INITIALIZE-DATA.
           MOVE "Alice"  TO WS-NAME
           MOVE 1250.75  TO WS-AMOUNT.

       PROCESS-DATA.
      *> Simulate applying a 10% bonus
           COMPUTE WS-AMOUNT = WS-AMOUNT * 1.1.

       DISPLAY-RESULTS.
           DISPLAY "Employee : " WS-NAME
           DISPLAY "Amount   : " WS-AMOUNT.
```

#### File handling — what COBOL was made for

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. FILE-EXAMPLE.

       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
           SELECT EMPLOYEE-FILE
               ASSIGN TO "employees.dat"
               ORGANIZATION IS LINE SEQUENTIAL.

       DATA DIVISION.
       FILE SECTION.
           FD EMPLOYEE-FILE.
           01 EMPLOYEE-RECORD.
               05 EMP-ID         PIC 9(6).
               05 EMP-NAME       PIC X(30).
               05 EMP-SALARY     PIC 9(7)V99.

       WORKING-STORAGE SECTION.
           01 WS-EOF             PIC X VALUE "N".
               88 END-OF-FILE    VALUE "Y".
           01 WS-COUNT           PIC 9(4) VALUE ZERO.
           01 WS-TOTAL-SALARY    PIC 9(10)V99 VALUE ZERO.

       PROCEDURE DIVISION.
           OPEN INPUT EMPLOYEE-FILE

           PERFORM UNTIL END-OF-FILE
               READ EMPLOYEE-FILE
                   AT END MOVE "Y" TO WS-EOF
                   NOT AT END
                       ADD 1              TO WS-COUNT
                       ADD EMP-SALARY     TO WS-TOTAL-SALARY
                       DISPLAY EMP-ID " | " EMP-NAME " | " EMP-SALARY
               END-READ
           END-PERFORM

           CLOSE EMPLOYEE-FILE

           DISPLAY "Records processed : " WS-COUNT
           DISPLAY "Total payroll     : " WS-TOTAL-SALARY

           STOP RUN.
```

#### A real-world example — interest calculation

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. INTEREST-CALC.
       AUTHOR. Community Sample.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
           01 WS-PRINCIPAL     PIC 9(9)V99  VALUE 10000.00.
           01 WS-RATE          PIC V9(6)    VALUE 0.035.
           01 WS-YEARS         PIC 99       VALUE 5.
           01 WS-INTEREST      PIC 9(9)V99  VALUE ZERO.
           01 WS-FINAL-AMOUNT  PIC 9(11)V99 VALUE ZERO.
           01 WS-YEAR-COUNTER  PIC 99       VALUE 1.

      *> Display formatting
           01 WS-DISPLAY-AMT   PIC $$$,$$$,$$9.99.

       PROCEDURE DIVISION.
           DISPLAY "=== COMPOUND INTEREST CALCULATOR ==="
           DISPLAY " "
           DISPLAY "Principal : $10,000.00"
           DISPLAY "Rate      : 3.5% per year"
           DISPLAY "Period    : 5 years"
           DISPLAY " "

           MOVE WS-PRINCIPAL TO WS-FINAL-AMOUNT

           PERFORM VARYING WS-YEAR-COUNTER FROM 1 BY 1
               UNTIL WS-YEAR-COUNTER > WS-YEARS

               COMPUTE WS-INTEREST =
                   WS-FINAL-AMOUNT * WS-RATE

               ADD WS-INTEREST TO WS-FINAL-AMOUNT

               MOVE WS-FINAL-AMOUNT TO WS-DISPLAY-AMT
               DISPLAY "Year " WS-YEAR-COUNTER " : " WS-DISPLAY-AMT

           END-PERFORM

           DISPLAY " "
           MOVE WS-FINAL-AMOUNT TO WS-DISPLAY-AMT
           DISPLAY "Final amount: " WS-DISPLAY-AMT

           STOP RUN.
```

---

## 🇫🇷 Français

### C'est quoi le COBOL ?

Le COBOL est le langage qui fait tourner l'argent du monde. Pas métaphoriquement — littéralement. On estime que **3 000 milliards de dollars de transactions quotidiennes** passent par des systèmes COBOL. La Sécurité Sociale américaine tourne sous COBOL. L'IRS (le fisc américain) tourne sous COBOL. La plupart des grandes banques tournent sous COBOL. Ton distributeur automatique de billets parle presque certainement à un backend COBOL. Quand la pandémie de COVID-19 a frappé et que les systèmes d'allocation chômage se sont effondrés, plusieurs États américains ont lancé des appels d'urgence pour des développeurs COBOL parce que leurs systèmes — écrits dans les années 70 — étaient toujours la colonne vertébrale de leurs opérations.

Le COBOL a été conçu pour le business : traiter des enregistrements, calculer des paies, gérer des transactions, produire des rapports. Il n'a pas été conçu pour être élégant. Il a été conçu pour être lisible par des non-programmeurs, auditable par des comptables, et pour gérer l'arithmétique décimale avec exactitude — quelque chose avec lequel la plupart des langages modernes luttent encore.

Le système bancaire n'a pas besoin d'être sauvé. Il a besoin de maintenance. Et en ce moment, il n'y a pas assez de personnes capables de la faire.

---

### Un peu d'histoire

| Année | Événement |
|-------|-----------|
| 1959 | Grace Hopper et le comité CODASYL conçoivent le COBOL |
| 1960 | Premiers compilateurs COBOL sur UNIVAC II et RCA 501 |
| 1968 | COBOL-68 — première norme ANSI |
| 1997 | Gartner estime qu'il y a **plus de lignes de COBOL en production que dans tout autre langage** |
| 2002 | COBOL 2002 — fonctionnalités orientées objet |
| 2020 | Plusieurs États américains lancent des appels d'urgence pour des développeurs COBOL pendant le COVID-19 |
| Aujourd'hui | Environ **800 milliards de lignes de COBOL** tournent en production dans le monde. Ce chiffre ne diminue pas. |

> Grace Hopper, qui a piloté le développement du COBOL, est aussi la personne qui a inventé le terme **"débogage"** — après avoir retiré une vraie mite d'un relais d'un ordinateur Harvard Mark II en 1947. Elle était Contre-Amiral de la Marine américaine, pionnière de l'informatique, et l'une des technologues les plus importantes du XXe siècle.

---

### Pourquoi s'y intéresser aujourd'hui ?

- **Sécurité de l'emploi** : Des centaines de milliers de postes COBOL dans le monde. Les développeurs qui ont écrit ces systèmes prennent leur retraite. Les compétences ne sont pas remplacées.
- **Rémunération** : Les développeurs COBOL commandent des salaires très élevés précisément parce que l'offre se réduit pendant que les systèmes restent critiques.
- **Compréhension des systèmes legacy** : Si tu travailles en finance, assurance ou informatique gouvernementale, tu vas rencontrer du COBOL.
- **Respect de l'histoire** : Le COBOL a résolu de vrais problèmes business à grande échelle avant que la plupart des concepts de programmation que nous tenons pour acquis n'existent.

---

### Les meilleurs outils

| Outil | Utilité | Lien |
|-------|---------|------|
| **GnuCOBOL** | Compilateur COBOL libre et gratuit — commence ici | https://gnucobol.sourceforge.io |
| **Micro Focus Visual COBOL** | COBOL d'entreprise standard | https://www.microfocus.com |
| **IBM Enterprise COBOL** | Le standard sur mainframes IBM (z/OS) | https://www.ibm.com |
| **IBM Z Learning** | Ressources gratuites d'apprentissage mainframe | https://www.ibm.com/training/mainframe |
| **Hercules** | Émulateur mainframe IBM — faire tourner z/OS en local | http://www.hercules-390.eu |

---

### Syntaxe de base

#### Hello, World

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. BONJOUR-MONDE.

       PROCEDURE DIVISION.
           DISPLAY "Bonjour, Monde !"
           STOP RUN.
```

#### Variables — la DATA DIVISION

```cobol
       DATA DIVISION.
       WORKING-STORAGE SECTION.

      *> PIC décrit la forme de la donnée
      *> 9 = chiffre numérique
      *> X = alphanumérique
      *> V = point décimal implicite
      *> S = signe

           01 WS-NOM             PIC X(50)   VALUE "Alice".
           01 WS-AGE             PIC 99      VALUE 30.
           01 WS-SALAIRE         PIC 9(7)V99 VALUE 0.

      *> Groupe — comme une structure
           01 WS-ADRESSE.
               05 WS-RUE         PIC X(50).
               05 WS-VILLE       PIC X(30).
               05 WS-CODE-POSTAL PIC X(10).
```

#### Niveaux 88 — la force secrète du COBOL

```cobol
           01 WS-STATUT         PIC X.
               88 ACTIF         VALUE "A".
               88 INACTIF       VALUE "I".
               88 EN-ATTENTE    VALUE "P".

           01 WS-NOTE           PIC 9(3).
               88 REUSSI        VALUE 60 THRU 100.
               88 ECHEC         VALUE 0  THRU 59.
               88 EXCELLENT     VALUE 90 THRU 100.

       PROCEDURE DIVISION.
           MOVE "A" TO WS-STATUT
           IF ACTIF
               DISPLAY "Compte actif"
           END-IF

           MOVE 85 TO WS-NOTE
           IF REUSSI  DISPLAY "Réussi" END-IF
           IF EXCELLENT DISPLAY "Excellent !" END-IF
```

#### Calcul d'intérêts composés — un exemple concret

```cobol
       IDENTIFICATION DIVISION.
       PROGRAM-ID. CALCUL-INTERETS.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
           01 WS-CAPITAL         PIC 9(9)V99  VALUE 10000.00.
           01 WS-TAUX            PIC V9(6)    VALUE 0.035.
           01 WS-ANNEES          PIC 99       VALUE 5.
           01 WS-INTERETS        PIC 9(9)V99  VALUE ZERO.
           01 WS-MONTANT-FINAL   PIC 9(11)V99 VALUE ZERO.
           01 WS-COMPTEUR        PIC 99       VALUE 1.
           01 WS-AFFICHAGE       PIC $$$,$$$,$$9.99.

       PROCEDURE DIVISION.
           DISPLAY "=== CALCUL D'INTÉRÊTS COMPOSÉS ==="
           DISPLAY " "

           MOVE WS-CAPITAL TO WS-MONTANT-FINAL

           PERFORM VARYING WS-COMPTEUR FROM 1 BY 1
               UNTIL WS-COMPTEUR > WS-ANNEES

               COMPUTE WS-INTERETS =
                   WS-MONTANT-FINAL * WS-TAUX

               ADD WS-INTERETS TO WS-MONTANT-FINAL

               MOVE WS-MONTANT-FINAL TO WS-AFFICHAGE
               DISPLAY "Année " WS-COMPTEUR " : " WS-AFFICHAGE

           END-PERFORM

           DISPLAY " "
           MOVE WS-MONTANT-FINAL TO WS-AFFICHAGE
           DISPLAY "Montant final : " WS-AFFICHAGE

           STOP RUN.
```
